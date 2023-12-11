#pragma once

#include "Board.hpp"
#include <vector>

typedef int (*evaluete_function)(Board *board, int my_k);

class Game_Node {
public:
    Board *current_board;
    std::vector<Game_Node*> children_node;

    // 自分が黒なのか白なのか
    int my_k;

    int evaluete_num;
    struct INPUT_DATA pre_act;

    Game_Node(int k);
    Game_Node(const Game_Node &node);

    Game_Node *parent_node;

    void expandChildren();
    void deleteChildren();

    evaluete_function ev_func;

};

void expandChildren_by_num(Game_Node *node, int n);
void printTree(Game_Node *node, int n = -1, int depth = -1);