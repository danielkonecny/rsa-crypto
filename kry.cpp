/**
 * KRY Project 2 - RSA
 * @author          Daniel Konecny (xkonec75)
 * @organisation    Brno University of Technology - Faculty of Information Technologies
 */

#include <iostream>
#include <vector>
#include <getopt.h>
#include <gmp.h>

#include "kry.h"

using namespace std;

enum Mode {
    G, E, D, B, F
};

long long int load_integer(char *numberAsChars, int base) {
    string::size_type length;
    string numberAsString = numberAsChars;
    long long int numberAsInt;

    try {
        numberAsInt = stoll(numberAsString, &length, base);
    }
    catch (const invalid_argument &ia) {
        cerr << "Invalid number conversion" << endl;
        throw;
    }
    catch (const out_of_range &oor) {
        cerr << "Number out of range" << endl;
        throw;
    }
    if (!numberAsString.substr(length).empty()) {
        cerr << "Invalid number format" << endl;
        throw;
    }

    return numberAsInt;
}

Mode parse_params(int argc, char *argv[], vector<long long int> *numbers) {
    int option;
    vector<char *> numbers_as_strings;

    Mode mode;

    while ((option = getopt(argc, argv, "gedb")) != -1) {
        switch (option) {
            case 'g':
                mode = G;
                break;
            case 'e':
                mode = E;
                break;
            case 'd':
                mode = D;
                break;
            case 'b':
                mode = B;
                break;
            case '?':
                std::cerr << "Unknown parameter: " << char(optopt) << std::endl;
                mode = F;
                break;
        }
    }

    for (; optind < argc; optind++) {
        numbers_as_strings.push_back(argv[optind]);
    }

    switch (mode) {
        case G:
            if (numbers_as_strings.size() != 1) {
                mode = F;
            } else {
                numbers->push_back(load_integer(numbers_as_strings[0], 10));
            }
            break;
        case E:
            if (numbers_as_strings.size() != 3) {
                mode = F;
            } else {
                numbers->push_back(load_integer(numbers_as_strings[0], 16));
                numbers->push_back(load_integer(numbers_as_strings[1], 16));
                numbers->push_back(load_integer(numbers_as_strings[2], 16));
            }
            break;
        case D:
            if (numbers_as_strings.size() != 3) {
                mode = F;
            } else {
                numbers->push_back(load_integer(numbers_as_strings[0], 16));
                numbers->push_back(load_integer(numbers_as_strings[1], 16));
                numbers->push_back(load_integer(numbers_as_strings[2], 16));
            }
            break;
        case B:
            if (numbers_as_strings.size() != 1) {
                mode = F;
            } else {
                numbers->push_back(load_integer(numbers_as_strings[0], 16));
            }
            break;
        case F:
            break;
    }

    return mode;
}

void mod_inverse(mpz_t d, mpz_t orig_a, mpz_t orig_m) {
    mpz_t a, m, m0, y, x, q, t;
    mpz_inits(a, m, m0, y, x, q, t, NULL);
    mpz_set(m0, orig_m);
    mpz_set(a, orig_a);
    mpz_set(m, orig_m);
    mpz_set_si(y, 0);
    mpz_set_si(x, 1);

    if (mpz_cmp_si(m, 1) == 0) {
        mpz_set_si(d, 0);
    }

    while (mpz_cmp_si(a, 1) > 0) {
        mpz_tdiv_q(q, a, m);
        mpz_set(t, m);
        mpz_mod(m, a, m);
        mpz_set(a, t);
        mpz_set(t, y);
        mpz_submul(x, q, y);
        mpz_set(y, x);
        mpz_set(x, t);
    }

    if (mpz_cmp_si(x, 0) < 0) {
        mpz_add(x, x, m0);
    }

    mpz_set(d, x);

    mpz_clears(a, m, m0, y, x, q, t, NULL);
}

void gcd(mpz_t divisor, mpz_t orig_a, mpz_t orig_b) {
    mpz_t a, b;
    mpz_inits(a, b, NULL);
    mpz_set(a, orig_a);
    mpz_set(b, orig_b);

    while (mpz_cmp(a, b) != 0) {
        if (mpz_cmp(a, b) > 0) {
            mpz_sub(a, a, b);
        } else {
            mpz_sub(b, b, a);
        }
    }
    mpz_set(divisor, a);

    mpz_clears(a, b, NULL);
}

bool is_coprime(mpz_t phi_n, mpz_t e) {
    mpz_t divisor;
    mpz_init(divisor);

    gcd(divisor, phi_n, e);

    bool is_it_coprime = mpz_cmp_ui(divisor, 1) == 0;

    mpz_clear(divisor);

    return is_it_coprime;
}

void get_random_coprime(mpz_t phi_n, mpz_t e, gmp_randstate_t state) {
    mpz_t phi_n1;
    mpz_init(phi_n1);
    mpz_sub_ui(phi_n1, phi_n, 1);

    do {
        mpz_urandomm(e, state, phi_n1);
        mpz_add_ui(e, e, 1);
    } while (!is_coprime(phi_n, e));

    mpz_clear(phi_n1);
}

