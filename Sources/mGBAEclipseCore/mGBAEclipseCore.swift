import EclipseKit
import Foundation.NSURL
import mGBA

let audioChannelCount: UInt8 = 2;
let audioSampleRate: Double = 32768;
let sampleCount = 1024;

private extension URL {
	func filePath() -> String {
		if #available(iOS 16.0, *) {
			self.path(percentEncoded: true)
		} else {
			self.path
		}
	}
}

private func VFileOpen(path: URL, flags: Int32) -> UnsafeMutablePointer<VFile>? {
	VFileOpen(path.filePath(), flags)
}

public struct mGBAEclipseCoreSettings: CoreSettings {
	public static var descriptor: CoreSettingsDescriptor<mGBAEclipseCoreSettings> = .init(
		version: 0,
		sections: []
	)

	public init() {}

	public static func migrate(_ data: Data, from oldVersion: UInt) -> mGBAEclipseCoreSettings {
		.init()
	}
}

private final class BridgeWrapper: CoreBridgeProtocol {
	public var bridge: any CoreBridgeProtocol

	init(bridge: consuming any CoreBridgeProtocol) {
		self.bridge = bridge
	}

	@inlinable
	func writeAudioSamples(samples: UnsafeRawBufferPointer) -> Int {
		self.bridge.writeAudioSamples(samples: samples)
	}
	
	@inlinable
	func didSave() {
		self.bridge.didSave()
	}
}

func handleSaveCallback(context: UnsafeMutableRawPointer?) {
	guard let context else { return }
	let wrapper = Unmanaged<BridgeWrapper>.fromOpaque(context)
	wrapper.takeUnretainedValue().didSave()
}

public struct mGBAEclipseCore: ~Copyable, CoreProtocol {
	public typealias VideoRenderer = CoreFrameBufferVideoRenderer
	public typealias Settings = mGBAEclipseCoreSettings

	public enum Failure: LocalizedError {
		case allocatingCore
		case failedToLoadROM
		case failedToOpenFile
		case failedToSave
		case failedToLoadState
		case failedToSaveState
	}

	public static let id: String = "dev.magnetar.eclipseemu.mba"
	public static let name: String = "mGBA"
	public static let sourceCodeRepository: URL = .init(string: "https://github.com/mgba-emu/mgba")!
	public static let developer: String = "Vicki Pfau"
	public static let version: String = "0.10.5"
	public static let systems: Set<System> = [.gba]

	public static func features(for system: EclipseKit.System) -> EclipseKit.CoreFeatures {
		guard system == .gba else { return [] }
		return [.cheats, .saving, .saveStates]
	}
	
	public static func cheatFormats(for system: System) -> [CoreCheatFormat] {
		guard system == .gba else { return [] }
		return [
			.init(
				id: "gba.gameshark",
				name: "GameShark",
				charset: .hexadecimal,
				pattern: "xxxxxxxx xxxxxxxx"
			),
			.init(
				id: "gba.actionreplay-max",
				name: "ActionReplay MAX",
				charset: .hexadecimal,
				pattern: "xxxxxxxx xxxxxxxx"
			),
			.init(
				id: "gba.codebreaker",
				name: "CodeBreaker",
				charset: .hexadecimal,
				pattern: "xxxxxxxx xxxx"
			)
		]
	}
	
	public var bridge: any CoreBridgeProtocol
	public let settings: CoreResolvedSettings<mGBAEclipseCoreSettings>
	public let playerConnectionBehavior: CorePlayerConnectionBehavior = .linear
	public let maxPlayers: UInt8 = 1
	public var desiredFrameRate: Double = 60.0

	private let core: UnsafeMutablePointer<mCore>
	private let callbackContext: UnsafeMutableRawPointer
	private var cheatSets: [CoreCheat : UnsafeMutablePointer<mCheatSet>] = [:]

	private let videoHeight: UInt16
	private let videoWidth: UInt16
	private let videoBufferSize: UInt32
	private var videoBuffer: UnsafeMutableBufferPointer<UInt8>!
	private let samples: UnsafeMutableBufferPointer<Int16>!

