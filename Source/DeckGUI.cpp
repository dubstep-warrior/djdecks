
#include "DeckGUI.h"

//==============================================================================

/**
 * Implementation of a constructor for DeckGUI
 *
 * In the constructor, binary data of svg assets are parsed in xml elements
 * and further parse into juce::Drawable members to define the appearance of
 * button components. Private data members are being initialized with hard values
 * or passed in references. Initial component configurations are performed here as
 * well
 *
 */
DeckGUI::DeckGUI(DJAudioPlayer* _player, juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, ZoomedWaveform* _zoomedDisplay, Library& _library, juce::Colour _colour) : player(_player), waveformDisplay(formatManagerToUse, cacheToUse, _colour), zoomedDisplay(_zoomedDisplay), jogWheel(formatManagerToUse, cacheToUse, _colour), library(&_library), theme(_colour)
{
	std::vector<juce::Label*> labels{ &volLabel, &speedLabel, &filterLabel, &lbLabel, &mbLabel, &hbLabel };
	for (auto& label : labels) {
		label->setEditable(false);
		label->setJustificationType(juce::Justification::centred);
		addAndMakeVisible(*label);
	}

	addAndMakeVisible(playButton);
	addAndMakeVisible(volSlider);
	addAndMakeVisible(speedSlider);
	addAndMakeVisible(loadButton);
	addAndMakeVisible(waveformDisplay);
	addAndMakeVisible(jogWheel);
	addAndMakeVisible(filter);
	addAndMakeVisible(lowBandFilter);
	addAndMakeVisible(midBandFilter);
	addAndMakeVisible(highBandFilter);

	volSlider.setRange(0, 1);
	speedSlider.setRange(0.8, 1.2);
	filter.setRange(-20000, 20000);
	lowBandFilter.setRange(0.01, 2);
	midBandFilter.setRange(0.01, 2);
	highBandFilter.setRange(0.01, 2);
	waveformDisplay.setRange(0, 1);
	zoomedDisplay->setRange(0, 1);
	jogWheel.setRange(0, 1);

	filter.setValue(0);
	lowBandFilter.setValue(1);
	midBandFilter.setValue(1);
	highBandFilter.setValue(1);
	volSlider.setValue(0.5);
	speedSlider.setValue(1);

	playButton.addListener(this);
	loadButton.addListener(this);
	volSlider.addListener(this);
	speedSlider.addListener(this);

	filter.addListener(this);
	lowBandFilter.addListener(this);
	midBandFilter.addListener(this);
	highBandFilter.addListener(this);

	startTimer(20);

	for (auto i = 0; i < 6; ++i) {
		cues.push_back(new juce::TextButton());
	}
	for (auto& cue : cues) {
		addAndMakeVisible(cue);
		cue->addListener(this);
	}

	const std::unique_ptr<juce::XmlElement> playButton_xml(juce::XmlDocument::parse(BinaryData::playButton_svg));
	const std::unique_ptr<juce::XmlElement> playButtonHover_xml(juce::XmlDocument::parse(BinaryData::playButtonHover_svg));
	const std::unique_ptr<juce::XmlElement> stopButton_xml(juce::XmlDocument::parse(BinaryData::pauseButton_svg));
	const std::unique_ptr<juce::XmlElement> stopButtonHover_xml(juce::XmlDocument::parse(BinaryData::pauseButtonHover_svg));
	const std::unique_ptr<juce::XmlElement> loadButton_xml(juce::XmlDocument::parse(BinaryData::loadButton_svg));
	const std::unique_ptr<juce::XmlElement> loadButtonHover_xml(juce::XmlDocument::parse(BinaryData::loadButtonHover_svg));
	playButtonImage = juce::Drawable::createFromSVG(*playButton_xml);
	playButtonHoverImage = juce::Drawable::createFromSVG(*playButtonHover_xml);
	stopButtonImage = juce::Drawable::createFromSVG(*stopButton_xml);
	stopButtonHoverImage = juce::Drawable::createFromSVG(*stopButtonHover_xml);
	loadButtonImage = juce::Drawable::createFromSVG(*loadButton_xml);
	loadButtonHoverImage = juce::Drawable::createFromSVG(*loadButtonHover_xml);

	playButton.setImages(playButtonImage.get(),
		playButtonHoverImage.get(),
		nullptr,
		nullptr,
		stopButtonImage.get(),
		stopButtonHoverImage.get(),
		nullptr,
		nullptr);
	loadButton.setImages(loadButtonImage.get(), loadButtonHoverImage.get());
	playButton.setClickingTogglesState(true);
	playButton.setEdgeIndent(0);
	loadButton.setEdgeIndent(0);

	volSlider.setLookAndFeel(&customLookAndFeel);
	speedSlider.setLookAndFeel(&customLookAndFeel);
	filter.setLookAndFeel(&customLookAndFeel);
	lowBandFilter.setLookAndFeel(&customLookAndFeel);
	midBandFilter.setLookAndFeel(&customLookAndFeel);
	highBandFilter.setLookAndFeel(&customLookAndFeel);
}

