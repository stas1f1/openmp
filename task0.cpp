#include <ctype.h>
#include <omp.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {

    int i = 0;
    bool state = (isspace(argv[1][0]) != 0);

    int count = 0;
    if (!state)
        count += 1;
    char cur;

    while (i < strlen(argv[1])) {
        if (isspace(argv[1][i]) && !state) 
            state = true;
        
        else if (!isspace(argv[1][i]) && state) {
            state = false;
            count += 1;
        }
        i += 1;
    }

    printf("# of words in %s : %d\n", argv[1], count);
}