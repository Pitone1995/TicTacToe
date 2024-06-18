#ifndef TicTacToe_H
#define TicTacToe_H

#include <string>
#include <vector>
#include <map>

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

class TicTacToe {

public:

    TicTacToe();
    ~TicTacToe();

    int run();

private:

    void clearBoard();
    void drawBoard();
	void drawBoard(int maskWin);
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
	bool checkMask(int movesPlayer, int mask);

	void getPlayerInfo();
    std::string getPlayerName(const Player &p);
    int getRandomInt(int min, int max);
    int getCoord(const std::string &coordType);

    int m_countMoves = 0;
    Move m_moves[MAX_MOVES];
    bool m_play = true;
    int m_sort = false;

    std::map<Player, int> m_movesPlayer;
    std::vector<int> m_movesAvailable;
	
	std::string m_player_name = "";
	char m_player_sym = '\0';
	char m_computer_sym = '\0';
};

#endif
