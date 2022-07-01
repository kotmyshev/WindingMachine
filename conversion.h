#pragma once

typedef unsigned int size_t;

/*calculate char[] size */
size_t csize(const char Str[]);

/* reverse symbols in char array*/
 void reverse(char s[]);
 
 /* itoa:  convert int n to char array s */
 void itoa(int n, char s[]);

 void reverse(char s[], size_t len);

void uintostr(size_t n, char s[], size_t len);

void uintos(size_t n, char s[], size_t len);

void booltoccw(char s[], bool ccw);
