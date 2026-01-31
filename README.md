# RayTracer - Native Export Layer

## Part 1: C++ Export (`RayTracing/native/export.cpp`)

C-compatible ABI 

### Methods I added for the exposed ABI

**Opaque Handles**: `SceneHandle` and `MaterialHandle` for C++/C# boundary

**Materials**: 
- `CreateLambertianMaterial(r, g, b)` / `DestroyMaterial()`
- `CreateDielectricMaterial(refraction_index)` / `DestroyMaterial()`

**Scene**:
- `CreateScene()` / `DestroyScene()` / `ClearScene()`
- `AddSphere(center_x/y/z, radius, material)`

**Rendering**:
- `RenderScene(scene, CameraConfig*, buffer, callback)` - Creates a scene where this camera will be there taken from CameraConfig
- `RenderCallback`: `void (*callback)(int samples, uint8_t* buffer)`

**Output**:
- `SavePng(filename, buffer, width, height)` - PNG export via stb_image_write


## TODO: Part 2 -> 4
