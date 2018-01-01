// Read from file many strings of length K and find the greedy lowest superstring 
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
#define MAX_SUPERSTRING_LENGTH 2000


/*
 *  @return the max overlap between a suffix of read1 and a prefix of read2.
 *          returns strlen(read2) if and only if read2 is contained in read1.
 */          
int overlap(char* read1, char* read2) {
    int max_overlap = 0;
    int read1_length = strlen(read1);
    int read2_length = strlen(read2);
    for (int i = read1_length-1; i >= 0; i = i-1) {
        int j = 0;
        bool overlapping = true;
        while (j < read2_length && i+j < read1_length && overlapping) {
            if (read1[i+j] != read2[j]) {
                overlapping = false;
            } else {
                j = j+1;
            }
        }
        // j is the current number of characters overlapping
        // We have 3 cases for the cycle to exit:
        // 1) j == read2_length means that read2 is a substring of read1
        // this means that no other best solution can be found
        if (j == read2_length) {
            return read2_length;
        }
        // 2) i+j == read1_length means a prefix of read2 is a suffix of read1
        // Candidate to be the best local overlap
        if (i+j == read1_length) {
            // update max_overlap
            max_overlap = j;
        }
        // 3) overlapping == false means that we must go on
    }
    return max_overlap;
}

int main (int argc, char** argv) {
    // <executable> <input_file>
    assert(argc > 1 && "Need exactly one additional argument (filename)");

    // Read from file
    FILE * input_file;
    input_file = fopen(argv[1], "r");
    assert(input_file != NULL);

    // Assuming first line is the superstring so I can check the output
    char superstring[MAX_SUPERSTRING_LENGTH];
    assert(fgets(superstring, sizeof(superstring), input_file));
    //printf("%s", superstring);

    // TODO: dynamic memory allocation
    char reads[MAX_READS][MAX_READ_LENGTH];
    char temp[MAX_READS][MAX_READ_LENGTH];
    int n = 0;
    while (fgets(reads[n], sizeof(reads[n]), input_file)) {
        n = n+1;
    }

    fclose(input_file);

    // strip '\n' read from file from n-1 strings
    for (int i = 0; i < n-1; i = i+1) {
        reads[i][strlen(reads[i])-1] = '\0';
    }

    // Calcolo overlap (numero caratteri che non overlappano) fra tutte le reads e salvo in una matrice di overlap
    // La matrice è n*n e l'overlap non è simmetrica: per ogni coppia (i,j) si calcola overlap di un suffisso di i come prefisso di j
    int overlap_matrix[n][n];

    // COMPLESSITA'??? (da calcolare)
    for (int i = 0; i < n; i = i+1) {
        for (int j = 0; j < n; j = j+1) {
            // Do not calculate overlap between same string
            if (i != j)
                overlap_matrix[i][j] = overlap(reads[i], reads[j]);
            else
                overlap_matrix[i][i] = -1; 
        }
    }

    bool used_strings[n];
    for (int i = 0; i < n; i = i+1) {
        used_strings[i] = false;
    }
    // Max su matrice di overlap per trovare indici i e j
    // Fondo i e j mettendo a MAX_READ_LENGTH+1 tutti gli overlap in posizione i e in posizione j aggiorno tutti gli overlap e salvo la stringa fusa
    for (int h = 0; h < n-1; h = h+1) {
        // conto le stringhe fuse, devono essere tutte (n-1 fusioni)
        int max = -1;
        // temp indexes
        int ii = -1;
        int jj = -1;
        for (int i = 0; i < n; i = i+1) {
            if (used_strings[i] == true)
                continue;
            for (int j = 0; j < n; j = j+1) {
                // do not count same strings and used ones
                if (i == j || used_strings[j] == true)
                    continue;

                if (overlap_matrix[i][j] > max) {
                    max = overlap_matrix[i][j];
                    ii = i;
                    jj = j;
                }
            }
        }
        printf("Max: %d in posizione %d %d\n", max, ii, jj);
        // Ho trovato il max, fondo le stringhe i e j in modo che il suffisso di i sia prefisso di j
        printf("Fondo %s + %s\n", reads[ii], reads[jj]);
        strcat(reads[ii], reads[jj]+max);
        printf("Diventa %s\n", reads[ii]);
        // in posizione ii avrò la nuova stringa fusa e jj la segno come posizione utilizzata
        used_strings[jj] = true;

        // Ricalcolo gli overlap solo per la nuova temp superstring evitando le stringhe già usate
        for (int i = 0; i < n; i = i+1) {
            if (used_strings[i] == false && i != ii) {
                overlap_matrix[i][ii] = overlap(reads[i], reads[ii]);
                overlap_matrix[ii][i] = overlap(reads[ii], reads[i]);
            }
        }
    }
    // All'ultimo passo devo comunicare in che posizione si trova la superstringa risultato
    // sarà l'unica posizione settata a false
    for (int i = 0; i < n; i = i+1) {
        if (used_strings[i] == false)
            printf("SUPERSTRINGA: %s\n", reads[i]);
    }
    // Ricorro fino a che ho fuso tutte le stringhe 
    return 0;
}