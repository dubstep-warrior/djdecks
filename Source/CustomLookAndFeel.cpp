
#include "CustomLookAndFeel.h"

//============================================================================== 


/**
 * Implementation of a constructor for CustomLookAndFeel
 *
 * In the constructor, binary data of svg assets are parsed in xml elements
 * and further parse into juce::Drawable members to define the appearance of
 * default sliders.
 *
 */
CustomLookAndFeel::CustomLookAndFeel() {
	const std::unique_ptr<juce::XmlElement> verticalSliderKnob_xml(juce::XmlDocument::parse(BinaryData::verticalKnob_svg));
	verticalSliderKnobImage = juce::Drawable::createFromSVG(*verticalSliderKnob_xml);;
	const std::unique_ptr<juce::XmlElement> horizontalSliderKnob_xml(juce::XmlDocument::parse(BinaryData::horizontalKnob_svg));
	horizontalSliderKnobImage = juce::Drawable::createFromSVG(*horizontalSliderKnob_xml);;
};

//============================================================================== 


/**
 * Implementation of a drawLinearSlider for CustomLookAndFeel
 *
 * Implementation is similar to drawLinearSlider for LookAndFeel_V4,
 * except the knob is drawn using the appropiate juce::Drawable knob,
 * and that the perpendicular lines are drawn on the slider at specific intervals.
 *
 */
