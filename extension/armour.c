#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "commands.h"

#define EMPTY(s) (s[0] == '\0')

static void set_tokens(char *str, Editor *state) {
  char *s = malloc(state->MAX_LINE_LENGTH * sizeof(char));
  strcpy(s, str); 
  char *token = strtok(s, " ");
  int len;
  for (len = 0; token != NULL; len++) {
	state->tokens[len] = strdup(token);
	token = strtok(NULL, " ");
  }
  state->noOfTokens = len;
  free(s);
}

static void mainloop(Editor *state) {
  char *input = calloc(state->MAX_LINE_LENGTH, sizeof(char));
  Command cmd;

  commands functions[] = {&help, &quit, &about, &info, &clear, &write, &display, &run, &finish, &next, &current_state,
			  &stop, &load, &export, &delete, &insert, &continue_break, &set_break, &disable_break, &append, &none};

  while (state->running) {
    printf("%s%s", YELLOW, "➤");

    if (state->isRunning) 
      printf("Running");

    printf("%s%s", "➤ ", CYAN);
    get_input(input, state->MAX_LINE_LENGTH);
    printf("%s", RESET);

    if (EMPTY(input))
      continue;
    
    set_tokens(input, state);
    cmd = get_command(state->tokens[0]);
    functions[cmd](state);
  }
  free(input);
}

/**
 * Initialises an instance of our Editor struct
 */
Editor *initialise_editor(void) {
  Editor *state = malloc(sizeof(Editor));
  state->noOfLines = 0;
  state->tokens = malloc(5 * sizeof(char *));
  state->running = true;
  state->MAX_LINE_LENGTH = 150;
  state->MAX_LINES = 10;
  state->lines = malloc(state->MAX_LINES * sizeof(char *));
  for (int i = 0; i < state->MAX_LINES; i++) {
    state->lines[i] = calloc(state->MAX_LINE_LENGTH, sizeof(char));
  }
  state->currentLine = -1;
  state->source = calloc(MAX_PATH_LENGTH, sizeof(char));
  state->assembled = calloc(MAX_PATH_LENGTH, sizeof(char));
  state->machineState = initialise_state();
  state->fetched = calloc(1, sizeof(INSTRUCTION));
  state->decoded = calloc(1, sizeof(DECODED_INSTR));
  state->toDecode = calloc(1, sizeof(int));
  state->toExecute = calloc(1, sizeof(int));
  state->isRunning = 0;
  strcpy(state->source, "text.s");
  strcpy(state->assembled, "temp.bin");
  state->nextLocation = true;
  state->CYCLES_LIMIT = 4000;

  return state;
}


void free_state(Editor *state) {
  for (int i = 0; i < state->noOfTokens; i++) {
    free(state->tokens[i]);
  }
  free(state->tokens);
  for (int i = 0; i < state->MAX_LINES; i++) {
	free(state->lines[i]);
  }
  free(state->source);
  free(state->assembled);
  free(state->fetched);
  free(state->decoded);
  free(state->toDecode);
  free(state->toExecute);
  if (state->breakpoints) {
    free(state->breakpoints);
  }
  free(state->lines);
  free(state->machineState->registers);
  free(state->machineState->memory);
  free(state->machineState);
  free(state);
}

int main(void) {
  Editor *state = initialise_editor();
  welcome();
  mainloop(state);
  free_state(state);

  return EXIT_SUCCESS;
}


