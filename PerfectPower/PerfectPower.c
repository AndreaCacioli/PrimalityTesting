#include <gmp.h>
#include <stdio.h>
#include <time.h>

void int_root(mpz_t res, mpz_t x, unsigned long n);

int main()
{
    mpz_t res;
    mpz_t x;
    mpz_init(x);
    mpz_init(res);

    mpz_set_ui(x, 100);
    mpz_set_ui(res, 10);

    int_root(res, x, 2);

    gmp_printf("\nThe final result of the calculation is %Zd\n", res);

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
    const int precision = 1024;
    mpf_t xk;
    mpf_t y;
    mpf_t x_const;
    mpf_t tmp;

    mpf_init2 (xk, precision);
    mpf_init2 (y, precision);
    mpf_init2 (x_const, precision);
    mpf_init2(tmp, precision);

    mpf_set_z(xk,x);
    mpf_set_z(x_const,x);

    mpz_t xk_intpart, y_intpart; //The integer parts of the terms in the succession
    mpz_init_set_ui(xk_intpart, 0);
    mpz_init_set_ui(y_intpart, 1);
    int i = 0;

    while(mpz_cmp(xk_intpart,y_intpart) != 0)
    {
        //Now we calculate the new value of x, but first we store the old one in y
        mpf_set(y,xk);
        //new value of xk will be [(n-1)xk - x/(xk^(n-1))] / n
        mpf_mul_ui(xk, xk, n-1); //xk = xk * n-1
        mpf_pow_ui(tmp, xk, n-1); //tmp = xk^(n-1)
        mpf_div(tmp, x_const,tmp); //tmp = x / tmp
        mpf_sub(xk, xk, tmp); //xk = xk - tmp
        mpf_div_ui(xk, xk, n); //xk = xk / n

        //Now we update the int parts
        mpz_set_f(xk_intpart,xk);
        mpz_set_f(y_intpart, y);
        i++;

        gmp_printf("The %dth term of the succession is: %.*Ff\n", i, xk);

    }
    mpz_set_f(res, xk); //This function also truncates the number
    mpf_clear (xk);
    mpf_clear (x_const);
    mpf_clear (tmp);
    mpf_clear (y);
}