# Pima CIS265 Week #8 Project

Develop a C program to perform several file I/O functions for the supplied "CIS265.txt" file. The user will be given a menu to choose from 
the following options:
```text
  1 - Press (S) to search for a word
  2 - Press (L) to display specified number of text lines to the screen
  3 - Press (A) to append new content to the file
  4 - Press (Q) to quit
```
For user option # 1, first ask the user to enter the word that they like to search for in the “CIS265.txt” file then display to the screen the 
entire file content including the found word(s) between straight brackets. At the end of the file, print a line saying how many occurrences have been 
found. For example, if the user enters the word “system” as the search word then the screen will display the following:
```text
I [like] potatoes and mushrooms. I aslo [like] watermelon. I forgot if I also told you that I [like] grapes.

Number of occurrences of the word "like" is 3
```
For user option #2, ask the user how many lines from the “CIS265.txt” would like to be printed out to the screen. Once the number is entered, 
display the content to the screen 

For user option #3, ask the use to type anything to be appended to the end of the current file “CIS265.txt”. Once entered, permanently append it 
to the file. If the user chooses to select option # 1 from the menu after appending then make sure you can search as well the newly appended text to 
the file.

After every operation completion, you need to re-display the menu until the user selects option #4 to quit the program.

Notes:
* Limited input validation is performed.
* Will search for whole words, parts of words and phrases.
* Does NOT perform a "caseless" search.
* Line length can be stipulated via the MAX_CHARS_PER_LINE macro.
* Line print incorporates rudimentary word-wrap.
* Compiled with MS Visual Studio 2017 Community (v141), using C language options.
