#include "tokenizer.h"

char token[1000];
double gamma[3];


int main(int argc, char *argv[]) {
	FILE *fin;
	FILE *fmodel;
	FILE *fout;
	
	if (argc < 3) {
		printf("Uzycie: find_gammas <model> <output> <lista_plikow_korpusu>\n"
			"gdzie: model - plik z modelem ngramowym (w formacie model_dumper)\n");
		return 1;
	}
	
/*	fmodel = fopen(argv[1], "r");
	
	for (int k=3; k<argc; ++k) {
		printf("\nProcessing '%s'...", argv[k]);
		fflush(stdout);
		fin = fopen(argv[k], "r");
		
		while (!get_token(fin, token)) {
			;
		}
		
		fclose(fin);
	}*/
	
	gamma[0] = 0.7;
	gamma[1] = 0.2;
	gamma[2] = 0.1;
	
	fout = fopen(argv[2], "w");
	fprintf(fout, "%lf %lf %lf\n", gamma[0], gamma[1], gamma[2]);
	fclose(fout);
	
	printf("\n");

	return 0;
}
