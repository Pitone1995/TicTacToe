#include <iostream>
#include "board.h"

using namespace std;

int main(int argc, char *argv[]) {

    Board board;

    while (board.wannaPlay()) {

        board.clearBoard();
        board.drawBoard();
        board.sortPlayer();

        while (!board.gameOver()) {

            Move m;

            Player p = board.getPlayer();
            switch (p) {

                case USER:
                    cout << "Player [USER]" << endl;
                    m = board.getUserMove();
                    break;

                default: // COMPUTER
                    cout << "Player [COMPUTER]" << endl;
                    m = board.getComputerMove();
                    break;
            }

            if (board.checkMove(m)) {

                board.setMove(m, p);
                board.drawBoard();

                if (board.checkWin(p))
                    break;
            }
            else {

                cout << "Cell is occupied: make another move!" << endl;
                cout << "" << endl;
            }
        }

        board.endGame();
    }    

    return 0;
}