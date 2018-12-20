#ifndef USERCONSOLE_H
#define USERCONSOLE_H

/*
 * write a char to the console
 * reg4 should contain the char to write
 */
void do_PutChar();

/*
 * write a string to the console
 * reg4 should contain the string's address
 */
void do_PutString();

/*
 * read a char from the console.
 * at the end, reg2 contains the char
 */
void do_GetChar();

/*
 * read a string from the console.
 * reg4 should contain the address to write it to
 * reg5 should contain the number of characters to read
 */
void do_GetString();

/*
 * write an int to the console
 * reg4 should contain the int
 * Does no perform any checks on the int value
 */
void do_PutInt();

/*
 * read an int from the console.
 * at the end, reg2 contains the int
 */
void do_GetInt();

#endif /* USERCONSOLE_H */
