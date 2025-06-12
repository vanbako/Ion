# Ion

**Ion** is a render/game engine written in C++20. It was created as a learning tool for building a multithreaded engine from scratch.

Ion currently uses:

- Direct3D 12
- ~~Direct2D~~
- DirectXTK12
- NVidia PhysX 4.1
- FMOD 2.01.10

## Features

- Multiple windows and canvases with multithreaded rendering
- Scenes, objects and components (Model, View, Controller)
- Scene divided into ModelC, ViewC and ControllerC "Cubes" for optimized updates and rendering (LOD planned)
- Materials, meshes and textures
- Input using the command pattern (keyboard)
- Hardware instancing
- Animation *(demo contains artifacts due to bad skinning)*
- Terrain (heightmap, PhysX)
- Instanced PhysX
- Audio and logging using the ServiceLocator pattern
- Steering behaviour (Wander, Seek)
- ~~Direct2D Text~~
- Threads for controller components, model components, view components, canvas and physics
- D2D removed permanently due to issues with multithreaded texture loading

## Getting Started

You can upvote enhancements in the GitHub issue tracker.

You'll need **Windows SDK 10.0.22000.0** to build.

Let me know if you're interested in contributing.
