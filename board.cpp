#include "board.h"

#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>

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

// Player symbols
#define S_USER 'X'
#define S_COMPUTER 'O'

Board::Board() {
}

void Board::clearBoard() {
 
    system("cls");
    countMoves = 0;
    movesPlayer.clear();
    movesAvailable = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    for (int i = 0; i < MAX_MOVES; i++) {
        moves[i].c = 0;
        moves[i].r = 0;
        moves[i].s = '\0';
    }
}

void Board::drawBoard() {

    cout << "" << endl;

    cout << "-------------------------------------------------" << endl;
    for (int i = 0; i < MAX_MOVES; i++) {

        cout << "|\t" << moves[i].s << "\t";

        if (!((i + 1) % 3) && i) {

            cout << "|" << endl;
            cout << "-------------------------------------------------" << endl;
        }
    }

    cout << "" << endl;
}

void Board::sortPlayer() {

    Player p = static_cast<Player>(getRandomInt(USER, COMPUTER));
    sort = static_cast<int>(p);
}

void Board::endGame() {

    string choice = "";
    while (choice != "y" && choice != "n") {

        cout << "Game over! Do you wanna play again (y/n)? ";
        cin >> choice;

        if (choice == "y")
            play = true;
        else if (choice == "n")
            play = false;
    }
}

bool Board::wannaPlay() {
    return play;
}

int Board::getRandomInt(int min, int max) {

    chrono::system_clock::time_point now = chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto value = now_ms.time_since_epoch();

    int seed = static_cast<int>(value.count());

    MyRNG rng;
    rng.seed(seed);

    uniform_int_distribution<int> dist(min, max);

    return dist(rng);
}

bool Board::gameOver() {
    return countMoves == MAX_MOVES;
}

 Player Board::getPlayer() {

    if (!(countMoves % 2) - sort)
        return USER;
    else
        return COMPUTER;
}

string Board::getPlayerName(const Player &p) {
    return p == USER ? "USER" : "COMPUTER";
}

Move Board::getUserMove() {

    Move m;

    m.r = getCoord("row");
    m.c = getCoord("column");
    m.s = S_USER;

    return m;
}

int Board::getCoord(const string &coordType) {

    // non farlo esplodere se inserisco stringa o carattere
    int c = 0;

    while (c < 1 || c > 3) {

        cout << "Insert " << coordType << ": ";
        cin >> c;
    }

    return c;
}

 Move Board::getComputerMove() {

    Move m;
    
    // Computer deve scegliere una tra le mosse_disp
    int index = getRandomInt(0, movesAvailable.size() - 1);
    int pos = movesAvailable.at(index);

    /*
    Mappo pos in r,c:

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

    m.s = S_COMPUTER;

    return m;
}

void Board::setMove(const Move &m, const Player &p) {

    int pos;

    /*
    Mappo r,c in pos:

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

    // Salvo la mossa nell'array generale
    moves[pos] = m;

    /*
    mosse_player[p] è un intero che rappresenta le mosse del giocatore p
    salvo la mossa mettendo a 1 il bit corrispondente

    esempio:

    {r,c} = 1,2 -> pos = 1
    mosse_player[p] =          0b 0 1 0 0 0 0 0 0 0

    {r,c} = 1,1 -> pos = 0
    mosse_player[p] =          0b 1 1 0 0 0 0 0 0 0

    {r,c} = 3,1 -> pos = 6
    mosse_player[p] =          0b 1 1 0 0 0 0 1 0 0

    {r,c} = 1,3 -> pos = 2
    mosse_player[p] =          0b 1 1 1 0 0 0 1 0 0
    */

    movesPlayer[p] |= (1 << (8 - pos));

    // Aggiorno le mosse disponibili
    movesAvailable.erase(find(movesAvailable.begin(), movesAvailable.end(), pos));

    countMoves++;
}

bool Board::checkMove(const Move &m) {

    for (int i = 0; i < MAX_MOVES; i++) {

        if (moves[i].r == m.r && moves[i].c == m.c)
            return false;
    }

    return true;
}

bool Board::checkWin(const Player &p) {

    /*
    mosse_player[USER]:     0b 1 1 1 0 0 0 1 0 0 (simbolo X)
    mosse_player[COMPUTER]: 0b 0 0 0 0 1 0 0 0 1 (simbolo O)

    Le mosse corrispondono a questa configurazione:
    -------------
    | X | X | X |
    -------------
    |   | O |   |
    -------------
    | X |   | O |
    -------------

    Confronto bit a bit (&) tra l'intero che rappresenta le mosse di USER e le maschere che
    rappresentano le condizioni di tris, ad esempio WIN_H_1 che è il tris sulla prima riga orizzontale:
    se il risultato è uguale alla maschera, USER ha fatto tris

    mosse_player[USER]:     0b 1 1 1 0 0 0 1 0 0
                            &
    WIN_H_1:                0b 1 1 1 0 0 0 0 0 0
    --------------------------------------------
                            0b 1 1 1 0 0 0 0 0 0 = WINCON_H_1
    */

    if (
        (movesPlayer[p] & WINCON_H_1) == WINCON_H_1 ||
        (movesPlayer[p] & WINCON_H_2) == WINCON_H_2 ||
        (movesPlayer[p] & WINCON_H_3) == WINCON_H_3 ||
        (movesPlayer[p] & WINCON_V_1) == WINCON_V_1 ||
        (movesPlayer[p] & WINCON_V_2) == WINCON_V_2 ||
        (movesPlayer[p] & WINCON_V_3) == WINCON_V_3 ||
        (movesPlayer[p] & WINCON_O_1) == WINCON_O_1 ||
        (movesPlayer[p] & WINCON_O_2) == WINCON_O_2
        ) {
        cout << "The winner is: [" << getPlayerName(p) << "]" << endl;
        cout << "" << endl;

        return true;
    }
    else
        return false;
}