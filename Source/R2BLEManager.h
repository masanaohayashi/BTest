/*
  ==============================================================================

    R2BLEManager.h
    Created: 23 Jan 2025 12:16:44am
    Author:  ring2

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include "R2BLEComponent.h"

class R2BLEManager : public juce::Timer
{
public:
    using DeviceFoundCallback = std::function<void(const std::string& deviceName)>;
    using ConnectionErrorCallback = std::function<void(const std::string& error)>;

    R2BLEManager()
    {
        startTimer (1000);
    }
    virtual ~R2BLEManager() = default;

    // Starts scanning for devices with the specified UUID
    virtual void startScanningWithUUID(const std::string& uuid) = 0;

    // Stops scanning for devices
    virtual void stopScanning() = 0;

    // Checks if Bluetooth is enabled
    virtual bool isBluetoothEnabled() const = 0;

    // Checks if the device is connected
    virtual bool isDeviceConnected(const std::string& deviceName) const = 0;

    // Checks if the device can be connected
    virtual bool canDeviceConnect(const std::string& deviceName) const = 0;

    // Connects to a device and handles errors through the callback
    virtual void connectToDevice(const std::string& deviceName, ConnectionErrorCallback onError) = 0;

    // Disconnects from a device
    virtual void disconnectFromDevice(const std::string& deviceName) = 0;

    // Returns the associated R2BLEComponent
    virtual R2BLEComponent* getComponent()
    {
        if (!component) {
            component = std::make_unique<R2BLEComponent>(*this);
        }
        return component.get();
    }
    
    void timerCallback()
    {
        const auto currentTime = juce::Time::currentTimeMillis();
        const juce::int64 timeoutInterval = 10000; // 10秒間更新がなければ削除

        for (int i = devices.size() - 1; i >= 0; --i)
        {
            if (currentTime - devices[i].foundTimeInMS > timeoutInterval)
            {
                devices.remove(i);
                if (component != nullptr)
                {
                    component->deviceListChanged();
                }
            }
        }
    }

    void addOrUpdateDevice (const juce::String& identifier, const juce::String& serviceUUID, const juce::String& name)
    {
        const auto currentTime = juce::Time::currentTimeMillis();
        for (auto& device : devices)
        {
            if (device.identifier == identifier)
            {
                device.foundTimeInMS = currentTime;
                device.serviceUUID = serviceUUID;
                device.name = name;
                //DBG("Updated device: " + name + " (Identifier: " + identifier + ", serviceUUID: " + serviceUUID + ")");
                return;
            }
        }

        devices.add({ identifier, serviceUUID, name, currentTime });
        //DBG("Added new device: " + name + " (Identifier: " + identifier + ")");

        if (component != nullptr)
        {
            component->deviceListChanged();
        }
    }

    struct DeviceInfo
    {
        juce::String identifier;
        juce::String serviceUUID;
        juce::String name;
        juce::int64 foundTimeInMS;
    };

    juce::Array<DeviceInfo> getDevices() const
    {
        return devices;
    }

protected:
    std::unique_ptr<R2BLEComponent> component;

private:
    juce::Array<DeviceInfo> devices;
};
