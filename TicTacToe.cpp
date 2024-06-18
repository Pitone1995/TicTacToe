#include "TicTacToe.h"

#include "ConsoleUtils.h"
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;

typedef mt19937 MyRNG;

// Win conditions
#define WINCON_H_1 0x1C0 // 0b111000000
#define WINCON_H_2 0x038 // 0b000111000
#define WINCON_H_3 0x007 // 0b000000111
#define WINCON_V_1 0x124 // 0b100100100
#define WINCON_V_2 0x092 // 0b010010010
#define WINCON_V_3 0x049 // 0b001001001
#define WINCON_O_1 0x111 // 0b100010001
#define WINCON_O_2 0x054 // 0b001010100

TicTacToe::TicTacToe() {
    Console::ShowConsoleCursor(true);
}


TicTacToe::~TicTacToe() {
    Console::ShowConsoleCursor(false);
}

int TicTacToe::run() {

	while (wannaPlay()) {

		clearBoard();
		getPlayerInfo();
        drawBoard();
        sortPlayer();

        while (!gameOver()) {

			Move m;

			Player p = getPlayer();
			switch (p) {
				
				case USER:
					// cout << "Player " << m_player_name << endl;
					m = getUserMove();
					break;
	
				default: // COMPUTER
					// cout << "Player COMPUTER" << endl;
					m = getComputerMove();
					break;
			}

			if (checkMove(m)) {

				setMove(m, p);

				if (checkWin(p))
					break;
				else
					drawBoard();
			}	
           	else {

				cout << "Cell is occupied: make another move!" << endl;
				cout << "" << endl;
			}
	}
	
        endGame();
	}

	return 0;
}

void TicTacToe::getPlayerInfo() {
	
	cout << "Enter your name: ";
	cin >> m_player_name;
	
	while (m_player_sym != 'X' && m_player_sym != 'O') {

		cout << "Player " << m_player_name << " choose your symbol (O/X): ";		
		cin >> m_player_sym;
		if (m_player_sym == 'X')
			m_computer_sym = 'O';
		else 
			m_computer_sym = 'X';
	}
}

void TicTacToe::clearBoard() {
 
    system("cls");
	m_player_sym = '\0';
	m_computer_sym = '\0';
    m_countMoves = 0;
    m_movesPlayer.clear();
    m_movesAvailable = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    for (int i = 0; i < MAX_MOVES; i++) {
        m_moves[i].c = 0;
        m_moves[i].r = 0;
        m_moves[i].s = '\0';
    }
	
	cout << "*************************************************" << endl;
	cout << "*                                               *" << endl;
	cout << "*                  Tic-Tac-Toe                  *" << endl;
	cout << "*                                               *" << endl;	
	cout << "*************************************************" << endl;
	cout << "" << endl;
}

void TicTacToe::drawBoard() {

	system("cls");
    
	cout << "-------------------------------------------------" << endl;
    for (int i = 0; i < MAX_MOVES; i++) {

        cout << "|\t" << m_moves[i].s << "\t";

        if (!((i + 1) % 3) && i) {

            cout << "|" << endl;
            cout << "-------------------------------------------------" << endl;
        }
    }

    cout << "" << endl;
}

void TicTacToe::drawBoard(int maskWin) {

	system("cls");
    
	cout << "-------------------------------------------------" << endl;
    for (int i = 0; i < MAX_MOVES; i++) {

		cout << "|\t";
		
		int highlight = (1 << (MAX_MOVES - i - 1)) & maskWin;
		if (highlight) {
			
			Console::setColor(GREEN);
			cout << m_moves[i].s;
			Console::resetColor();
		}
		else
			cout << m_moves[i].s;
		
		cout << "\t";
        
		if (!((i + 1) % 3) && i) {

            cout << "|" << endl;
            cout << "-------------------------------------------------" << endl;
        }
    }

    cout << "" << endl;
}

void TicTacToe::sortPlayer() {

    Player p = static_cast<Player>(getRandomInt(USER, COMPUTER));
    m_sort = static_cast<int>(p);
}

void TicTacToe::endGame() {

    string choice = "";
    while (choice != "y" && choice != "n") {

        cout << "Game over! Do you wanna play again (y/n)? ";
        cin >> choice;

        if (choice == "y")
            m_play = true;
        else if (choice == "n")
            m_play = false;
    }
}

bool TicTacToe::wannaPlay() {
    return m_play;
}

int TicTacToe::getRandomInt(int min, int max) {

    chrono::system_clock::time_point now = chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto value = now_ms.time_since_epoch();

    int seed = static_cast<int>(value.count());

    MyRNG rng;
    rng.seed(seed);

    uniform_int_distribution<int> dist(min, max);

    return dist(rng);
}

bool TicTacToe::gameOver() {
    return m_countMoves == MAX_MOVES;
}

 Player TicTacToe::getPlayer() {

    if (!(m_countMoves % 2) - m_sort)
        return USER;
    else
        return COMPUTER;
}

