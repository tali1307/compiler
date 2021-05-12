/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
Lexer Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Mitali Sen
Student ID - 201262647
Email: sc19ms2@leeds.ac.uk
Date Work Commenced: 19/02/2021
*************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"


// YOU CAN ADD YOUR OWN FUNCTIONS, DECLARATIONS AND VARIABLES HERE

char* CharArray = 0;

int spaceCount = 0;

char* Reswords[] = {"class", "constructor", "method",	"function", "int", "boolean", "char", "void", "var", "static", "field", "let", "do", "if", "else", "while", "return", "true", "false", "null", "this"};

Token TokenArray[1000];

int nextToken = 0;

int NumChar = 0;
int NumTokens = 0;
int lineNum = 1;


Token CheckComment(int *k) {
  Token t;
  int flag = 0;
  *k = *k+2;
  while (flag != -1) {
    if (CharArray[*k] == 0) {

      t.ec = EofInCom;
      t.tp = ERR;
      strcpy(t.lx, "Error: unexpected eof in comment");
      flag = -1;
      return t;
    }
    else if (CharArray[*k] == '\n') {
      lineNum++;
      *k = *k + 1;
    }
    else if (CharArray[*k] == '*' && CharArray[*k+1] == '/') {
      *k = *k + 1;
      flag = -1;
    }
    else
      *k = *k + 1;
  }
}

Token CheckDigit(int *k) {
  Token t;
  t.lx[0] = CharArray[*k];
  *k = *k+1;
  int  i = 1;
  while(isdigit(CharArray[*k]) != 0) {
    t.lx[i] = CharArray[*k];
    i++;
    *k = *k+1;
  }
  t.lx[i] = '\0';
  t.tp = INT;
  return t;
}

Token CheckSingle(int *k) {
  Token t;
  int flag = 0;
  *k = *k + 2;
  while (flag != -1) {
    if (CharArray[*k] == 0) {
      t.ec = EofInCom;
      t.tp = ERR;
      strcpy(t.lx, "Error: unexpected eof in comment");
      flag = -1;
      return t;
    }
    else if (CharArray[*k] == '\n') {
      *k = *k-1;
      flag = -1;
    }
    else 
      *k = *k+1;
  }
}

Token CheckString(int *k) {
  Token t;
  *k = *k+1;
  int flag = 0;
  int m = 0;

  while (flag != -1) {
    if (CharArray[*k] == 0) {
      t.ec = EofInStr;
      t.tp = ERR;
      strcpy(t.lx, "Error: unexpected eof in string constant");
      flag = -1;
      return t;
    }
    else if (CharArray[*k] == '\n') {
      t.ec = NewLnInStr;
      t.tp = ERR;
      strcpy(t.lx, "Error: new line in string constant");
      flag = -1;
      return t;
    }
    else if (CharArray[*k] == '"') {
      t.tp = STRING;
      flag = -1;
    }
    else {
        t.lx[m] = CharArray[*k];
        m++;
        *k = *k + 1;
    }
  }
  t.lx[m] = '\0';
  *k = *k + 1;
  return t;
}

Token CheckResOrID(int *k) {
  Token t;
  t.lx[0] = CharArray[*k];
  *k = *k + 1;
  int  i = 1;
  int flag = 0;
  while(isdigit(CharArray[*k]) != 0 || isalpha(CharArray[*k]) != 0 || CharArray[*k] == '_') {
    t.lx[i] = CharArray[*k];
    i++;
    *k = *k+1;
  }
  t.lx[i] = '\0';
  for (int  j = 0; j < 21; j++) {
    if (strcmp(t.lx, Reswords[j]) == 0) {
      t.tp = RESWORD;
      flag = -1;
    }
  }
  if (flag != -1 && flag == 0)
    t.tp = ID;
  return t;
}

// need to add getting lexemes
Token CreateTokens(char* filename) {
  Token t; 
  char ch, ch2;
  int k = 0;
  int EOFreached = 0;
  while (CharArray && EOFreached == 0) {
    strcpy(t.fl, filename);
    ch = CharArray[k];
    ch2 = CharArray[k+1];
    if(ch == ' ' || ch == '\r' || ch == '\t'){
      k++;
      continue;
    } else if(ch == '\n') {
      lineNum++;
      k++;
      continue;
    } else 
    if (ch == '/' && ch2 == '/') {
      t = CheckSingle(&k);
      if(t.tp == ERR){
        EOFreached = 1;
        t.ln = lineNum;
        strcpy(t.fl, filename);
        TokenArray[NumTokens++] = t;
      }
      k++;
      continue;
    } else if (ch == '/' && ch2 == '*') {
      t = CheckComment(&k);
      if(t.tp == ERR){
        EOFreached = 1;
        t.ln = lineNum;
        strcpy(t.fl, filename);
        TokenArray[NumTokens++] = t;
      }
      k++;
      continue;
    } else  if (ch == '_' || isalpha(ch) != 0){
      t = CheckResOrID(&k);
      t.ln = lineNum;
      strcpy(t.fl, filename);
    } else if (ispunct(ch) != 0 && ch2 != '/' && ch2 != '*' && ch != '_' && ch != '"' && ch != '?') {
      t.tp = SYMBOL;
      t.ln = lineNum;
      strcpy(t.fl, filename);
      t.lx[0] = ch;
      t.lx[1] = '\0';
      k++;
    } else if (isdigit(ch) != 0) {
      t = CheckDigit(&k);
      t.ln = lineNum;
      strcpy(t.fl, filename);
    } else if (ch == 0) {
      t.tp = EOFile;
      strcpy(t.fl, filename);
      strcpy(t.lx, "End of File");
      t.ln = lineNum;
      EOFreached = 1;
    } else if (ch == '"') {
      t = CheckString(&k);
      if (t.tp == ERR) {
        EOFreached = 1;
        t.ln = lineNum;
        strcpy(t.fl, filename);
        TokenArray[NumTokens++] = t;
      }
      t.ln = lineNum;
      strcpy(t.fl, filename);
    } else {
      t.tp = ERR;
      t.ec = IllSym;
      t.ln = lineNum;
      strcpy(t.lx, "Error: illegal symbol in source file");
      EOFreached = 1;
    }

    TokenArray[NumTokens++] = t;
  }
}

// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
int InitLexer (char* file_name)
{
  FILE *fp;
  long lSize;

  fp = fopen ( file_name , "rb" );
  if( !fp )
    return 0;

  fseek( fp , 0L , SEEK_END);
  lSize = ftell( fp );
  rewind( fp );

  CharArray = calloc( 1, lSize+1 );
  if( !CharArray ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

  fread( CharArray , lSize, 1 , fp);

  nextToken = 0;
  NumTokens = 0;
  lineNum = 1;

  fclose(fp);

  CreateTokens(file_name);
  return 1;
}

// Get the next token from the source file
Token GetNextToken ()
{ 
	Token t;
  t = TokenArray[nextToken++];
  return t;
}

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken ()
{
  Token t;
  t = TokenArray[nextToken];
  return t;
}

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer ()
{
  free(CharArray);
	return 0;
}

// do not remove the next line
#ifndef TEST
int main ()
{ 
  //printf("%d", NumTokens);
  /*char *filename = "Ball.jack";
  InitLexer(filename);
  Token token;
	token = GetNextToken ();
  int i = 0;
  while (i < NumTokens)
  {
    token = GetNextToken ();
  }*/

  InitLexer("NewLineInStr.jack");
  StopLexer();
	return 0;
}
// do not remove the next line
#endif
