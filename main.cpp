#include <stdio.h>
#include <stdbool.h>
#define N 10
#define T 100

struct INPUT_DATA {
    int x, y;
};

typedef struct LOG LOG;

struct LOG {
    LOG *next;
    LOG *prev;
    int **board;
};

void make_board(int board[][N])
{
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(i == 0 || j == 0 || i == 9 || j == 9) {
                board[i][j] = 2;
                continue;
            } else {
                board[i][j] = 0;
            }
        }
    }
    board[4][4] = board[5][5] = 1;
    board[4][5] = board[5][4] = -1;
}

void print_board(int board[][N])
{
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            switch(board[i][j]) {
                case 0:
                    printf("--"); break;
                case 1:
                    printf("●"); break;
                case -1:
                    printf("★"); break;
                case 2:
                default:
            }
        }
        printf("\n");
    }
}

int check_finish(int t, int board[][N])
{
    return 0;
}

int check_pass(int k, int board[][N])
{
    return 0;
}

struct INPUT_DATA input_key(int k, int board[][N])
{
    int x, y;
    struct INPUT_DATA data;

    printf("手を打つ場所を決めてください:\n");
    printf("x:"); scanf("%d", &x);
    printf("y:"); scanf("%d", &y);

    data.x = x;
    data.y = y;

    return data;

}

void change_board(int k, int x, int y, int board[][N])
{

}

int main(void)
{
    int board[N][N] = { 0 };
    int t;
    int k;
    struct INPUT_DATA input_data;

    make_board(board);
    print_board(board);

    t = 0;
    k = 1;
    while (check_finish(t, board) != 0) {
        t++;
        if (check_pass(k, board) != 0) {
            input_data = input_key(k, board);
            change_board(k, input_data.x, input_data.y, board);
            print_board(board);
        }
        k = k * -1;
    }
}