/**
 * Implementation of a destructor for DeckGUI
 *
 * DeckGUI instance calls it's timer to stop and free dynamically allocated juce::TextButton objects in cues
 *
 */
DeckGUI::~DeckGUI()
{
	stopTimer();
	for (auto& cue : cues) {
		delete cue;
	}
}

//============================================================================== 

/**
 * Implementation of paint method for DeckGUI
 *
 * Cue button colours are being set and the volume meter is drawn based on the volRMS value.
 *
 */
void DeckGUI::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colour::fromRGBA(50, 50, 50, 255));

	double rowH = getHeight() / 9;
	float offset = rowH * 2.23;
	float volMeterHeight = rowH * 2.5;
	float volCurrentHeight = juce::jmap(player->getRMSLevel(), -60.0f, 0.0f, offset + volMeterHeight - 5, offset);

	for (auto i = offset + volMeterHeight - 5; i > offset; i -= volMeterHeight / 10) {
		float pos = i;
		float redStrength = juce::jmap(pos, offset + volMeterHeight - 5, offset, 0.0f, 255.0f);

		juce::Colour colorRGB(redStrength, 255 - redStrength, 0);
		g.setColour(colorRGB);

		if (volCurrentHeight < pos) {
			g.setColour(colorRGB);
		}
		else {
			g.setColour(juce::Colour::fromRGBA(25, 25, 25, 255));
		}

		double volXOffset = theme == juce::Colours::hotpink ? 62.5 : getWidth() - (double)75;

		juce::Rectangle<float> rect(volXOffset, pos, 12.5, (volMeterHeight / 10) - 2);
		g.fillRect(rect);
	}

	for (auto& cue : cues) {
		juce::TextButton* thisButton = cue;
		if (cueTargets.find(thisButton) != cueTargets.end() && flash) {
			thisButton->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromHSL(cueTargets[thisButton].second, (float)1, (float)0.5, (float)1));
		}
		else {
			thisButton->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour::fromRGBA(25, 25, 25, 255));
		}
	}

	double mainXOffset = theme == juce::Colours::hotpink ? getWidth() * 7 / 32 : getWidth() * 25 / 32;
	g.setColour(juce::Colour::fromRGBA(25, 25, 25, 255));
	g.drawLine(mainXOffset, 0, mainXOffset, getHeight());
}

/**
 * Implementation of resized method for DeckGUI
 *
 * All juce::Component data members call it's setBounds method to achieve uniform space and sizing.
 *
 */
