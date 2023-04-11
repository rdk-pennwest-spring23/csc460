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
    parseErrors = 0;

    print_line_to_listings();

    parse_systemGoal();

    write_to_file(listingFilePtr, FMT_LEXICAL_ERRORS_TOTAL, lexicalErrors);
    write_to_file(listingFilePtr, FMT_PARSE_ERROR_TOTAL, parseErrors);

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
    
    /* Check for EOF */
    if (!match( peek_next_token(), scaneofToken ))
    {
        write_to_file(listingFilePtr, FMT_PARSE_ERROR, scaneofToken.name, read_token().name);
        parseErrors++;
    }
    else
        read_token();

    return 1;
    
}

int parse_program()
{
    log_debug("Parsing program.");
    struct token inToken;
    struct token beginToken = tokenList[TOKEN_ID_BEGIN];
    struct token endToken = tokenList[TOKEN_ID_END];

    /* Begin */
    inToken = read_token();
    write_to_file(outputFilePtr, FMT_MATCH_TOKEN, beginToken.name, buffer);
    if ( !match(inToken, beginToken ))
    {
        write_to_file(listingFilePtr, FMT_PARSE_ERROR, beginToken.name, inToken.name);
        parseErrors++;
    }

    /* Statement List */
    parse_statementList();


    /* End */
    inToken = read_token();
    write_to_file(outputFilePtr, FMT_MATCH_TOKEN, endToken.name, buffer);
    if (!match(inToken, endToken))
    {
        write_to_file(listingFilePtr, FMT_PARSE_ERROR, endToken.name, inToken.name);
        parseErrors++;
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

    /* Case 1: <ID> := <expression> */
    if ( match(peek_next_token(), idToken) )
    {
        /* ID */
        read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, idToken.name, buffer);

        /* Assign Op */
        inToken = read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, assignopToken.name, buffer);
        if (!match(inToken, assignopToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, assignopToken.name, inToken.name);
            parseErrors++;
        }

        /* Expression */
        parse_expression();

        /* ; */
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, semiToken.name, buffer);
        inToken = read_token();
        if (!match(inToken, semiToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, semiToken.name, inToken.name);
            parseErrors++;
        }
    }

    /* READ ( <id list> ); */
    else if (match(peek_next_token(), readToken))
    {
        /* READ */
        read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, readToken.name, buffer);

        /* ( */
        inToken = read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, leftparenToken.name, buffer);
        if (!match(leftparenToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, leftparenToken.name, inToken.name);
            parseErrors++;
        }

        /* <id list> */
        parse_idList();

        /* ) */
        inToken = read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, rightparenToken.name, buffer);
        if (!match(rightparenToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, rightparenToken.name, inToken.name);
            parseErrors++;
        }

        /* ; */
        inToken = read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, semiToken.name, buffer);
        if (!match(semiToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, semiToken.name, inToken.name);
            parseErrors++;
        }
    }

    /* WRITE ( <expr list> ); */
    else if (match(peek_next_token(), writeToken))
    {
        /* WRITE */
        read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, writeToken.name, buffer);

        /* ( */
        inToken = read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, leftparenToken.name, buffer);
        if (!match(leftparenToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, leftparenToken.name, inToken.name);
            parseErrors++;
        }

        /* <expr list> */
        parse_exprList();

        /* ) */
        inToken = read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, rightparenToken.name, buffer);
        if (!match(rightparenToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, rightparenToken.name, inToken.name);
            parseErrors++;
        }

        /* ; */
        inToken = read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, semiToken.name, buffer);
        if (!match(semiToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, semiToken.name, inToken.name);
            parseErrors++;
        }
        
    }

    /* IF ( <condition> ) THEN <stmt list> <if tail> */
    else if (match(peek_next_token(), ifToken))
    {
        /* IF */
        read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, ifToken.name, buffer);

        /* ( */
        inToken = read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, leftparenToken.name, buffer);
        if (!match(leftparenToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, leftparenToken.name, inToken.name);
            parseErrors++;
        }

        /* <condition> */
        parse_condition();

        /* ) */
        inToken = read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, rightparenToken.name, buffer);
        if (!match(rightparenToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, rightparenToken.name, inToken.name);
            parseErrors++;
        }

        /* THEN */
        inToken = read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, thenToken.name, buffer);
        if (!match(thenToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, thenToken.name, inToken.name);
            parseErrors++;
        }

        /* <stmt list> */
        parse_statementList();

        /* <if tail> */
        parse_ifTail();

    }

    /* WHILE ( <condition> ) {<statementlist>} ENDWHILE */
    else if (match(peek_next_token(), tokenList[TOKEN_ID_WHILE]))
    {
        /* WHILE */
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, tokenList[TOKEN_ID_WHILE].name, buffer);
        read_token();

        /* ( */
        inToken = read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, leftparenToken.name, buffer);
        if (!match(inToken, leftparenToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, leftparenToken.name, inToken.name);
            parseErrors++;
        }

        /* <condition> */
        parse_condition();

        /* ) */
        inToken = read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, rightparenToken.name, buffer);
        if (!match(inToken, rightparenToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, rightparenToken.name, inToken.name);
            parseErrors++;
        }

        /* <statement list> */
        parse_statementList();

        /* ENDWHILE */
        inToken = read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, tokenList[TOKEN_ID_ENDWHILE].name, buffer);
        if (!match(inToken, tokenList[TOKEN_ID_ENDWHILE]))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, tokenList[TOKEN_ID_ENDWHILE].name, inToken.name);
            parseErrors++;
        }

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

    /* ELSE <stmt list> */
    if (match(peek_next_token(), elseToken))
    {
        /* ELSE */
        read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, elseToken.name, buffer);
        
        /* <stmt list> */
        parse_statementList();
    }

    inToken = read_token();
    write_to_file(outputFilePtr, FMT_MATCH_TOKEN, endifToken.name, buffer);
    if (!match(endifToken, inToken))
    {
        write_to_file(listingFilePtr, FMT_PARSE_ERROR, endifToken.name, inToken.name);
        parseErrors++;
    }

    return status;

}

