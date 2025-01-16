# Level Editor User Guide

This guide will walk you through the installation and usage of the **Level Editor** software. The Level Editor allows you to create and edit game levels with sprites, waves, backgrounds, and music.

![Main](https://i.imgur.com/vrQ8G7C.png)

---

## Table of Contents
1. [Installation](#installation)
2. [Getting Started](#getting-started)
3. [User Interface Overview](#user-interface-overview)
4. [Creating Levels](#creating-levels)
   - [Adding Sprites](#adding-sprites)
   - [Editing Sprites](#editing-sprites)
   - [Adding Waves](#adding-waves)
   - [Selecting Waves](#selecting-waves)
5. [Saving and Loading Levels](#saving-and-loading-levels)
6. [Troubleshooting](#troubleshooting)

---

## Installation

### Prerequisites
- **Python 3.x**: Ensure Python is installed on your system. You can download it from [python.org](https://www.python.org/).
- **Tkinter**: This is included with Python by default. If it’s missing, install it using your package manager.

### Steps

1. **Install Dependencies**:
   - No additional dependencies are required as the Level Editor uses only Python's built-in libraries.

2. **Run the Level Editor**:
   - Open a terminal or command prompt.
   - Navigate to the folder where `main.py.py` is located.
   - Run the following command:
     ```bash
     python main.py.py
     ```

---

## Getting Started

Once the Level Editor is running, you’ll see the main window with the following components:
- **Canvas**: The area where you place and edit sprites.
- **Control Panel**: Contains buttons and dropdowns for adding sprites, waves, and saving/loading levels.
- **Edit Panel**: Allows you to modify sprite properties.

---

## User Interface Overview

### Canvas
- The canvas is where you place and visualize sprites.
- The blue dashed rectangle represents the **play area** (the player's screen).

### Control Panel
- **Sprite Dropdown**: Select the type of sprite to add (e.g., Basic Enemy, Shooter Enemy, Boss, Bonus).
- **Background Dropdown**: Choose the background for the level.
- **Music Dropdown**: Select the music for the level.
- **Add Sprite Button**: Add a sprite to the current wave.
- **Add Wave Button**: Create a new wave.
- **Wave Selector**: Switch between waves to edit them.
- **Save Button**: Save the current level to a file.
- **Load Button**: Load a level from a file.

### Edit Panel
- Displays fields for editing the selected sprite’s properties:
  - **X Position**: Horizontal position (0-1 inside the play area, >1 or <0 outside).
  - **Y Position**: Vertical position (0-1 inside the play area, >1 or <0 outside).
  - **X Velocity**: Horizontal speed (0-1).
  - **Y Velocity**: Vertical speed (0-1).
  - **Health**: Health of the sprite.
  - **Bonus Type**: Type of bonus (0-2).

---

## Creating Levels

### Adding Sprites
1. Select a sprite type from the **Sprite Dropdown**.
2. Click the **Add Sprite** button.
3. Click on the canvas to place the sprite. You can place it inside or outside the play area.

![Add Sprite](https://i.imgur.com/PIvyKAA.png)

### Editing Sprites
1. Click on a sprite to select it.
2. Modify its properties in the **Edit Panel**.
3. Click **Apply Changes** to save your edits.

![Edit Sprite](https://i.imgur.com/rKkcF0L.png)

### Adding Waves
1. Click the **Add Wave** button to create a new wave.
2. The new wave will be added to the **Wave Selector** dropdown.

![Add Wave](https://i.imgur.com/hQOZoyk.png)

### Selecting Waves
1. Use the **Wave Selector** dropdown to switch between waves.
2. The canvas will update to show the sprites for the selected wave.

![Select Wave](https://i.imgur.com/p4zh3F4.png)

---

## Saving and Loading Levels

### Saving a Level
1. Click the **Save Button**.
2. Choose a location and filename for your level file (e.g., `level1.txt`).
3. The level will be saved with the selected background, music, and wave data.

### Loading a Level
1. Click the **Load Button**.
2. Select a level file (e.g., `level1.txt`).
3. The level will be loaded, and you can continue editing it.

![Save and Load](https://i.imgur.com/0ZQr8yG.png)

---

## Troubleshooting

### Common Issues
1. **Sprites Not Appearing**:
   - Ensure the sprite images are in the `sprites` folder.
   - Check the console for error messages.

2. **File Not Saving**:
   - Ensure you have write permissions for the selected directory.
   - Check the console for error messages.

3. **File Not Loading**:
   - Ensure the file is in the correct format.
   - Check the console for error messages.

---

## Conclusion

The Level Editor is a powerful tool for creating and editing game levels. With its intuitive interface and flexible features, you can design levels with ease. If you encounter any issues or have suggestions for improvement, feel free to reach out!
