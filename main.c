#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int random(void) {
    return rand();
}


/*
 *-----------------------------------------------------------------------------------------------------------
 * roll: Takes an input of amount of dice, faces on the dice, whether dice should be dropped from calculation
 *       and if so how many dice should be dropped. Simulates rolling a set of dice and returns the result.
 *-----------------------------------------------------------------------------------------------------------
 */
int roll(int amount, int faces, char mode, int dropAmount) {
    int rollResult;
    int rolls[amount];
    int result = 0;

    printf("\n");
    for( int i = 0; i < amount; i++ )    // Rolling each die and adding it to a list
    {
        rollResult = (random() % faces) + 1;
        rolls[i] = rollResult;
        if( i == 0 )
        {
            printf("     Rolled: %d", rollResult);
        }
        else
        {
            printf(" + %d", rollResult);
        }
    }
    printf("\n");

    if( mode == 'D' )   // Drop lowest X amount of results
    {
        int smallest;
        int smallestIndex;

        for( int j = 0; j < dropAmount; j++ )
        {
            smallest = faces + 1;
            for( int i = 0; i < amount; i++ )
            {
                if( rolls[i] < smallest && rolls[i] != 0 ) {
                    smallest = rolls[i];
                    smallestIndex = i;
                }
            }
            printf("  D Dropped: %d\n", rolls[smallestIndex]);
            rolls[smallestIndex] = 0;
        }
    }
    else if( mode == 'P' )    // Drop highest X amount of results
    {
        int largest;
        int largestIndex;

        for( int j = 0; j < dropAmount; j++ )
        {
            largest = rolls[0];
            largestIndex = 0;
            for( int i = 1; i < amount; i++ )
            {
                if( rolls[i] > largest )
                {
                    largest = rolls[i];
                    largestIndex = i;
                }
            }
            printf("  P Dropped: %d\n", rolls[largestIndex]);
            rolls[largestIndex] = 0;
        }
    }

    for( int i = 0; i < amount; i++ )    // Adding the rolls to the result
    {
        result += rolls[i];
    }

    return result;
}

/*
 *-----------------------------------------------------------------------------------------------------------
 * parser: Takes an expression in terms of 'd', 'D', 'P', '+' and ints. Strips the '+' and detects whether
 *         each term is a dice set or not, represented by the 'd'. If it is a dice set (1d6), it then strips
 *         the 'd' and uses the numbers either side to roll the set. It will also detect the mode if the term
 *         has one (2d20D1, 2d20DP1) and strip the 'D' or 'P' to input how many dice should be dropped. After
 *         converting all the terms it will print each term and then return a sum of all the results.
 *-----------------------------------------------------------------------------------------------------------
 */
int parser(char* str) {
    int amount, faces, result, dropAmount;
    int totalResult = 0;
    int i = 0;
    int terms = 0;
    int init_size = strlen(str);
    char *ptr = strtok(str, " ");
    char *array[init_size];
    char results[init_size];
    char operator = '+';
    int skip;
    int j = 0;

    while( ptr != NULL )    // Going through the original input and putting each item into an array
    {
        array[i++] = ptr;
        terms++;
        ptr = strtok(NULL, " ");
    }

    for( i = 0; i < terms; ++i )    // Checking if each item is a roll and adding it to the result
    {
        skip = 0;
        if( strchr(array[i], '+') != NULL)
        {
            operator = '+';
            skip = 1;
        }
        else if( strchr(array[i], '-') != NULL)
        {
            operator = '-';
            skip = 1;
        }
        else if( strchr(array[i], 'D') != NULL )    // The term is a roll with low dropping
        {
            ptr = strtok(array[i], "dD");
            amount = atoi(ptr);
            ptr = strtok(NULL, "dD");
            faces = atoi(ptr);
            ptr = strtok(NULL, "dD");
            dropAmount = atoi(ptr);
            result = roll(amount, faces, 'D', dropAmount);
        }
        else if( strchr(array[i], 'P') != NULL )    // The term is a roll with high dropping
        {
            ptr = strtok(array[i], "dP");
            amount = atoi(ptr);
            ptr = strtok(NULL, "dP");
            faces = atoi(ptr);
            ptr = strtok(NULL, "dP");
            dropAmount = atoi(ptr);
            result = roll(amount, faces, 'P', dropAmount);
        }
        else if ( strchr(array[i], 'd') != NULL )    // The term is a roll with no dropping
        {
            ptr = strtok(array[i], "d");
            amount = atoi(ptr);
            ptr = strtok(NULL, "d");
            faces = atoi(ptr);
            result = roll(amount, faces, '-', 0);
        }
        else    // The term is a modifier
        {
            result = atoi(array[i]);
        }

        if( skip == 0 )
        {
            if(operator == '+')
            {
                results[j] = result;
                totalResult += result;    // Add the result to the total result count
            }
            else
            {
                results[j] = -result;
                totalResult -= result;    // Minus the result to the total result count
            }
            j += 1;
        }
    }
    printf("\n");

    for( int i = 0; i < j; i++ )
    {
        if( i == 0)
        { 
            if( results[i] > 0)
            {
                printf("Calculation: %d", results[i]);
            }
            else
            {
                printf("Calculation: 0 - %d", -results[i]);
            }
        }
        else
        {
            if( results[i] > 0)
            {
                printf(" + %d", results[i]);
            }
            else
            {
                printf(" - %d", -results[i]);
            }
        }
    }
    printf("\n\n");

    return totalResult;
}

