# Space Foosball  

## Overview  
**Space Foosball** is a fast-paced 2D arcade game where two players control some **child ships** to deflect meteors into the opponent's **motherships** ship. The game features a simple yet effective physics system that ensures accurate deflection of meteors.  

## Features  
- **Multiplayer**: Two players compete head-to-head.  
- **Physics-based Gameplay**: Meteors react dynamically to collisions.  
- **Retro Space Theme**: Classic arcade-style visuals and sounds.  
- **Powered by SDL2**: Developed using SDL2 and its extensions for graphics, sound, and text rendering.  

## Technologies Used  
- **Language**: C++  
- **Frameworks**:  
  - [SDL2](https://www.libsdl.org/) – Core library for rendering and event handling
  - [SDL2_image](https://github.com/libsdl-org/SDL_image) – For loading images  
  - [SDL2_mixer](https://github.com/libsdl-org/SDL_mixer) – For handling audio
  - [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf) – For text rendering
- **IDE**: Visual Studio  

## Installation & Setup  
### Prerequisites  
- Install **SDL2** and required extensions (SDL2_image, SDL2_mixer, SDL2_ttf).  
- A C++ compiler (preferably **Visual Studio**).  

### Build Instructions  
1. Clone this repository:  
   ```sh  
   https://github.com/TranNgocLamVy/Space-foosball.git 
   cd Space-foosball
   ```  
2. Open the **Visual Studio solution** file (`.sln`).  
3. Configure the include and library directories for SDL2 and its extensions.  
4. Build and run the project.  

## Controls  
- **Player 1 (Left Side)**:  
  - Control child ships: `W / S`  
  - Switch ships: `A / D`  

- **Player 2 (Right Side)**:  
  - Control child ships: `Up / Down Arrow`  
  - Switch ships : `Left / Right Arrow`  

## License  
This project is released under the **MIT License**. Feel free to modify and distribute it.  

## Future Improvements  
- AI opponent for single-player mode.  
- More meteor types with different physics behaviors. 
- Power-ups and special abilities. 
