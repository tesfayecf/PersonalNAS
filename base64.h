#ifndef _BASE64_H
#define _BASE64_H

extern const char b64_alphabet[];

void b64_encode(char *output, char *input, int inputLen);

void b64_decode(char *output, char *input, int inputLen);

#endif // _BASE64_H
