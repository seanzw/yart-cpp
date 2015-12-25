
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

%}

%union {
    char string[1024];
    float num;
    vector<float> *ps;                  /* Always remember to delete this pointer... */
}

%token NUMBER
%type <num> NUMBER
%token STR
%type <string> STR
%token SIZE
%token INTEGRATOR
%token PIXELSAMPLER
%token OUTPUT
%token WORLDBEGIN
%token WORLDEND
%token OBJBEGIN
%token OBJEND
%token INCLUDE
%token CAMERA
%token MAXVERTS
%token MAXVERTNORMS
%token VERTEX
%token TRI
%token SPHERE
%token REFINEMESH
%token TRANSLATE
%token SCALE
%token ROTATE
%token PUSHTRANSFORM
%token POPTRANSFORM
%token DIRECTIONAL
%token POINT
%token AREALIGHT
%token ENVLIGHT
%token ATTENUATION
%token MATERIAL
%token BUILDOCTREE

%type <ps> param_list

%%

start: yart_stmt_list {

};

yart_stmt_list: yart_stmt_list yart_stmt {

}

| yart_stmt {

};

param_list: {
    $$ = new vector<float>();
}

| param_list NUMBER {
    $$ = $1;
    $$->push_back($2);
}

yart_stmt: SIZE NUMBER NUMBER {
    DEBUG("PARSE SIZE %d, %d\n", (int)$2, (int)$3);
	yart->yartSize((int)$2, (int)$3);
}

| INTEGRATOR STR param_list {
	DEBUG("PARSE INTEGRATOR %s\n", $2);
	string integrator($2);
	yart->yartIntegrator(integrator, $3);
    delete $3;
}

| PIXELSAMPLER STR param_list {
	DEBUG("PARSE PIXELSAMPLER %s %f\n", $2, (*$3)[0]);
	string pixelSampler($2);
	yart->yartPixelSampler(pixelSampler, $3);
    delete $3;
}
	
| OUTPUT STR {
	string outfn($2);
	DEBUG("PARSE OUTPUT %s\n", outfn.c_str());
	yart->yartOutput(outfn);
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

| world {

};

world: WORLDBEGIN world_stmt_list WORLDEND {

};

world_stmt_list: world_stmt_list world_stmt {

}

| world_stmt {

};

world_stmt: DIRECTIONAL NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER {
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

| AREALIGHT NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER {
    vec3 center($2, $3, $4);
	vec3 color($5, $6, $7);
    vec3 normal($8, $9, $10);
    float radius($11);
    DEBUG("PARSE AREALIGHT d %.2f %.2f %.2f | c %.2f %.2f %.2f\n",
		center[0], center[1], center[2],
		color[0], color[1], color[2]);
    yart->yartAreaLight(center, color, normal, radius);
}

| ENVLIGHT STR {

    size_t pos = current_file.find_last_of('/');
    string fn;
    if (pos != string::npos) {
        fn = current_file.substr(0, pos + 1);
        fn.append($2);
    }
    yart->yartEnvLight(fn);
}

| ATTENUATION NUMBER NUMBER NUMBER {
	vec3 a($2, $3, $4);
	yart->yartAttenuation(a);
}

| OBJBEGIN obj_type obj_stmt_list OBJEND {
    yart->yartObjEnd();
};

obj_type: STR {
    string type($1);
	yart->yartObjBegin(type);
    DEBUG("PARSE OBJBEGIN %s\n", type.c_str());
}

obj_stmt_list: obj_stmt_list obj_stmt {

}

| obj_stmt {

};

obj_stmt: MAXVERTS NUMBER {

}

| MAXVERTNORMS NUMBER {

}

| VERTEX NUMBER NUMBER NUMBER {
	vec3 v($2, $3, $4);
	yart->yartVertex(v);
	// DEBUG("PARSE VERTEX %.2f %.2f %.2f\n", v[0], v[1], v[2]);
}

| TRI NUMBER NUMBER NUMBER {
	
	int id1 = (int)$2 - 1;
	int id2 = (int)$3 - 1;
	int id3 = (int)$4 - 1;
	yart->yartTri(id1, id2, id3);
	// DEBUG("PARSE TRI %d %d %d\n", id1, id2, id3);
}

| SPHERE NUMBER NUMBER NUMBER NUMBER {
	vec3 center($2, $3, $4);
	yart->yartSphere(center, $5);
	DEBUG("PARSE SPHERE c %.2f %.2f %.2f | r %.2f\n",
		center[0], center[1], center[2], $5);
}

| INCLUDE STR {
	include_push($2);
}

| REFINEMESH {
    DEBUG("PARSE REFINE MESH\n");
    yart->yartRefineMesh();
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
    DEBUG("PARSE PUSHTRANSFORM\n");
	yart->yartPushTransform();
}

| POPTRANSFORM {
	yart->yartPopTransform();
}

| MATERIAL STR param_list {
    yart->yartMaterial(string($2), $3);
    delete $3;
}

| BUILDOCTREE NUMBER {
	yart->yartBuildOCTree((int)$2);
};


%%

void yyerror(const std::string &s) {
    std::cerr << s << std::endl;
    system("pause");
    exit(-1);
}