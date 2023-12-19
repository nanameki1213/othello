#include <iostream>
#include <vector>
#include "GameTree.hpp"
#include <string>
using namespace std;

Game_Node::Game_Node(int k)
{
    my_k = k;
}

Game_Node::Game_Node(const Game_Node &node)
{
    my_k = node.my_k;
    ev_func = node.ev_func;
    pre_act = node.pre_act;
}

void Game_Node::expandChildren()
{
    // 既にchildren_nodeがいる場合
    if(!children_node.empty())
        return;

    vector<struct INPUT_DATA> act;

    current_board->get_legal_act(act);
    // for(auto itr = act.begin(); itr != act.end(); itr++) {
    //      cout << "(" << (*itr).x << ", " << (*itr).y << ")" << endl;
	// }
    // current_board->print_board();

    // 合法手の数だけノードを増やす
    for(auto itr = act.begin(); itr != act.end(); itr++) {
        Board *try_board = new Board(*current_board);
        Game_Node *child = new Game_Node(*this);

        try_board->k = current_board->k;

        // 行動の軌跡を残す
        child->pre_act = *itr;

        // curren_boardの設定
        try_board->change_board((*itr).x, (*itr).y);
		// 打ち手を交代
		try_board->k *= -1;
        child->current_board = try_board;
        // 親の設定
        child->parent_node = this;

        children_node.push_back(child);

		// cout << "expandChildren\n";
		// child->current_board->print_board();
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

// ゲーム木の深さをnにし，ミニマックス法で最善手を得る
void expandChildren_by_num(Game_Node *node, int n)
{
    // cout << "called expandbynum\n";
    if(n == 0) {
        node->evaluete_num = node->ev_func(node->current_board, node->my_k);
        // cout << "評価値: " << node->evaluete_num << endl;
        return;
    }

    node->expandChildren();
    // cout << "子供の座標を調べる" << endl;
    // for(auto itr = node->children_node.begin(); itr != node->children_node.end(); itr++) {
    //     cout << "(" << (*itr)->pre_act.x << ", " << (*itr)->pre_act.y << endl;
    // }
    // cout << "children_num: " << node->children_node.size() << endl;

    int max_ev_num = 0;
    int min_ev_num = 0;

    for(auto itr = node->children_node.begin(); itr != node->children_node.end(); itr++) {
        // cout << "n: " << n << endl;
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

void printTree(Game_Node *root, int n, int depth)
{
    if(n == -1) {
        cout << "root:" << root->evaluete_num << endl;
        for(auto itr = root->children_node.begin(); itr != root->children_node.end(); itr++) {
            printTree(*itr, n + 1);
        }
        return;
    }
    if(root->children_node.empty()) {
        for(int j = 0; j < n; j++) {
            cout << "| ";
        }
        cout << "|";
        string coord = "(" + to_string(root->pre_act.x) + "," + to_string(root->pre_act.y) + ")";
        cout << "-- " << root->evaluete_num << " " << coord << endl;
        return;
    }

    for(int j = 0; j < n; j++) {
        cout << "| ";
    }
    cout << "|";
    string coord = "(" + to_string(root->pre_act.x) + "," + to_string(root->pre_act.y) + ")";
    cout << "-- " << root->evaluete_num << " " << coord << endl;
    for(auto itr = root->children_node.begin(); itr != root->children_node.end(); itr++) {
        Game_Node *node = *itr;
        printTree(node, n + 1);
    }
}