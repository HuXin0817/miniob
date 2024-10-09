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
  ASC          = 260, /* ASC  */
  BY           = 261, /* BY  */
  CREATE       = 262, /* CREATE  */
  DROP         = 263, /* DROP  */
  EXISTS       = 264, /* EXISTS  */
  GROUP        = 265, /* GROUP  */
  ORDER        = 266, /* ORDER  */
  TABLE        = 267, /* TABLE  */
  TABLES       = 268, /* TABLES  */
  INDEX        = 269, /* INDEX  */
  CALC         = 270, /* CALC  */
  SELECT       = 271, /* SELECT  */
  DESC         = 272, /* DESC  */
  SHOW         = 273, /* SHOW  */
  SYNC         = 274, /* SYNC  */
  INSERT       = 275, /* INSERT  */
  DELETE       = 276, /* DELETE  */
  UPDATE       = 277, /* UPDATE  */
  LBRACE       = 278, /* LBRACE  */
  RBRACE       = 279, /* RBRACE  */
  COMMA        = 280, /* COMMA  */
  TRX_BEGIN    = 281, /* TRX_BEGIN  */
  TRX_COMMIT   = 282, /* TRX_COMMIT  */
  TRX_ROLLBACK = 283, /* TRX_ROLLBACK  */
  INT_T        = 284, /* INT_T  */
  IN           = 285, /* IN  */
  STRING_T     = 286, /* STRING_T  */
  FLOAT_T      = 287, /* FLOAT_T  */
  DATE_T       = 288, /* DATE_T  */
  TEXT_T       = 289, /* TEXT_T  */
  NOT          = 290, /* NOT  */
  UNIQUE       = 291, /* UNIQUE  */
  NULL_T       = 292, /* NULL_T  */
  NULLABLE     = 293, /* NULLABLE  */
  HELP         = 294, /* HELP  */
  EXIT         = 295, /* EXIT  */
  DOT          = 296, /* DOT  */
  INTO         = 297, /* INTO  */
  VALUES       = 298, /* VALUES  */
  FROM         = 299, /* FROM  */
  WHERE        = 300, /* WHERE  */
  AND          = 301, /* AND  */
  OR           = 302, /* OR  */
  SET          = 303, /* SET  */
  ON           = 304, /* ON  */
  LOAD         = 305, /* LOAD  */
  DATA         = 306, /* DATA  */
  INFILE       = 307, /* INFILE  */
  EXPLAIN      = 308, /* EXPLAIN  */
  STORAGE      = 309, /* STORAGE  */
  FORMAT       = 310, /* FORMAT  */
  INNER        = 311, /* INNER  */
  JOIN         = 312, /* JOIN  */
  EQ           = 313, /* EQ  */
  LT           = 314, /* LT  */
  GT           = 315, /* GT  */
  LE           = 316, /* LE  */
  GE           = 317, /* GE  */
  NE           = 318, /* NE  */
  LIKE         = 319, /* LIKE  */
  IS           = 320, /* IS  */
  NUMBER       = 321, /* NUMBER  */
  FLOAT        = 322, /* FLOAT  */
  ID           = 323, /* ID  */
  SSS          = 324, /* SSS  */
  UMINUS       = 325  /* UMINUS  */
};
typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 132 "yacc_sql.y"

  ParsedSqlNode                            *sql_node;
  Value                                    *value;
  enum CompOp                               comp;
  RelAttrSqlNode                           *rel_attr;
  std::vector<AttrInfoSqlNode>             *attr_infos;
  AttrInfoSqlNode                          *attr_info;
  Expression                               *expression;
  std::vector<std::unique_ptr<Expression>> *expression_list;
  std::vector<Value>                       *value_list;
  std::vector<std::vector<Value>>          *values_list;
  SetClauseSqlNode                         *set_clause;
  std::vector<SetClauseSqlNode>            *set_clauses;
  JoinSqlNode                              *join_clauses;
  std::vector<RelAttrSqlNode>              *rel_attr_list;
  std::vector<RelationNode>                *relation_list;
  OrderBySqlNode                           *orderby_unit;
  std::vector<OrderBySqlNode>              *orderby_list;
  char                                     *string;
  int                                       number;
  float                                     floats;
  bool                                      nullable_info;
  std::vector<std::string>                 *index_attr_list;
  bool                                      unique;

#line 160 "yacc_sql.hpp"
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
