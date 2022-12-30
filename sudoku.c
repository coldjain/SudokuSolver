#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
Sudoku Solver with input validation

Programm askes for each row
    row 1: '0 0 0 0 0 8 9 6 1'
    row 2: ...
    seperate numbers with space (0 = empty spot)

*/




#define LENGTH 9

int solver(int (*pArr)[9]);

bool valid_number(int (*pArr)[9], int check_number, int max_occurence, int row_check_index, int column_check_index);

bool valid_input_format(char *pChar);

char *read_input(int n_chars_to_read);

void print_array(int (*pArr)[9], char *name);



int main() {
    int array[9][9] = {{0, 0, 0, 0, 0, 0, 0, 0, 0}, 
                       {0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0},
                       };

    int i, j, k;
    char *prow_input; 
    for (i = 0; i < LENGTH; i++) {
        printf("Enter row %d: ", i + 1);

        // reading 17 chars not inlcluding '\0'
        prow_input = read_input(17);  // returns pointer to char on the heap

        if (valid_input_format(prow_input)) {           
            // inserting the prow_input into array
            int a_index = 0; 
            for (j = 0; j <= 16; j += 2) { // j += 2, because we only want the numbers, no spaces
                array[i][a_index] = prow_input[j] - '0';
                a_index++;
            }

            // check for each number in the current row (except for 0 because 0 = empty spot) if its valid
            for (j = 0; j < LENGTH; j++) {
                if (array[i][j] == 0) {
                    continue;
                }

                if (!(valid_number(array, array[i][j], 1, i, j))) {
                    printf("(Invalid Number) The number %d you entered occurs more than ones\n", array[i][j]);
                    i -= 1;
                    break;
                }
            }
            
        } else {
            printf("(Wrong format) format e.g: '0 1 0 6 9 0 2 0 7' (0 = for empty spot)\n");
            i -= 1;
        }
        // free memory alloacted by read_input function
        free(prow_input);  

    }
    
    // solve and print Input and Solution
    printf("\n");
    print_array(array, "Input");
    solver(array);
    print_array(array, "Solution");


    return EXIT_SUCCESS;
}



int solver(int (*pArr)[9]) {
    int i, j, k;

    for (i = 0; i < LENGTH; i++) {
        for (j = 0; j < LENGTH; j++) {
            if (pArr[i][j] == 0) {
                for (k = 1; k < 10; k++) {
                    if (valid_number(pArr, k, 0, i, j)) {
                        pArr[i][j] = k;
                        if (solver(pArr) == 1) { // checking if solver found a solution
                            return 1;
                        }
                        pArr[i][j] = 0;
                    }
                }
                // failed to find a valid number at that spot
                // return 0, go to previous function call 
                return 0;
            }
        }
    }
    // this function call did not find a empty spot so
    // return 1 to indicate we found a solution
    return 1;
}



bool valid_number(int (*pArr)[9], int check_number, int max_occurence, int row_check_index, int column_check_index) {
    int i, j;
    int occurrences = 0;

    int box_y = (row_check_index / 3) * 3;     
    int box_x = (column_check_index / 3) * 3;   

    // checking 3x3 box          
    for (i = box_y; i < box_y + 3; i++) {
        for (j = box_x; j < box_x + 3; j++) {
            if (pArr[i][j] == check_number) {
                occurrences++;
            }
        }
    }

    if (occurrences > max_occurence) {
        return false;
    }

    occurrences = 0;

    for (i = 0; i < LENGTH; i++) {
        if (i == row_check_index) {
            // checking row
            for (j = 0; j < LENGTH; j++) {
                if (check_number == (*pArr)[j]) {
                    occurrences++;
                }
            }
        } else {
            // checking column
            if ((*pArr)[column_check_index] == check_number) {
                occurrences++;
            }
        }
        if (occurrences > max_occurence) {
            return false;
        }
        pArr++; // 36 bytes up in memory to the next array
    }

    return true;


}


bool valid_input_format(char *pChar) {
    int i;

    // less than 17 because, 9 numbers required + 8 spaces '0 0 0 0 0 0 0 0 0'
    if (strlen(pChar) < 17) {
        return false;
    }

    // make sure user written the numbers separated by a space if not return false
    for (i = 0; i < strlen(pChar); i++) {
        if (i % 2 == 0) {
            if (!(pChar[i] - '0' >= 0 && pChar[i] - '0' <= 9)) {
                return false;
            }
        } else {
            if (pChar[i] != ' ') {
                return false;
            }
        }
    }

    return true;

}


char *read_input(int n_chars_to_read) {
    // allocate memeory  ... + 1 for '\0'
    char *pChar = calloc(n_chars_to_read + 1, sizeof(char));

    int input;
    int i;
    i = 0;
    
    while ((input = getchar()) != '\n' && input != EOF) {
        // check whether maximum characters were inserted
        if (i < n_chars_to_read) {
            pChar[i] = input;
            i++;
        }
    }
    pChar[i] = '\0';

    return pChar;

}



void print_array(int (*pArr)[9], char *name) {
    int i, j;

    printf("%s\n", name);
    for (i = 0; i < LENGTH; i++) {
        if (i == 3 || i == 6) {
            printf("\n");
        }
        for (j = 0; j < LENGTH; j++) {
            printf("%d ", pArr[i][j]);
            if (j == 2 || j == 5) {
                printf(" ");
            }
        }
        printf("\n");
    }

    printf("\n");
    printf("\n");

}




/*
----------------------------

Input
{0, 4, 2, 9, 6, 7, 0, 1, 0},
{7, 0, 1, 0, 0, 2, 5, 6, 4},
{0, 6, 0, 0, 0, 4, 0, 0, 7},
{0, 0, 0, 1, 4, 6, 0, 8, 0},
{0, 2, 4, 0, 8, 0, 0, 5, 1},
{8, 1, 0, 0, 0, 0, 4, 0, 6},
{1, 0, 0, 0, 0, 9, 3, 0, 0},
{4, 0, 0, 0, 7, 0, 1, 2, 9},
{2, 7, 0, 3, 5, 1, 6, 0, 0},


Solution
{5, 4, 2, 9, 6, 7, 8, 1, 3},
{7, 9, 1, 8, 3, 2, 5, 6, 4},
{3, 6, 8, 5, 1, 4, 2, 9, 7},
{9, 3, 5, 1, 4, 6, 7, 8, 2},
{6, 2, 4, 7, 8, 3, 9, 5, 1},
{8, 1, 7, 2, 9, 5, 4, 3, 6},
{1, 8, 6, 4, 2, 9, 3, 7, 5},
{4, 5, 3, 6, 7, 8, 1, 2, 9},
{2, 7, 9, 3, 5, 1, 6, 4, 8},


----------------------------


Input
0 0 0 0 0 8 9 6 1
0 6 7 5 9 0 0 0 0
1 0 9 0 2 6 5 0 0
0 0 8 0 0 3 7 5 0
9 7 0 8 4 0 6 1 0
0 0 6 0 5 0 4 0 0
6 0 0 0 0 0 0 0 5
5 8 0 9 0 7 1 0 6
7 4 0 6 0 5 0 0 2


Solution
2 5 4 3 7 8 9 6 1
8 6 7 5 9 1 2 3 4
1 3 9 4 2 6 5 8 7
4 2 8 1 6 3 7 5 9
9 7 5 8 4 2 6 1 3
3 1 6 7 5 9 4 2 8
6 9 3 2 1 4 8 7 5
5 8 2 9 3 7 1 4 6
7 4 1 6 8 5 3 9 2

*/

