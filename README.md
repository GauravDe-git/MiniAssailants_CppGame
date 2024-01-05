# Mini Assailants

This is my intake assignment for the BUAS intake 2024 - Programming. Initially conceived as a project for the C++ for Games Microcredentials Course at BUAS, the project's scope posed a considerable challenge for me as a beginner in C++. Regrettably, I fell short of my expectations by the end of the microcredentials course, only managing to create a rudimentary game in about a month and a half. The initial outcome featured a basic player moving forward and defeating a single enemy with a punch. Although I was far from proud of my initial work, I persevered and continued refining the project. After investing an additional month and a half, I was finally able to bring the game to a somewhat presentable state, aligning with my initial game design vision. Throughout this journey, I received invaluable assistance from various individuals, particularly Jeremiah and Bert during the microcredentials course. Subsequently, I garnered further feedback in the 3dgep server CPP Fast Track Discord events. Without their support, creating the game would have been an insurmountable task for me.

## Game Overview
Mini Assailant's is a 2D Beat'em Up fighting game that draws inspiration from childhood favorites like Hero Fighter and Little Fighter 2, both still beloved indie games with a strong fanbase. In this game, players command a formidable fighter with special abilities such as a fire punch and uppercut. Engaging in intense button-mashing, players confront incoming enemies. Special attacks consume 'Mana Points' (MP), and defeating enemies offers a chance to obtain health (hp) or mana (mp) potions, along with collectible coins. The game spans three levels: the first set in a cyberpunk city with goblins and skeletons, leading to a powerful golem boss; the second in an abandoned industry site featuring harpies and centaurs, concluding with a formidable gargoyle boss; and the final level in an experimental lab where agile cerberus creatures culminate in a showdown with the ultimate boss, the Flying Eye. Players can select their desired level from the menu.

## Controls

- Movement: WASD/Arrow keys
- Light Attack: H (double tap for light attack 2)
- Heavy Attack: J (double tap for heavy attack 2)
- Special Attack 1: Y
- Special Attack 2: U
- Pause Game: P
- Toggle VSync: V
- Toggle Fullscreen/Windowed: F11
- Quit Game: Escape

## Built With

The game is built using the C++ For Games Framework created by Jeremiah Van Oosten (@JPVanOosten). 

The framework includes:

- 2D software rasterizer in the Image class.
- Support for gamepads (currently using XInput on Windows)
- Spatialized audio library with support for wav, mp3, ogg, flac audio file sources.
- Waveform class for creating custom waveforms.
- Math helpers in the math library (AABB, Camera2D, Transform2D, etc...)

Check out the framework [here](https://github.com/jpvanoosten/SoftwareRasterizer).

## Credits

### Sprites

- **Player Sprites:** [Sagak Art Pururu](https://sagak-art-pururu.itch.io/)
- **Enemy Sprites:** [MattzArt](https://xzany.itch.io/)
- **Background Sprites:** [Ansimuz](https://ansimuz.itch.io/)
- **Potion Sprites:** [ProjectTea](https://projecttea.itch.io/potion-icons-volume-1)
- **Coin Sprites:** [Totus Lotus](https://totuslotus.itch.io/pixel-coins)
- **Button Sprites:** [Hio Respace](https://hiorespace.itch.io/buttonsanimationpack)

### Sounds

- [Punch Sound](https://mixkit.co/free-sound-effects/punch/)
- [Sword Sound](https://mixkit.co/free-sound-effects/sword/)
- [RPG Sound Pack](https://opengameart.org/content/rpg-sound-pack)
- [Coin Sound](https://pixabay.com/sound-effects/search/game-coin/)
- [Hits/Punches Sound](https://opengameart.org/content/37-hitspunches)

### Fonts

- [Tafel Sans Pro Bold](https://en.bestfonts.pro/font/tafel-sans-pro)
- [Symtext Font](https://www.dafont.com/symtext.font)

### Exe Icon

- [Portable Icon](https://www.freepik.com/icon/portable_1707147#fromView=keyword&term=2d+Game&page=1&position=36&uuid=f16f4fda-a231-41fe-981d-a54735e83ee3)

Feel free to explore the game, contribute, or provide feedback!