int jacobi(mpz_t orig_a, mpz_t orig_n) {
    mpz_t a, n, remainder;
    mpz_inits(a, n, remainder, NULL);
    mpz_set(a, orig_a);
    mpz_set(n, orig_n);

    if (mpz_cmp_si(a, 0) == 0) {         // (0/n) = 0
        return 0;
    }
    int j = 1;
    if (mpz_cmp_si(a, 0) < 0) {
        mpz_mul_si(a, a, -1);               // (a/n) = (-a/n)*(-1/n)
        mpz_mod_ui(remainder, n, 4);
        if (mpz_cmp_si(remainder, 3) == 0) {
            j *= -1;                         // (-1/n) = -1 if n = 3 (mod 4)
        }
    }
    if (mpz_cmp_si(a, 1) == 0) {         // (1/n) = 1
        return j;
    }
    while (mpz_cmp_si(a, 0) != 0) {
        if (mpz_cmp_si(a, 0) < 0) {
            mpz_mul_si(a, a, -1);           // (a/n) = (-a/n)*(-1/n)
            mpz_mod_ui(remainder, n, 4);
            if (mpz_cmp_si(remainder, 3) == 0) {
                j *= -1;                     // (-1/n) = -1 if n = 3 (mod 4)
            }
        }
        while (mpz_even_p(a)) {
            mpz_tdiv_q_ui(a, a, 2);
            mpz_mod_ui(remainder, n, 8);
            if ((mpz_cmp_si(remainder, 3) == 0) || (mpz_cmp_si(remainder, 5) == 0)) {
                j *= -1;
            }
        }
        mpz_swap(a, n);
        if ((mpz_mod_ui(remainder, a, 4) == 3) && (mpz_mod_ui(remainder, n, 4) == 3)) {
            j *= -1;
        }
        mpz_mod(a, a, n);
        if (mpz_cmp_si(a, mpz_tdiv_ui(n, 2)) > 0) {
            mpz_sub(a, a, n);
        }
    }
    if (mpz_cmp_si(n, 1) == 0) {
        return j;
    }

    mpz_clears(a, n, remainder, NULL);
    return 0;
}

/**
 * Inspired with: https://www.tutorialspoint.com/cplusplus-program-to-implement-the-solovay-strassen-primality-test
 * -to-check-if-a-given-number-is-prime.
 * @param n
 * @param k
 * @return Is "n" a prime boolean.
 */
bool is_prime(mpz_t n, int k) {
    bool is_it_prime;

    int test_two = mpz_cmp_si(n, 2);
    if (test_two < 0) {
        is_it_prime = false;
    } else if (test_two == 0) {
        is_it_prime = true;
    } else if (mpz_even_p(n)) {  // Is also: test_two > 0
        is_it_prime = false;
    } else {  // Is also: test_two > 0
        is_it_prime = true;
        mpz_t a, n2, x, exp, mod;
        mpz_inits(a, n2, x, exp, mod, NULL);

        gmp_randstate_t state;
        gmp_randinit_default(state);

        mpz_sub_ui(n2, n, 2);
        mpz_sub_ui(exp, n, 1);
        mpz_tdiv_q_ui(exp, exp, 2);

        for (int i = 0; i < k; i++) {
            mpz_urandomm(a, state, n2);  // from 0 to number-3
            mpz_add_ui(a, a, 2);        // from 2 to number-1

            mpz_set_si(x, jacobi(a, n));
            mpz_add(x, x, n);
            mpz_mod(x, x, n);

            mpz_powm(mod, a, exp, n);

            if ((mpz_cmp_si(x, 0) == 0) || (mpz_cmp(x, mod) != 0)) {
                is_it_prime = false;
                break;
            }
        }
        gmp_randclear(state);
        mpz_clears(a, n2, x, exp, mod, NULL);
    }

    return is_it_prime;
}

void get_random_prime(int bits, mpz_t random, gmp_randstate_t state) {
    int k = 50;

    do {
        mpz_urandomb(random, state, bits / 2);
        mpz_setbit(random, bits / 2 - 1);
    } while (!is_prime(random, k));
}

void generate(int bits) {
    mpz_t p, q, n, phi_n, p1, q1, e, d;
    mpz_inits(p, q, n, phi_n, p1, q1, e, d, NULL);

    gmp_randstate_t state;
    gmp_randinit_default(state);

    get_random_prime(bits, p, state);
    get_random_prime(bits, q, state);

    mpz_mul(n, p, q);
    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(phi_n, p1, q1);

    get_random_coprime(phi_n, e, state);

    mod_inverse(d, e, phi_n);

    gmp_printf("%#Zx %#Zx %#Zx %#Zx %#Zx\n", p, q, n, e, d);
    gmp_printf("p=%Zd\nq=%Zd\nn=%Zd\ne=%Zd\nd=%Zd\n", p, q, n, e, d);

    gmp_randclear(state);
    mpz_clears(p, q, n, phi_n, p1, q1, e, d, NULL);
}

void encrypt(int e, int n, int m) {
    mpz_t cipher, key, modulus, message;
    mpz_inits(cipher, key, modulus, message, NULL);
    mpz_set_ui(key, e);
    mpz_set_ui(modulus, n);
    mpz_set_ui(message, m);

    mpz_powm(cipher, message, key, modulus);

    gmp_printf("%#Zx\n", cipher);

    mpz_clears(cipher, key, modulus, message, NULL);
}

int main(int argc, char *argv[]) {
    vector<long long int> numbers;
    Mode mode;
    mode = parse_params(argc, argv, &numbers);

    // TODO - Load directly to mpz_t.

    switch (mode) {
        case G:
            cout << "Generate" << endl;
            generate(numbers[0]);
            break;
        case E:
            cout << "Encode" << endl;
            encrypt(numbers[0], numbers[1], numbers[2]);
            break;
        case D:
            cout << "Decode" << endl;
            encrypt(numbers[0], numbers[1], numbers[2]);
            break;
        case B:
            cout << "Break" << endl;
            break;
        case F:
            cout << "Fail" << endl;
            return 1;
    }

    return 0;
}
