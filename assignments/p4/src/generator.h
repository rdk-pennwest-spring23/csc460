/**
 * @file generator.h
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * @brief A Generator for the Compiler
 * 
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#define FMT_GEN_TIME "/*\nCurrent Date and Time: \n%s*/"

typedef struct symbol_table {
    char symbol[100];
    SymbolTable next;    
} SymbolTable;

SymbolTable symbolTable;

typedef struct operation_record {
    char operation[2];
} OpRec;

enum expr_type {IDExpression, LiteralExpression, TempExpression};

typedef struct expression_record
{
    char e[1000];
    enum expr_type type;
} ExprRec;

OpRec create_op_rec(char op);
ExprRec create_expr_rec(char expr[1000], enum expr_type type);

void generate_start();
void generate_finish();

int lookup(char* symbol);
void enter(char* symbol);
void check_id(char* symbol);
void generate();
void gettemp();

void assign();
void read_id();
void write_expr();
void gen_infix();
void process_literal();
void process_op();
void process_id();


#endif