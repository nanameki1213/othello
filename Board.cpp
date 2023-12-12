#include "Board.hpp"
#include <math.h>
#include <string.h>
#include <vector>
using namespace std;

#define PI 3.14

Board::Board()
{
    cur.x = 1;
    cur.y = 1;

    k = BLACK;

    board = new int*[N];
    for(int i = 0; i < N; i++) {
        board[i] = new int[N];
    }

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(i == 0 || j == 0 || i == 9 || j == 9) {
                board[i][j] = OUT_OF_RANGE;
                continue;
            } else {
                board[i][j] = NONE;
            }
        }
    }
    board[4][4] = board[5][5] = WHITE;
    board[4][5] = board[5][4] = BLACK;

    turn = 0;
}

Board::Board(const Board &b)
{
    board = new int*[N]();
    for(int i = 0; i < N; i++) {
        board[i] = new int[N]();
    }
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            board[i][j] = b.board[i][j];
        }
    }

    cur.x = b.cur.x;
    cur.y = b.cur.y;
    turn = b.turn;
    k = b.k;
}

void Board::print_board()
{
    cout << "turn: " << turn << endl;
    printf("\n現在のターンは%sです\n\n", k == BLACK ? "〇" : "●");

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(can_put(j, i)) {
                cout << "\x1b[43m";
            }
            if(cur.x == j && cur.y == i) {
                cout << "\x1b[48;5;242m";
            }


            switch(board[i][j]) {
                case NONE:
                    cout << "--"; break;
                case WHITE:
                    cout << "●"; break;
                case BLACK:
                    cout << "〇"; break;
                case OUT_OF_RANGE:
                    cout << "■";
                    break;
                default:
                    ;
            }
            cout << "\x1b[49m";
            cout << "\x1b[39m";
        }
        cout << endl;
    }

    cout << "\nCursor:(" << cur.x << "," << cur.y << ")\n";
}

int Board::get_current_num(int target_k)
{
    int num = 0;
    for(int i = 1; i < N - 1; i++) {
        for(int j = 1; j < N - 1; j++) {
            if(board[i][j] == target_k)
                num++;
        }
    }

    return num;
}

// 盤面が終了状態ならtrueを返す
bool Board::check_finish()
{
  Board *check = new Board(*this);

  bool is_white_pass = false, is_black_pass = false;

  check->k = WHITE;

  if(check->check_pass())
    is_white_pass = true;
  
  check->k = BLACK;

  if(check->check_pass())
    is_black_pass = true;

  delete check;

  if(is_black_pass && is_white_pass)
    return true;

  return false;
}

// パスならtrueを返す
bool Board::check_pass()
{
  for(int i = 1; i < N - 1; i++) {
    for(int j = 1; j < N - 1; j++) {
      if(can_put(j, i)) {
        return false;
      }
    }
  }

  return true;
}

void Board::change_board(int x, int y)
{
    board[y][x] = k;

    int mx, my;

    int opp = k * -1;

    for(int i = 0; i < N -2; i++) {
        change_in_row(x, y, i);
    }
}

void Board::change_in_row(int x, int y, int direc)
{
    int opp = k * -1;
    int mx, my;
    bool check = false;

    mx = x + round(cos(PI/4 * direc));
    my = y + round(sin(PI/4 * direc));

    if(board[my][mx] == OUT_OF_RANGE) {
        return;
    }
    // printf("mx:%d, my:%d\n", mx, my);

    while(board[my][mx] == opp) {
        check = true;

        mx += round(cos(PI/4 * direc));
        my += round(sin(PI/4 * direc));

        if(board[my][mx] == OUT_OF_RANGE)
            return;

        // printf("mx:%d, my:%d\n", mx, my);
    }

    if(board[my][mx] != k || !check) {
        return;    
    }

    mx = x + round(cos(PI/4 * direc));
    my = y + round(sin(PI/4 * direc));

    while(board[my][mx] == opp) {
        board[my][mx] = k;
        
        mx += round(cos(PI/4 * direc));
        my += round(sin(PI/4 * direc));
    }
}

bool Board::can_put(int x, int y)
{
    // 配置するマスに何も置かれていないことの確認
    if(board[y][x] != NONE)
        return false;

    for(int i = 0; i < N - 2; i++) {
        if(check_change(x, y, i)) {
            return true;
        }
    }
    
    return false;
}

bool Board::check_change(int x, int y, int direc)
{
    int opp = k * -1;
    int mx, my;
    bool check = false;

    mx = x + round(cos(PI/4 * direc));
    my = y + round(sin(PI/4 * direc));

    if(board[my][mx] == OUT_OF_RANGE) {
        return false;
    }
    // printf("mx:%d, my:%d\n", mx, my);

    while(board[my][mx] == opp) {
        check = true;

        mx += round(cos(PI/4 * direc));
        my += round(sin(PI/4 * direc));

        if(board[my][mx] == OUT_OF_RANGE)
            return false;

        // printf("mx:%d, my:%d\n", mx, my);
    }

    if(board[my][mx] == k && check) {
        return true;
    }

    return false;
}

void Board::get_legal_act(vector<struct INPUT_DATA> &act)
{
    if(!act.empty()) {
        cout << "空のデータを渡してください";
        return;
    }

    for(int i = 1; i < N - 1; i++) {
        for(int j = 1; j < N - 1; j++) {
            if(can_put(j, i)) {
                struct INPUT_DATA legal_act = {j, i};
                act.push_back(legal_act);
            }
        }
    }
}