	public init(
		system: System,
		settings: consuming CoreResolvedSettings<mGBAEclipseCoreSettings>,
		bridge: consuming any CoreBridgeProtocol
	) throws(Self.Failure) {
		guard let core = GBACoreCreate() else {
			throw .allocatingCore
		}

		var videoWidth: UInt32 = 0
		var videoHeight: UInt32 = 0

		core.pointee.desiredVideoDimensions(core, &videoWidth, &videoHeight)
		core.pointee.setAudioBufferSize(core, sampleCount)

		let left = core.pointee.getAudioChannel(core, 0)
		let right = core.pointee.getAudioChannel(core, 1)
		let clockRate = core.pointee.frequency(core)
		let fauxClock = GBAAudioCalculateRatio(1, 60, 1)
		blip_set_rates(left, Double(clockRate), audioSampleRate * Double(fauxClock))
		blip_set_rates(right, Double(clockRate), audioSampleRate * Double(fauxClock))

		let wrapper = BridgeWrapper(bridge: bridge)
		callbackContext = Unmanaged.passRetained(wrapper).toOpaque()

		var coreCallbacks: mCoreCallbacks = .init()
		coreCallbacks.context = callbackContext
		coreCallbacks.savedataUpdated = handleSaveCallback

		core.pointee.addCoreCallbacks(core, &coreCallbacks)

		self.bridge = wrapper
		self.settings = settings
		self.core = core
		self.videoWidth = UInt16(videoWidth)
		self.videoHeight = UInt16(videoHeight)
		self.videoBufferSize = videoHeight * videoWidth * 4
		self.samples = .allocate(capacity: MemoryLayout<Int16>.stride * sampleCount * Int(audioSampleRate))
	}

	deinit {
		mCoreConfigDeinit(&core.pointee.config)
		core.pointee.deinit(core)
		self.samples.deallocate()
		_ = Unmanaged<BridgeWrapper>.fromOpaque(callbackContext).takeRetainedValue()
	}

	public func getVideoDescriptor() -> CoreVideoDescriptor {
		return .init(
			width: self.videoWidth,
			height: self.videoHeight,
			pixelFormat: .rgba8Unorm,
			frameBuffer: .assignable
		)
	}

	public func getAudioDescriptor() -> CoreAudioDescriptor {
		return .init(
			sampleRate: audioSampleRate,
			sampleFormat: .int16,
			channelCount: audioChannelCount
		)
	}
	
	public mutating func setFrameBuffer(to pointer: UnsafeMutableBufferPointer<UInt8>) {
		self.videoBuffer = pointer
	}

	public mutating func start(romPath: URL, savePath: URL) throws(Self.Failure) {
		let savePath = savePath.filePath()
		core.pointee.opts.savegamePath = strdup(savePath)
		guard mCoreLoadFile(core, romPath.filePath()) else {
			throw .failedToLoadROM
		}
		mCoreLoadSaveFile(core, savePath, false)
		core.pointee.reset(core)
	}

	public mutating func stop() {}

	public mutating func play() {}

	public mutating func pause() {}

	public mutating func reset(kind: CoreResetKind) {
		core.pointee.reset(core)
	}

	public mutating func step(timestamp: CFAbsoluteTime, willRender: Bool) {
		core.pointee.runFrame(core)

		let left = core.pointee.getAudioChannel(core, 0)
		let right = core.pointee.getAudioChannel(core, 1)

		var available = blip_samples_avail(left)
		let isOverLimit: Int32 = available > sampleCount ? 1 : 0
		available = isOverLimit * Int32(sampleCount) + (isOverLimit ^ 1) * available

		blip_read_samples(left, samples.baseAddress, available, 1)
		blip_read_samples(right, samples.baseAddress?.advanced(by: 1), available, 1)

		let samples = UnsafeRawBufferPointer(start: samples.baseAddress, count: Int(available) << 2)
		bridge.writeAudioSamples(samples: samples)
	}

