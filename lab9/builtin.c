/*
 * - contains the switch and the
 * - functions for builtin cmds
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "smsh.h"
#include "varlib.h"

int assign(char *); // for assignment cmd
int okname(char *); // check legal name
int var2val(char** args); // change $var to val for all argument(token)

// purpose: run a builtin command
// returns: 1 if args[0] is built-in, 0 if not
// details: change from $var to val for all argument(token)
//          then test args[0] against all known built-ins.
//          Call functions.
//          resultp points result of builtin_command
int builtin_command(char** args, int* resultp){
    int rv = 0;
    
    *resultp = var2val(args);
    
    if(strcmp(args[0], "set") == 0){
        VLlist(); // we call 'set'.
        *resultp = 0;
        rv = 1;
    }else if (strchr(args[0], '=') != NULL){
        *resultp = assign(args[0]);
        if(*resultp != -1) rv = 1;
    }else if (strcmp(args[0], "export") == 0){
        if(args[1] != NULL && okname(args[1])){
            *resultp = VLexport(args[1]); // make it global
        }
        else *resultp = 1;
        rv = 1;
    }
    return rv;
}

// purpose: execute name=val AND ensure that name is legal
// returns: -1 for illegal lval, or result of VLstore
// warning: modifies the string, but restores it to normal
int assign(char* str){
    char* cp;
    int rv;
    // strchr: returns a pointer to the first occurrence of '='
    cp = strchr(str, '=');
    *cp = '\0'; // modify
    rv = (okname(str) ? VLstore(str, cp+1) : -1); // check str is really name, and store the value
    *cp = '='; // restore
    
    return rv;
}

//purpose: determmines if a string is a legal var name
// returns: 0 for no, 1 for yes
int okname(char* str){
    char* cp;
    for(cp = str; *cp; cp++){
        //check var name below
        if ( (isdigit(*cp) && cp == str) || !( isalnum(*cp) || *cp=='_' ) )
            return 0;
    }
    return (cp != str); // empty string이면 0 반환됨.
}

int var2val(char **args){
    int rv = 0;
    char *cp, *val;
    int new_len;
    for( ;*args; args++){
        cp = strchr(*args,'$');
        if(cp == NULL)
            continue;
        *cp = '\0';

        val = VLlookup(cp+1);
        if(val == NULL){
            val = emalloc(1);
            val[0] = '\0';
        }

        new_len = strlen(*args) + strlen(val) + 1;
        *args = erealloc(*args, new_len);
        strcat(*args, val);
    }
}