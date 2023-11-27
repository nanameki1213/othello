#include <iostream>
#include <vector>
#include "GameTree.hpp"
using namespace std;

Game_Node::Game_Node()
{

}

Game_Node::Game_Node(const Game_Node &node)
{
    my_k = node.my_k;
    ev_func = node.ev_func;
}

void Game_Node::expandChildren()
{
    // 既にchildren_nodeがいる場合
    if(!children_node.empty())
        return;

    vector<struct INPUT_DATA> act;

    current_board->get_legal_act(act);

    // 合法手の数だけノードを増やす
    for(auto itr = act.begin(); itr != act.end(); itr++) {
        Board *try_board = new Board(*current_board);
        Game_Node *child = new Game_Node(*this);

        try_board->k = current_board->k * -1;

        // 行動の軌跡を残す
        pre_act = *itr;

        // curren_boardの設定
        try_board->change_board((*itr).y, (*itr).x);
        child->current_board = try_board;
        // 親の設定
        child->parent_node = this;

        children_node.push_back(child);
    }
}

void Game_Node::deleteChildren()
{
    // 既にchildren_nodeがいない場合
    if(children_node.empty())
        return;

    for(auto itr = children_node.begin(); itr != children_node.end(); itr++) {
        delete *itr;
    }
}

// ゲーム木の深さをnにし，アルファベータ法で最善手を得る
void expandChildren_by_num(Game_Node *node, int n)
{
    cout << "called expandbynum\n";
    if(n == 1) {
        node->evaluete_num = node->ev_func(node->current_board);
        cout << "評価値: " << node->evaluete_num << endl;
        return;
    }

    node->expandChildren();
    cout << "children_num: " << node->children_node.size() << endl;

    int max_ev_num;
    int min_ev_num;

    for(auto itr = node->children_node.begin(); itr != node->children_node.end(); itr++) {
        cout << "n: " << n << endl;
        expandChildren_by_num(*itr, n - 1);

        if(itr == node->children_node.begin()) {
            max_ev_num = min_ev_num = (*itr)->evaluete_num;
            continue;
        }

        if((*itr)->evaluete_num > max_ev_num) {
            max_ev_num = (*itr)->evaluete_num;
        }
        if((*itr)->evaluete_num < min_ev_num) {
            min_ev_num = (*itr)->evaluete_num;
        }
    }

    if(node->my_k == node->current_board->k) {
        node->evaluete_num = max_ev_num;
    } else {
        node->evaluete_num = min_ev_num;
    }
}

void printTree(Game_Node *node, int n) {
    
    

    for(auto itr = node->children_node.begin(); itr != node->children_node.end(); itr++) {
        printTree(*itr, n - 1);
    }
}