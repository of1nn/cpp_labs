#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ostream>

class Entity {
protected:
    std::string name; // Защищенное поле: имя
    int health;      // Защищенное поле: здоровье

public:
    // Конструктор базового класса
    Entity(const std::string& n, int h) : name(n), health(h) {}

    // Метод для вывода информации
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << std::endl;
    }

    int getHealth() const {
        return health;
    }

    virtual void save(std::ofstream& out) const = 0;
    virtual void load(std::ifstream& in) = 0;

    virtual ~Entity() {}
};

class Player : public Entity {
private:
    int level; // Приватное поле: опыт

public:
    // Конструктор производного класса
    Player(const std::string& n, int h, int exp)
        : Entity(n, h), level(exp) {
    }
    Player() : Entity("", 0), level(0) {}

    // Переопределение метода displayInfo
    void displayInfo() const override {
        Entity::displayInfo(); // Вызов метода базового класса
        std::cout << "Level: " << level << std::endl;
    }

    void save(std::ofstream& out) const override {
        out << "Player\n" << name << "\n" << health << "\n" << level << "\n";
    }

    void load(std::ifstream& in) override {
        std::getline(in, name);
        in >> health >> level;
        in.ignore(); // Пропустить оставшийся символ новой строки
    }
};

class Enemy : public Entity {
private:
    std::string type; // Приватное поле: тип врага

public:
    // Конструктор производного класса
    Enemy(const std::string& n, int h, const std::string& t)
        : Entity(n, h), type(t) {
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        Entity::displayInfo(); // Вызов метода базового класса
        std::cout << "Type: " << type << std::endl;
    }
    void save(std::ofstream& out) const override {
        out << "Enemy\n" << name << "\n" << health << "\n" << type << "\n";
    }

    void load(std::ifstream& in) override {
        std::getline(in, name);
        in >> health >> type;
        in.ignore(); // Пропустить оставшийся символ новой строки
    }
};

template <typename T>
class Queue {
private:
    std::vector<T> elements; // Вектор для хранения элементов очереди

public:
    // Добавление элемента в конец очереди
    void push(const T& item) {
        elements.push_back(item);
    }

    // Удаление и возврат первого элемента из очереди
    T pop() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        T frontItem = elements.front(); // Сохраняем первый элемент
        elements.erase(elements.begin()); // Удаляем первый элемент
        return frontItem;
    }

    // Получение первого элемента без удаления
    T front() const {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        return elements.front();
    }

    // Проверка, пуста ли очередь
    bool isEmpty() const {
        return elements.empty();
    }
};

template <typename T>
class GameManager {
private:
    std::vector<T> entities;

public:
    void addEntity(const T& entity) {
        if (entity->getHealth() <= 0) {
            throw std::invalid_argument("Entity has invalid health");
        }
        entities.push_back(entity);
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity->displayInfo();
        }
    }

    std::vector<T> getEntities() const {
        return entities;
    }

};

void saveToFile(const GameManager<Entity*>& manager, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing.");
    }
    if (file.is_open()) 
    {
        const auto& entities = manager.getEntities();
        file << entities.size() << "\n";
        for (const auto& entity : entities) {
            entity->save(file);
        }
    }
    // Запись данных в файл
    
}

void loadFromFile(GameManager<Entity*>& manager, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for reading.");
    }
    size_t count;
    file >> count;
    file.ignore();
    for (size_t i = 0; i < count; ++i) {
        std::string type;
        std::getline(file, type);

        if (type == "Player") {
            Player* player = new Player();
            player->load(file);
            manager.addEntity(player);
        }
        else {
            throw std::runtime_error("Unknown entity type in file: " + type);
        }
    }


    // Чтение данных из файла
}

int main() {
    GameManager<Entity*> manager;
    manager.addEntity(new Player("Hero", 100, 5));
    manager.addEntity(new Player("Mage", 80, 3));
    manager.addEntity(new Player("Rogue", 90, 4));
    saveToFile(manager, "game_save.txt");

    GameManager<Entity*> loadedManager;
    loadFromFile(loadedManager, "game_save.txt");
    loadedManager.displayAll();

    return 0;
}
