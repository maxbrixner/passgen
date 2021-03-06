/* -------------------------------------------------------------------------- */

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "mersenne.h"

/* -------------------------------------------------------------------------- */

#define ERR_ARGUMENT -1
#define ERR_UNIQUE -2
#define ERR_ALPHABET_LENGTH -3

#define N 624
#define M 397

/* -------------------------------------------------------------------------- */

char *alphabet1 = "abcdefghijklmnopqrstuvwxyz";
char *alphabet2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char *alphabet3 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
char *alphabet4 =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
char *alphabet5 =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!$=?+#,;.:-"
    "_";

/* -------------------------------------------------------------------------- */

int uniqueCharacters(char *str) {
    int i = 0;
    int j = 0;
    int l = 0;

    l = strlen(str);

    for (i = 0; i < l; i++) {
        for (j = i + 1; j < l; j++) {
            if (str[i] == str[j]) return 0;
        }
    }

    return 1;
}

/* -------------------------------------------------------------------------- */

unsigned long mix(unsigned long a, unsigned long b, unsigned long c) {
    a = a - b;
    a = a - c;
    a = a ^ (c >> 13);
    b = b - c;
    b = b - a;
    b = b ^ (a << 8);
    c = c - a;
    c = c - b;
    c = c ^ (b >> 13);
    a = a - b;
    a = a - c;
    a = a ^ (c >> 12);
    b = b - c;
    b = b - a;
    b = b ^ (a << 16);
    c = c - a;
    c = c - b;
    c = c ^ (b >> 5);
    a = a - b;
    a = a - c;
    a = a ^ (c >> 3);
    b = b - c;
    b = b - a;
    b = b ^ (a << 10);
    c = c - a;
    c = c - b;
    c = c ^ (b >> 15);
    return c;
}

/* -------------------------------------------------------------------------- */

unsigned long randomNumber(unsigned long l, unsigned long u) {
    return (genrand_int32() % u + l);
}

/* -------------------------------------------------------------------------- */

void printHelp(FILE *stream) {
    fprintf(stream, "Usage: passgen <options>\n\n");
    fprintf(stream, "Generates random passwords using a Mersenne Twister.\n\n");

    fprintf(stream, "Options\n");
    fprintf(stream, "  --alphabet, -a    Set the alphabet. This can be any\n");
    fprintf(stream, "                    string or a number between 1-6 for\n");
    fprintf(stream, "                    one of the default alphabets.\n");
    fprintf(stream, "                    Default is 5.\n");
    fprintf(stream, "  --length, -l      Set the length of the password.\n");
    fprintf(stream, "                    Default is 25.\n");
    fprintf(stream,
            "  --count, -c       Set the number of passwords to create.\n");
    fprintf(stream, "                    Default is 30.\n");
    fprintf(stream, "  --seed, -s        Mix in random seed.\n");
    fprintf(stream,
            "                    If left empty this is the system time.\n\n");

    fprintf(stream, "Default Alphabets\n");
    fprintf(stream, "  1                Lower case letters\n");
    fprintf(stream, "  2                Upper case letters\n");
    fprintf(stream, "  3                Lower and upper case letters\n");
    fprintf(stream, "  4                Letters and numbers\n");
    fprintf(
        stream,
        "  5                Letters, numbers, common special characters\n\n");

    fprintf(stream, "Examples\n");
    fprintf(stream, "  $ passgen -l=10 -c=24 -a=\"abcdefg\"\n");
    fprintf(stream, "  Generates 24 passwords of length 10 using the\n");
    fprintf(stream, "  given alphabet.\n\n");
    fprintf(stream, "  $ passgen -l=10 -c=24 -a=3\n");
    fprintf(stream, "  Generates 24 passwords of length 10 using the\n");
    fprintf(stream, "  default alphabet no. 3.\n\n");
    fprintf(stream, "  $ passgen -s=263127394\n");
    fprintf(stream, "  Generates passwords mixing in the custom seed.\n");
}

