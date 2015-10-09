
%{

#include <string>
void yyerror(const std::string &s);
#include <cstdlib>
#include "yart-cpp.tab.hh"

%}

/* eliminate isatty */
%option never-interactive

/* no unistd.h */
%option nounistd

WHITESPACE  [ \t\r]+
Number      [-+]?([0-9]+|(([0-9]+\.[0-9]*)|(\.[0-9]+)))([eE][-+]?[0-9]+)?

%%


size {
    return SIZE;
}

{WHITESPACE}    /* do nothing */

{Number} {
    yylval.num = atof(yytext);
    return NUMBER;
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
