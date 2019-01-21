# Chroma

Chroma is a real-time physically based rendering engine for Android, iOS, Linux, macOS, and Windows. It is designed to be as small as possible and as efficient as possible on Android.

Chroma is based on Google's Filament, which is currently used in the
[Sceneform](https://developers.google.com/ar/develop/java/sceneform/) library both at runtime on
Android devices and as the renderer inside the Android Studio plugin.

## Features

### APIs

- Native C++ API for Android, iOS, Linux, macOS and Windows
- Java/JNI API for Android, Linux, macOS and Windows
- JavaScript API

### Backends

- OpenGL 4.1+ for Linux, macOS and Windows
- OpenGL ES 3.0+ for Android and iOS
- Vulkan 1.0 for Android, Linux, macOS and iOS (with MoltenVk), and Windows
- WebGL 2.0 for all platforms

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

- IES light profiles
- Area lights
- Fog
- Color grading
- Bloom
- TAA
- etc.
