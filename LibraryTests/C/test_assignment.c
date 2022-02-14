#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

// A very simple program to run factorial on large numbers with arbitrary precision
int main(int argc, char const *argv[])
{
    // Variables
    long startingValue, endingValue, step;
    mpz_t n;
    clock_t start, end;
    double cpuTimeUsed;
    srand(time(NULL));

    // Parameters
    printf("Insert the starting value: ");
    scanf("%ld", &startingValue);
    printf("Insert the ending value: ");
    scanf("%ld", &endingValue);
    printf("Insert the step: ");
    scanf("%ld", &step);

    // Initialize the file writer
    FILE *file = fopen("./CAssignmentResults.csv", "w");
    fprintf(file, "Value,Time Taken to Assign Values (C)\n");
    fflush(file);

    // Calculating the specified number of times
    for (long trial = startingValue; trial <= endingValue; trial += step)
    {
        // Building the string
        char *str = malloc(trial * sizeof(char));
        for (long i = 0; i < trial; i++)
        {
            str[i] = (char)(rand() % 10 + '0');
        }

        start = clock();

        // Initialization And set to a given String
        mpz_init_set_str(n, str, 10);

        end = clock();

        cpuTimeUsed = ((double)(end - start)) / CLOCKS_PER_SEC;
        long timeInMicroiseconds = cpuTimeUsed * 1000000;

        // Printing the result
        printf("\nTime taken by %ld long input = %ld microseconds\n", trial, timeInMicroiseconds);
        fprintf(file, "%ld,%ld\n", trial, timeInMicroiseconds);
        fflush(file);
    }

    fclose(file);
    printf("The test is over, check out the results in ./CAssignmentResults\n");

    return 0;
}
