# Greedy approximation algorithm to find shortest common superstring

## Time

Given N reads of K max characters the algorithm takes O(N^2*K^2) to calculate the superstring

## Usage

Compile it and give him a file containing reads:

        gcc overlap_matrix.c
        ./a.out <input_file>

## Input format

The file containing reads must be made of N lines without blank lines at the end.

Every line contains a read. Check `input.txt` as an example file