string TicTacToe::getPlayerName(const Player &p) {
    return p == USER ? m_player_name : "COMPUTER";
}

Move TicTacToe::getUserMove() {

    Move m;

    m.r = getCoord("row");
    m.c = getCoord("column");
    m.s = m_player_sym;

    return m;
}

int TicTacToe::getCoord(const string &coordType) {

    // TODO: don't crash if insert char or string
    int c = 0;

    while (c < 1 || c > 3) {

        cout << "Insert " << coordType << ": ";
        cin >> c;
    }

    return c;
}

 Move TicTacToe::getComputerMove() {

    Move m;

    // COMPUTER picks a random move in movesAvailable
    int index = getRandomInt(0, m_movesAvailable.size() - 1);
    int pos = m_movesAvailable.at(index);

    /*
    Map pos in r,c:

    pos:    | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
    {r,c}:  |1,1|1,2|1,3|2,1|2,2|2,3|3,1|3,2|3,3|
    */

    switch (pos) {

    case 0:
    case 1:
    case 2:
        m.r = 1;
        m.c = pos + 1;
        break;

    case 3:
    case 4:
    case 5:
        m.r = 2;
        m.c = pos - 2;
        break;

    default:
        m.r = 3;
        m.c = pos - 5;
        break;
    }

    m.s = m_computer_sym;

    return m;
}

void TicTacToe::setMove(const Move &m, const Player &p) {

    int pos;

    /*
    Map r,c in pos:

    {r,c}:  |1,1|1,2|1,3|2,1|2,2|2,3|3,1|3,2|3,3|
    pos:    | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
    */
    switch (m.r) {

    case 1:
        pos = m.c - 1;
        break;

    case 2:
        pos = m.c + 2;
        break;

    default: // 3
        pos = m.c + 5;
        break;
    }

    // Save move in the general array
    m_moves[pos] = m;

    /*
    movesPlayer[p] is an integer that represents moves of player p
    the move is saved setting corresponding bit to 1

    example:

    {r,c} = 1,2 -> pos = 1
    movesPlayer[p] =          0b 0 1 0 0 0 0 0 0 0

    {r,c} = 1,1 -> pos = 0
    movesPlayer[p] =          0b 1 1 0 0 0 0 0 0 0

    {r,c} = 3,1 -> pos = 6
    movesPlayer[p] =          0b 1 1 0 0 0 0 1 0 0

    {r,c} = 1,3 -> pos = 2
    movesPlayer[p] =          0b 1 1 1 0 0 0 1 0 0
    */
    m_movesPlayer[p] |= (1 << (8 - pos));

    // Update available moves
    m_movesAvailable.erase(find(m_movesAvailable.begin(), m_movesAvailable.end(), pos));

    m_countMoves++;
}

bool TicTacToe::checkMove(const Move &m) {

    for (int i = 0; i < MAX_MOVES; i++) {

        if (m_moves[i].r == m.r && m_moves[i].c == m.c)
            return false;
    }

    return true;
}

bool TicTacToe::checkWin(const Player &p) {

    /*
    movesPlayer[USER]:     0b 1 1 1 0 0 0 1 0 0 (X)
    movesPlayer[COMPUTER]: 0b 0 0 0 0 1 0 0 0 1 (O)

    movesPlayer correspond to this board configuration:
    -------------
    | X | X | X |
    -------------
    |   | O |   |
    -------------
    | X |   | O |
    -------------

    bit to bit comparison (&) between movesPlayer (int), representing USER's moves and masks which correspond
    to win conditions, i.e. WINCON_H_1 win on first row:
    if the result equals the mask, USER wins

    movesPlayer[USER]:      0b 1 1 1 0 0 0 1 0 0
                            &
    WINCON_H_1:             0b 1 1 1 0 0 0 0 0 0
    --------------------------------------------
                            0b 1 1 1 0 0 0 0 0 0 = WINCON_H_1
    */

	if (
        checkMask(m_movesPlayer[p], WINCON_H_1) ||
		checkMask(m_movesPlayer[p], WINCON_H_2) ||
		checkMask(m_movesPlayer[p], WINCON_H_3) ||
		checkMask(m_movesPlayer[p], WINCON_V_1) ||
		checkMask(m_movesPlayer[p], WINCON_V_2) ||
		checkMask(m_movesPlayer[p], WINCON_V_3) ||
		checkMask(m_movesPlayer[p], WINCON_O_1) ||
		checkMask(m_movesPlayer[p], WINCON_O_2)
		) {
        cout << "The winner is: " << getPlayerName(p) << endl;
        cout << "" << endl;

        return true;
    }
    else
        return false;
}

bool TicTacToe::checkMask(int movesPlayer, int mask) {
	
	if ((movesPlayer & mask) == mask) {
	
		drawBoard(mask);
		return true;
	}
	else
		return false;
}
