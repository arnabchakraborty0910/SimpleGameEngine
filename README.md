# SimpleGameEngine

A small C++ / OpenGL 3.3 engine built while following LearnOpenGL, then split into engine, app, and editor folders. It is a learning project, not a shipped engine.

The demo shows a large textured floor, a scene of cubes, an orbiting point light, simple rigid-body physics, and an ImGui editor. Press R to throw a cube from the camera.

## How to run

1. Open `SimpleGameEngine.sln` in Visual Studio 2026.
2. Set the configuration to **Debug** and the platform to **x64**.
3. Leave **Debugging → Working Directory** blank (defaults to `$(ProjectDir)`), or set it to `$(ProjectDir)`. Shader and texture paths are loaded from that folder, not from `app/`.
4. Build and press **F5**.

Dependencies already live in `vendor/` (GLFW, GLAD, GLM, stb_image, Dear ImGui). This is a hand-made `.vcxproj`, not CMake. Do not edit files under `vendor/`.

## Controls

| Key / mouse | Action |
|---|---|
| W A S D | Move camera |
| Mouse | Look (when the editor is closed) |
| Scroll | Zoom |
| Tab | Toggle ImGui editor (releases or captures the cursor) |
| R | Throw a cube from the camera |
| Escape | Quit |

While the editor is open, mouse look is disabled so you can use ImGui. The engine ignores camera keys and look when ImGui wants the keyboard or mouse (`WantCaptureKeyboard` / `WantCaptureMouse`).

## Layout

```
app/                  Demo executable. main.cpp still builds the sample scene.
engine/core/          Window, Clock (dt), Input, Application loop
engine/renderer/      Shader, Mesh, Texture, Camera, Renderer
engine/scene/         Entity, Transform, Light, Scene
engine/physics/       PhysicsWorld, RigidBody
editor/               ImGui panels and spawn
shaders/              GLSL loaded at runtime
textures/             Images loaded at runtime
vendor/               Third-party libraries (do not edit)
```

On disk the core folder may appear as `engine/Core`. Windows treats that the same as `engine/core`.

## What still lives in main

`app/main.cpp` constructs `Application`, the demo meshes, shaders, textures, `Scene`, `PhysicsWorld`, and `Editor`. It still wires GLFW callbacks, bootstraps ImGui, draws the floor, and orbits the light. The long-term shape is a thinner `main` that constructs those objects and calls `Application::run`.

## Include order

Always include `glad/glad.h` before `GLFW/glfw3.h`.

