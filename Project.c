//
// Created by tiago on 19/11/2022.
//

#include "Project.h"

#define NLINES 20
#define NCOLUMNS 20

int main_project(int argc, const char *argv[]) {

    //short **matrix_ints = alloc_matrix_int(NLINES, NCOLUMNS);
    //int *size = NULL;
    //key_long_2_digits_int(12345, size);
    //unsigned long long key = new_public_key_int();
    //printf("%llu\n",key);
    unsigned long long privetkey = calc_private_key_int(251);
    //calc_runlength_int(111111111112);
    printf("%llu\n", privetkey);
    unsigned long long key = private_key_from_runlength_int(privetkey);
    printf("%llu", key);
    /**---------------------------------------------------------------------------------------------------------------*/
    char **matrix_chars = alloc_matrix_char(NLINES, NCOLUMNS);
    return 0;
}


unsigned long long new_public_key_int(void) {
    srand(time(NULL));
    unsigned long long key = rand() % 3000 + 1;
    return key;
}

short *key_long_2_digits_int(unsigned long long key) {
    unsigned long long mod = 0, mod1 = 0, n = 0;
    int count = init_size(key);

    short *key1 = (short *) malloc((count + 1) * sizeof(short));

    for (int i = 0; i < count; i++) {
        mod = key % 10;
        n = n * 10 + mod;
        key = key / 10;
    }


    for (int i = 0; i < count; i++) {
        mod1 = n % 10;
        *(key1 + i) = mod1;
        n = n / 10;
    }
    *(key1 + count + 1) = -1;
    return key1;
}


unsigned long long key_digits_2_long_int(const short *keydigits) {
    unsigned long long key = 0;

    for (int i = 0; *(keydigits + i + 1) != -1; i++) {
        if (*(keydigits + i) == -2) {
            i++;
        }
        key = key * 10 + *(keydigits + i);
    }
    return key;
}

int verify_private_key(const short *privkey) {
    int count = countDistinct(privkey);
    int a = ascending(privkey);
    int d = descending(privkey);

    if (count == 2 && a == 1 || count == 2 && d == 1) {
        return 1;
    }
    return 0;
}

int descending(const short *privkey) {
    int d = 1;

    for (int i = 0; *(privkey + i + 2) != -1; i++) {
        if (*(privkey + i) < *(privkey + i + 1) && *(privkey + i + 1) != -1) {
            d = 0;
        }
    }
    return d;
}

int ascending(const short *privkey) {
    int c = 1;

    for (int i = 0; *(privkey + i + 2) != -1; i++) {
        if (*(privkey + i) > *(privkey + i + 1) && *(privkey + i + 1) != -1) {
            c = 0;
        }
    }
    return c;
}

int countDistinct(const short *privkey) {
    int res = 1;
    int j = 0;

    for (int i = 1; *(privkey + i + 1) != -1; i++) {

        for (j = 0; j < i; j++)

            if (*(privkey + i) == *(privkey + j))
                break;

        if (i == j)
            res++;
    }

    return res;
}

int init_size(unsigned long long key) {
    int count = 0;
    unsigned long long aux = key;
    while (aux != 0) {
        aux = aux / 10;
        count++;
    }
    return count;
}

unsigned long long calc_private_key_int(unsigned long long pubkey) {
    unsigned long long privkey = 0;
    unsigned long long max = ULLONG_MAX;
    short *aux;
    for (int i = 2; (privkey * i) < max / 2; i++) {
        privkey = pubkey * i;
        aux = key_long_2_digits_int(privkey);

        if (verify_private_key(aux) == 1) {
            break;
        }
    }

    return privkey;
}

unsigned long long calc_runlength_int(unsigned long long privkey) {
    unsigned long long privkey_rle = 0;
    int count = 1;
    int j = 0;
    short *aux = key_long_2_digits_int(privkey);
    short *aux1 = (short *) malloc(7 * sizeof(short));
    for (int i = 0; *(aux + i + 1) != -1; i++) {
        if (*(aux + i) == *(aux + i + 1)) {
            count++;
        } else {
            if (count >= 10) {
                *(aux1 + j) = count / 10;
                *(aux1 + j + 1) = count % 10;
                *(aux1 + j + 2) = -2;
                *(aux1 + j + 3) = *(aux + i);
                j += 4;
                count = 1;

            } else {
                *(aux1 + j) = count;
                *(aux1 + j + 1) = -2;
                *(aux1 + j + 2) = *(aux + i);
                j += 3;
                count = 1;
            }
        }
    }
    *(aux1 + j + 1) = -1;
    privkey_rle = key_digits_2_long_int(aux1);
    printf("%llu", privkey_rle);
    return privkey_rle;
}

unsigned long long private_key_from_runlength_int(unsigned long long runlengthkey) {
    unsigned long long privetkey = 0;
    int count = init_size(runlengthkey);
    int aux1 = 0, aux2 = 0;
    short *array = key_long_2_digits_int(runlengthkey);
    switch (count) {
        case 4:
            aux1 = *(array + 0);
            privetkey = concatenar_key(aux1, *(array + 1), privetkey);
            aux2 = *(array + 2);
            privetkey = concatenar_key(aux2, *(array + 3), privetkey);
            break;
        case 5:
            for (int i = 0; i < 2; i++) {
                aux1 = aux1 * 10 + *(array + i);
                aux2 = aux2 * 10 + *(array + i + 2);
            }
            if (aux1 >= 19) {
                aux1 = *array;
                privetkey = concatenar_key(aux1, *(array + 1), privetkey);
                privetkey = concatenar_key(aux2, *(array + 4), privetkey);
            } else if (aux2 >= 19) {
                aux2 = *(array + 3);
                privetkey = concatenar_key(aux1, *(array + 2), privetkey);
                privetkey = concatenar_key(aux2, *(array + 4), privetkey);
            } else if (aux1 == aux2) {
                printf("Problem 2 solucoes!");
            }
            break;
        case 6:
            for (int i = 0; i < 2; i++) {
                aux1 = aux1 * 10 + *(array + i);
                aux2 = aux2 * 10 + *(array + i + 3);
            }
            if (aux1 + aux2 <= 20) {
                privetkey = concatenar_key(aux1, *(array + 2), privetkey);
                privetkey = concatenar_key(aux2, *(array + 5), privetkey);
            }
            break;
        default:
            break;
    }

    return privetkey;
}

unsigned long long concatenar_key(int aux, short key, unsigned long long privatkey) {
    unsigned long long a = 1;
    if (key == 0) {
        for (int i = 0; i < aux; i++) {
            a *= 10;
        }
        privatkey = privatkey * a;
    } else {
        for (int i = 0; i < aux; i++) {
            privatkey = privatkey * 10 + key;
        }
    }

    return privatkey;
}

short **alloc_matrix_int(int nlines, int ncolumns) {

    short **matrix = (short **) malloc(nlines * sizeof(short *));
    for (int i = 0; i < nlines; i++) {
        *(matrix + i) = (short *) malloc(NCOLUMNS * sizeof(short));
    }
    return matrix;
}

/**-------------------------------------------------------------------------------------------------------------------*/

char **alloc_matrix_char(int nlines, int ncolumns) {
    char **matrix = (char **) malloc(nlines * sizeof(char *));

    for (int i = 0; i < nlines; i++) {
        *(matrix + i) = (char *) malloc(NCOLUMNS * sizeof(char));
    }
    return matrix;
}