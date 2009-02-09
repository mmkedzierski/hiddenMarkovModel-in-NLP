#include "tokenizer.h"
#include "ngram_tools.h"
#include "model_dumper.h"

FILE *fout;
char token[1000];


trigram_map_t tag_trigrams; // c(t1 t2 t3)
bigram_map_t tag_bigrams; // c(t1 t2)
unigram_map_t tag_unigrams; // c(t1)
trigram_map_t word_tag_grams; // c(t1 t2 w2)
bigram_map_t beginner_tag_bigrams;

string last_tag, prelast_tag, cur_tag, cur_word;




int main(int argc, char *argv[]) {
	FILE *fin;
	bool res;
	int word_no;
	
	if (argc < 3) {
		printf("Uzycie: extract_ngrams <output> <lista_plikow_korpusu>\n");
		return 1;
	}
	
	fout = fopen(argv[1], "w");
	assert(fout != NULL);
	
	for (int k=2; k<argc; ++k) {
		printf("\nProcessing '%s'...", argv[k]);
		fflush(stdout);
		fin = fopen(argv[k], "r");
		
		while (!get_token(fin, token)) {
			if (starts_with(token, "<chunk ")) {
				// flush all
				last_tag.clear();
				prelast_tag.clear();
				cur_tag.clear();
				word_no = 0;
			}
			else if (equal(token, "</tok>")) {
			}
			else if (equal(token, "<orth>")) {
				res = get_token(fin, token);
				assert(!res);
				assert(!starts_with(token, "<"));
				++word_no;
				cur_word = token;
			}
			else if (equal(token, "<lex disamb=\"1\">")) {
				get_token(fin, token); // <base>
				get_token(fin, token); // bezokolicznik
				get_token(fin, token); // </base>
				
				res = get_token(fin, token);
				assert(!res);
				assert(equal(token, "<ctag>"));
				
				res = get_token(fin, token);
				assert(!res);
				assert(!starts_with(token, "<"));
				
				prelast_tag = last_tag;
				last_tag = cur_tag;
				cur_tag = token;
				
				while (!get_token(fin, token))
					if (equal(token, "</tok>"))
						break;
						
				// aktualizuj statystyki
 				if (!prelast_tag.empty()) incr_in_map(tag_trigrams, make_trigram(prelast_tag, last_tag, cur_tag));
 				if (!last_tag.empty()) incr_in_map(tag_bigrams, make_bigram(last_tag, cur_tag));
 				incr_in_map(tag_unigrams, make_unigram(cur_tag));
 				if (!last_tag.empty()) incr_in_map(word_tag_grams, make_trigram(last_tag, cur_tag, cur_word));
 				if (word_no == 2) {
 					incr_in_map(beginner_tag_bigrams, make_bigram(last_tag, cur_tag));
 				}
			}
			
		}
		
		fclose(fin);
	}
	
 	output_maps(fout, tag_trigrams, tag_bigrams, tag_unigrams, word_tag_grams, beginner_tag_bigrams);
	
	fclose(fout);
	
	printf("\n");

	return 0;
}
