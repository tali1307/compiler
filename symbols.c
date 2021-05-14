
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
table *symbols_table[TABLE_SIZE];
int scope_count = 0;
int symbols_table_index = 0;
char *className;

bool initsymbolsTable()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        symbols_table[i] = NULL;
    }
}

bool symbolsTableLookup(table *t)
{
    ;
}

void PrintTable()
{
    for (int i = 1; i <= TABLE_SIZE; i++)
    {
        if (symbols_table[i] == NULL)
            printf("\t%d\t-----\n", i);
        else
            printf("\t%d\t%s\n", i, symbols_table[i]->name);
    }
}

int main()
{
    initsymbolsTable();
    PrintTable();
    return 1;
}