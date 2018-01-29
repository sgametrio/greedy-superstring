# Greedy approximation algorithm to find shortest common superstring

## Time

Given N reads of K max characters the algorithm takes O(N^2*K^2) to calculate the superstring

## Usage

Make it and run it passing as first argument the file containing input:

        make
        ./greedy-superstring <input_file>

As a working example, we include `input.txt`. Try running this:

        ./greedy-superstring input.txt

Output will be the shortest superstring found.

## Input format

The file containing reads must be made of a # of lines without blank lines at the end.

Every line is read as a single string. Check `input.txt` as an example file