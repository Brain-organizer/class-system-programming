/*
 * command processing layer
 * The process (char **arglist) function is called by the main loop
 * It sits in front the execute() function. This layer handles
 * two main classes of processing:
 *      a) built-in functions (e.g. exit(), set, =, read, ..)
 *      b) control structures (e.g. if, while, for)
 */

#include <stdio.h>
#include "smsh.h"

// is a boolean-balued function
// that tells process if the command is
// part of the shell programming lang. or
// if the command is something to execute
int is_control_command(char*);

// handles the keywords, if, then, else, and fi.
// (Each word is the boundary between two states.)
// updates the state variables and
// performs any appropriate operations.
int do_control_command(char**);

// checks the current state and
// the result of the condition command
// and returns a boolean value to indicate
// if the current command should be executed
int ok_to_execute();

// change from $var to val for all argument(token)
// and if args[0] is built-in command, execute the command
// returns 1 if args[0] is built-in, 0 if not
int builtin_command(char** args, int* resultp);

/*
 * purpose: process user command
 * returns: result of processing command
 * details: if a built-in then call appropriate function,
 * if not execute()
 * errors: arise from subroutines, handled there
 */
int process (char** args){
    int rv = 0;

    if (args[0] == NULL) 
        rv = 0;
    else if (is_control_command(args[0]))
        rv = do_control_command(args);
    else if (!builtin_command(args, &rv) && ok_to_execute())
        rv = execute(args);
    
    return rv;        
}