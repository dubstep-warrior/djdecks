 

<div align="center">
<h1 align="center">
<img src="https://cdn-icons-png.flaticon.com/512/5750/5750739.png" width="100" />
<br>DJDecks
</h1>
<h3>◦ Unleash your music talents with DJDecks!</h3>
<h3>◦ Developed with the software and tools listed below.</h3>

<p align="center"> 
<img src="https://img.shields.io/badge/C++-00599C?style&logo=C%2B%2B&logoColor=white" alt="C++" /> 
<img src="https://img.shields.io/static/v1?style=for-the-badge&message=JUCE&color=222222&logo=JUCE&logoColor=8DC63F&label=" alt="Juce">
</p>
<img src="https://img.shields.io/github/languages/top/dubstep-warrior/DJDecks?style&color=5D6D7E" alt="GitHub top language" />
<img src="https://img.shields.io/github/languages/code-size/dubstep-warrior/DJDecks?style&color=5D6D7E" alt="GitHub code size in bytes" />
<img src="https://img.shields.io/github/commit-activity/m/dubstep-warrior/DJDecks?style&color=5D6D7E" alt="GitHub commit activity" />
<img src="https://img.shields.io/github/license/dubstep-warrior/DJDecks?style&color=5D6D7E" alt="GitHub license" />
</div>

---

