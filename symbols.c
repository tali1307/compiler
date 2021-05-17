
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

typedef struct
{
    int scope_level;             // class, subroutine, loop
    char scope_name[MAX_STRING]; // so we know which subroutine, loop we're in etc.
    char name[MAX_STRING];
    char type[MAX_STRING]; // int, char, float, struct etc.
    char kind[MAX_STRING]; // static, field, argument, var etc.
} table;

// let this store all the symbols tables
// each scope == 1 symbol table
table symbols_table[TABLE_SIZE];
int symbols_table_index = 0;
char className[MAX_STRING];

char *MathsFunctions[] = {"multiply", "divide", "min", "max", "sqrt", "abs"};
char *StringMethods[] = {"new", "dispose", "length", "charAt", "setCharAt", "appendChar", "eraseLastChar", "intValue", "setInt", "backSpace", "doubleQuote", "newLine"};
char *ArrayMethods[] = {"new", "dispose"};
char *OutputFunctions[] = {"moveCursor", "printChar", "printString", "printInt", "println", "backspace"};
char *ScreenFunctions[] = {"clearScreen", "setColor", "drawPixel", "drawLine", "drawRectangle", "drawCircle"};
char *KeyboardFunctions[] = {"keyPressed", "readChar", "readLine", "readInt"};
char *MemoryFunctions[] = {"peek", "poke", "alloc", "deAlloc"};
char *SysFunctions[] = {"halt", "error", "wait"};

void initSymbolsTable()
{
    symbols_table_index = 0;
}

bool isRedeclar(table t)
{
    for (int i = 0; i < symbols_table_index; i++)
    {
        if (strcmp(symbols_table[i].name, t.name) == 0 && strcmp(symbols_table[i].scope_name, t.scope_name) == 0 && strcmp(t.kind, "argument") != 0)
            return true;
    }
    return false;
}

