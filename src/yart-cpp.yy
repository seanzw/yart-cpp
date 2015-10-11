
%{
 
#include <iostream>
#include <string>
#include "RayTracer.h"

/* The yart object */
extern RayTracer *yart;
extern int yylex();
void yyerror(const std::string &s);

extern void include_push(char *filename);
int line_num = 0;
string current_file;

bool faceIndexStartFromOne = false;		/* For obj file. */

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
%token INTEGRATOR
%token PIXELSAMPLER
%token OUTPUT
%token OBJBEGIN
%token OBJEND
%token INCLUDE
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
	yart->yartSize((int)$2, (int)$3);
}

| INTEGRATOR STR NUMBER {
	DEBUG("PARSE INTEGRATOR %s %d\n", $2, (int)$3);
	string integrator($2);
	yart->yartIntegrator(integrator, (int)$3);
}

| PIXELSAMPLER STR NUMBER {
	DEBUG("PARSE PIXELSAMPLER %s %d\n", $2, (int)$3);
	string pixelSampler($2);
	yart->yartPixelSampler(pixelSampler, (int)$3);
}
	
| OUTPUT STR {
	string outfn($2);
	DEBUG("PARSE OUTPUT %s\n", outfn.c_str());
	yart->yartOutput(outfn);
}

| INCLUDE STR {
	include_push($2);
}

| OBJBEGIN STR {
	string type($2);
	yart->yartObjBegin(type);
}

| OBJEND {
	yart->yartObjEnd();
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
	yart->yartCamera(eye, center, up, $11);
}

| MAXVERTS NUMBER {

}

| MAXVERTNORMS NUMBER {

}

| VERTEX NUMBER NUMBER NUMBER {
	vec3 v($2, $3, $4);
	yart->yartVertex(v);
	// DEBUG("PARSE VERTEX %.2f %.2f %.2f\n", v[0], v[1], v[2]);
}

| TRI NUMBER NUMBER NUMBER {
	
	int id1 = (int)$2;
	int id2 = (int)$3;
	int id3 = (int)$4;
	if (faceIndexStartFromOne) {
		id1--; id2--; id3--;
	}
	yart->yartTri(id1, id2, id3);
	// DEBUG("PARSE TRI %d %d %d\n", id1, id2, id3);
}

| SPHERE NUMBER NUMBER NUMBER NUMBER {
	vec3 center($2, $3, $4);
	yart->yartSphere(center, $5);
	DEBUG("PARSE SPHERE c %.2f %.2f %.2f | r %.2f\n",
		center[0], center[1], center[2], $5);
}

| TRANSLATE NUMBER NUMBER NUMBER {
	yart->yartTranslate($2, $3, $4);
}

| SCALE NUMBER NUMBER NUMBER {
	yart->yartScale($2, $3, $4);
}

| ROTATE NUMBER NUMBER NUMBER NUMBER {
	vec3 axis($2, $3, $4);
	yart->yartRotate(axis, $5);
}

| PUSHTRANSFORM {
	yart->yartPushTransform();
}

| POPTRANSFORM {
	yart->yartPopTransform();
}

| DIRECTIONAL NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER {
	vec3 direction($2, $3, $4);
	vec3 color($5, $6, $7);
	yart->yartDirectional(direction, color);
	DEBUG("PARSE DIRECTIONAL d %.2f %.2f %.2f | c %.2f %.2f %.2f\n",
		direction[0], direction[1], direction[2],
		color[0], color[1], color[2]);
}

| POINT NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER {
	vec3 direction($2, $3, $4);
	vec3 color($5, $6, $7);
	yart->yartPoint(direction, color);
	DEBUG("PARSE POINT  d %.2f %.2f %.2f | c %.2f %.2f %.2f\n",
		direction[0], direction[1], direction[2],
		color[0], color[1], color[2]);
}

| ATTENUATION NUMBER NUMBER NUMBER {
	vec3 a($2, $3, $4);
	yart->yartAttenuation(a);
}

| DIFFUSE NUMBER NUMBER NUMBER {
	vec3 a($2, $3, $4);
	yart->yartDiffuse(a);
	DEBUG("PARSE DIFFUSE %.2f %.2f %.2f\n", a[0], a[1], a[2]);
}

| AMBIENT NUMBER NUMBER NUMBER {
	vec3 a($2, $3, $4);
	yart->yartAmbient(a);
	DEBUG("PARSE AMBIENT %.2f %.2f %.2f\n", a[0], a[1], a[2]);
}

| SPECULAR NUMBER NUMBER NUMBER {
	vec3 a($2, $3, $4);
	yart->yartSpecular(a);
	DEBUG("PARSE SPECULAR %.2f %.2f %.2f\n", a[0], a[1], a[2]);
}

| EMISSION NUMBER NUMBER NUMBER {
	vec3 a($2, $3, $4);
	yart->yartEmission(a);
	DEBUG("PARSE EMISSION %.2f %.2f %.2f\n", a[0], a[1], a[2]);
}

| SHININESS NUMBER {
	yart->yartShininess($2);
}

| BUILDOCTREE NUMBER {
	yart->yartBuildOCTree((int)$2);
};


%%

void yyerror(const std::string &s) {
    std::cerr << s << std::endl;
}