#include "mGBAEclipseCore.hpp"

#include <mgba-util/memory.h>
#include <mgba-util/vfs.h>
#include <mgba/core/blip_buf.h>
#include <mgba/core/cheats.h>
#include <mgba/core/core.h>
#include <mgba/core/serialize.h>
#include <mgba/core/version.h>
#include <mgba/gba/core.h>
#include <mgba/internal/gba/audio.h>
#include <mgba/internal/gba/cheats.h>
#include <mgba/internal/gba/gba.h>
#include <mgba/internal/gba/input.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>

void _log(
    struct mLogger* logger,
    int category,
    enum mLogLevel level,
    const char* format,
    va_list args)
{
}

static struct mLogger noopLogCtx = { .log = _log };

namespace eclipsecore {

struct CoreContext {
    const EKCoreCallbacks *callbacks;
    struct mCore *core;
    
    int16_t *samples;
    
    uint8_t *videoBuffer;
    uint32_t videoHeight;
    uint32_t videoWidth;
    uint32_t videoBufferSize;
    
    std::unordered_map<std::string, struct mCheatSet *> cheatSets;
};

#pragma mark - Consts

static const size_t audioChannelCount = 2;
static const size_t audioSampleRate = 32768;
static const size_t sampleCount = 1024;

const size_t audioBufferSize = sampleCount * audioSampleRate;
const EKCoreSetting settings[] = {};

const char *characterSetHexadeciaml = "1234567890ABCDEFabcdef";

EKCheatFormat cheatFormats[] = {
    {
        .id = "gba.gameshark",
        .displayName = "GameShark",
        .characterSet = EKCoreCheatCharacterSetHexadecimal,
        .format = "xxxxxxxx xxxxxxxx",
    },
    {
        .id = "gba.actionreplay-max",
        .displayName = "ActionReplay MAX",
        .characterSet = EKCoreCheatCharacterSetHexadecimal,
        .format = "xxxxxxxx xxxxxxxx"
    },
    {
        .id = "gba.codebreaker",
        .displayName = "CodeBreaker",
        .characterSet = EKCoreCheatCharacterSetHexadecimal,
        .format = "xxxxxxxx xxxx",
    }
};

#pragma mark - Setup

void deallocate(void* data)
{
    CoreContext* ctx = static_cast<CoreContext*>(data);

    mCoreConfigDeinit(&ctx->core->config);
    ctx->core->deinit(ctx->core);
    free(ctx->samples);
    free(ctx);
}

EKCoreAudioFormat getAudioFormat(void* data)
{
    return {
        .commonFormat = EKCoreCommonAudioFormatPcmInt16,
        .sampleRate = audioSampleRate,
        .channelCount = audioChannelCount,
    };
}

EKCoreVideoFormat getVideoFormat(void* data)
{
    CoreContext* ctx = static_cast<CoreContext*>(data);

    return {
        .pixelFormat = EKCoreVideoPixelFormatRgba8Unorm,
        .renderingType = EKCoreVideoRenderingTypeFrameBuffer,
        .width = ctx->videoWidth,
        .height = ctx->videoHeight,
    };
}

double getDesiredFrameRate(void* data)
{
    CoreContext* ctx = static_cast<CoreContext*>(data);
    return ctx->core->frequency(ctx->core) / ctx->core->frameCycles(ctx->core);
}

bool canSetVideoPointer(void* data)
{
    return true;
}

uint8_t* getVideoPointer(void* data, uint8_t* preferredPointer)
{
    CoreContext* ctx = static_cast<CoreContext*>(data);
    if (preferredPointer != nullptr) {
        ctx->videoBuffer = preferredPointer;
        ctx->core->setVideoBuffer(ctx->core, (color_t*)preferredPointer, ctx->videoWidth);
    }
    return ctx->videoBuffer;
}

#pragma mark - Lifecycle

bool start(void* data, const char* const romPath, const char* const savePath)
{
    CoreContext* ctx = static_cast<CoreContext*>(data);
    ctx->core->opts.savegamePath = strdup(savePath);
    
    if (!mCoreLoadFile(ctx->core, romPath)) {
        return false;
    }

    mCoreLoadSaveFile(ctx->core, savePath, false);

    ctx->core->reset(ctx->core);

    return true;
}

void stop(void* data) { }

void restart(void* data)
{
    CoreContext* ctx = static_cast<CoreContext*>(data);
    ctx->core->reset(ctx->core);
}

void play(void* data) { }
void pause(void* data) { }

void executeFrame(void* data, bool willRender)
{
    CoreContext* ctx = static_cast<CoreContext*>(data);

    ctx->core->runFrame(ctx->core);

    blip_t* left = ctx->core->getAudioChannel(ctx->core, 0);
    blip_t* right = ctx->core->getAudioChannel(ctx->core, 1);

    int available = blip_samples_avail(left);
    int isOverLimit = (available > eclipsecore::sampleCount);
    available = isOverLimit * eclipsecore::sampleCount + (isOverLimit ^ 1) * available;

    blip_read_samples(left, ctx->samples, available, true);
    blip_read_samples(right, ctx->samples + 1, available, true);

    size_t interleavedAvailableBytes = available << 2;
    ctx->callbacks->writeAudio(ctx->callbacks->callbackContext, ctx->samples, interleavedAvailableBytes);
}

#pragma mark - Saves & Save States

bool save(void* data, const char* path)
{
    CoreContext* ctx = static_cast<CoreContext*>(data);
    struct VFile* vf = VFileOpen(path, O_CREAT | O_TRUNC | O_RDWR);
    struct GBA *gba = static_cast<struct GBA *>(ctx->core->board);
    bool success = GBASavedataClone(&gba->memory.savedata, vf);
    return success;
}

void saveCallback(void* data)
{
    CoreContext* ctx = static_cast<CoreContext*>(data);
    ctx->callbacks->didSave(ctx->callbacks->callbackContext);
}

bool saveState(void* data, const char* path)
{
    CoreContext* ctx = static_cast<CoreContext*>(data);
    struct VFile* vf = VFileOpen(path, O_CREAT | O_TRUNC | O_RDWR);
    bool success = mCoreSaveStateNamed(ctx->core, vf, SAVESTATE_SAVEDATA);
    vf->close(vf);
    return success;
}

bool loadState(void* data, const char* path)
{
    CoreContext* ctx = static_cast<CoreContext*>(data);
    struct VFile* vf = VFileOpen(path, O_RDONLY);
    bool success = mCoreLoadStateNamed(ctx->core, vf, SAVESTATE_SAVEDATA);
    vf->close(vf);
    return success;
}

uint8_t getMaxPlayers(void* data) { return 1; }

/// Notifies a core that a new player has connected.
bool playerConnected(void* data, uint8_t player) { return true; }

/// Notifies a core that a player has been disconnected.
void playerDisconnected(void* data, uint8_t player) { }

void playerSetInputs(void* data, uint8_t player, uint32_t inputs)
{
    CoreContext* ctx = static_cast<CoreContext*>(data);

    // FIXME: see about implementing some GPIO-related inputs like gyro and maybe even the solar sensor
    
    uint32_t keys =
        ((inputs & EKInputFaceButtonRight) != 0) * (1 << GBA_KEY_A) |
        ((inputs & EKInputFaceButtonDown) != 0) * (1 << GBA_KEY_B) |
        ((inputs & EKInputShoulderLeft) != 0) * (1 << GBA_KEY_L) |
        ((inputs & EKInputShoulderRight) != 0) * (1 << GBA_KEY_R) |
        ((inputs & EKInputStartButton) != 0) * (1 << GBA_KEY_START) |
        ((inputs & EKInputSelectButton) != 0) * (1 << GBA_KEY_SELECT) |
        ((inputs & EKInputDpadUp) != 0) * (1 << GBA_KEY_UP) |
        ((inputs & EKInputDpadDown) != 0) * (1 << GBA_KEY_DOWN) |
        ((inputs & EKInputDpadLeft) != 0) * (1 << GBA_KEY_LEFT) |
        ((inputs & EKInputDpadRight) != 0) * (1 << GBA_KEY_RIGHT);
    
    ctx->core->setKeys(ctx->core, keys);
}

bool setCheat(void* data, const char* format, const char *code, bool enabled)
{
    CoreContext* ctx = static_cast<CoreContext*>(data);
    
    std::string codeStr = code;
    codeStr.erase(std::remove(codeStr.begin(), codeStr.end(), ' '), codeStr.end());
    
    std::stringstream hashKeyBuilder;
    hashKeyBuilder << format << '$' << codeStr;
    std::string hashKey = hashKeyBuilder.str();
    
    mCheatSet *set = ctx->cheatSets[hashKey];
    if (set != nullptr) {
        set->enabled = enabled;
        return;
    }

    struct mCheatDevice* device = ctx->core->cheatDevice(ctx->core);
    set = device->createSet(device, hashKey.c_str());

    size_t size = mCheatSetsSize(&device->cheats);
    if (size) {
        set->copyProperties(set, *mCheatSetsGetPointer(&device->cheats, size - 1));
    }
    
    size_t last = 0;
    size_t next = 0;
    while ((next = codeStr.find('\n', last)) != std::string::npos) {
        mCheatAddLine(set, codeStr.substr(last, next - last).c_str(), GBA_CHEAT_AUTODETECT);
        last = next + 1;
    }
    mCheatAddLine(set, codeStr.substr(last).c_str(), GBA_CHEAT_AUTODETECT);

    set->enabled = enabled;
    ctx->cheatSets[hashKey] = set;
    mCheatAddSet(device, set);
    
    return true;
}

void clearCheats(void *data) {
    CoreContext* ctx = static_cast<CoreContext*>(data);
    mCheatDeviceClear(ctx->core->cheatDevice(ctx->core));
    ctx->cheatSets.clear();
}
}

