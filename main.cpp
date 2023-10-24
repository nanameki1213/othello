#include <iostream>
#include "Board.hpp"

using namespace std;
#define T 100

typedef struct LOG LOG;

struct LOG {
    LOG *next;
    LOG *prev;
    int **board;
};

void make_board(int board[][N])
{
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
}

void print_board(int board[][N])
{
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            switch(board[i][j]) {
                case 0:
                    printf("--"); break;
                case 1:
                    printf("●"); break;
                case -1:
                    printf("★"); break;
                case 2:
                    printf("■");
                default:
                    ;
            }
        }
        printf("\n");
    }
}

int check_finish(int t, int board[][N])
{
    return 0;
}

int check_pass(int k, int board[][N])
{
    return 0;
}

struct INPUT_DATA input_key(int k, int board[][N])
{
    int x, y;
    struct INPUT_DATA data;

    cout << "手を打つ場所を決めてください:" << endl;
    cout << "x:"; cin >> x;
    cout << "y:"; cin >> y; 

    data.x = x;
    data.y = y;

    return data;

}

void change_board(int k, int x, int y, int board[][N])
{

}

int main(void)
{
    int board[N][N] = { 0 };
    int k;
    struct INPUT_DATA input_data;

    Board match;
    match.print_board();

    k = 1;
    match.k = k;
    while (match.check_finish() != 0) {
        match.k = k;
        match.turn++;
        if (match.check_pass() != 0) {
            input_data = input_key(k, board);
            match.change_board(input_data);
            match.print_board();
        }
        k = k * -1;
    }
}
