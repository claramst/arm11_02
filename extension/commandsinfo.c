#include "commandsinfo.h"

// have a "information mnemonic"
// "information data processing" will return the formats you can use to write these instructions in arm.
// and then maybe "data processing structure" which returns the binary form of a dpi.

void mnemonic(void) {
  FILE *file;
  char s;
  file = fopen("mnemonic.txt", "r");
  while ((s = fgetc(file)) != EOF) {
	printf("%c", s);
  }
}

void dataProcessingInfo(void) {
  printf("%s%s%s\n", RESET, "=====DATA PROCESSING INSTRUCTIONS=====", RESET);
  printf("These instructions perform arithmetic and logical operations on registers and/or immediate operands.\n");
  printf("Data processing instructions can be broken down into 3 main types. \n");
  printf("Instructions that compute results: and, eor, sub, rsb, add, orr. \n");
  printf("Their syntax is <opcode> Rd, Rn, <Operand2>. \n");
  printf("Single operand assignment: mov. \n");
  printf("Its syntax is: mov Rd, <Operand2> \n");
  printf("Instructions that set the CPSR ﬂags: tst, teq, cmp. \n");
  printf("Their syntax is: <opcode> Rn, <Operand2>\n");
  printf("Operand2 can take 3 forms.\n");
  printf("<#expression> is a numeric constant.\n");
  printf("<register> is a register.\n");
  printf("Rm{,<shift>} is a shifted register. \n");
}

void branchInfo(void) {
  printf("%s%s%s\n", RESET, "=====BRANCH INSTRUCTIONS=====", RESET);
  printf("If the branch condition is met, these instructions change the PC to point to a specific target address.\n");
  printf("All branch instructions are of the form b<cond> <expression>.\n");
}

void multiplyInfo(void) {
  printf("%s%s%s\n", RESET, "=====MULTIPLY INSTRUCTIONS=====", RESET);
  printf("These perform multiplication on register operands.\n");
  printf("Multiply instructions are broken down into 2 types \n");
  printf("Multiply, with syntax: mul Rd, Rm, Rs. \n");
  printf("Multiply with accumulate, with syntax: mla Rd, Rm, Rs, Rn. \n");
}

void singleDataTransferInfo(void) {
  printf("%s%s%s\n", RESET, "=====SINGLE DATA TRANSFER INSTRUCTIONS=====", RESET);
  printf("These transfer data between memory and registers.\n");
  printf("All data transer instructions are of the form <operation> <Rd> <address>, \n");
  printf(
	  "When writing data transfer instructions you must be careful where you place brackets as this will impact when"
   " the offset will be applied to the address.\n");
  printf("Pre-indexed addresses are of the form. \n");
  printf("[Rn], using base register Rn, with an oﬀset of zero. \n");
  printf("[Rn,<#expression>], using base register Rn, oﬀset of <#expression> bytes. \n");
  printf(
	  "[Rn,{+/-}Rm{,<shift>},], using base register Rn, oﬀset by +/- the contents of index register Rm which has been"
   " shifted by <shift>. \n");
  printf("Post-indexed addresses are of the form. \n");
  printf("[Rn],<#expression>, oﬀset base register Rn by <#expression> bytes. \n");
  printf(
	  "[Rn],{+/-}Rm{,<shift>}, oﬀset base register Rn by +/- the contents of index register Rm which has been shifted"
   " by <shift>. \n");
}

void dataProcessingBinary(void) {
  printf("%s%s%s\n", RESET, "=====DATA PROCESSING INSTRUCTIONS=====", RESET);
  printf("%s%s%s\n", CYAN, "|31      28      25  24      21 20  19    16 15    12 11           0|", RESET);
  printf("|   Cond   | 00 | I |  OpCode  | S |   Rn   |   Rd   |   Operand2   |\n");
}

void branchBinary(void) {
  printf("%s%s%s\n", RESET, "=====BRANCH INSTRUCTIONS=====", RESET);
  printf("%s%s%s\n", CYAN, "|31      28 27 25 24                                                0|", RESET);
  printf("|   Cond   | 101 | I |                  Offset                       |\n");
}

void multiplyBinary(void) {
  printf("%s%s%s\n", RESET, "=====MULTIPLY INSTRUCTIONS=====", RESET);
  printf("%s%s%s\n", CYAN, "|31      28          21  20  19    16 15    12 11     8        3      0|", RESET);
  printf("|   Cond   | 000000 | A | S |   Rd   |   Rn   |   Rs   | 1001 |   Rm   |\n");
}

