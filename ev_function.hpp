#pragma once

#include "Board.hpp"

int ev_score(Board *board, int my_k);
int ev_legal_act(Board *board, int my_k);
int ev_best(Board *board, int my_k);