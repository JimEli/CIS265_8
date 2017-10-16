/*************************************************************************
* Title: (CIS265_8) Text File Manipulator.
* File: text_processor.h
* Author: James Eli
* Date: 9/9/2017
*
* Text processing functions header file. See "main.c" for more information.
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
#ifndef _TEXT_PROCESSOR_H_
#define _TEXT_PROCESSOR_H_

#define MAX_CHARS_PER_LINE     40  // Maximum characters on a display line.
#define MAX_TEXT_LENGTH        100 // Maximum length of text buffer.
#define MAX_SEARCH_TEXT_LENGTH 10  // Maximum length of search text.
#define MAX_INPUT_ATTEMPTS     3   // Maximum failed inputs attempts before abort.
#define MIN_NUM_LINES          0   // Minimum lines of text for display.
#define MAX_NUM_LINES          99  // Maximum lines of text for display.
#define DISPLAY_ENTIRE_TEXT    -1  // Flag to display entire text.

#define MENU_TEXT              "\nProgram Menu:\n"                 \
                               "S - Search for word.\n"            \
                               "L - Display lines.\n"              \
                               "A - Append new content to file.\n" \
                               "Q - Quit.\nEnter S, L, A or Q: "

// Display lines of text with word wrap.
void displayText(const char *, int, const int);
// Append command, add new content to end of file.
char *appendText(char *, const char *);
// Search for specified word.
void searchText(char *);

#endif //_TEXT_PROCESSOR_H_
