#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum { WID = 7, HGT = 6, LEN = 4, ARE = WID * HGT };
const char* FMT[] = {"\x1b[90m.\x1b[0m", "\x1b[92mX\x1b[0m",
                     "\x1b[91mO\x1b[0m"};
static uint64_t bit_at(size_t row, size_t col) {
  return 1ULL << row << col * (HGT + 1);
}
struct state {
  uint64_t msk;
  uint64_t cur;
  size_t siz;
};
static void state_init(struct state* sta) {
  sta->msk = 0;
  sta->cur = 0;
  sta->siz = 0;
}
static bool state_full(const struct state* sta) { return sta->siz >= ARE; }
static bool state_can_play(const struct state* sta, size_t col) {
  return col < WID && (sta->msk & bit_at(HGT - 1, col)) == 0;
}
static struct state* state_play(const struct state* sta, size_t col) {
  struct state* nxt = malloc(sizeof(struct state));
  state_init(nxt);
  nxt->msk = sta->msk | (sta->msk + bit_at(0, col));
  nxt->cur = sta->msk ^ sta->cur;
  nxt->siz = sta->siz + 1;
  return nxt;
}
static bool _state_win(const struct state* sta, size_t dir) {
  uint64_t msk = sta->msk ^ sta->cur;
  for (size_t sft = 1; sft < LEN; sft *= 2) {
    msk &= msk >> sft * dir;
  }
  return msk != 0;
}
static bool state_win(const struct state* sta) {
  return _state_win(sta, 1) || _state_win(sta, HGT) ||
         _state_win(sta, HGT + 1) || _state_win(sta, HGT + 2);
}
static bool state_first(const struct state* sta) { return sta->siz % 2 == 0; }
static void state_print(const struct state* sta) {
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
struct game {
  struct state* sta[ARE + 1];
  size_t siz;
};
static void game_init(struct game* gam) {
  memset(gam->sta, 0, sizeof(gam->sta));
  gam->sta[0] = malloc(sizeof(struct state));
  state_init(gam->sta[0]);
  gam->siz = 1;
}
static struct state* game_last(const struct game* gam) {
  return gam->sta[gam->siz - 1];
}
static bool game_full(const struct game* gam) {
  return state_full(game_last(gam));
}
static void _game_undo(struct game* gam) {
  free(game_last(gam));
  --gam->siz;
}
static void game_undo(struct game* gam, size_t n) {
  for (size_t i = 0; i < n && gam->siz > 1; ++i) {
    _game_undo(gam);
  }
}
static bool game_can_play(const struct game* gam, size_t col) {
  return state_can_play(game_last(gam), col);
}
static void game_play(struct game* gam, size_t col) {
  if (gam->siz >= ARE + 1) {
    return;
  }
  gam->sta[gam->siz] = state_play(game_last(gam), col);
  ++gam->siz;
}
static bool game_win(const struct game* gam) {
  return state_win(game_last(gam));
}
static bool game_first(const struct game* gam) {
  return state_first(game_last(gam));
}
static void game_print(const struct game* gam) { state_print(game_last(gam)); }
static void game_free(struct game* gam) {
  for (size_t i = 0; i < gam->siz; ++i) {
    free(gam->sta[i]);
  }
}
int main(void) {
  struct game gam;
  game_init(&gam);
  while (!game_full(&gam)) {
    size_t opt;
    game_print(&gam);
    scanf("%zu", &opt);
    if (!game_can_play(&gam, opt)) {
      game_undo(&gam, 2);
      continue;
    }
    game_play(&gam, opt);
    if (game_win(&gam)) {
      break;
    }
  }
  game_print(&gam);
  if (game_win(&gam)) {
    printf("%s won the game.\n", game_first(&gam) ? FMT[2] : FMT[1]);
  } else {
    printf("the game ends in draw.\n");
  }
  game_free(&gam);
  return 0;
}
