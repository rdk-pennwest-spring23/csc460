/**
 * @file parser.c
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * @brief An LL(1) Parser implementation
 */

#include "scanner.h"
#include "parser.h"
#include "log_util.h"
#include "file_util.h"

int run_parser()
{
    struct token curToken;

    print_line_to_listings();

    parse_systemGoal();

    return 1;
}

int match(struct token t1, struct token t2)
{
    return t1.id == t2.id;
}

int parse_systemGoal()
{
    log_debug("Parsing system goal.");
    parse_program();
    
    // Check for EOF
    if (!match( peek_next_token(), scaneofToken ))
    {
        write_to_file(listingFilePtr, FMT_PARSE_ERROR, scaneofToken.name, read_token().name);
    }
    else
        read_token();

    return 1;
    
}

int parse_program()
{
    log_debug("Parsing program.");
    struct token beginToken = tokenList[TOKEN_ID_BEGIN];
    struct token endToken = tokenList[TOKEN_ID_END];

    // Begin
    if ( !match(peek_next_token(), beginToken ))
    {
        write_to_file(listingFilePtr, FMT_PARSE_ERROR, beginToken.name, read_token().name);
    }
    else
        read_token();

    // Statement List
    parse_statementList();

    // End
    if (!match(peek_next_token(), endToken))
    {
        write_to_file(listingFilePtr, FMT_PARSE_ERROR, endToken.name, read_token().name);
    }
    else
        read_token();

    return 1;

}

int parse_statementList()
{
    log_debug("Parsing statement list.");
    if (parse_statement())
        parse_statementList();

    return 1;
}

/**
 * STMT -> <ID> := <expression>;
 *      -> READ ( <id list> );
 *      -> WRITE ( <expr list> );
 */
int parse_statement()
{
    log_debug("Parsing statement.");
    int status = 1;
    struct token inToken;
    struct token idToken = tokenList[TOKEN_ID_ID];
    struct token assignopToken = tokenList[TOKEN_ID_ASSIGNOP];
    struct token readToken = tokenList[TOKEN_ID_READ];
    struct token writeToken = tokenList[TOKEN_ID_WRITE];
    struct token ifToken = tokenList[TOKEN_ID_IF];
    struct token thenToken = tokenList[TOKEN_ID_THEN];
    struct token leftparenToken = tokenList[TOKEN_ID_LPAREN];
    struct token rightparenToken = tokenList[TOKEN_ID_RPAREN];
    struct token semiToken = tokenList[TOKEN_ID_SEMICOLON];

    // Case 1: <ID> := <expression>
    if ( match(peek_next_token(), idToken) )
    {
        // ID
        read_token();

        // Assign Op
        if (!match(peek_next_token(), assignopToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, assignopToken.name, read_token().name);
        }
        else 
            read_token();

        // Expression
        parse_expression();

        // Semicolon
        if (!match(peek_next_token(), semiToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, semiToken.name, read_token().name);
        }
        else
            read_token();
    }

    // READ ( <id list> );
    else if (match(peek_next_token(), readToken))
    {
        // READ
        read_token();

        // (
        inToken = read_token();
        if (!match(leftparenToken, inToken))
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, leftparenToken.name, inToken.name);

        // <id list>
        parse_idList();

        // )
        inToken = read_token();
        if (!match(rightparenToken, inToken))
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, rightparenToken.name, inToken.name);

        // ;
        inToken = read_token();
        if (!match(semiToken, inToken))
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, semiToken.name, inToken.name);
    }

    // WRITE ( <expr list> );
    else if (match(peek_next_token(), writeToken))
    {
        // WRITE
        read_token();

        // (
        inToken = read_token();
        if (!match(leftparenToken, inToken))
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, leftparenToken.name, inToken.name);

        // <expr list>
        parse_exprList();

        // )
        inToken = read_token();
        if (!match(rightparenToken, inToken))
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, rightparenToken.name, inToken.name);

        // ;
        inToken = read_token();
        if (!match(semiToken, inToken))
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, semiToken.name, inToken.name);
        
    }

    // IF ( <condition> ) THEN <stmt list> <if tail>
    else if (match(peek_next_token(), ifToken))
    {
        // IF
        read_token();

        // (
        inToken = read_token();
        if (!match(leftparenToken, inToken))
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, leftparenToken.name, inToken.name);

        // <condition>
        parse_condition();

        // )
        inToken = read_token();
        if (!match(rightparenToken, inToken))
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, rightparenToken.name, inToken.name);

        // THEN
        inToken = read_token();
        if (!match(thenToken, inToken))
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, thenToken.name, inToken.name);

        // <stmt list>
        parse_statementList();

        // <if tail>
        parse_ifTail();

    }

    else {
        status = 0;
    }

    return status;
}

/**
 * @brief Parses an if tail.
 * 
 * IFTAIL   -> ELSE <stmt list> ENDIF
 *          -> ENDIF
 * 
 * @return int 
 */
int parse_ifTail()
{
    log_debug("Parsing If Tail.");
    
    int status = 1;
    struct token inToken;
    struct token elseToken = tokenList[TOKEN_ID_ELSE];
    struct token endifToken = tokenList[TOKEN_ID_ENDIF];

    // ELSE <stmt list>
    if (match(peek_next_token(), elseToken))
    {
        // ELSE
        read_token();
        
        // <stmt list>
        parse_statementList();
    }

    inToken = read_token();
    if (!match(endifToken, inToken))
        write_to_file(listingFilePtr, FMT_PARSE_ERROR, endifToken.name, inToken.name);

}

