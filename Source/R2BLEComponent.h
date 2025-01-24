/*
  ==============================================================================

    R2BLEComponent.h
    Created: 23 Jan 2025 12:37:28am
    Author:  ring2

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <vector>
#include <string>

class R2BLEManager;

class R2BLEComponent : public juce::Component,
                       public juce::TableListBoxModel,
                       public juce::Button::Listener
{
public:
    explicit R2BLEComponent (R2BLEManager& manager);
    ~R2BLEComponent() override;

    void resized() override;
    void buttonClicked (juce::Button* button) override;

    // TableListBoxModel methods
    int getNumRows() override;
    void paintRowBackground (juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell (juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    void deviceListChanged();
    
private:
    void updateConnectButtonState();

    R2BLEManager& manager;
    juce::TableListBox deviceListBox;
    juce::TextButton connectButton { "Connect" };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(R2BLEComponent)
};
