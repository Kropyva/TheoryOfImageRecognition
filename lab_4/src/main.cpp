#define _USE_MATH_DEFINES

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
private:
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
    vector<Animal> group;
    double ave_hg;
    double ave_wg;

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

    const double get_ave_hg() const
    {
        return ave_hg;
    }

    const double get_ave_wg() const
    {
        return ave_wg;
    }

    const vector<Animal>& get_group() const
    {
        return group;
    }

    void output()
    {
        cout << format("Average value of {}", name) << endl;
        cout << format("Height: {}", ave_hg) << endl;
        cout << format("Weight: {}", ave_wg) << endl;
        cout << endl;
    }
};

class Aim {
private:
    vector<Animals*> animals;
    Animal& obj;
    size_t size;
    long double f;

public:
    Aim(Animals& first, Animals& second, Animal& obj)
        : animals({ &first, &second })
        , obj(obj)
        , size(first.get_group().size() + second.get_group().size())
        , f {}
    {
        for (auto& group : animals) {
            f += function(*group);
        }
    }

    const long double probability(Animals& animals) const
    {
        return static_cast<long double>(animals.get_group().size()) / size;
    }

    const long double obj_minus_average(Animals& animals, const Animal& obj) const
    {
        double x { pow(obj.get_weight() - animals.get_ave_wg(), 2) };
        double y { pow(obj.get_height() - animals.get_ave_hg(), 2) };
        return y + x;
    }

    const long double dispersion(Animals& animals) const
    {
        double value {};
        const vector<Animal>& ref_group { animals.get_group() };

        for (auto& animal : ref_group) {
            value += obj_minus_average(animals, animal);
        }

        value = value / ref_group.size();
        return value <= 0 ? 1.0 : value;
    }

    const long double fi(Animals& animals) const
    {
        long double disper { dispersion(animals) };
        long double x { pow(2 * M_PI * disper, size / 2) };
        long double y { exp(-obj_minus_average(animals, obj) / (2 * disper)) };

        return 1.0 / x * y;
    }

    const long double function(Animals& animals) const
    {
        return fi(animals) * probability(animals);
    }

    const long double bayes(Animals& animals) const
    {
        return (probability(animals) * fi(animals)) / f;
    }

    const bool compare_bayes() const
    {
        const long double value_a { bayes(*animals[0]) };
        const long double value_b { bayes(*animals[1]) };

        cout << format("1: Bayes = {}", value_a) << endl;
        cout << format("2: Bayes = {}", value_b) << endl;

        return value_a > value_b ? true : false;
    }

    const long double get_f() const
    {
        return f;
    }
};

int main()
{
    thread go_away([]() {
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
        cout << "The data of the test object." << endl;

        auto obj { Animal().from() };
        auto aim { Aim(cats, dogs, obj) };

        cout << format("1: Dispersion = {}", aim.dispersion(cats)) << endl;
        cout << format("1: p = {}", aim.probability(cats)) << endl;
        cout << format("1: (X - C)^2 = {}", aim.obj_minus_average(cats, obj)) << endl;
        cout << format("1: fi = {}", aim.fi(cats)) << endl;
        cout << endl;
        cout << format("2: Dispersion = {}", aim.dispersion(dogs)) << endl;
        cout << format("2: p = {}", aim.probability(dogs)) << endl;
        cout << format("2: (X - C)^2 = {}", aim.obj_minus_average(dogs, obj)) << endl;
        cout << format("2: fi = {}", aim.fi(dogs)) << endl;
        cout << endl;
        cout << format("F = {}", aim.get_f()) << endl;

        if (aim.compare_bayes()) {
            cout << "The test object is cat" << endl;
        } else {
            cout << "The test object is dog" << endl;
        }
    }

    return 0;
}