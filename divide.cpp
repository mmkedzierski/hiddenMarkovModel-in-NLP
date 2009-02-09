#include "tokenizer.h"

#define MAXBINS 100
#define NONE (-1)

int nbins;
FILE *fout[MAXBINS];
char token[1000];

int main(int argc, char *argv[]) {
	FILE *fin;
	int fout_no;
	int nested_chunks;
	
	if (argc < 3) {
		printf("Uzycie: divide <liczba_czesci> <lista_plikow_korpusu>\n");
		return 1;
	}
	
	sscanf(argv[1], "%d", &nbins);
	
	for (int i=0; i<nbins; ++i) {
		char name[20];
		sprintf(name, "bins/bin%d", i);
		fout[i] = fopen(name, "w");
		fprintf(fout[i], "<chunkList>");
	}
	
	for (int k=2; k<argc; ++k) {
		printf("\nDividing '%s'...", argv[k]);
		fflush(stdout);
		fin = fopen(argv[k], "r");
		
		fout_no = NONE;
		nested_chunks = 0;
		while (!get_token(fin, token)) {
			if (starts_with(token, "<chunk ")) {
 				if (!nested_chunks)
 					fout_no = rand() % nbins;
				++nested_chunks;
			}
			else if (equal(token, "</chunk>")) {
				--nested_chunks;
			}
				
			if (fout_no != NONE)
				fprintf(fout[fout_no], "%s", token);
		}
		
		fclose(fin);
	}
	
	for (int i=0; i<nbins; ++i) {
		fprintf(fout[i], "</chunkList>");
		fclose(fout[i]);
	}
	
	printf("\n");

	return 0;
}