void PrintTable()
{
    printf("\n\tindex\t\tscope_name\t\tname\t\ttype\t\tkind\t\tscope_level\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < symbols_table_index; i++)
    {
        if (symbols_table[i].scope_level == 0)
            printf("\t%d\t\t-----\n", i);
        else
        {
            printf("\t%d\t\t%s\t\t%s\t\t%s\t\t%s\t\t%d\n", i, symbols_table[i].scope_name, symbols_table[i].name, symbols_table[i].type, symbols_table[i].kind, symbols_table[i].scope_level);
        }
    }
}

ParserInfo argsList(ParserInfo pi, char *scope_name)
{
    table t;
    while (strcmp(pi.tk.lx, ")") != 0)
    {
        pi = type();
        if (strcmp(pi.tk.lx, ",") != 0)
        {
            if (pi.er == none)
            {
                strcpy(t.type, pi.tk.lx);
                pi.tk = GetNextToken();
                strcpy(t.name, pi.tk.lx);
                strcpy(t.kind, "argument");
                t.scope_level = SUBROUTINE_SCOPE;
                strcpy(t.scope_name, scope_name);

                if (isRedeclar(t) == true)
                {
                    pi.er = redecIdentifier;
                    return pi;
                }
                else
                    symbols_table[symbols_table_index++] = t;
            }
        }
    }
}
ParserInfo identifierList(ParserInfo pi, char *scope_name)
{
    table t;
    char *kind = malloc(MAX_STRING * sizeof(char));
    char *type = malloc(MAX_STRING * sizeof(char));
    strcpy(t.kind, pi.tk.lx);
    strcpy(kind, pi.tk.lx);
    pi.tk = GetNextToken();
    strcpy(t.type, pi.tk.lx);
    strcpy(type, pi.tk.lx);
    pi.tk = GetNextToken();
    strcpy(t.name, pi.tk.lx);
    strcpy(t.scope_name, scope_name);
    if (strcmp(kind, "var") == 0)
        t.scope_level = SUBROUTINE_SCOPE;
    else
        t.scope_level = CLASS_SCOPE;
    if (isRedeclar(t) == true)
    {
        pi.er = redecIdentifier;
        return pi;
    }
    else
        symbols_table[symbols_table_index++] = t;
    if (strcmp(GetNextToken().lx, ",") == 0)
    {
        pi.tk = PeekNextToken();
        while (strcmp(pi.tk.lx, ";") != 0)
        {
            pi.tk = GetNextToken();
            if (strcmp(pi.tk.lx, ",") != 0 && strcmp(pi.tk.lx, ";") != 0)
            {
                strcpy(t.name, pi.tk.lx);
                strcpy(t.type, type);
                strcpy(t.kind, kind);
                strcpy(t.scope_name, scope_name);
                if (strcmp(kind, "var") == 0)
                    t.scope_level = SUBROUTINE_SCOPE;
                else
                    t.scope_level = CLASS_SCOPE;
                if (isRedeclar(t) == true)
                {
                    pi.er = redecIdentifier;
                    return pi;
                }
                else
                    symbols_table[symbols_table_index++] = t;
            }
        }
    }
    free(kind);
    free(type);
}

ParserInfo classTable()
{
    ParserInfo pi;
    pi.er = none;
    pi.tk = GetNextToken();
    strcpy(className, pi.tk.lx);
    pi.tk = GetNextToken();
    pi.tk = PeekNextToken();
    int fieldsDetected = 0;
    while (strcmp(pi.tk.lx, "constructor") != 0 && strcmp(pi.tk.lx, "method") != 0 && strcmp(pi.tk.lx, "function") != 0 && strcmp(pi.tk.lx, "}") != 0)
    {
        pi.tk = GetNextToken();
        if (strcmp(pi.tk.lx, "static") == 0 || strcmp(pi.tk.lx, "field") == 0)
        {
            fieldsDetected = 1;
            pi = identifierList(pi, className);
            if (pi.er != none)
                return pi;
        }
    }
    if (fieldsDetected == 0)
    {
        GetNextToken();
    }
    return pi;
}

ParserInfo methodTable()
{
    ParserInfo pi;
    pi.er = none;
    table t;
    char *scope_name = malloc(MAX_STRING * sizeof(char));

    strcpy(t.kind, "method");
    strcpy(t.type, GetNextToken().lx);
    strcpy(t.name, GetNextToken().lx);
    t.scope_level = CLASS_SCOPE;
    strcpy(t.scope_name, className);

    strcpy(scope_name, t.name);

    if (isRedeclar(t) == true)
    {
        pi.er = redecIdentifier;
        return pi;
    }
    else
        symbols_table[symbols_table_index++] = t;

    strcpy(t.name, "this");
    strcpy(t.kind, "argument");
    strcpy(t.type, className);
    t.scope_level = SUBROUTINE_SCOPE;
    strcpy(t.scope_name, scope_name);

    if (isRedeclar(t) == true)
    {
        pi.er = redecIdentifier;
        return pi;
    }
    else
        symbols_table[symbols_table_index++] = t;

    while (strcmp(pi.tk.lx, "constructor") != 0 && strcmp(pi.tk.lx, "function") != 0 && strcmp(pi.tk.lx, "while") != 0 && strcmp(pi.tk.lx, "do") != 0 && strcmp(pi.tk.lx, "if") != 0 && strcmp(pi.tk.lx, "let") != 0)
    {
        pi.tk = GetNextToken();
        if (strcmp(pi.tk.lx, "(") == 0)
            pi = argsList(pi, scope_name);
        if (strcmp(pi.tk.lx, "var") == 0)
            pi = identifierList(pi, scope_name);
        if (strcmp(pi.tk.lx, "method") == 0)
            break;
    }
    free(scope_name);
    return pi;
}

ParserInfo funcTable(char *func_or_cons)
{
    ParserInfo pi;
    pi.er = none;
    table t;
    char *scope_name = malloc(MAX_STRING * sizeof(char));

    strcpy(t.kind, func_or_cons);
    pi.tk = GetNextToken();
    strcpy(t.type, pi.tk.lx);
    pi.tk = GetNextToken();
    strcpy(t.name, pi.tk.lx);
    t.scope_level = CLASS_SCOPE;
    strcpy(t.scope_name, className);

    strcpy(scope_name, t.name);

    if (isRedeclar(t) == true)
    {
        pi.er = redecIdentifier;
        return pi;
    }
    else
        symbols_table[symbols_table_index++] = t;

    while (strcmp(pi.tk.lx, "method") != 0 && strcmp(pi.tk.lx, "while") != 0 && strcmp(pi.tk.lx, "do") != 0 && strcmp(pi.tk.lx, "if") != 0 && strcmp(pi.tk.lx, "let") != 0)
    {
        pi.tk = GetNextToken();
        if (strcmp(pi.tk.lx, "(") == 0)
            pi = argsList(pi, scope_name);
        if (strcmp(pi.tk.lx, "var") == 0)
            pi = identifierList(pi, scope_name);
        if (strcmp(pi.tk.lx, "constructor") == 0 || strcmp(pi.tk.lx, "function") == 0)
            break;
        if (pi.er == redecIdentifier || pi.er == undecIdentifier)
            return pi;
    }
    free(scope_name);
    return pi;
}

ParserInfo AddSymbols()
{
    ParserInfo pi;
    pi.er = none;
    int mathFlag = 0, sysFlag = 0, keyFlag = 0, stringFlag = 0, outFlag = 0, memoryFlag = 0, arrayFlag = 0, screenFlag = 0;
    pi.tk = GetNextToken();
    while (pi.tk.tp != EOFile)
    {
        if (strcmp(pi.tk.lx, "class") == 0)
        {
            pi = classTable();
            if (pi.er != none)
                return pi;
        }
        else if (strcmp(pi.tk.lx, "method") == 0)
        {
            pi = methodTable();
            if (pi.er != none)
            {
                if (pi.er != redecIdentifier && pi.er != undecIdentifier)
                    pi.er = none;
                else
                    return pi;
            }
        }
        else if (strcmp(pi.tk.lx, "function") == 0 || strcmp(pi.tk.lx, "constructor") == 0)
        {
            pi = funcTable(pi.tk.lx);
            if (pi.er != none)
            {
                if (pi.er != redecIdentifier && pi.er != undecIdentifier)
                    pi.er = none;
                else
                    return pi;
            }
        }
        else if (strcmp(pi.tk.lx, "Math") == 0 && mathFlag == 0)
        {
            table t;
            mathFlag = -1;
            for (int i = 0; i < 6; i++)
            {
                strcpy(t.kind, "function");
                strcpy(t.name, MathsFunctions[i]);
                strcpy(t.type, "int");
                strcpy(t.scope_name, "Math");
                t.scope_level = CLASS_SCOPE;
                symbols_table[symbols_table_index++] = t;
            }
            pi.tk = GetNextToken();
        }
        else if (strcmp(pi.tk.lx, "Memory") == 0 && memoryFlag == 0)
        {
            table t;
            memoryFlag = -1;
            for (int i = 0; i < 4; i++)
            {
                strcpy(t.kind, "function");
                strcpy(t.name, MemoryFunctions[i]);
                strcpy(t.type, "class");
                strcpy(t.scope_name, "Memory");
                t.scope_level = CLASS_SCOPE;
                symbols_table[symbols_table_index++] = t;
            }
            pi.tk = GetNextToken();
        }
        else if (strcmp(pi.tk.lx, "String") == 0 && stringFlag == 0)
        {
            table t;
            stringFlag = -1;
            for (int i = 0; i < 12; i++)
            {
                strcpy(t.kind, "method");
                strcpy(t.name, StringMethods[i]);
                strcpy(t.type, "class");
                strcpy(t.scope_name, "String");
                t.scope_level = CLASS_SCOPE;
                symbols_table[symbols_table_index++] = t;
            }
            pi.tk = GetNextToken();
        }
        else if (strcmp(pi.tk.lx, "Array") == 0 && arrayFlag == 0)
        {
            table t;
            arrayFlag = -1;
            for (int i = 0; i < 2; i++)
            {
                strcpy(t.kind, "method");
                strcpy(t.name, ArrayMethods[i]);
                strcpy(t.type, "class");
                strcpy(t.scope_name, "Array");
                t.scope_level = CLASS_SCOPE;
                symbols_table[symbols_table_index++] = t;
            }
            pi.tk = GetNextToken();
        }
        else if (strcmp(pi.tk.lx, "Output") == 0 && outFlag == 0)
        {
            table t;
            outFlag = -1;
            for (int i = 0; i < 6; i++)
            {
                strcpy(t.kind, "function");
                strcpy(t.name, OutputFunctions[i]);
                strcpy(t.type, "void");
                strcpy(t.scope_name, "Output");
                t.scope_level = CLASS_SCOPE;
                symbols_table[symbols_table_index++] = t;
            }
            pi.tk = GetNextToken();
        }
        else if (strcmp(pi.tk.lx, "Screen") == 0 && screenFlag == 0)
        {
            table t;
            screenFlag = -1;
            for (int i = 0; i < 6; i++)
            {
                strcpy(t.kind, "function");
                strcpy(t.name, ScreenFunctions[i]);
                strcpy(t.type, "void");
                strcpy(t.scope_name, "Screen");
                t.scope_level = CLASS_SCOPE;
                symbols_table[symbols_table_index++] = t;
            }
            pi.tk = GetNextToken();
        }
        else if (strcmp(pi.tk.lx, "Keyboard") == 0 && keyFlag == 0)
        {
            table t;
            keyFlag = -1;
            for (int i = 0; i < 4; i++)
            {
                strcpy(t.kind, "function");
                strcpy(t.name, KeyboardFunctions[i]);
                strcpy(t.type, "class");
                strcpy(t.scope_name, "Keyboard");
                t.scope_level = CLASS_SCOPE;
                symbols_table[symbols_table_index++] = t;
            }
            pi.tk = GetNextToken();
        }
        else if (strcmp(pi.tk.lx, "Sys") == 0 && sysFlag == 0)
        {
            table t;
            sysFlag = -1;
            for (int i = 0; i < 3; i++)
            {
                strcpy(t.kind, "function");
                strcpy(t.name, SysFunctions[i]);
                strcpy(t.type, "void");
                strcpy(t.scope_name, "Sys");
                t.scope_level = CLASS_SCOPE;
                symbols_table[symbols_table_index++] = t;
            }
            pi.tk = GetNextToken();
        }
        else
            pi.tk = GetNextToken();
    }
    return pi;
}

ParserInfo CheckUndeclared()
{
    ParserInfo pi;
    pi.er = none;
    pi.tk = GetNextToken();
    while (pi.tk.tp != EOFile)
    {
        if (strcmp(pi.tk.lx, "let") == 0 || strcmp(pi.tk.lx, "do") == 0)
        {
            while (strcmp(pi.tk.lx, ";") != 0)
            {
                int flag = 0;
                pi.tk = GetNextToken();
                if (pi.tk.tp == ID)
                {
                    for (int i = 0; i < symbols_table_index; i++)
                    {
                        if (strcmp(pi.tk.lx, symbols_table[i].name) == 0 || strcmp(pi.tk.lx, symbols_table[i].scope_name) == 0)
                        {
                            flag = -1;
                            break;
                        }
                    }
                    if (flag == 0)
                    {
                        pi.er = undecIdentifier;
                        return pi;
                    }
                }
            }
        }
        else if (strcmp(pi.tk.lx, "var") == 0 || strcmp(pi.tk.lx, "field") == 0 || strcmp(pi.tk.lx, "static") == 0)
        {
            int flag = 0;
            pi.tk = GetNextToken();
            if (pi.tk.tp == ID)
            {
                for (int i = 0; i < symbols_table_index; i++)
                {
                    if (strcmp(pi.tk.lx, symbols_table[i].scope_name) == 0)
                    {
                        flag = -1;
                        break;
                    }
                }
                if (flag == 0)
                {
                    pi.er = undecIdentifier;
                    return pi;
                }
            }
        }
        else
            pi.tk = GetNextToken();
    }
    return pi;
}
