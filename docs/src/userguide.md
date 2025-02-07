# R-Type Multiplayer Game - User Documentation

## Version

v8.0

## Last Updated

19 January 2025

## Authors

- **[RICARD Alexandre]**
- **[Loustalot Lucas]**
- **[Scandella Raphaël]**
- **[AUGAIT Arnaud]**
- **[NASSET Samy]**

## License & Distribution

This document is for internal and external use. Reproduction is allowed with proper credit to the authors.

## Introduction

This document provides user instructions for setting up and playing the multiplayer R-Type game. It is designed for users who want to host or join a multiplayer game session.

### Target Audience

Gamers who want to play the R-Type multiplayer game.

### Prerequisites

A computer running Windows, Linux, or macOS.

## Installation & Setup

### Step 0 : Open a terminal or command-line interface

On Linux and macOS, open a terminal.
On Windows, open Powershell.

### Step 1 : Clone the Repository

First, clone the project repository using Git :

```
git clone <repository_url>
```

### Step 2 : Navigate to the Tools Directory

Navigate to the tools directory :

#### Windows / Linux / macOS :

```
cd tools
```

Then, execute the appropriate build script based on your operating system :

#### Windows :

```
./build-windows.ps1
```

#### Linux :

```
./build-linux.sh
```

#### macOS :

```
./build-macos.sh
```

### Step 3 : Access the Release Directory

Once the build process is complete, leave the tools directory and navigate to the release directory :

#### Windows / Linux / macOS :

```
cd ..
cd release
```

## Running the Game

### Single Computer (Local Server and Client)

To play on the same computer, open two terminal windows : one for the server and one for the client.

#### Start the Server :

#### Windows :

```
./server.exe
```

#### Linux & macOS :

```
./server
```

#### Start the Client :

#### Windows :

```
./client.exe
```

#### Linux & macOS :

```
./client
```

### Multiplayer Across Two Computers

To play with another person on a different computer :

- One computer runs the server.

- The second computer runs the client.

- Alternatively, one computer can run both the server and a client, while another computer runs an additional client.

- There can be multiple clients running on one computer, but only one server per computer.

## How to Play

### Server Functionality

Once launched, the server runs in the background and displays logs. No user interaction is required.

### Client Side

When launching the client, three buttons appear :

- PLAY

- SETTINGS

- EXIT

![](images/mainmenu.png "Main Menu")

### Play Menu

The PLAY button leads to another menu with four input fields and two buttons :

- Nickname input : Click and enter your desired username.

- IP Address input : Click and enter the server's IP.

- If playing locally or hosting, enter **127.0.0.1**.

- If the server is running on another computer, enter its IP address (e.g., 192.168.123.132).

- TCP Port and UDP Port inputs (default 8081 and 8082). Usually, these do not need modification.

- **Play Game button** : Starts the game.

- BACK button : Returns to the main menu.

![](images/undermenu.png "Under Menu")

### Settings Menu

The settings menu contains seven buttons :

- Four buttons to configure movement controls.

- One button to toggle Auto Fire.

- One button to change resolution (works after restart).

- BACK button to return to the main menu.

![](images/settings.png "Settings Menu")

#### To change movement controls :

- Click the button (e.g., MOVE_UP).

- Then press the desired key on your keyboard. The new key is saved automatically.

- For Auto Fire, it's disabled by default :

- Click Auto Fire to toggle between "YES" and "NO".

- For Resolution, it's in 720p per default :

- Click on the Resolution button to switch from 720p to 1080p, 2k and 4k. The resolution change only works after restart.

- Movement keys control ship movement, and auto fire allows automatic shooting. If auto fire is off, pressing the **Spacebar** is required to shoot.

### The EXIT button closes the game

### Lobby

- After clicking **Play Game**, the lobby screen appears.

- Displays all connected players.

- The first player to join is the Host.

- If the Host disconnects, the second player becomes the new Host automatically.

- The Host can configure game settings.

![](images/lobby.png "Lobby")

### Lobby Options

The host can modify six game settings :

- Gamemode : WAVE (enemy waves) or PVP (team-based battle).

- Number of Players : Choose between 4 and 10.

- Difficulty : Easy, Medium, or Hard (affects enemy damage and health).

- Level: Select a level from the /assets/levels directory. Each level is defined by a .txt configuration file, specifying enemy types, spawn locations, background, music, and other game parameters.

- Bonus : Yes or No (enables/disables in-game bonuses).

- Launch Game : Starts the match.

### In-Game Controls

- Use movement keys to navigate.

- Shoot enemies to survive (press **Spacebar** if Auto Fire is off).

- Avoid getting hit (you have 100 HP).

![](images/ingame.png "In Game")

### In-Game Exit

Press Esc to exit the current game and go back to the main menu.

### Game Objective

In Wave Mode, survive as long as possible and defeat enemy waves.

In PVP Mode, you’ll face off in team-based battles. The teams are formed based on the number of players in the lobby. For example, if there are three players, Player 1 and Player 3 will be on Team 1, and Player 2 will be on Team 2. If there are four players, Team 1 will consist of Player 1 and Player 3, while Team 2 will have Player 2 and Player 4. Each player starts with 300 HP, so you can take up to three hits before you’re out. Additionally, to prevent players from being eliminated too quickly, you can only be hit once per second. Be sure to avoid getting hit, as working with your team and staying alive is the key to victory.

#### Window scaling : Your dekstop fractionnal scaling may affect the game true resolution

*For feedback or suggestions, please contact us via LinkedIn or open an issue on the project repository.*

## Elements in game

### There's many entities in the game :

- Player

![](images/player.png "Player")

- Basic Enemy

![](images/enemy.png "Basic Enemy")

- Shooter Enemy

![](images/shooter.png "Shooter Enemy")

- Boss

![](images/boss.png "Boss")

### There's also bonuses :

![](images/bonus.png "Bonus")
