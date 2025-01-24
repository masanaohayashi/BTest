/*
  ==============================================================================

    R2BLEManagerIOS.h
    Created: 23 Jan 2025 12:26:27am
    Author:  ring2

  ==============================================================================
*/
#pragma once

#include "R2BLEManager.h"
#include <memory>

class R2BLEManagerIOS : public R2BLEManager {
public:
    R2BLEManagerIOS();
    ~R2BLEManagerIOS() override;

    void startScanningWithUUID(const std::string &uuid) override;
    void stopScanning() override;
    bool isBluetoothEnabled() const override;
    bool isDeviceConnected(const std::string &deviceName) const override { return false; }
    bool canDeviceConnect(const std::string &deviceName) const override { return true; }
    void connectToDevice(const std::string &deviceName, ConnectionErrorCallback onError) override {}
    void disconnectFromDevice(const std::string &deviceName) override {}

private:
    class Impl; // 前方宣言
    std::unique_ptr<Impl> impl;
};