void DeckGUI::resized()
{
	double rowH = getHeight() / 9;
	double volXOffset = theme == juce::Colours::hotpink ? 5.5 : getWidth() - (double)55;
	volSlider.setBounds(volXOffset, rowH * 2, 50, rowH * 3);
	volLabel.setBounds(volXOffset, rowH * 5 + 5, 50, rowH * 0.5);
	filter.setBounds(volXOffset, rowH * 5.8, 50, 50);
	filterLabel.setBounds(volXOffset, rowH * 6.9, 50, 50);
	double mainXOffset = theme == juce::Colours::hotpink ? getWidth() * 7 / 32 : 0;
	speedSlider.setBounds(mainXOffset, rowH * 2, getWidth() / 8, rowH * 3);
	speedLabel.setBounds(mainXOffset, rowH * 5 + 5, getWidth() / 8, rowH * 0.5);
	jogWheel.setBounds(mainXOffset + getWidth() * 22.5 / 32 - 98.9, 5 + rowH * 2, (rowH * 3.3) - 10, (rowH * 3.3) - 10);
	loadButton.setBounds(mainXOffset + getWidth() * 22.5 / 32, rowH * 2 + 5, rowH * 0.7, rowH * 0.7);
	playButton.setBounds(mainXOffset + getWidth() * 22.5 / 32, rowH * 5 - 10, rowH * 0.7, rowH * 0.7);

	waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);

	double xOffset = mainXOffset + getWidth() * 4 / 32;
	double yOffset = 5 + rowH * 2;
	double cellLength = (getWidth() * 18.5 / 32 - 105) / 3;
	double cellHeight = 44.45;
	for (auto i = 0; i < 3; ++i) {
		for (auto j = 0; j < 2; ++j) {
			int index = i * 2 + j;
			cues[index]->setBounds(i * cellLength + xOffset, j * cellHeight + 4 + yOffset, cellLength - 4, cellHeight - 4);
		}
	}

	lowBandFilter.setBounds(xOffset, rowH * 5.8, 50, 50);
	midBandFilter.setBounds(xOffset + getWidth() / 5, rowH * 5.8, 50, 50);
	highBandFilter.setBounds(xOffset + getWidth() * 2 / 5, rowH * 5.8, 50, 50);
	lbLabel.setBounds(xOffset, rowH * 6.9, 50, 50);
	mbLabel.setBounds(xOffset + getWidth() / 5, rowH * 6.9, 50, 50);
	hbLabel.setBounds(xOffset + getWidth() * 2 / 5, rowH * 6.9, 50, 50);
}

//============================================================================== 

/**
 * Implementation of buttonClicked method for DeckGUI
 *
 * All juce::Button data members are compared to the triggered juce::Button pointer.
 * Based on which juce::Button data member it is, calls either a specific method in
 * the DJAudioPlayer instance, its own methods or component methods.
 *
 */
void DeckGUI::buttonClicked(juce::Button* button) {

	if (button == &playButton) {
		DBG("MainComponent::buttonClicked: They clicked the play button");
		modeIsPlaying = !modeIsPlaying;
		playButton.setButtonStyle(juce::DrawableButton::ButtonStyle::ImageFitted);
	}


	if (button == &loadButton && library->selectionIsValid()) {
		loadDeck(library->getSelectedTrack());
	}

	if (player->isLoaded()) {
		for (auto& cue : cues) {
			juce::TextButton* thisButton = cue;
			if (button == thisButton) {
				if (cueTargets.find(thisButton) != cueTargets.end()) {
					player->setPositionRelative(cueTargets[thisButton].first);
					if (!modeIsPlaying) {
						modeIsPlaying = true;
						playButton.setToggleState(true, juce::NotificationType::dontSendNotification);
					}
				}
				else {
					cueTargets[thisButton] = std::make_pair(player->getPositionRelative(), static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
					waveformDisplay.setCuePoints(cueTargets);
					zoomedDisplay->setCuePoints(cueTargets);
				}
			}
		}
	}

	modeIsPlaying ? player->start() : player->stop();
};

//============================================================================== 

/**
 * Implementation of sliderValueChanged method for DeckGUI
 *
 * All juce::Slider data members are compared to the triggered juce::Slider pointer.
 * Based on which juce::Slider data member it is, calls a specific method in the
 * DJAudioPlayer instance.
 *
 */
void DeckGUI::sliderValueChanged(juce::Slider* slider) {

	if (slider == &volSlider) {
		DBG("MainComponent::sliderValueChanged: They change the volume slider " << slider->getValue());
		player->setGain(slider->getValue());
	}

	if (slider == &speedSlider) {
		DBG("MainComponent::sliderValueChanged: They change the speed slider " << slider->getValue());
		player->setSpeed(slider->getValue());
	}

	if (slider == &filter) {
		DBG("MainComponent::sliderValueChanged: They change the filter slider " << slider->getValue());
		player->setFilter(slider->getValue());
	}

	if (slider == &lowBandFilter) {
		DBG("MainComponent::sliderValueChanged: They change the LB slider " << slider->getValue());
		player->setLBFilter(slider->getValue());
	}

	if (slider == &midBandFilter) {
		DBG("MainComponent::sliderValueChanged: They change the MB slider " << slider->getValue());
		player->setMBFilter(slider->getValue());
	}

	if (slider == &highBandFilter) {
		DBG("MainComponent::sliderValueChanged: They change the HB slider " << slider->getValue());
		player->setHBFilter(slider->getValue());
	}
};

//============================================================================== 

/**
 * Implementation of isInterestedInFileDrag method for DeckGUI
 *
 * returns true
 *
 */
bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files) {
	return true;
};


