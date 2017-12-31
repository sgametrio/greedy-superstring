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


//  @return the max overlap between a suffix of read1 and a prefix of read2.
//          returns strlen(read2) if and only if read2 is contained in read1
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

/*
*   contained è true se un read è contenuto interamente nell'altro. Per sapere quale dei due contiene l'altro basta confrontare le lunghezze, il maggiore contiene il minore, la fusione sarà uguale al maggiore visto che il minore non ci dà ulteriori informazioni
*   @return melt string 
*/
char* fuse_overlap (char* read1, char* read2) {
    int suffix_overlap_1 = overlap(read1, read2);
    if (suffix_overlap_1 == strlen(read2)) {
        // read2 è contenuta in read1
        return read1;
    }
    int suffix_overlap_2 = overlap(read2, read1);
    if (suffix_overlap_2 == strlen(read1)) {
        // read1 è contenuta in read2
        return read2;
    }

    // Fondi basandoti sul massimo overlap
    if (suffix_overlap_1 > suffix_overlap_2) {
        // un suffisso di read1 è prefisso di read2 
        // ritorna la concatenazione di read1 con la restante parte di read2
        // sfrutto come offset il numero di caratteri che overlappano
        printf("Read 1 è prefisso di %d", suffix_overlap_1);
        strcat(read1, read2+suffix_overlap_1);
        return read1;
    }

    printf("Read 2 è prefisso di %d", suffix_overlap_2);
    strcat(read2, read1+suffix_overlap_2);
    return read2;    
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
        // copy reads in another structure so that I can operate as I want on source
        //strcpy(temp[n], reads[n]);
        n = n+1;
    }
    // strip '\n' from n-1 strings
    for (int i = 0; i < n-1; i = i+1) {
        reads[i][strlen(reads[i])-1] = '\0';
    }
    /*
    for (int i = 0; i < n; i = i+1) {
        printf("%s", reads[i]);
    }
    */

    // operate greedy on a generalized suffix array (how??)

    /*  Altra implementazione stupida:
        Tengo una superstringa temp e calcolo ogni volta l'overlap su quella
        Fondo la superstringa temp con la read che ha massimo overlap
        Ripeto finchè non ho usato tutte le stringhe
    */
    char temp_superstring[MAX_SUPERSTRING_LENGTH] = "";
    bool used_strings[n];
    int remaining_strings = n;
    int strings_overlap[n];
    for (int i = 0; i < n; i++) {
        used_strings[i] = false;
    }

    printf("0: %s di lunghezza %ld", reads[0], strlen(reads[0]));
    printf("1: %s", reads[1]);
    printf("n: %s di lunghezza %ld", reads[n-1], strlen(reads[n-1]));
    printf("=: %s", fuse_overlap(reads[0], reads[1]));
    return 0;
    /*
    while (remaining_strings > 0) {
        for (int i = 0; i < n; i = i+1) {
            // Calcolo overlap con temp_superstring
            overlap(temp_superstring, reads[i])
        }
        
        remaining_strings--;
    }
    */






    /********* Implementazione stupida  (devo far rimanere sotto O(n^3)) **********/
    /*
    // Calcolo overlap (numero caratteri che non overlappano) fra tutte le reads e salvo in una matrice di overlap
    // La matrice è n*n e l'overlap non è simmetrico. Per ogni coppia (i,j) si calcola overlap di un suffisso di i come prefisso di j
    uint32_t overlap_matrix[n][n];

    // COMPLESSITA'???
    for (int i = 0; i < n; i = i+1) {
        for (int j = 0; j < n; j = j+1) {
            // Do not calculate overlap between same string
            if (i != j)
                overlap_matrix[i][j] = overlap(reads[i], reads[j]);
            else
                overlap_matrix[i][i] = -1; 
        }
    }

    // Min su matrice di overlap per trovare indici i e j
    // Fondo i e j mettendo a MAX_READ_LENGTH+1 tutti gli overlap in posizione i e in posizione j aggiorno tutti gli overlap e salvo la stringa fusa
    // Ricorro fino a che min = MAX_READ_LENGTH
    // concateno tutti i reads rimanenti (< MAX_READ_LENGTH + 1)

    */

    fclose(input_file);
    return 0;
}