const EKCoreInfo mGBACoreInfo = {
    .id = "dev.magnetar.eclipseemu.mgba",
    .name = "mGBA",
    .developer = "Vicki Pfau",
    .version = "0.10.3",
    .sourceCodeUrl = "https://github.com/mgba-emu/mgba",
    .settings = {
        .version = 0,
        .itemsCount = sizeof(eclipsecore::settings) / sizeof(EKCoreSetting),
        .items = eclipsecore::settings,
    },
    .cheatFormatsCount = sizeof(eclipsecore::cheatFormats) / sizeof(EKCheatFormat),
    .cheatFormats = eclipsecore::cheatFormats,

    .setup = [](EKSystem system, const EKCoreCallbacks* callbacks) -> EKCore* {
        // do allocations

        eclipsecore::CoreContext* coreContext = static_cast<eclipsecore::CoreContext*>(malloc(sizeof(eclipsecore::CoreContext)));
        if (coreContext == nullptr) {
            return nullptr;
        }

        int16_t* audioBuffer = static_cast<int16_t*>(malloc(sizeof(int16_t) * eclipsecore::audioBufferSize));
        if (audioBuffer == nullptr) {
            free(coreContext);
            return nullptr;
        }

        EKCore* core = static_cast<EKCore*>(malloc(sizeof(EKCore)));
        if (core == nullptr) {
            free(coreContext);
            free(audioBuffer);
            return nullptr;
        }

        mLogSetDefaultLogger(&noopLogCtx);

        struct mCore* mgbaCore = GBACoreCreate();
        if (mgbaCore == nullptr) {
            free(coreContext);
            free(audioBuffer);
            free(core);
            return nullptr;
        }

        // setup mgba

        mCoreInitConfig(mgbaCore, nullptr);

        struct mCoreOptions mgbaCoreOptions = {
            .useBios = true
        };
        mCoreConfigLoadDefaults(&mgbaCore->config, &mgbaCoreOptions);
        if (mgbaCore->init(mgbaCore) != true) {
            free(coreContext);
            free(audioBuffer);
            free(core);
            mgbaCore->deinit(mgbaCore);
            return nullptr;
        }

        struct mCoreCallbacks mgbaCallbacks = {
            .context = (void*)coreContext,
            .savedataUpdated = eclipsecore::saveCallback,
        };
        mgbaCore->addCoreCallbacks(mgbaCore, &mgbaCallbacks);
        
        unsigned int videoWidth = 0;
        unsigned int videoHeight = 0;

        mgbaCore->desiredVideoDimensions(mgbaCore, &videoWidth, &videoHeight);
        mgbaCore->setAudioBufferSize(mgbaCore, eclipsecore::sampleCount);

        blip_t* left = mgbaCore->getAudioChannel(mgbaCore, 0);
        blip_t* right = mgbaCore->getAudioChannel(mgbaCore, 1);

        double clockRate = mgbaCore->frequency(mgbaCore);
        double fauxClock = GBAAudioCalculateRatio(1, 60, 1);

        blip_set_rates(left, clockRate, eclipsecore::audioSampleRate * fauxClock);
        blip_set_rates(right, clockRate, eclipsecore::audioSampleRate * fauxClock);

        // setup the core context

        coreContext->core = mgbaCore;
        coreContext->callbacks = callbacks;
        coreContext->samples = audioBuffer;
        coreContext->videoHeight = videoHeight;
        coreContext->videoWidth = videoWidth;
        coreContext->videoBufferSize = videoHeight * videoWidth * BYTES_PER_PIXEL;
        coreContext->cheatSets = std::unordered_map<std::string, struct mCheatSet *>();

        // setup the core

        core->data = coreContext;
        core->deallocate = eclipsecore::deallocate;
        core->getAudioFormat = eclipsecore::getAudioFormat;
        core->getVideoFormat = eclipsecore::getVideoFormat;
        core->getDesiredFrameRate = eclipsecore::getDesiredFrameRate;
        core->canSetVideoPointer = eclipsecore::canSetVideoPointer;
        core->getVideoPointer = eclipsecore::getVideoPointer;
        core->start = eclipsecore::start;
        core->stop = eclipsecore::stop;
        core->restart = eclipsecore::restart;
        core->play = eclipsecore::play;
        core->pause = eclipsecore::pause;
        core->executeFrame = eclipsecore::executeFrame;
        core->save = eclipsecore::save;
        core->saveState = eclipsecore::saveState;
        core->loadState = eclipsecore::loadState;
        core->getMaxPlayers = eclipsecore::getMaxPlayers;
        core->playerConnected = eclipsecore::playerConnected;
        core->playerDisconnected = eclipsecore::playerDisconnected;
        core->playerSetInputs = eclipsecore::playerSetInputs;
        core->setCheat = eclipsecore::setCheat;
        core->clearCheats = eclipsecore::clearCheats;

        return core;
    }
};
