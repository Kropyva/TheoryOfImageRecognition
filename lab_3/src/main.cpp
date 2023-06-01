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

    Animal from()
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
    const char* name;
    double ave_hg;
    double ave_wg;
    vector<Animal> group;

public:
    Animals(const char* name, vector<Animal> animals)
        : name(name)
        , group(animals)
        , ave_hg {}
        , ave_wg {}
    {
        for (auto& animal : animals) {
            ave_hg += animal.get_height();
            ave_wg += animal.get_weight();
        }

        ave_hg /= animals.size();
        ave_wg /= animals.size();
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

class Aim {
private:
    double x;
    double y;
    double k;

    void constant(Animals& cats, Animals& dogs)
    {
        k = (cats.get_ave_hg() - dogs.get_ave_hg()) / (cats.get_ave_wg() - dogs.get_ave_wg());
    }

public:
    Aim(Animals& cats, Animals& dogs)
    {
        constant(cats, dogs);

        x = (cats.get_ave_wg() + dogs.get_ave_wg()) / 2;
        y = (cats.get_ave_hg() + dogs.get_ave_hg()) / 2;
    }

    const double distribution(Animal& obj)
    {
        return obj.get_height() + (obj.get_weight() - x) / k - y;
    }

    void output()
    {
        cout << format("Constant: {}", k) << endl;
        cout << format("M(x0, y0): M({}, {})", x, y) << endl;
        cout << endl;
    }
};

int main()
{
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

    Aim aim(cats, dogs);
    aim.output();

    while (true) {
        cout << endl;
        cout << "The data of the test object." << endl;
        Animal obj { Animal().from() };

        double value { aim.distribution(obj) };
        cout << format("Distribution function: {}", value) << endl;

        if (value < 0) {
            cout << "The test object is cat" << endl;
        } else {
            cout << "The test object is dog" << endl;
        }
    }

    return 0;
}