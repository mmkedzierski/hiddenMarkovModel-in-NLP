#include "model_dumper.h"

#define TRIGRAMS 1
#define BIGRAMS 2
#define UNIGRAMS 3
#define WORD_TAG_GRAMS 4
#define BEGINNER_TAG_BIGRAMS 5


void input_maps
(
	FILE *fin,
	trigram_map_t& tag_trigrams, 
	bigram_map_t& tag_bigrams, 
	unigram_map_t& tag_unigrams, 
	trigram_map_t& word_tag_grams, 
	bigram_map_t& beginner_tag_bigrams
)
{
	char str1[1000], str2[1000], str3[1000];
	double num;
	int read_mode = TRIGRAMS;
	while (1) {
		if (fscanf(fin, "%s", str1) != 1)
			break;
			
		if (!strcmp("---", str1)) {
			++read_mode;
			continue;
		}
		switch (read_mode) {
			case TRIGRAMS:
				fscanf(fin, "%s%s%lf", str2, str3, &num);
				tag_trigrams[make_trigram(str1, str2, str3)] = num;
				break;
				
			case BIGRAMS:
				fscanf(fin, "%s%lf", str2, &num);
				tag_bigrams[make_bigram(str1, str2)] = num;
				break;
				
			case UNIGRAMS:
				fscanf(fin, "%lf", &num);
				tag_unigrams[make_unigram(str1)] = num;
				break;
				
			case WORD_TAG_GRAMS:
				fscanf(fin, "%s%s%lf", str2, str3, &num);
				word_tag_grams[make_trigram(str1, str2, str3)] = num;
				break;
				
			case BEGINNER_TAG_BIGRAMS:
				fscanf(fin, "%s%lf", str2, &num);
				beginner_tag_bigrams[make_bigram(str1, str2)] = num;
				break;
				
			default:
				perror("wrong read_mode?\n");
				exit(1);
		}
	}
}


void output_maps
(
	FILE *fout,
	trigram_map_t& tag_trigrams, 
	bigram_map_t& tag_bigrams, 
	unigram_map_t& tag_unigrams, 
	trigram_map_t& word_tag_grams, 
	bigram_map_t& beginner_tag_bigrams
)
{
	for (trigram_map_t::iterator itor = tag_trigrams.begin();  itor != tag_trigrams.end();  ++itor)
		fprintf(fout, "%s %s %s %.20lf\n", 
			itor->first.t[0].c_str(), itor->first.t[1].c_str(), itor->first.t[2].c_str(), itor->second);
	fprintf(fout, "---\n");
	
	for (bigram_map_t::iterator itor = tag_bigrams.begin();  itor != tag_bigrams.end();  ++itor)
		fprintf(fout, "%s %s %.20lf\n", 
			itor->first.t[0].c_str(), itor->first.t[1].c_str(), itor->second);
	fprintf(fout, "---\n");
	
	for (unigram_map_t::iterator itor = tag_unigrams.begin();  itor != tag_unigrams.end();  ++itor)
		fprintf(fout, "%s %.20lf\n", 
			itor->first.t[0].c_str(), itor->second);
	fprintf(fout, "---\n");
		
	for (trigram_map_t::iterator itor = word_tag_grams.begin();  itor != word_tag_grams.end();  ++itor)
		fprintf(fout, "%s %s %s %.20lf\n", 
			itor->first.t[0].c_str(), itor->first.t[1].c_str(), itor->first.t[2].c_str(), itor->second);
	fprintf(fout, "---\n");
	
	for (bigram_map_t::iterator itor = beginner_tag_bigrams.begin();  itor != beginner_tag_bigrams.end();  ++itor)
		fprintf(fout, "%s %s %.20lf\n", 
			itor->first.t[0].c_str(), itor->first.t[1].c_str(), itor->second);
		
	fprintf(fout, "\n");
}


void input_unknowns
(
	FILE *fin,
	double& unknown_tag_trigrams, 
	double& unknown_tag_bigrams, 
	double& unknown_tag_unigrams, 
	double& unknown_word_tag_grams, 
	double& unknown_beginner_tag_bigrams
)
{
	char ignorant[1000];
	double utri, ubi, uuni, uwt, ubeg;
	
	fscanf(fin, "%s%lf%lf%lf%lf%lf\n", ignorant, &utri, &ubi, &uuni, &uwt, &ubeg);
	unknown_tag_trigrams = utri;
	unknown_tag_bigrams = ubi;
	unknown_tag_unigrams = uuni;
	unknown_word_tag_grams = uwt;
	unknown_beginner_tag_bigrams = ubeg;
}


void output_unknowns
(
	FILE *fout,
	double& unknown_tag_trigrams, 
	double& unknown_tag_bigrams, 
	double& unknown_tag_unigrams, 
	double& unknown_word_tag_grams, 
	double& unknown_beginner_tag_bigrams
)
{
	fprintf(fout, "***_data_for_unknown:\n%.20lf\n%.20lf\n%.20lf\n%.20lf\n%.20lf\n\n", 
		unknown_tag_trigrams, unknown_tag_bigrams, unknown_tag_unigrams, 
		unknown_word_tag_grams, unknown_beginner_tag_bigrams);
}

