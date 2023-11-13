#include <iostream>
#include "Board.hpp"
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <math.h>

using namespace std;
#define T 100

#define KEY_INPUT 0
#define KEY_WAIT 1
#define KEY_BACK 2
#define KEY_EXIT 3

typedef struct LOG LOG;

struct LOG {
    LOG *next;
    LOG *prev;
    int **board;
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
    // テスト用盤面作成
    int x = 4, y = 4;
    int mx, my;
    for(int direc = 0; direc < DIRECTION_MAX; direc++) {

        int **test_board = new int*[N];
        for(int i = 0; i < N; i++) {
            test_board[i] = new int[N];
        }

        Board tmp;

        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                if(i == 0 || j == 0 || i == 9 || j == 9) {
                    test_board[i][j] = OUT_OF_RANGE;
                    continue;
                } else {
                    test_board[i][j] = NONE;
                }
            }
        }

        mx = x;
        my = y;
        while(tmp.is_legal_coord(mx + round(cos(PI/4*direc)), my + round(sin(PI/4*direc)))) {
            mx += round(cos(PI/4 * direc));
            my += round(sin(PI/4 * direc));
            test_board[my][mx] = BLACK;
        }
        test_board[my][mx] = WHITE;

        Board match(test_board);
        match.k = WHITE;

        match.print_board();

        if(match.can_put(x, y)) {
            cout << "おけます\n";
        }

        for(int i = 0; i < N; i++) {
            delete test_board[i];
        }
        delete test_board;
    }



    return 0;
}