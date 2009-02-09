#include "tokenizer.h"
#include <string>
#include <list>
using namespace std;

list<string> fin_tags;

char token[1000];
int fp, fn, tp, tn;


int main(int argc, char *argv[]) {
	FILE *fgold, *fin;
	int ret;
	
	if (argc != 3) {
		printf("Uzycie: score <plik_zloty_standard> <otagowany_plik>\n");
		return 1;
	}
	
	fgold = fopen(argv[1], "r");
	fin = fopen(argv[2], "r");
	assert(fin != NULL && fgold != NULL);
	
	// odczytywanie kolejnych tagow w fin - zakladamy, ze kazdy orth ma dokladnie 1 disamb
	while (!get_token(fin, token))
		if (equal(token, "<lex disamb=\"1\">")) {
			get_token(fin, token); // <base>
			get_token(fin, token); // bezokolicznik
			get_token(fin, token); // </base>
			get_token(fin, token); // <ctag>
			assert(equal(token, "<ctag>"));
			get_token(fin, token); // tag
			fin_tags.push_back(token);
		}
	
	// przyrownywanie ich do zaznaczonych takow w fgold
	list<string>::iterator itor = fin_tags.begin();
	while (!get_token(fgold, token)) {
		if (itor == fin_tags.end())
			break; 
		
		if (equal(token, "</tok>")) {
			++itor;
			continue;
		}
			
		if (equal(token, "<lex disamb=\"1\">")) {
			get_token(fgold, token); // <base>
			get_token(fgold, token); // bezokolicznik
			get_token(fgold, token); // </base>
			get_token(fgold, token); // <ctag>
			assert(equal(token, "<ctag>"));
			get_token(fgold, token); // tag
			if (equal(token, itor->c_str()))
				++tp;
			else
				++fn;
		}
		else if (equal(token, "<lex>")) {
			get_token(fgold, token); // <base>
			get_token(fgold, token); // bezokolicznik
			get_token(fgold, token); // </base>
			get_token(fgold, token); // <ctag>
			assert(equal(token, "<ctag>"));
			get_token(fgold, token); // tag
			assert(itor != fin_tags.end());
			if (equal(token, itor->c_str()))
				++fp;
			else 
				++tn;
		}
	}
	
	printf("%d %d %d %d\n", tp, tn, fp, fn);
		
	fclose(fin);
	fclose(fgold);

	return 0;
}
