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
#include "compiler.h"

char *readDirFiles(char *dir_name)
{
	char *files;
	strcpy(files, " ");
	DIR *d;
	struct dirent *dir;
	d = opendir(".");
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			strcat(files, " ");
			strcat(files, dir->d_name);
		}
		closedir(d);
	}
	return files;
}

int InitCompiler()
{
	return 1;
}

ParserInfo compile(char *dir_name)
{
	ParserInfo p;

	// write your code below

	p.er = none;
	return p;
}

int StopCompiler()
{
	StopParser();
	return 1;
}

#ifndef TEST_COMPILER
int main()
{
	InitCompiler();
	ParserInfo p = compile("Pong");
	PrintError(p);
	StopCompiler();
	return 1;
}
#endif
