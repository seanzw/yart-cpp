
%{
 
#include <iostream>
#include <string>
#include "RayTracer.h"

/* The yart object */
extern RayTracer *yart;
extern int yylex();
void yyerror(const std::string &s);

%}

%union {
    char string[1024];
    float num;
}

%token NUMBER
%type <num> NUMBER
%token STR
%type <string> STR
%token SIZE
%token MAXDEPTH
%token OUTPUT
%token CAMERA
%token MAXVERTS
%token MAXVERTNORMS
%token VERTEX
%token TRI
%token SPHERE
%token TRANSLATE
%token SCALE
%token ROTATE
%token PUSHTRANSFORM
%token POPTRANSFORM
%token DIRECTIONAL
%token POINT
%token ATTENUATION
%token DIFFUSE
%token AMBIENT
%token SPECULAR
%token EMISSION
%token SHININESS
%token BUILDOCTREE

%%

start: yart_stmt_list {

};

yart_stmt_list: yart_stmt_list yart_stmt {

}

| yart_stmt {

};

yart_stmt: SIZE NUMBER NUMBER {
    DEBUG("PARSE SIZE %d, %d\n", (int)$2, (int)$3);
	yart->yart_size((int)$2, (int)$3);
}

| MAXDEPTH NUMBER {
	DEBUG("PARSE MAXDEPTH %d\n", (int)$2);
	yart->yart_maxdepth((int)$2);
}
	
| OUTPUT STR {
	string outfn($2);
	DEBUG("PARSE OUTPUT %s\n", outfn.c_str());
	yart->yart_output(outfn);
}

| CAMERA NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER {
	vec3 eye($2, $3, $4);
	vec3 center($5, $6, $7);
	vec3 up($8, $9, $10);
	DEBUG("PARSE CAMERA e %.2f %.2f %.2f | c %.2f %.2f %.2f | u %.2f %.2f %.2f | f %.2f\n",
                        eye[0], eye[1], eye[2],
                        center[0], center[1], center[2],
                        up[0], up[1], up[2],
                        $11);
	yart->yart_camera(eye, center, up, $11);
}

| MAXVERTS NUMBER {

}

| MAXVERTNORMS NUMBER {

}

| VERTEX NUMBER NUMBER NUMBER {
	vec3 v($2, $3, $4);
	yart->yart_vertex(v);
	// DEBUG("PARSE VERTEX %.2f %.2f %.2f\n", v[0], v[1], v[2]);
}

| TRI NUMBER NUMBER NUMBER {
	int id1 = (int)$2;
	int id2 = (int)$3;
	int id3 = (int)$4;
	yart->yart_tri(id1, id2, id3);
	// DEBUG("PARSE TRI %d %d %d\n", id1, id2, id3);
}

| SPHERE NUMBER NUMBER NUMBER NUMBER {
	vec3 center($2, $3, $4);
	yart->yart_sphere(center, $5);
	DEBUG("PARSE SPHERE c %.2f %.2f %.2f | r %.2f\n",
		center[0], center[1], center[2], $5);
}

| TRANSLATE NUMBER NUMBER NUMBER {
	yart->yart_translate($2, $3, $4);
}

| SCALE NUMBER NUMBER NUMBER {
	yart->yart_scale($2, $3, $4);
}

| ROTATE NUMBER NUMBER NUMBER NUMBER {
	vec3 axis($2, $3, $4);
	yart->yart_rotate(axis, $5);
}

| PUSHTRANSFORM {
	yart->yart_pushTransform();
}

| POPTRANSFORM {
	yart->yart_popTransform();
}

| DIRECTIONAL NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER {
	vec3 direction($2, $3, $4);
	vec3 color($5, $6, $7);
	yart->yart_directional(direction, color);
	DEBUG("PARSE DIRECTIONAL d %.2f %.2f %.2f | c %.2f %.2f %.2f\n",
		direction[0], direction[1], direction[2],
		color[0], color[1], color[2]);
}

| POINT NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER {
	vec3 direction($2, $3, $4);
	vec3 color($5, $6, $7);
	yart->yart_point(direction, color);
	DEBUG("PARSE POINT  d %.2f %.2f %.2f | c %.2f %.2f %.2f\n",
		direction[0], direction[1], direction[2],
		color[0], color[1], color[2]);
}

| ATTENUATION NUMBER NUMBER NUMBER {
	vec3 a($2, $3, $4);
	yart->yart_attenuation(a);
}

| DIFFUSE NUMBER NUMBER NUMBER {
	vec3 a($2, $3, $4);
	yart->yart_diffuse(a);
	DEBUG("PARSE DIFFUSE %.2f %.2f %.2f\n", a[0], a[1], a[2]);
}

| AMBIENT NUMBER NUMBER NUMBER {
	vec3 a($2, $3, $4);
	yart->yart_ambient(a);
	DEBUG("PARSE AMBIENT %.2f %.2f %.2f\n", a[0], a[1], a[2]);
}

| SPECULAR NUMBER NUMBER NUMBER {
	vec3 a($2, $3, $4);
	yart->yart_specular(a);
	DEBUG("PARSE SPECULAR %.2f %.2f %.2f\n", a[0], a[1], a[2]);
}

| EMISSION NUMBER NUMBER NUMBER {
	vec3 a($2, $3, $4);
	yart->yart_emission(a);
	DEBUG("PARSE EMISSION %.2f %.2f %.2f\n", a[0], a[1], a[2]);
}

| SHININESS NUMBER {
	yart->yart_shininess($2);
}

| BUILDOCTREE NUMBER {
	yart->yart_buildOCTree((int)$2);
};


%%

void yyerror(const std::string &s) {
    std::cerr << s << std::endl;
}