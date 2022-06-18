
#pragma once

#include <JuceHeader.h>
#include "Track.h"
//==============================================================================

/**
 * Definition of a WaveformDisplay Component
 *
 * A component to display the loaded audio file's waveform.
 * Playback functionality is included to set the current player position.
 * Communicates with DJAudioPlayer controls via the DeckGUI interface
 *
 */
class WaveformDisplay : public juce::Slider,
	public juce::ChangeListener
{
public:

	//==============================================================================

	/**
	   * Class Constructor for WaveformDisplay, initializes member variables and configure component details.
	   *
	   * @param juce::AudioFormatManager reference that manages audio formats
	   * @param AudioThumbnailCache reference that manages a cache of juce::AudioThumbnail objects
	   * @param juce::Colour that defines the theme colour of the component
   */
	WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, juce::Colour _colour);

	/**
	  * Class destructor for WaveformDisplay
	*/
	~WaveformDisplay() override;

	//==============================================================================

	 /**
		* @return Position of the playhead
	*/
	double getPosition();

	/**
		* @return If component is dragged
	*/
	bool isSliderDragged();

	/**
		* @return If audio file is loaded into component
	*/
	bool isFileLoaded();

	//==============================================================================

	/**
		 * Loads component with track
		 *
		 * @param track object
	 */
	void loadTrack(track track);

	//==============================================================================

	/**
		* Set the relative position of the playhead
		*
		* @param double between 0 and 1
	*/
	void setPositionRelative(double pos);

	/**
		* Set the cueTargets data structure
		*
		* @param std::map between juce::TextButton pointers and std::pair<double, float>
	*/
	void setCuePoints(std::map<juce::TextButton*, std::pair<double, float>>& _cueTargets);

	//============================================================================== 

private:

	//============================================================================== 

	/**
	   * Paints the WaveformDisplay Component.
	   *
	   * @param juce::Graphics object for the component to draw itself on
   */
	void paint(juce::Graphics&) override;

	/**
		* Set bounds of member components
	*/
	void resized() override;

	//============================================================================== 

	/**
		* Called when change is detected in audioThumb member
		*
		* @param juce::ChangeBroadcaster pointer
	*/
	void changeListenerCallback(juce::ChangeBroadcaster* source) override;

	//============================================================================== 

	/**
		* Called when mouse is moving in component
		*
		* @param juce::MouseEvent reference triggered by user
	*/
	void mouseMove(const juce::MouseEvent& e);

	/**
		* Called when mouse has exited component
		*
		* @param juce::MouseEvent reference triggered by user
	*/
	void mouseExit(const juce::MouseEvent& e);

	/**
		* Called when mouse is clicked on component
		*
		* @param juce::MouseEvent reference triggered by user
	*/
	void mouseDown(const juce::MouseEvent& e);

	/**
		* Called when mouse is dragging on component
		*
		* @param juce::MouseEvent reference triggered by user
	*/
	void mouseDrag(const juce::MouseEvent& e);

	/**
		* Called when mouse click on component has been lifted
		*
		* @param juce::MouseEvent reference triggered by user
	*/
	void mouseUp(const juce::MouseEvent& e);

	//============================================================================== 

	/**
		* Loads component with audio URL of file
		*
		* @param juce::URL object
	*/
	void loadURL(juce::URL audioURL);

	//============================================================================== 


	/// Tracks if mouse has entered component
	bool mouseEntered = false;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay);

protected:

	/// Song name of the loaded audio file
	juce::String songNameLoaded;

	/// AudioThumbnail to draw waveform
	juce::AudioThumbnail audioThumb;

	/// Position of the audio song
	double position = 0;

	/// Previous mouse Y value
	double prevY = 0;

	/// Previous mouse X value
	double prevX = 0;

	/// Data structure to store cue points
	std::vector<std::pair<double, float>*> cueTargets;

	/// juce::Colour to color the waveform
	juce::Colour theme;

	/// Flags if the slider is dragged
	bool sliderIsDragged = false;

	/// Flags if the file is loaded
	bool isLoaded = false;
};
