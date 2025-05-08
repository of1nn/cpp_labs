#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <memory>
#include <algorithm> // для std::find

// Исключение при смерти персонажа
class DeathException : public std::runtime_error {
public:
    DeathException(const std::string& msg) : std::runtime_error(msg) {}
};

// Шаблонный класс для записи логов в файл
template <typename T>
class Logger {
private:
    std::ofstream file;
public:
    Logger(const std::string& filename) {
        file.open(filename, std::ios::app);
        if (!file) throw std::runtime_error("Не удалось открыть лог-файл");
    }
    ~Logger() { if (file.is_open()) file.close(); }
    void log(const T& entry) { file << entry << std::endl; }
};

// Базовый класс персонажа и монстра
class Character {
protected:
    std::string name;
    int health;
    int attack;
    int defense;
    int level;
    int experience;
public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d), level(1), experience(0) {
    }
    virtual ~Character() = default;

    void takeDamage(int dmg) {
        health -= dmg;
        if (health < 0) health = 0;
    }
    bool isDead() const { return health <= 0; }

    void attackEnemy(Character& enemy, Logger<std::string>& logger) {
        int dmg = attack - enemy.defense;
        if (dmg <= 0) dmg = 1;
        enemy.takeDamage(dmg);
        logger.log(name + " атакует " + enemy.getName() + " на " + std::to_string(dmg));
    }

    void heal(int amount, Logger<std::string>& logger) {
        health += amount;
        if (health > 100) health = 100;
        logger.log(name + " восстанавливает " + std::to_string(amount) + " HP");
    }

    void gainExp(int exp, Logger<std::string>& logger) {
        experience += exp;
        logger.log(name + " получает " + std::to_string(exp) + " опыта");
        if (experience >= 100) {
            level++;
            experience -= 100;
            logger.log(name + " повышает уровень до " + std::to_string(level));
        }
    }

    virtual void showInfo() const {
        std::cout << "Имя: " << name
            << ", HP: " << health
            << ", Атака: " << attack
            << ", Защита: " << defense
            << ", Уровень: " << level
            << ", Опыт: " << experience << std::endl;
    }

    virtual void save(std::ofstream& fout) const {
        fout << name << ' ' << health << ' ' << attack << ' ' << defense
            << ' ' << level << ' ' << experience << '\n';
    }
    virtual void load(std::ifstream& fin) {
        fin >> name >> health >> attack >> defense >> level >> experience;
    }

    const std::string& getName() const { return name; }
    int getHealth() const { return health; }
};

class Monster : public Character {
public:
    using Character::Character;
};

class Goblin : public Monster { public: Goblin() : Monster("Гоблин", 30, 10, 5) {} };
class Dragon : public Monster { public: Dragon() : Monster("Дракон", 200, 40, 20) {} };
class Skeleton : public Monster { public: Skeleton() : Monster("Скелет", 50, 15, 10) {} };

// Инвентарь
class Inventory {
private:
    std::vector<std::string> items;
public:
    void add(const std::string& item) { items.push_back(item); }
    void remove(const std::string& item) {
        auto it = std::find(items.begin(), items.end(), item);
        if (it != items.end()) items.erase(it);
    }
    void show() const {
        std::cout << "Инвентарь:";
        if (items.empty()) std::cout << " пуст";
        for (const auto& i : items) std::cout << " " << i;
        std::cout << std::endl;
    }
    bool has(const std::string& item) const {
        return std::find(items.begin(), items.end(), item) != items.end();
    }
    void save(std::ofstream& fout) const {
        fout << items.size() << '\n';
        for (const auto& i : items) fout << i << '\n';
    }
    void load(std::ifstream& fin) {
        size_t n; fin >> n; fin.ignore();
        items.clear();
        for (size_t i = 0; i < n; ++i) {
            std::string it; std::getline(fin, it);
            items.push_back(it);
        }
    }
};

class Game {
private:
    Character player;
    Inventory inv;
    Logger<std::string> log;

    // Использовать зелье лечения
    void usePotion() {
        const std::string potion = "Health Potion";
        if (inv.has(potion)) {
            player.heal(20, log);
            inv.remove(potion);
            std::cout << "Вы использовали зелье. HP = " << player.getHealth() << std::endl;
        }
        else {
            std::cout << "У вас нет зелья!" << std::endl;
        }
    }

public:
    Game(const std::string& name)
        : player(name, 100, 20, 10), log("game.log") {
        // Начальный предмет
        inv.add("Health Potion");
    }

    void start() {
        std::cout << "=== Добро пожаловать в RPG ===" << std::endl;
        player.showInfo(); inv.show();
    }

    void battle(std::unique_ptr<Monster> m) {
        std::cout << "Появился " << m->getName() << "!" << std::endl;
        while (!player.isDead() && !m->isDead()) {
            std::cout << "Выберите действие: 1) Атаковать 2) Зелье\n";
            int choice; std::cin >> choice;
            if (choice == 2) {
                usePotion();
            }
            else {
                player.attackEnemy(*m, log);
            }
            if (m->isDead()) break;
            m->attackEnemy(player, log);
            std::cout << "Ваше HP: " << player.getHealth() << std::endl;
        }

        if (!player.isDead()) {
            std::cout << "Вы победили " << m->getName() << "!" << std::endl;
            player.gainExp(50, log);
            // шанс дропа
            inv.add("Health Potion");
            std::cout << "Вы получили Health Potion!" << std::endl;
        }
        else {
            throw DeathException(player.getName() + " погиб!");
        }
    }

    void save(const std::string& file) {
        std::ofstream fout(file);
        player.save(fout);
        inv.save(fout);
        std::cout << "Сохранено в " << file << std::endl;
    }
    void load(const std::string& file) {
        std::ifstream fin(file);
        if (!fin) { std::cerr << "Не удалось загрузить файл" << std::endl; return; }
        player.load(fin); inv.load(fin);
        std::cout << "Загружено из " << file << std::endl;
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    Game game("Герой");
    game.start();
    game.battle(std::make_unique<Goblin>());
    game.save("save.txt");
    Game game2("Герой"); game2.load("save.txt"); game2.start();
    game2.battle(std::make_unique<Skeleton>());
    try {
        game2.battle(std::make_unique<Dragon>());
    }
    catch (const DeathException& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}