#include "tokenizer.h"

// zwraca 0, gdy udalo sie odczytac lub 1 wpp. (gdy np. EOF)
int get_token(FILE *fin, char *output) {
	char c;
	int nested = 0;
	int pos = 0;
	
	assert(fin != NULL);
	
	while (1) {
		c = fgetc(fin);
		
		if (c == EOF) 
			break;
			
		if (c == '<') {
			if (pos) {
				assert(fseek(fin, -1, SEEK_CUR) == 0);
				output[pos++] = '\0';
				return 0;
			}
			else {
				output[pos++] = c;
				++nested;
			}
		}
		else if (c == '>') {
			--nested;
			output[pos++] = c;
			
			if (!nested) {
				output[pos++] = '\0';
				return 0;
			}
		}
		else 
			output[pos++] = c;
	}
	
	return -1;
}


bool starts_with(const char *token, const char *literal) {
	return !strncmp(token, literal, strlen(literal));
}


bool equal(const char *token, const char *literal) {
	return !strcmp(token, literal);
}
