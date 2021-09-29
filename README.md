# C-SDL-Raytracer
Raytracing using SDL in C.

### Abstract
Application 16x16 icons are from http://www.forrestwalter.com/icons/

Just a simple ray tracer that uses spheres.

Lambertian/Fullbright shaded with the light source being behind the ray origin (camera).

---

### Notes

Initially thought this would be a fun idea, to create a simplified raytraced renderer for a game engine that utilises mutliple CPU cores. Until I realised that sending a 3.14mb screen buffer of only 1024x768 a minimum of 30 times per second was ~100MB of data per second. Which means really raytracing on a CPU is only worth while if a render is going to take a long time before you bother trying to display it.

Also I have some aliasing artifact that the original C++ code I transcribed the sphere intersection from does not have. No idea what is causing it but it seems to be something to do with the rays diverging more as they travel farther from the screen.

I am not sure if I am going to continue this project because it just seems a little insane to do all this just so that I can raytrace some (at best) lambaritan spheres at ~30fps.

Also multi-threaded mod does not seem to render anything on my RTX graphics card. But it seems to work fine on my integrated Intel Iris XE. ¯\\_(ツ)_/¯ not that multi-threading a real-time raytracer has much benefit anyway.

If you are compiling on windows unzip SDL2.zip to the project directory.

To launch multi-threaded: ./ray multi

But only single threaded has crude sphere motion to demonstrate the distance aliasing.

Compile Linux: ./compile.sh
Compile Windows: ./compile.bat

Compile for Windows on Linux: ./compile_win.sh

---

### Origin

This is a C implementation of:<br>
https://www.scratchapixel.com/code.php?id=3&origin=/lessons/3d-basic-rendering/introduction-to-ray-tracing&src=0

Which is part of this article:<br>
https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/how-does-it-work

Which is written by Jean-Colas Prunier who runs Scratchapixel, more information about Jean can be found here:<br>
https://www.scratchapixel.com/about.php

---

### References

https://fabiensanglard.net/rayTracing_back_of_business_card/<br>
https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/how-does-it-work<br>
https://raytracing.github.io/<br>
https://github.com/define-private-public/PSRayTracing (very cool project by Benjamin Summerton)
