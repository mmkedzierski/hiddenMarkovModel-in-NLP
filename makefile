progs=divide extract_ngrams gt_smoothen find_gammas untag score interpret_scores
taggers=hmm_tag unigram_tag
tools=ngram_tools.o tokenizer.o model_dumper.o 
objs=$(tools) tagger_wrapper.o get_prob.o
tagger_objs=hmm_tag.o unigram_tag.o
CC=g++

all: $(progs) $(taggers) generate_probs

hmm_tag : hmm_tag.o $(objs) get_prob.o
	$(CC) $^ -o $@

unigram_tag : unigram_tag.o $(objs) get_prob.o
	$(CC) $^ -o $@
	
generate_probs : % : %.cpp $(tools) get_prob.o
	$(CC) $^ -o $@

$(objs) $(tagger_objs) : %.o : %.cpp
	$(CC) -c $^

$(progs) : % : %.cpp $(tools)
	$(CC) $^ -o $@

.PHONY: clean_data clean

clean_data:
	rm -rf *.dat 

clean:
	rm -rf *~ $(progs) $(taggers) *.dat *.o generate_probs
	