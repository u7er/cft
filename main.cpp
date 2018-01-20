#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <vector>
#include <cstring>
#include <getopt.h>

using namespace std;

enum SortType {
    up,
    down
};

template<typename Type, SortType sort_type>
void insertionSort(Type *source_array, const int &size_array) {
    function<bool(Type, Type)> sort_method;
    if (sort_type == up)
        sort_method = [](Type a, Type b) { return a > b; };
    else if (sort_type == down)
        sort_method = [](Type a, Type b) { return a < b; };

    Type temp_element;
    for (int i = 1; i < size_array; i++) {
        temp_element = source_array[i];
        int j = i - 1;
        while (j >= 0 && sort_method(source_array[j], temp_element)) {
            source_array[j + 1] = source_array[j];
            j--;
        }
        source_array[j + 1] = temp_element;
    }
}

template<typename Type>
void printArray(vector<Type> array, const string &file_out) {
    ofstream output_data(file_out);
    for (auto &it: array)
        output_data << it << '\n';
    output_data.close();
}

// return code 2 - data is type string  (-s)
// return code 1 - data is type int     (-i)
int checkData(const string &file_name) {
    ifstream input(file_name);

    if (!input.is_open()) {
        cout << "Error open input file" << endl;
        exit(0);
    }

    while (!input.eof()) {
        string temp_string;
        getline(input, temp_string);
        for (char i : temp_string)
            if (i < '0' || i > '9') {
                input.close();
                return 2;
            }
    }

    input.close();
    return 1;
}

void checkDataKey(const int type_data_file, const char *key_data_type) {
    switch (type_data_file) {
        case 1: {
            if (key_data_type[1] == 's') {
                cout << "File data does not match the type of the key." << endl;
                exit(0);
            }
            break;
        }
        case 2: {
            if (key_data_type[1] == 'i') {
                cout << "File data does not match the type of the key." << endl;
                exit(0);
            }
            break;
        }
        default:
            break;
    }
}

int main(int agrc, char **agrv) {
    if (agrc != 5) {
        cout << "Error entering arguments.\n";
        cout
                << "Correct format keys: input_file, output_file, key_data_type(\"-i\" or \"-s\"), key_sort_method(\"-a\" or \"-d\")"
                << endl;
        exit(EXIT_FAILURE);
    }

    if (strlen(agrv[1]) > 254) {
        cout << "Name input file is very big." << endl;
        exit(0);
    }
    if (strlen(agrv[2]) > 254) {
        cout << "Name output file is very big." << endl;
        exit(0);
    }


    if (strlen(agrv[3]) != 2 || strlen(agrv[4]) != 2) {
        cout << "Invalid key lenght." << endl;
        exit(0);
    }

    if (agrv[3][0] != '-' || agrv[4][0] != '-' ||
        (agrv[3][1] != 's' && agrv[3][1] != 'i') ||
        (agrv[4][1] != 'a' && agrv[4][1] != 'd')) {
        cout << "Invalid key format." << endl;
        exit(0);
    }

    // check type data
    int whatData = checkData(agrv[1]);
    // check data type & type key data
    checkDataKey(whatData, agrv[3]);

    ifstream input_data(agrv[1]);

    vector<int> int_array;
    vector<string> str_array;

    while (!input_data.eof()) {
        string s;
        getline(input_data, s);
        str_array.push_back(s);
    }

    if (agrv[3][1] == 's') {
        if (agrv[4][1] == 'a')
            insertionSort<string, up>(str_array.data(), static_cast<const int &>(str_array.size()));
        if (agrv[4][1] == 'd')
            insertionSort<string, down>(str_array.data(), static_cast<const int &>(str_array.size()));

        printArray(str_array, agrv[2]);
    } else if (agrv[3][1] == 'i') {
        for (auto &it: str_array)
            int_array.push_back(atoi(it.data()));

        if (agrv[4][1] == 'a')
            insertionSort<int, up>(int_array.data(), static_cast<const int &>(int_array.size()));
        if (agrv[4][1] == 'd')
            insertionSort<int, down>(int_array.data(), static_cast<const int &>(int_array.size()));

        printArray(int_array, agrv[2]);
    }

    return EXIT_SUCCESS;
}