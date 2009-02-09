# HMM based tagger

This is a tagger for Natural Languages based on Hidden Markov Model. 

## Usage

All programs in the package are compiled by issuing the `make` command in the main directory.
Note that the main script does calls make by itself.

The main script is `tagger`. It should be used as follows:

    tagger <body1> [<body2>]
    
where parameters denote catalogs with language corpuses in the IPI PAN format. It has
two modes:

It can be run in two modes:

- one-parameter - then it performs a 10-fold cross-validation in order to
    test the HMM tagger and compare it with a simple unigram tagger

- two-parameter - then it trains the model on the first given body and
    tags the second body using the created model.

After launching, the program prints all information to the standard output including a progress bar.