int parse_idList()
{
    log_debug("Parsing ID List.");
    int status = 1;
    struct token inToken;
    struct token idToken = tokenList[TOKEN_ID_ID];
    struct token commaToken = tokenList[TOKEN_ID_COMMA];

    /* ID */
    inToken = read_token();
    write_to_file(outputFilePtr, FMT_MATCH_TOKEN, idToken.name, buffer);
    if (!match(idToken, inToken))
    {
        write_to_file(listingFilePtr, FMT_PARSE_ERROR, idToken.name, inToken.name);
        status = 0;
        parseErrors++;
    }

    /* Optional: , <id list> */
    if (match(peek_next_token(), commaToken))
    {
        /* , */
        read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, commaToken.name, buffer);

        /* <id list> */
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
        /* , */
        read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, commaToken.name, buffer);

        /* <expr list> */
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
        if (parse_addOp())
        {
            /* + */
            read_token();
            write_to_file(outputFilePtr, FMT_MATCH_TOKEN, plusopToken.name, buffer);

            /* <term> */
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
            /* * */
            read_token();
            write_to_file(outputFilePtr, FMT_MATCH_TOKEN, multopToken.name, buffer);

            /* <term> */
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

    /* ( <expression> ) */
    if (match(peek_next_token(), leftparenToken))
    {
        /* ( */
        read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, leftparenToken.name, buffer);

        /* <expression> */
        parse_expression();

        /* ) */
        inToken = read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, rightparenToken.name, buffer);
        if (!match(rightparenToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, rightparenToken.name, inToken.name);
            parseErrors++;
        }
    }

    /* - <factor> */
    else if (match(peek_next_token(), minusopToken))
    {
        /* - */
        read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, minusopToken.name, buffer);

        /* <factor> */
        parse_factor();
    }

    /* ID */
    else if (match(peek_next_token(), idToken))
    {
        read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, idToken.name, buffer);
    }

    /* INTLITERAL */
    else if (match(peek_next_token(), intlitToken))
    {
        read_token();
        write_to_file(outputFilePtr, FMT_MATCH_TOKEN, intlitToken.name, buffer);
    }

    else
        status = 0;
    
    return status;
}

int parse_addOp()
{
    log_debug("Parsing AddOp.");
    int status = 1;
    struct token peekToken;

    peekToken = peek_next_token();

    if (match(peekToken, tokenList[TOKEN_ID_PLUSOP]) || match(peekToken, tokenList[TOKEN_ID_MINUSOP]))
        read_token();
    else
        status = 0;

    return status;
}

