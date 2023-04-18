/**
 * @file parser.c
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * @brief An LL(1) Parser implementation
 */

#include "scanner.h"
#include "parser.h"
#include "generator.h"
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
    log_debug("Parsing System Goal.");

    parse_program();
        
    /* Check for EOF */
    if (!match( peek_next_token(), scaneofToken ))
    {
        write_to_file(listingFilePtr, FMT_PARSE_ERROR, scaneofToken.name, read_token().name);
        parseErrors++;
    }
    else
        read_token();

    generate_finish();

    return 1;
    
}

int parse_program()
{     
    log_debug("Parsing Program.");
    
    struct token inToken;
    
    /* #start */
    generate_start();

    /* Begin */
    inToken = read_token();
    
    if ( !match(inToken, beginToken ))
    {
        write_to_file(listingFilePtr, FMT_PARSE_ERROR, beginToken.name, inToken.name);
        parseErrors++;
    }

    /* Statement List */
    parse_statementList(); 

    /* End */
    inToken = read_token();
    
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
    log_debug("Parsing StatementList.");

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
    log_debug("Parsing Statement.");

    int status = 1;
    struct token inToken;

    /* Case 1: <ID> := <expression> */
    if ( match(peek_next_token(), idToken) )
    {
        struct expr_rec source;
        
        /* ID */
        struct expr_rec target = parse_ident();
        

        /* Assign Op */
        inToken = read_token();
        
        if (!match(inToken, assignOpToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, assignOpToken.name, inToken.name);
            parseErrors++;
        }

        /* Expression */
        parse_expression(&source);

        /* ; */
        inToken = read_token();
        if (!match(inToken, semicolonToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, semicolonToken.name, inToken.name);
            parseErrors++;
        }
        log_debug("Source expression for assignment: %s", source.e);
        assign(target, source);
    }

    /* READ ( <id list> ); */
    else if (match(peek_next_token(), readToken))
    {

        struct expr_rec readExpr;
        
        /* READ */
        read_token();

        /* ( */
        inToken = read_token();
        
        if (!match(LParenToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, LParenToken.name, inToken.name);
            parseErrors++;
        }

        /* <id list> */
        parse_idList();

        /* ) */
        inToken = read_token();
        
        if (!match(RParenToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, RParenToken.name, inToken.name);
            parseErrors++;
        }

        /* ; */
        inToken = read_token();
        
        if (!match(semicolonToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, semicolonToken.name, inToken.name);
            parseErrors++;
        }
    }

    /* WRITE ( <expr list> ); */
    else if (match(peek_next_token(), writeToken))
    {
        
        struct expr_rec writeExpr;

        /* WRITE */
        read_token();
        

        /* ( */
        inToken = read_token();
        
        if (!match(LParenToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, LParenToken.name, inToken.name);
            parseErrors++;
        }

        /* <expr list> */
        parse_exprList();

        /* ) */
        inToken = read_token();
        
        if (!match(RParenToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, RParenToken.name, inToken.name);
            parseErrors++;
        }

        /* ; */
        inToken = read_token();
        
        if (!match(semicolonToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, semicolonToken.name, inToken.name);
            parseErrors++;
        }
        
    }

    /* IF ( <condition> ) THEN <stmt list> <if tail> */
    else if (match(peek_next_token(), ifToken))
    {
        struct expr_rec condRec;

        /* IF */
        read_token();

        /* ( */
        inToken = read_token();
        
        if (!match(LParenToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, LParenToken.name, inToken.name);
            parseErrors++;
        }

        /* <condition> */
        parse_condition(&condRec);

        /* ) */
        inToken = read_token();
        
        if (!match(RParenToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, RParenToken.name, inToken.name);
            parseErrors++;
        }

        /* THEN */
        inToken = read_token();
        
        if (!match(thenToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, thenToken.name, inToken.name);
            parseErrors++;
        }

        generate("if (%s) {", condRec.e);

        /* <stmt list> */
        parse_statementList();

        generate("}");

        /* <if tail> */
        parse_ifTail();

    }

    /* WHILE ( <condition> ) {<statementlist>} ENDWHILE */
    else if (match(peek_next_token(), tokenList[TOKEN_ID_WHILE]))
    {
        struct expr_rec condRec;
        /* WHILE */
        read_token();

        /* ( */
        inToken = read_token();
        
        if (!match(inToken, LParenToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, LParenToken.name, inToken.name);
            parseErrors++;
        }

        /* <condition> */
        parse_condition(&condRec);

        /* ) */
        inToken = read_token();
        
        if (!match(inToken, RParenToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, RParenToken.name, inToken.name);
            parseErrors++;
        }

        generate("while(%s) {", condRec.e);

        /* <statement list> */
        parse_statementList();

        /* ENDWHILE */
        inToken = read_token();
        
        if (!match(inToken, tokenList[TOKEN_ID_ENDWHILE]))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, tokenList[TOKEN_ID_ENDWHILE].name, inToken.name);
            parseErrors++;
        }

        generate("}");

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

    log_debug("Parsing IfTail.");
    
    int status = 1;
    struct token inToken;

    /* ELSE <stmt list> */
    if (match(peek_next_token(), elseToken))
    {
        /* ELSE */
        read_token();

        generate("else {");
        
        /* <stmt list> */
        parse_statementList();

        generate("}");
    }

    inToken = read_token();
    
    if (!match(endIfToken, inToken))
    {
        write_to_file(listingFilePtr, FMT_PARSE_ERROR, endIfToken.name, inToken.name);
        parseErrors++;
    }

    return status;

}

