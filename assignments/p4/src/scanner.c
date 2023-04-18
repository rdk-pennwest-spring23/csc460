/**
 * @file scanner.c
 * 
 * CSC 460 Language Translations
 * Group 2
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * 
 * @brief Program 2: Scanner
 */

#include "scanner.h"
#include "log_util.h"
#include "file_util.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_scanner()
{
    log_info("Initializing scanner.");
    int i;

    lineNumber = 1;
    lexicalErrors = 0;

    /* Set up the special tokens */
    log_debug("Initializing special tokens.");
    errorToken = create_token (0, "ERROR");
    idToken = create_token (1, "ID");
    intToken = create_token (2, "INTLITERAL");
    scaneofToken = create_token (33, "SCANEOF");
    newlineToken = create_token(34, "NEWLINE");

    /* Set up the special states */
    log_debug("Initializing special states.");
    startState = create_new_transition(startState, errorToken);
    for (i = 0; i < 128; i++)
        startState->transition_table[i] = startState;

    idState = create_new_transition(idState, idToken);
    for (i = 'A'; i <= 'Z'; i++)
    {
        idState->transition_table[i] = idState;
        idState->transition_table[i+32] = idState;
    }

    intState = create_new_transition(intState, intToken);
    for (i = '0'; i <= '9'; i++)
        intState->transition_table[i] = intState;


    /* Add the string tokens */
    log_debug("Adding alpha transitions.");
    add_alpha_transition(startState, "BEGIN", beginToken = create_token(3, "BEGIN"));
    add_alpha_transition(startState, "END", endToken = create_token(4, "END"));
    add_alpha_transition(startState, "READ", readToken = create_token(5, "READ"));
    add_alpha_transition(startState, "WRITE", writeToken = create_token(6, "WRITE"));
    add_alpha_transition(startState, "IF", ifToken = create_token(7, "IF"));
    add_alpha_transition(startState, "THEN", thenToken = create_token(8, "THEN"));
    add_alpha_transition(startState, "ELSE", elseToken = create_token(9, "ELSE"));
    add_alpha_transition(startState, "ENDIF", endIfToken = create_token(10, "ENDIF"));
    add_alpha_transition(startState, "WHILE", whileToken = create_token(11, "WHILE"));
    add_alpha_transition(startState, "ENDWHILE", endWhileToken = create_token(12, "ENDWHILE"));
    add_alpha_transition(startState, "FALSE", falseToken = create_token(13, "FALSEOP"));
    add_alpha_transition(startState, "TRUE", trueToken = create_token(14, "TRUEOP"));
    add_alpha_transition(startState, "NULL", nullToken = create_token(15, "NULLOP"));

    /* Add the integer transitions */
    log_debug("Adding integer transitions.");
    for (i = '0'; i <= '9'; i++)
        startState->transition_table[i] = intState;

    /* Add the symbol transitions */
    log_debug("Adding symbol transitions.");
    add_symbol_transition(startState, "(", LParenToken = create_token(16, "LPAREN"));
    add_symbol_transition(startState, ")", RParenToken = create_token(17, "RPAREN"));
    add_symbol_transition(startState, ";", semicolonToken = create_token(18, "SEMICOLON"));
    add_symbol_transition(startState, ",", commaToken = create_token(19, "COMMA"));
    add_symbol_transition(startState, ":=", assignOpToken = create_token(20, "ASSIGNOP"));
    add_symbol_transition(startState, "+", plusOpToken = create_token(21, "PLUSOP"));
    add_symbol_transition(startState, "-", minusOpToken = create_token(22, "MINUSOP"));
    add_symbol_transition(startState, "*", multOpToken = create_token(23, "MULTOP"));
    add_symbol_transition(startState, "/", divOpToken = create_token(24, "DIVOP"));
    add_symbol_transition(startState, "!", notOpToken = create_token(25, "NOTOP"));
    add_symbol_transition(startState, "<", lessOpToken = create_token(26, "LESSOP"));
    add_symbol_transition(startState, "<=", lessEqualOpToken = create_token(27, "LESSEQUALOP"));
    add_symbol_transition(startState, ">", greaterOpToken = create_token(28, "GREATEROP"));
    add_symbol_transition(startState, ">=", greaterEqualOpToken = create_token(29, "GREATEREQUALOP"));
    add_symbol_transition(startState, "=", equalOpToken = create_token(30, "EQUALOP"));
    add_symbol_transition(startState, "<>", notEqualOpToken = create_token(31, "NOTEQUALOP"));
    add_symbol_transition(startState, "--", commentToken = create_token(32, "COMMENT"));
    

    /* Handle Negative Numbers */
    log_debug("Handling negative numbers transitions.");
    struct state *minusState = startState->transition_table['-'];
    for (i = '0'; i <= '9'; i++)
        minusState->transition_table[i] = intState;
 
    return 1;
}


struct token peek_next_token()
{
    /* Save the current position in the file */
    long filePos = ftell(inputFilePtr);

    /* Get the next token */
    struct token returnToken;

    /* Make sure we don't use a newline token */
    do {
        returnToken = get_next_token();
    } while (returnToken.id == newlineToken.id || returnToken.id == tokenList[TOKEN_ID_COMMENT].id);

    /* Reset the file pointer to the previous position */
    fseek(inputFilePtr, filePos, SEEK_SET);

    /* Return the token */
    return returnToken;
}

