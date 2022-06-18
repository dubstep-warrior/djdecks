
#include "WaveformDisplay.h"

//==============================================================================

/**
 * Implementation of a constructor for WaveformDisplay
 *
 * Initializes data members and configure component details
 *
 */
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, juce::Colour _colour) : audioThumb(50, formatManagerToUse, cacheToUse), position(0), theme(_colour)
{
	audioThumb.addChangeListener(this);
}

/**
 * Implementation of a destructor for WaveformDisplay
 *
 */
WaveformDisplay::~WaveformDisplay()
{
}

//==============================================================================

/**
 * Implementation of getPosition method for WaveformDisplay
 *
 * Returns the position data member
 *
 */
double WaveformDisplay::getPosition() {
	return position;
}

/**
 * Implementation of isSliderDragged method for WaveformDisplay
 *
 * Returns the sliderIsDragged data member
 *
 */
bool WaveformDisplay::isSliderDragged() {
	return sliderIsDragged;
};

/**
 * Implementation of isFileLoaded method for WaveformDisplay
 *
 * Returns the isLoaded data member
 *
 */
bool WaveformDisplay::isFileLoaded() {
	return isLoaded;
};

//==============================================================================

/**
 * Implementation of loadTrack method for WaveformDisplay
 *
 * Calls loadURL using the track object's url data member
 * and stores the loaded song name
 *
 */
void WaveformDisplay::loadTrack(track track) {
	loadURL(track.url);
	if (isLoaded) {
		songNameLoaded = track.title;
	}
};

//==============================================================================

/**
 * Implementation of setPositionRelative method for WaveformDisplay
 *
 * Sets the relative position playhead of the component and repaints the component
 *
 */
void WaveformDisplay::setPositionRelative(double pos) {
	if (pos != position) {
		position = pos;
		repaint();
	}
}

/**
 * Implementation of setCuePoints method for WaveformDisplay
 *
 * Updates the cueTargets data member with std::pairs containing
 * the cue positions and cue colours.
 *
 */
void WaveformDisplay::setCuePoints(std::map<juce::TextButton*, std::pair<double, float>>& _cueTargets) {
	cueTargets.clear();
	std::map<juce::TextButton*, std::pair<double, float>>::iterator it;
	for (it = _cueTargets.begin(); it != _cueTargets.end(); it++)
	{
		cueTargets.push_back(&(it->second));
	}
	DBG("cueTargets size" << cueTargets.size());
};

//==============================================================================

/**
 * Implementation of paint method for WaveformDisplay
 *
 * Checks if track is loaded onto component.
 * Upon loading, calls the drawChannel method on audioThumb to
 * draw the waveform.
 * Cue point data containing time stamps are drawn on the component as
 * vertical lines.
 */
void WaveformDisplay::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
	g.setColour(juce::Colours::grey);
	g.drawRect(getLocalBounds(), 1);

	g.setColour(theme);
	if (isLoaded) {
		g.drawText(songNameLoaded, 5, 5, getWidth() * 3 / 4, 6, juce::Justification::left);
		audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 0.55);
		g.setColour(juce::Colours::lightgreen);
		g.drawRect(position * getWidth(), 0, 1, getHeight());

		if (mouseEntered) {
			g.setColour(juce::Colours::white);
			g.drawRect(prevX, 0, 1, getHeight());
		}

		for (auto i = 0; i < cueTargets.size(); ++i) {
			g.setColour(juce::Colour::fromHSL(cueTargets[i]->second, 1, 0.5, 1));
			g.drawRect(cueTargets[i]->first * getWidth(), 0, 1, getHeight());
		}
	}
	else {
		g.setFont(20.0f);
		g.drawText("File not loaded...", getLocalBounds(),
			juce::Justification::centred, true);
	}
}

/**
 * Implementation of resized method for WaveformDisplay
 *
 */
void WaveformDisplay::resized()
{
}

//==============================================================================

/**
 * Implementation of changeListenerCallback method for WaveformDisplay
 *
 * Calls repaint function to draw the component again.
 *
 */
void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source) {
	repaint();
}

//==============================================================================

/**
 * Implementation of mouseMove method for WaveformDisplay
 *
 * Sets the mouseEntered data member to true
 * Updates the prevX data member with the mouse X position
 * and calls repaint
 *
 */
void WaveformDisplay::mouseMove(const juce::MouseEvent& e) {
	mouseEntered = true;
	if (isEnabled() && prevX != e.x) {
		prevX = e.x;
		repaint();
	}
};

/**
 * Implementation of mouseExit method for WaveformDisplay
 *
 * Sets the mouseEntered data member to false.
 *
 */
void WaveformDisplay::mouseExit(const juce::MouseEvent& e) {
	mouseEntered = false;
};

/**
 * Implementation of mouseDown method for WaveformDisplay
 *
 * Sets it's slider value and relative position using the mouse X position
 * Sets sliderIsDragged to true
 */
void WaveformDisplay::mouseDown(const juce::MouseEvent& e) {
	if (isEnabled()) {
		sliderIsDragged = true;
		DBG("MOUSE DOWNED");
		setValue((double)e.x / (double)getWidth());
		setPositionRelative(getValue());
	}
}

/**
 * Implementation of mouseDrag method for WaveformDisplay
 *
 * Does the same thing as MouseDown. But updates the prevX data member
 *
 */
void WaveformDisplay::mouseDrag(const juce::MouseEvent& e) {
	if (isEnabled()) {
		prevX = e.x;
		mouseDown(e);
	}
}

/**
 * Implementation of mouseUp method for WaveformDisplay
 *
 * Sets sliderIsDragged to false
 *
 */
void WaveformDisplay::mouseUp(const juce::MouseEvent& e) {
	sliderIsDragged = false;
}

//==============================================================================

/**
 * Implementation of loadURL method for WaveformDisplay
 *
 * Calls the setSource method on the audioThumb and
 * clears all previous track data on data members.
 *
 */
void  WaveformDisplay::loadURL(juce::URL audioURL) {
	isLoaded = false;
	DBG("WaveformDispaly loadURL");
	audioThumb.clear();
	if (audioThumb.setSource(new juce::URLInputSource(audioURL))) {
		DBG("Successfully loaded wfd");
		isLoaded = true;
		setPositionRelative(0);
		cueTargets.clear();
	}
	else {
		DBG("Failed loaded wfd");
	}
}

//==============================================================================


