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
All the objects used in yart inherit from the abstract class `Object`, which defines an interface including `intersect`, `occlude` and other APIs.

Basically the yart system supports only two kinds of objects: sphere and mesh. There are other objects only for inner use such as BBox(bounding box), but you should not define such an object in the scene description file.

##### Sphere
Spere is defined with a radius and a center. Notice that it will be transformed by the transform matrix currently in the stack. So you can define ellipse by applying a scale transform on a sphere. For intersection test, the ray is first transformed into the sphere coordinate and do intersection test.
Then the result is transformed back into world coordinate.

##### Mesh
Mesh is the object you should use to represent any object that can't be represented by sphere 
(since the system supports only two kinds of objects). A mesh is composed of many triangles. 
You can use plan triangles whose normal is uniform everwhere, or you can refine the mesh by the 
following command after the definition of the mesh:
```
# Some mesh definition.
...
refineMesh
```
This will refine the mesh and calculate a normal for each vertex. When a ray hits the mesh, 
the normals is calculated by weighting the normals of the three vertex. This usually smooth the mesh.
####Lights

####BRDF
The BRDF offers the following interface:
- brdf(intersection, in, out) -> the brdf function value
- sample(intersection)        -> samples an outgoing ray according to a pdf
- pdf(intersection, out)      -> the probability of the outgoing ray.

Notice that BRDF base class provides a default implementation of `sample` and `pdf` fuctions, which samples according to a cosin-like pdf.
##### Lambertian
The lambertian BRDF reflects the incoming light in all direction uniformly. It's value is always one over pi. The normalized pdf is `cos(theta)/PI`. It uses the default sampling strategy.
####Pixel Sampler
Pixel sampler is used to generate rays for a pixel. It is important because without it there would be jaggies.
##### Uniform Sampler
A uniform pixel sampler will break a pixel into some small subpixels and shoot one ray for each subpixels. The mean of the returned color of these rays is the color of this pixel. This reduces the jaggies but cannot elminate them.
##### Jittered Sampler
Just like the uniform sampler, the jittered sampler also breaks one pixel into subpixels. However, instead of shooting one ray from the center of the subpixel, the origin of the ray is chosen randomly inside the subpixel. Generally this is better than uniform sampler due to the randomness. 
####Integrator
#####Multiple Importance Integrator
This integrator use multiple importance sampling to sovle the integral. It has two parameters: maxDepth for the recursion depth, nBSDFSamples for the number of samples taken for BSDF.
```
integrator "MultipleImportance" maxDepth nBSDFSamples
```
It samples the light and the BSDF to get a lower variance. For the weight function, it uses power heuristics. Compare with the simple ray tracing which samples the BSDF only at mirror reflection, this integrator may introduce more noise. But it is a general unbiased integrator.

####Scene Description File
Here is a full list of the commands you can use in the scene description file. You can find more samples in the test folder.
- `size 640 480` will set the size of the output image to 640x480.
- `output "area-16shadowRay.png"` will set the output image's name.
- `integrator "MultipleImportance" 5 4` will use a multiple importance sampler with 5 level recursion and 4 samples for the BSDF.
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
