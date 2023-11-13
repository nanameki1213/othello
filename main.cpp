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
                    return KEY_WAIT;
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

int main(void)
{
    int k;
    struct INPUT_DATA input_data;

    Board match;
    vector<Board> log;

    k = 1;
    match.k = k;
    bool is_wait = false;
    bool is_acceptable_input = false;

    // for(int i = 0; i < 100; i++) {
    while (match.check_finish() != 0) {
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

        if (match.check_pass() != 0) {
            do {
                is_acceptable_input = true;
                int key = input_key(&match, input_data);
                switch(key) {
                    case KEY_INPUT:
                        match.change_board(input_data.x, input_data.y);
                        break;
                    case KEY_WAIT:
                        if(match.turn == 0) { // ターン1では待った!できない
                            cout << "このターンでは待ったできません" << endl;
                            is_acceptable_input = false;
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
                        cout << "ログを表示\n";
                        for(auto itr = log.begin(); itr != log.end(); itr++) {
                            (*itr).print_board();
                        }
                        return 0;
                }
            }while(is_acceptable_input == false);
        }
        match.k *= -1;
        match.turn++;
    }

    for(auto itr = log.begin(); itr != log.end(); itr++) {
        (*itr).print_board();
    }

    return 0;
}