#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

int test(mpz_t x, long k);

int main(int argc, char const *argv[])
{
    char* message = "Welcome to the Primality Test, please follow this syntax:\n\t-x Per indicare il valore da testare.\n\t-t per indicare il numero di test da superare per poter essere considerato primo.\n";
    int t = 8;
    mpz_t x;

    if(argc != 5)
    {
        puts(message);
        printf("%d", argc);
        return 0;
    }
    else
    {
        for(int i = 1; i < argc; i++)
        {
            if(strcmp("-x", argv[i]) == 0)
            {
                mpz_init_set_str(x, argv[++i], 10);
            }
            else if(strcmp("-t", argv[i]) == 0)
            {
                t = atoi(argv[++i]);
            }
            else{
                puts(message);
                exit(0);
            }
        }
    }

    gmp_printf("The number %Zd is %s\n", x, test(x,t) == 1 ? "Probably Prime!" : "Composite");

    return 0;
}

/**
 * @brief A function that performs the primality test
 * 
 * @returns  -1 if there is an error, 1 if the number is probably prime, 0 if the number is composite.
 * 
 * @param x Number to understand wether it is prime or composite.
 * @param k Number of trials.
 * 
 */
int test(mpz_t x, long k)
{
    if(mpz_cmp_ui(x,2) == 0) return 1;
    else if(mpz_tstbit(x, 0) == 0) return 0;
    long number_of_subdivisions;
    mpz_t* sequence;
    //Generating the seed
    gmp_randstate_t state;
    gmp_randinit_default(state);

    //x - 1
    mpz_t x_minus_one;
    mpz_init(x_minus_one);
    mpz_sub_ui(x_minus_one,x,1); 

    //For k trials
    for (int i = 0; i < k; i++)
    {
        //A copy of x we will work on
        mpz_t x_copy;
        mpz_init(x_copy);
        mpz_set(x_copy,x);

        //extract random value between 1 and x-1
        mpz_t a;
        mpz_init(a);
        mpz_urandomm(a, state, x_minus_one);
        mpz_add_ui(a,a,1);
        //TODO avoid repetitions on same test
        
        //fermat Test
        mpz_t fermat;
        mpz_init(fermat);
        mpz_powm(fermat,a,x,x);  //fermat = a ^ (x) mod x
        if(mpz_cmp(fermat,a) != 0)
        {
            gmp_printf("Fermat test FAILED at %Zd❌\n", a); 
            return 0; //x is composite
        } 
        // Fermat Test passed at a
        gmp_printf("Fermat test PASSED at %Zd ✅\n", a);

        //Calculate the amount of elements in the sequence which is equal to the number of zeros at the least significant positions
        if(i == 0)
        {
            number_of_subdivisions = mpz_scan1(x_minus_one,0);
            printf("found the number of subdivision to be %ld\n", number_of_subdivisions);
            sequence = malloc(sizeof(mpz_t) * number_of_subdivisions);
        }
        

        mpz_sub_ui(x_copy,x_copy,1); //x_copy -= 1;
        for(int j = 0; j< number_of_subdivisions; j++)
        {
            //Save the result
            if(i == 0) mpz_init(sequence[j]);
            //Shift the input to the right by one
            mpz_fdiv_q_2exp(x_copy,x_copy,1);
            mpz_powm(sequence[j], a, x_copy, x);
            gmp_printf("%Zd ^ %Zd = %Zd (mod %Zd)\n", a, x_copy, sequence[j], x);
           
        }
        //Now the sequence contains the powers of a in decreasing order
        gmp_printf("Sequence: [");
        for(int j = 0; j < number_of_subdivisions-1; j++)
        {
            gmp_printf("%Zd, ", sequence[j]);
        }
        gmp_printf("%Zd]\n", sequence[number_of_subdivisions - 1]);
        
        //We check if the last element is 1 or if any of the others is x-1
        int condition1 = mpz_cmp_ui(sequence[number_of_subdivisions - 1], 1) == 0;
        if(!condition1)
        {
             int condition2 = 0;

            for(int j = number_of_subdivisions - 1; j>=0; j--)
            {
                if(mpz_cmp(sequence[j], x_minus_one) == 0)
                { 
                    condition2 = 1;
                    break;
                }
            }
            if(!condition2) 
            {
                gmp_printf("Miller test FAILED❌:\n\tcondition1: %d,\n\tcondition2: %d\n",condition1, condition2);
                return 0; //x is composite
            }
        }
        gmp_printf("Miller test PASSED✅\n");
       
    }
    free(sequence);
    return 1; //x is probably prime
} 