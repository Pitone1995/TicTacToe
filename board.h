#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include <map>

using namespace std;

#define MAX_MOVES 9

typedef struct {

    int r;
    int c;
    char s;
} Move;

typedef enum {

    USER,
    COMPUTER
} Player;

class Board {

public:

    Board();

    void clearBoard();
    void drawBoard();
    void sortPlayer();
    void endGame();
    bool wannaPlay();    

    bool gameOver();
    Player getPlayer();
    Move getUserMove();
    Move getComputerMove();

    void setMove(const Move &m, const Player &p);
    bool checkMove(const Move &m);
    bool checkWin(const Player &p);

private:

    string getPlayerName(const Player &p);
    int getRandomInt(int min, int max);
    int getCoord(const string &coordType);

    int countMoves = 0;
    Move moves[MAX_MOVES];
    bool play = true;
    int sort = false;

    map<Player, int> movesPlayer;
    vector<int> movesAvailable;
};

#endif