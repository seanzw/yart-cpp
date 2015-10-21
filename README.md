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

####Features
#####Objects

#####Pixel Sampler
Currently yart support the following pixel samplers:
- Uniform sampler

For example, to use a 4 by 4 uniform pixel sampler, type the following line in the script.
```
pixelSampler "UniformPixelSampler" 4
```

#####Integrator
Currently yart support the following integrators:
- DirectLight

####Scene Description File
Here is a full list of the commands you can use in the scene description file.
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
