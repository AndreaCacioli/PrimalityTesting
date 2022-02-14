#include <stdio.h>
#include <gmp.h>
#include <time.h>

// A very simple program to run factorial on large numbers with arbitrary precision
int main(int argc, char const *argv[])
{
    // Variables
    long startingValue, endingValue, step;
    mpz_t n, mult;
    clock_t start, end;
    double cpuTimeUsed;

    // Parameters
    printf("Insert the starting value: ");
    scanf("%ld", &startingValue);
    printf("Insert the ending value: ");
    scanf("%ld", &endingValue);
    printf("Insert the step: ");
    scanf("%ld", &step);

    // Initialize the file writer
    FILE* file = fopen("./CFactorialResults.csv","w");
    fprintf(file, "Value,Time Taken to Process Factorial (C)\n");
    fflush(file);

    //Calculating the specified number of times
    for (long trial = startingValue; trial <= endingValue; trial += step)
    {
        // Initialization
        mpz_init_set_ui(n, 1);
        mpz_init(mult);
        start = clock();
        // Actual Calculation
        for (long i = 1; i <= trial; i++)
        {
            mpz_set_ui(mult, i); // Have to initialize each time because mpz_mult requires mpz_t types
            mpz_mul(n, n, mult); // n = n * mult
        }
        end = clock();
        cpuTimeUsed = ((double)(end - start)) / CLOCKS_PER_SEC;
        long timeInMicroseconds = cpuTimeUsed * 1000000;

        // Printing the result
        printf("\nTime taken by %ld! = %ld microseconds\n", trial, timeInMicroseconds);
        fprintf(file,"%ld,%ld\n",trial, timeInMicroseconds);
        fflush(file);
    }

    fclose(file);
    printf("The test is over, check out the results in ./CFactorialResults\n");

    return 0;
}
