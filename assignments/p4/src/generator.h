/**
 * @file generator.h
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * @brief A Generator for the Compiler
 * 
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#define FMT_GEN_TIME "/*\nCurrent Date and Time: \n%s*/"
#define FMT_VAR_DECL "int %s;"

#define SYMBOL_TABLE_SIZE 100
#define SYMBOL_SIZE 50

struct op_rec 
{
    char operation[3];
};

typedef enum {IDExpression, LiteralExpression, TempExpression, NullExpression} EXPR_TYPE;

struct expr_rec
{
    char e[1000];
    EXPR_TYPE type;
};

int tempNum;

int numSymbols;
char symbolTable[SYMBOL_TABLE_SIZE][SYMBOL_SIZE];

struct op_rec create_op_rec(char op);
struct expr_rec create_expr_rec(char* expr, EXPR_TYPE type);

int init_generator();


/* ---------- Action symbol functions ---------- */


/*
#start initialization of intermediate C code file and other initializations
write a descriptive heading to the listing and output files.
initialize symbol table
initialize temp counter
initialize line counter
*/
void generate_start();


/**
 * @brief Assign generates the code for the assignment statement. It will
 * receive two expression records, one for the left hand side of the statement
 * and one for the right hand side of the statement. It will call the generate
 * method with the information from the two expression records, and insert the
 * "=" to generate a correct C assignment statement.
 * 
 */
void assign(struct expr_rec leftRec, struct expr_rec rightRec);


/**
 * @brief #read _id generates the code for the read statement
 It will receive an expression record and generate a scan statement
i.e. scanf("%d", &x);
 * 
 */
void read_id(struct expr_rec rec);


/**
 * @brief #write_expr generates the code for the write statement
It will receive an expression record and generate a print statement
i.e. printf("%d\n",×);
 * 
 */
void write_expr();


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
struct expr_rec gen_infix(char* left, char* op, char* right);


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
struct expr_rec process_literal(char* num);


/**
 * @brief #process op generates the code for the operator semantic record
It creates an operation record
It sets it's string to the type of operation specified, some translation may be needed for =, <>, true, and false so we need to decode the token instead of using the token buffer
It then returns the operation record
 * 
 */
struct op_rec process_op(char* op);


/**
 * @brief  #process_id generates the code for the ID semantic record
It creates an expression record and sets it's kind to IDEXPR
It sets it's string to the contents of the token buffer which is the ID
It then returns the ID expression record
 * 
 */
void process_id();


/*
#finish completes the intermediate C code file and other finalizations
write a descriptive closing to the listing and output files
concatenates the two parts of the final C code together
*/
void generate_finish();


/* ---------- Additional functions ---------- */


int lookup(char* symbol);
void enter(char* symbol);
void check_id(char* symbol);
void generate(const char* code, ...);
void gettemp(char* tempVar);

#endif