/* -------------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    char *alphabet = alphabet5;
    int standardAlphabet = 0;
    int pwLength = 25;
    int pwCount = 30;

    int alphabet_size = 0;
    int alphabet_no = 0;

    int i, j = 0;
    unsigned long k = 0;
    int r = 0;

    double entropy = 0;
    double guessTime = 0;

    unsigned long seed = time(NULL);

    /* command line arguments */

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printHelp(stdout);
            return 0;
        } else if (strncmp(argv[i], "--alphabet=", 11) == 0)
            alphabet = argv[i] + 11;
        else if (strncmp(argv[i], "-a=", 3) == 0)
            alphabet = argv[i] + 3;
        else if (strncmp(argv[i], "--length=", 9) == 0)
            pwLength = atoi(argv[i] + 9);
        else if (strncmp(argv[i], "-l=", 3) == 0)
            pwLength = atoi(argv[i] + 3);
        else if (strncmp(argv[i], "--count=", 8) == 0)
            pwCount = atoi(argv[i] + 8);
        else if (strncmp(argv[i], "-c=", 3) == 0)
            pwCount = atoi(argv[i] + 3);
        else if (strncmp(argv[i], "--seed=", 7) == 0)
            seed = strtoul(argv[i] + 7, NULL, 10);
        else if (strncmp(argv[i], "-s=", 3) == 0)
            seed = strtoul(argv[i] + 3, NULL, 10);
        else {
            fprintf(stdout, "passgen: invalid arguments\n");
            fprintf(stdout, "Try 'passgen --help'.\n");
            return ERR_ARGUMENT;
        }
    }

    seed = mix(clock(), seed, getpid());

    /* initialize mersenne random number generator */

    srand(seed);
    init_genrand(rand());

    /* generate 800,000 random numbers to make up for bad initialization */

    for (k = 0; k < 800000; k++) {
        randomNumber(0, 10);
    }

    /* choose standard alphabet */

    if (strlen(alphabet) < 2) {
        if (strcmp(alphabet, "1") == 0)
            alphabet = alphabet1;
        else if (strcmp(alphabet, "2") == 0)
            alphabet = alphabet2;
        else if (strcmp(alphabet, "3") == 0)
            alphabet = alphabet3;
        else if (strcmp(alphabet, "4") == 0)
            alphabet = alphabet4;
        else if (strcmp(alphabet, "5") == 0)
            alphabet = alphabet5;
        else {
            fprintf(stdout, "passgen: alphabet is too short\n");
            fprintf(stdout, "Try 'passgen --help'.\n");
            return ERR_ALPHABET_LENGTH;
        }
    }

    /* check for uniqueness */

    if (!uniqueCharacters(alphabet)) {
        fprintf(stdout,
                "passgen: at least one non-unique character in alphabet\n");
        fprintf(stdout, "Try 'passgen --help'.\n");
        return ERR_UNIQUE;
    }

    /* print parameters */

    fprintf(stdout, "random seed:               %lu\n", seed);

    fprintf(stdout, "alphabet length:           %lu\n", strlen(alphabet));

    fprintf(stdout, "password length:           %d\n", pwLength);

    /* compute password entropy */

    fprintf(stdout, "possible combinations:     %e\n",
            pow(strlen(alphabet), pwLength));

    entropy = log2(pow(strlen(alphabet), pwLength));

    fprintf(stdout, "password entropy:          %.2f\n", entropy);

    /* compute time needed to guess */

    guessTime = pow(2, entropy - 1) / pow(10, 9);

    if (guessTime < 60)
        fprintf(stdout, "time needed to guess:      %e seconds\n\n", guessTime);
    else if (guessTime < 60 * 60)
        fprintf(stdout, "time needed to guess:      %e minutes\n\n",
                guessTime / 60);
    else if (guessTime < 60 * 60 * 24)
        fprintf(stdout, "time needed to guess:      %e hours\n\n",
                guessTime / (60 * 60));
    else if (guessTime < 60 * 60 * 24 * 365)
        fprintf(stdout, "time needed to guess:      %e days\n\n",
                guessTime / (60 * 60 * 24));
    else if (guessTime < 60 * 60 * 24 * 365 * 10)
        fprintf(stdout, "time needed to guess:      %e years\n\n",
                guessTime / (60 * 60 * 24 * 365));
    else if (guessTime < (unsigned long)60 * 60 * 24 * 365 * 10 * 10)
        fprintf(stdout, "time needed to guess:      %e decades\n\n",
                guessTime / ((unsigned long)60 * 60 * 24 * 365 * 10));
    else if (guessTime < (unsigned long)60 * 60 * 24 * 365 * 10 * 10 * 10)
        fprintf(stdout, "time needed to guess:      %e centuries\n\n",
                guessTime / ((unsigned long)60 * 60 * 24 * 365 * 10 * 10));
    else
        fprintf(stdout, "time needed to guess:      %e millenia\n\n",
                guessTime / ((unsigned long)60 * 60 * 24 * 365 * 10 * 10 * 10));

    /* generate and print password */

    for (i = 0; i < pwCount; i++) {
        for (j = 0; j < pwLength; j++) {
            r = randomNumber(0, strlen(alphabet));
            fprintf(stdout, "%c", alphabet[r]);
        }
        if (pwLength + 5 >= 100)
            fprintf(stdout, "\n");
        else if ((i + 1) % (100 / (pwLength + 5)) == 0)
            fprintf(stdout, "\n");
        else
            fprintf(stdout, "     ");
    }

    printf("\n");

    return 0;
}

/* -------------------------------------------------------------------------- */