#include <iostream>
#include <string>
#include <vector>

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

    virtual ~Entity() {}
};

class Player : public Entity {
private:
    int experience; // Приватное поле: опыт

public:
    // Конструктор производного класса
    Player(const std::string& n, int h, int exp)
        : Entity(n, h), experience(exp) {
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        Entity::displayInfo(); // Вызов метода базового класса
        std::cout << "Experience: " << experience << std::endl;
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
        entities.push_back(entity);
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity -> displayInfo();
        }
    }
};

int main() {
    GameManager<Entity*> manager;
    manager.addEntity(new Player("Hero", 100, 0));
    manager.addEntity(new Enemy("Goblin", 50, "Goblin"));
    manager.displayAll();
    Queue<std::string> stringQueue;
    stringQueue.push("Hello");
    stringQueue.push("World");
    stringQueue.push("!");

    std::cout << "String Queue:" << std::endl;
    while (!stringQueue.isEmpty()) {
        std::cout << stringQueue.pop() << std::endl;
    }

    Queue<int> intQueue;
    intQueue.push(10);
    intQueue.push(20);
    intQueue.push(30);

    std::cout << "\nInteger Queue:" << std::endl;
    while (!intQueue.isEmpty()) {
        std::cout << intQueue.pop() << std::endl;
    }

    // Проверка обработки пустой очереди
    try {
        intQueue.pop();
    }
    catch (const std::runtime_error& e) {
        std::cout << "\nError: " << e.what() << std::endl;
    }

    return 0;
}