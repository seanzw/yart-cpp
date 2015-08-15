# version 120

/* This is the fragment shader for reading in a scene description, including 
   lighting.  Uniform lights are specified from the main program, and used in 
   the shader.  As well as the material parameters of the object.  */

// Mine is an old machine.  For version 130 or higher, do 
// in vec4 color;  
// in vec3 mynormal; 
// in vec4 myvertex;
// That is certainly more modern

varying vec4 color;
varying vec3 mynormal; 
varying vec4 myvertex; 

const int numLights = 10; 
uniform bool enablelighting; // are we lighting at all (global).
uniform vec4 lightposn[numLights]; // positions of lights 
uniform vec4 lightcolor[numLights]; // colors of lights
uniform int numused;               // number of lights used

// Now, set the material parameters.  These could be varying and/or bound to 
// a buffer.  But for now, I'll just make them uniform.  
// I use ambient, diffuse, specular, shininess as in OpenGL.  
// But, the ambient is just additive and doesn't multiply the lights.  

uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular; 
uniform vec4 emission; 
uniform float shininess; 

void main (void) 
{       
    if (enablelighting) {

        vec4 final_color = ambient + emission;

        vec4 vertex = gl_ModelViewMatrix * myvertex;
        vec3 v = vertex.xyz / vertex.w;
        vec3 e = normalize(vec3(0.0, 0.0, 0.0) - v);
        vec3 n = normalize(gl_NormalMatrix * mynormal);

        for (int i = 0; i < numLights; ++i) {

            if (i >= numused) {
                break;
            }

            vec3 l;
            if (abs(lightposn[i].w) >= 0.0001f) {
                l = normalize(lightposn[i].xyz / lightposn[i].w - v);   // Point light.
            } else {
                l = normalize(lightposn[i].xyz);                        // Direction lignt.
            }

            vec3 h = normalize(e + l);
            vec4 diff = diffuse * max(0.0, dot(n, l));
            vec4 spec = specular * pow(max(0.0, dot(n, h)), shininess);
            final_color += lightcolor[i] * (diff + spec);

        }

        final_color = clamp(final_color, 0.0, 1.0);
        gl_FragColor = final_color;

    } else {
        gl_FragColor = color; 
    }
}
