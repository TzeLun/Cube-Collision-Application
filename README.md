# Cube-Collision-Application
This is an old side project on developing a physics demonstration application via OpenGL, NVIDIA PhysX and C++. Unfortunately, most of the configuration files were lost as it was an old project. However, the core source code, shaders, and assets are available in this repository.

## Description
The goal was to showcase NVIDIA PhysX SDK to equip graphical objects with physics, mainly object-object collision. The application features a huge static cube at the center stage and user can strafe around the environment through keyboard controls and spawn smaller cubes as projectiles. Watch how the cubes collide and stack onto each other in a pile. The graphics are rendered with OpenGL using GLSL shader programming. The program and logic are all written in C and C++.

## Libraries
Below are the important libraries used in this code:
- glfw, glm, glad
- NVIDIA PhysX
- stb_image (already provided in the src folder)

## Setup
Back then the code was compiled and run using Microsoft Visual Studio. These days, you can create a CMAKE file to streamline configuration and build setup. Once compiled and built, run `Main.cpp` to launch the application. You might need to edit the paths for certain files, ie.: shaders etc.

## Demo
If run successfully, the application appears as a pop-up window: </br>
[![demo](https://github.com/TzeLun/Cube-Collision-Application/blob/main/Cube%20Collision%20Application.JPG)](https://github.com/TzeLun/Cube-Collision-Application/blob/main/CCA_Demo.mp4)
</br>
Check out the [demo video here](https://github.com/TzeLun/Cube-Collision-Application/blob/main/CCA_Demo.mp4) if interested. 

## Controls
|Keys|Control|
|----|------|
|A|Left|
|W|Forward|
|S|Backward|
|D|Right|
|Mouse left click|Spawn cubes|
|Mouse hover|Cursor view change|
