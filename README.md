# Yart-cpp
Yart stands for "yet another ray tracer". This is a simple ray tracer implemented in `C++`.

Just for fun.

####Build
#####Flex/Bison
Make sure you have cgywin and installed flex, bison and make. Then you can generate the lexer and parser simply by typing the following command in terminal:
```
> cd src
> make
```
#####Main
This program depends on FreeImage to store the image amd glm for linear algebra. Make sure you have these. Simply build it in VS. This should give you no warning.

####Usage
To use it, simply type the following command in the terminal. At the end of this file you can find a full list of the commands to build the scene description file.
```
> yart-cpp.exe scene.yart
```
####Objects
All the objects used in yart inherit from the abstract class `Object`, which defines an interface including `intersect`, `occlude` and other API.

Basically the yart system supports only two kinds of objects: sphere and mesh. There are other objects only for inner use such as BBox(bounding box), but you should not define such an object in the scene description file.

The mesh supports only triangle.
####Lights

####BRDF
##### Lambertian
####Pixel Sampler
Currently yart support the following pixel samplers:
##### Uniform Sampler
A uniform pixel sampler will break a pixel into some small subpixels and shoot one ray for each subpixels. The mean of the returned color of these rays is the color of this pixel. This reduces the jaggies but cannot elminate them.
##### Jittered Sampler
Just like the uniform sampler, the jittered sampler also breaks one pixel into subpixels. However, instead of shooting one ray from the center of the subpixel, the origin of the ray is chosen randomly inside the subpixel. Generally this is better than uniform sampler due to the randomness. 
####Integrator
Currently yart support the following integrators:
- DirectLight

####Scene Description File
Here is a full list of the commands you can use in the scene description file. You can find more samples in the test folder.
- `size 640 480` will set the size of the output image to 640x480.
- `output "area-16shadowRay.png"` will set the output image's name.
- `integrator "DirectLight" 5` will set the integrator to be used.
- `pixelSampler "UniformPixelSampler" 4` will set the pixel sampler to be used.
- `camera center lookAt up fov(degrees)` defines a camera at `center` looking at `lookAt`. Fov is defined in degrees.
- `worldBegin` starts the definition of the world, including lights, objects, materials.
- `worldEnd` ends the world definition stage, as you can image.
- `objBegin "Type"` starts to define a new object.
- `objEnd` means we are done with the previous object.
- `material "Type"` defines the material for an object.
- `translate`, `scale`, `rotate` will mulitply a transform matrix on the top of the transform stack.
- `pushTransform`, `popTransform` manipulate the transform stack.
- `v x y z` defines a vertex
- `f id1 id2 id3` defines a face with three vertex index.
- `buildOCTree` build an OC Tree for the current object, usually for a complicate mesh.
- `include "filename"` will include the file at this postion. This is usually used to include other big `.obj` file.
