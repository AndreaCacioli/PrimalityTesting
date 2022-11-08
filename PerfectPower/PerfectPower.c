#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

void int_root(mpz_t res, mpz_t x, unsigned long n);

int main(int argc, char **argv)
{
    int save = 0;
    mpz_t start;
    mpz_t end;
    mpz_t step;
    unsigned long startexp;
    unsigned long endexp;
    unsigned long stepexp;
    mpz_t result;

    mpz_init(start);
    mpz_init(end);
    mpz_init(step);
    mpz_init(result);

    char *message = "Welcome to the Newton Root Approximation Test\nPlease specify the nth root after a \t-n\nPlease specify the value after a \t-x\nBoth of the previous flags can be used in a range if needed: just use -xrange [start_value_for_x] [end_value_for_x] [x_step]\nIf you wish to save your calculations along with execution time in a csv file, use -s\n";
    if (argc < 5)
    {
        puts(message);
        return 0;
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            if (strcmp("-x", argv[i]) == 0)
            {
                mpz_set_str(start, argv[++i], 10);
                mpz_set_str(end, argv[i], 10);
                mpz_set_ui(step, 1);
            }
            else if (strcmp("-xrange", argv[i]) == 0)
            {
                mpz_set_str(start, argv[++i], 10);
                mpz_set_str(end, argv[++i], 10);
                mpz_set_str(step, argv[++i], 10);
            }
            else if (strcmp("-n", argv[i]) == 0)
            {
                startexp = atoi(argv[++i]);
                endexp = atoi(argv[i]);
                stepexp = 1;
            }
            else if (strcmp("-nrange", argv[i]) == 0)
            {
                startexp = atoi(argv[++i]);
                endexp = atoi(argv[++i]);
                stepexp = atoi(argv[++i]);
            }
            else if (strcmp("-s", argv[i]) == 0)
            {
                save = 1;
            }
            else
            {
                puts(message);
                exit(0);
            }
        }
    }

    gmp_printf("\n .\tStart: %Zd\n .\tEnd: %Zd\n .\tStep: %Zd\n .\tsaving: %s\n .\n", start, end, step, (save == 1) ? "true" : "false");

    FILE *file;
    if (save)
    {
        char str[10000];
        gmp_sprintf(str, "PP%Zd-%Zd-%Zd|%ld-%ld-%ld.csv", start, end, step, startexp, endexp, stepexp);
        file = fopen(str, "w");
        if (file == NULL)
        {
            printf("An error while opening the file occurred.\n");
            file = fopen("NameTooLong.csv", "w");
        }
    }

    if (save)
        gmp_fprintf(file, "Input, Root, Time In Microseconds, Result\n");

    while (mpz_cmp(start, end) <= 0)
    {
        for (unsigned long i = startexp; i <= endexp; i += stepexp)
        {
            clock_t startTime = clock();
            int_root(result, start, i);
            clock_t endTime = clock();
            double cpuTimeUsed = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;
            long timeInMicroiseconds = cpuTimeUsed * 1000000;

            gmp_printf("%ld√%Zd = %Zd\n", i, start, result);
            if (save)
            {
                gmp_fprintf(file, "%Zd,%ld,%ld,%Zd\n", start, i, timeInMicroiseconds, result);
                fflush(file);
            }
        }

        mpz_add(start, start, step);
    }
    if (save)
        fclose(file);
    return 0;
}

/**
 * @brief a function that calculates the integer part of an nth root
 *
 * @param res where the final result will be stored
 * @param x the input
 * @param n the exponent
 */
void int_root(mpz_t res, mpz_t x, unsigned long n)
{
    if (n == 0 || n == 1)
    {
        mpz_set(res, x);
        return;
    }

    const int precision = 1024;
    mpf_t solution_precision;
    mpf_t xk;
    mpf_t y;
    mpf_t x_const;
    mpf_t tmp;
    mpf_t diff;

    mpf_init2(xk, precision);
    mpf_init2(y, precision);
    mpf_init2(x_const, precision);
    mpf_init2(tmp, precision);
    mpf_init2(diff, precision);
    mpf_init2(solution_precision, precision);

    mpf_set_d(solution_precision, 0.5); // we stop when we reach 1/2 precision
    mpf_set_d(diff, 1);
    mpf_set_z(xk, x);
    mpf_set_z(x_const, x);
    mpf_set_ui(tmp, 0);
    mpf_set_ui(y, 0);

    int i = 0;

    while (mpf_cmp(diff, solution_precision) >= 0)
    {
        // Now we calculate the new value of x, but first we store the old one in y
        mpf_set(y, xk);
        // new value of xk will be [(n-1)xk + x/(xk^(n-1))] / n
        mpf_pow_ui(tmp, xk, n - 1); // tmp = xk^(n-1)
        mpf_div(tmp, x_const, tmp); // tmp = x / tmp
        mpf_mul_ui(xk, xk, n - 1);  // xk = xk * n-1
        mpf_add(xk, xk, tmp);       // xk = xk + tmp
        mpf_div_ui(xk, xk, n);      // xk = xk / n

        i++;

        mpf_pow_ui(diff, xk, n);
        mpf_sub(diff, diff, x_const); // diff = xk^n - x
    }

    mpz_set_f(res, xk); // This function also truncates the number

    mpf_clear(xk);
    mpf_clear(x_const);
    mpf_clear(tmp);
    mpf_clear(solution_precision);
    mpf_clear(diff);
    mpf_clear(y);
}