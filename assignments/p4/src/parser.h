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
#include "generator.h"

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
int parse_expression(struct expr_rec * exprRec);
int parse_term(struct expr_rec * exprRec);
int parse_factor(struct expr_rec * exprRec);
int parse_addOp(struct op_rec * opRec);
int parse_multOp(struct op_rec * opRec);
int parse_condition(struct expr_rec * exprRec);
int parse_addition(struct expr_rec * exprRec);
int parse_multiplication(struct expr_rec * exprRec);
int parse_unary(struct expr_rec * exprRec);
int parse_lPrimary(struct expr_rec * exprRec);
int parse_relOp(struct op_rec * opRec);
struct expr_rec parse_ident();

#endif