/**
 * Implementation of filesDropped method for DeckGUI
 *
 * Checks if the files array is of size 1. Converts the files element
 * into a juce::File object, into a track object before loading the deck
 * with the track object.
 *
 */
void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y) {
	DBG("DeckGUI::filesDropped");
	if (files.size() == 1 && x < getWidth() && y < getHeight()) {
		juce::File file = juce::File{ files[0] };
		track track{ file.getFileNameWithoutExtension(), 0, juce::URL{ file } };
		loadDeck(track);
	}
};

//============================================================================== 

/**
 * Implementation of timerCallback method for DeckGUI
 *
 * Continuously update any WaveformDisplay objects from the player's position.
 * Check if any WaveformDisplay objects' playback control is triggered, and setting
 * the DJAudioPlayer instance's playback with the triggered playback control value.
 * This is also where the DJAudioPlayer instance's root mean square value is updated.
 *
 */
void DeckGUI::timerCallback() {
	counter++;
	if (counter % 10 == 0) {
		flash = !flash;
		repaint();
	}

	for (auto i = 0; i < displays.size(); ++i) {
		if (displays[i]->isFileLoaded()) {
			double pos = displays[i]->getValue();
			if (displays[i]->isSliderDragged()) {
				draggedIndex = i;
				canContinue = false;
				if (displays[i] == &waveformDisplay) {
					player->stop();
				}
				else {
					if (prevPlayerPos == pos) {
						player->stop();
					}
					else {
						!player->isPlaying() ? player->start() : NULL;
					}
				}
				player->setPositionRelative(pos);
				prevPlayerPos = pos;
			}
			else if (canContinue == false && !(displays[i]->isSliderDragged()) && draggedIndex == i) {
				DBG("YESSSS " << (displays[i]->isSliderDragged() ? "true" : "false"));
				modeIsPlaying ? player->start() : player->stop();
				canContinue = true;
				draggedIndex = -1;
			}
			else {
				displays[i]->setPositionRelative(player->getPositionRelative());
			}
		}
	}

	if (volRMS != player->getRMSLevel()) {
		volRMS = player->getRMSLevel();
		repaint();
	}
}

//============================================================================== 

/**
 * Implementation of loadDeck method for DeckGUI
 *
 * Loads the player with the track object.
 * Loads all WaveformDisplay objects with the track object.
 * Cue point data from previously loaded tracks are cleared.
 *
 */
void DeckGUI::loadDeck(track track) {
	player->loadURL(track.url);
	if (player->isLoaded()) {
		for (auto& display : displays) {
			display->loadTrack(track);
			display->addListener(this);
		}
	}

	player->setGain(volSlider.getValue(), true);
	cueTargets.clear();

	if (modeIsPlaying) {
		playButton.setToggleState(true, juce::NotificationType::dontSendNotification);
		player->start();
	}
	else {
		playButton.setToggleState(false, juce::NotificationType::dontSendNotification);
	}
};

//============================================================================== 
