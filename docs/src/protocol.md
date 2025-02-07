---

title: "RFC999 - R-Type Protocol"
abbrev: "R-Type Protocol"
docname: r-type-protocol-latest
category: info

ipr: trust200902
area: General
workgroup: R-Type
keyword: Internet-Draft

stand_alone: yes
smart_quotes: no
pi: [toc, sortrefs, symrefs]

author:
-
ins: R. Scandella
name: Raphael Scandella
organization: F-TYPE V8 444 CH, 0-100 km/h 3.7s, TOP SPEED 300 km/h
email: raphael.scandella@epitech.eu

normative:

informative:


--- abstract

This document provides the informations regarding the communication protocol for the R-Type. This protocol will be used both by the client and the server.

The format for the instuctions will always be the following

CODE "SPACE" [ARGS] "HLTF"

i.e. "901 1\t\n"

--- middle

# Introduction

The communication between the server and the client will be done using TCP as well as UDP. The protocol used will be specified for each instructions and must be used accordingly by both sides.

The format that should be use is

CODE "SPACE" [ARGS] "HLTF"

The code is composed of three numbers, each ten representing a category of instructions (100 for Player, 110 for enemies, etc...), excepting the connection management who will be set at 900.

followed by a space, then the optional arguments, represented between parenthesis on the next pages.

All of the instructions will be ended by a "\t\n", which will be used by both the client and server to separate paquets.

An exemple of a valid instructions would be

"901 4\t\n"

The protocol used (TPC/UDP) will be explicitly precised, and the way the paquet is sent will also be showned

- -> From Server to Client
- <- From Client to Server
- <-> From Both

All of the communication betweet the server and clients will be obsufcated, meaning that an XOR operation will be applied on the string
using a secret key, shared by both the client and server.

i.e.
"901 4\t\n" -> "\a\000\000\000\000\000\000\000#*+:+\023\020"

# Conventions and Definitions

{::boilerplate bcp14-tagged}

# Connection handshake

This process is done when a new player connect. The process is made so the server can identify which client is associated to which UDP socket.

TCP Server--901-ClientId-->Client

UDP Server<--902-ClientId--Client

TCP Server---903-OK/KO---->Client

# Connection Management (900)

The instructions used regarding the connection of the client and server are

- Init UDP Connection (clientID) - 901 -> TCP

This is the first instruction sent to the server, its used so the client knows its id. (The server is free to resend this packet multiple times if it did not receive any response on it's UDP socket.)

- Start UDP (clientId) - 902 <- UDP

As seen in the handshake, this instructions is used to associate a UDP socket to a client.

- Authenticated client (OK/KO) - 903 -> TCP


# Player Management (100)

The following isntructions are the one used to manage players

- Player connection (clientId, x, y, name) - 101 <-> TCP

This is sent to all the clients when a new player join the game.

- Player change position(tick, clientId, x, y) - 102 <-> UDP

This instructions is sent by both the server and the client to update the position of a specific player.

- Player killed (id) - 103 -> TCP

Used when a player die, so it can be removed from the clients and server.

- Player shooting (id) - 104 <-> UDP

Allow the server to process the shoot and its collision, and the client to create a projectiles on their screen.

- Player damage (tick, id, newHealth) - 105 -> UDP

Set the player health.

- Player disconnection (id) - 106 <-> TCP

Used when a player disconnect, has the same behaviour as the instruction 103.

- Player Set Name (id, name) - 107 <- TCP
Sets the name of a player.

# Enemy Management (110)

This are the instructions used to manage the enemies

- Enemy spawn (id, type, x, y, health, wave, velocityX, velocityY) - 111 -> TCP

The type represent the enemy type (0 for basic, 1 for shooter)
Sent by the server to all the clients to spawn an enemy at a specific position.

- Enemy change position (tick, id, x, y) - 112 -> UDP

- Enemy killed (id) - 113 -> TCP

Remove an enemy if it has been shot down or if it reach the border of the screen.

- Enemy shooting (tick, id) - 114 -> UDP

Create a projectile from an enemy position. It allows the client to create a projectile at the position.

- Enemy damage (tick, id, new_health) - 115 -> UDP

Set the health of an enemy.

# Terrain management (120)

To manage the terrain, this are the instructions used

- Terrain spawn (tick, x, y) - 121 -> UDP

Spawn terrain at a specific postion on all the clients.

- Terrain destroyed (tick, x, y) - 122 -> UDP

Removed a terrain, if its has been destroyed or went off the screen.

# Game Mechanics (130)

- Set wave (waveNb) - 131 -> TCP
- Win - 133 -> TCP
- Lose - 134 -> TCP
- Game Over - 135 -> TCP
- Music change - 136 (filename) -> TCP
- Background change - 137 (filename) -> TCP

# Bonus Management (150)

- Spawn bonus (bonusID, posX, posY, wave) - 151 -> TCP
- Get bonus (playerID, bonusID, bonusType) - 152 -> TCP

# Game Management (240)

- Launch Game (id) - 241 <-> TCP
When in loby, launch the game. (Only the first connected player can send this, then the server replicates it to all connected clients).

- Set max player (id, max) - 242 <-> TCP
Set max players.

- Set difficulty (id, diff) - 243 <-> TCP
Set the difficulty of the game (1, for easy (default), 2 for medium, 3 for hard)

- Set bonus (id, state) - 244 <-> TCP
Set if bonus appears on the map (bool)

- Set level (id, levelId)- 245 <-> TCP
Set the level config map

- Set game mode (id, mode) - 246 <-> TCP
Set the game mode, 0 for wave, 1 for pvp

- Send levels (id, mode) - 247 <-> TCP
Send number of levels available in assets/levels
