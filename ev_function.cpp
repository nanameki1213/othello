#include "ev_function.hpp"
#include <iostream>
#include "GameTree.hpp"

using namespace std;

int corner[N][N] = {
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    { 2,10,-5, 5, 0, 0, 5,-5,10, 2},
    { 2,-5,-5, 0, 0, 0, 0,-5,-5, 2},
    { 2, 5, 0, 5, 0, 0, 5, 0, 5, 2},
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    { 2, 5, 0, 5, 0, 0, 5, 0, 5, 2},
    { 2,-5,-5, 0, 0, 0, 0,-5,-5, 2},
    { 2,10,-5, 5, 0, 0, 5,-5,10, 2},
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
};

int ev_score(Board *board, int my_k)
{
    int ev_num = board->get_current_num(my_k);

    return ev_num;
}

int ev_legal_act(Board *board, int my_k)
{
    Board *opp = new Board(*board);
    opp->k = board->k * -1;

    vector<struct INPUT_DATA> my_act;
    vector<struct INPUT_DATA> opp_act;

    board->get_legal_act(my_act);
    opp->get_legal_act(opp_act);

    int can_put_diff = (board->k == my_k) ? (my_act.size() - opp_act.size()) : (opp_act.size() - my_act.size());

    return can_put_diff;
}

int ev_best(Board *board, int my_k)
{
    Board *own = new Board(*board);
    Board *opp = new Board(*board);
    own->k = my_k;
    opp->k = my_k * -1;

    int score_diff = board->get_current_num(my_k) - board->get_current_num(my_k * -1);

    int k = 0.1; // 係数

    // 角の形の良さの比較
    int my_corner = 0;
    int opp_corner = 0;
    for(int i = 1; i < N - 1; i++) {
        for(int j = 1; j < N - 1; j++) {
            if(own->board[i][j] == my_k) {
                my_corner += corner[i][j];
            }
            if(opp->board[i][j] == my_k * -1) {
                opp_corner += corner[i][j];
            }
        }
    }

    int corner_score = my_corner - opp_corner;

    int ev_num = corner_score  + score_diff;
    
    cout << "corner_score:" << "(" << my_corner << "-" << opp_corner << "=" << corner_score << ")" << " + " << "score_diff:" << score_diff << " = " << ev_num << endl;

    return ev_num;
}