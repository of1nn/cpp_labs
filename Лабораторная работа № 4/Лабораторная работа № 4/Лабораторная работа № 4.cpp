#include <iostream>
#include <memory>
#include <string>


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
    Weapon() : name(""), damage(0), weight(0) {}

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

    // Перегрузка оператора <<
    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon) {
        os << "Weapon: " << weapon.name << ", Damage: " << weapon.damage;
        return os;
    }
};


class Inventory {
private:
    std::unique_ptr<Weapon[]> items;
    int capacity;
    int size;

public:
    Inventory(int cap = 10) : capacity(cap), size(0) {
        items = std::make_unique<Weapon[]>(capacity);
    }

    void addItem(const Weapon& item) {
        if (size < capacity) {
            items[size++] = item;
        }
        else {
            std::cout << "Inventory is full!\n";
        }
    }

    void displayInventory() const {
        std::cout << "Inventory contents:\n";
        for (int i = 0; i < size; ++i) {
            std::cout << "- " << items[i] << "\n";
        }
    }
};

int main() {
    Inventory inventory;

    Weapon one("One", 100, 50);
    Weapon two("Two", 20, 10);

    inventory.addItem(one);
    inventory.addItem(two);
    inventory.displayInventory();

    return 0;
}
