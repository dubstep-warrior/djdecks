
#include "PlaylistComponent.h"

//==============================================================================

/**
 * Implementation of a constructor for PlaylistComponent
 *
 * Data members are initialized and initial configurations are applied to components here.
 *
 */
PlaylistComponent::PlaylistComponent(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager), trackTitles(NULL)
{
	tableComponent.getHeader().addColumn("Track Title", 1, 300);
	tableComponent.getHeader().addColumn("Length", 2, 150);
	tableComponent.setModel(this);
	tableComponent.setColour(juce::TableListBox::ColourIds::backgroundColourId, juce::Colour::fromRGBA(25, 25, 25, 255));
	addAndMakeVisible(tableComponent);

	search.addListener(this);
	search.setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colour::fromRGBA(25, 25, 25, 255));
	addAndMakeVisible(search);

	setInterceptsMouseClicks(false, true);
}

/**
 * Implementation of a destructor for PlaylistComponent
 *
 */
PlaylistComponent::~PlaylistComponent()
{
}

//==============================================================================

/**
 * Implementation of trackIsSelected method for PlaylistComponent
 *
 * Check that tableComponent.getSelectedRow() respect the boundaries of displayTrackTitles size.
 *
 */
bool PlaylistComponent::trackIsSelected() {
	return (tableComponent.getSelectedRow() >= 0 && tableComponent.getSelectedRow() < displayTrackTitles.size());
}

//==============================================================================

/**
 * Implementation of setTrackTitles method for PlaylistComponent
 *
 * Sets the trackTitles data structure with the reference vector passed in at the library level.
 * Fills the displayTrackTitles data structure with trackTitles.
 *
 */
void PlaylistComponent::setTrackTitles(std::vector<track>& _trackTitles) {
	trackTitles = &_trackTitles;
	displayTrackTitles.clear();
	for (auto i = 0; i < trackTitles->size(); i++) {
		displayTrackTitles.push_back(&trackTitles->at(i));
	}
	tableComponent.deselectAllRows();
	tableComponent.updateContent();
};

//==============================================================================

/**
 * Implementation of getSelectedTrack method for PlaylistComponent
 *
 * Returns the track that is selected in displayTrackTitles
 *
 */
track PlaylistComponent::getSelectedTrack() {
	return *displayTrackTitles[getSelectedTrackIndex()];
};

/**
 * Implementation of getSelectedTrackIndex method for PlaylistComponent
 *
 * Returns the track index that is selected in tableComponent
 *
 */
int PlaylistComponent::getSelectedTrackIndex() {
	return tableComponent.getSelectedRow();
};

//==============================================================================

/**
 * Implementation of paint method for PlaylistComponent
 *
 */
void PlaylistComponent::paint(juce::Graphics& g)
{
}

/**
 * Implementation of resized method for PlaylistComponent
 *
 * Calls setBounds method on the TableListBox and TextEditor data members
 *
 */
void PlaylistComponent::resized()
{
	tableComponent.setBounds(0, 0, getWidth(), getHeight());
	search.setBounds(getWidth() - 105, 2, 100, tableComponent.getHeaderComponent()->getBounds().getHeight() - 4);
}

//==============================================================================

/**
 * Implementation of getNumRows method for PlaylistComponent
 *
 * Returns the size of the data structure displayTrackTitles
 */
int PlaylistComponent::getNumRows() {
	return displayTrackTitles.size();
}

/**
 * Implementation of paintRowBackground method for PlaylistComponent
 *
 * Change the colour of the row if they are selected.
 * Selection between two colours to colour the row depending on their row numbers
 *
 */
void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
	if (rowNumber < displayTrackTitles.size()) {
		if (rowIsSelected) {
			g.fillAll(juce::Colour::fromRGBA(0, 125, 225, 255));
			DBG(tableComponent.getSelectedRow());
		}
		else {
			rowNumber % 2 == 0 ? g.fillAll(juce::Colour::fromRGBA(50, 50, 50, 255)) : g.fillAll(juce::Colour::fromRGBA(12.5, 12.5, 12.5, 255));
		}
	}

};

/**
 * Implementation of paintCell method for PlaylistComponent
 *
 * Draw the text of the track names and song length on the rows using displayTrackTitles data structure
 *
 */
void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {

	if (rowNumber < displayTrackTitles.size()) {
		g.setColour(juce::Colours::white);
		if (tableComponent.getHeader().getColumnName(columnId) == "Track Title") {
			g.drawText(displayTrackTitles.at(rowNumber)->title, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
		}
		else if (tableComponent.getHeader().getColumnName(columnId) == "Length") {
			std::string time = track::getLengthString(displayTrackTitles.at(rowNumber)->lengthInSeconds);
			g.drawText(time, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
		}
	}
};

//==============================================================================

/**
 * Implementation of textEditorTextChanged method for PlaylistComponent
 *
 * Fills the displayTrackTitles data structure with trackTitles tracks
 * that contain the substring in the text editor.
 *
 */
void PlaylistComponent::textEditorTextChanged(juce::TextEditor& e) {

	displayTrackTitles.clear();
	DBG("displayTrackTitles size" << displayTrackTitles.size());
	if (e.getText().isEmpty()) {
		for (auto i = 0; i < trackTitles->size(); i++) {
			displayTrackTitles.push_back(&trackTitles->at(i));
		}
	}
	else {
		for (auto i = 0; i < trackTitles->size(); i++) {
			if (trackTitles->at(i).title.containsIgnoreCase(e.getText())) {

				displayTrackTitles.push_back(&trackTitles->at(i));
			}
		}
	}
	DBG("displayTrackTitles size" << displayTrackTitles.size());
	tableComponent.updateContent();
};

//==============================================================================