struct token get_next_token()
{
    char curChar;
    int foundToken = 0;
    struct state *curState = startState;
    struct token returnToken;
    memset(buffer, 0, 1000);

    curChar = fgetc(inputFilePtr);

    /* Skip whitespace */
    while (curChar == ' ' || curChar == '\t')
        curChar = fgetc(inputFilePtr);

    /* Check for new Line */
    if (curChar == 13 || curChar == 10)
    {
        foundToken = 1;
        returnToken = newlineToken;
        while(curChar != 10)
            curChar = fgetc(inputFilePtr);
    }

    /* If we're at the end of the file, return the SCANEOF Token */
    if (curChar == EOF)
    {
        foundToken = 1;
        returnToken = scaneofToken;
    }
    
    while(!foundToken)
    {
        /* Upper case if char is an alpha */
        if (curChar >= 'a' && curChar <= 'z')
            curChar ^= 0x20;

        /* Check if the transition is to a valid state */
        if (curChar == EOF)
        {
            /* We found our token */
            foundToken = 1;
            returnToken = curState->token;
        }
        else if ( curState->transition_table[curChar]->id != startState->id )
        {
            curState = curState->transition_table[curChar];
            buffer[strlen(buffer)] = curChar;
            curChar = fgetc(inputFilePtr);
        }
        /* If the transition is to an invalid state */
        else 
        {
            /* Rewind one character */
            if (curState->token.id != errorToken.id)
                fseek(inputFilePtr, -1, SEEK_CUR);

            /* We found our token */
            foundToken = 1;
            returnToken = curState->token;
        }
    }

    if (returnToken.id == errorToken.id)
    {
        lexicalErrors++;
        write_to_file(listingFilePtr, FMT_TOKEN_LINE, returnToken.id, returnToken.name, buffer);
    }

    if (returnToken.id == tokenList[TOKEN_ID_COMMENT].id)
    {
        while (curChar != '\n' && curChar != EOF)
            curChar = fgetc(inputFilePtr);
            
        fseek(inputFilePtr, -1, SEEK_CUR);
    }


    return returnToken;
    
}


struct token read_token()
{
    struct token returnToken;

    /* Get the next token */
    returnToken = get_next_token();

    /* Check for new lines */
    while (returnToken.id == newlineToken.id || returnToken.id == tokenList[TOKEN_ID_COMMENT].id)
    {
        if (returnToken.id == newlineToken.id)
        {
            write_to_file(listingFilePtr, "");
            print_line_to_listings();

            /* Write out the Statement Buffer to the Output File */
            memset(stmtBuffer, 0, 1000);
        }
        /* Get the next token */
        returnToken = get_next_token();
    }

    strcat(stmtBuffer, buffer);
    log_debug("Current Token: %s", buffer);

    return returnToken;
}


struct state * create_new_transition(struct state *state, struct token token)
{
    state = malloc(sizeof(struct state));
    state->id = stateID++;
    state->token = token;
    int i;
    for (i = 0; i < 128; i++)
    {
        state->transition_table[i] = startState;
    }
    return state;
}


void add_alpha_transition(struct state *curState, char *str, struct token target)
{
    char curChar = str[0];

    /* If this is the last state */
    if (curChar == '\0')
    {
        curState->token = target;
    }

    if (str[0] != '\0')
    {
        struct state *nextState;
        if (curState->transition_table[curChar] == startState || curState->transition_table[curChar] == idState)
        {
            nextState = create_new_transition(nextState, errorToken);
            nextState->token = idToken;
            curState->transition_table[curChar] = nextState;
        }
        else
        {
            nextState = curState->transition_table[curChar];
        }
        add_alpha_transition(nextState, ++str, target);
    }
    
    /* Setup the transition to any other alpha characters */
    int i;
    for (i = 'A'; i <= 'Z'; i++)
    {
        if (curState->transition_table[i]->id == startState->id)
        {
            curState->transition_table[i] = idState;    /* Upper case alphas */
            curState->transition_table[i+32] = idState; /* Lower case alphas */
        }
    }

}


void add_symbol_transition(struct state *curState, char *str, struct token target)
{
    char curChar = str[0];

    /* If this is the last state */
    if (curChar == '\0')
    {
        curState->token = target;
    }
    else
    {
        struct state *nextState;
        if (curState->transition_table[curChar]->id == startState->id)
        {
            nextState = create_new_transition(nextState, errorToken);
            curState->transition_table[curChar] = nextState;
        }
        else
        {
            nextState = curState->transition_table[curChar];
        }
        add_symbol_transition(nextState, ++str, target);
    }
}

struct token create_token(int id, char* name)
{
    struct token newToken;
    newToken.id = id;
    newToken.name = name;
    tokenList[id] = newToken;
    return newToken;
}

void print_line_to_listings()
{
    char buffer[1000], c;
    int pos = 0;
    long startPos = ftell(inputFilePtr);

    /*  */
    c = fgetc(inputFilePtr);
    while (c != 13 && c != EOF)
    {
        buffer[pos] = c;
        pos++;
        c = fgetc(inputFilePtr);
    }
    buffer[pos] = 0;
    write_to_file(listingFilePtr, FMT_LINE_FEED, lineNumber++, buffer);

    /* Return the file to the previous position */
    fseek(inputFilePtr, startPos, SEEK_SET);
}



/* EOF */