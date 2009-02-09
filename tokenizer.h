#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <map>
#include <string>
#include <list>
using namespace std;

// zwraca 0, gdy udalo sie odczytac lub 1 wpp. (gdy np. EOF)
int get_token(FILE *fin, char *output);

bool starts_with(const char *token, const char *literal);
bool equal(const char *token, const char *literal);

#endif // _TOKENIZER_H
