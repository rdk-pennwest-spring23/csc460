/**
 * @file generator.h
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
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
#include <stdarg.h>

int init_generator()
{
    numSymbols = 0;
    tempNum = 0;
}

struct op_rec create_op_rec(char op)
{
    struct op_rec rec;
    rec.operation[0] = op;
    rec.operation[1] = '\0';
    return rec;
}

struct expr_rec create_expr_rec(char* expr, EXPR_TYPE type)
{
    struct expr_rec rec;
    strcpy(rec.e, expr);
    rec.type = type;
    return rec;
}


/* ---------- Action symbol functions ---------- */


/*
#start initialization of intermediate C code file and other initializations
write a descriptive heading to the listing and output files.
initialize symbol table
initialize temp counter
initialize line counter
*/
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


/**
 * @brief Assign generates the code for the assignment statement. It will
 * receive two expression records, one for the left hand side of the statement
 * and one for the right hand side of the statement. It will call the generate
 * method with the information from the two expression records, and insert the
 * "=" to generate a correct C assignment statement.
 */
void assign(struct expr_rec target, struct expr_rec source)
{
    log_debug("Generating assignment statement for %s = %s.", target.e, source.e);
    generate("%s = %s;", target.e, source.e);
}


/**
 * @brief #read _id generates the code for the read statement
 It will receive an expression record and generate a scan statement
i.e. scanf("%d", &x);
 * 
 */
 void read_id(struct expr_rec rec)
 {
    log_debug("Generating read statement.");
    generate("scanf(\"%%s\", %s);", rec.e);
 }


 /**
 * @brief #write_expr generates the code for the write statement
It will receive an expression record and generate a print statement
i.e. printf("%d\n",×);
 * 
 */
void write_expr(struct expr_rec rec)
{
    log_debug("Generating write expression.");
    generate("printf(\"%%s\", %s);", rec.e);
}


/**
 * @brief •#gen_infix generates the code for the infix semantic record
It will accept two expression records one for the left hand side and one for the right hand side and an operation record
It creates third expression record and sets it's kind to TEMPEXPR
It sets it's string to a new TempID by using gettemp
It generates C code with generate for the assignment of the left expression operation right expression to the Temp
i.e. Temp5 = X + 7;
It then returns the Temp expression record
This process will always have pairs of parameters condensing down to one new temporary which build a more complex expression
 * 
 * @param left 
 * @param op 
 * @param right 
 * @return struct expr_rec 
 */
struct expr_rec gen_infix(char* left, char* op, char* right)
{
    log_debug("Generating infix expression.");
    char temp[SYMBOL_SIZE];
    gettemp(temp);
    generate("%s = %s %s %s;", temp, left, op, right);
    return create_expr_rec(temp, IDExpression);
}


/**
 * @brief  #process _literal generates the code for the literal semantic record
It creates an expression record and sets it's kind to LITERALEXPR
It sets it's string to the contents of the token buffer which is the number
We can keep the number as a string since it will eventually be outputted via a C printf.
It then returns the literal expression record
 * 
 * @param num The integer interal.
 * @return struct expr_rec The expression record referencing 
 * the integer literal.
 */
struct expr_rec process_literal(char* num)
{
    log_debug("Processing literal expression.");
    return create_expr_rec(num, LiteralExpression);
}


/**
 * @brief #process op generates the code for the operator semantic record
It creates an operation record
It sets it's string to the type of operation specified, some translation may be needed for =, <>, true, and false so we need to decode the token instead of using the token buffer
It then returns the operation record
 * 
 */
struct op_rec process_op(char* op)
{
    log_debug("Processing operation expression.");
    struct op_rec o;
    strcpy(o.operation, op);
    return o;
}


/**
 * @brief  #process_id generates the code for the ID semantic record
It creates an expression record and sets it's kind to IDEXPR
It sets it's string to the contents of the token buffer which is the ID
It then returns the ID expression record
 * 
 */
void process_id(char* symbol)
{
    log_debug("Processing id expression.");
    check_id(symbol);
}


/*
#finish completes the intermediate C code file and other finalizations
write a descriptive closing to the listing and output files
concatenates the two parts of the final C code together
*/
void generate_finish()
{
    log_debug("Generating end of output.");
    write_to_file(tempFilePtr, "return 0;\n}");
}


/* --------- Additional Functions ---------- */


int lookup(char* symbol)
{
    int exists = 0;
    
    int i;
    for (i = 0; i < numSymbols && !exists; i++)
    {
        if (strcmp(symbolTable[i], symbol) == 0)
        {
            exists = 1;
        }
    }

    return exists;
}

void enter(char* symbol)
{
    log_debug("Entering new symbol %s into the symbol table at slot %d.", symbol, numSymbols);
    strcpy(symbolTable[numSymbols], symbol);
    numSymbols++;

    write_to_file(outputFilePtr, FMT_VAR_DECL, symbol);
}

void check_id(char* symbol)
{
    int exists = lookup(symbol);
    if (!exists)
        enter(symbol);
    
}

void generate(const char* code, ...)
{
    va_list args;
    va_start( args, code);
    vfprintf(tempFilePtr, code, args);
    fprintf(tempFilePtr, "\n");
    va_end(args);
}

void gettemp(char* tempVar)
{
    log_debug("Getting new Temp variable.");
    memset(tempVar, '\0', SYMBOL_SIZE);

    sprintf(tempVar, "Temp%d", tempNum);
    tempNum++;

    enter(tempVar);
}