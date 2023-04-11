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

#define TOKEN_ID_ERROR          0
#define TOKEN_ID_ID             1
#define TOKEN_ID_INTLITERAL     2
#define TOKEN_ID_BEGIN          3
#define TOKEN_ID_END            4
#define TOKEN_ID_READ           5
#define TOKEN_ID_WRITE          6
#define TOKEN_ID_IF             7
#define TOKEN_ID_THEN           8
#define TOKEN_ID_ELSE           9
#define TOKEN_ID_ENDIF          10
#define TOKEN_ID_WHILE          11
#define TOKEN_ID_ENDWHILE       12
#define TOKEN_ID_FALSE          13
#define TOKEN_ID_TRUE           14
#define TOKEN_ID_NULL           15
#define TOKEN_ID_LPAREN         16
#define TOKEN_ID_RPAREN         17
#define TOKEN_ID_SEMICOLON      18
#define TOKEN_ID_COMMA          19
#define TOKEN_ID_ASSIGNOP       20
#define TOKEN_ID_PLUSOP         21
#define TOKEN_ID_MINUSOP        22
#define TOKEN_ID_MULTOP         23
#define TOKEN_ID_DIVOP          24
#define TOKEN_ID_NOTOP          25
#define TOKEN_ID_LESSOP         26
#define TOKEN_ID_LESSEQUALOP    27
#define TOKEN_ID_GREATEROP      28
#define TOKEN_ID_GREATEREQUALOP 29
#define TOKEN_ID_EQUALOP        30
#define TOKEN_ID_NOTEQUALOP     31
#define TOKEN_ID_COMMENT        32
#define TOKEN_ID_SCANEOF        33
#define TOKEN_ID_NEWLINE        34


#define FMT_LINE_FEED "[%d] %s\n"
#define FMT_STMT_FEED "Statement: %s\n"
#define FMT_TOKEN_LINE "Token ID: %2d \t Token Type: %-10s \t Actual Token: %s"
#define FMT_LEXICAL_ERRORS_TOTAL "Total Lexical Errors: %d"

int lineNumber, lexicalErrors;
char buffer[1000];

struct token {
    int id;
    char *name;
};

struct token errorToken, idToken, intToken, scaneofToken, newlineToken;
struct token tokenList[35];

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

struct token create_token(int id, char* name);


struct token get_next_token();

struct token peek_next_token();

struct token read_token();

void print_line_to_listings();

#endif

/* EOF */