/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_YACC_SQL_HPP_INCLUDED
#define YY_YY_YACC_SQL_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
#define YYTOKENTYPE
enum yytokentype
{
  YYEMPTY      = -2,
  YYEOF        = 0,   /* "end of file"  */
  YYerror      = 256, /* error  */
  YYUNDEF      = 257, /* "invalid token"  */
  SEMICOLON    = 258, /* SEMICOLON  */
  AS           = 259, /* AS  */
  BY           = 260, /* BY  */
  CREATE       = 261, /* CREATE  */
  DROP         = 262, /* DROP  */
  GROUP        = 263, /* GROUP  */
  TABLE        = 264, /* TABLE  */
  TABLES       = 265, /* TABLES  */
  INDEX        = 266, /* INDEX  */
  CALC         = 267, /* CALC  */
  SELECT       = 268, /* SELECT  */
  DESC         = 269, /* DESC  */
  SHOW         = 270, /* SHOW  */
  SYNC         = 271, /* SYNC  */
  INSERT       = 272, /* INSERT  */
  DELETE       = 273, /* DELETE  */
  UPDATE       = 274, /* UPDATE  */
  LBRACE       = 275, /* LBRACE  */
  RBRACE       = 276, /* RBRACE  */
  COMMA        = 277, /* COMMA  */
  TRX_BEGIN    = 278, /* TRX_BEGIN  */
  TRX_COMMIT   = 279, /* TRX_COMMIT  */
  TRX_ROLLBACK = 280, /* TRX_ROLLBACK  */
  INT_T        = 281, /* INT_T  */
  STRING_T     = 282, /* STRING_T  */
  FLOAT_T      = 283, /* FLOAT_T  */
  DATE_T       = 284, /* DATE_T  */
  NOT          = 285, /* NOT  */
  NULL_T       = 286, /* NULL_T  */
  NULLABLE     = 287, /* NULLABLE  */
  HELP         = 288, /* HELP  */
  EXIT         = 289, /* EXIT  */
  DOT          = 290, /* DOT  */
  INTO         = 291, /* INTO  */
  VALUES       = 292, /* VALUES  */
  FROM         = 293, /* FROM  */
  WHERE        = 294, /* WHERE  */
  AND          = 295, /* AND  */
  SET          = 296, /* SET  */
  ON           = 297, /* ON  */
  LOAD         = 298, /* LOAD  */
  DATA         = 299, /* DATA  */
  INFILE       = 300, /* INFILE  */
  EXPLAIN      = 301, /* EXPLAIN  */
  STORAGE      = 302, /* STORAGE  */
  FORMAT       = 303, /* FORMAT  */
  INNER        = 304, /* INNER  */
  JOIN         = 305, /* JOIN  */
  EQ           = 306, /* EQ  */
  LT           = 307, /* LT  */
  GT           = 308, /* GT  */
  LE           = 309, /* LE  */
  GE           = 310, /* GE  */
  NE           = 311, /* NE  */
  LIKE         = 312, /* LIKE  */
  IS           = 313, /* IS  */
  NUMBER       = 314, /* NUMBER  */
  FLOAT        = 315, /* FLOAT  */
  ID           = 316, /* ID  */
  SSS          = 317, /* SSS  */
  UMINUS       = 318  /* UMINUS  */
};
typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 125 "yacc_sql.y"

  ParsedSqlNode                            *sql_node;
  ConditionSqlNode                         *condition;
  Value                                    *value;
  enum CompOp                               comp;
  RelAttrSqlNode                           *rel_attr;
  std::vector<AttrInfoSqlNode>             *attr_infos;
  AttrInfoSqlNode                          *attr_info;
  Expression                               *expression;
  std::vector<std::unique_ptr<Expression>> *expression_list;
  std::vector<Value>                       *value_list;
  std::vector<std::vector<Value>>          *values_list;
  std::vector<ConditionSqlNode>            *condition_list;
  std::vector<RelAttrSqlNode>              *rel_attr_list;
  std::vector<RelationNode>                *relation_list;
  SetClauseSqlNode                         *set_clause;
  std::vector<SetClauseSqlNode>            *set_clauses;
  JoinSqlNode                              *join_clause;
  std::vector<JoinSqlNode>                 *join_clauses;
  char                                     *string;
  int                                       number;
  float                                     floats;
  bool                                      nullable_info;

#line 152 "yacc_sql.hpp"
};
typedef union YYSTYPE YYSTYPE;
#define YYSTYPE_IS_TRIVIAL 1
#define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if !defined YYLTYPE && !defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
#define YYLTYPE_IS_DECLARED 1
#define YYLTYPE_IS_TRIVIAL 1
#endif

int yyparse(const char *sql_string, ParsedSqlResult *sql_result, void *scanner);

#endif /* !YY_YY_YACC_SQL_HPP_INCLUDED  */
