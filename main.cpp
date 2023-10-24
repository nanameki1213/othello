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

struct INPUT_DATA input_key(Board *match)
{
    struct INPUT_DATA data;

    struct INPUT_DATA cur = match->cur;

    char c;
    bool is_decide = false;
    while(!is_decide) {
        c = getc(stdin);
        switch(c) {
            case 'w':
                cur.y--;
                break;
            case 's':
                cur.y++;
                break;
            case 'a':
                cur.x--;
                break;
            case 'd':
                cur.x++;
                break;

            case '\n':
                is_decide = true;
                break;
        }
        if(cur.x > N - 2)
            cur.x = N - 2;
        if(cur.x < 1)
            cur.x = 1;
        if(cur.y > N - 2)
            cur.y = N - 2;
        if(cur.y < 1)
            cur.y = 1;
        match->cur = cur;

        system("clear"); // windows環境ならsystem("cls");
        match->print_board();

    }

    data.x = cur.x;
    data.y = cur.y;

    return data;

}

void change_board(int k, int x, int y, int board[][N])
{

}

int main(void)
{
    Board match;
    INPUT_DATA data;
    INPUT_DATA cur = {0, 0};

    match.print_board();

    while(match.check_finish() != 0) {
        match.turn++;
        if(match.check_pass() != 0) {
            data = input_key(&match);
        }
    }
}
