#ifndef STRINGS_H
#define STRINGS_H

typedef char *string;

// class string {
//     private:
//         char* content;
//     public:
//         string(char* content);
// };

void int_to_ascii(int n, string str);
void hex_to_ascii(int n, string str);
void reverse(string s);
int strlen(string s);
void backspace(string s);
void append(string s, char n);
int strcmp(string s1, string s2);

#endif