/**
 * @file scanner.h
 * 
 * CSC 460 Language Translations
 * Group 2
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * 
 * @brief Program 2: Scanner
 */

#ifndef SCANNER_H
#define SCANNER_H

#define FMT_LINE_FEED "[%d] %s"
#define FMT_TOKEN_LINE "[%d] Token ID: %3d \t Token Type: %15s \t Actual Token: %s"

struct token {
    int id;
    char *name;
};

struct token errorToken, idToken, intToken;

static int stateID = 0;

struct state {
    int id;
    struct state *transition_table[128];
    struct token token;
};

struct state *startState;
struct state *idState;
struct state *intState;

int init_scanner();

int run_scanner();

struct state * create_new_transition(struct state *state, struct token token);

void add_alpha_transition(struct state *curState, char *str, struct token target);

void add_symbol_transition(struct state *curState, char *str, struct token target);


#endif

// EOF