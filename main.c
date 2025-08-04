#include <stdio.h>

#include "game.h"
#include "ui.h"

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
