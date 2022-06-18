
#define _USE_MATH_DEFINES
#include "JogWheel.h" 
//==============================================================================

/**
 * Implementation of a constructor for JogWheel
 *
 * Having inherited from ZoomWaveform, the passed in values are passed as arguments
 * into the ZoomWaveform constructor.
 *
 */
JogWheel::JogWheel(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, juce::Colour _colour) : ZoomedWaveform(formatManagerToUse, cacheToUse, _colour)
{
}

/**
 * Implementation of a destructor for JogWheel
 *
 */
JogWheel::~JogWheel()
{
}

//==============================================================================

/**
 * Implementation of paint method for JogWheel
 *
 * The number of total rotations is defined here, with the angle position
 * of the current playhead determined by mapping the current position of
 * the song to an angle between 0 and the number of rotations multiplied
 * by 360.
 * With the current angle position, a line is drawn from the middle to the
 * edge of the component, creating a playhead for the component.
 * The current time is also drawn on the component when a file is loaded
 */
void JogWheel::paint(juce::Graphics& g)
{
	g.setColour(juce::Colours::darkslategrey);
	g.fillEllipse(2, 2, getWidth() - 4, getHeight() - 4);

	g.setColour(theme);
	noRotations = audioThumb.getTotalLength() / 2;
	float angle = getPosition() * 360 * noRotations;
	float piAngle = angle * M_PI / 180;

	startPoint.x = getWidth() / 2;
	startPoint.y = getHeight() / 2;
	line.setStart(startPoint);
	endPoint.x = (getWidth() / 2) + (getWidth() / 2 - 4) * std::cos(piAngle);
	endPoint.y = (getHeight() / 2) + (getHeight() / 2 - 4) * std::sin(piAngle);
	line.setEnd(endPoint);
	g.drawLine(line, 8);

	g.setColour(juce::Colours::black);
	g.fillEllipse(10, 10, getWidth() - 20, getHeight() - 20);
	g.setColour(juce::Colours::white);
	g.drawEllipse(10, 10, getWidth() - 20, getHeight() - 20, 1.5);

	if (isLoaded) {
		std::string time = track::getLengthString(position * audioThumb.getTotalLength(), true);
		juce::Rectangle<float> rect(0, getHeight() / 2 - 10, getWidth(), 10);
		g.drawText(time, rect, juce::Justification::centred);
	}
}

/**
 * Implementation of resized method for JogWheel
 *
 */
void JogWheel::resized()
{
}

//==============================================================================

/**
 * Implementation of mouseDrag method for JogWheel
 *
 * The current angle of the mouse is compared to the previous recorded angle
 * to determine if the playhead's value should move forward or backwards.
 *
 */
void JogWheel::mouseDrag(const juce::MouseEvent& e) {
	juce::Point<double> centre(getWidth() / 2, getHeight() / 2);
	juce::Point<double> currentPoint(e.x, e.y);
	juce::Point<double> prevPoint(prevX, prevY);
	if (isEnabled()) {
		sliderIsDragged = true;
		DBG("MOUSE DRAGGED :: jog");
		if (centre.getAngleToPoint(currentPoint) - centre.getAngleToPoint(prevPoint) > 0) {
			setValue(position + 0.1 / audioThumb.getTotalLength());
		}
		else if (centre.getAngleToPoint(currentPoint) - centre.getAngleToPoint(prevPoint) < 0) {
			setValue(position - 0.1 / audioThumb.getTotalLength());
		}
		prevX = e.x;
		prevY = e.y;
		setPositionRelative(getValue());
	}
};

//==============================================================================
