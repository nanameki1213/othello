#include "Board.hpp"
using namespace std;

Board::Board()
{
    board = new int*[N];
    for(int i = 0; i < N; i++) {
        board[i] = new int[N];
    }

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(i == 0 || j == 0 || i == 9 || j == 9) {
                board[i][j] = 2;
                continue;
            } else {
                board[i][j] = 0;
            }
        }
    }
    board[4][4] = board[5][5] = 1;
    board[4][5] = board[5][4] = -1;

    turn = 0;
}

void Board::print_board()
{
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            switch(board[i][j]) {
                case 0:
                    cout << "--"; break;
                case 1:
                    cout << "●"; break;
                case -1:
                    cout << "★"; break;
                case 2:
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