int parse_multOp()
{
    log_debug("Parsing MultOp");
    int status = 1;
    struct token peekToken;

    peekToken = peek_next_token();

    if (match(peekToken, tokenList[TOKEN_ID_MULTOP]) || match(peekToken, tokenList[TOKEN_ID_DIVOP]))
        read_token();
    else
        status = 0;

    return status;
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

    /* <addition> */
    status = parse_addition();

    /* <relop> */
    if (status = parse_relOp())
    {
        /* <addition> */
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
    log_debug("Parsing addition.");
    int status = 1;
    /* <multiplication> */
    status = parse_multiplication();

    /* <addOp> */
    if (status = parse_addOp())
    {
        /* <multiplication> */
        status = parse_multiplication();
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
    log_debug("Parsing multiplication.");
    int status = 1;

    /* <unary> */
    status = parse_unary();

    /* <multOp> */
    if (status = parse_multOp())
    {
        /* <unary> */
        status = parse_unary();
    }

    return status;
}


/**
 * @brief Parses a unary clause
 * 
 * UNARY    -> ! <unary>
 *          -> - <unary>
 *          -> <lprimary> 
 * 
 * @return int 
 */
int parse_unary()
{
    log_debug("Parse unary.");
    int status = 1;
    struct token inToken;
    struct token notToken = tokenList[TOKEN_ID_NOTOP];
    struct token minusOp = tokenList[TOKEN_ID_MINUSOP];

    /* ! <unary> */
    if (match(peek_next_token(), notToken))
    {
        /* ! */
        read_token();

        /* <unary> */
        status = parse_unary();
    }

    /* - <unary> */
    else if (match(peek_next_token(), minusOp))
    {
        /* - */
        read_token();

        /* <unary> */
        status = parse_unary();
    }

    /* <lprimary> */
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
 *          -> ID
 *          -> ( <condition> )
 *          -> FALSEOP
 *          -> TRUEOP
 *          -> NULLOP
 * 
 * @return int 
 */
int parse_lPrimary()
{
    log_debug("Parsing lprimary.");
    int status = 1;
    struct token inToken, peekToken;
    struct token intlitToken = tokenList[TOKEN_ID_INTLITERAL];
    struct token idToken = tokenList[TOKEN_ID_ID];
    struct token falseToken = tokenList[TOKEN_ID_FALSE];
    struct token trueToken = tokenList[TOKEN_ID_TRUE];
    struct token nullToken = tokenList[TOKEN_ID_NULL];
    struct token leftparenToken = tokenList[TOKEN_ID_LPAREN];
    struct token rightparenToken = tokenList[TOKEN_ID_RPAREN];

    peekToken = peek_next_token();

    /* INTLITERAL */
    if (match(peekToken, intlitToken))
        read_token();

    /* ID */
    else if (match(peekToken, idToken))
        read_token();

    /* ( <condition> ) */
    else if (match(peekToken, leftparenToken))
    {
        /* ( */
        read_token();

        /* <condition> */
        status = parse_condition();

        /* ) */
        inToken = read_token();
        if (!match(rightparenToken, inToken))
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, rightparenToken.name, inToken.name);
    }

    /* FALSEOP */
    else if (match(peekToken, falseToken))
        read_token();

    /* TRUEOP */
    else if (match(peekToken, trueToken))
        read_token();

    /* NULLOP */
    else if (match(peekToken, nullToken))
        read_token();

    else
        status = 0;

    return status;
}

/**
 * @brief Parses a RelOp clause.
 * 
 * @return int 
 */
int parse_relOp()
{
    log_debug("Parse RelOp");
    int status = 1;
    struct token peekToken = peek_next_token();

    if (match(peekToken, tokenList[TOKEN_ID_LESSOP]) || 
        match(peekToken, tokenList[TOKEN_ID_LESSEQUALOP]) || 
        match(peekToken, tokenList[TOKEN_ID_GREATEROP]) || 
        match(peekToken, tokenList[TOKEN_ID_GREATEREQUALOP]) || 
        match(peekToken, tokenList[TOKEN_ID_EQUALOP]) || 
        match(peekToken, tokenList[TOKEN_ID_NOTEQUALOP])
    )
        read_token();
    else
        status = 0;

    return status;
}

/* EOF */