void singleDataTransferBinary(void) {
  printf("%s%s%s\n", RESET, "=====SINGLE DATA TRANSFER INSTRUCTIONS=====", RESET);
  printf("%s%s%s\n", CYAN, "|31      28      25  24  23       20  19    16 15    12 11          0|", RESET);
  printf("|   Cond   | 01 | I | P | U | 00 | L |   Rn   |   Rd   |   Offset   |\n");
}

void instruction(Editor *state) {
  char *name = malloc(state->MAX_LINE_LENGTH * sizeof(char));
  bool running = true;

  while (running) {
	printf("Enter \"mnemonic\" to see a table of all supported ARM instructions.\nWhich instruction type do you want to"
		" know about?: \n");
	getInput(name, state->MAX_LINE_LENGTH);
	if (SAME(name, "options")) {
	  printf("-data processing\n-branch\n-multiply\n-single data transfer\n-mnemonic\n-exit\n");
	} else if (SAME(name, "mnemonic")) {
	  mnemonic();
	} else if (SAME(name, "data processing")) {
	  dataProcessingInfo();
	  dataProcessingBinary();
	} else if (SAME(name, "branch")) {
	  branchInfo();
	  branchBinary();
	} else if (SAME(name, "multiply")) {
	  multiplyInfo();
	  multiplyBinary();
	} else if (SAME(name, "single data transfer")) {
	  singleDataTransferInfo();
	  singleDataTransferBinary();
	} else if (SAME(name, "exit")) {
	  running = false;
	}
	printf("\n");
  }
  free(name);
}

void info(Editor *state) {
  if (state->noOfTokens == 1) {
	printf("command \"info\" requires at least one argument.\n");
  } else if (state->noOfTokens == 2) {
	if (SAME(state->tokens[1], "options")) {
	  printf("- any command\n- \"instruction\"\n- \"shortcuts\"\n");
	} else if (SAME(state->tokens[1], "add")) {
	  printf("Adds two registers\n");
	} else if (SAME(state->tokens[1], "about")) {
	  printf("Prints an \"about page\" for our program.\n");
	} else if (SAME (state->tokens[1], "quit")) {
	  printf("Type \"quit\" to exit the program.\n");
	} else if (SAME(state->tokens[1], "help")) {
	  printf("Type \"help\" to get  a list of all supported commands.\n");
	} else if (SAME(state->tokens[1], "info")) {
	  printf("Type info _____ to get more information about _____ (if available).\n");
	} else if (SAME(state->tokens[1], "clear")) {
	  printf("Type \"clear\" to reset view.\n");
	} else if (SAME(state->tokens[1], "ARMOUR")) {
	  about(state);
	} else if (SAME(state->tokens[1], "instruction")) {
	  instruction(state);
	} else if (SAME(state->tokens[1], "display")) {
	  printf("Shows the program you have written. Use \"options\" to see  more features.\n");
	} else if (SAME(state->tokens[1], "write")) {
	  printf("Allows you to edit your assembly program. Use \"options\" to see more features.\nEnter \"exit\" on a line"
		  " to exit write mode.\nEnter \"back\" on a line to go back to the previous line.\n");
	} else if (SAME(state->tokens[1], "shortcuts")) {
	  printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
			 "help - h",
			 "quit - q",
			 "clear - c",
			 "info - i",
			 "write - w",
			 "display - d",
			 "load - l",
			 "save - s",
			 "state - st",
			 "run - r",
			 "stop - X",
			 "next - n",
			 "about - a");
	} else if (SAME(state->tokens[1], "run")) {
	  printf("Enters run mode with the saved program.\n");
	} else if (SAME(state->tokens[1], "next")) {
	  printf("Whilst in run mode, executes the current line and goes to the next line.\n");
	} else if (SAME(state->tokens[1], "state")) {
	  printf("Whilst in run mode, prints the current state of registers and memory.\n");
	} else if (SAME(state->tokens[1], "stop")) {
	  printf("Whilst in run mode, halts execution of the program, prints the state of the registers and memory, and "
		  "then exits run mode.\n");
	} else if (SAME(state->tokens[1], "finish")) {
	  printf("Whilst in run mode, runs the remainder of the program from the current line, prints the final machine "
		  "state then exits run mode.\n");
	} else if (SAME(state->tokens[1], "load")) {
	  printf("Loads all the lines of code from a text file.\n");
	} else if (SAME(state->tokens[1], "export")) {
	  printf("Saves the written lines to a specified output file, comments included.\n");
	}
  }
}

