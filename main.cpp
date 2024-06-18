#include <iostream>
#include "board.h"

using namespace std;

int main(int argc, char *argv[]) {

    Board board;

    while (board.wannaPlay()) {

        board.clearBoard();
        board.getPlayerInfo();
        board.drawBoard();
        board.sortPlayer();

        while (!board.gameOver()) {

            Move m;

            Player p = board.getPlayer();
            switch (p) {

                case USER:
                    m = board.getUserMove();
                    break;

                default: // COMPUTER
                    m = board.getComputerMove();
                    break;
            }

            if (board.checkMove(m)) {

                board.setMove(m, p);

                if (board.checkWin(p))
                    break;
                else
                    board.drawBoard();
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