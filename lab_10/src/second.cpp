#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

bool compare(const string& str, const vector<string>& words)
{
    for (auto& word : words) {
        if (str == word) {
            return true;
        }
    }
    return false;
}

bool is_type(const string& str)
{
    const vector<string> types { "u32", "u64", "i32", "i64", "f32", "f64" };
    return compare(str, types);
}

bool is_keyword(const string& str)
{
    const vector<string> keywords { "var", "const", "fn" };
    return compare(str, keywords);
}

bool is_operator(const string& str)
{
    const vector<string> operators { "+", "-", "*", "/", "%" };
    return compare(str, operators);
}

bool is_separator(const string& str)
{
    const vector<string> separators { ",", ";", ":", "(", ")", "{", "}" };
    return compare(str, separators);
}

bool is_assignment(const string& str)
{
    const vector<string> asssignment { "=" };
    return compare(str, asssignment);
}

bool is_number(const string& str)
{
    try {
        double number { stod(str) };
        return true;
    } catch (const exception&) {
        return false;
    }
}

int is_sign_at_the_end(const string& str)
{
    const string last { str.back() };

    if (is_operator(last)) {
        return 1;
    }
    if (is_separator(last)) {
        return 2;
    }
    if (is_assignment(last)) {
        return 3;
    }

    return 0;
}

int main()
{
    ifstream file("./files/file.txt");

    if (!file.is_open()) {
        cout << "Failed to open file!" << endl;
        return 1;
    }

    string line {};

    while (getline(file, line)) {
        istringstream line_stream(line);
        string word {};

        while (getline(line_stream, word, ' ')) {
            int sign_at_the_end { is_sign_at_the_end(word) };
            string sign {};

            if (sign_at_the_end > 0) {
                if (word.find('(') && word.back() == ')') {
                    cout << word << " - is a function." << endl;
                    continue;
                }

                sign = string(1, word[word.size() - 1]);
                word = word.substr(0, word.size() - 1);
            }

            if (word.size() >= 1) {
                if (is_keyword(word)) {
                    cout << word << " - is a keyword." << endl;
                } else if (is_type(word)) {
                    cout << word << " - is a type." << endl;
                } else if (is_number(word)) {
                    cout << word << " - is a number." << endl;
                } else {
                    cout << word << " - is a variable." << endl;
                }
            }

            switch (sign_at_the_end)
            {
            case 1:
                cout << sign << " - is an operator." << endl;
                break;
            case 2:
                cout << sign << " - is a separator." << endl;
                break;
            case 3:
                cout << sign << " - is an assignment." << endl;
                break;
            default:
                break;
            }
        }
    }

    file.close();
    return 0;
}