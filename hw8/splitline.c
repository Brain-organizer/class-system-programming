/*
 * command reading and parsing functions for smsh
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smsh.h"

int nonblocking = 0;

int is_nonblocking(){
    if(nonblocking)
        return 1;
    return 0;
}

// read next command line for fp
char* next_cmd(char *prompt, FILE* fp){
// return: dynamically allocated string holding command line
    char* buf;
    int bufspace = 0; // total buf size
    int pos = 0; // current position
    int c; // input char

    printf("%s", prompt); // prompt sign
    while ((c = getc(fp)) != EOF){
        // need space?
        if(pos+1 >= bufspace){  // 1 for \0
            if(bufspace == 0){  // y: 1st time
                buf = emalloc(BUFSIZ); // 8K in stdio.h
            }
            else // or expand buffer using realloc
                buf = erealloc(buf, bufspace+BUFSIZ);
            bufspace += BUFSIZ; // update size
        }
        // end of command?
        if(c == '\n') break;
        // if not, then add to buffer
        buf[pos++] = c;
    }
    if(c == EOF && pos == 0)
        return NULL;
    buf[pos] = '\0';
    
    return buf;
}

//parse a line into an array of strings
#define is_delim(x) ((x) == ' ' || (x) == '\t')
char** splitline(char *line){
// purpose: split a line into array of white-space separated token
// return: a NULL-terminated array of pointers to copies of the token 
//         or NULL if no tokens on the line
// action: traverse array, locate strings, make copies
// note: strtok() could work, but we may want to add quotes later
    char *newstr();
    char **args;
    int spots = 0; // max size for num of token
    int bufspace = 0; //buffer size
    int argnum = 0; // argment number that currently found
    char *cp = line; // copy of input line
    char *start;
    int len;

    nonblocking = 0;

    if(line == NULL)
        return NULL;
    args = emalloc(BUFSIZ);
    bufspace = BUFSIZ;
    spots = BUFSIZ / sizeof(char*);

    while(*cp != '\0'){
        while(is_delim(*cp)) // skip leading spaces
            cp++;
        if(*cp == '\0') // quit at end of string
            break;
        // make sure the array has room (+1 for NULL)
        if(argnum+1 >= spots){
            args = erealloc(args, bufspace+BUFSIZ);
            bufspace += BUFSIZ;
            spots += (BUFSIZ/sizeof(char*));
        }
        // mark start, then find end of word
        start = cp;
        len = 1;
        while(*++cp != '\0' && !(is_delim(*cp)))
            len++;
        args[argnum++] = newstr(start, len);
    }
    if(strcmp(args[argnum-1],"&") == 0){
        nonblocking = 1;
        args[argnum-1] = NULL;
    }
    else if(args[argnum-1][len-1] == '&'){
        nonblocking = 1;
        args[argnum-1][len-1] = '\0';
    }
    args[argnum] = NULL;

    return args;
}

// constructor for string
// a string, never NULL
char* newstr(char *s, int l){
    char *rv = emalloc(l+1);

    strncpy(rv, s, l);
    rv[l] = '\0';

    return rv;
}

// free the list returned by splitline
// free all strings in list and then
// free the list
void freelist(char **list){
    char **cp = list;
    while (*cp){
        free(*cp++);
    }
    free(list);
}

// extended malloc
void* emalloc(size_t n){
    void *rv = (void*)malloc(n);
    if (rv == NULL){
        fatal("out of memory", "", 1);
    }
    return rv;
}

void* erealloc(void *p, size_t n){
    void* rv = realloc(p, n);
    if (rv == NULL){
        fatal("realloc() failed", "", 1);
    }
    return rv;
}