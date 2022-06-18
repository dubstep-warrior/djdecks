
#include "Library.h"

//==============================================================================

/**
 * Implementation of a constructor for Library
 *
 * Data members are initialized and initial configurations are applied to components here.
 * File reading occurs from a fixed path defined in the header file.
 * The retrieved value tree from reading off the xml file is used to populate
 * the elements of the trackFolders data structure.
 *
 */
Library::Library(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager), playlist(_formatManager)
{
	juce::File file(filePath);
	if (!file.existsAsFile())
	{
		DBG("FILE DONT EXIST");
		DBG((file.create().wasOk() ? "Creation Success" : "Creation Failed"));
		std::pair<juce::String, std::vector<track>> folder;
		folder.first = "Main";
		trackFolders.push_back(folder);
	}
	else {
		DBG("FILE EXIST");
		juce::FileInputStream in(file);
		if (in.openedOk()) {
			auto newValueTree = juce::ValueTree::readFromStream(in);
			for (auto i = 0; i < newValueTree.getNumChildren(); ++i) {
				std::pair<juce::String, std::vector<track>> folder;
				folder.first = newValueTree.getChild(i).getProperty("name");
				for (auto j = 0; j < newValueTree.getChild(i).getNumChildren(); ++j) {
					auto song = newValueTree.getChild(i).getChild(j);
					track refSong{ song.getProperty("title"), song.getProperty("length") , song.getProperty("url") , song.getProperty("identity") };
					folder.second.push_back(refSong);
				}
				trackFolders.push_back(folder);
			}
		}
	}

	selectedFolderIndex = 0;
	playlist.setTrackTitles(trackFolders[selectedFolderIndex].second);
	addAndMakeVisible(playlist);
	playlist.setLookAndFeel(&customLookAndFeel);

	directoryComponent.getHeader().addColumn("Folders", 1, 360);
	directoryComponent.setModel(this);
	addAndMakeVisible(directoryComponent);
	directoryComponent.setColour(juce::ListBox::ColourIds::backgroundColourId, juce::Colour::fromRGBA(25, 25, 25, 255));
	directoryComponent.selectRow(selectedFolderIndex);
}

/**
 * Implementation of a destructor for Library
 *
 * The trackFolders data structure is used to populate a value tree before
 * writing it to the xml file at the same path.
 *
 */
Library::~Library()
{
	juce::File file(filePath);
	file.deleteFile();

	juce::ValueTree main(juce::Identifier("main"));
	for (auto i = 0; i < trackFolders.size(); ++i) {
		juce::ValueTree folder(juce::Identifier(std::to_string(i)));
		folder.setProperty(juce::Identifier("name"), trackFolders[i].first, nullptr);
		for (auto j = 0; j < trackFolders[i].second.size(); ++j) {
			juce::ValueTree song(juce::Identifier(std::to_string(j)));
			song.setProperty("title", trackFolders[i].second[j].title, nullptr);
			song.setProperty("length", trackFolders[i].second[j].lengthInSeconds, nullptr);
			song.setProperty("url", trackFolders[i].second[j].url.toString(false), nullptr);
			song.setProperty("identity", trackFolders[i].second[j].identity, nullptr);
			folder.addChild(song, j, nullptr);
		}
		main.addChild(folder, i, nullptr);
	}

	DBG("FILE SAVING");
	file.create();
	juce::FileOutputStream outstream(file);
	main.writeToStream(outstream);
}

//==============================================================================

/**
 * Implementation of selectionIsValid method for Library
 *
 * Returns if the folder and track selection is valid.
 * This is ensured by checking that the selectedFolderIndex respect the bounds of the trackFolders size
 * and that the playlist track is selected
 *
 */
bool Library::selectionIsValid() {
	DBG((playlist.trackIsSelected() ? "true" : "3false"));
	DBG((selectedFolderIndex >= 0 ? "true" : "1false"));
	DBG((selectedFolderIndex < trackFolders.size() ? "true" : "2false"));
	return selectedFolderIndex >= 0 && selectedFolderIndex < trackFolders.size() && playlist.trackIsSelected();
};

/**
 * Implementation of getSelectedTrack method for Library
 *
 * Returns the selected track from the playlist instance
 *
 */
track Library::getSelectedTrack() {
	return playlist.getSelectedTrack();
};

/**
 * Implementation of deleteItem method for Library
 *
 * Check if only folder is selected or both folder and track is selected.
 * In the former case, the entire folder element is erased off the trackFolders data structure.
 * In the latter case, the track element is erased off the selected folder in trackFolders. Identity
 * hash strings of tracks are compared to confirm the track to be deleted.
 *
 */
void Library::deleteItem() {
	if (selectedFolderIndex >= 0 && selectedFolderIndex < trackFolders.size()) {
		if (playlist.trackIsSelected()) {
			auto& selectedPlaylist = trackFolders[selectedFolderIndex].second;
			auto selectedTrack = -1;
			for (auto i = 0; i < selectedPlaylist.size(); ++i) {
				if (selectedPlaylist[i].identity == getSelectedTrack().identity) {
					DBG("True delete match");
					selectedTrack = i;
					break;
				}
			}
			if (selectedTrack > -1) {
				selectedPlaylist.erase(selectedPlaylist.begin() + selectedTrack);
			}
			playlist.setTrackTitles(trackFolders[selectedFolderIndex].second);
		}
		else {
			if (trackFolders.size() > 1) {
				trackFolders.erase(trackFolders.begin() + selectedFolderIndex);
				selectedFolderIndex = 0;
				playlist.setTrackTitles(trackFolders[selectedFolderIndex].second);
				directoryComponent.selectRow(selectedFolderIndex);
			}
			directoryComponent.updateContent();
		}
	}
};

