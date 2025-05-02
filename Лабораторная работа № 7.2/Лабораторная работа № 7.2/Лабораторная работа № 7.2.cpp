#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

// Класс персонажа
class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Character " << name << " created! HP=" << health << "\n";
    }

    // Методы для безопасного доступа к здоровью
    int getHealth() const { return health; }
    void takeDamage(int dmg) { health -= dmg; }

    int getAttack() const { return attack; }
    int getDefense() const { return defense; }

    void displayInfo() const {
        std::cout << name
            << " [HP=" << health
            << ", ATK=" << attack
            << ", DEF=" << defense
            << "]\n";
    }

    const std::string& getName() const { return name; }
};

// Класс монстра
class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Monster " << name << " created! HP=" << health << "\n";
    }

    int getHealth() const { return health; }
    void takeDamage(int dmg) { health -= dmg; }

    int getAttack() const { return attack; }
    int getDefense() const { return defense; }

    void displayInfo() const {
        std::cout << name
            << " [HP=" << health
            << ", ATK=" << attack
            << ", DEF=" << defense
            << "]\n";
    }

    const std::string& getName() const { return name; }
};

// Мьютекс для синхронизации боя
std::mutex battleMutex;

// Функция, выполняемая в отдельном потоке: бой героя и монстра
void battle(Character& hero, Monster& monster) {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(battleMutex);

            // Проверяем, кто ещё жив
            if (hero.getHealth() <= 0 || monster.getHealth() <= 0)
                break;

            // Герой атакует монстра
            int damageToMonster = std::max(0, hero.getAttack() - monster.getDefense());
            monster.takeDamage(damageToMonster);
            std::cout << hero.getName() << " hits " << monster.getName()
                << " for " << damageToMonster << " dmg. "
                << monster.getName() << " HP=" << monster.getHealth() << "\n";

            // Если монстр погиб — выход
            if (monster.getHealth() <= 0) {
                std::cout << monster.getName() << " is defeated!\n";
                break;
            }

            // Монстр отвечает атакой
            int damageToHero = std::max(0, monster.getAttack() - hero.getDefense());
            hero.takeDamage(damageToHero);
            std::cout << monster.getName() << " hits " << hero.getName()
                << " for " << damageToHero << " dmg. "
                << hero.getName() << " HP=" << hero.getHealth() << "\n";

            if (hero.getHealth() <= 0) {
                std::cout << hero.getName() << " is defeated!\n";
                break;
            }
        }

        // Пауза между раундами боя
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    // Создаём героя и монстра
    Character hero("Hero", 100, 20, 10);
    Monster   monster("Orc", 80, 15, 5);

    // Запускаем поток боя
    std::thread battleThread(battle, std::ref(hero), std::ref(monster));

    battleThread.join();

    // Выводим итоговые параметры
    std::cout << "\n=== Battle Result ===\n";
    hero.displayInfo();
    monster.displayInfo();

    return 0;
}
