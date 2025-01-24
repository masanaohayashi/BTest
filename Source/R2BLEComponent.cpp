/*
  ==============================================================================

    R2BLEComponent.cpp
    Created: 23 Jan 2025 12:37:28am
    Author:  ring2

  ==============================================================================
*/

#include "R2BLEComponent.h"
#include "R2BLEManager.h"
#include <iostream>

R2BLEComponent::R2BLEComponent(R2BLEManager& theManager)
: manager(theManager)
{
    addAndMakeVisible(deviceListBox);
    deviceListBox.setModel(this);

    addAndMakeVisible(connectButton);
    connectButton.addListener(this);
    connectButton.setEnabled(false); // 初期状態では無効化

    deviceListBox.getHeader().addColumn("Name", 1, 400);
    deviceListBox.getHeader().addColumn("Identifier", 2, 400);

    // スキャン開始
    manager.startScanningWithUUID ("");
}

R2BLEComponent::~R2BLEComponent()
{
//    manager.stopScanning();   //  SIGABORT
}


void R2BLEComponent::resized()
{
    auto bounds = getLocalBounds();
    deviceListBox.setBounds(bounds.removeFromTop(bounds.getHeight() - 40));
    connectButton.setBounds(bounds);
}

void R2BLEComponent::buttonClicked(juce::Button* button)
{
    if (button == &connectButton) {
        int selectedRow = deviceListBox.getSelectedRow();
        const auto& devices = manager.getDevices();

        if (selectedRow >= 0 && selectedRow < static_cast<int>(devices.size())) {
            const auto& deviceName = devices[selectedRow];
/*
            if (connectButton.getButtonText() == "Connect") {
                manager.connectToDevice(deviceName, [this](const std::string& error) {
                    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                           "Connection Error",
                                                           juce::String(error));
                });
                connectButton.setButtonText("Disconnect");
            } else if (connectButton.getButtonText() == "Disconnect") {
                manager.disconnectFromDevice(deviceName);
                connectButton.setButtonText("Connect");
            }
*/
        }
    }
}

int R2BLEComponent::getNumRows()
{
    return static_cast<int>(manager.getDevices().size());
}

void R2BLEComponent::paintRowBackground (juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected) {
        g.fillAll(juce::Colours::lightblue);
    } else {
        g.fillAll(juce::Colours::white);
    }
}

void R2BLEComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    const auto& devices = manager.getDevices();
    if (rowNumber < static_cast<int>(devices.size()))
    {
        auto devices = manager.getDevices();
        g.setColour(juce::Colours::black);
        if (columnId == 1)
        {
            g.drawText (devices[rowNumber].name, 2, 0, width - 4, height, juce::Justification::centredLeft);
        }
        else if (columnId == 2)
        {
            g.drawText (devices[rowNumber].identifier, 2, 0, width - 4, height, juce::Justification::centredLeft);
        }
    }

    g.setColour(juce::Colours::lightgrey);
    g.drawRect(0, 0, width, height);
}

void R2BLEComponent::updateConnectButtonState()
{
    int selectedRow = deviceListBox.getSelectedRow();
    const auto& devices = manager.getDevices();

    if (selectedRow >= 0 && selectedRow < static_cast<int>(devices.size())) {
        const auto& deviceName = devices[selectedRow];
/*
        if (manager.isDeviceConnected(deviceName)) {
            connectButton.setButtonText("Disconnect");
            connectButton.setEnabled(true);
        } else if (manager.canDeviceConnect(deviceName)) {
            connectButton.setButtonText("Connect");
            connectButton.setEnabled(true);
        } else {
            connectButton.setButtonText("Connect");
            connectButton.setEnabled(false);
        }
*/
    } else {
        connectButton.setButtonText("Connect");
        connectButton.setEnabled(false);
    }
}

void R2BLEComponent::deviceListChanged()
{
    deviceListBox.updateContent();
}

