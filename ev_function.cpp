#include "ev_function.hpp"
#include <iostream>
#include "GameTree.hpp"

using namespace std;

int corner[N - 2][N - 2] = {
    {10,-5,1,0,0,1,-5,10},
    {-5,-5,0,0,0,0,-5,-5},
    {1,0,1,0,0,1,0,1},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {1,0,1,0,0,1,0,1},
    {-5,-5,0,0,0,0,-5,-5},
    {10,-5,1,0,0,1,-5,10},
};

int ev_score(Board *board, int my_k)
{
    int ev_num = board->get_current_num(my_k);

    return ev_num;
}

int ev_legal_act(Board *board, int my_k)
{
    Board *opp = new Board(*board);

    vector<struct INPUT_DATA> my_act;
    vector<struct INPUT_DATA> opp_act;

    board->get_legal_act(my_act);
    opp->get_legal_act(opp_act);

    int can_put_diff = (board->k);
}

int ev_best(Board *board, int my_k)
{
    Board *opp = new Board(*board);

    vector<INPUT_DATA> my_act;
    vector<INPUT_DATA> opp_act;
    board->get_legal_act(my_act);
    opp->get_legal_act(opp_act);

    int can_put_diff;
    if(board->k == my_k)
        can_put_diff = my_act.size() - opp_act.size();
    else 
        can_put_diff = opp_act.size() - my_act.size();

    int k = 0.5; // 係数

    // 角の形の良さの比較
    int my_corner = 0;
    int opp_corner = 0;
    for(int i = 1; i < N - 1; i++) {
        for(int j = 1; j < N - 1; j++) {
            if(board->board[i][j] == board->k) {
                my_corner += corner[i][j];
            }
            if(opp->board[i][j] == opp->k) {
                my_corner += corner[i][j];
            }
        }
    }
    if(board->k != my_k) {
        int tmp = my_corner;
        my_corner = opp_corner;
        opp_corner = tmp;
    }

    int corner_score = my_corner - opp_corner;

    int ev_num = can_put_diff + corner_score * k;
    

    return ev_num;
}