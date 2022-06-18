
#include "DJAudioPlayer.h"

//============================================================================== 

/**
 * Implementation of a constructor for DJAudioPlayer
 *
 * Initializes juce::AudioFormatManager pointer data member
 *
 */
DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
	: formatManager(_formatManager)
{
};

/**
 * Implementation of a destructor for DJAudioPlayer
 *
 */
DJAudioPlayer::~DJAudioPlayer() {};

//==============================================================================

/**
 * Implementation of prepareToPlay method for DJAudioPlayer
 *
 * Calls prepareToPlay methods on all AudioSource data members and saves the sample rate
 *
 */
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	audioLPFilter.prepareToPlay(samplesPerBlockExpected, sampleRate);
	audioHPFilter.prepareToPlay(samplesPerBlockExpected, sampleRate);
	audioLBFilter.prepareToPlay(samplesPerBlockExpected, sampleRate);
	audioMBFilter.prepareToPlay(samplesPerBlockExpected, sampleRate);
	audioHBFilter.prepareToPlay(samplesPerBlockExpected, sampleRate);
	thisSampleRate = sampleRate;
};

/**
 * Implementation of getNextAudioBlock method for DJAudioPlayer
 *
 * Calls getNextAudioBlock methods on the main AudioSource data member and updates the root mean square value
 *
 */
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
	audioLPFilter.getNextAudioBlock(bufferToFill);
	float rmsLevelLeft = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(0, 0, bufferToFill.buffer->getNumSamples()));
	float rmsLevelRight = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(1, 0, bufferToFill.buffer->getNumSamples()));
	level = (rmsLevelLeft + rmsLevelRight) / 2;
};

/**
 * Implementation of releaseResources method for DJAudioPlayer
 *
 * Calls releaseResources methods on the main AudioSource data member
 *
 */
void DJAudioPlayer::releaseResources() {
	audioLPFilter.releaseResources();
};

//============================================================================== 

/**
 * Implementation of start method for DJAudioPlayer
 *
 *  Calls the start method on the AudioTransportSource data member
 *
 */
void DJAudioPlayer::start() {
	transportSource.start();
};

/**
 * Implementation of stop method for DJAudioPlayer
 *
 *  Calls the stop method on the AudioTransportSource data member
 *
 */
void DJAudioPlayer::stop() {
	transportSource.stop();
};

/**
 * Implementation of isPlaying method for DJAudioPlayer
 *
 * Returns if the AudioTransportSource data member is playing
 *
 */
bool DJAudioPlayer::isPlaying() {
	return transportSource.isPlaying();
}

/**
 * Implementation of isLoaded method for DJAudioPlayer
 *
 * Returns the loaded data member
 *
 */
bool DJAudioPlayer::isLoaded() {
	return loaded;
}

/**
 * Implementation of returnURL method for DJAudioPlayer
 *
 * Returns the currentAudioURL data member
 *
 */
juce::URL DJAudioPlayer::returnURL() {
	return currentAudioURL;
}

/**
 * Implementation of loadURL method for DJAudioPlayer
 *
 * Creates a reader for the juce::URL and parses it into a juce::AudioFormatReaderSource
 * The AudioTransportSource data member sets it source using the juce::AudioFormatReaderSource
 *
 */
void DJAudioPlayer::loadURL(juce::URL audioURL) {
	auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
	if (reader != nullptr) {
		std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
		transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		readerSource.reset(newSource.release());
		DBG("real metadata size: " << reader->metadataValues.size());
		loadedFileName = audioURL.getFileName();
		loaded = true;
		currentAudioURL = audioURL;
	}
	else
	{
		DBG("Something went wrong loading the file ");
		loaded = false;
	}
};

//==============================================================================

/**
 * Implementation of getRMSLevel method for DJAudioPlayer
 *
 * Returns the level data member
 *
 */
float DJAudioPlayer::getRMSLevel() {
	return level;
};

/**
 * Implementation of getPositionRelative method for DJAudioPlayer
 *
 * Returns the relative position of the AudioTransportSource data member.
 * Value returned is between 0 and 1.
 *
 */
double DJAudioPlayer::getPositionRelative() {
	return (transportSource.getLengthInSeconds() == 0 ? 0 : transportSource.getCurrentPosition() / transportSource.getLengthInSeconds());
}

//==============================================================================

