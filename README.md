# 2D Space Game - Qt / C++

Source-code snapshot of a three-person school project built with C++ and Qt Creator.

The game includes player movement and collision handling, TCP client communication, enemy and asteroid events, audio playback and video backgrounds using Qt Multimedia.

## Notes

- This archive contains the source code and Qt project file only.
- Media assets (images, music and videos) are not included in this portfolio copy. Media paths use generic placeholder filenames for readability.
- The TCP host in `network.cpp` points to the private address used during development and would need to be changed for another environment.

## Project structure

- `network.*` - TCP client and command processing
- `game_engine.*` - movement, asteroid and gameplay logic
- `input_engine.*` - keyboard controls
- `sound_engine.*` - music and sound playback
- `videoplayer.*` - background video playback
- `widget.*` - main game window, rendering and game state
- `config.*` - media paths and shared configuration
