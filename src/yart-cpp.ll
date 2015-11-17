
%top{

// Avoid stdint.h redefinition warning.
#include <cstdint>

}


%{

#include <string>
#include <cstdlib>
#include <vector>
#include "Const.h"
#include "yart-cpp.tab.hh"

using namespace std;

void yyerror(const std::string &s);

extern int line_num;
int str_pos;

void add_string_char(char c) {
    yylval.string[str_pos++] = c;
    yylval.string[str_pos] = '\0';
}

struct IncludeInfo {
    std::string filename;
    YY_BUFFER_STATE bufState;
    int lineNum;
};

vector<IncludeInfo> includeStack;

void include_push(char *filename) {

    extern std::string current_file;

    if (includeStack.size() > 32) {
        yyerror("Only 32 levels of nested Include allowed in scene files.");
        exit(1);
    }

    std::string new_file(filename);
    size_t pos = current_file.find_last_of('/');
    
    if (pos != string::npos) {
        new_file = current_file.substr(0, pos + 1);
        new_file.append(filename);
    }

    DEBUG("INCLUDE %s\n", new_file.c_str());

    FILE *f = fopen(new_file.c_str(), "r");
    if (!f) {
        string msg = "Unable to open included scene file: ";
        msg.append(new_file);
        yyerror(msg);
    }
    else {

        
        IncludeInfo ii;
        ii.filename = current_file;
        ii.bufState = YY_CURRENT_BUFFER;
        ii.lineNum = line_num;
        includeStack.push_back(ii);

        pos = new_file.find_last_of('.');
        
        yyin = f;
        current_file = new_file;
        line_num = 1;

        yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
    }
}

void include_pop() {
    extern int line_num;
    extern std::string current_file;
    fclose(yyin);
    yy_delete_buffer(YY_CURRENT_BUFFER);
    yy_switch_to_buffer(includeStack.back().bufState);
    current_file = includeStack.back().filename;
    line_num = includeStack.back().lineNum;
    includeStack.pop_back();
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
<COMMENT>\n {
    line_num++;
    BEGIN INITIAL;
}

size                { return SIZE;          }
integrator          { return INTEGRATOR;    }
pixelSampler        { return PIXELSAMPLER;  }
output              { return OUTPUT;        }
worldBegin          { return WORLDBEGIN;    }
worldEnd            { return WORLDEND;      }
objBegin            { return OBJBEGIN;      }
objEnd              { return OBJEND;        }
include             { return INCLUDE;       }    
camera              { return CAMERA;        }
maxverts            { return MAXVERTS;      }
maxvertnorms        { return MAXVERTNORMS;  }
v                   { return VERTEX;        }
f                   { return TRI;           }
sphere              { return SPHERE;        }
refineMesh          { return REFINEMESH;    }
translate           { return TRANSLATE;     }
scale               { return SCALE;         }
rotate              { return ROTATE;        }
pushTransform       { return PUSHTRANSFORM; }
popTransform        { return POPTRANSFORM;  }
directional         { return DIRECTIONAL;   }
point               { return POINT;         }
areaLight           { return AREALIGHT;     }
attenuation         { return ATTENUATION;   }
material            { return MATERIAL;      }
buildOCTree         { return BUILDOCTREE;   }

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
    line_num++;
}

. {
    std::string msg("ILLEGAL CHARACTER: ");
    msg.append(yytext);
    yyerror(msg);
}

%%

int yywrap(void) {
    if (includeStack.size() == 0)
        return 1;
    include_pop();
    return 0;
}
