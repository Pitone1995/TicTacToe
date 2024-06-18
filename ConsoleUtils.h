#include <windows.h>
#include <string>
#include <iostream>

// From color --help
#define HIGHLIGHT_TXT 0xF0
#define DEFAULT_TXT 0x7
#define GREEN 0xA

class Console {

public:
	static void errorTextColor() {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 79);
	}

	static void setColor(unsigned short int code) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, code);
	}

	static void resetColor() {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, DEFAULT_TXT);
	}

	static void ShowConsoleCursor(bool showFlag)
	{
		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_CURSOR_INFO cursorInfo;

		GetConsoleCursorInfo(out, &cursorInfo);
		cursorInfo.bVisible = showFlag; // set the cursor visibility
		SetConsoleCursorInfo(out, &cursorInfo);
	}

	static void printAllColor() {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		// you can loop k higher to see more color choices
		for (int k = 1; k < 255; k++)
		{
			// pick the colorattribute k you want
			SetConsoleTextAttribute(hConsole, k);
			std::cout << k << " I want to be nice today!\n";
		}
	}

	static void readKey() {
		char c{};
		std::string s;
		std::getline(std::cin, s);

		std::cout << "Enter a Key" << std::endl;
		std::getline(std::cin, s);
	}

	static void consolePause(std::string msg) {
		std::cout << msg;
		std::string s{};
		std::getline(std::cin, s); // Attende l'input da parte dell'utente
	}

	static void clearScreen() {
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		COORD coord = { 0, 0 };
		DWORD count;

		if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
		{
			DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;

			// Ripristina il testo e il colore di sfondo predefiniti
			WORD defaultAttributes = csbi.wAttributes;
			FillConsoleOutputAttribute(hStdOut, defaultAttributes, consoleSize, coord, &count);

			// Pulisce lo screen buffer con spazi bianchi
			FillConsoleOutputCharacter(hStdOut, ' ', consoleSize, coord, &count);

			// Riposiziona il cursore nella posizione iniziale
			SetConsoleCursorPosition(hStdOut, coord);
		}
	}


};



