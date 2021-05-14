
/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
The Symbol Tables Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Mitali Sen
StudentID: 201262647
Email: sc19ms2@leeds.ac.uk
Date Work Commenced: 12 May 2021
*************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols.h"
#include "lexer.h"
#include "parser.h"

#define TABLE_SIZE 500
#define MAX_STRING 50
#define CLASS_SCOPE 1
#define SUBROUTINE_SCOPE 2 // method, constructor, function
#define STATEMENT_SCOPE 3  // do, while, if, else

typedef struct
{
    int scope_level;  // class, subroutine, loop
    int scope_number; // uses scope_count, so we know which subroutine, loop we're in etc.
    char name[MAX_STRING];
    char type[MAX_STRING]; // int, char, float, struct etc.
    char kind[MAX_STRING]; // static, field, argument, var etc.
} table;

// let this store all the symbols tables
// each scope == 1 symbol table
table *hash_table[TABLE_SIZE];
int scope_count = 0;
int hash_table_index = 0;

unsigned int hash(table *t)
{
    // define a way to hash through our scope

    /* 
        for undeclared:
        - hashLookup using the var name and its scope 
        - if something is declared in a higher scope it's not undeclared

        for redeclared:
        - take care of it in hashInsert using the var name and its scope; if already in table its redeclared
        - its ok to redelcare something if it's in a different scope

        - need to use scope_number for both of the above to differentiate between different scopes of same level
        - ex: 2 different methods which both have var int x - no error

        how to maintain a hierarchy of the scopes? Number them??
    */
    int index = 0;
    int ascii[MAX_STRING];
    for (int i = 0; i < strlen(t->name) - 1; i++)
    {
        ascii[i] = t->name[i];
    }
    ascii[strlen(t->name)] = t->scope_number;
    return index;
}

bool initHashTable()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        hash_table[i] = NULL;
    }
}

bool hashTableInsert(table *t)
{
    ;
}

bool hashTableLookup(table *t)
{
    if (t != NULL && hash(t) == true)
        return true;
    else
        return false;
}

void PrintTable()
{
    for (int i = 1; i <= TABLE_SIZE; i++)
    {
        if (hash_table[i] == NULL)
            printf("\t%d\t-----\n", i);
        else
            printf("\t%d\t%s\n", i, hash_table[i]->name);
    }
}

void CreateTables()
{
    ParserInfo pi;
    pi.er = none;
    pi.tk = GetNextToken();
    if (strcmp(pi.tk.lx, "class") == 0)
    {
        while (hash_table_index < TABLE_SIZE)
        {
            pi.tk = GetNextToken();
            if (strcmp(pi.tk.lx, "static") == 0 || strcmp(pi.tk.lx, "field") == 0)
            {
                strcpy(hash_table[hash_table_index]->kind, pi.tk.lx);
                pi.tk = GetNextToken();
                strcpy(hash_table[hash_table_index]->type, pi.tk.lx);
                pi.tk = GetNextToken();
                strcpy(hash_table[hash_table_index]->name, pi.tk.lx);
                hash_table[hash_table_index]->scope_level = CLASS_SCOPE;
                hash_table[hash_table_index]->scope_number = ++scope_count;
            }
            else if (strcmp(pi.tk.lx, "constructor") == 0 || strcmp(pi.tk.lx, "method") == 0 || strcmp(pi.tk.lx, "function") == 0)
                break;
            hash_table_index++;
        }
    }
    else if (strcmp(pi.tk.lx, "method") == 0)
    {
        table *t = NULL;
        strcpy(t->name, "this");
        strcpy(t->kind, "argument");
        pi.tk = GetNextToken();
        strcpy(t->type, pi.tk.lx);
        t->scope_level = SUBROUTINE_SCOPE;
        t->scope_number = ++scope_count;

        if (hashTableLookup(t) == true)
        {
            pi.er = redecIdentifier;
            return pi;
        }
        else
        {
            hash_table[hash_table_index] = t;
            hash_table_index++;
        }

        while (hash_table_index < TABLE_SIZE)
        {

            pi.tk = GetNextToken();

            strcpy(hash_table[hash_table_index]->kind, pi.tk.lx);
            pi.tk = GetNextToken();
            strcpy(hash_table[hash_table_index]->type, pi.tk.lx);
            pi.tk = GetNextToken();
            strcpy(hash_table[hash_table_index]->name, pi.tk.lx);
            hash_table[hash_table_index]->scope_level = SUBROUTINE_SCOPE;
            hash_table[hash_table_index]->scope_number = ++scope_count;

            if (strcmp(pi.tk.lx, "constructor") == 0 || strcmp(pi.tk.lx, "function") == 0 || strcmp(pi.tk.lx, "while") == 0 || strcmp(pi.tk.lx, "do") == 0 || strcmp(pi.tk.lx, "if") == 0)
                break;
            hash_table_index++;
        }
    }
}

int main()
{
    initHashTable();
    PrintTable();
    return 1;
}