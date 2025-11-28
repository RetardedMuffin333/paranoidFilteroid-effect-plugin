#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p), processor(p) {
    // Set editor size
    setSize(400, 300);

    // Mode ComboBox
    modeCombo.addItem("Telephone", 1);
    modeCombo.addItem("Radio", 2);
    modeCombo.addItem("Custom", 3);
    modeCombo.setSelectedItemIndex(0);
    addAndMakeVisible(modeCombo);

    modeLabel.setText("Mode:", juce::dontSendNotification);
    modeLabel.attachToComponent(&modeCombo, true);
    addAndMakeVisible(modeLabel);

    // Mix Slider
    mixSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mixSlider.setRange(0.0, 1.0, 0.01);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    addAndMakeVisible(mixSlider);

    mixLabel.setText("Mix:", juce::dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, true);
    addAndMakeVisible(mixLabel);

    // Enabled Toggle Button
    enabledButton.setButtonText("Enabled");
    enabledButton.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(enabledButton);

    // Create APVTS attachments
    modeAttachment = std::make_unique<ComboBoxAttachment>(
        processor.apvts, "mode", modeCombo
    );

    mixAttachment = std::make_unique<SliderAttachment>(
        processor.apvts, "mix", mixSlider
    );

    enabledAttachment = std::make_unique<ButtonAttachment>(
        processor.apvts, "enabled", enabledButton
    );
}

PluginEditor::~PluginEditor() {
}

//==============================================================================
void PluginEditor::paint(juce::Graphics& g) {
    // Fill background
    g.fillAll(getLookAndFeel().findColour(
        juce::ResizableWindow::backgroundColourId
    ));
}

void PluginEditor::resized() {
    auto area = getLocalBounds().reduced(20);

    // Mode ComboBox (with label space)
    auto modeArea = area.removeFromTop(30);
    modeCombo.setBounds(modeArea.removeFromLeft(100));

    // Mix Slider (with label space)
    area.removeFromTop(10);
    auto mixArea = area.removeFromTop(30);
    mixSlider.setBounds(mixArea.removeFromLeft(200));

    // Enabled Toggle Button
    area.removeFromTop(10);
    auto enabledArea = area.removeFromTop(30);
    enabledButton.setBounds(enabledArea.removeFromLeft(100));
}
