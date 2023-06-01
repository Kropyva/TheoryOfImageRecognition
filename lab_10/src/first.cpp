#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

const string get_string()
{
    string str {};

    try {
        getline(cin, str);
    } catch (const exception&) {
        cout << "Failed to read a line!" << endl;
    }

    if (str == "") {
        str = " ";
    }

    return str;
}

int get_number()
{

    int value {};
    size_t position {};
    string str(get_string());

    try {
        value = stoi(str, &position);

        if (position != str.size()) {
            throw runtime_error("There are characters after the number");
        }

        if (value < 1 || value > 4) {
            throw runtime_error("Possible values are 1..4");
        }
    } catch (const exception& e) {
        cout << e.what() << endl;
        return 1;
    }

    return value;
}

bool file_exists(const string& file_path)
{
    ifstream file(file_path);
    return file.good();
}

void create_file(const string& file_path)
{
    ofstream file(file_path);

    if (file.is_open()) {
        file.close();
    } else {
        throw string("Failed to create the file: " + file_path);
    }
}

void create_files(char start, char end)
{
    for (char letter { start }; letter <= end; letter++) {
        string file_path("./files/" + string(1, letter) + ".txt");

        if (!file_exists(file_path)) {
            try {
                create_file(file_path);
            } catch (const string& e) {
                throw e;
            }
        }
    }
}

void initialize_files()
{
    try {
        create_files('A', 'Z');
        create_files('0', '9');
        cout << "The files have been initialized" << endl;
    } catch (const string& e) {
        throw e;
    }
}

void input_word()
{
    string word(get_string());
    string file_path("./files/" + string(1, word[0]) + ".txt");

    if (file_exists(file_path)) {
        ofstream file(file_path, ios::app);

        if (file.is_open()) {
            file << word << endl;
            file.close();

            throw string("The word '" + word + "' has been append to the file: " + file_path);
        } else {
            throw string("Failed to write the file: " + file_path);
        }
    } else {
        throw string("The file '" + file_path + "' doesn't exist!");
    }
}

void check_word()
{
    string word(get_string());
    string file_path("./files/" + string(1, word[0]) + ".txt");
    ifstream file(file_path);

    if (file.is_open()) {
        string line {};

        while (getline(file, line)) {
            if (line == word) {
                throw string("The word '" + word + "' exists in the file " + file_path);
            }
        }
        throw string("The word '" + word + "' doesn't exist in the file " + file_path);
    } else {
        throw string("Failed to open the file: " + file_path);
    }
}

void process_function(void (*fn)())
{
    try {
        fn();
    } catch (const string& e) {
        cout << e << endl;
    }
}

int main()
{
    while (true) {
        cout << "Press a button to do:" << endl;
        cout << "    1. Exit;" << endl;
        cout << "    2. Initialize files;" << endl;
        cout << "    3. Input a word;" << endl;
        cout << "    4. Check if a word exists in the files;" << endl;

        switch (get_number()) {
        case 1:
            exit(0);
            break;
        case 2:
            process_function(initialize_files);
            break;
        case 3:
            cout << "Enter your word: ";
            process_function(input_word);
            break;
        case 4:
            cout << "Enter your word: ";
            process_function(check_word);
            break;
        }

        cout << endl;
    }

    return 0;
}