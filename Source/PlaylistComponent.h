

#pragma once

#include <JuceHeader.h>
#include "Track.h"
//==============================================================================

/**
 * Definition of a Playlist Component
 *
 * A component that manages a folder of tracks.
 * Contains track selection and track searching functionalities.
 *
 */
class PlaylistComponent : public juce::Component,
	public juce::TableListBoxModel,
	public juce::TextEditor::Listener
{
public:

	//==============================================================================

	/**
	   * Class Constructor for PlaylistComponent, initializes member variables and configures component details.
	   *
	   * @param AudioFormatManager reference
   */
	PlaylistComponent(juce::AudioFormatManager& _formatManager);

	/**
		* Class destructor for PlaylistComponent
	*/
	~PlaylistComponent() override;

	//==============================================================================

	/**
		* @return If table component row is selected
	*/
	bool trackIsSelected();

	//==============================================================================

	/**
		* Updates the trackTitles vector pointer
	*/
	void setTrackTitles(std::vector<track>& _trackTitles);

	//==============================================================================

	/**
		* @return The selected track object
	*/
	track getSelectedTrack();

	/**
		* @return The selected tableComponent row number
	*/
	int getSelectedTrackIndex();

	//==============================================================================

private:

	//==============================================================================

	/**
		* Paints the PlaylistComponent Component.
		*
		* @param juce::Graphics object
	*/
	void paint(juce::Graphics&) override;

	/**
		* Set bounds of member components
	*/
	void resized() override;

	//==============================================================================

	/**
		* @return Number of rows in the library selection
	*/
	int getNumRows() override;

	/**
		* Paints the row's background of the tableComponent member.
		*
		* @param juce::Graphics object
		* @param Row number
		* @param Row width
		* @param Row height
		* @param If the row is selected
	*/
	void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;

	/**
		* Paints each cell of the tableComponent member.
		*
		* @param juce::Graphics object
		* @param Row number
		* @param Column number
		* @param Cell width
		* @param Cell height
		* @param If the row is selected
	*/
	void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

	//==============================================================================

	/**
	   * Called when the search TextEditor faces change
	   *
	   * @param juce::TextEditor reference that has added this component as its listener
   */
	void textEditorTextChanged(juce::TextEditor& e);

	//==============================================================================

	/// Reference assigned to the AudioFormatManager passed into the constructor
	juce::AudioFormatManager& formatManager;

	/// Reflects the displayTrackTitles' elements
	juce::TableListBox tableComponent;

	/// Search box to search for tracks by name
	juce::TextEditor search;

	/// Pointer to std::vector<track>  
	std::vector<track>* trackTitles;

	/// std::vector of pointers to track objects 
	std::vector<track*> displayTrackTitles;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};


