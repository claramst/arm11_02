#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "commands.h"

#define EMPTY(s) (s[0] == '\0')

void setTokens(char *str, Editor *state) {
  char *s = malloc(state->MAX_LINE_LENGTH * sizeof(char));
  strcpy(s, str);
  char *token = strtok(s, " ");
  int len;
  for (len = 0; token != NULL; len++) {
	state->tokens[len] = token;
	token = strtok(NULL, " ");
  }
  state->noOfTokens = len;
}

void mainloop(Editor *state) {
  char *input = malloc(state->MAX_LINE_LENGTH * sizeof(char));
  Command cmd;

  commands functions[] = {&help, &quit, &about, &info, &clear, &write, &display, &run, &finish, &next, &currentState,
	  &stop, &load, &export, &delete, &none};

  while (state->running) {
	printf("%s%s", YELLOW, "➤ ");
	if (state->isRunning) printf("Running");
	printf("%s%s", "➤ ", CYAN);
	getInput(input, state->MAX_LINE_LENGTH);
	printf("%s", RESET);
	if (EMPTY(input))
	  continue;
	setTokens(input, state);
	cmd = getCommand(state->tokens[0]);
	functions[cmd](state);
  }
}

Editor *initialise_state(void) {
  Editor *state = malloc(sizeof(Editor));
  state->noOfLines = 0;
  state->tokens = malloc(5 * sizeof(char *));
  state->running = true;
  state->MAX_LINE_LENGTH = 100;
  state->MAX_LINES = 20;
  state->lines = calloc(state->MAX_LINES, sizeof(char *));
  for (int i = 0; i < state->MAX_LINES; i++) {
	state->lines[i] = malloc(state->MAX_LINE_LENGTH * sizeof(char));
  }
  state->currentLine = -1;
  state->source = calloc(MAX_PATH_LENGTH, sizeof(char));
  state->assembled = calloc(MAX_PATH_LENGTH, sizeof(char));
  state->machineState = initialiseState();
  state->fetched = calloc(1, sizeof(INSTRUCTION));
  state->decoded = calloc(1, sizeof(DECODED_INSTR));
  state->toDecode = calloc(1, sizeof(int));
  state->toExecute = calloc(1, sizeof(int));
  strcpy(state->source, "text.s");
  strcpy(state->assembled, "temp.bin");
  return state;
}

void freeState(Editor *state) {
  free(state->tokens);
  for (int i = 0; i < state->noOfLines; i++) {
	free(state->lines[i]);
  }
  free(state->lines);
  free(state->machineState->registers);
  free(state->machineState->memory);
  free(state->machineState);
  free(state);
}

int main(void) {
  Editor *state = initialise_state();
  welcome();
  mainloop(state);
  freeState(state);
  return 0;
}


