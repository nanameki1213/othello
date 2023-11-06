#include "Board.hpp"
#include <math.h>
using namespace std;

#define PI 3.14

Board::Board()
{
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

void Board::print_board()
{
    printf("\n現在のターンは%sです\n\n", k == BLACK ? "★" : "●");
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            switch(board[i][j]) {
                case NONE:
                    cout << "--"; break;
                case WHITE:
                    cout << "●"; break;
                case BLACK:
                    cout << "★"; break;
                case OUT_OF_RANGE:
                    cout << "■";
                default:
                    ;
            }
        }
        cout << endl;
    }
}

int Board::check_finish()
{
    return 1;
}

int Board::check_pass()
{
    return 1;
}

void Board::change_board(INPUT_DATA data)
{

}

bool Board::can_put(int x, int y)
{
    // 配置するマスに何も置かれていないことの確認
    if(board[y][x] != NONE)
        return false;

    for(int i = 0; i < 8; i++) { 
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
    printf("mx:%d, my:%d\n", mx, my);

    while(board[my][mx] == opp) {
        check = true;

        mx += round(cos(PI/4 * direc));
        my += round(sin(PI/4 * direc));

        if(board[my][mx] == OUT_OF_RANGE)
            return false;

        printf("mx:%d, my:%d\n", mx, my);
    }

    if(board[my][mx] == k && check) {
        return true;
    }

    return false;
}