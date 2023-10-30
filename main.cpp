#include <iostream>
#include "Board.hpp"

using namespace std;
#define T 100

int check_finish(int t, int board[][N])
{
    return 0;
}

int check_pass(int k, int board[][N])
{
    return 0;
}

struct INPUT_DATA input_key(Board *match)
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
    int k;
    struct INPUT_DATA input_data;

    Board match;

    k = 1;
    match.k = k;
    while (match.check_finish() != 0) {
        match.print_board();
        match.k = k;
        match.turn++;
        if (match.check_pass() != 0) {
            input_data = input_key(&match);
            match.change_board(input_data);

            match.board[input_data.y][input_data.x] = match.k;
        }
        k = k * -1;
    }

    return 0;
}
