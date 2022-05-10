#include <gmp.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

void int_root(mpz_t res, mpz_t x, unsigned long n);

int main(int argc, char** argv)
{

    mpz_t x;
    mpz_init(x);

    unsigned long root = 12; 

    char* message = "Welcome to the Newton Root Approximation Test\nPlease specify the nth root after a \t-n\nPlease specify the value after a \t-x";
    if(argc < 5)
    {
        puts(message);
        return 0;
    }
    else
    {
        for(int i = 1; i < argc; i++)
        {
            if(strcmp("-x", argv[i]) == 0)
            {
                mpz_set_str(x, argv[++i], 10);
            }
            else if(strcmp("-n", argv[i]) == 0)
            {
                root = atoi(argv[++i]);
            }
            else{
                puts(message);
                exit(0);
            }
        }
    }
    
    mpz_t res;
    mpz_init(res);
    int_root(res, x, root);

    gmp_printf("\nThe final result of the calculation %d√(%Zd) is %Zd\n",root, x, res);
    gmp_printf("%Zd ^ %d = ", res, root);
    mpz_pow_ui(res, res, root);
    gmp_printf("%Zd\n", res);

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
    if(n == 0 || n == 1)
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

    mpf_set_d(solution_precision, 0.5); //we stop when we reach 1/2 precision
    mpf_set_d(diff, 1);
    mpf_set_z(xk,x);
    mpf_set_z(x_const,x);
    mpf_set_ui(tmp, 0);
    mpf_set_ui(y, 0);

    int i = 0;

    while(mpf_cmp(diff, solution_precision) >= 0) 
    {
        //Now we calculate the new value of x, but first we store the old one in y
        mpf_set(y,xk);
        //new value of xk will be [(n-1)xk + x/(xk^(n-1))] / n
        mpf_pow_ui(tmp, xk, n-1); //tmp = xk^(n-1)
        mpf_div(tmp, x_const,tmp); //tmp = x / tmp
        mpf_mul_ui(xk, xk, n-1); //xk = xk * n-1
        mpf_add(xk, xk, tmp); //xk = xk + tmp
        mpf_div_ui(xk, xk, n); //xk = xk / n

        i++;

        gmp_printf("The %dth term of the succession is:\t%Ff\n", i, xk);
        mpf_pow_ui(diff, xk, n);
        mpf_sub(diff, diff, x_const); //diff = xk^n - x
        gmp_printf("Current Error:\t%Ff (Stopping when the error is less than 0.5)\n", diff);

    }
    mpz_set_f(res, xk); //This function also truncates the number

    mpf_clear(xk);
    mpf_clear(x_const);
    mpf_clear(tmp);
    mpf_clear(solution_precision);
    mpf_clear(diff);
    mpf_clear(y);
}