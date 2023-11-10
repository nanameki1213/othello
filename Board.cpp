#include "Board.hpp"
#include <math.h>
#include <string.h>
using namespace std;

#define PI 3.14

Board::Board()
{
    cur.x = 1;
    cur.y = 1;

    board = new int*[N];
    for(int i = 0; i < N; i++) {
        board[i] = new int[N];
    }

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(i == 0 || j == 0 || i == 9 || j == 9) {
                board[i][j] = OUT_OF_RANGE;
                continue;
            } else {
                board[i][j] = NONE;
            }
        }
    }
    board[4][4] = board[5][5] = WHITE;
    board[4][5] = board[5][4] = BLACK;

    turn = 0;
}

Board::Board(const Board &b)
{
    board = new int*[N]();
    for(int i = 0; i < N; i++) {
        board[i] = new int[N]();
    }
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            board[i][j] = b.board[i][j];
        }
    }

    cur.x = b.cur.x;
    cur.y = b.cur.y;
    turn = b.turn;
    k = b.k;
}

void Board::print_board()
{
    cout << "turn: " << turn << endl;
    printf("\n現在のターンは%sです\n\n", k == BLACK ? "〇" : "●");
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(cur.x == j && cur.y == i) {
                cout << "\x1b[48;5;242m";
            }
            switch(board[i][j]) {
                case NONE:
                    cout << "--"; break;
                case WHITE:
                    cout << "●"; break;
                case BLACK:
                    cout << "〇"; break;
                case OUT_OF_RANGE:
                    cout << "■";
                    break;
                default:
                    ;
            }
            cout << "\x1b[49m";
            cout << "\x1b[39m";
        }
        cout << endl;
    }

    cout << "\nCursor:(" << cur.x << "," << cur.y << ")\n";
}

int Board::check_finish()
{
    return 1;
}

int Board::check_pass()
{
    return 1;
}

void Board::change_board(int x, int y)
{
    board[y][x] = k;

    int mx, my;

    int opp = k * -1;

    for(int i = 0; i < N -2; i++) {
        change_in_row(x, y, i);
    }
}

void Board::change_in_row(int x, int y, int direc)
{
    int opp = k * -1;
    int mx, my;
    bool check = false;

    mx = x + round(cos(PI/4 * direc));
    my = y + round(sin(PI/4 * direc));

    if(board[my][mx] == OUT_OF_RANGE) {
        return;
    }
    // printf("mx:%d, my:%d\n", mx, my);

    while(board[my][mx] == opp) {
        check = true;

        mx += round(cos(PI/4 * direc));
        my += round(sin(PI/4 * direc));

        if(board[my][mx] == OUT_OF_RANGE)
            return;

        // printf("mx:%d, my:%d\n", mx, my);
    }

    if(board[my][mx] != k || !check) {
        return;    
    }

    mx = x + round(cos(PI/4 * direc));
    my = y + round(sin(PI/4 * direc));

    while(board[my][mx] == opp) {
        board[my][mx] = k;
        
        mx += round(cos(PI/4 * direc));
        my += round(sin(PI/4 * direc));
    }
}

bool Board::can_put(int x, int y)
{
    // 配置するマスに何も置かれていないことの確認
    if(board[y][x] != NONE)
        return false;

    for(int i = 0; i < N - 2; i++) {
        if(check_change(x, y, i)) {
            return true;
        }
    }
    
    return false;
}

bool Board::check_change(int x, int y, int direc)
{
    int opp = k * -1;
    int mx, my;
    bool check = false;

    mx = x + round(cos(PI/4 * direc));
    my = y + round(sin(PI/4 * direc));

    if(board[my][mx] == OUT_OF_RANGE) {
        return false;
    }
    // printf("mx:%d, my:%d\n", mx, my);

    while(board[my][mx] == opp) {
        check = true;

        mx += round(cos(PI/4 * direc));
        my += round(sin(PI/4 * direc));

        if(board[my][mx] == OUT_OF_RANGE)
            return false;

        // printf("mx:%d, my:%d\n", mx, my);
    }

    if(board[my][mx] == k && check) {
        return true;
    }

    return false;
}