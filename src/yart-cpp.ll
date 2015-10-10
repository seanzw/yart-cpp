
%{

#include <string>
#include <cstdlib>
#include "yart-cpp.tab.hh"

void yyerror(const std::string &s);

int str_pos;

void add_string_char(char c) {
    yylval.string[str_pos++] = c;
    yylval.string[str_pos] = '\0';
}

%}

/* eliminate isatty */
%option never-interactive

/* no unistd.h */
%option nounistd

WHITESPACE  [ \t\r]+
Number      [-+]?([0-9]+|(([0-9]+\.[0-9]*)|(\.[0-9]+)))([eE][-+]?[0-9]+)?

/* STRING, COMMAND */
%x STRING COMMENT

%%

"#" { BEGIN COMMENT; }
<COMMENT>.
<COMMENT>\n { BEGIN INITIAL; }

size			{ return SIZE;			}
integrator		{ return INTEGRATOR;	}
output			{ return OUTPUT;		}
camera			{ return CAMERA;		}
maxverts		{ return MAXVERTS;		}
maxvertnorms	{ return MAXVERTNORMS;	}
vertex			{ return VERTEX;		}
tri				{ return TRI;			}
sphere			{ return SPHERE;		}
translate		{ return TRANSLATE;		}
scale			{ return SCALE;			}
rotate			{ return ROTATE;		}
pushTransform	{ return PUSHTRANSFORM; }
popTransform	{ return POPTRANSFORM;	}
directional		{ return DIRECTIONAL;	}
point			{ return POINT;			}
attenuation		{ return ATTENUATION;	}
diffuse			{ return DIFFUSE;		}
ambient			{ return AMBIENT;		}
specular		{ return SPECULAR;		}
emission		{ return EMISSION;		}
shininess		{ return SHININESS;		}
buildOCTree		{ return BUILDOCTREE;	}


{WHITESPACE}    /* do nothing */

{Number} {
    yylval.num = atof(yytext);
    return NUMBER;
}

\" {
	BEGIN STRING;
	str_pos = 0;
	yylval.string[0] = '\0';
}

<STRING>\\n {
	add_string_char('\n');
}
<STRING>\\t {
	add_string_char('\t');
}
<STRING>\\r {
	add_string_char('\r');
}
<STRING>\\b {
	add_string_char('\b');
}
<STRING>\\f {
	add_string_char('\f');
}
<STRING>\\\" {
	add_string_char('\"');
}
<STRING>\\\\ {
	add_string_char('\\');
}
<STRING>\\[0-9]{3} {
  int val = atoi(yytext+1);
  while (val > 256)
    val -= 256;
  add_string_char(val);
}
<STRING>\" {
	BEGIN INITIAL;
	return STR;
}
<STRING>. {
	add_string_char(yytext[0]);
}

\n {
	
}

. {
    std::string msg("ILLEGAL CHARACTER: ");
    msg.append(yytext);
    yyerror(msg);
}

%%

int yywrap(void) {
    return 1;
}