int parse_idList()
{
    log_debug("Parsing ID List.");
    int status = 1;
    struct token inToken;
    struct token idToken = tokenList[TOKEN_ID_ID];
    struct token commaToken = tokenList[TOKEN_ID_COMMA];

    //ID
    inToken = read_token();
    if (!match(idToken, inToken))
    {
        write_to_file(listingFilePtr, FMT_PARSE_ERROR, idToken.name, inToken.name);
        status = 0;
    }

    // Optional: , <id list>
    if (match(peek_next_token(), commaToken))
    {
        // ,
        read_token();

        // <id list>
        parse_idList();
    }

    return status;
}

int parse_exprList()
{
    log_debug("Parsing expression list.");
    int status = 1;
    struct token inToken;
    struct token commaToken = tokenList[TOKEN_ID_COMMA];

    parse_expression();

    if (match(peek_next_token(), commaToken))
    {
        // ,
        read_token();

        // <expr list>
        parse_exprList();
    }

    
    return status;
}

/**
 * @brief Parses an expression rule.
 * 
 * EXPRESSION -> <term> {<add op> <term>}
 * 
 * @return int 
 */
int parse_expression()
{
    log_debug("Parsing expression.");
    int status = 1;
    struct token plusopToken = tokenList[TOKEN_ID_PLUSOP];

    if (status = parse_term())
    {
        if (match(peek_next_token(), plusopToken))
        {
            // +
            read_token();

            // <term>
            parse_term();
        }
    }

    return status;
}

/**
 * @brief Parses a term
 * 
 * TERM -> <factor> {<mult op> <factor>}
 * 
 * @return int 
 */
int parse_term()
{
    log_debug("Parsing term.");
    int status = 1;

    struct token multopToken = tokenList[TOKEN_ID_MULTOP];

    if (status = parse_factor())
    {
        if (match(peek_next_token(), multopToken))
        {
            // *
            read_token();

            // <term>
            parse_factor();
        }
    }

    return status;
}


/**
 * @brief Parses a factor
 * 
 * FACTOR   -> ( <expression> )
 *          -> - <factor>
 *          -> ID
 *          -> INTLITERAL
 * 
 * @return int 
 */
int parse_factor()
{
    log_debug("Parsing factor.");
    int status = 1;
    struct token inToken;
    struct token idToken = tokenList[TOKEN_ID_ID];
    struct token intlitToken = tokenList[TOKEN_ID_INTLITERAL];
    struct token minusopToken = tokenList[TOKEN_ID_MINUSOP];
    struct token leftparenToken = tokenList[TOKEN_ID_LPAREN];
    struct token rightparenToken = tokenList[TOKEN_ID_RPAREN];

    // ( <expression> )
    if (match(peek_next_token(), leftparenToken))
    {
        // (
        read_token();

        // <expression>
        parse_expression();

        // )
        inToken = read_token();
        if (!match(rightparenToken, inToken))
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, rightparenToken.name, inToken.name);
    }

    // - <factor>
    else if (match(peek_next_token(), minusopToken))
    {
        // -
        read_token();

        // <factor>
        parse_factor();
    }

    // ID
    else if (match(peek_next_token(), idToken))
    {
        read_token();
    }

    // INTLITERAL
    else if (match(peek_next_token(), intlitToken))
    {
        read_token();
    }

    else
        status = 0;
    
    return status;
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


/**
 * @brief Parses a condition.
 * 
 * CONDITION    -> <addition> {<rel op> <addition>}
 * 
 * @return int 
 */
int parse_condition()
{
    log_debug("Parsing condition.");
    int status = 1;

    // <addition>
    parse_addition();

    // <relop>
    if (status = parse_relOp())
    {
        // <addition>
        parse_addition();
    }

    return status;
}

/**
 * @brief Parses an addition clause.
 * 
 * ADDDITION -> <multiplication> {<add op> <mulitiplication>}
 * 
 * @return int 
 */
int parse_addition()
{
    int status = 1;
    // <multiplication>
    status = parse_multiplication();

    // <addOp>
    if (parse_addOp())
    {
        // <multiplication>
        parse_multiplication();
    }

    return 1;
}

/**
 * @brief Parses a multiplication clause
 * 
 * MULTIPLICATION -> <unary> {<multop> <unary>}
 * 
 * @return int 
 */
int parse_multiplication()
{
    int status = 1;

    // <unary>
    status = parse_unary();

    // <multOp>
    if (parse_multOp())
    {
        // <unary>
        parse_unary();
    }
}

int parse_unary()
{
    int status = 1;
    struct token inToken;
    struct token notToken = tokenList[TOKEN_ID_NOTOP];
    struct token minusOp = tokenList[TOKEN_ID_MINUSOP];

    // ! <unary>
    if (match(peek_next_token(), notToken))
    {
        // !
        read_token();

        // <unary>
        parse_unary();
    }

    // - <unary>
    else if (match(peek_next_token(), minusOp))
    {
        // -
        read_token();

        // <unary>
        parse_unary();
    }

    // <lprimary>
    else
    {
        status = parse_lPrimary();
    }
    
    return status;
}


/**
 * @brief Parses an lprimary clause.
 * 
 * lprimary -> INTLITERAL
 * 
 * @return int 
 */
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