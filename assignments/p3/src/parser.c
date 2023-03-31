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

    curToken = get_next_token();

    while( curToken.id != scaneofToken.id)
    {
        log_info("Token: %s", curToken.name);
        write_to_file(outputFilePtr, FMT_TOKEN_LINE, curToken.id, curToken.name, "TOKEN");
        curToken = read_token();
    }

    return 1;
}


// EOF