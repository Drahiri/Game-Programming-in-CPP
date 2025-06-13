# Game Programming in C++

Codes from the book [*Game Programming in C++*](https://github.com/gameprogcpp/code) by Sanjay Madhav. Updated to work with SDL3.  
As I was rewriting from the book and not copied repository coding style may be different.

Solutions to exercises are in respective branches. Except for Chapter 03 Exercise 1, which is math.


## Building the code
CMake is required to build.  
Create **build** directory in root directory and from inside call in terminal:
```
cmake ..
```
If using *Ninja* generator use:
```
cmake .. -GNinja
```


## Required libraries
All libraries should be inside **extern** directory in root directory or in default PATH:
* [SDL3](https://www.libsdl.org/index.php)
* [SDL_image](https://github.com/libsdl-org/SDL_image)
* [GLEW](https://glew.sourceforge.net/)
* [SOIL](https://github.com/kbranigan/Simple-OpenGL-Image-Library) - official site doesn't work
