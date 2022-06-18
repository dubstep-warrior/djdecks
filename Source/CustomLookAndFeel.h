
#include <JuceHeader.h>

#pragma once

/**
 * Definition of a custom LookAndFeel class template
 *
 * Every graphics component has a LookAndFeel configured, usually configured
 * to the standard juce::LookAndFeel_V4 instance, which determines the
 * appearance of the component. The component calls on it's LookAndFeel
 * methods to draw the component, and overriding inherited methods of
 * juce::LookAndFeel_V4 allows this LookAndFeel class to provide custom
 * appearance to certain components. The purpose of this CustomLookAndFeel
 * class is to customize the appearance of default sliders and table headers.
 * Using an instance of this class, components can set this instance as it's LookAndFeel configuration.
 *
 */
class CustomLookAndFeel : public juce::LookAndFeel_V4 {
public:

	//============================================================================== 

	/**
		* Class constructor for CustomLookAndFeel. Uses the constructor to parse svg images in the Assets folders into a unique pointer of xml element, and then creating a juce::Drawable from it.
		* Inherits LookAndFeel methods from juce::LookAndFeel_V4
	*/
	CustomLookAndFeel();

	//==============================================================================

	/**
		* Overrides LookAndFeelV4::drawLinearSlider to draw a standard linear slider on the graphics object
		*
		* @param The graphics object
		* @param x position of the component
		* @param y position of the component
		* @param Width of the component
		* @param Height of the component
		* @param Knob position of the slider
		* @param Minimum knob position of the slider
		* @param Maximum knob position of the slider
		* @param SliderStyle enum value
		* @param Slider object to be drawn
	*/
	void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider&) override;

	/**
		* Overrides LookAndFeelV4::drawRotarySlider to draw a standard rotary slider on the graphics object
		*
		* @param The graphics object
		* @param x position of the component
		* @param y position of the component
		* @param Width of the component
		* @param Height of the component
		* @param Knob position of the slider
		* @param Starting angle of the knob rotation
		* @param Ending angle of the knob rotation
		* @param Slider object to be drawn
	*/
	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;

	/**
		* Overrides LookAndFeelV4::drawTableHeaderBackground to draw a Table Header background on the graphics object
		*
		* @param The graphics object
		* @param TableHeaderComponent to be drawn
	*/
	void drawTableHeaderBackground(juce::Graphics& g, juce::TableHeaderComponent& header) override;

	//==============================================================================

private:

	/** Unique Pointer to a juce::Drawable, to store svg image of a vertical slider knob */
	std::unique_ptr<juce::Drawable> verticalSliderKnobImage;

	/** Unique Pointer to a juce::Drawable, to store svg image of a horizontal slider knob */
	std::unique_ptr<juce::Drawable> horizontalSliderKnobImage;

};