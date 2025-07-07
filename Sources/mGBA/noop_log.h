#pragma once

#include <mgba/core/log.h>

void _log(struct mLogger*, int category, enum mLogLevel level, const char* format, va_list args) {}

static struct mLogger noopLogCtx = { .log = _log };

void setNoopLoggerAsGlobal() {
	mLogSetDefaultLogger(&noopLogCtx);
}