## 📒 Table of Contentsf
- [📒 Table of Contents](#-table-of-contents)
- [📍 Overview](#-overview)
- [🕹️ Preview](#-preview)
- [⚙️ Features](#-features)
- [📂 Project Structure](#project-structure)
- [🧩 Modules](#modules)  
- [👏 Acknowledgments](#-acknowledgments)

---


## 📍 Overview

The DJDecks project is a DJ audio player application aimed at providing users with a visually appealing and feature-rich interface to explore and play audio tracks. Its core functionalities include audio playback, crossfading between decks, waveform visualization, track library management, cue points, filtering, and speed adjustment. The project's value proposition lies in its ability to offer a seamless and intuitive user experience for DJs and music enthusiasts, empowering them to mix and manipulate audio tracks with ease.

---

## 🕹️ Preview
![DJDecks](https://user-images.githubusercontent.com/75065469/174426015-2e10ea66-df57-4a42-bc0b-92532712c6dc.gif)

---

## ⚙️ Features

| Feature                | Description                           |
| ---------------------- | ------------------------------------- |
| **⚙️ Architecture**     | The codebase follows a modular design pattern with clear separation of concerns. It utilizes the JUCE framework for building the DJ audio player application. The code has defined components for managing the library, decks, waveforms, and audio playback. Overall, the architecture promotes code reuse, maintainability, and scalability.    |
| **📖 Documentation**   | The documentation is minimal and needs improvement. It lacks detailed information about the project structure, design choices, and how different components interact with each other. To enhance usability and maintainability, better documentation should be provided, including usage examples and design decisions.    |
| **🔗 Dependencies**    | The project heavily relies on the JUCE framework for audio processing, UI components, and build system. Other external dependencies include XML parsing for data persistence, SVG images for custom look and feel, and potentially some open-source audio-processing libraries.    |
| **🧩 Modularity**      | The codebase demonstrates good modularity by separating functionalities into smaller components. Each component is responsible for specific tasks and can be easily replaced or extended. The well-defined component boundaries allow for easier understanding and maintainability of the system.    |
| **✔️ Testing**          | Testing strategies and tools are not evident based on the codebase analysis. The absence of unit tests implies a lack of formalized testing and may introduce challenges for reproducibility, regression detection, and maintaining code quality over time.    |
| **⚡️ Performance**      | The codebase does not exhibit significant performance concerns based on the analysis. However, since audio processing can be resource-intensive, performance optimizations may still be necessary during real-time processing and playback. A profiling phase should help identify bottlenecks if needed.    |
| **🔐 Security**        | The codebase lacks explicit security measures based on the analysis. Common security concerns for audio player applications include permissions handling, file input validation, and potential vulnerabilities in third-party libraries or plugins. A security audit and best practices implementation would be beneficial.    |
| **🔀 Version Control** | The project uses Git for version control. Regular and descriptive commits have been made, indicating a collaborative approach while developing the application. Branching strategies, code review, and issue tracking can provide further insights into well-regulated development practices.    |
| **🔌 Integrations**    | The codebase integrates well with the JUCE framework, leveraging its powerful audio and UI capabilities. As JUCE is highly extensible, additional integrations with audio processing libraries, hardware interfaces, or cloud-based music platforms can be implemented through the existing architecture.    |
| **📶 Scalability**     | The codebase should be scalable since it follows modularity principles. As the system grows, new components can be added, existing components can be extended, and externals dependencies can be integrated. These design decisions contribute to the application's ability to accommodate future enhancements and expansions.    |

---


## 📂 Project Structure

```bash
.
├── Assets
│   ├── Image backup
│   │   └── ...17 files
│   └── ...8 files
├── OtoDecks.jucer
├── README.md
└── Source
    ├── CustomLookAndFeel.cpp
    ├── CustomLookAndFeel.h
    ├── DJAudioPlayer.cpp
    ├── DJAudioPlayer.h
    ├── DeckGUI.cpp
    ├── DeckGUI.h
    ├── JogWheel.cpp
    ├── JogWheel.h
    ├── Library.cpp
    ├── Library.h
    ├── Main.cpp
    ├── MainComponent.cpp
    ├── MainComponent.h
    ├── PlaylistComponent.cpp
    ├── PlaylistComponent.h
    ├── Track.h
    ├── WaveformDisplay.cpp
    ├── WaveformDisplay.h
    ├── ZoomedWaveform.cpp
    └── ZoomedWaveform.h

3 directories, 47 files

```


---

## 🧩 Modules

<details closed><summary>Root</summary>

| File                                                                                  | Summary                                                                                                                                                                                                                                                                                         |
| ---                                                                                   | ---                                                                                                                                                                                                                                                                                             |
| [OtoDecks.jucer](https://github.com/dubstep-warrior/DJDecks/blob/main/OtoDecks.jucer) | This code is an XML file containing a JUCE project configuration. It includes information about the project structure, file dependencies, and export formats for building the project. The code defines the project's assets and source files, as well as the JUCE modules used in the project. |

</details>

<details closed><summary>Source</summary>

| File                                                                                                       | Summary                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                |
| ---                                                                                                        | ---                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    |
| [JogWheel.cpp](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/JogWheel.cpp)                   | The code implements a JogWheel component that displays a circular playhead and allows the user to drag it to change the current position of the song.                                                                                                                                                                                                                                                                                                                                                  |
| [JogWheel.h](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/JogWheel.h)                       | The JogWheel class is a component that functions as a playback control interface with a DJ deck appearance. It inherits functionalities from the ZoomedWaveform class and communicates with the DJAudioPlayer through the DeckGUI interface. It allows for drag interactions and displays a playhead with a timer.                                                                                                                                                                                     |
| [Library.cpp](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/Library.cpp)                     | The code defines the core functionalities of a Library component. It reads XML files, populates track folders, allows for folder and track selection, deletion of items, file drag and drop, and saves the data back to the XML file. The component also includes a playlist and a directory widget.                                                                                                                                                                                                   |
| [MainComponent.h](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/MainComponent.h)             | This code defines the MainComponent class which handles the core functionalities of a DJ audio player application. It includes features such as loading and playing audio files, crossfading between decks, displaying waveforms, and managing a library of audio files. The class also handles audio processing and user interactions through key press and slider events.                                                                                                                            |
| [DeckGUI.cpp](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/DeckGUI.cpp)                     | This code defines a C++ class named DeckGUI, which is part of an audio player application. Its core functionalities include initializing and managing the graphical user interface (GUI) elements for controlling audio playback. It creates buttons for play/pause and loading tracks, sliders for volume and speed control, and waveform displays for visualizing audio. The class handles user interactions with these components, such as button clicks and slider adjustments, to control the audio playback through a DJAudioPlayer instance. It also manages cue points associated with the audio tracks, updates the visual representation of the volume meter, and responds to file drag-and-drop events for loading audio tracks. Overall, it serves as the control interface for the audio player, allowing users to control playback, adjust settings, and visualize audio waveforms.                                                                                                                                                                                                                                                                                                                                                                                                                                                                            |
| [Track.h](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/Track.h)                             | The code defines a struct for a track, with properties like title, length, url, and identity. It also includes a static function to convert the track length to a string format.                                                                                                                                                                                                                                                                                                                       |
| [MainComponent.cpp](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/MainComponent.cpp)         | The code defines a MainComponent class that represents the main graphical user interface. It sets up audio channels, configures components, and adds listeners. The class also handles resizing, painting, and key press events. Additionally, it manages audio sources, prepares them for playback, and processes audio blocks. The cross fader functionality is implemented, allowing user control over audio gain between two decks.                                                                |
| [Library.h](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/Library.h)                         | The Library class manages a library of playlist folders. It allows users to select folders, view track lists, and add/delete folders and tracks. It uses XML for data persistance and supports drag and drop functionality.                                                                                                                                                                                                                                                                            |
| [ZoomedWaveform.cpp](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/ZoomedWaveform.cpp)       | The code implements a class called ZoomedWaveform that adds functionality to display a zoomed waveform and a fixed playhead. The waveform is drawn based on the position and the audio thumbnail. The mouseDrag method allows the user to move the playhead by dragging the mouse.                                                                                                                                                                                                                     |
| [ZoomedWaveform.h](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/ZoomedWaveform.h)           | The ZoomedWaveform component is a subclass of the WaveformDisplay component. It has a similar appearance but different playback control functionality. It communicates with the DJAudioPlayer through the DeckGUI interface. Its core functions include painting the component, setting component bounds, and handling mouse events.                                                                                                                                                                   |
| [PlaylistComponent.h](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/PlaylistComponent.h)     | The PlaylistComponent class manages a folder of tracks and provides functionalities for track selection and searching. It includes methods for retrieving selected tracks and track indices, painting the component, handling text editor changes, and managing track title vectors.                                                                                                                                                                                                                   |
| [PlaylistComponent.cpp](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/PlaylistComponent.cpp) | The code is for a PlaylistComponent that displays a table of track titles and lengths. It allows for searching the tracks by text and selecting a track.                                                                                                                                                                                                                                                                                                                                               |
| [Main.cpp](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/Main.cpp)                           | The code defines the OtoDecksApplication class, which serves as a JUCE application. It initializes and shuts down the application, handles system requests to quit, and manages the main application window. The MainWindow class is a desktop window that contains the MainComponent class.                                                                                                                                                                                                           |
| [DeckGUI.h](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/DeckGUI.h)                         | The DeckGUI class is responsible for creating a graphical user interface (GUI) for a DJ deck. It includes functionalities such as track loading, play/pause control, volume and speed adjustment, and filter manipulation. It also provides visual feedback through waveform displays and cue buttons. The class integrates with other components such as DJAudioPlayer, ZoomedWaveform, and Library to manage audio functionality. The GUI is customized with a theme color and custom look and feel. |
| [WaveformDisplay.h](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/WaveformDisplay.h)         | The WaveformDisplay component allows the user to load and display audio waveforms. It includes playback functionality, can set the playhead position, and communicates with DJAudioPlayer controls. The component also supports cue points and allows the user to interact with the waveform through mouse events.                                                                                                                                                                                     |
| [DJAudioPlayer.h](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/DJAudioPlayer.h)             | The DJAudioPlayer class is an AudioSource that provides functionalities such as loading, playing, stopping, filtering, and adjusting gain and speed of audio files. It can also provide information about the loaded file, such as its URL and position.                                                                                                                                                                                                                                               |
| [DJAudioPlayer.cpp](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/DJAudioPlayer.cpp)         | The code defines a DJAudioPlayer class that contains functions for playing audio, controlling playback parameters like volume and speed, and applying filters. It uses the JUCE library for audio processing and handles file loading, playback control, and audio processing.                                                                                                                                                                                                                         |
| [CustomLookAndFeel.h](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/CustomLookAndFeel.h)     | The code defines a custom LookAndFeel class that customizes the appearance of sliders and table headers in a GUI application. It overrides the default methods for drawing linear and rotary sliders, as well as the table header background. It uses SVG images to create custom slider knobs.                                                                                                                                                                                                        |
| [CustomLookAndFeel.cpp](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/CustomLookAndFeel.cpp) | The code defines a custom look and feel for various UI components such as sliders and table headers. The functions in the code draw the components with specific styles and appearance, such as drawing knobs, ticks, and markers.                                                                                                                                                                                                                                                                     |
| [WaveformDisplay.cpp](https://github.com/dubstep-warrior/DJDecks/blob/main/Source/WaveformDisplay.cpp)     | The code creates a WaveformDisplay component that can load and display audio waveforms. It allows for setting cue points and provides functionalities for interacting with the waveform, such as dragging the playhead and displaying cue points.                                                                                                                                                                                                                                                      |

</details>

 
---  

## 👏 Acknowledgments

> - `ℹ️  Juce Framework`

---