	public func save(to path: URL) async throws(Self.Failure) {
		guard let vf = VFileOpen(path: path, flags: O_CREAT | O_TRUNC | O_RDWR) else {
			throw .failedToOpenFile
		}
		defer { _ = vf.pointee.close(vf) }

		let gba = core.pointee.board.assumingMemoryBound(to: GBA.self)
		guard GBASavedataClone(&gba.pointee.memory.savedata, vf) else {
			throw .failedToSave
		}
	}

	public func saveState(to path: URL) throws(Self.Failure) {
		guard let vf = VFileOpen(path: path, flags: O_CREAT | O_TRUNC | O_RDWR) else {
			throw .failedToOpenFile
		}
		defer { _ = vf.pointee.close(vf) }

		guard mCoreSaveStateNamed(core, vf, SAVESTATE_SAVEDATA) else {
			throw .failedToLoadState
		}
	}

	public mutating func loadState(from path: URL) throws(Self.Failure) {
		guard let vf = VFileOpen(path: path, flags: O_RDONLY) else {
			throw .failedToOpenFile
		}
		defer { _ = vf.pointee.close(vf) }

		guard mCoreLoadStateNamed(core, vf, SAVESTATE_SAVEDATA) else {
			throw .failedToLoadState
		}
	}

	public mutating func playerConnected(to port: UInt8) {}

	public mutating func playerDisconnected(from port: UInt8) {}

	public mutating func writeInput(_ delta: CoreInputDelta, for player: UInt8) {
		var keys: UInt32 = 0
		keys |= delta.input.contains(.faceButtonRight) ? 1 << GBA_KEY_A.rawValue : 0
		keys |= delta.input.contains(.faceButtonDown) ? 1 << GBA_KEY_B.rawValue : 0
		keys |= delta.input.contains(.leftShoulder) ? 1 << GBA_KEY_L.rawValue : 0
		keys |= delta.input.contains(.rightShoulder) ? 1 << GBA_KEY_R.rawValue : 0
		keys |= delta.input.contains(.start) ? 1 << GBA_KEY_START.rawValue : 0
		keys |= delta.input.contains(.select) ? 1 << GBA_KEY_SELECT.rawValue : 0
		keys |= delta.input.contains(.dpad) && delta.isUp ? 1 << GBA_KEY_UP.rawValue : 0
		keys |= delta.input.contains(.dpad) && delta.isDown ? 1 << GBA_KEY_DOWN.rawValue : 0
		keys |= delta.input.contains(.dpad) && delta.isLeft ? 1 << GBA_KEY_LEFT.rawValue : 0
		keys |= delta.input.contains(.dpad) && delta.isRight ? 1 << GBA_KEY_RIGHT.rawValue : 0
		core.pointee.setKeys(core, keys)
	}
	
	public mutating func setCheat(cheat: CoreCheat, enabled: Bool) {
		if let set = cheatSets[cheat] {
			set.pointee.enabled = enabled
		}

		guard
			let device = core.pointee.cheatDevice(core),
			let set = device.pointee.createSet(device, "\(cheat.format)\(cheat.code)")
		else {
			return
		}

		let size = mCheatSetsSize(&device.pointee.cheats)
		if size > 0 {
			set.pointee.copyProperties(set, mCheatSetsGetPointer(&device.pointee.cheats, size - 1).pointee)
		}

		for line in cheat.code.trimmingCharacters(in: .whitespacesAndNewlines).split(separator: "\n") {
			_ = line.withCString { cString in
				mCheatAddLine(set, cString, Int32(GBA_CHEAT_AUTODETECT.rawValue))
			}
		}

		set.pointee.enabled = enabled
		cheatSets[cheat] = set
		mCheatAddSet(device, set)
	}

	public mutating func clearCheats() {
		mCheatDeviceClear(core.pointee.cheatDevice(core))
		cheatSets.removeAll()
	}
}
