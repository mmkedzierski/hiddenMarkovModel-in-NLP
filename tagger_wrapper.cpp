#include "tokenizer.h"
#include "ngram_tools.h"
#include "model_dumper.h"
#include "generic_tagger.h"

#define PROGRESS_BAR_SIZE 30

trigram_map_t tag_trigrams; // c(t1 t2 t3) 
bigram_map_t tag_bigrams; // c(t1 t2)
unigram_map_t tag_unigrams; // c(t1)
trigram_map_t word_tag_grams; // c(t1 t2 w2)
bigram_map_t beginner_tag_bigrams;

double unknown_tag_trigrams;
double unknown_tag_bigrams;
double unknown_tag_unigrams;
double unknown_word_tag_grams;
double unknown_beginner_tag_bigrams;

char token[1000];

int chunk_offset; // gdzie rozpoczyna sie w pliku aktualnie analizowany chunk
long fin_size;
long cur_bar_pos;

list<word_t> 	word_list;


void notify_step(long pos) {
	int next_bar_pos = (int)(1.0 * pos / fin_size * PROGRESS_BAR_SIZE);
	while (cur_bar_pos < next_bar_pos) {
		printf("#");
		fflush(stdout);
		++cur_bar_pos;
	}
}


// sprawdza, czy tag, ktory aktualnie czytamy w pliku wejsciowym (lex) 
// jest tym, ktory wybralismy. Jezeli tak - wstawia klauzule 'disamb="1"'
// w kazdym przypadku przetwarza caly tekst od (po) <lex> do </lex>
void try_put_disamb(FILE *fin, FILE *fout, word_t& w) {
	char base[1000];
	
	get_token(fin, token);
	assert(equal(token, "<base>"));
	get_token(fin, base);
	get_token(fin, token);
	assert(equal(token, "</base>"));
	get_token(fin, token);
	assert(equal(token, "<ctag>"));
	get_token(fin, token);
	
	// sprawdzenie
	if (equal(token, w.chosen_tag.c_str()))
		fprintf(fout, "<lex disamb=\"1\"><base>%s</base><ctag>%s</ctag></lex>", base, token);
	else
		fprintf(fout, "<lex><base>%s</base><ctag>%s</ctag></lex>", base, token);
	
	get_token(fin, token);
	assert(equal(token, "</ctag>"));
	get_token(fin, token);
	assert(equal(token, "</lex>"));
}



int main(int argc, char *argv[]) {
	FILE *fmodel, *fin, *fout;
	char fname[100];
	int ret;
	int nested_chunks;
	
	if (argc < 3) {
		printf("Uzycie: %s <probability_model> <lista_plikow_korpusu>\n", argv[0]);
		return 1;
	}
	
	fmodel = fopen(argv[1], "r");
	assert(fmodel != NULL);
	
	input_unknowns(fmodel, unknown_tag_trigrams, unknown_tag_bigrams, unknown_tag_unigrams,
		unknown_word_tag_grams, unknown_beginner_tag_bigrams);
	input_maps(fmodel, tag_trigrams, tag_bigrams, tag_unigrams, word_tag_grams, beginner_tag_bigrams);
		
	fclose(fmodel);

	for (int k=2; k<argc; ++k) {
		fin = fopen(argv[k], "r");
		sprintf(fname, "%s.tagged", argv[k]);
		fout = fopen(fname, "w");
		
		// oblicz wielkosc pliku
		fseek(fin, 0L, SEEK_END);
		fin_size = ftell(fin);
		rewind(fin);
		
		// zresetuj pasek postepu
		cur_bar_pos = 0;
		
		printf("\nTagging '%s'...\t", argv[k]);
		for (int i=0; i<PROGRESS_BAR_SIZE; ++i)
			printf("-");
		for (int i=0; i<PROGRESS_BAR_SIZE; ++i)
			printf("\b");
		fflush(stdout);
		
		// przepisz naglowek
		while (!get_token(fin, token)) {
			fprintf(fout, "%s", token);
			if (starts_with(token, "<chunkList"))
				break;
		}
		// znak konca linii
		get_token(fin, token);
		fprintf(fout, "%s", token);
		
		
		// *** glowna petla przetwarzajaca plik
		nested_chunks = 0;
		while (!get_token(fin, token)) {
			notify_step(ftell(fin));
		
			// wstepne przetworzenie tokena
			if (equal(token, "</chunkList>")) {
				fprintf(fout, "%s", token);
				break;
			}
			if (starts_with(token, "<chunk ")) {
				if (!nested_chunks) {
					// nowy chunk do otagowania!
					chunk_offset = ftell(fin) - strlen(token);
					word_list.clear();
				}
				++nested_chunks;
			}
			
			
			// zbieranie informacji o chunku
			
			if (equal(token, "<orth>")) {
				get_token(fin, token);
				word_t w;
				w.word = token;
				word_list.push_back(w);
				get_token(fin, token);
				assert(equal(token, "</orth>"));
				continue;
			}
			
			if (starts_with(token, "<lex")) {
				get_token(fin, token);
				assert(equal(token, "<base>"));
				get_token(fin, token);
				get_token(fin, token);
				assert(equal(token, "</base>"));
				get_token(fin, token);
				assert(equal(token, "<ctag>"));
				get_token(fin, token);
				word_list.back().tags.push_back(token);
				get_token(fin, token);
				assert(equal(token, "</ctag>"));
				get_token(fin, token);
				assert(equal(token, "</lex>"));
				continue;
			}
			
			
			// czy juz nalezy wypisac otagowany chunk?
			if (equal(token, "</chunk>")) {
				--nested_chunks;
				
				if (!nested_chunks) {
					fflush(stdout);
					do_tag(
						word_list,
						tag_trigrams,
						tag_bigrams, 
						tag_unigrams, 
						word_tag_grams, 
						beginner_tag_bigrams,
						unknown_tag_trigrams,
						unknown_tag_bigrams,
						unknown_tag_unigrams,
						unknown_word_tag_grams,
						unknown_beginner_tag_bigrams
					);
				
					// skonczyl sie ten chunk - wypisz output!
					fseek(fin, chunk_offset, SEEK_SET);
					
					list<word_t>::iterator word_itor = word_list.begin();
					while (!get_token(fin, token)) {
						if (starts_with(token, "<lex")) {
							try_put_disamb(fin, fout, *word_itor);
							continue;
						}
								
						if (equal(token, "</tok>"))
							++word_itor;
								
						fprintf(fout, "%s", token);
						if (starts_with(token, "<chunk "))
							++nested_chunks;
						if (equal(token, "</chunk>"))
							--nested_chunks;
						if (!nested_chunks)
							break;
					}
					// znak konca linii
					get_token(fin, token);
					fprintf(fout, "%s", token);
				}
			}
		}
		
		// przepisz koncowke
		while (!get_token(fin, token))
			fprintf(fout, "%s", token);
 		fprintf(fout, "\n");
		
		notify_step(fin_size);
		
		fclose(fin);
		fclose(fout);
	}
	
	printf("\n");
	
	return 0;
}
