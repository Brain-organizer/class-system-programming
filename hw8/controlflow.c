/*
 * - "if" processing is done with two state variables
 * - if_state and if_result
 */

#include <stdio.h>
#include <string.h>
#include "smsh.h"

enum states {NEUTRAL, WANT_THEN, THEN_BLOCK, ELSE_BLOCK};
enum results {SUCCESS, FAIL};
static int if_state = NEUTRAL;
static int if_result = SUCCESS;
static int last_stat = 0;
int syn_err(char*);

// determine the shell should execute a command
// returns: 1 for yes, 0 for no
// details: if in THEN_BLOCK and if_result was SUCCESS then yes
//          if in THEN_BLOCK and if_result was FAIL then no
//          if in WANT_THEN and syntax error
//          if in ELSE_BLOCK and if_result was SUCCESS then no
//          if in ELSE_BLOCK and if_result was FAIL then yes
int ok_to_execute(){
    int rv = 1;
    if( if_state == WANT_THEN ){
        syn_err("then expected");
        rv = 0;
    }
    else if (if_state == THEN_BLOCK && if_result == SUCCESS)
        rv = 1;
    else if (if_state == THEN_BLOCK && if_result == FAIL)
        rv = 0;
    else if (if_state == ELSE_BLOCK && if_result == FAIL)
        rv = 1;
    else if (if_state == ELSE_BLOCK && if_result == SUCCESS)
        rv = 0;
    
    return rv;
}

// boolean to report if the command is a shell control command
// return 0 or 1
int is_control_command(char* s){
    return (strcmp(s, "if") == 0 || strcmp(s,"then")==0 || strcmp(s,"else") == 0 || strcmp(s,"fi") == 0);
}

// process "if", "then", "else", "fi" - change state or detect error
// returns 0 if ok -1 for syntax error
int do_control_command(char** args){
    char* cmd = args[0];
    int rv = -1;
    
    if(strcmp(cmd, "if") == 0){
        if(if_state != NEUTRAL){ // expected neutral (when we see if)
            rv = syn_err("if unexpected");
        }
        else{
            last_stat = process(args+1);
            if_result = (last_stat == 0 ? SUCCESS : FAIL);
            if_state = WANT_THEN; //update the (next) state
            rv = 0;
        }
    }
    else if(strcmp(cmd, "then")==0){
        if(if_state != WANT_THEN){
            rv = syn_err("then unexpected");
        }else{
            if_state = THEN_BLOCK; //update the (next) state;
            rv = 0;
        }
    }
    else if(strcmp(cmd, "else") == 0){
        if(if_state != THEN_BLOCK){
            rv = syn_err("else unexpected");
        }else{
            if_state = ELSE_BLOCK; //update the (next) state;
            rv = 0;
        }
    }
    else if(strcmp(cmd, "fi")==0){ // expect then_block or else_block
        if(if_state != THEN_BLOCK && if_state != ELSE_BLOCK){
            rv = syn_err("fi unexpected");
        }else{
            if_state = NEUTRAL; // update the (next) state
            rv = 0;
        }
    }
    else   
        fatal("internal error processing:", cmd, 2);
    
    return rv;
}

// handles syntax errors in control structures
// resets state to NEUTRAL
// returns: -1 in interactive mode. Should call fatal in scripts
int syn_err(char* msg){
    if_state = NEUTRAL; // back to neutral
    fprintf(stderr, "syntax error: %s\n", msg);
    return -1;
}
