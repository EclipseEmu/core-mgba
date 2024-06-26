/* Copyright (c) 2013-2016 Jeffrey Pfau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include <mgba/core/version.h>

MGBA_EXPORT const char* const gitCommit = "${GIT_COMMIT}";
MGBA_EXPORT const char* const gitCommitShort = "${GIT_COMMIT_SHORT}";
MGBA_EXPORT const char* const gitBranch = "${GIT_BRANCH}";
MGBA_EXPORT const int gitRevision = 0;
MGBA_EXPORT const char* const binaryName = "${BINARY_NAME}";
MGBA_EXPORT const char* const projectName = "${PROJECT_NAME}";
MGBA_EXPORT const char* const projectVersion = "${VERSION_STRING}";
