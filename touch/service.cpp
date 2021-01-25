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

#define LOG_TAG "evervolv.touch@1.0-service.oneplus3"

#include <android-base/logging.h>
#include <binder/ProcessState.h>
#include <hidl/HidlTransportSupport.h>

#include "GloveMode.h"
#include "KeyDisabler.h"
#include "KeySwapper.h"
#include "TouchscreenGesture.h"

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::sp;
using android::status_t;
using android::OK;

using ::vendor::evervolv::touch::V1_0::implementation::GloveMode;
using ::vendor::evervolv::touch::V1_0::implementation::KeyDisabler;
using ::vendor::evervolv::touch::V1_0::implementation::KeySwapper;
using ::vendor::evervolv::touch::V1_0::implementation::TouchscreenGesture;

int main() {
    sp<GloveMode> gloveMode;
    sp<KeyDisabler> keyDisabler;
    sp<KeySwapper> keySwapper;
    sp<TouchscreenGesture> touchscreenGesture;
    status_t status;

    LOG(INFO) << "Touch HAL service is starting.";

    gloveMode = new GloveMode();
    if (gloveMode == nullptr) {
        LOG(ERROR) << "Can not create an instance of Touch HAL GloveMode Iface, exiting.";
        goto shutdown;
    }

    keyDisabler = new KeyDisabler();
    if (keyDisabler == nullptr) {
        LOG(ERROR) << "Can not create an instance of Touch HAL KeyDisabler Iface, exiting.";
        goto shutdown;
    }

    keySwapper = new KeySwapper();
    if (keySwapper == nullptr) {
        LOG(ERROR) << "Can not create an instance of Touch HAL KeySwapper Iface, exiting.";
        goto shutdown;
    }

    touchscreenGesture = new TouchscreenGesture();
    if (touchscreenGesture == nullptr) {
        LOG(ERROR) << "Can not create an instance of Touch HAL TouchscreenGesture Iface, exiting.";
        goto shutdown;
    }

    android::hardware::configureRpcThreadpool(1, true /*callerWillJoin*/);

    if (gloveMode->isSupported()) {
        status = gloveMode->registerAsService();
        if (status != OK) {
            LOG(ERROR)
                << "Could not register service for Touch HAL GloveMode Iface ("
                << status << ")";
            goto shutdown;
        }
    }

    if (keyDisabler->isSupported()) {
        status = keyDisabler->registerAsService();
        if (status != OK) {
            LOG(ERROR)
                << "Could not register service for Touch HAL KeyDisabler Iface ("
                << status << ")";
            goto shutdown;
        }
    }

    if (keySwapper->isSupported()) {
        status = keySwapper->registerAsService();
        if (status != OK) {
            LOG(ERROR)
                << "Could not register service for Touch HAL KeySwapper Iface ("
                << status << ")";
            goto shutdown;
        }
    }

    status = touchscreenGesture->registerAsService();
    if (status != OK) {
        LOG(ERROR)
            << "Could not register service for Touch HAL TouchscreenGesture Iface ("
            << status << ")";
        goto shutdown;
    }

    LOG(INFO) << "Touch HAL service is ready.";
    joinRpcThreadpool();
    // Should not pass this line

shutdown:
    // In normal operation, we don't expect the thread pool to shutdown
    LOG(ERROR) << "Touch HAL service is shutting down.";
    return 1;
}
