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
```
> yart-cpp.exe scene.yart
```
