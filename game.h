#ifndef GAME_H
#define GAME_H

#include "state.h"

struct game {
  struct state* sta[ARE + 1];
  size_t siz;
};

void game_init(struct game* gam);

struct state* game_last(const struct game* gam);

bool game_full(const struct game* gam);

void game_undo(struct game* gam, size_t n);

bool game_can_play(const struct game* gam, size_t col);

void game_play(struct game* gam, size_t col);

bool game_win(const struct game* gam);

bool game_first(const struct game* gam);

void game_print(const struct game* gam);

void game_free(struct game* gam);

#endif
