#include "tokenizer.h"
#include "ngram_tools.h"
#include "model_dumper.h"

trigram_map_t tag_trigrams; // c(t1 t2 t3) 
bigram_map_t tag_bigrams; // c(t1 t2)
trigram_map_t word_tag_grams; // c(t1 t2 w2)
unigram_map_t beginner_tags;

string last_tag, prelast_tag, cur_tag, cur_word;

double unknown_tag_trigrams;
double unknown_tag_bigrams;
double unknown_word_tag_grams;
double unknown_beginner_tags;

char token[1000];


int main(int argc, char *argv[]) {
	FILE *fout, *fin;
	char fname[100];
	
	if (argc < 2) {
		printf("Uzycie: untag <lista_plikow_do_usuniecia_tagow>\n");
		return 1;
	}
	
	for (int k=1; k<argc; ++k) {
		fin = fopen(argv[k], "r");
		sprintf(fname, "%s.untagged", argv[k]);
		fout = fopen(fname, "w");
		
		printf("\nProcessing '%s'...", argv[k]);
		fflush(stdout);
		
		while (!get_token(fin, token)) {
			if (equal(token, "<lex disamb=\"1\">"))
				fprintf(fout, "<lex>");
			else 
				fprintf(fout, "%s", token);
		}
		
		fclose(fin);
		fclose(fout);
	}
	
	printf("\n");
	
	return 0;
}
