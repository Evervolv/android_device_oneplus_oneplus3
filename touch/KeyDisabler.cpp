/*
 * Copyright (C) 2019 The LineageOS Project
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

#define LOG_TAG "KeyDisablerService"

#include <fstream>

#include "KeyDisabler.h"

namespace vendor {
namespace evervolv {
namespace touch {
namespace V1_0 {
namespace implementation {

bool KeyDisabler::isSupported() {
    std::ofstream file("/proc/s1302/virtual_key");
    return file.good();
}

// Methods from ::vendor::evervolv::touch::V1_0::IKeyDisabler follow.
Return<bool> KeyDisabler::isEnabled() {
    std::ifstream file("/proc/s1302/virtual_key");
    int status = -1;

    if (file.is_open()) {
        file >> status;
    }

    return file.good() && status == 0;
}

Return<bool> KeyDisabler::setEnabled(bool enabled) {
    std::ofstream file("/proc/s1302/virtual_key");
    file << (enabled ? "1" : "0");
    return true;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace evervolv
}  // namespace vendor
