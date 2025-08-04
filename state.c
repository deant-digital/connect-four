#include "state.h"

#include <stdio.h>
#include <stdlib.h>

#include "ui.h"

static uint64_t bit_at(size_t row, size_t col) {
  return 1ULL << row << col * (HGT + 1);
}

static bool _state_win(const struct state* sta, size_t dir) {
  uint64_t msk = sta->msk ^ sta->cur;
  for (size_t sft = 1; sft < LEN; sft *= 2) {
    msk &= msk >> sft * dir;
  }
  return msk != 0;
}

void state_init(struct state* sta) {
  sta->msk = 0;
  sta->cur = 0;
  sta->siz = 0;
}

bool state_full(const struct state* sta) { return sta->siz >= ARE; }

bool state_can_play(const struct state* sta, size_t col) {
  return col < WID && (sta->msk & bit_at(HGT - 1, col)) == 0;
}

struct state* state_play(const struct state* sta, size_t col) {
  struct state* nxt = malloc(sizeof(struct state));
  state_init(nxt);
  nxt->msk = sta->msk | (sta->msk + bit_at(0, col));
  nxt->cur = sta->msk ^ sta->cur;
  nxt->siz = sta->siz + 1;
  return nxt;
}

bool state_win(const struct state* sta) {
  return _state_win(sta, 1) || _state_win(sta, HGT) ||
         _state_win(sta, HGT + 1) || _state_win(sta, HGT + 2);
}

bool state_first(const struct state* sta) { return sta->siz % 2 == 0; }

void state_print(const struct state* sta) {
  bool b = state_first(sta);
  for (size_t row = HGT; row-- > 0;) {
    for (size_t col = 0; col < WID; ++col) {
      if ((sta->msk & bit_at(row, col)) == 0) {
        printf(" %s ", FMT[0]);
      } else {
        if ((sta->cur & bit_at(row, col)) != 0) {
          printf(" %s ", b ? FMT[1] : FMT[2]);
        } else {
          printf(" %s ", b ? FMT[2] : FMT[1]);
        }
      }
    }
    putchar('\n');
  }
}
