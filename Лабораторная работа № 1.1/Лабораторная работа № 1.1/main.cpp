#include <iostream>
#include <string>

class Character {
private:
    std::string name;  // ��������� ����: ��� ���������
    int health;        // ��������� ����: ������� ��������
    int attack;        // ��������� ����: ������� �����
    int defense;       // ��������� ����: ������� ������

public:
    // ����������� ��� ������������� ������
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    // ����� ��� ��������� ������ ��������
    int getHealth() const {
        return health;
    }

    void heal(int amount) {
        if (amount > 0) {
            health += amount;
            if (health > 100) {
                health = 100;
            }
            std::cout << name << " ��������� �� " << amount << " HP!" << std::endl;
        }
    }

    void takeDamage(int amount) {
        if (amount > 0) {
            health -= amount;
            if (health < 0) {
                health = 0;
            }
            std::cout << name << " ������� " << amount << " �����!" << std::endl;
        }
    }

    // ����� ��� ������ ���������� � ���������
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    // ����� ��� ����� ������� ���������
    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.health -= damage;
            std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
        }
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    // ������� ������� ����������
    Character hero("Hero", 100, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    // ������� ���������� � ����������
    hero.displayInfo();
    monster.displayInfo();

    // ����� ������� �������
    hero.attackEnemy(monster);
    monster.displayInfo();

    // ������ ������� �����
    hero.takeDamage(50);
    hero.displayInfo();

    // ����� �������������� ��������
    hero.heal(1000);
    hero.displayInfo();

    return 0;
}
