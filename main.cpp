#include <iostream>
#include "Board.hpp"
#include <termio.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;
#define T 100

int check_finish(int t, int board[][N])
{
    return 0;
}

int check_pass(int k, int board[][N])
{
    return 0;
}

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

    k = 1;
    while (match.check_finish() != 0) {
        match.print_board();
        match.k = k;
        match.turn++;
        if (match.check_pass() != 0) {
            input_data = input_key(&match);

            if(match.can_put(input_data.x, input_data.y))
                match.board[input_data.y][input_data.x] = k;
        }
        match.k *= -1;
    }

    return 0;
}
