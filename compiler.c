/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
The Compiler Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Mitali Sen
StudentID: 201262647
Email: sc19ms2@leeds.ac.uk
Date Work Commenced: 12 May 2021
*************************************************************************/

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

int InitCompiler()
{
	return 1;
}

ParserInfo compile(char *dir_name)
{
	ParserInfo pi;
	pi.er = none;
	DIR *d;
	struct dirent *dir;
	d = opendir(dir_name);

	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
			{
				printf("the file name : %s\n", dir->d_name);
				InitParser(dir->d_name);
				pi = Parse(); // Parses
				StopParser();
				printf("ok whats the fucking error: %d at line %d, the lexeme: %s\n", pi.er, pi.tk.ln, pi.tk.lx);

				if (pi.er == none)
				{
					printf("are we even in here?");
					InitLexer(dir->d_name); // call again for the symbols table
					pi = CheckSymbols();
					StopLexer();
				}
			}
		}
		closedir(d);
	}
	return pi;
}

int StopCompiler()
{
	return 1;
}

#ifndef TEST_COMPILER
int main()
{
	ParserInfo pi = compile("Pong");
	PrintTable();
	return 1;
}
#endif
