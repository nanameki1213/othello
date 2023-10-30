#include <iostream>
#include "Board.hpp"
#include <string.h>
#include <vector>

using namespace std;
#define T 100

typedef struct LOG LOG;

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

    vector<Board> log;

    k = 1;
    match.k = k;
    for(int i = 0; i < 10; i++) {
    // while (match.check_finish() != 0) {

        // boardに変更を加える前にログをとる
        Board tmp = Board(match);
        log.push_back(tmp);

        match.k = k;
        match.turn++;
        if (match.check_pass() != 0) {
            input_data = input_key(k, board);
            match.change_board(input_data);
            match.print_board();
        }
        k = k * -1;
    }

    for(auto itr = log.begin(); itr != log.end(); itr++) {
        cout << "turn: " << itr - log.begin() << endl;
        (*itr).print_board();
    }

    return 0;
}