void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) {
	if (slider.isBar())
	{
		g.setColour(slider.findColour(juce::Slider::trackColourId));
		g.fillRect(slider.isHorizontal() ? juce::Rectangle<float>(static_cast<float> (x), (float)y + 0.5f, sliderPos - (float)x, (float)height - 1.0f)
			: juce::Rectangle<float>((float)x + 0.5f, sliderPos, (float)width - 1.0f, (float)y + ((float)height - sliderPos)));
	}
	else
	{
		auto isTwoVal = (style == juce::Slider::SliderStyle::TwoValueVertical || style == juce::Slider::SliderStyle::TwoValueHorizontal);
		auto isThreeVal = (style == juce::Slider::SliderStyle::ThreeValueVertical || style == juce::Slider::SliderStyle::ThreeValueHorizontal);

		auto trackWidth = juce::jmin(1.5f, slider.isHorizontal() ? (float)height * 0.25f : (float)width * 0.05f);

		juce::Point<float> startPoint(slider.isHorizontal() ? (float)x : (float)x + (float)width * 0.5f,
			slider.isHorizontal() ? (float)y + (float)height * 0.5f : (float)(height + y));

		juce::Point<float> endPoint(slider.isHorizontal() ? (float)(width + x) : startPoint.x,
			slider.isHorizontal() ? startPoint.y : (float)y);

		juce::Path backgroundTrack;
		backgroundTrack.startNewSubPath(startPoint);
		backgroundTrack.lineTo(endPoint);
		g.setColour(juce::Colour::fromRGBA(255, 255, 255, 255));
		g.strokePath(backgroundTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

		juce::Path valueTrack;
		juce::Point<float> minPoint, maxPoint, thumbPoint;

		if (isTwoVal || isThreeVal)
		{
			minPoint = { slider.isHorizontal() ? minSliderPos : (float)width * 0.5f,
						 slider.isHorizontal() ? (float)height * 0.5f : minSliderPos };

			if (isThreeVal)
				thumbPoint = { slider.isHorizontal() ? sliderPos : (float)width * 0.5f,
							   slider.isHorizontal() ? (float)height * 0.5f : sliderPos };

			maxPoint = { slider.isHorizontal() ? maxSliderPos : (float)width * 0.5f,
						 slider.isHorizontal() ? (float)height * 0.5f : maxSliderPos };
		}
		else
		{
			auto kx = slider.isHorizontal() ? sliderPos : ((float)x + (float)width * 0.5f);
			auto ky = slider.isHorizontal() ? ((float)y + (float)height * 0.5f) : sliderPos;

			minPoint = startPoint;
			maxPoint = { kx, ky };
		}

		auto thumbWidth = getSliderThumbRadius(slider);


		valueTrack.startNewSubPath(minPoint);
		valueTrack.lineTo(isThreeVal ? thumbPoint : maxPoint);
		g.setColour(juce::Colour::fromRGBA(255, 255, 255, 255));
		g.strokePath(valueTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

		if (!isTwoVal)
		{

			if (slider.isHorizontal()) {
				for (float i = x; i < x + width + 3; i += (width + 3) / 5) {
					g.drawLine(i, startPoint.getY() - height / 3, i, endPoint.getY() + height / 3);
				}
				horizontalSliderKnobImage->drawWithin(g, juce::Rectangle<float>(static_cast<float> (thumbWidth * 2.5), static_cast<float> (thumbWidth * 2.5)).withCentre(isThreeVal ? thumbPoint : maxPoint), juce::RectanglePlacement::centred, 1.0);
			}
			else {
				for (float i = y; i < y + height + 4; i += (height + 4) / 5) {
					g.drawLine(startPoint.getX() - trackWidth * 6, i, endPoint.getX() + trackWidth * 6, i);
				}
				g.setColour(juce::Colours::white);
				g.fillRect(juce::Rectangle<float>(static_cast<float> (thumbWidth * 1.2), static_cast<float> (thumbWidth * 2)).withCentre(isThreeVal ? thumbPoint : maxPoint));
				verticalSliderKnobImage->drawWithin(g, juce::Rectangle<float>(static_cast<float> (thumbWidth * 2.5), static_cast<float> (thumbWidth * 2.5)).withCentre(isThreeVal ? thumbPoint : maxPoint), juce::RectanglePlacement::centred, 1.0);
			}
		}

		if (isTwoVal || isThreeVal)
		{
			auto sr = juce::jmin(trackWidth, (slider.isHorizontal() ? (float)height : (float)width) * 0.4f);
			auto pointerColour = slider.findColour(juce::Slider::thumbColourId);

			if (slider.isHorizontal())
			{
				drawPointer(g, minSliderPos - sr,
					juce::jmax(0.0f, (float)y + (float)height * 0.5f - trackWidth * 2.0f),
					trackWidth * 2.0f, pointerColour, 2);

				drawPointer(g, maxSliderPos - trackWidth,
					juce::jmin((float)(y + height) - trackWidth * 2.0f, (float)y + (float)height * 0.5f),
					trackWidth * 2.0f, pointerColour, 4);
			}
			else
			{

				drawPointer(g, juce::jmax(0.0f, (float)x + (float)width * 0.5f - trackWidth * 2.0f),
					minSliderPos - trackWidth,
					trackWidth * 2.0f, pointerColour, 1);

				drawPointer(g, juce::jmin((float)(x + width) - trackWidth * 2.0f, (float)x + (float)width * 0.5f), maxSliderPos - sr,
					trackWidth * 2.0f, pointerColour, 3);
			}
		}
	}

}


/**
 * Implementation of a drawRotarySlider for CustomLookAndFeel
 *
 * Implementation is similar to drawRotarySlider for LookAndFeel_V4,
 * except the knob is a circle that covers the entire slider, and the
 * knob marker is drawn using the same angle calculations.
 *
 */
void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
	auto outline = slider.findColour(juce::Slider::rotarySliderOutlineColourId);
	auto fill = slider.findColour(juce::Slider::rotarySliderFillColourId);
	auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10);
	auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
	auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
	auto lineW = juce::jmin(8.0f, radius * 0.5f);
	auto arcRadius = radius - lineW * 0.5f;
	auto thumbWidth = radius * 2.0f;

	juce::Point<float> endPoint(bounds.getCentreX() + arcRadius * std::cos(toAngle - juce::MathConstants<float>::halfPi),
		bounds.getCentreY() + arcRadius * std::sin(toAngle - juce::MathConstants<float>::halfPi));
	juce::Point<float> thumbPoint(x + width / 2, y + height / 2);

	int count = 0;
	for (auto i = rotaryStartAngle; i < rotaryEndAngle; i += (rotaryEndAngle - rotaryStartAngle) / 10) {
		juce::Point<float> start(bounds.getCentreX() + arcRadius * 1.5 * std::cos(i - juce::MathConstants<float>::halfPi),
			bounds.getCentreY() + arcRadius * 1.7 * std::sin(i - juce::MathConstants<float>::halfPi));
		float scale = 2.0;
		juce::Point<float> end(bounds.getCentreX() + arcRadius * scale * std::cos(i - juce::MathConstants<float>::halfPi),
			bounds.getCentreY() + arcRadius * scale * std::sin(i - juce::MathConstants<float>::halfPi));
		juce::Path marking;
		marking.startNewSubPath(start);
		marking.lineTo(end);
		g.setColour(juce::Colour::fromRGBA(255, 255, 255, 255));

		count % 5 == 0 ? g.strokePath(marking, juce::PathStrokeType(2)) : g.strokePath(marking, juce::PathStrokeType(1));
		count++;
	}

	juce::Path pointer;
	pointer.startNewSubPath(thumbPoint);
	pointer.lineTo(endPoint);
	g.setColour(juce::Colour::fromRGBA(25, 25, 25, 255));
	g.fillEllipse(juce::Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint));
	g.setColour(juce::Colour::fromRGBA(255, 255, 255, 255));
	g.strokePath(pointer, juce::PathStrokeType(3));
}


/**
 * Implementation of a drawRotarySlider for CustomLookAndFeel
 *
 * Implementation is similar to drawRotarySlider for LookAndFeel_V4,
 * except that the background colour is changed.
 *
 */
void CustomLookAndFeel::drawTableHeaderBackground(juce::Graphics& g, juce::TableHeaderComponent& header)
{
	auto r = header.getLocalBounds();
	auto outlineColour = header.findColour(juce::TableHeaderComponent::outlineColourId);
	g.setColour(outlineColour);
	g.fillRect(r.removeFromBottom(1));
	g.setColour(juce::Colour::fromRGBA(200, 200, 200, 255));
	g.fillRect(r);
	g.setColour(outlineColour);
	for (int i = header.getNumColumns(true); --i >= 0;)
		g.fillRect(header.getColumnPosition(i).removeFromRight(1));
}

//============================================================================== 


