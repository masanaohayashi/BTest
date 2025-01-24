/*
  ==============================================================================

    R2BLEManagerDummy.h
    Created: 23 Jan 2025 8:29:00pm
    Author:  ring2

  ==============================================================================
*/

#pragma once

#include "R2BLEManager.h"

class R2BLEManagerDummy : public R2BLEManager
{
public:
    R2BLEManagerDummy();
    virtual ~R2BLEManagerDummy() = default;

    void timerCallback() override;

    void startScanningWithUUID (const std::string& uuid) override;
    void stopScanning() override;
    bool isBluetoothEnabled() const override;
    bool isDeviceConnected (const std::string& deviceName) const override;
    bool canDeviceConnect (const std::string& deviceName) const override;
    void connectToDevice (const std::string& deviceName, ConnectionErrorCallback onError) override;
    void disconnectFromDevice (const std::string& deviceName) override;
    
private:
    juce::Random random;
    bool scanStarted;
};
