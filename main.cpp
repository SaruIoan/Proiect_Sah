#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <stdlib.h>
#include <conio.h>

using namespace std;

char board[8][8] = {
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
};

bool whiteTurn = true;

void drawBoard() {
    cout << "   a  b  c  d  e  f  g  h" << endl;
    cout << "  +--+--+--+--+--+--+--+--+" << endl;

    for (int i = 0; i < 8; ++i) {
        cout << 8 - i << " |";
        for (int j = 0; j < 8; ++j) {
            cout << board[i][j] << " |";
        }
        cout << " " << 8 - i << endl;
        cout << "  +--+--+--+--+--+--+--+--+" << endl;
    }
    cout << "   a  b  c  d  e  f  g  h" << endl;
}

bool isValidPosition(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

bool isOpponent(char piece, bool white) {
    if (white) return islower(piece);
    return isupper(piece);
}

bool isEmpty(char piece) {
    return piece == ' ';
}

bool canAttack(int fx, int fy, int tx, int ty, char piece);
bool isPathClear(int fx, int fy, int tx, int ty);
bool isValidMove(int fx, int fy, int tx, int ty, char piece);

bool isKingInCheck(bool white) {
    // Gaseste pozitia regelui
    char king = white ? 'K' : 'k';
    int kx, ky;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] == king) {
                kx = i; ky = j;
            }
        }
    }
    // Verifica daca regele este in sah
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (!isEmpty(board[i][j]) && isOpponent(board[i][j], white)) {
                if (canAttack(i, j, kx, ky, board[i][j])) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool canAttack(int fx, int fy, int tx, int ty, char piece) {
    int dx = tx - fx;
    int dy = ty - fy;
    switch (tolower(piece)) {
        case 'p':
            if (isupper(piece)) { // Pionul alb
                return dx == -1 && abs(dy) == 1;
            } else {
                return dx == 1 && abs(dy) == 1;
            }
        case 'r':
            return (dx == 0 || dy == 0) && isPathClear(fx, fy, tx, ty);
        case 'n':
            return (abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2);
        case 'b':
            return abs(dx) == abs(dy) && isPathClear(fx, fy, tx, ty);
        case 'q':
            return ((dx == 0 || dy == 0) || abs(dx) == abs(dy)) && isPathClear(fx, fy, tx, ty);
        case 'k':
            return abs(dx) <= 1 && abs(dy) <= 1;
    }
    return false;
}

bool isPathClear(int fx, int fy, int tx, int ty) {
    int dx = (tx > fx) ? 1 : (tx < fx) ? -1 : 0;
    int dy = (ty > fy) ? 1 : (ty < fy) ? -1 : 0;
    int x = fx + dx;
    int y = fy + dy;
    while (x != tx || y != ty) {
        if (!isEmpty(board[x][y])) return false;
        x += dx;
        y += dy;
    }
    return true;
}
//verifica daca mutarea este valida
bool isValidMove(int fx, int fy, int tx, int ty, char piece) {
    if (!isValidPosition(tx, ty)) return false;
    if (fx == tx && fy == ty) return false;
    if (!isEmpty(board[tx][ty]) && !isOpponent(board[tx][ty], isupper(piece))) return false;
    int dx = tx - fx;
    int dy = ty - fy;
    switch (tolower(piece)) {
        case 'p': {
            int dir = isupper(piece) ? -1 : 1;
            int start = isupper(piece) ? 6 : 1;
            if (dy == 0 && dx == dir && isEmpty(board[tx][ty])) return true;
            if (dy == 0 && dx == 2 * dir && fx == start && isEmpty(board[tx][ty]) && isEmpty(board[fx + dir][fy])) return true;
            if (abs(dy) == 1 && dx == dir && isOpponent(board[tx][ty], isupper(piece))) return true;
            return false;
        }
        case 'r':
            return (fx == tx || fy == ty) && isPathClear(fx, fy, tx, ty);
        case 'n':
            return (abs(tx - fx) == 2 && abs(ty - fy) == 1) || (abs(tx - fx) == 1 && abs(ty - fy) == 2);
        case 'b':
            return abs(tx - fx) == abs(ty - fy) && isPathClear(fx, fy, tx, ty);
        case 'q':
            return ((fx == tx || fy == ty) || abs(tx - fx) == abs(ty - fy)) && isPathClear(fx, fy, tx, ty);
        case 'k':
            return abs(tx - fx) <= 1 && abs(ty - fy) <= 1;
    }
    return false;
}

bool isCheckmate(bool white) {
    if (!isKingInCheck(white)) return false;
    // Verifica daca regele mai poate scapa
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (!isEmpty(board[i][j]) && ((isupper(board[i][j]) ? true : false) == white)) {
                for (int x = 0; x < 8; ++x) {
                    for (int y = 0; y < 8; ++y) {
                        if (isValidMove(i, j, x, y, board[i][j])) {
                            // Simuleaza mutarea
                            char temp = board[x][y];
                            board[x][y] = board[i][j];
                            board[i][j] = ' ';
                            bool stillInCheck = isKingInCheck(white);
                            // Undo
                            board[i][j] = board[x][y];
                            board[x][y] = temp;
                            if (!stillInCheck) return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

int main() {
    while (true) {
        drawBoard();
        cout << (whiteTurn ? "Player 1 (White) move: " : "Player 2 (Black) move: ");
        string move;
        getline(cin, move);
        if (move.length() != 5 || move[2] != ' ') {
            cout << "Invalid input format. Use 'e2 e4'" << endl;
            cout<<".................................press enter";cout<<endl;
            char ch=getch();
            system("cls");
            continue;
        }
        int fx = 8 - (move[1] - '0');
        int fy = move[0] - 'a';
        int tx = 8 - (move[4] - '0');
        int ty = move[3] - 'a';
        if (!isValidPosition(fx, fy) || !isValidPosition(tx, ty)) {
            cout << "Invalid position." << endl;
            cout<<".................................press enter";cout<<endl;
            char ch=getch();
            system("cls");
            continue;
        }
        char piece = board[fx][fy];
        if (isEmpty(piece) || (isupper(piece) != whiteTurn)) {
            cout << "No your piece there." << endl;
            cout<<".................................press enter";cout<<endl;
            char ch=getch();
            system("cls");
            continue;
        }
        if (!isValidMove(fx, fy, tx, ty, piece)) {
            cout << "Invalid move." << endl;
            cout<<".................................press enter";cout<<endl;
            char ch=getch();
            system("cls");
            continue;
        }
        if(!isCheckmate(whiteTurn)){
            char temp = board[tx][ty];
            board[tx][ty] = board[fx][fy];
            board[fx][fy] = ' ';
            bool stillInCheck = isKingInCheck(whiteTurn);

            if(!stillInCheck){
                whiteTurn = !whiteTurn;
                cout<<".................................press enter";cout<<endl;
                char ch=getch();
                system("cls");
            }else {
                cout<<"King in check! Move the king first"<<endl;
                // Undo
                board[fx][fy] = board[tx][ty];
                board[tx][ty] = temp;
                cout<<".................................press enter";cout<<endl;
                char ch=getch();
                system("cls");
            }
        }
        else{
            drawBoard();
            cout << (whiteTurn ? "Player 2 (Black) wins!" : "Player 1 (White) wins!") << endl;
            cout<<".................................press enter";cout<<endl;
            char ch=getch();
            system("cls");
            break;
        }
    }
    return 0;
}
