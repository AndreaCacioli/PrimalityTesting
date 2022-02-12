#include <stdio.h>
#include <gmp.h>
#include <time.h>

// A very simple program to run factorial on large numbers with arbitrary precision
int main(int argc, char const *argv[])
{
    //Variables
    int limit;
    mpz_t n, mult;
    clock_t start, end;
    double cpu_time_used; 

    //Read from input
    scanf("%d", &limit);

    //Initialization
    mpz_init_set_ui(n, 1);
    mpz_init(mult);

    //Actual Calculation
    start = clock();
    for (int i = 1; i <= limit; i++)
    {
        mpz_set_ui(mult, i); //Have to initialize each time because mpz_mult requires mpz_t types
        mpz_mul(n, n, mult); // n = n * mult
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    //Printing the result
    printf("%d! = ", limit);
    mpz_out_str(stdout, 10, n);
    printf("\n");
    printf("\nTime taken by this trial = %f seconds\n",cpu_time_used);

    return 0;
}
