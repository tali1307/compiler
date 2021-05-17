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
	initSymbolsTable();

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
				char dirPath[200];
				snprintf(dirPath, 200, "%s/%s", dir_name, dir->d_name);

				InitParser(dirPath);
				pi = Parse(); // Parses
				StopParser();
				if (pi.er == none)
				{
					InitParser(dirPath); // call again for the symbols table
					pi = AddSymbols();
					StopParser();
					if (pi.er == redecIdentifier)
						return pi;
				}
			}
		}
	}
	closedir(d);

	d = opendir(dir_name);

	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
			{
				char dirPath[200];
				snprintf(dirPath, 200, "%s/%s", dir_name, dir->d_name);

				InitParser(dirPath);
				pi = CheckUndeclared();
				StopParser();
				if (pi.er == undecIdentifier)
					return pi;
				return pi;
			}
		}
	}
	closedir(d);

	return pi;
}

int StopCompiler()
{
	return 1;
}

#ifndef TEST_COMPILER
int main()
{
	ParserInfo pi = compile("UNDECLAR_VAR");
	printf("error: %d %s at line %d in file %s\n", pi.er, pi.tk.lx, pi.tk.ln, pi.tk.fl);
	PrintTable();

	/* ParserInfo p = compile("UNDECLAR_CLASS");
	printf("error: %d %s at line %d in file %s\n", p.er, p.tk.lx, p.tk.ln, p.tk.fl);
	PrintTable(); */
	return 1;
}
#endif
