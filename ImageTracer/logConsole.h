#ifndef LOGCONSOLE
#define LOGCONSOLE


#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>


void createLogConsole()
{
	// create console window ***********************
	int hConHandle;
	long lStdHandle;
	FILE *fp;


	AllocConsole();


	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;

	setvbuf(stdout, NULL, _IONBF, 0);
	//**********************************************

	printf("log console created\n");
}

#endif