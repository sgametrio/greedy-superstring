// Read from file many strings and find the shortest superstring using greedy algorithm
// USAGE: <executable> <input_file>
// OUTPUT: <superstring>

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_READS 100
#define MAX_READ_LENGTH 20

/** Compute max overlap between a suffix of read1 and a prefix of read2
 * @param read1: first string processed
 * @param read2: second string processed
 * @return max overlap. Returns strlen(read2) if and only if read2 is contained in its entirety in read1.
 */          
size_t overlap(char* read1, char* read2) {
    size_t max_overlap = 0;
    size_t read1_length = strlen(read1);
    size_t read2_length = strlen(read2);
    // i must be declared as int because size_t vars can't go <0
    for (int i = read1_length-1; i >= 0; i = i-1) {
        size_t j = 0;  // current number of overlapping characters
        bool overlapping = true;
        while (j < read2_length && i+j < read1_length && overlapping) {
            if (read1[i+j] != read2[j]) {
                overlapping = false;
            } else {
                j = j+1;
            }
        }
        // We have 3 cases for the cycle to exit:
        // 1) j == read2_length means that read2 is a substring of read1
        // this means that no other best solution can be found
        if (j == read2_length) {
            return read2_length;
        }
        // 2) i+j == read1_length means a prefix of read2 is a suffix of read1
        // Candidate to be the best local overlap
        if (i+j == read1_length) {
            max_overlap = j;
        }
        // 3) overlapping == false means that we must go on
    }
    return max_overlap;
}

int main (int argc, char** argv) {
    // <executable> <input_file>
    assert(argc > 1 && "Need one additional argument (filename)");

    FILE * input_file;
    input_file = fopen(argv[1], "r");
    assert(input_file != NULL && "Input file not found");

    // TODO: dynamic memory allocation
    char reads[MAX_READS][MAX_READ_LENGTH];
    size_t n = 0;
    while (fgets(reads[n], sizeof(reads[n]), input_file)) {
        n = n+1;
    }
    fclose(input_file);
    assert(n >= 1 && "File hasn't enough lines");

    // '\n' is counted as a valid character in strings so we have to strip it
    // This is done by inserting a terminator ('\0') at the last position
    for (size_t i = 0; i < n-1; i = i+1) {
        reads[i][strlen(reads[i])-1] = '\0';
    }

    /** Compute overlaps between every pair of reads and save them into a matrix
     * DP overlap matrix is n*n and it's not symmetric: for every pair (i,j), overlap
     * is computed as the overlap of a suffix of i to a prefix of j
     */
    int overlap_matrix[n][n];

    for (size_t i = 0; i < n; i = i+1) {
        for (size_t j = 0; j < n; j = j+1) {
            if (i != j)
                overlap_matrix[i][j] = overlap(reads[i], reads[j]);
            else
                overlap_matrix[i][j] = -1; // Do not calculate overlap between same strings
        }
    }

    bool used_strings[n];
    for (size_t i = 0; i < n; i = i+1) {
        used_strings[i] = false;
    }
    
    // For n-1 times I have to melt the two most overlapping strings
    // Compute n-1 times the max on a matrix (This has to be improved)
    for (size_t h = 0; h < n-1; h = h+1) {
        int max = -1;
        size_t ii, jj;    // temp indexes
        for (size_t i = 0; i < n; i = i+1) {
            if (used_strings[i] == true)    // Skip used strings
                continue;
            
            for (size_t j = 0; j < n; j = j+1) {
                if (i == j || used_strings[j] == true)  // do not count same strings and used ones
                    continue;

                if (overlap_matrix[i][j] > max) {
                    max = overlap_matrix[i][j];
                    ii = i;
                    jj = j;
                }
            }
        }
        used_strings[jj] = true;    // Mark jj as used
        if (max == strlen(reads[jj]))
            // reads[jj] is contained in its entirety in reads[ii] so:
            // * don't have to melt them
            // * don't have to compute again overlaps
            continue;

        // melt reads[ii] and reads[jj] so that the suffix of ii is prefix of jj by max characters
        strcat(reads[ii], reads[jj]+max);

        // Compute again new overlaps ONLY for new melt string
        for (size_t i = 0; i < n; i = i+1) {
            if (used_strings[i] == false && i != ii) {
                overlap_matrix[i][ii] = overlap(reads[i], reads[ii]);
                overlap_matrix[ii][i] = overlap(reads[ii], reads[i]);
            }
        }
    }
    // Resulting superstring is found in the unique "false" position
    for (size_t i = 0; i < n; i = i+1) {
        if (used_strings[i] == false)
            printf("%s\n", reads[i]);
    } 
    return 0;
}