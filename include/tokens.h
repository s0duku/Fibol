/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_INCLUDE_TOKENS_H_INCLUDED
# define YY_YY_INCLUDE_TOKENS_H_INCLUDED
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
    TOKENS_EOF = 0,
    TOKENS_ID = 258,
    TOKENS_INT = 259,
    TOKENS_REAL = 260,
    TOKENS_IF = 261,
    TOKENS_ELSE = 262,
    TOKENS_RETURN = 263,
    TOKENS_PLUS = 264,
    TOKENS_STAR = 265,
    TOKENS_MINUS = 266,
    TOKENS_DIV = 267,
    TOKENS_LBRACKET = 268,
    TOKENS_RBRACKET = 269,
    TOKENS_ASSIGN = 270,
    TOKENS_SEMICOLON = 271,
    TOKENS_LBRACE = 272,
    TOKENS_RBRACE = 273,
    TOKENS_EQUAL = 274,
    TOKENS_NEQUAL = 275,
    TOKENS_NOT = 276,
    TOKENS_GREATER = 277,
    TOKENS_LESSER = 278,
    TOKENS_AND = 279,
    TOKENS_OR = 280,
    TOKENS_DOT = 281,
    TOKENS_COMMA = 282,
    TOKENS_FUNCTION = 283,
    TOKENS_STRING = 284,
    TOKENS_ERROR = 285
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 51 "parser/parser.y"

  BaseAST * ast;
  int op;
  std::string* id_sym;

#line 95 "include/tokens.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDE_TOKENS_H_INCLUDED  */
