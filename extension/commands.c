#include <stdio.h>
#include <stdlib.h>
#include "commands.h"
#include <stdbool.h>


Command getCommand(char *str);

char *RED = "\033[1;31m";
char *YELLOW = "\033[1;33m";
char *BLUE = "\033\[0;34m";
char *RESET = "\033\033[0m";
char *GREEN = "\033\[0;32m";
char *CYAN = "\033\[0;36m";

void getinput(char *input, int MAX_LINE_LENGTH) {
  fgets(input, MAX_LINE_LENGTH, stdin);
  input[strlen(input) - 1] = '\0';
}

Command getCommand(char *str) {
  if (SAME(str, "help") || SAME(str, "h")) {
	return HELP;
  } else if (SAME(str, "quit") || SAME(str, "q")) {
	return QUIT;
  } else if (SAME(str, "about") || SAME(str, "a")) {
    return ABOUT;
  } else if (SAME(str, "info") || SAME(str, "i")) {
	return INFO;
  } else if (SAME(str, "write") || SAME(str, "w")) {
	return WRITE;
  } else if (SAME(str, "clear") || SAME(str, "c")) {
	return CLEAR;
  } else if (SAME(str, "display") || SAME(str, "d")) {
	return DISPLAY;
  } else if (SAME(str, "next") || SAME(str, "+")) {
	return NEXT;
  } else if (SAME(str, "save") || SAME(str, "s")) {
	return SAVE;
  } else if (SAME(str, "run") || SAME(str, "r")) {
	return RUN;
  } else if (SAME(str, "state") || SAME(str, "st")) {
	return STATE;
  } else if (SAME(str, "stop") || SAME(str, "X")) {
	return STOP;
  }
  return NONE;
}

void help(Editor *state) {
  printf("--COMMANDS--\n");
  printf("%s", CYAN);
  printf("| %s |", "help");
  printf("| %s |", "quit");
  printf("| %s |", "info");
  printf("| %s |", "about");
  printf("| %s |", "clear");
  printf("| %s |", "display");
  printf("| %s |", "write");
  printf("| %s |", "save");
  printf("| %s ", "run\n");
  printf("========= Running mode commands: =========\n");
  printf("| %s |", "next");
  printf("| %s |", "state");
  printf("| %s |", "stop");
  printf("\n");
  printf("%s", RESET);
  printf("Type \"info <command>\" to learn more about what it does.\n");
  printf("Type \"options\" after a command when unsure about which arguments to give.\n");
}

void quit(Editor *state) {
  printf("Hope you liked it...\n");
  state->running = false;
}

void about(Editor *state) {
  printf("ARMOUR is a tool for writing and executing small assembly programs using ARM11 architecture\n");
  printf("Made as part of an extension to an emulator and assembler for Rasberrry Pi.\n");
}


void print_lines(Editor *state, int start, int end, bool lineNumbers) {
  char *spaces;
  for (int i = start; i < end; i++) {
	if (i == state->currentLine) {
	  printf("=>");
	}
	if (lineNumbers) { printf(" %d| ", i + 1); }
	if (i + 1 >= 10) {
	  spaces = " ";
	} else
	  spaces = "  ";

	printf("%s\n", state->lines[i]);
  }
}

void display(Editor *state) {
  if (state->noOfLines == 0){
	printf("Nothing has been written yet. Type \"write\" to start editing.\n");
	return;
  }
  int start, end;
  switch (state->noOfTokens) {
	case 1:
	  start = 0;
	  end = state->noOfLines;
	  break;
	case 2:
	  if (SAME(state->tokens[1], "options")) {
		printf("display <start> <end> \n prints lines corresponding to line numbers between <start> and <end> inclusive. If either are unspecified then the lines from that number onwards are printed.");
		return;
	  } else {
		start = atoi(state->tokens[1]) - 1;
		end = state->noOfLines;
	  }
	  break;
	case 3:
	  start = atoi(state->tokens[1]) - 1;
	  end = atoi(state->tokens[2]);
	  break;
	default:
	  printf("Too many aguments");
	  return;
  }

  if (start < 0 || start > state->noOfLines + 1 || end > state->MAX_LINES || end < start) {
	printf("Invalid line number(s)\n");
	return;
  }
  bool lineNumbers = true;
  print_lines(state, start, end, lineNumbers);
}

char *trim(char *str) {
  int i;
  for (i = 0; str[i] != '/' && i < strlen(str); i++);
  char *str2 = calloc((i + 2), sizeof(char));
  CHECK_PRED(!str2, "Allocating memory failed");
  strncpy(str2, str, i);
  str2[i + 1] = '\0';
  return str2;
}


void write(Editor *state) {
  if (state->isRunning) {
	printf("You can't write while you're running!\n");
	return;
  }
  int start, end;
  if (state->noOfTokens > 1) {
	if (SAME(state->tokens[1], "options")) {
	  printf(
		  "write <START (= 1)> \nYou can start writing from line START.\n NOTE: writing on a line will overwrite what was previous there.\n");
	  return;
	} else if (SAME(state->tokens[1], "end")) {
	  start = state->noOfLines;
	} else {
	  start = atoi(state->tokens[1]);
	  start--;
	}
  } else
	start = 0;

  end = state->MAX_LINES;
  if (start < 0 || start > state->noOfLines + 1) {
	printf("Invalid start");
	return;
  }

  int i;
  char *spaces;
  for (i = start; i < end; i++) {
	char *instr = calloc(state->MAX_LINE_LENGTH, sizeof(char));
	if (i + 1 >= 10) {
	  spaces = " ";
	} else
	  spaces = "  ";

	printf("%s%d| ", spaces, i + 1);
	getinput(instr, state->MAX_LINE_LENGTH);

	if (SAME(instr, "exit")) { break; }
	if (!validInstr(trim(instr))) {
	  printf("%sFailed to write line due to syntax error, try again.%s\n", RED, RESET);
	  i--; continue;
	}
	strcpy(state->lines[i], instr);
  }

  if (i >= state->noOfLines) {
	state->noOfLines = i;
  }
}

