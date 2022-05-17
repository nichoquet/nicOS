#ifndef STRINGS_H
#define STRINGS_H

typedef char *string;

void int_to_ascii(int n, string str);
void hex_to_ascii(int n, string str);
void reverse(string s);
int strlen(string s);
void backspace(string s);
void append(string s, char n);
int strcmp(string s1, string s2);

#endif