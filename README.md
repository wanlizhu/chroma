# Chroma

Chroma is a real-time physically based rendering engine for Windows. It is designed to be as small as possible and as efficient as possible.

## Features

### APIs

- Native C++ API for Windows
- Java/JNI API for Android, Linux, macOS and Windows
- Python API

### Backends

- OpenGL 4.1+ for Windows
- Vulkan 1.0 for Windows

### Rendering

- Clustered forward renderer
- Cook-Torrance microfacet specular BRDF
- Lambertian diffuse BRDF
- HDR/linear lighting
- Metallic workflow
- Clear coat
- Anisotropic lighting
- Approximated translucent (subsurface) materials (direct and indirect lighting)
- Cloth shading
- Normal mapping & ambient occlusion mapping
- Image-based lighting
- Physically-based camera (shutter speed, sensitivity and aperture)
- Physical light units
- Point light, spot light and directional light
- ACES-like tone-mapping
- Temporal dithering
- FXAA or MSAA
- Dynamic resolution (on Android)

### Future

Many other features have been either prototyped or planned:

- Support MacOS and Linux platforms
- IES light profiles
- Area lights
- Fog
- Color grading
- Bloom
- TAA
- etc.