void runAll(Editor *state) {
  for (int i = 0; i < state->noOfLines; i++)
	pipelineCycle(state->machineState, state->fetched, state->decoded, state->toDecode, state->toExecute);
  printf("End of program reached. Final machine state:\n");
  currentState(state);
  stop(state);
}

// Saves the current file, assembles this file.
// Need to save assembled file.
// Assemble takes in two arguments. The file
// to read from (which we should get from save)
// and the file to write to.
// Then we pass this file into emulate, which prints the machine state.
// PRE: assemble and emulate should be compiled
// into executeables
// Need absolutePath to file
void run(Editor *state) {
  if (state->isRunning) {
	printf("You're already in run mode!\n");
	return;
  }
  if (!*state->path) {
    printf("You haven't saved a file yet.\n");
    return;
  }
  printf("%s", RED);
  printf("Entering run mode.");
  printf("%s", RESET);
  printf("\nPossible commands:\n next\n state\n halt\n display\n run all\n Type \"info <command>\" to learn more about what it does. \n");
  state->currentLine = 0;
  char *assembledBinary = "temp";
  // a temporary file for assemble to write to. idk we might need a path to assembledBinary
  char assembleCommand[100];

  snprintf(assembleCommand, 100, "./assemble \"%s\" \"%s\"", state->path, assembledBinary);

  int status = system(assembleCommand);
  status += 0;
  //just for debugging purposes so we can see if the system() call was sucessful.

  FILE *objCode = fopen(assembledBinary, "rb");
  CHECK_PRED(!objCode, "File could not be opened.");

  for (int i = 0; fread(&state->machineState->memory[i], sizeof(BYTE), 1, objCode); i++);
  CHECK_PRED(ferror(objCode), "An error has occurred whilst file reading.");
  fclose(objCode);

  if (state->noOfTokens == 2) {
	if (SAME(state->tokens[1], "all")) {
	  return runAll(state);
	} else {
	  printf("Invalid argument, run can only be called with no arguments or the argument \"all\"");
	  return;
	}
  } else {

	*(state->toDecode) = 0;
	*(state->toExecute) = 0;
	for (int i = 0; i < 2; i++) {
	  pipelineCycle(state->machineState, state->fetched, state->decoded, state->toDecode, state->toExecute);
	}
	state->isRunning = 1;
  }
}

void next(Editor *state) {
  int n;
  switch (state->noOfTokens) {
	case 1: n = 1;
	  break;
	case 2: n = atoi(state->tokens[1]);
	  break;
	default: printf("Too many arguments");
	  return;
  }
  if (state->currentLine < 0) {
	printf("%s", "You need to run first!\n");
	return;
  }
  for (int i = 0; i < n; i++) {
	do {
	  pipelineCycle(state->machineState, state->fetched, state->decoded, state->toDecode, state->toExecute);
	} while (*(state->toExecute) != 1);
  }
  int i, instrCount = 0;
  for (i = 0; instrCount < (state->machineState->registers[15] / 4) - 2; i++) {
	if (!strstr(state->lines[i], ":")) {
	  instrCount++;
	}
  }
  state->currentLine = (strstr(state->lines[i], ":")) ? i + 1 : i;
  int startLine = state->currentLine - 3 >= 0 ? state->currentLine - 3 : 0;
  int endLine = state->currentLine + 3 < state->noOfLines ? state->currentLine + 3 : state->noOfLines;
  print_lines(state, startLine, endLine, true);

  if (state->currentLine >= state->noOfLines) {
	printf("End of program reached. Final machine state:\n");
	// todo: decide whether or not we want to delete these files here.
	// remove("temp");
	// remove(state->path);
	stop(state);
  }
}


/**
 * Saves the written lines of assembly into a file, omitting comments written.
 */
void save(Editor *state) {
  if (state->noOfTokens != 2) {
	printf("Save requires you to specify only a path to which the file should be saved.\n");
	return;
  } else {
	FILE *outputFile = fopen(state->tokens[1], "w");
	if (!outputFile) {
	  fprintf(stderr, "Error opening file.");
	}
	char *trimmed;
	for (int i = 0; i < state->noOfLines; i++) {
	  trimmed = trim(state->lines[i]);
	  if (fputs(trimmed, outputFile) <= 0)
		fprintf(stderr, "fputs failure when writing to file.");
	  if (fputs("\n", outputFile) <= 0)
		fprintf(stderr, "fputs failure when writing to file.");
	  free(trimmed);
	}
	fclose(outputFile);
	strcpy(state->path, state->tokens[1]);
  }
}

void currentState(Editor *state) {
  if (state->currentLine < 0) {
	printf("You need to run first!\n");
	return;
  }
  printState(state->machineState);
}

void stop(Editor *state) {
  if (!state->isRunning) {
    printf("You need to run first!\n");
    return;
  }
  printState(state->machineState);
  resetState(state->machineState);
  state->currentLine = -1;
  state->isRunning = 0;
}

void none(Editor *state) {
  printf("I cannot do that :(\n");
}

void clear(Editor *state) {
  system("clear");
}