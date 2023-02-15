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

#include <stdio.h>
#include <stdlib.h>

int init_scanner()
{
    log_info("Initializing scanner.");
    int i;

    // Set up the special tokens
    log_debug("Initializing special tokens.");
    errorToken = (struct token) {0, "ERROR"};
    idToken = (struct token) {1, "ID"};
    intToken = (struct token) {2, "INTLITERAL"};

    // Set up the special states
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

    // Add the string tokens
    log_debug("Adding alpha transitions.");
    add_alpha_transition(startState, "BEGIN", (struct token){3, "BEGIN"});
    add_alpha_transition(startState, "END", (struct token){4, "END"});
    add_alpha_transition(startState, "READ", (struct token){5, "READ"});
    add_alpha_transition(startState, "WRITE", (struct token){6, "WRITE"});
    add_alpha_transition(startState, "IF", (struct token){7, "IF"});
    add_alpha_transition(startState, "THEN", (struct token){8, "THEN"});
    add_alpha_transition(startState, "ELSE", (struct token){9, "ELSE"});
    add_alpha_transition(startState, "ENDIF", (struct token){10, "ENDIF"});
    add_alpha_transition(startState, "WHILE", (struct token){11, "WHILE"});
    add_alpha_transition(startState, "ENDWHILE", (struct token){12, "ENDWHILE"});
    add_alpha_transition(startState, "FALSE", (struct token){13, "FALSEOP"});
    add_alpha_transition(startState, "TRUE", (struct token){14, "TRUEOP"});
    add_alpha_transition(startState, "NULL", (struct token){15, "NULLOP"});

    // Add the integer transitions
    log_debug("Adding integer transitions.");
    for (i = '0'; i <= '9'; i++)
        startState->transition_table[i] = intState;

    // Add the symbol transitions
    log_debug("Adding symbol transitions.");
    add_symbol_transition(startState, "(", (struct token){16, "LPAREN"});
    add_symbol_transition(startState, ")", (struct token){17, "RPAREN"});
    add_symbol_transition(startState, ";", (struct token){18, "SEMICOLON"});
    add_symbol_transition(startState, ",", (struct token){19, "COMMA"});
    add_symbol_transition(startState, ":=", (struct token){20, "ASSIGNOP"});
    add_symbol_transition(startState, "+", (struct token){21, "PLUSOP"});
    add_symbol_transition(startState, "-", (struct token){22, "MINUSOP"});
    add_symbol_transition(startState, "*", (struct token){23, "MULTOP"});
    add_symbol_transition(startState, "/", (struct token){24, "DIVOP"});
    add_symbol_transition(startState, "!", (struct token){25, "NOTOP"});
    add_symbol_transition(startState, "<", (struct token){26, "LESSOP"});
    add_symbol_transition(startState, "<=", (struct token){27, "LESSEQUALOP"});
    add_symbol_transition(startState, ">", (struct token){28, "GREATEROP"});
    add_symbol_transition(startState, ">=", (struct token){29, "GREATEREQUALOP"});
    add_symbol_transition(startState, "=", (struct token){30, "EQUALOP"});
    add_symbol_transition(startState, "<>", (struct token){31, "NOTEQUALOP"});
    add_symbol_transition(startState, "--", (struct token){32, "COMMENT"});

    // Handle Negative Numbers
    log_debug("Handling negative numbers transitions.");
    struct state *minusState = startState->transition_table['-'];
    for (i = '0'; i <= '9'; i++)
        minusState->transition_table[i] = intState;

    for (i = 'A'; i <= 'Z'; i++)
    {
        if (startState->transition_table[i] == startState)
        {
            startState->transition_table[i] = idState;    // Upper case alphas
            startState->transition_table[i+32] = idState; // Lower case alphas
        }
    }

    return 1;
}


int run_scanner()
{
    char curChar, buffer[1000];
    memset(buffer, 0, 1000);
    struct state *curState = startState;

    curChar = fgetc(inputFilePtr);

    while (curChar != EOF)
    {
        if ( curState->transition_table[curChar] == startState )
        {
            // End of Token
            struct token token = curState->token;
            log_info(FMT_TOKEN_LINE, 0, token.id, token.name, buffer);
            curState = startState;
            memset(buffer, 0, 1000);

            // If token is a comment, go to next line
            if (token.id == 32)
            {
                while(curChar != 10)
                    curChar = fgetc(inputFilePtr);
            }
            
            // Go past the whitespace
            do{
                curChar = fgetc(inputFilePtr);
            } while (curChar == ' ' || curChar == '\t' || curChar == 10 || curChar == 13);
        }
        else {
            curState = curState->transition_table[curChar];
            strncat(buffer, &curChar, 1);
            struct token token = curState->token;
            log_info(FMT_TOKEN_LINE, 0, token.id, token.name, buffer);
            for (int i = 0; i < 128; i++)
            {
                printf("%d ", startState->transition_table[i]->id);
            }
            printf("\n");
            curChar = fgetc(inputFilePtr);
        }
    }

    struct token token = curState->token;
    log_info(FMT_TOKEN_LINE, 0, token.id, token.name, buffer);
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

    // If this is the last state
    if (str[1] == '\0')
    {
        curState->token = target;
    }

    if (str[0] != '\0')
    {
        struct state *nextState;
        if (curState->transition_table[curChar]->id == 0)
        {
            nextState = create_new_transition(nextState, errorToken);
            curState->transition_table[curChar] = nextState;
        }
        else
        {
            nextState = curState->transition_table[curChar];
        }
        add_alpha_transition(nextState, ++str, target);
    }
    
    // Setup the transition to any other alpha characters
    int i;
    for (i = 'A'; i <= 'Z'; i++)
    {
        if (curState->transition_table[i] == startState)
        {
            curState->transition_table[i] = idState;    // Upper case alphas
            curState->transition_table[i+32] = idState; // Lower case alphas
        }
    }

}


void add_symbol_transition(struct state *curState, char *str, struct token target)
{
    char curChar = str[0];

    // If this is the last state
    if (curChar == '\0')
    {
        curState->token = target;
    }
    else
    {
        struct state *nextState;
        if (curState->transition_table[curChar]->id == 0)
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

// EOF