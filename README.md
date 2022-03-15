# Ion
Ion Render/Game Engine<br>
It is meant as a learning tool for developing a game engine in C++20, multithreaded, ...<br>
Using: Direct3D 12, <del>Direct2D</del>, DirectXTK12, NVidia Physx 4.1, FMOD 2.01.10<br>
<br>
Current Features:<br>
Multiple Windows & Canvases with multithreaded rendering<br>
Scenes - Objects - Components (Model, View, Controller)<br>
Scene divided in Cubes (per material) for faster rendering and LOD usage (LOD's not implemented yet)<br>
Materials, Meshes, Textures<br>
Input using command pattern: keyboard<br>
Hardware Instancing<br>
Animation<br>
Terrain (heightmap, physx) <br>
Instanced physx<br>
Audio, Logger using ServiceLocator pattern<br>
Steering behaviour (Wander, Seek)<br>
<del>Direct2D Text</del><br>
Threads: ControllerComponents, ModelComponents, ViewComponents, Canvas, Physics<br>
D2D removed temporary because it was interfering with texture loading in combination with multithreaded rendering<br>
<br>
You can upvote enhancements in the Github Issue Tracker<br>
<br>
https://discord.gg/72Xbv5Rwfa<br>
<br>
You'll need Windows SDK 10.0.22000.0 to build<br>
<br>
Let me know if you're interested in contributing<br>