
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

#define TABLE_SIZE 50
#define MAX_STRING 50
#define CLASS_SCOPE 1
#define SUBROUTINE_SCOPE 2 // method, constructor, function
#define STATEMENT_SCOPE 3  // while, if, else

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

void initsymbolsTable()
{
    className = malloc(MAX_STRING * sizeof(char));
    *symbols_table = malloc(TABLE_SIZE * sizeof(table));
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        symbols_table[i] = malloc(sizeof(table));
        symbols_table[i]->scope_number = 0;
        symbols_table[i]->scope_level = 0;
        *symbols_table[i]->name = malloc(MAX_STRING * sizeof(char));
        *symbols_table[i]->kind = malloc(MAX_STRING * sizeof(char));
        *symbols_table[i]->type = malloc(MAX_STRING * sizeof(char));
        symbols_table[i] = NULL;
    }
}

bool isRedeclar(table *t)
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
        {
            printf("what about here?");
            printf("\t%d\t%s\t%s\t%s\n", i, symbols_table[i]->name, symbols_table[i]->type, symbols_table[i]->kind);
        }
    }
}

void classTable()
{
    ParserInfo pi;
    pi.er = none;
    symbols_table[symbols_table_index]->scope_number = ++scope_count;
    pi.tk = GetNextToken();
    strcpy(className, pi.tk.lx);
    while (symbols_table_index < TABLE_SIZE)
    {
        pi.tk = GetNextToken();
        if (strcmp(pi.tk.lx, "static") == 0 || strcmp(pi.tk.lx, "field") == 0)
        {
            char *kind, *type;
            strcpy(symbols_table[symbols_table_index]->kind, pi.tk.lx);
            strcpy(kind, pi.tk.lx);
            pi.tk = GetNextToken();
            strcpy(symbols_table[symbols_table_index]->type, pi.tk.lx);
            strcpy(type, pi.tk.lx);
            pi.tk = GetNextToken();
            strcpy(symbols_table[symbols_table_index]->name, pi.tk.lx);
            symbols_table[symbols_table_index]->scope_level = CLASS_SCOPE;
            symbols_table[symbols_table_index]->scope_number = scope_count;
            if (strcmp(GetNextToken().lx, ",") == 0)
            {
                while (strcmp(PeekNextToken().lx, ";") != 0)
                {
                    pi.tk = GetNextToken();
                    strcpy(symbols_table[symbols_table_index]->name, pi.tk.lx);
                    strcpy(symbols_table[symbols_table_index]->type, type);
                    strcpy(symbols_table[symbols_table_index]->kind, kind);
                    symbols_table[symbols_table_index]->scope_level = CLASS_SCOPE;
                    symbols_table[symbols_table_index]->scope_number = scope_count;
                }
            }
        }
        else if (strcmp(pi.tk.lx, "constructor") == 0 || strcmp(pi.tk.lx, "method") == 0 || strcmp(pi.tk.lx, "function") == 0)
            break;
        symbols_table_index++;
    }
}

void cleanup()
{
    free(className);
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        free(symbols_table[i]->name);
        free(symbols_table[i]->kind);
        free(symbols_table[i]->type);
        free(symbols_table[i]);
    }
}

ParserInfo CheckSymbols()
{
    initsymbolsTable();
    ParserInfo pi;
    pi.er = none;
    pi.tk = GetNextToken();
    printf("token here %s", pi.tk.lx);
    if (strcmp(pi.tk.lx, "class") == 0)
    {
        classTable();
    }
    else if (strcmp(pi.tk.lx, "method") == 0)
    {
        ;
    }
    cleanup();
    return pi;
}
