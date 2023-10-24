#pragma

#include <iostream>
#define N 10

struct INPUT_DATA {
    int x, y;
};

class Board {
public:
    int **board;
    int turn;
    int k;

    Board();

    void print_board();
    int check_finish();
    int check_pass();
    void change_board(INPUT_DATA data);
};