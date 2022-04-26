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
    g, e, d, b, f
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
                mode = g;
                break;
            case 'e':
                mode = e;
                break;
            case 'd':
                mode = d;
                break;
            case 'b':
                mode = b;
                break;
            case '?':
                std::cerr << "Unknown parameter: " << char(optopt) << std::endl;
                break;
        }
    }

    for (; optind < argc; optind++) {
        numbers_as_strings.push_back(argv[optind]);
    }

    switch(mode) {
        case g:
            if(numbers_as_strings.size() != 1) {
                mode = f;
            }
            else {
                numbers->push_back(load_integer(numbers_as_strings[0], 10));
            }
            break;
        case e:
            if(numbers_as_strings.size() != 3) {
                mode = f;
            }
            else {
                numbers->push_back(load_integer(numbers_as_strings[0], 16));
                numbers->push_back(load_integer(numbers_as_strings[1], 16));
                numbers->push_back(load_integer(numbers_as_strings[2], 16));
            }
            break;
        case d:
            if(numbers_as_strings.size() != 3) {
                mode = f;
            }
            else {
                numbers->push_back(load_integer(numbers_as_strings[0], 16));
                numbers->push_back(load_integer(numbers_as_strings[1], 16));
                numbers->push_back(load_integer(numbers_as_strings[2], 16));
            }
            break;
        case b:
            if(numbers_as_strings.size() != 1) {
                mode = f;
            }
            else {
                numbers->push_back(load_integer(numbers_as_strings[0], 16));
            }
            break;
        case f:
            break;
    }

    return mode;
}

int main(int argc, char *argv[]) {
    vector<long long int> numbers;
    Mode mode;
    mode = parse_params(argc, argv, &numbers);

    switch (mode) {
        case g:
            cout << "Generate" << endl;
            break;
        case e:
            cout << "Encode" << endl;
            break;
        case d:
            cout << "Decode" << endl;
            break;
        case b:
            cout << "Break" << endl;
            break;
        case f:
            cout << "Fail" << endl;
            return 1;
    }

    for(auto &number : numbers) {
        cout << number << endl;
    }

    return 0;
}
