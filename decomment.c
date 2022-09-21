#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
/* Remove all comments while keeping line numbers */
enum Statetype {NORMAL_CODE, IN_STRING, IGNORED_CHARACTER_IN_STRING, 
IN_CHAR_LITERAL, IGNORED_CHARACTER_IN_CHAR_LITERAL, MAYBE_COMMENT_START, 
IN_COMMENT, MAYBE_LEAVING_COMMENT};
int ERROR_LINE;

/* Function when reading character for normal code. Takes in a character c, 
prints the character to stdout (unless it changes to the MAYBE_COMMENT state),
 and determines the new state. 
stdout */
enum Statetype handleNormalCodeState(int c)
{
    enum Statetype state;
    if (c == '"') {
        printf("%c", c);
        state = IN_STRING;
    }
    else if (c == '\'') {
        printf("%c", c);
        state = IN_CHAR_LITERAL;
    } 
    else if (c == '/') {
        state = MAYBE_COMMENT_START;
    }
    else  {
        printf("%c", c);
        state = NORMAL_CODE; 
        
}
return state;
}
/* Function for when in InString state. Takes in a character, c, and prints
it, and returns the new state */
enum Statetype handleInStringState(char c)
{
    enum Statetype state;
    if (c == '\\') {
    printf("%c", c);
    state = IGNORED_CHARACTER_IN_STRING;
    }
    else if (c == '"') {
       printf("%c", c);
        state = NORMAL_CODE;
    } 
    else {
        printf("%c", c);
    state = IN_STRING;
}
return state;
}
/* Function for when in IGNORED_CHARACTER_IN_STRING state. Takes in a character,
 c, prints it, and returns the new state */
enum Statetype handleIgnoredCharacterInStringState(char c)
{
    enum Statetype state;
    printf("%c", c);
    state = IN_STRING;
    return state; 
}
/* Function for when in IN_CHAR_LITERAL state. Takes in a character, c, 
prints it, and returns the new state */
enum Statetype handleInCharLiteralState(char c)
{
    enum Statetype state;
    if (c == '\\') {
    printf("%c", c);
    state = IGNORED_CHARACTER_IN_CHAR_LITERAL;
    }
    else if (c == '\'') {
        printf("%c", c);
        state = NORMAL_CODE;
    } 
    else {
    printf("%c", c);
    state = IN_CHAR_LITERAL;
}
return state;
}
/* Function for IgnoredCharacterInCharLiteral State. Takes in a character, c, 
prints it, and returns the new state */ 
enum Statetype handleIgnoredCharacterInCharLiteral(char c) 
{
    enum Statetype state;   
    printf("%c", c);
    state = IN_CHAR_LITERAL;
    return state;
    
}

/* Function for MAYBE_COMMENT_START state. Takes in a character, c, 
prints it and the "/" (if the new state is not IN_COMMENT), and returns 
the new state */
enum Statetype handleMaybeCommentStartState(char c)
{
    enum Statetype state;
    /* If another "/", print the first one  and remain in state */
    if (c == '/') {
        printf("/%c", c);
        state = MAYBE_COMMENT_START;
    }
    else if (c == '\'') {
        printf("/%c", c);
        state = IN_CHAR_LITERAL;
    }
    /* If in comment now, print spaces for the "/" and the "*" */
    else if (c == '*') {
        printf("  ");
        state = IN_COMMENT;
    } 
    else {
        printf("/%c", c);
        state = NORMAL_CODE;
    }
    return state;
}
/* Function for InComment state. Takes in a character, c, 
and returns the new state. Prints whitespace or newline if necessary. */
enum Statetype handleInCommentState(char c)
{
    enum Statetype state;
    if (c == '*') {
        printf(" ");
        state = MAYBE_LEAVING_COMMENT;
    }
    /* Print newline if c is a newline character */ 
    else if (c == '\n') {
        printf("%c", c);
        state = IN_COMMENT;
    }
    /* Print whitespace, otherwise */
    else {
        printf(" ");
        state = IN_COMMENT;
    }
    return state;
}
/* Function for MaybeLeavingComment State. Takes in a character, c, 
and returns the new state */
enum Statetype handleMaybeLeavingCommentState(char c)
{
    enum Statetype state;
    if (c == '*') {
        printf(" ");
        state = MAYBE_LEAVING_COMMENT;
    }
    if (c == '/') {
        printf(" ");
        state = NORMAL_CODE;
    }
    else {
        printf(" ");
        state = IN_COMMENT;
    }
return state;
}

/* Read in characters and delete all comments in code. Return EXIT_SUCCESS if 
there are no unterminated comments and EXIT_FAILURE if there are. */ 
int main(void) {
int c;
int ERROR_LINE_NUMBER = 0;
/* The first state is NormalCode */
enum Statetype state = NORMAL_CODE;
/* Read in character, one at a time, and update state depending on what 
character c is */ 
while ((c = getchar()) != EOF) {
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
            state = handleIgnoredCharacterInCharLiteral(c);
            break;
        case MAYBE_COMMENT_START: 
            state = handleMaybeCommentStartState(c); 
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
    fprintf(stderr, "Error: Line %d: unterminated error", ERROR_LINE);
    exit(EXIT_FAILURE);
}
/* Return EXIT_SUCCESS if it doesn't end in an error */
else return EXIT_SUCCESS;
}
