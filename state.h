#ifndef STATE_H
#define STATE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum { WID = 7, HGT = 6, LEN = 4, ARE = WID * HGT };

struct state {
  uint64_t msk;
  uint64_t cur;
  size_t siz;
};

void state_init(struct state* sta);

bool state_full(const struct state* sta);

bool state_can_play(const struct state* sta, size_t col);

struct state* state_play(const struct state* sta, size_t col);

bool state_win(const struct state* sta);

bool state_first(const struct state* sta);

void state_print(const struct state* sta);

#endif
