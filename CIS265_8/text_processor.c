/*************************************************************************
* Title: (CIS265_8) Text File Manipulator.
* File: text_processor.c
* Author: James Eli
* Date: 9/9/2017
*
* Text processing functions. See "main.c" for more information.
*
* Notes:
*  (1) Will search for whole words, parts of words and phrases.
*  (2) Does NOT perform a "caseless" search.
*  (3) Line length can be stipulated via the MAX_CHARS_PER_LINE macro.
*  (4) Line print incorporates rudimentary word-wrap.
*  (5) Compiled with MS Visual Studio 2017 Community (v141), using C
*      language options.
*
* Submitted in partial fulfillment of the requirements of PCC CIS-265.
*************************************************************************
* Change Log:
*   10/14/2017: Moved text funcitons to separate files. JME
*************************************************************************/
#include <assert.h> 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "text_processor.h"

#ifdef _MSC_VER
// C/C++ Preprocessor Definitions: _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996) 
#endif

char txt[MAX_TEXT_LENGTH + 1];    // String array for search and append texts.

// Attempt to save modified file.
static bool saveFile(const char *fileName, const char *file) {
	FILE *filePtr;
	bool retVal = false;

	// Attempt to open file.
	filePtr = fopen(fileName, "w");
	if (filePtr == NULL) {
		printf("Can't open file for writing\n");
		fclose(filePtr);
	}
	else {
		// Write to file.
		if (fwrite(file, strlen(file), 1, filePtr) == 1)
			retVal = true;
		fclose(filePtr);
	}
	return retVal;
}

// Get search and/or append text.
static char *getText(const char *prompt, const char terminator, const int len) {
	int attempts = MAX_INPUT_ATTEMPTS;

	while (attempts--) {
		// Enter text.
		fprintf(stdout, "%s (limited to %d characters):%c", prompt, len, terminator);
		if (!fgets(txt, len, stdin)) {
			fputs("Fatal program error!\n", stderr);
			return NULL;
		}
		else if (!strchr(txt, '\n')) {
			// Input too long, eat remainder.
			while (fgets(txt, len, stdin) && !strchr(txt, '\n'))
				; // Empty body.
			fputs("Too many characters input.\n", stdout);
		}
		else {
			// Remove trailing '\n' character.
			if (*(txt + strlen(txt) - 1) == '\n')
				*(txt + strlen(txt) - 1) = 0;
			return txt;
		}
	}
	return NULL; // Failure if here.
}

// Append command, add new content to end of file.
char *appendText(char *text, const char *fileName) {
	char *target = getText("Enter text to append to file", '\n', MAX_TEXT_LENGTH);

	// If valid append text, re-allocate storage and copy.
	if (target != NULL && strlen(target) >= 1) {
		text[strlen(text) - 1] = 0; // Properly terminate file string.
		// Re-allocate new memory accounting for appended text.
		char *_text = (char *)realloc(text, strlen(text) + strlen(target) + 2);
		if (_text == NULL) {
			// If new pointer invalid, old memory freed upon return, and program aborted.
			fputs("\nUnable to allocate memory to append text.\n", stdout);
			return NULL;
		}
		else {
			if (text != _text)
				free(text);
			text = _text; // Juggle pointers.
		}
		strcat(text, target); // Append new text.
		strcat(text, "\n");   // Add new line to end.
		fprintf(stdout, "File write %s.\n", saveFile(fileName, text) ? "succeeded" : "failed");
	}
	else
		fputs("No file written.\n", stdout);

	return text; // Return pointer to text.
}

// Display lines of text with word wrap.
void displayText(const char *s, int totalChars, const int reset) {
	static const char *delimiters = " ,.!?";
	static int charCount, lineCount; // Display character and line counters.

	// Start over.
	if (reset == 0) {
		charCount = 1;
		lineCount = 0;
	}

	// Calculate total characters to display.
	totalChars += charCount;

	// Iterate through string.
	while (*s) {
		// Maximum text displayed and end of word?
		if (charCount > totalChars)
			break;

		// Due a line break?
		if (charCount - (lineCount*MAX_CHARS_PER_LINE) >= MAX_CHARS_PER_LINE) {
			while (!strchr(delimiters, *s)) { // Continue until a delimiter.
				putchar(*s++);
				charCount++;
			}
			while (strchr(delimiters, *s)) { // Continue while a delimiter.
				putchar(*s++);
				charCount++;
			}
			putchar('\n'); // LF.
			lineCount++;
		}
		else {
			// Display next character.
			putchar(*s++);
			charCount++;
		}
	}
}

// Search for specified word.
void searchText(char *text) {
	char *target = getText("Enter text to search for", ' ', MAX_SEARCH_TEXT_LENGTH);

	// If valid search target, iterate through string looking for target(s).
	if (target != NULL && strlen(target) >= 1) {
		char *s, *_text = text;
		int count = 0; // Count of words found.

		while (((s = strstr(_text, target)) != NULL)) {
			// Found target, so highlight it.
			displayText(_text, s - _text - 1, count++);
			//displayText("[", 1, count);
			//displayText(target, strlen(target), count);
			//displayText("]", 1, count);
			printf("[%s]", target);
			_text = s + strlen(target);
		}
		// Display remaining text.
		displayText(_text, strlen(_text), count);
		// Display stats.
		fprintf(stdout, "\nNumber of occurrences of the text \"%s\" is %d\n", target, count);
	}
	else
		fputs("\nNo text to search for.\n", stdout);
}
