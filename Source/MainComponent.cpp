#include "MainComponent.h"

//==============================================================================

/**
 * Implementation of a constructor for MainComponent
 *
 * In the constructor, graphic component data members are made visible here.
 * Initial component configurations are performed here, such as
 * registering basic formats, configuring look and feel properties of components,
 * and adding component listeners.
 *
 */
MainComponent::MainComponent()
{
	setSize(800, 600);

	// Some platforms require permissions to open input channels so request that here
	if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
		&& !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
	{
		juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
			[&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
	}
	else
	{
		// Specify the number of input and output channels that we want to open
		setAudioChannels(2, 2);
	}


	addAndMakeVisible(deckGUI1);
	addAndMakeVisible(deckGUI2);
	addAndMakeVisible(library);
	addAndMakeVisible(zoomedDisplay1);
	addAndMakeVisible(zoomedDisplay2);
	addAndMakeVisible(crossFader);

	crossFader.setRange(-1, 1);
	crossFader.setValue(0);
	crossFader.addListener(this);

	formatManager.registerBasicFormats();

	getLookAndFeel().setColour(juce::ResizableWindow::backgroundColourId, juce::Colour::fromRGBA(25, 25, 25, 255));

	crossFader.setLookAndFeel(&customLookAndFeel);
	library.setLookAndFeel(&customLookAndFeel);
	library.addKeyListener(this);
}

/**
 * Implementation of a destructor for MainComponent
 *
 * Shuts down the audio device and clears the audio source.
 */
MainComponent::~MainComponent()
{
	shutdownAudio();
}

//==============================================================================

/**
 * Implementation of prepareToPlay method for MainComponent
 *
 * Calls prepareToPlay methods on all AudioSource data members and adds audio sources
 * in the MainComponentLevel to the MixerAudioSource
 *
 */
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	mixerSource.addInputSource(&player1, false);
	mixerSource.addInputSource(&player2, false);

	player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
	player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

/**
 * Implementation of getNextAudioBlock method for MainComponent
 *
 * Calls getNextAudioBlock methods on the MixerAudioSource data member.
 *
 */
void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	mixerSource.getNextAudioBlock(bufferToFill);
}

/**
 * Implementation of releaseResources method for MainComponent
 *
 * Calls releaseResources methods on all AudioSource data members.
 *
 */
void MainComponent::releaseResources()
{
	mixerSource.removeAllInputs();
	mixerSource.releaseResources();
	player1.releaseResources();
	player2.releaseResources();
}

//==============================================================================

/**
 * Implementation of paint method for MainComponent
 *
 * Sets global background
 */
void MainComponent::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
	g.setFont(20.0f);
	g.setColour(juce::Colour::fromRGBA(25, 25, 25, 255));
	g.fillRect(crossFader.getLocalBounds());
}

/**
 * Implementation of resized method for MainComponent
 *
 * All juce::Component data members call it's setBounds method to achieve coherent space and sizing.
 *
 */
void MainComponent::resized()
{
	DBG("MainComponent::resized");
	double rowH = getHeight() / 8;

	zoomedDisplay1.setBounds(0, 0, getWidth(), 75 + getHeight() / 32);
	zoomedDisplay2.setBounds(0, 75 + getHeight() / 32, getWidth(), 75 + getHeight() / 32);
	deckGUI1.setBounds(0, 150 + getHeight() / 16, getWidth() / 2, 300);
	deckGUI2.setBounds(getWidth() / 2, 150 + getHeight() / 16, getWidth() / 2, 300);
	crossFader.setBounds(getWidth() / 2 - 80, 412.5 + getHeight() / 16, 160, 37.5);
	library.setBounds(0, 450 + getHeight() / 16, getWidth(), getHeight() - 450 - getHeight() / 16);

}

//==============================================================================

/**
 * Implementation of sliderValueChanged method for MainComponent
 *
 * juce::Slider cross fader is compared to the triggered juce::Slider pointer.
 * If the cross fader called this function, calls a setGain method in the
 * DJAudioPlayer instances. The gain set is inversely proportional to the distance
 * of the knob from the deck.
 *
 */
void MainComponent::sliderValueChanged(juce::Slider* slider) {
	if (slider == &crossFader) {
		double val;
		if (slider->getValue() > 0) {
			val = 1 - slider->getValue();
			player1.setGain(val, false);
			player2.setGain(1, false);
		}
		else if (slider->getValue() < 0) {
			val = 1 + slider->getValue();
			player2.setGain(val, false);
			player1.setGain(1, false);
		}
	}
}

//==============================================================================

/**
 * Implementation of keyPressed method for MainComponent
 *
 * Checks if the key pressed is the 'd' key.
 * If so calls on the library to delete an item.
 *
 */
bool MainComponent::keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent) {
	DBG(key.getKeyCode());
	if (key.getKeyCode() == 68) {
		DBG("Delete Match");
		library.deleteItem();
	}
	return true;
};

//==============================================================================



