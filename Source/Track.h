
#include <JuceHeader.h>
#pragma once

/**
 * Definition of a track struct
 *
 * A track object that is representative of a custom audio file,
 * containing a title string, song length double, url of audio file,
 * as well as a double to string conversion function of song lengths.
 *
 */
struct track {

	//==============================================================================

	/// Title of song
	juce::String title;

	/// Length of song in seconds
	double lengthInSeconds;

	/// juce::URL of the track object
	juce::URL url;

	/// Identity hash of track
	juce::String identity;

	//==============================================================================

	/**
		* Converts a Song length/position in seconds into string format
		*
		* @param Song length/position in seconds
		* @param If given function is called regularly
		* @return A song length in a string format
	*/
	static std::string getLengthString(double songLength, bool regularUpdate = false) {
		std::string time = "";
		int hour = std::floor(songLength / 3600);
		int min = std::floor((songLength - ((double)hour * 3600)) / 60);
		double sec = std::floor((songLength - ((double)min * 60)) * 100) / 100;
		int miniSec = std::floor((sec - std::floor(sec)) * 100);
		if (!regularUpdate) {
			if (hour < 10) {
				time += "0" + std::to_string(hour) + ":";
			}
			else {
				time += std::to_string(hour) + ":";
			}
		}

		if (min < 10) {
			time += "0" + std::to_string(min) + ":";
		}
		else {
			time += std::to_string(min) + ":";
		}

		if (sec < 10) {
			time += "0" + std::to_string((int)sec);
		}
		else {
			time += std::to_string((int)sec);
		}

		if (regularUpdate) {
			if (miniSec < 10) {
				time += ":0" + std::to_string(miniSec);
			}
			else {
				time += ":" + std::to_string(miniSec);
			}
		}

		return time;
	};

	//==============================================================================
};
