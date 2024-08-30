#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Task 1 */
game_state_t *create_default_state() {
  game_state_t *game_state = (game_state_t*) malloc(sizeof(game_state_t));
  game_state->snakes = (snake_t*) malloc(sizeof(snake_t));
  *(game_state->snakes) = (snake_t){2, 2, 2, 4, 1};
  unsigned int num_rows = 18, num_cols = 20;
  game_state->board = (char**) malloc(num_rows * sizeof(char*));
  for (int i = 0; i < num_rows; i ++) {
    game_state->board[i] = (char*) malloc((num_cols + 1) * sizeof(char));
  }
  for (int i = 0; i < num_rows; i ++) {
    for (int j = 0; j < num_cols; j ++) {
      if (i == 0 || i == num_rows - 1 || j == 0 || j == num_cols - 1) {
        game_state->board[i][j] = '#';
      } else if (i == game_state->snakes[0].tail_row && j == game_state->snakes[0].tail_col) {
        game_state->board[i][j] = 'd';
      } else if (i == game_state->snakes[0].head_row && j == game_state->snakes[0].head_col) {
        game_state->board[i][j] = 'D';
      } else if (i <= game_state->snakes[0].head_row && i >= game_state->snakes[0].tail_row &&
                 j <= game_state->snakes[0].head_col && j >= game_state->snakes[0].tail_col) {
        game_state->board[i][j] = '>';
      } else if (i == 2 && j == 9) {
        game_state->board[i][j] = '*';
      } else {
        game_state->board[i][j] = ' ';
      }
    }
    // game_state->board[i][num_cols] = '\0';
  }
  game_state->num_rows = num_rows;
  game_state->num_cols = num_cols;
  game_state->num_snakes = 1;
  return game_state;
}

/* Task 2 */
void free_state(game_state_t *state) {
  for (int i = 0; i < state->num_rows; i ++) {
    free(state->board[i]);
  }
  free(state->board);
  free(state->snakes);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
  for (int i = 0; i < state->num_rows; i ++) {
    for (int j = 0; j < state->num_cols; j ++) {
      fprintf(fp, "%c", state->board[i][j]);
    }
    fprintf(fp, "\n");
  }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col) { return state->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  char op[4] = "wasd";
  for (int i = 0; i < 4; i ++) {
    if (c == op[i])
      return true;
  }
  return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  char op[4] = "WASD";
  for (int i = 0; i < 4; i ++) {
    if (c == op[i])
      return true;
  }
  return false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  char op[13] = "wasd^<v>WASDx";
  for (int i = 0; i < 4; i ++) {
    if (c == op[i])
      return true;
  }
  return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implement this function.
  return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  // TODO: Implement this function.
  return '?';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  if (c == 's' || c == 'S' || c == 'v') 
    cur_row ++;
  else if (c == 'w' || c == 'W' || c == '^')
    cur_row --;
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  if (c == 'd' || c == 'D' || c == '>') 
    cur_col ++;
  else if (c == 'a' || c == 'A' || c == '<')
    cur_col --;
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum) {
  snake_t snake = state->snakes[snum];
  unsigned int head_row = snake.head_row;
  unsigned int head_col = snake.head_col;
  char head = get_board_at(state, head_row, head_col);
  if (head == 'W') head_row --;
  if (head == 'S') head_row ++;
  if (head == 'A') head_col --;
  if (head == 'D') head_col ++;
  return get_board_at(state, head_row, head_col);
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum) {
  snake_t snake = state->snakes[snum];
  unsigned int head_row = snake.head_row;
  unsigned int head_col = snake.head_col;
  unsigned int nxt_head_row = head_row;
  unsigned int nxt_head_col = head_col;
  char head = get_board_at(state, head_row, head_col);
  char body = '\0';
  if (head == 'W') nxt_head_row --, body = '^';
  else if (head == 'S') nxt_head_row ++, body = 'v';
  else if (head == 'A') nxt_head_col --, body = '<';
  else nxt_head_col ++, body = '>';
  set_board_at(state, head_row, head_col, body);
  set_board_at(state, nxt_head_row, nxt_head_col, head);
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum) {
  snake_t snake = state->snakes[snum];
  unsigned int tail_row = snake.tail_row;
  unsigned int tail_col = snake.tail_col;
  char tail = get_board_at(state, tail_row, tail_col);
  set_board_at(state, tail_row, tail_col, ' ');
  if (tail == 'w') tail_row --;
  else if (tail == 's') tail_row ++;
  else if (tail == 'a') tail_col --;
  else tail_col ++;
  set_board_at(state, tail_row, tail_col, tail);
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state))
{
  unsigned int snum = state->num_snakes;
  for (unsigned int i = 0; i < snum; i++) {
    snake_t *snake = state->snakes + i;
    unsigned int head_row = snake->head_row;
    unsigned int head_col = snake->head_col;
    char next_step = next_square(state, i);
    if (is_snake(next_step)) {
      set_board_at(state, head_row, head_col, 'x');
      snake->live = false;
    }

    switch (next_step) {
      case ' ' : 
        update_head(state, i);
        update_tail(state, i);
        break;
      case '#' :
        set_board_at(state, head_row, head_col, 'x');
        snake->live = false;
        break;
      case '*' :
        update_head(state, i);
        add_food(state); // add a new food in the board
        break;
    }
  }
}

/* Task 5.1 */
char *read_line(FILE *fp) {
  // TODO: Implement this function.
  return NULL;
}

/* Task 5.2 */
game_state_t *load_board(FILE *fp) {
  // TODO: Implement this function.
  return NULL;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
  // TODO: Implement this function.
  return NULL;
}