void self_test(int run_test);

int main(int argc, char** argv) {

    srand(time(0));    // Seed rand for use in roll

    if( (argc > 1) && (!strcmp(argv[1], "-t")) )
    {
        for( int i = 1; i < 18; i++ )
        {
            for( int j = 0; j < 100; j++ )
            {
                self_test(i);
            }
        }
    }
    else
    {
        char str[50];
        printf("\n      Input: ");
        fflush(stdout);
        fgets(str, 3, stdin);
        str[strcspn(str, "\n")] = 0;
        printf("     Result: %d\n\n",parser(str));
    }

    return 0;
}

/*
 *-----------------------------------------------------------------------------------------------------------
 * self_test: <description>
 *-----------------------------------------------------------------------------------------------------------
 */

void self_test(int run_test) {
    printf("\n\n\n\n");
    switch( run_test )
    {
        case 1:
        {
            char str[] = "1";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
        case 2:
        {
            char str[] = "1d20";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
        case 3:
        {
            char str[] = "2d20";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
        case 4:
        {
            char str[] = "1d20 + 1";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
        case 5:
        {
            char str[] = "2d20 + 1";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
        case 6:
        {
            char str[] = "2d20D1";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
        case 7:
        {
            char str[] = "2d20P1";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
        case 8:
        {
           char str[] = "3d20D2";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
        case 9:
        {
            char str[] = "3d20P2";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
        case 10:
        {
            char str[] = "2d20D1 + 1d20 + 1";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
        case 11:
        {
            char str[] = "2d20P1 + 1d20 + 1";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
        case 12:
        {
            char str[] = "1d20 + 2d20D1 + 1d20 + 2d20D1 + 1d20 + 2d20D1";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
        case 13:
        {
            char str[] = "1d20 + 2d20D1 + 2d20P1 + 3d20D2 + 3d20P2 + 1";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
        case 14:
        {
            char str[] = "4d20 - 3d20D2 - 1d20D1 - 1";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
        case 15:
        {
            char str[] = "5d20 - 4d20 + 4d20";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
        case 16:
        {
            char str[] = "1d20 - 3d20 + 5d20 - 7d20 + 9d20 - 11d20 + 13d20 - 15d20 + 13d20 - 11d20 + 9d20 - 7d20 + 5d20 - 3d20 + 1d20";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
        case 17:
        {
            char str[] = "1d20D0 - 3d20D1 + 5d20D2 - 7d20D3 + 9d20D4 - 11d20D5 + 13d20D6 - 15d20D7 + 13d20D6 - 11d20D5 + 9d20D4 - 7d20D3 + 5d20D2 - 3d20D1 + 1d20D0";
            printf("      Input: %s\n", str);
            printf("     Result: %d\n\n",parser(str));
            break;
        }
    }
}