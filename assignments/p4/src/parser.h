/**
 * @file parser.h
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * @brief An LL(1) Parser implementation
 */

#ifndef PARSER_H
#define PARSER_H

#define FMT_PARSE_ERROR "Parse Error. Expected Token: %-20s Actual Token: %s"
#define FMT_PARSE_ERROR_TOTAL "Total Syntax Errors: %d"
#define FMT_MATCH_TOKEN "ExpectedToken: %-20s Actual: %s"

#include "scanner.h"

int parseErrors;
char stmtBuffer[1000];

int run_parser();

int match(struct token t1, struct token t2);

int parse_systemGoal();
int parse_program();
int parse_statementList();
int parse_statement();
int parse_ifTail();
int parse_idList();
int parse_exprList();
int parse_expression();
int parse_term();
int parse_factor();
int parse_addOp();
int parse_multOp();
int parse_condition();
int parse_addition();
int parse_multiplication();
int parse_unary();
int parse_lPrimary();
int parse_relOp();
int parse_ident();

#endif