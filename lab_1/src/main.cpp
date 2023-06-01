#include <Windows.h>

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

using namespace std;

// Функція, що переводить введений рядок до типу
// double; якщо можливо, в іншому випадку повертає 0.
double get_double()
{
    double value {};
    string str {};

    try {
        getline(cin, str);

        size_t position {};
        value = stod(str, &position);

        if (position != str.size()) {
            throw runtime_error("There are characters after the number!");
        }
        if (value < 0) {
            throw runtime_error("The entered value cannot be negative");
        }
    } catch (const exception& e) {
        cout << format("Exception: {}. The return value is 0!", e.what()) << endl;

        value = 0;
    }

    return value;
}

class Animal {
protected:
    double height;
    double weight;

public:
    Animal()
        : height {}
        , weight {}
    {
    }

    Animal(double height, double weight)
        : height(height)
        , weight(weight)
    {
    }

    const double get_height() const
    {
        return height;
    }

    const double get_weight() const
    {
        return weight;
    }

    Animal& from()
    {
        cout << "Enter the data about an animal." << endl;

        cout << "Height: ";
        height = get_double();

        cout << "Weight: ";
        weight = get_double();

        cout << endl;
        return *this;
    }
};

class Animals {
private:
    vector<Animal> group;
    const char* name;
    double ave_hg;
    double ave_wg;

public:
    Animals(const char* name, vector<Animal> animals)
        : name(name)
        , group(animals)
        , ave_hg {}
        , ave_wg {}
    {
        for (auto& animal : group) {
            ave_hg += animal.get_height();
            ave_wg += animal.get_weight();
        }

        ave_hg /= group.size();
        ave_wg /= group.size();
    }

    void output() const
    {
        cout << format("Average value of {}.", name) << endl;
        cout << format("Height: {}", ave_hg) << endl;
        cout << format("Weight: {}", ave_wg) << endl;
        cout << endl;
    }

    const double get_ave_hg() const
    {
        return ave_hg;
    }

    const double get_ave_wg() const
    {
        return ave_wg;
    }
};

// Ступінь/ відстань між вхідним образом та еталоном.
double id(Animals& group, Animal& obj)
{
    double x { group.get_ave_wg() - obj.get_weight() };
    double y { group.get_ave_hg() - obj.get_height() };

    return sqrt(pow(x, 2) + pow(y, 2));
}

int main()
{
    // Лямбда функція, що починається на початку програми
    // та відловлює натискання клавіші 'esc'.
    thread task([]() {
        while (true) {
            this_thread::sleep_for(chrono::milliseconds(100));

            if (GetAsyncKeyState(VK_ESCAPE)) {
                exit(0);
            }
        }
    });

    Animals cats("cats",
        vector<Animal> {
            Animal(20, 4),
            Animal(24, 4.2),
            Animal(22, 4.6),
            Animal(31, 5),
            Animal(27, 5.1),
            Animal(29, 4.4),
        });

    Animals dogs("dogs",
        vector<Animal> {
            Animal(40, 6),
            Animal(60, 12),
            Animal(100, 20),
            Animal(55, 9),
            Animal(69, 15),
            Animal(36, 7),
        });

    cats.output();
    dogs.output();

    while (true) {
        cout << endl;
        cout << "Enter the data of the test object." << endl;
        Animal obj { Animal().from() };

        double is_cat { id(cats, obj) };
        double is_dog { id(dogs, obj) };

        cout << format("Cat's id number is (similarity with the ethalon): {}", is_cat) << endl;
        cout << format("Dog's id number is (similarity with the ethalon): {}", is_dog) << endl;

        Animal temp {};
        if (is_cat > id(cats, temp) && is_dog > id(dogs, temp)) {
            cout << "This objects is different of these both classes. " << endl;
            continue;
        }

        if (is_cat < is_dog) {
            cout << "The test object is cat" << endl;
        } else {
            cout << "The test object is dog" << endl;
        }
    }

    return 0;
}