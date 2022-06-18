#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "Library.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/*
	This component lives inside our window, and this is where you should put all
	your controls and content.
*/
class MainComponent : public juce::AudioAppComponent, public juce::Slider::Listener, public juce::KeyListener
{
public:
	//==============================================================================

	/**
		* Class Constructor for MainComponent, initializes member variables and configures component details.
	*/
	MainComponent();

	/**
		* Class Destructor for MainComponent, initializes member variables and configures component details.
	*/
	~MainComponent() override;

	//==============================================================================

	/**
		* Called when key is pressed.
		*
		* @param juce::KeyPress object
		* @param Component that has added this MainComponent as its listener
	*/
	bool keyPressed(const juce::KeyPress& key, Component* originatingComponent);
	//==============================================================================

	/**
		* Prepares audio source members
		*
		* @param Expected samples in a block
		* @param Number of samples per second
	*/
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

	/**
		* Called repeatedly to fetch subsequent blocks of audio data.
		*
		* @param juce::AudioSourceChannelInfo&: Buffer to be filled by audio source
	*/
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

	/**
		* Release resources on audio sources.
	*/
	void releaseResources() override;
	//==============================================================================

	/**
		* Paints the MainComponent.
		*
		* @param juce::Graphics object for the component to draw itself on
	*/
	void paint(juce::Graphics& g) override;

	/**
		* Set bounds of member components
	*/
	void resized() override;
	//==============================================================================

	/**
		* Called when slider knob in MainComponent listener is dragged.
		*
		* @param juce::Slider object that has added this component as its listener
	*/
	void sliderValueChanged(juce::Slider* slider) override;

	//==============================================================================

private:
	//==============================================================================

	/// Instance of CustomLookAndFeel class.
	CustomLookAndFeel customLookAndFeel;

	/// Instance of Library class.
	Library library{ formatManager };

	/// Instance of AudioFormatManager class.
	juce::AudioFormatManager formatManager;

	/// Instance of AudioThumbnailCache class.
	juce::AudioThumbnailCache thumbCache{ 100 };

	/// Instance of DJAudioPlayer class for the left DJ Deck.
	DJAudioPlayer player1{ formatManager };

	/// Instance of DJAudioPlayer class for the right DJ Deck.
	DJAudioPlayer player2{ formatManager };

	/// Instance of MixerAudioSource class for managing both DJAudioPlayer instances.
	juce::MixerAudioSource mixerSource;

	/// Instance of ZoomedWaveform class for the left DJ Deck's audio track.
	ZoomedWaveform zoomedDisplay1{ formatManager, thumbCache, juce::Colours::aqua };

	/// Instance of ZoomedWaveform class for the right DJ Deck's audio track.
	ZoomedWaveform zoomedDisplay2{ formatManager, thumbCache,juce::Colours::hotpink };

	/// Instance of DeckGUI class for the left DJ Deck.
	DeckGUI deckGUI1{ &player1, formatManager, thumbCache,&zoomedDisplay1 , library,juce::Colours::aqua };

	/// Instance of DeckGUI class for the right DJ Deck.
	DeckGUI deckGUI2{ &player2, formatManager, thumbCache,&zoomedDisplay2 , library,juce::Colours::hotpink };

	/// Instance of juce::Slider for cross fading functionality.
	juce::Slider crossFader{ juce::Slider::SliderStyle::LinearHorizontal , juce::Slider::TextEntryBoxPosition::NoTextBox };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
