/**
 * @file parser.c
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * @brief An LL(1) Parser implementation
 */

#include "scanner.h"
#include "log_util.h"
#include "file_util.h"

int run_parser()
{
    struct token curToken;

    print_line_to_listings();

    curToken = read_token();

    while( curToken.id != scaneofToken.id)
    {
        log_info("Token: %s", curToken.name);
        write_to_file(outputFilePtr, FMT_TOKEN_LINE, curToken.id, curToken.name, "TOKEN");
        curToken = read_token();
    }
        
    write_to_file(outputFilePtr, FMT_TOKEN_LINE, curToken.id, curToken.name, "TOKEN");

    return 1;
}

int parse_systemGoal()
{
    parse_program();
    
    // Scan EOF
}

int parse_program()
{
    // Begin

    parse_statementList();

    // End
}

int parse_statementList()
{
    parse_statement();

    // (optional) parse_statementList();
}

int parse_statement()
{
    // Case 1:
    // ID
    // :=
    parse_expression();

    // Case 2:
    // READ
    // (
    parse_idList();
    // )
    // ;

    // Case 3:
    // WRITE
    // (
    parse_exprList();
    // )
    // ;

    // Case 4:
    // IF
    // (
    parse_condition();
    // )
    // THEN
    parse_statementList();
    parse_ifTail();
}

int parse_ifTail()
{
    //Case 1: 
    //ELSE 
    parse_statementList(); 
    //ENDIF
    
    //Case 2:
    //ENDIF

}

int parse_idList()
{
    //ID 

    //optional:
    parse_idList();
}

int parse_exprList()
{
    parse_expression();

    //optional:
    parse_exprList();

}

int parse_expression()
{
    parse_term();

    //optional:
    parse_addOp();
    parse_term();
}

int parse_term()
{
    parse_factor();

    // (optional)   parse_multOP();
    //              parse_factor();
}

int parse_factor()
{
    //Case 1:
    //(
    parse_expression();
    //)

    //Case 2:
    parse_factor();

    //Case 3:
    //ID

    //Case 4:
    //INTLITERAL
}

int parse_addOp()
{
    // Case 1:
    // +

    // Case 2:
    // -
}

int parse_multOp()
{
    // Case 1:
    // *

    // Case 2:
    // /
}

int parse_condition()
{
    parse_addition();

    //optional:
    parse_relOp();
    parse_addition();
}

int parse_addition()
{
    parse_multiplication();

    // (optional)   parse_addOp();
    //              parse_multiplication();
}

int parse_multiplication()
{
    parse_unary();

    // (optional)   parse_multOp();
    //              parse_unary();
}

int parse_unary()
{
    //Case 1:
    //! 
    parse_unary();
    
    //Case 2:
    //-
    parse_unary();
}

int parse_lPrimary()
{
    // Case 1:
    // INTLITERAL

    // Case 2:
    // ID

    // Case 3:
    // (
    parse_condition();
    // )
    
    // Case 4:
    // FALSEOP

    // Case 5:
    // TRUEOP

    // Case 6:
    // NULLOP
}

int parse_relOp()
{
    //Case 1:
    //<

    //Case 2:
    //<=

    //Case 3
    //>
    
    //Case 4:
    //>=

    //Case 5:
    //=

    //Case 6:
    //<>
}

// EOF