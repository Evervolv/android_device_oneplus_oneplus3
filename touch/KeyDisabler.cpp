/*
 * Copyright (C) 2019,2021 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

#include "KeyDisabler.h"

namespace {
constexpr const char kControlPath[] = "/proc/s1302/virtual_key";
};  // anonymous namespace

namespace vendor {
namespace evervolv {
namespace touch {
namespace V1_0 {
namespace implementation {

KeyDisabler::KeyDisabler() : has_key_disabler_(!access(kControlPath, R_OK | W_OK)) {}

// Methods from ::vendor::evervolv::touch::V1_0::IKeyDisabler follow.
Return<bool> KeyDisabler::isEnabled() {
    if (!has_key_disabler_) return false;

    std::string buf;
    if (!android::base::ReadFileToString(kControlPath, &buf)) {
        LOG(ERROR) << "Failed to read " << kControlPath;
        return false;
    }

    return std::stoi(android::base::Trim(buf)) == 1;
}

Return<bool> KeyDisabler::setEnabled(bool enabled) {
    if (!has_key_disabler_) return false;

    if (!android::base::WriteStringToFile(std::to_string(enabled), kControlPath)) {
        LOG(ERROR) << "Failed to write " << kControlPath;
        return false;
    }

    return true;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace evervolv
}  // namespace vendor
