#include <iostream>
#include <vector>
#include "Menu.h"
#include "TicTacToe.h"

using namespace std;

int main(int argc, char *argv[]) {
 
	vector<string> options = {"New game", "Load game", "Quit"};
	Menu mainMenu(options);

	while (1) {
		
		int selIndex = mainMenu.run();
		switch (selIndex) {

			case NEW_GAME: {
			
				TicTacToe game;
				game.run();
			}
				break;

			case LOAD_GAME:
				mainMenu.clear();
				cout << "Load game selected: press any key to return to main menu...";
				system("pause > nul");
				break;

			default: // QUIT
				return 0;
				break;
		}
	}

	return 0;
}
