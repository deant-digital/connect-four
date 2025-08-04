#include "game.h"

#include <stdlib.h>
#include <string.h>

static void _game_undo(struct game* gam) {
  free(game_last(gam));
  --gam->siz;
}

void game_init(struct game* gam) {
  memset(gam->sta, 0, sizeof(gam->sta));
  gam->sta[0] = malloc(sizeof(struct state));
  state_init(gam->sta[0]);
  gam->siz = 1;
}

struct state* game_last(const struct game* gam) {
  return gam->sta[gam->siz - 1];
}

bool game_full(const struct game* gam) { return state_full(game_last(gam)); }

void game_undo(struct game* gam, size_t n) {
  for (size_t i = 0; i < n && gam->siz > 1; ++i) {
    _game_undo(gam);
  }
}

bool game_can_play(const struct game* gam, size_t col) {
  return state_can_play(game_last(gam), col);
}

void game_play(struct game* gam, size_t col) {
  if (gam->siz >= ARE + 1) {
    return;
  }
  gam->sta[gam->siz] = state_play(game_last(gam), col);
  ++gam->siz;
}

bool game_win(const struct game* gam) { return state_win(game_last(gam)); }

bool game_first(const struct game* gam) { return state_first(game_last(gam)); }

void game_print(const struct game* gam) { state_print(game_last(gam)); }

void game_free(struct game* gam) {
  for (size_t i = 0; i < gam->siz; ++i) {
    free(gam->sta[i]);
  }
}
