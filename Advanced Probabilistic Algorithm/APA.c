#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gmp.h>

int test(mpz_t x, long k, int* length_of_sequence);

int main(int argc, char const *argv[])
{
    char* message = "Benvenuta/o nel Primality Test, si prega di seguire questa sintassi:\n\t-x Per indicare il valore da testare.\n\t-t per indicare il numero di test da superare per poter essere considerato primo.\n\t-r Per indicare che si vuole operare su un insieme di valori [start] [finish] [step]\n\t-s Per salvare il tempo di esecuzione e i risultati in un file .csv\n\t-b Per specificare le cifre decimali di partenza e quelle di fine [start] [finish] \n";
    int t = 8;
    int startPlaces = -1;
    int endPlaces = -1;
    mpz_t start;
    mpz_t end;
    mpz_t step;
    int save = 0;
    int noHeader = 0;
    int lenseq = 0;
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
                i++;
                mpz_init_set_str(start, argv[i], 10);
                mpz_init_set_str(end, argv[i], 10);
                mpz_init_set_ui(step, 1);
            }
            else if(strcmp("-t", argv[i]) == 0)
            {
                t = atoi(argv[++i]);
            }
            else if(strcmp("-r", argv[i]) == 0){
                mpz_init_set_str(start, argv[++i], 10);
                mpz_init_set_str(end, argv[++i], 10);
                mpz_init_set_str(step, argv[++i], 10);
            }
            else if(strcmp("-b", argv[i]) == 0){
                startPlaces = atoi(argv[++i]);
                endPlaces = atoi(argv[++i]);
                //initialize start and end variable
                char* str1 = malloc(sizeof(char) * startPlaces);
                char* str2 = malloc(sizeof(char) * endPlaces);

                time_t t;
                srand((unsigned) time(&t));
                for(int i = 0; i < startPlaces; i++)
                {
                    str1[i] = rand() % 2 + '0';
                }
                for(int i = 0; i < endPlaces; i++)
                {
                    str2[i] = rand() % 2 + '0';
                }
                
                mpz_init_set_str(start, str1, 2);
                mpz_init_set_str(end, str2, 2);
                mpz_init_set_ui(step, 0);

                free(str1);
                free(str2);
            }
            else if(strcmp("-s", argv[i]) == 0){
                save = 1;
            }
            else if(strcmp("-nh", argv[i]) == 0){
                noHeader = 1;
            } 
            else{
                puts(message);
                exit(0);
            }
        }
    }

    gmp_printf("\n .\tStart: %Zd\n .\tEnd: %Zd\n .\tStep: %Zd\n .\tsaving: %s\n .\tTrials: %d\n" ,start, end , step, (save == 1) ? "true" : "false", t);

    FILE *file;
    if(save)
    {
        char str[10000];
        gmp_sprintf(str, "./APAResults-%Zd-%Zd-%Zd.csv", start, end, step);
        file = fopen(str, "w");
        if(file == NULL){
            printf("An error while opening the file occurred.\n");
            file = fopen("NameTooLong.csv", "w");
        }
    } 

    if(save && !noHeader) gmp_fprintf(file,"Input, Time In Microseconds, isPrime, Sequence Length\n");
    while(mpz_cmp(start, end) <= 0)
    {
        
        clock_t startTime = clock();
        //What will be measured
        int ret = test(start, t, &lenseq);
        clock_t endTime = clock();
        double cpuTimeUsed = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;
        long timeInMicroiseconds = cpuTimeUsed * 1000000;
        gmp_printf("%Zd is deemed %s, it took the algorithm %ld µs and a sequence of %d elements\n", start, ret ? "PRIME" : "NOT PRIME", timeInMicroiseconds, lenseq);
        if(save)
        {
            gmp_fprintf(file,"%Zd,%d,%d,%d\n",start, timeInMicroiseconds, ret, lenseq);
            fflush(file);
        }

        if(startPlaces == -1)
        {
            mpz_add(start, start, step);
        }
        else
        {
            mpz_mul_ui(start, start, 2);
            mpz_add_ui(start, start, 1);
        }
    }
    if(save) fclose(file);

    mpz_clear(start);
    mpz_clear(end);
    mpz_clear(step);

    return 0;
}

/**
 * @brief A function that performs the primality test
 * 
 * @returns 1 if the number is probably prime, 0 if the number is composite.
 * 
 * @param x Number to understand wether it is prime or composite.
 * @param k Number of trials.
 * 
 */
int test(mpz_t x, long k, int* length_of_sequence)
{
    int ret = 1;
    *length_of_sequence = 0;
    if(mpz_cmp_ui(x,2) <= 0) return 0;
    if(mpz_cmp_ui(x,2) == 0) return 1;
    else if(mpz_tstbit(x, 0) == 0) return 0;
    int number_of_subdivisions;
    mpz_t* sequence = NULL;
    //Generating the seed
    gmp_randstate_t state;
    gmp_randinit_default(state);

    mpz_t x_minus_one;
    mpz_init(x_minus_one);
    mpz_t x_copy;
    mpz_init(x_copy);
    mpz_t a;
    mpz_init(a);
    mpz_t fermat;
    mpz_init(fermat);

    //For k trials
    for (int i = 0; i < k; i++)
    {
        //x - 1
        mpz_sub_ui(x_minus_one,x,1); 
        //A copy of x we will work on
        mpz_set(x_copy,x);
        //extract random value between 1 and x-1
        mpz_urandomm(a, state, x_minus_one);
        mpz_add_ui(a,a,1);
        
        //Fermat Test
        mpz_powm(fermat,a,x,x);  //fermat = a ^ (x) mod x
        if(mpz_cmp(fermat,a) != 0)
        {
            ret = 0; //x is composite
            break;
        } 

        //Calculate the amount of elements in the sequence which is equal to the number of zeros at the least significant positions
        if(i == 0)
        {
            number_of_subdivisions = mpz_scan1(x_minus_one,0);
            *length_of_sequence = number_of_subdivisions;
            sequence = malloc(sizeof(mpz_t) * number_of_subdivisions);
        }
        
        mpz_sub_ui(x_copy,x_copy,1); //x_copy -= 1;
        for(int j = 0; j < number_of_subdivisions; j++)
        {
            //Save the result
            if(i == 0) mpz_init_set_ui(sequence[j], 0);
            //Shift the input to the right by one
            mpz_fdiv_q_2exp(x_copy,x_copy,1); //x_copy /= 2
            mpz_powm(sequence[j], a, x_copy, x);
        }
        //Now the sequence contains the powers of a in decreasing order
        
        //We check if the last element is 1 or if any of the others is x-1
        int condition1 = mpz_cmp_ui(sequence[number_of_subdivisions - 1], 1) == 0;
        if(!condition1)
        {
            int condition2 = 0;

            for(int j = number_of_subdivisions - 1; j>=0; j--)
            {
                int cmp = mpz_cmp(sequence[j], x_minus_one);
                if(cmp == 0)
                { 
                    condition2 = 1;
                    break;
                }
            }
            if(!condition2) 
            {
                ret = 0; //x is composite
                break;
            }
        }
    }
    mpz_clear(x_minus_one);
    mpz_clear(x_copy);
    mpz_clear(a);
    mpz_clear(fermat);
    free(sequence);
    gmp_randclear(state);
    return ret; //x is probably prime
} 
