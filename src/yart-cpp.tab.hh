/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_YART_CPP_TAB_HH_INCLUDED
# define YY_YY_YART_CPP_TAB_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUMBER = 258,
    STR = 259,
    SIZE = 260,
    INTEGRATOR = 261,
    PIXELSAMPLER = 262,
    OUTPUT = 263,
    INCLUDE = 264,
    CAMERA = 265,
    MAXVERTS = 266,
    MAXVERTNORMS = 267,
    VERTEX = 268,
    TRI = 269,
    SPHERE = 270,
    TRANSLATE = 271,
    SCALE = 272,
    ROTATE = 273,
    PUSHTRANSFORM = 274,
    POPTRANSFORM = 275,
    DIRECTIONAL = 276,
    POINT = 277,
    ATTENUATION = 278,
    DIFFUSE = 279,
    AMBIENT = 280,
    SPECULAR = 281,
    EMISSION = 282,
    SHININESS = 283,
    BUILDOCTREE = 284
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 21 "yart-cpp.yy" /* yacc.c:1909  */

    char string[1024];
    float num;

#line 89 "yart-cpp.tab.hh" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_YART_CPP_TAB_HH_INCLUDED  */
