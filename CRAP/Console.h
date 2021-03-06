#ifndef CONSOLE_ALREADY_INCLUDED
	#define CONSOLE_ALREADY_INCLUDED

	#ifndef LINUX
		#include <conio.h>
		#include <stdio.h>
		#include <iostream>
		#include <string.h>
		#include <windows.h>
		#include "OS_Version.h"

		using namespace std;

		void cls();
		void gotoXY(int x, int y);
		COORD getXY();

		int gettextcolor();
		int gettextbackground();
		void settextcolor( int color );
		void settextbackground( int color );

		void Green(bool highlite = false);
		void Red(bool highlite = false);
		void Blue(bool highlite = false);
		void White(bool highlite = false);
		
		__int32 keyhit();

	#endif

	#ifdef LINUX
		void gotoXY(int x, int y);
		COORD getXY();
		void Green(bool highlite = false);
		void Blue(bool highlite = false);
		void Red(bool highlite = false);
		void White(bool highlite = false);
		__int32 keyhit(void);
	#endif

#endif