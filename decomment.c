#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
/* defines constants representing each state in the DFA */
enum Statetype {NORMAL_CODE, IN_STRING, IGNORED_CHARACTER_IN_STRING, 
IN_CHAR_LITERAL, IGNORED_CHARACTER_IN_CHAR_LITERAL, MAYBE_COMMENT_START, 
IN_COMMENT, MAYBE_LEAVING_COMMENT};

/* Function when reading character for normal code. Takes in a character
 c, prints the character to stdout (unless it changes to the 
 MAYBE_COMMENT state), and determines the new state. */
enum Statetype handleNormalCodeState(int c)
{
    enum Statetype state;
    if (c == '"') {
        putchar(c);
        state = IN_STRING;
    }
    else if (c == '\'') {
        putchar(c);
        state = IN_CHAR_LITERAL;
    } 
    else if (c == '/') {
        state = MAYBE_COMMENT_START;
    }
    else  {
        putchar(c);
        state = NORMAL_CODE; 
        
}
return state;
}
/* Function for when in InString state. Takes in a character, c, and 
prints it. Returns the new state */
enum Statetype handleInStringState(int c)
{
    enum Statetype state;
    if (c == '\\') {
        putchar(c);
        state = IGNORED_CHARACTER_IN_STRING;
    }
    else if (c == '"') {
        putchar(c);
        state = NORMAL_CODE;
    } 
    else {
         putchar(c);
         state = IN_STRING;
}
return state;
}
/* Function for when in IGNORED_CHARACTER_IN_STRING state. Takes in a 
character, c, prints it, and returns the IN_STRING state */
enum Statetype handleIgnoredCharacterInStringState(int c)
{
    enum Statetype state;
    putchar(c);
    state = IN_STRING;
    return state; 
}
/* Function for when in IN_CHAR_LITERAL state. Takes in a character, c, 
prints it, and returns the new state */
enum Statetype handleInCharLiteralState(int c)
{
    enum Statetype state;
    if (c == '\\') {
        putchar(c);
        state = IGNORED_CHARACTER_IN_CHAR_LITERAL;
    }
    else if (c == '\'') {
        putchar(c);
        state = NORMAL_CODE;
    } 
    else {
        putchar(c);
        state = IN_CHAR_LITERAL;
}
return state;
}
/* Function for IgnoredCharacterInCharLiteral State. Takes in a 
character, c, prints it, and returns the state IN_CHAR_LITERAL */ 
enum Statetype handleIgnoredCharacterInCharLiteralState(int c) 
{
    enum Statetype state;   
    putchar(c);
    state = IN_CHAR_LITERAL;
    return state;
    
}

/* Function for MAYBE_COMMENT_START state. Takes in a character, c, 
prints it and the "/" (if the new state is not IN_COMMENT), and returns 
the new state */
enum Statetype handleMaybeCommentStartState(int c)
{
    enum Statetype state;
    /* If another "/", print the first one  and remain in state */
    if (c == '/') {
        printf("/");
        state = MAYBE_COMMENT_START;
    }
    /* If start of char literal, print first "/" and change state */
    else if (c == '\'') {
        printf("/%c", c);
        state = IN_CHAR_LITERAL;
    }
    /* If start of string literal, print first "/" and change state */
    else if (c =='"') {
        printf("/%c", c);
        state = IN_STRING;
    }
    /* If in comment now, print space for the comment. Change state */
    else if (c == '*') {
        printf(" ");
        state = IN_COMMENT;
    } 
    /* Otherwise, print the "/" and the new character. Change state */
    else {
        printf("/%c", c);
        state = NORMAL_CODE;
    }
    return state;
}
/* Function for InComment state. Takes in a character, c, and does not 
print it. Prints newline if appropriate. Returns the new state based on 
the value of c.  */
enum Statetype handleInCommentState(int c)
{
    enum Statetype state;
    if (c == '*') {
        state = MAYBE_LEAVING_COMMENT;
    }
    /* Print newline if c is a newline character */ 
    else if (c == '\n') {
        putchar(c);
        state = IN_COMMENT;
    }
    /* Print nothing, otherwise */
    else {
        state = IN_COMMENT;
    }
    return state;
}
/* Function for MaybeLeavingComment State. Takes in a character, c, 
and returns the new state. Prints a newline if appropriate. */
enum Statetype handleMaybeLeavingCommentState(int c)
{
    enum Statetype state;
    if (c == '*') {
        state = MAYBE_LEAVING_COMMENT;
    }
    /* Print newline if c is a newline character */ 
    else if (c == '\n') {
        putchar(c);
        state = IN_COMMENT;
    }
    else if (c == '/') {
        state = NORMAL_CODE;
    }
    else {
        state = IN_COMMENT;
    }
return state;
}

/* Read in text from stdin and print the text, not including characters 
that are part of a comment. Return EXIT_SUCCESS if there are no 
unterminated comments and EXIT_FAILURE if there are. */ 
int main(void) {
/* c is the character that will be read in from stdin */ 
int c;
int current_line_number = 1;
int error_line_number;
/* The first state is NormalCode */
enum Statetype state = NORMAL_CODE;
/* Read in character, one at a time, until reaching EOF and update state
depending on what character c is */ 
while ((c = getchar()) != EOF) {
    /* Keep track of current line number */ 
    if (c == '\n') {
        current_line_number += 1; 
    }
    switch (state) {
        case NORMAL_CODE: 
            state = handleNormalCodeState(c);
            break;
        case IN_STRING:
            state = handleInStringState(c);
            break;
        case IGNORED_CHARACTER_IN_STRING: 
            state = handleIgnoredCharacterInStringState(c);
            break;
        case IN_CHAR_LITERAL: 
            state = handleInCharLiteralState(c);
            break;
        case IGNORED_CHARACTER_IN_CHAR_LITERAL: 
            state = handleIgnoredCharacterInCharLiteralState(c);
            break;
        case MAYBE_COMMENT_START: 
            state = handleMaybeCommentStartState(c); 
            /* Updates the line number of the start of the comment */
            if (state == IN_COMMENT) {
                error_line_number = current_line_number;
            }
            break;
        case IN_COMMENT: 
            state = handleInCommentState(c);
            break;
        case MAYBE_LEAVING_COMMENT: 
            state = handleMaybeLeavingCommentState(c);
            break;
    }
}
/* print failed line to stderr stream if there is unterminated error, 
and return EXIT_FAILURE */
if (state == IN_COMMENT || state == MAYBE_LEAVING_COMMENT) {
    fprintf(stderr, "Error: line %d: unterminated comment\n", 
                                                    error_line_number);
    exit(EXIT_FAILURE);
}
/* print the final "/" if ended in the MAYBE_COMMENT_START state */
if (state == MAYBE_COMMENT_START) {
    printf("/");
}
/* Return EXIT_SUCCESS if it doesn't end in an error */
return EXIT_SUCCESS;
}