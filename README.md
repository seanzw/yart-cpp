# Yart-cpp
Yart stands for "yet another ray tracer". This is a simple ray tracer implemented in `C++`.

Just for fun.
Build it!
-----------------------------------------------
####Windows
#####Prerequisites
- Visual Studio 2015 (for some `C++14` features).
- Cgywin with flex, bison and make installed.
- Download [FreeImage](http://freeimage.sourceforge.net/) and make sure VS can find it.
#####Parser
Open Cgywin terminal and change to the repo directory.
```
> cd src
> make
```
#####Main
Simply open the VS solution and build it. This should give you no warning.
####Ubuntu
#####Prerequisites
- Clang++ (for some `C++14` features).
- Flex, bison for the parser.
- Makepp as the build system.
- Download [FreeImage](http://freeimage.sourceforge.net/) source code, compile it and make install.

#####Parser
Exactly the same as in Windows.
```
> cd src
> make
```
#####Main
I use [makepp](http://makepp.sourceforge.net/) to build it on Ubuntu ,as it seems to be much handy than make. To build it:
```
> cd unix
> makepp
```

And that's it!

Usage
----------------------------------------------
To use it, simply type the following command in the terminal. At the end of this file you can find a full list of the commands to build the scene description file.
```
> yart-cpp.exe scene.yart
```
Objects
----------------------------------------------
All the objects used in yart inherit from the abstract class `Object`, which defines an interface including `intersect`, `occlude` and other APIs.

Basically the yart system supports only two kinds of objects: sphere and mesh. There are other objects only for inner use such as BBox(bounding box), but you should not define such an object in the scene description file.

#####Sphere
Spere is defined with a radius and a center. Notice that it will be transformed by the transform matrix currently in the stack. So you can define ellipse by applying a scale transform on a sphere. For intersection test, the ray is first transformed into the sphere coordinate and do intersection test.
Then the result is transformed back into world coordinate.

#####Mesh
Mesh is the object you should use to represent any object that can't be represented by sphere 
(since the system supports only two kinds of objects). A mesh is composed of many triangles. 
You can use plan triangles whose normal is uniform everwhere, or you can refine the mesh and calculate a normal for each vertex. When a ray hits the mesh, the normals is calculated by weighting the normals of the three vertex. This usually smooth the mesh.

Lights
----------------------------------------------
All the lights offer an interface to do intersection test, to return the emission radiance, to sample a point on the light and return the pdf of sampling a specific point. 
#####Point light
A point light is defined with its position and radiance. It's a uniform light and will never be intersected with a ray.
#####Area light
Area light is a circle light. It takes four parameters: center, radius, normal direction and power.

BSDF
----------------------------------------------
The BSDF offers the following interface:
- bsdf(intersection, in, out) -> the bsdf function value
- sample(intersection)        -> samples an outgoing ray according to a pdf
- pdf(intersection, out)      -> the probability in projected solid angle of the outgoing ray.

Notice that BSDF base class provides a default implementation of `sample` and `pdf` fuctions, which samples according to a cosin-like pdf.

#####Lambertian
The lambertian BSDF reflects the incoming light in all direction uniformly. It's value is always one over pi. The normalized pdf is `1/PI`. It uses the default sampling strategy.

#####Specular
The specular BSDF represents perfect reflection. Notice that there is a delta function in the pdf and bsdf value, which leads to problem when calculating the weight of this path in bidirectional path tracing. Here we use idea from veach's [thesis](https://graphics.stanford.edu/papers/veach_thesis/) that return values of pdf and bsdf methods are the coefficients of a delta fuction. The probability of a path should be set to zero if there is a delta function in the denominator.

#####Refraction
The refraction BSDF represents the perfect refraction module. Just like the specular BSDF, there is a delfa function in pdf and bsdf, which need to be handled carefully.

#####Cook-Torrance
Cook-Torrance BSDF is used to model the glossy material.

This Cornell box image shows an example for different materials.
<img src="outputs/cornell-box-dl.png" style="text-align:center">

Pixel Sampler
----------------------------------------------
Pixel sampler is used to generate rays for a pixel. It is important because without it there would be jaggies.

#####Uniform Sampler
A uniform pixel sampler will break a pixel into some small subpixels and shoot one ray for each subpixels. The mean of the returned color of these rays is the color of this pixel. This reduces the jaggies but cannot elminate them.

#####Jittered Sampler
Just like the uniform sampler, the jittered sampler also breaks one pixel into subpixels. However, instead of shooting one ray from the center of the subpixel, the origin of the ray is chosen randomly inside the subpixel. Generally this is better than uniform sampler due to the randomness.

#####Adaptive Sampler
The Adaptive Sampler uses a Jittered Sampler to generate some samples first and evaluates the result. The result is compared with the previous one and if the difference is trivial, it terminates. This helps the ray tracer generates more samples for the pixel which converges slower. It can also dump the samples per pixel matrix to `spp.dat`.

Integrator
----------------------------------------------
The integrator is used to solve the lighting equation, which is actually an integral.

#####Direct Light Integrator
This integrator use multiple importance sampling to sovle the integral. It has two parameters: maxDepth for the recursion depth, nBSDFSamples for the number of samples taken for BSDF.
It samples the light and the BSDF to get a lower variance. For the weight function, it uses power heuristics. Compare with the simple ray tracing which samples the BSDF only at mirror reflection, this integrator may introduce more noise. But it is a general unbiased integrator.

#####Bidirectional Path Integrator
The bidirectional path tracer in [veach's thesis](https://graphics.stanford.edu/papers/veach_thesis/) is implemented. This tracer samples the path by connecting two subpaths, one starting from the light and one from the camera. It performs much better than unidirectional path tracer when the scene is illuminated by indirect light, which are unlikely to be sampled by unidirectional path tracer. Just as mentioned in specular BSDF, special attention is needed to handle the delta function in pdf and bsdf.

Scene Description File
----------------------------------------------
Here is a real scene description file with comments. You can find more examples in the `/test` folder.
```
# Test Scene 1 
# A simple quad viewed from different camera positions

size 640 480
#size 120 80
output "area.png"

#integrator "MultipleImportance" 2 16 16
integrator "BidirectionalPath" 4 4
pixelSampler "JitteredPixelSampler" 4

camera 0 3 10 0 0 0 0 1 0 60
#camera 0 3 1 0 4 0 0 0 1 60

worldBegin

# Center Power Normal Radius NSamples
areaLight -2 4 2 70 70 70 0 -1 0 0.5
# areaLight -4 2 0 100 100 100 0 -1 0 0.5

objBegin "Mesh"
material "Lambertian"
pushTransform
translate 0 0 0
scale 100 1 100
v -1 0 -1
v -1 0 1
v  1 0 1
v  1 0 -1
f 1 2 3
f 3 4 1
popTransform
objEnd

objBegin "Mesh"
material "Lambertian"
pushTransform
translate 0 2 0
scale 1 1 1
rotate 0 1 0 45
v -1 0 -1
v -1 0 1
v  1 0 1
v  1 0 -1
v -1 -1 -1
v -1 -1 1
v  1 -1 1
v  1 -1 -1
# upper
f 1 2 3
f 3 4 1

# down
f 7 6 5
f 5 7 8

# left
f 1 5 6
f 1 6 2

# right
f 3 7 8
f 3 8 4

# front
f 2 6 7
f 2 7 3

# back
f 4 8 5
f 4 5 1

popTransform
objEnd

objBegin "Sphere"
material "Specular" 1.0 0.84 0.0
pushTransform
sphere 2 0.5 2 0.5
popTransform
objEnd

objBegin "Sphere"
material "CookTorrance" 1.0 1.0 1.0 1.0 1.5
pushTransform
sphere -2 0.5 2 0.5
popTransform
objEnd

worldEnd
```
