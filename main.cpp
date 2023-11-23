#include <iostream>
#include "Board.hpp"
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <vector>

using namespace std;
#define T 100

#define KEY_INPUT 0
#define KEY_WAIT 1
#define KEY_BACK 2
#define KEY_EXIT 3

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

    struct INPUT_DATA max_coord = act[0];
    int max_num = 0;

    for(auto itr = act.begin(); itr != act.end(); itr++) {
        Board *try_board = new Board(*match);
        try_board->change_board((*itr).x, (*itr).y);
        int try_num = try_board->get_current_num();
        if(try_board->get_current_num() - current_num > max_num) {
            max_coord = *itr;
            max_num = try_board->get_current_num() - current_num;
        }
    }
    return max_coord;
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
    bool is_algorithm_first_enable = false;

    do {
        cout << "探索アルゴリズムを実行しますか?(y/n):";
        char c = getchar();
        if(c == 'y' || c == 'Y') {
            is_algorithm_enable = true;
            break;
        } else if(c == 'n' || c == 'N') {
            is_algorithm_enable = false;
            break;
        }
    } while(1);

    if(is_algorithm_enable) {
        do {
            cout << "探索を実行する打ち手を選択(先攻:1,後攻:2):";
            int num;
            cin >> num;
            if(num == 1) {
                is_algorithm_first_enable = false;
                break;
            } else if(num == 2) {
                is_algorithm_first_enable = true;
                break;
            }
        } while(1);
    }

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
            if(!is_algorithm_enable || 
              (is_algorithm_enable && ((is_algorithm_first_enable && match.k != WHITE) || (!is_algorithm_first_enable && match.k != BLACK)))) {
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
            } else if(is_algorithm_first_enable && match.k == WHITE || !is_algorithm_first_enable && match.k == BLACK) {
                // vector<struct INPUT_DATA> act;
                // match.get_legal_act(act);
                // int act_num = rand()%act.size();

                // match.change_board(act[act_num].x, act[act_num].y);
                struct INPUT_DATA max = get_max(&match);
                match.change_board(max.x, max.y);

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