//==============================================================================

/**
 * Implementation of paint method for Library
 */
void Library::paint(juce::Graphics& g)
{
}

/**
 * Implementation of resized method for Library
 *
 * Call setBounds method on the juce::Component data members playlist and directoryComponent.
 */
void Library::resized()
{
	if (selectedFolderIndex != -1) {
		playlist.setBounds(1.5 * getWidth() / 8, 0, 6.5 * getWidth() / 8, getHeight());
	}
	directoryComponent.setBounds(0, 0, 1.5 * getWidth() / 8, getHeight());
}

//==============================================================================

/**
 * Implementation of getNumRows method for Library
 *
 * Returns the size of the data structure trackFolders
 */
int Library::getNumRows() {
	return trackFolders.size();
};

/**
 * Implementation of paintRowBackground method for Library
 *
 * Change the colour of the row if they are selected.
 */
void Library::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
	if (rowNumber < trackFolders.size()) {
		if (rowIsSelected) {
			g.fillAll(juce::Colour::fromRGBA(0, 125, 225, 255));
		}
		else {
			g.fillAll(juce::Colour::fromRGBA(100, 100, 100, 255));
		}
	}
};

/**
 * Implementation of paintCell method for Library
 *
 * Draw the text of the folder names on the rows
 *
 */
void Library::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
	g.setColour(juce::Colours::white);
	if (rowNumber < trackFolders.size()) {
		g.drawText(trackFolders[rowNumber].first, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
	}

};

/**
 * Implementation of cellClicked method for Library
 *
 * Sets the selected folder index
 * Sets the playlist folder with the selected folder
 *
 */
void Library::cellClicked(int rowNumber, int columnId, const juce::MouseEvent& e) {
	DBG(" PlaylistComponent::cellClicked " << rowNumber);
	selectedFolderIndex = rowNumber;
	playlist.setTrackTitles(trackFolders[selectedFolderIndex].second);
};

//==============================================================================

/**
 * Implementation of isInterestedInFileDrag method for Library
 *
 * Returns true
 *
 */
bool Library::isInterestedInFileDrag(const juce::StringArray& files) {
	return true;
};

/**
 * Implementation of filesDropped method for Library
 *
 * Checks if the dropped items are in the library or playlist components.
 * The addition of tracks/folders are performed on the trackFolders data structure, storing all
 * the folder/track data in the library level. Selection of the folder and what the playlist displays
 * is communicated to the playlist instance using the trackFolders data.
 * Adds tracks into the currently selected folder if items are dropped on the playlist component.
 * Adds folder of tracks into the library if items are dropped on the library component.
 *
 */
void Library::filesDropped(const juce::StringArray& files, int x, int y) {
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
	auto timeString = oss.str();

	if (x > 1.5 * getWidth() / 8) {
		if (selectedFolderIndex != -1) {
			for (auto i = 0; i < files.size(); ++i) {
				DBG("YES ADDED FILE " << files.size());
				auto audioFile = juce::File{ files[i] };
				audioReader.reset(formatManager.createReaderFor(audioFile));
				if (audioReader != nullptr) {
					DBG("YES ADDED FILE");
					std::hash<std::string> hasher;
					track thisTrack = { audioFile.getFileNameWithoutExtension() ,audioReader->lengthInSamples / audioReader->sampleRate,juce::URL{audioFile} };
					size_t hash = hasher(thisTrack.title.toStdString() + std::to_string(thisTrack.lengthInSeconds) + thisTrack.url.toString(false).toStdString() + std::to_string(trackFolders[selectedFolderIndex].second.size()) + timeString);
					char hashString[256] = "";
					snprintf(hashString, sizeof hashString, "%zu", hash);
					DBG(hashString);
					thisTrack.identity = juce::String(hashString);
					trackFolders[selectedFolderIndex].second.push_back(thisTrack);
				}
			}
		}
	}
	else {
		for (auto i = 0; i < files.size(); ++i) {
			auto audioFile = juce::File{ files[i] };
			if (audioFile.isDirectory()) {
				std::pair<juce::String, std::vector<track>> thisFolder;
				thisFolder.first = audioFile.getFileNameWithoutExtension();
				auto folder = audioFile.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false);
				for (auto& file : folder) {
					audioReader.reset(formatManager.createReaderFor(file));
					if (audioReader != nullptr) {
						std::hash<std::string> hasher;
						track thisTrack = { file.getFileNameWithoutExtension() ,audioReader->lengthInSamples / audioReader->sampleRate,juce::URL{file} };
						size_t hash = hasher(thisTrack.title.toStdString() + std::to_string(thisTrack.lengthInSeconds) + thisTrack.url.toString(false).toStdString() + std::to_string(trackFolders[selectedFolderIndex].second.size()) + timeString);
						char hashString[256] = "";
						snprintf(hashString, sizeof hashString, "%zu", hash);
						DBG(hashString);
						thisTrack.identity = juce::String(hashString);
						thisFolder.second.push_back(thisTrack);
					}
				}
				trackFolders.push_back(thisFolder);
				selectedFolderIndex = trackFolders.size() - 1;
			}
		}
	}
	if (selectedFolderIndex != -1) {
		playlist.setTrackTitles(trackFolders[selectedFolderIndex].second);
	}
	directoryComponent.updateContent();
	directoryComponent.selectRow(selectedFolderIndex, true);
};

//==============================================================================

