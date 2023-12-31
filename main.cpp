#include <iostream>
#include "Board.hpp"
#include "GameTree.hpp"
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;
#define T 100

#define KEY_INPUT 0
#define KEY_WAIT 1
#define KEY_BACK 2
#define KEY_EXIT 3

int score[N][N] = {
    {2,2,2,2,2,2,2,2,2,2},
    {2,1000,5,100,10,10,100,5,1000,2},
    {2,5,1,15,20,20,15,1,5,2},
    {2,100,15,25,50,50,25,15,100,2},
    {2,10,20,50,200,200,50,20,10,2},
    {2,10,20,50,200,200,50,20,10,2},
    {2,100,15,25,50,50,25,15,100,2},
    {2,5,1,15,20,20,15,1,5,2},
    {2,1000,5,100,10,10,100,5,1000,2},
    {2,2,2,2,2,2,2,2,2,2}
};

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

int getch(void)
{
	struct termios oldattr, newattr;
	int ch;
	tcgetattr(STDIN_FILENO, &oldattr);
	newattr = oldattr;
	newattr.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
	return ch;
}

// キーボードからの入力を受け取り、対応するラベルを返却する
int input_key(Board *match, struct INPUT_DATA &data)
{

    struct INPUT_DATA cur = match->cur;

    do {
        bool is_decide = false;
        while(!is_decide) {
            switch(getch()) {
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
                case 'b':
                    if(match->turn == 0) {
                        cout << "このターンでは待ったできません" << endl;
                        break;
                    } else {
                        return KEY_WAIT;
                    }
                case 't':
                    return KEY_BACK;
                case 'x':
                    return KEY_EXIT;

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
            vector<struct INPUT_DATA> act;
            match->get_legal_act(act);
            cout << "legal act num:" << act.size() << endl;
            match->print_board();
            cout << "\nw,a,s,dでカーソルを移動\n待った!:b\n終了:x\n";

        }


        if(match->can_put(cur.x, cur.y)) {
            break;
        } else {
            cout << "(" << cur.x << "," << cur.y << ")には置けません\n";
        }

    }while(1);

    data.x = cur.x;
    data.y = cur.y;

    return KEY_INPUT;
}

struct INPUT_DATA get_max(Board *match)
{
    vector<struct INPUT_DATA> act;
    match->get_legal_act(act);
    int black,white;
    int current_num = match->get_current_num();

    vector<struct INPUT_DATA> max_coord_arr;
    int max_num;

    // 一度駒のmaxの値を取得する．
    for(auto itr = act.begin(); itr != act.end(); itr++) {
        Board *try_board = new Board(*match);
        try_board->change_board((*itr).x, (*itr).y);
        int try_num = try_board->get_current_num();

        if(itr == act.begin()) {
            max_num = try_num;
            max_coord_arr.push_back((*itr));
            continue;
        }
        cout << "現在のmax_numは" << max_num << endl; 
        // 最大値と一致した場合は配列に追加
        if(try_num == max_num) {
            cout << "重複" << endl;
            max_coord_arr.push_back((*itr));
        }
        
        if(try_num > max_num) {
            cout << "max_num更新:" << try_num << endl;
            max_num = try_num;
            if(!max_coord_arr.empty()) {
                max_coord_arr.clear();
                max_coord_arr.push_back((*itr));
            }
        }

        delete try_board;
    }

    int rand_num = rand()%max_coord_arr.size();

    return max_coord_arr[rand_num];
}

struct INPUT_DATA get_score_max(Board *match)
{
    vector<struct INPUT_DATA> act;
    match->get_legal_act(act);
    
    int max = score[act[0].y][act[0].x];
    struct INPUT_DATA max_coord = act[0];
    for(auto itr = act.begin() + 1; itr != act.end(); itr++) {
        if(score[(*itr).y][(*itr).x] > max) {
            max = score[(*itr).y][(*itr).x];
            max_coord = *itr;
        }
    }

    return max_coord;
}

void print_vs(unsigned char flag, bool is_first_cur)
{
    cout << "先攻       後攻\n";
    if(is_first_cur == true) {
        switch(flag) {
            case 0:
                cout << "\x1b[48;5;242m人間\x1b[49m  vs   人間\n";
                break;
            case 1:
                cout << "\x1b[48;5;242m AI \x1b[49m  vs   人間\n";
                break;
            case 2:
                cout << "\x1b[48;5;242m人間\x1b[49m  vs   AI\n";
                break;
            case 3:
                cout << "\x1b[48;5;242m AI \x1b[49m  vs    AI\n";
        }
    } else {
        switch(flag) {
            case 0:
                cout << "人間  vs   \x1b[48;5;242m人間\x1b[49m\n";
                break;
            case 1:
                cout << " AI   vs   \x1b[48;5;242m人間\x1b[49m\n";
                break;
            case 2:
                cout << "人間  vs   \x1b[48;5;242m AI \x1b[49m\n";
                break;
            case 3:
                cout << " AI   vs   \x1b[48;5;242m AI \x1b[49m\n";
        }
    }
}

unsigned char edit_vs(unsigned char flag)
{
    bool is_first_cur = true;
    bool is_decide = false;
    do {
        system("clear");
        print_vs(flag, is_first_cur);
        switch(getch()) {
            case 'a':
                is_first_cur = true;
                break;
            case 'd':
                is_first_cur = false;
                break;
            case 'w':
            case 's':
                if(is_first_cur) {
                    if(flag & 1U)
                        flag &= ~1U;
                    else
                        flag |= 1U;
                } else {
                    if(flag & 2U)
                        flag &= ~2U;
                    else
                        flag |= 2U;
                }
                break;
            case '\n':
                is_decide = true;
                break;
        }
    } while(is_decide == false);

    return flag;
}

int ev_score(Board *board, int my_k)
{
    int ev_num;
    if(board->k == my_k) {
        ev_num = board->get_current_num();
    } else {
        Board *my_board = new Board(*board);
        my_board->k = board->k * -1;
        ev_num = my_board->get_current_num();
    }

    return ev_num;
}

int ev_best(Board *board, int my_k)
{
    Board *opp = new Board(*board);
    opp->k = board->k * -1;

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
    
}

int main(void)
{
    srand((unsigned)time(NULL));

    int k;
    struct INPUT_DATA input_data;

    Board match;
    vector<Board> log;

    k = BLACK;
    match.k = k;
    bool is_wait = false;
    bool is_algorithm_enable = false;
    int algorithm_k = BLACK;
    unsigned char flag = edit_vs(0);

    switch(flag) {
        case 0:
            is_algorithm_enable = false;
            break;
        case 1:
            is_algorithm_enable = true;
            algorithm_k = BLACK;
            break;
        case 2:
            is_algorithm_enable = true;
            algorithm_k = WHITE;
            break;
        case 3: 
            cout << "たいおうしてません\n";
            break;
    }

    // do {
    //     cout << "探索アルゴリズムを実行しますか?(y/n):";
    //     char c = getchar();
    //     if(c == 'y' || c == 'Y') {
    //         is_algorithm_enable = true;
    //         break;
    //     } else if(c == 'n' || c == 'N') {
    //         is_algorithm_enable = false;
    //         break;
    //     }
    // } while(1);

    // if(is_algorithm_enable) {
    //     do {
    //         cout << "探索を実行する打ち手を選択(先攻:1,後攻:2):";
    //         int num;
    //         cin >> num;
    //         if(num == 1) {
    //             algorithm_k = BLACK;
    //             break;
    //         } else if(num == 2) {
    //             algorithm_k = WHITE;
    //             break;
    //         }
    //     } while(1);
    // }

    // for(int i = 0; i < 100; i++) {
    while (!match.check_finish()) {
    // while (1) {
        system("clear");

        // boardに変更を加える前にログをとる
        if(is_wait) { // 待った!されたらログを書き換える
            cout << "一手戻る\n";
            log.pop_back();
            is_wait = false;
            cout << log.size() << endl;
        } else {
            log.push_back(match);
        }
        match.print_board();

        if (!match.check_pass()) {

            // アルゴリズムを使用しない，または，アルゴリズムを使用するが該当する打ち手ではないとき
            if(is_algorithm_enable && algorithm_k == match.k) {
                // ランダムアルゴリズム
                // vector<struct INPUT_DATA> act;
                // match.get_legal_act(act);
                // int act_num = rand()%act.size();

                // match.change_board(act[act_num].x, act[act_num].y);

                // 最大値を得るアルゴリズム
                // struct INPUT_DATA max = get_max(&match);
                // match.change_board(max.x, max.y);

                // アルファベータ法
                Game_Node *node = new Game_Node(algorithm_k);
                node->current_board = &match;

                vector<INPUT_DATA> act;
                match.get_legal_act(act);

                for(auto itr = act.begin(); itr != act.end(); itr++) {
                    cout << "(" << (*itr).x << ", " << (*itr).y << ")" << endl;
                }

                node->ev_func = ev_best;

                expandChildren_by_num(node, 8);

                // printTree(node);

                int max_ev_num;
                INPUT_DATA best_act;
                for(auto itr = node->children_node.begin(); itr != node->children_node.end(); itr++) {
                    if(itr == node->children_node.begin()) {
                        max_ev_num = (*itr)->evaluete_num;
                        best_act = (*itr)->pre_act;
                        continue;
                    }
                    if((*itr)->evaluete_num > max_ev_num) {
                        max_ev_num = (*itr)->evaluete_num;
                        best_act = (*itr)->pre_act;
                    }
                }
                cout << "評価値:" << max_ev_num << endl;
                cout << "(" << best_act.x << ", " << best_act.y << ")" << endl;
                match.change_board(best_act.x, best_act.y);
                // getchar();
            } else {
                int key = input_key(&match, input_data);
                switch(key) {
                    case KEY_INPUT:
                        match.change_board(input_data.x, input_data.y);
                        break;
                    case KEY_WAIT:
                        if(match.turn == 0) { // ターン1では待った!できない
                            cout << "このターンでは待ったできません" << endl;
                            break;
                        }
                        // 1つ前のターンの盤面を現在の盤面にコピー
                        for(int i = 0; i < N; i++) {
                            for(int j = 0; j < N; j++) {
                                match.board[i][j] = log[match.turn - 1].board[i][j];
                            }
                        }
                        match.turn -= 2;
                        is_wait = true;
                        break;
                    case KEY_BACK:
                        break;
                    case KEY_EXIT:
                        // cout << "ログを表示\n";
                        // for(auto itr = log.begin(); itr != log.end(); itr++) {
                        //     (*itr).print_board();
                        // }
                        return 0;
                }

            }
        } else {
            cout << "パス!\n";
        }
        match.k *= -1;
        match.turn++;
    }

    cout << "終了\n";

    int black_num = 0, white_num = 0;

    for(int i = 1; i < N - 1; i++) {
        for(int j = 0; j < N - 1; j++) {
            if(match.board[i][j] == WHITE)
                white_num++;
            else if(match.board[i][j] == BLACK)
                black_num++;
        }
    }

    cout << "黒のコマ数: " << black_num << endl;
    cout << "白のコマ数: " << white_num << endl;

    // // 最終盤面のログをとる
    // log.push_back(match);

    // for(auto itr = log.begin(); itr != log.end(); itr++) {
    //     (*itr).print_board();
    // }

    return 0;
}