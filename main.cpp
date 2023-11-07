#include <iostream>
#include "Board.hpp"
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <vector>

using namespace std;
#define T 100

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

struct INPUT_DATA input_key(Board *match)
{
    struct INPUT_DATA data;

    struct INPUT_DATA cur = match->cur;

    char c;
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

    }

    data.x = cur.x;
    data.y = cur.y;

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
        system("clear");
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
