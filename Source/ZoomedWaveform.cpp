
#include "ZoomedWaveform.h"
//==============================================================================

/**
 * Implementation of a constructor for ZoomedWaveform
 *
 * Having inherited from WaveformDisplay, the passed in values are passed as arguments
 * into the WaveformDisplay constructor.
 *
 */
ZoomedWaveform::ZoomedWaveform(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, juce::Colour _colour) : WaveformDisplay(formatManagerToUse, cacheToUse, _colour)
{
}

/**
 * Implementation of a destructor for ZoomedWaveform
 *
 */
ZoomedWaveform::~ZoomedWaveform()
{
}

//==============================================================================

/**
 * Implementation of paint method for ZoomedWaveform
 *
 * Similar to WaveformDisplay, calls the drawChannel method on audioThumb to
 * draw the waveform.
 * However, the waveform drawn is zoomed in and instead of a moving playhead,
 * the drawn waveform moves against a fixed playhead in the middle.
 *
 */
void ZoomedWaveform::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colour::fromRGBA(0, 0, 0, 255));
	g.setColour(juce::Colours::grey);

	if (isLoaded) {
		double thisPos = position * audioThumb.getTotalLength();
		double half = audioThumb.getTotalLength() / 80;
		double left = thisPos - half;
		double right = thisPos + half;
		g.setColour(theme);
		audioThumb.drawChannel(g, getLocalBounds(), left, right, 0, .7);
		if (left < 0) {
			double widthRect = juce::jmap(fabs(left), (double)0, half * 2, (double)0, (double)getWidth());
			g.setColour(juce::Colour::fromRGBA(0, 0, 0, 255));
			g.fillRect(0.0f, 0.0f, (float)widthRect, (float)getHeight() - 1);   // draw an outline around the component
		}

		for (auto i = 0; i < cueTargets.size(); ++i) {
			if ((cueTargets[i]->first * audioThumb.getTotalLength()) > left && (cueTargets[i]->first * audioThumb.getTotalLength()) < right) {
				g.setColour(juce::Colour::fromHSL(cueTargets[i]->second, 1, 0.5, 1));
				double widthPos = juce::jmap(cueTargets[i]->first * audioThumb.getTotalLength(), left, right, (double)0, (double)getWidth());
				g.drawRect(widthPos, 0, 1, getHeight());
			}
		}
		g.setColour(juce::Colours::grey);
		g.drawRect(getWidth() / 2, 0, 1, getHeight());
	}
}

/**
 * Implementation of resized method for ZoomedWaveform
 *
 */
void ZoomedWaveform::resized() {}

//==============================================================================

/**
 * Implementation of mouseDown method for ZoomedWaveform
 *
 * Overrides WaveformDisplay::mouseDown method with an empty
 * implementation.
 *
 */
void ZoomedWaveform::mouseDown(const juce::MouseEvent& e) {}

/**
 * Implementation of mouseDrag method for ZoomedWaveform
 *
 * The current X position of the mouse is compared to the previous recorded X position
 * to determine if the playhead's value should move forward or backwards.
 *
 */
void ZoomedWaveform::mouseDrag(const juce::MouseEvent& e) {
	if (isEnabled()) {
		sliderIsDragged = true;
		DBG("MOUSE DRAGGED :: Zoomed");
		if ((double)prevX > (double)e.x) {
			setValue(position + 0.1 / audioThumb.getTotalLength());
		}
		else if ((double)prevX < (double)e.x) {
			setValue(position - 0.1 / audioThumb.getTotalLength());
		}
		prevX = e.x;
		setPositionRelative(getValue());
	}
}

//==============================================================================




