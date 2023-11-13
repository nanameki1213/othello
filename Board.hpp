#pragma

#include <iostream>
#define N 10
#define NONE 0
#define WHITE 1
#define BLACK -1
#define OUT_OF_RANGE 2

#define PI 3.14

struct INPUT_DATA {
    int x, y;
};

enum {
    RIGHT = 0,
    RIGHT_UP,
    UP,
    LEFT_UP,
    LEFT,
    LEFT_DOWN,
    DOWN,
    RIGHT_DOWN,
    DIRECTION_MAX
};

class Board {
public:
    int **board;
    int turn;
    int k;
    struct INPUT_DATA cur;

    Board(int **init_board = nullptr);
    Board(const Board &b);

    bool can_put(int x, int y);
    bool is_legal_coord(int x, int y);
    bool check_change(int x, int y, int direc);
    void change_in_row(int x, int y, int direc);
    void print_board();
    int check_finish();
    int check_pass();
    void change_board(int x, int y);
};
