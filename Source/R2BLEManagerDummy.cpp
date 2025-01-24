/*
  ==============================================================================

    R2BLEManagerDummy.cpp
    Created: 23 Jan 2025 8:29:00pm
    Author:  ring2

  ==============================================================================
*/

#include "R2BLEManagerDummy.h"

R2BLEManagerDummy::R2BLEManagerDummy()
: random (juce::Time::getCurrentTime().toMilliseconds())
, scanStarted (false)
{
    
}

void R2BLEManagerDummy::timerCallback()
{
    DBG ("R2BLEManagerDummy::timerCallback() called.");
    R2BLEManager::timerCallback();
    
    auto r = random.nextInt() % 100;
    juce::Uuid uuid;
    juce::String identifier = uuid.toString();
    juce::String serviceUuid = "0x1850";
    juce::String name = "Dummy Device " + juce::String(r);
    
    addOrUpdateDevice (identifier, serviceUuid, name);
}

void R2BLEManagerDummy::startScanningWithUUID (const std::string& uuid)
{
    scanStarted = true;
}

void R2BLEManagerDummy::stopScanning()
{
    scanStarted = false;
}

bool R2BLEManagerDummy::isBluetoothEnabled() const
{
    return true;
}

bool R2BLEManagerDummy::isDeviceConnected (const std::string& deviceName) const
{
    return false;
}

bool R2BLEManagerDummy::canDeviceConnect (const std::string& deviceName) const
{
    return false;
}

void R2BLEManagerDummy::connectToDevice (const std::string& deviceName, ConnectionErrorCallback onError)
{
}

void R2BLEManagerDummy::disconnectFromDevice (const std::string& deviceName)
{
}
