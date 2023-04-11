/**
 * @file generator.h
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * @brief A Generator for the Compiler
 * 
 */

#include "log_util.h"
#include "generator.h"
#include "parser.h"
#include "scanner.h"
#include "file_util.h"

#include <time.h>

OpRec create_op_rec(char op)
{
    OpRec rec;
    rec.operation[0] = op;
    rec.operation[1] = '\0';
    return rec;
}

ExprRec create_expr_rec(char expr[1000], enum expr_type type)
{
    ExprRec rec;
    strcpy(rec.e, expr);
    rec.type = type;
    return rec;
}

void generate_start()
{
    /*  Get the current datetime */
    time_t now;
    time(&now);

    write_to_file(outputFilePtr, FMT_GEN_TIME, ctime(&now));
    write_to_file(outputFilePtr, "");

    /*  Write out the includes */
    write_to_file(outputFilePtr, "#include <stdio.h>");
    write_to_file(outputFilePtr, "");

    /*  Write out the main function tag */
    write_to_file(outputFilePtr, "main()\n{");
}

void generate_finish()
{
    write_to_file(tempFilePtr, "return 0;\n}");
}

int lookup(char* symbol)
{
    int exists = 0;
    SymbolTable table = symbolTable;

    while (table.symbol != NULL && exists == 0)
    {
        if (strcmp(symbol, table.symbol) == 0)
            exists = 1;
        else
            table = table.next;
    }

    return exists;
}

void enter(char* symbol)
{
    SymbolTable table = symbolTable;

    strcpy(table.symbol, symbol);
    table = table.next;
}