/**
 * Implementation of setGain method for DJAudioPlayer
 *
 * Checks if gain value passed in is called from a volume
 * functionality before setting the playerVolume.
 * Non volume functionality would impact the cross fader
 * volume.
 * Calls the setGain method on the AudioTransportSource data member,
 * passing in the multiplication of the player volume and cross fader volume.
 *
 */
void DJAudioPlayer::setGain(double gain, bool isVol) {
	if (isVol) {
		playerVol = gain;
	}
	else {
		crossFadeVol = gain;
	}
	if (gain < 0 || gain > 1.0) {
		DBG("DJAudioPlayer:: setGain Gain should be between 0 and 1");
	}
	else {
		transportSource.setGain(playerVol * crossFadeVol);
	}

};

/**
 * Implementation of setSpeed method for DJAudioPlayer
 *
 * If conditional acting as guard clause, ensuring resampling
 * ratio isnt set below 0 or above 100.
 * Sets ResamplingAudioSource data member's resampling ratio with
 * passed in value
 *
 */
void DJAudioPlayer::setSpeed(double ratio) {
	if (ratio < 0 || ratio > 100.0) {
		DBG("DJAudioPlayer:: setGain Gain should be between 0 and 100");
	}
	else {
		resampleSource.setResamplingRatio(ratio);
	}
};

/**
 * Implementation of setPosition method for DJAudioPlayer
 *
 * Sets the playback position by calling setPosition method
 * in the AudioTransportSource data member
 *
 */
void DJAudioPlayer::setPosition(double posInSecs) {
	transportSource.setPosition(posInSecs);
};

/**
 * Implementation of setPositionRelative method for DJAudioPlayer
 *
 * If conditional guard clause ensuring passed in value is between 0 and 1.
 * Converts value into a length in seconds and calls setPosition with converted value.
 *
 */
void DJAudioPlayer::setPositionRelative(double pos) {
	if (pos < 0 || pos > 1) {
		DBG("DJAudioPlayer:: setPositionRelative pos should be between 0 and 1");
	}
	else {
		double posInSecs = transportSource.getLengthInSeconds() * pos;
		setPosition(posInSecs);
	}
}

/**
 * Implementation of setFilter method for DJAudioPlayer
 *
 * Sets the high pass IIRCoefficients or low pass IIRCoefficients
 * on the IIRFilterAudioSource data members depending on the freq
 * value passed in
 *
 */
void DJAudioPlayer::setFilter(double freq) {
	if (freq > 0 && freq < 20000) {
		audioLPFilter.makeInactive();
		audioHPFilter.setCoefficients(juce::IIRCoefficients::makeHighPass(thisSampleRate, freq));
	}
	else if (freq < 0 && freq > -20000) {
		audioHPFilter.makeInactive();
		audioLPFilter.setCoefficients(juce::IIRCoefficients::makeLowPass(thisSampleRate, 20000 + freq));
	}
	else if (freq == 0) {
		audioHPFilter.makeInactive();
		audioLPFilter.makeInactive();
	}
}

/**
 * Implementation of setLBFilter method for DJAudioPlayer
 *
 * Sets the low shelf IIRCoefficients on the IIRFilterAudioSource
 * data member depending on the gain value passed in
 *
 */
void DJAudioPlayer::setLBFilter(double gain) {
	audioLBFilter.setCoefficients(juce::IIRCoefficients::makeLowShelf(thisSampleRate, 500, 1.0 / juce::MathConstants<double>::sqrt2, gain));
};

/**
 * Implementation of setMBFilter method for DJAudioPlayer
 *
 * Sets the peak filter IIRCoefficients on the IIRFilterAudioSource
 * data member depending on the gain value passed in
 *
 */
void DJAudioPlayer::setMBFilter(double gain) {
	audioMBFilter.setCoefficients(juce::IIRCoefficients::makePeakFilter(thisSampleRate, 3250, 1.0 / juce::MathConstants<double>::sqrt2, gain));
};

/**
 * Implementation of setHBFilter method for DJAudioPlayer
 *
 * Sets the high shelf IIRCoefficients on the IIRFilterAudioSource
 * data member depending on the gain value passed in
 *
 */
void DJAudioPlayer::setHBFilter(double gain) {
	audioHBFilter.setCoefficients(juce::IIRCoefficients::makeHighShelf(thisSampleRate, 5000, 1.0 / juce::MathConstants<double>::sqrt2, gain));
};

//==============================================================================

