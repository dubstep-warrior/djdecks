
#pragma once

#include <JuceHeader.h>
#include "ZoomedWaveform.h";
//==============================================================================

/**
 * Definition of a JogWheel Component
 *
 * A component that has similar playback control functionality to ZoomedWaveform
 * but a different appearance. Acts as a DJ Deck's JogWheel display with playback functionality.
 * Communicates with DJAudioPlayer via the DeckGUI interface
 *
 */
class JogWheel : public ZoomedWaveform
{
public:

	//==============================================================================

	/**
		* Class Constructor for JogWheel, initializes member variables.
		*
		* @param juce::AudioFormatManager reference that manages audio formats
		* @param AudioThumbnailCache reference that manages a cache of juce::AudioThumbnail objects
		* @param juce::Colour that defines the theme colour of the component
	*/
	JogWheel(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, juce::Colour _colour);

	/**
		* Class Destructor for JogWheel
	*/
	~JogWheel() override;

	//==============================================================================

private:

	//==============================================================================

	/**
		* Paints the JogWheel Component.
		*
		* @param juce::Graphics object for the component to draw itself on
	*/
	void paint(juce::Graphics&);

	/**
		* Set bounds of member components
	*/
	void resized();

	//==============================================================================

	/**
	   * Called when the mouse is dragged on the JogWheel.
	   *
	   * @param juce::MouseEvent triggered by user
   */
	void mouseDrag(const juce::MouseEvent& e);

	//==============================================================================

	/// juce::Points for the JogWheel's playhead
	juce::Point<float> startPoint, endPoint;

	/// juce::Line to draw JogWheel's playhead
	juce::Line<float> line;

	/// Label to mark the audio current time
	juce::Label timer;

	/// Number of rotations of the JogWheel playhead
	float noRotations = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JogWheel)
};
