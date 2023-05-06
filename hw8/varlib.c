/*
 *  a simple storage system to store name=val pairs
 *  with facility to mark items as part of the env
 */
#include <stdio.h>
#include <stdlib.h>
#include "varlib.h"
#include <string.h>

#define MAXVARS 200

struct var{
    char* str; // name=val string
    int global;// boolean
};
static struct var tab[MAXVARS]; // table

static char *new_string(char*, char*); 
static struct var* find_item(char*, int); 

// traverse list, if found, replace it
// else add at end
// a blank one is a free one
// return: 1 if trouble, 0 if ok
int VLstore(char* name, char* val){
    struct var *itemp; //spot to put
    char *s;
    int rv = 1;

    //find spot to put it and make new string
    if ((itemp=find_item(name,1)) != NULL && (s=new_string(name,val)) != NULL){
        if(itemp->str) // has a val?
            free(itemp->str); // remove old one to replace to new one
        itemp->str = s;
        rv = 0;
    }

    return rv; 
}

//return new string of form name=value or NULL on error
char* new_string(char* name, char* val){
    char* retval;
    retval = malloc(strlen(name)+strlen(val)+2);
    if(retval != NULL)
        sprintf(retval, "%s=%s", name, val);

    return retval;
}

//returns value of var or empty string if not there
char* VLlookup(char* name){
    struct var* itemp;
    if((itemp = find_item(name, 0)) != NULL){
        return itemp->str+1+strlen(name);
    }else
        return NULL;
}

// adds name to list of env vars
// marks a var for export, adds it if not there
// returns 1 for no, 0 for ok
int VLexport(char* name){
    struct var* itemp;
    int rv = 1;

    if((itemp = find_item(name,0)) != NULL){
        itemp->global = 1;
        rv = 0;
    }
    else if( VLstore(name, "") == 0)
        rv = VLexport(name);
    return rv;
}

// searches table for an item(element)
// returns ptr to struct or NULL if not found
// or if (first_blank) then ptr to (first blank one, name에 해당하는 item 둘 중 먼저 나온거)
static struct var* find_item(char* name, int first_blank){
    int i;
    int len = strlen(name);
    char *s;
    
    for(i = 0; i < MAXVARS && tab[i].str != NULL; i++){
        s = tab[i].str;
        if(strncmp(s, name, len) == 0 && s[len] == '='){
            return &tab[i];
        }
    }

    if(i < MAXVARS && first_blank){
        return &tab[i];
    }

    return NULL;
}

// performs the shell's set command
// lists the contents of the variable table, marking each
// exported variable with the symbol '*'
void VLlist(){
    int i;

    for(i = 0; i < MAXVARS && tab[i].str != NULL; i++){
        if(tab[i].global) 
            printf("    * %s\n", tab[i].str);
        else
            printf("      %s\n", tab[i].str);
    }
}

//copy from environ to table
// initialize the var table by loading array of strings
// return 1 for ok, 0 for not ok
int VLenviron2table(char* env[]){
    int i;
    char *newstring;
    
    for(i = 0; env[i] != NULL; i++){
        if(i == MAXVARS) return 0;
        
        newstring = malloc(1+strlen(env[i]));
        if(newstring == NULL) return 0;
        
        strcpy(newstring, env[i]);
        tab[i].str = newstring;
        tab[i].global = 1;
    }
    // we set NULL to each entry just in case
    while(i < MAXVARS){
        tab[i].str = NULL;
        tab[i++].global = 0;
    }
    return 1;
}

// copy from table to environ
// build an array of pointers for making a new environment
// note that free() is needed when done to avoid leaks
char** VLtable2environ(){
    int i, j; // indexes
    int n = 0; // counter
    char** envtab; // array of pointers
    
    // first, count # of global vars
    for(i = 0; i< MAXVARS && tab[i].str != NULL; i++){
        if(tab[i].global == 1) n++;
    } 
    // then, allocate space for those many variables
    envtab = (char**)malloc((n+1)*sizeof(char*));
    if(envtab == NULL) return NULL;
    // then, load the array with ptrs
    for(i=0,j=0; i< MAXVARS && tab[i].str != NULL; i++){
        if(tab[i].global == 1) envtab[j++] = tab[i].str;
    }
    envtab[j] = NULL;

    return envtab;
}