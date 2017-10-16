/*************************************************************************
* Title: (CIS265_8) Text File Manipulator.
* File: main.c
* Author: James Eli
* Date: 9/9/2017
*
* Develop a C program to perform several file I/O functions for the 
* supplied "CIS265.txt" file. The user will be given a menu to choose from 
* the following options:
*  1 - Press (S) to search for a word
*  2 - Press (L) to display specified number of text lines to the screen
*  3 - Press (A) to append new content to the file
*  4 - Press (Q) to quit
*
* For user option # 1, first ask the user to enter the word that they like 
* to search for in the “CIS265.txt” file then display to the screen the 
* entire file content including the found word(s) between straight brackets. 
* At the end of the file, print a line saying how many occurrences have been 
* found. For example, if the user enters the word “system” as the search 
* word then the screen will display the following:
*
* I [like] potatoes and mushrooms. I aslo [like] watermelon. I forgot if I 
* also told you that I [like] grapes.
*
* Number of occurrences of the word "like" is 3
*
* For user option #2, ask the user how many lines from the “CIS265.txt” 
* would like to be printed out to the screen. Once the number is entered, 
* display the content to the screen 
*
* For user option #3, ask the use to type anything to be appended to the 
* end of the current file “CIS265.txt”. Once entered, permanently append it 
* to the file. If the user chooses to select option # 1 from the menu after 
* appending then make sure you can search as well the newly appended text to 
* the file.
*
* After every operation completion, you need to re-display the menu until 
* the user selects option #4 to quit the program.
*
* Notes:
*  (1) Limited input validation is performed.
*  (2) Will search for whole words, parts of words and phrases.
*  (3) Does NOT perform a "caseless" search.
*  (4) Line length can be stipulated via the MAX_CHARS_PER_LINE macro.
*  (5) Line print incorporates rudimentary word-wrap.
*  (6) Compiled with MS Visual Studio 2017 Community (v141), using C
*      language options.
*
* Submitted in partial fulfillment of the requirements of PCC CIS-265.
*************************************************************************
* Change Log:
*   09/09/2017: Initial release. JME
*   10/01/2017: Corrected append/realloc functionality. JME
*   10/06/2017: Added functionality to handle failed file read. JME
*   10/14/2017: Moved text funcitons to separate files. JME
*************************************************************************/
#include <assert.h> 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "text_processor.h"

#ifdef _MSC_VER
// C/C++ Preprocessor Definitions: _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996) 
#endif

// Get number of lines to display.
int getNumLines(void) {
	int lines = -1;          // Return value (-1 = failure).
	char input[5];           // Holds user input as string.
	int d;                   // Temporary holds input number.
	int attempts = MAX_INPUT_ATTEMPTS;

	while (attempts--) {
		// Prompt and grab user input.
		fprintf(stdout, "Enter number of lines to display (%d - %d): ", MIN_NUM_LINES, MAX_NUM_LINES);
		if (!fgets(input, sizeof input, stdin)) {
			fputs("\nFatal program error!\n", stderr);
			return lines;
		}
		else if (!strchr(input, '\n')) {
			// input too long, eat remaining characters.
			while (fgets(input, sizeof input, stdin) && !strchr(input, '\n'))
				; // Empty body.
			fputs("Too many characters input.\n", stdout);
		}
		else {
			// Catch special case of null input.
			if (strlen(input) <= 1) 
				return DISPLAY_ENTIRE_TEXT;
			// Attempt to convert from string to int, and validate.
			if (sscanf(input, "%d", &d)) {
				if (d >= MIN_NUM_LINES && d <= MAX_NUM_LINES) {
					lines = d;
					break; // Exit.
				}
				else
					fputs("Value entered is outside allowable range.\n", stdout);
			}
			else
				fputs("Invalid input.\n", stdout);
		}
	}
	return lines; // Return number of lines or -1 for failure.
}

// Attempt to open & read text content of file.
bool readFile(const char *fileName, char **file) {
	long fileLen = 0;
	FILE *filePtr = NULL;

	// Attempt to open file.
	if ((filePtr = fopen(fileName, "r")) == NULL) {
		printf("Error opening file (%u).", errno);
		return false;
	}

	// Determine length of file, and rewind to beginning of file.
	fseek(filePtr, 0L, SEEK_END);
	fileLen = ftell(filePtr);
	fseek(filePtr, 0L, SEEK_SET); // rewind(filePtr);
	
	// Reserve space to hold file.
	*file = calloc(fileLen + 1, sizeof(char));
	if (*file == NULL) {
		fputs("Insufficient memory to read file.\n", stderr);
		return false;
	}

	// Read entire file contents.
	errno = 0;
	if (fileLen != fread(*file, sizeof(char), fileLen + 1, filePtr) + 1 || errno)
		return false;

	// Close the file.
	fclose(filePtr);
	return true;
}

// Command entry handled here.
char fetchCommand(void) {
	char input[3];  // Holds user input for number of array elements.

	while (1) {
		// Prompt and grab user input.
		fputs(MENU_TEXT, stdout);
		if (!fgets(input, sizeof input, stdin)) {
			fputs("\nFatal program error!\n", stderr);
			return 'q'; // Force quit.
		}
		else if (!strchr(input, '\n')) {
			// input too long, eat remainder.
			while (fgets(input, sizeof input, stdin) && !strchr(input, '\n'))
				; // Empty body.
			fputs("Too many characters input.\n", stdout);
		}
		else if (strlen(input) == 2) {
			char command = tolower(input[0]);
			if (strchr("slaq", command))
				return command;
			else
				return '\0';
		}
	}
}

// Program starts here.
int main(int argc, char *argv[]) {
	char fileName[80] = "CIS265.txt"; // Default text file.
	char *txt = NULL;

	// Required to make eclipse console output work properly.
	setvbuf(stdout, NULL, _IONBF, 0);
	fflush(stdout);

	// Check command line arguments 
	if (argc == 2) {
		// Open the commandline file name. 
		strcpy(fileName, argv[1]);
	} 
	// Attempt to open default filename.
	if (!readFile(fileName, &txt)) {
		fputs("\nUsage: cis265_8 foo.txt\n", stderr);
		exit(EXIT_FAILURE);
	}

	// Dispatch menu command.
	while (1) {
		int numLines;

		switch (fetchCommand()) {
		case 's':
			// Search for text.
			searchText(txt);
			break;

		case 'l': 
			// Display lines.
			numLines = getNumLines();
			if (numLines == DISPLAY_ENTIRE_TEXT)
				displayText(txt, strlen(txt), 0);
			else if (numLines > 0)
				displayText(txt, numLines*MAX_CHARS_PER_LINE - 1, 0);
			break;

		case 'a':
			// Append text.
			if ((txt = appendText(txt, fileName)) != NULL)
				break;
			/* no break */

		case 'q':
			// Quit.
			fputs("\nTerminating program.\n", stdout);
			free(txt);
			exit(EXIT_SUCCESS);
			break;

		case '\0':
		default:
			// Unsupported command.
			fputs("\nUnsupported command entered.\n", stdout);
			break;
		}
	}
}