/**
 * @brief 
 * 
 * <id list> -> <ident> {, <ident>}
 * 
 * @return int 
 */
int parse_idList()
{
    log_debug("Parsing IDList");
    
    int status = 1;
    struct token inToken;
    struct expr_rec exprRec;
    
    /* ID */
    if (match(peek_next_token(), idToken))
    {
        exprRec = parse_ident();
        read_id(exprRec);
    }
    else
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

        if (match(peek_next_token(), idToken))
        {
            exprRec = parse_ident();
            read_id(exprRec);
        }
        else
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, idToken.name, inToken.name);
            status = 0;
            parseErrors++;
        }
    }

    return status;
}

int parse_exprList()
{
    
    log_debug("Parsing ExprList");

    int status = 1;
    struct token inToken;
    struct expr_rec exprRec;

    parse_expression(&exprRec);
    write_expr(exprRec);

    if (match(peek_next_token(), commaToken))
    {
        /* , */
        read_token();

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
int parse_expression(struct expr_rec * exprRec)
{
    log_debug("Parsing expression.");

    int status = 1;
    struct expr_rec term1, term2;
    struct op_rec opRec;

    status = parse_term(&term1);

    if(parse_addOp(&opRec))
    {
        parse_term(&term2);

        *exprRec = gen_infix(term1.e, opRec.operation, term2.e);
    }
    else
    {
        *exprRec = term1;
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
int parse_term(struct expr_rec * exprRec)
{

    log_debug("Parsing Term.");

    int status = 1;
    struct expr_rec term1, term2;
    struct op_rec opRec;

    status = parse_factor(&term1);

    if (status && parse_multOp(&opRec))
    {
        status = parse_factor(&term2);
        *exprRec = gen_infix(term1.e, opRec.operation, term2.e);
    }
    else
    {
        *exprRec = term1;
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
int parse_factor(struct expr_rec * exprRec)
{
    log_debug("Parsing Factor.");

    int status = 1;
    struct token inToken;
    struct expr_rec term1, term2;

    /* ( <expression> ) */
    if (match(peek_next_token(), LParenToken))
    {
        /* ( */
        read_token();

        /* <expression> */
        parse_expression(&term1);

        /* ) */
        inToken = read_token();
        
        if (!match(RParenToken, inToken))
        {
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, RParenToken.name, inToken.name);
            parseErrors++;
        }

        *exprRec = term1;
    }

    /* - <factor> */
    else if (match(peek_next_token(), minusOpToken))
    {
        /* - */
        read_token();

        /* <factor> */
        parse_factor(&term1);

        *exprRec = term1;
    }

    /* ID */
    else if (match(peek_next_token(), idToken))
    {   
        *exprRec = parse_ident();
    }

    /* INTLITERAL */
    else if (match(peek_next_token(), intToken))
    {
        read_token();
        *exprRec = process_literal(buffer);
    }

    else
        status = 0;
    
    return status;
}

int parse_addOp(struct op_rec * opRec)
{
    log_debug("Parsing AddOp.");

    int status = 1;
    struct token peekToken;

    peekToken = peek_next_token();

    if (match(peekToken, plusOpToken) || match(peekToken, minusOpToken))
    {
        read_token();
        *opRec = process_op(buffer);
    }
    else
        status = 0;

    return status;
}

int parse_multOp(struct op_rec * opRec)
{
    
    log_debug("Parsing MultOp.");

    int status = 1;
    struct token peekToken;

    peekToken = peek_next_token();

    if (match(peekToken, multOpToken) || match(peekToken, divOpToken))
    {
        read_token();
        *opRec = process_op(buffer);
    }
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
int parse_condition(struct expr_rec * exprRec)
{

    log_debug("Parsing Condition.");

    int status = 1;
    struct op_rec opRec;
    struct expr_rec leftRec, rightRec;

    /* <addition> */
    status = parse_addition(&leftRec);

    /* <relop> */
    if (status = parse_relOp(&opRec))
    {
        /* <addition> */
        status = parse_addition(&rightRec);

        *exprRec = gen_infix(leftRec.e, opRec.operation, rightRec.e);
    }
    else
    {
        *exprRec = leftRec;
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
int parse_addition(struct expr_rec * exprRec)
{
    
    log_debug("Parsing addition.");

    int status = 1;
    struct expr_rec leftRec, rightRec;
    struct op_rec opRec;

    /* <multiplication> */
    status = parse_multiplication(&leftRec);

    /* <addOp> */
    if (status = parse_addOp(&opRec))
    {
        /* <multiplication> */
        status = parse_multiplication(&rightRec);

        *exprRec = gen_infix(leftRec.e, opRec.operation, rightRec.e);
    }
    else
    {
        *exprRec = leftRec;
    }

    return status;
}

/**
 * @brief Parses a multiplication clause
 * 
 * MULTIPLICATION -> <unary> {<multOp> <unary>}
 * 
 * @return int 
 */
int parse_multiplication(struct expr_rec * exprRec)
{

    log_debug("Parsing Multiplication.");
    
    int status = 1;
    struct expr_rec leftRec, rightRec;
    struct op_rec opRec;

    /* <unary> */
    status = parse_unary(&leftRec);

    /* <multOp> */
    if (status = parse_multOp(&opRec))
    {
        /* <unary> */
        status = parse_unary(&rightRec);

        *exprRec = gen_infix(leftRec.e, opRec.operation, rightRec.e);
    }
    else
    {
        *exprRec = leftRec;
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
int parse_unary(struct expr_rec * outRec)
{

    log_debug("Parsing Unary.");
    
    int status = 1;
    struct token inToken;
    struct expr_rec exprRec;

    /* ! <unary> */
    if (match(peek_next_token(), notOpToken))
    {
        /* ! */
        read_token();

        /* <unary> */
        status = parse_unary(&exprRec);
        
        *outRec = gen_infix("", "!", exprRec.e);
    }

    /* - <unary> */
    else if (match(peek_next_token(), minusOpToken))
    {
        /* - */
        read_token();

        /* <unary> */
        status = parse_unary(&exprRec);
        *outRec = gen_infix("", "-", exprRec.e);
    }

    /* <lprimary> */
    else
    {
        status = parse_lPrimary(outRec);
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
int parse_lPrimary(struct expr_rec * exprRec)
{

    log_debug("Parsing LPrimary.");

    int status = 1;
    struct token inToken, peekToken;
    peekToken = peek_next_token();

    /* INTLITERAL */
    if (match(peekToken, intToken))
    {
        read_token();
        *exprRec = process_literal(buffer);
    }

    /* ID */
    else if (match(peekToken, idToken))
    {
        *exprRec = parse_ident();
    }

    /* ( <condition> ) */
    else if (match(peekToken, LParenToken))
    {
        /* ( */
        read_token();

        /* <condition> */
        status = parse_condition(exprRec);

        /* ) */
        inToken = read_token();
        if (!match(RParenToken, inToken))
            write_to_file(listingFilePtr, FMT_PARSE_ERROR, RParenToken.name, inToken.name);
    }

    /* FALSEOP */
    else if (match(peekToken, falseToken))
    {
        read_token();
        *exprRec = process_literal("0");
    }

    /* TRUEOP */
    else if (match(peekToken, trueToken))
    {
        read_token();
        *exprRec = process_literal("1");
    }

    /* NULLOP */
    else if (match(peekToken, nullToken))
    {
        read_token();
        *exprRec = process_literal("1");
    }

    else
        status = 0;

    return status;
}

/**
 * @brief Parses a RelOp clause.
 * 
 * @return int 
 */
int parse_relOp(struct op_rec * opRec)
{

    log_debug("Parsing RelOp.");

    int status = 1;
    struct token peekToken = peek_next_token();

    if (match(peekToken, lessOpToken))
    {
        read_token();
        *opRec = process_op("<");
    }
    else if (match(peekToken, lessEqualOpToken))
    {
        read_token();
        *opRec = process_op("<=");
    }
    else if (match(peekToken, greaterOpToken))
    {
        read_token();
        *opRec = process_op(">");
    }
    else if (match(peekToken, greaterEqualOpToken))
    {
        read_token();
        *opRec = process_op(">=");
    }
    else if (match(peekToken, equalOpToken))
    {
        read_token();
        *opRec = process_op("==");
    }
    else if (match(peekToken, notEqualOpToken))
    {
        read_token();
        *opRec = process_op("!=");
    }
    else
        status = 0;

    return status;
}

struct expr_rec parse_ident()
{

    log_debug("Parsing Ident.");
    
    struct expr_rec rec;
     
    if (match(peek_next_token(), idToken))
    {
        read_token();
        strcpy(rec.e, buffer);
        rec.type = IDExpression;
        process_id(buffer);
    }
    else
        rec.type = NullExpression;

    return rec;
}

/* EOF */