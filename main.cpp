#include <iostream>
#include "Board.hpp"
#include <string.h>
#include <vector>

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

    do {

        cout << "手を打つ場所を決めてください:" << endl;
        cout << "x:"; cin >> x;
        cout << "y:"; cin >> y;

        if(match->can_put(x, y)) {
            break;
        } else {
            printf("その場所にはおけません\n");
        }

    } while(1);

    data.x = x;
    data.y = y;



    return data;

}

int main(void)
{
    int k;
    struct INPUT_DATA input_data;

    Board match;

    vector<Board> log;

    k = 1;
    match.k = k;
    for(int i = 0; i < 3; i++) {
    // while (match.check_finish() != 0) {

        // boardに変更を加える前にログをとる
        Board tmp = Board(match);
        log.push_back(tmp);

        match.print_board();

        if (match.check_pass() != 0) {
            input_data = input_key(&match);
            match.change_board(input_data.x, input_data.y);
        }
        
        match.k *= -1;
        match.turn++;
    }

    for(auto itr = log.begin(); itr != log.end(); itr++) {
        cout << "turn: " << itr - log.begin() << endl;
        (*itr).print_board();
    }

    return 0;
}
