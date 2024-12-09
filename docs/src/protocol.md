# Protocol R-Type

format : CODE SP <?ARGS> HTLF

- ➡️ : From Server to Client
- ⬅️ : From Client to Server
- 🔄 : From Both

## List of actions needed

- Connection Management (900)
    - Init UDP Connection (client_id) - 901 ➡️ [TCP]
    - Start UDP (client_id) - 902 ⬅️ [UDP]
    - Authenticated client (OK/KO) - 903 ➡️ [TCP]

- Player (100)

    - Player connection (id, x, y) - 101 🔄
    - Player change position (id, x, y) - 102 🔄
    - Player killed (id) - 103 ➡️
    - Player shooting (id, x, y) - 104 🔄
    - Player damage (id, amount) - 105 ➡️
    - Player disconnection (id) - 106 🔄

- Enemy (110)

    - Enemy spawn (id, x, y) - 111 ➡️
    - Enemy change position (id, x, y) - 112 ➡️
    - Enemy killed (id) - 113 ➡️
    - Enemy shooting (id) - 114 ➡️
    - Enemy damage (id, amount) - 115 ➡️

- Terrain (120)

    - Terrain spawn (x, y) - 121 ➡️
    - Terrain destroyed (x, y) - 122 ➡️

- Game mechs (130)
    - New wave (wave_nb) - 131 ➡️
    - Wave over (wave_nb) - 132 ➡️
    - Win - 133 ➡️
    - Lose - 134 ➡️
    - Game Over - 135 ➡️
