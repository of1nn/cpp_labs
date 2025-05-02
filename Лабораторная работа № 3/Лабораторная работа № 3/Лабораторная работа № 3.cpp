#include <iostream>


class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    // Перегрузка оператора ==
    bool operator==(const Character& other) const {
        return name == other.name && health == other.health;
    }

    // Перегрузка оператора <<
    friend std::ostream& operator<<(std::ostream& os, const Character& character) {
        os << "Character: " << character.name << ", HP: " << character.health
            << ", Attack: " << character.attack << ", Defense: " << character.defense;
        return os;
    }
};

class Weapon {
private:
    std::string name;
    int damage;
    int weight;
public:
    Weapon(const std::string& n, int d, int w)
        : name(n), damage(d), weight(w) {
    }

    Weapon operator+(const Weapon& other) const {
        return Weapon("new", damage + other.damage, weight + other.weight);
    }

    bool operator>(const Weapon& other) const {
        return damage > other.damage;
    }

    ~Weapon() {
        std::cout << "Weapon " << name << " destroyed!\n";
    }
    void displayInfo() const {
        std::cout << "Name: " << name << ", Damage: " << damage
            << ", Weight: " << weight << std::endl;
    }
};

int main() {
    Character hero1("Hero", 100, 20, 10);
    Character hero2("Hero", 100, 20, 10);
    Character hero3("Warrior", 150, 25, 15);

    if (hero1 == hero2) {
        std::cout << "Hero1 and Hero2 are the same!\n";
    }
    if (!(hero1 == hero3)) {
        std::cout << "Hero1 and Hero3 are different!\n";
    }

    Weapon one("One", 100, 50);
    Weapon two("Two", 20, 10);
    Weapon three = one + two;

    three.displayInfo();
    bool b = one > three;

    std::cout << b << std::endl;

    std::cout << hero1 << std::endl; // Вывод информации о персонаже
    return 0;
}
