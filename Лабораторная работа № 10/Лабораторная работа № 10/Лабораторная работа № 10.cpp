#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>

// Базовый класс пользователя
class User {
protected:
    std::string name;
    int id;
    int accessLevel;

public:
    User(const std::string& name, int id, int accessLevel)
        : name(name), id(id), accessLevel(accessLevel)
    {
        if (name.empty()) throw std::invalid_argument("Имя не может быть пустым");
        if (accessLevel < 0) throw std::invalid_argument("Недопустимый уровень доступа");
    }

    virtual ~User() = default;

    // Геттеры
    std::string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    // Сеттеры с проверкой входных данных
    void setName(const std::string& newName) {
        if (newName.empty()) throw std::invalid_argument("Имя не может быть пустым");
        name = newName;
    }

    void setAccessLevel(int newLevel) {
        if (newLevel < 0) throw std::invalid_argument("Недопустимый уровень доступа");
        accessLevel = newLevel;
    }

    // Виртуальный метод для отображения информации
    virtual void displayInfo() const {
        std::cout << "Имя: " << name << ", ID: " << id
            << ", Уровень доступа: " << accessLevel;
    }
};

// Класс студента
class Student : public User {
    std::string group;

public:
    Student(const std::string& name, int id, int accessLevel, const std::string& group)
        : User(name, id, accessLevel), group(group) {
    }

    std::string getGroup() const { return group; }

    void displayInfo() const override {
        User::displayInfo();
        std::cout << ", Группа: " << group << " (Студент)\n";
    }
};

// Класс преподавателя
class Teacher : public User {
    std::string department;

public:
    Teacher(const std::string& name, int id, int accessLevel, const std::string& department)
        : User(name, id, accessLevel), department(department) {
    }

    std::string getDepartment() const { return department; }

    void displayInfo() const override {
        User::displayInfo();
        std::cout << ", Кафедра: " << department << " (Преподаватель)\n";
    }
};

// Класс администратора
class Administrator : public User {
    std::string office;

public:
    Administrator(const std::string& name, int id, int accessLevel, const std::string& office)
        : User(name, id, accessLevel), office(office) {
    }

    std::string getOffice() const { return office; }

    void displayInfo() const override {
        User::displayInfo();
        std::cout << ", Офис: " << office << " (Администратор)\n";
    }
};

// Класс ресурса
class Resource {
    std::string name;
    int requiredAccess;

public:
    Resource(const std::string& name, int requiredAccess)
        : name(name), requiredAccess(requiredAccess)
    {
        if (name.empty()) throw std::invalid_argument("Имя ресурса не может быть пустым");
        if (requiredAccess < 0) throw std::invalid_argument("Недопустимый уровень доступа");
    }

    // Проверка доступа пользователя к ресурсу
    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccess;
    }

    std::string getName() const { return name; }
    int getRequiredAccess() const { return requiredAccess; }
};

// Шаблонный класс системы контроля доступа
template<typename T = Resource>
class AccessControlSystem {
    std::vector<std::unique_ptr<User>> users;
    std::vector<T> resources;

public:
    // Добавление пользователя
    void addUser(std::unique_ptr<User> user) {
        users.push_back(std::move(user));
    }

    // Добавление ресурса
    void addResource(const T& resource) {
        resources.push_back(resource);
    }

    // Проверка доступа пользователя к ресурсу
    bool checkAccess(int userId, const std::string& resourceName) const {
        // Поиск пользователя
        auto userIt = std::find_if(users.begin(), users.end(),
            [userId](const auto& u) { return u->getId() == userId; });
        if (userIt == users.end())
            throw std::runtime_error("Пользователь не найден");

        // Поиск ресурса
        auto resIt = std::find_if(resources.begin(), resources.end(),
            [&resourceName](const auto& r) { return r.getName() == resourceName; });
        if (resIt == resources.end())
            throw std::runtime_error("Ресурс не найден");

        return resIt->checkAccess(**userIt);
    }

    // Сохранение пользователей в файл
    void saveUsersToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Не удалось открыть файл");

        for (const auto& user : users) {
            // Запись типа пользователя и его данных
            if (auto s = dynamic_cast<Student*>(user.get())) {
                file << "Student " << s->getName() << " " << s->getId() << " "
                    << s->getAccessLevel() << " " << s->getGroup() << "\n";
            }
            else if (auto t = dynamic_cast<Teacher*>(user.get())) {
                file << "Teacher " << t->getName() << " " << t->getId() << " "
                    << t->getAccessLevel() << " " << t->getDepartment() << "\n";
            }
            else if (auto a = dynamic_cast<Administrator*>(user.get())) {
                file << "Administrator " << a->getName() << " " << a->getId() << " "
                    << a->getAccessLevel() << " " << a->getOffice() << "\n";
            }
        }
    }

    // Загрузка пользователей из файла
    void loadUsersFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Не удалось открыть файл");

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string type, name, data;
            int id, accessLevel;

            iss >> type >> name >> id >> accessLevel >> data;

            if (type == "Student") {
                addUser(std::make_unique<Student>(name, id, accessLevel, data));
            }
            else if (type == "Teacher") {
                addUser(std::make_unique<Teacher>(name, id, accessLevel, data));
            }
            else if (type == "Administrator") {
                addUser(std::make_unique<Administrator>(name, id, accessLevel, data));
            }
        }
    }

    // Сохранение ресурсов в файл
    void saveResourcesToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Не удалось открыть файл");

        for (const auto& resource : resources) {
            file << resource.getName() << " " << resource.getRequiredAccess() << "\n";
        }
    }

    // Загрузка ресурсов из файла
    void loadResourcesFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Не удалось открыть файл");

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string name;
            int requiredAccess;

            iss >> name >> requiredAccess;
            addResource(T(name, requiredAccess));
        }
    }

    // Поиск пользователей по имени (может быть несколько с одинаковым именем)
    std::vector<User*> findUsersByName(const std::string& name) const {
        std::vector<User*> result;
        for (const auto& u : users) {
            if (u->getName() == name)
                result.push_back(u.get());
        }
        return result;
    }

    // Поиск пользователя по ID
    User* findUserById(int id) const {
        auto it = std::find_if(users.begin(), users.end(),
            [id](const auto& u) { return u->getId() == id; });
        return it != users.end() ? it->get() : nullptr;
    }

    // Сортировка пользователей по уровню доступа
    void sortByAccessLevel() {
        std::sort(users.begin(), users.end(),
            [](const auto& a, const auto& b) {
                return a->getAccessLevel() < b->getAccessLevel();
            });
    }

    // Вывод информации о всех пользователях
    void printAllUsers() const {
        for (const auto& user : users) {
            user->displayInfo();
        }
    }

    // Вывод информации о всех ресурсах
    void printAllResources() const {
        for (const auto& resource : resources) {
            std::cout << "Ресурс: " << resource.getName()
                << ", Требуемый уровень доступа: "
                << resource.getRequiredAccess() << "\n";
        }
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    try {
        AccessControlSystem<> system;

        // Добавление пользователей
        system.addUser(std::make_unique<Student>("Иван", 1, 1, "Группа101"));
        system.addUser(std::make_unique<Teacher>("Мария", 2, 2, "Информатика"));
        system.addUser(std::make_unique<Administrator>("Ольга", 3, 5, "Кабинет200"));
        system.addUser(std::make_unique<Student>("Иван", 4, 1, "Группа102")); // Еще один Иван

        // Добавление ресурсов
        system.addResource(Resource("Лаборатория1", 2));
        system.addResource(Resource("Архив", 4));
        system.addResource(Resource("Библиотека", 1));

        // Сохранение данных
        system.saveUsersToFile("users.txt");
        system.saveResourcesToFile("resources.txt");

        // Загрузка данных (можно закомментировать для теста)
         AccessControlSystem<> newSystem;
         newSystem.loadUsersFromFile("users.txt");
         newSystem.printAllUsers();
         newSystem.loadResourcesFromFile("resources.txt");
         newSystem.printAllResources();
        // Поиск пользователей по имени
        auto ivanUsers = system.findUsersByName("Иван");
        std::cout << "Найдено пользователей с именем Иван: " << ivanUsers.size() << "\n";
        for (auto user : ivanUsers) {
            user->displayInfo();
        }

        // Проверка доступа
        std::cout << "Доступ Ивана в Лаборатория1: "
            << system.checkAccess(1, "Лаборатория1") << "\n";
        std::cout << "Доступ Марии в Архив: "
            << system.checkAccess(2, "Архив") << "\n";

        // Вывод всех ресурсов
        std::cout << "\nВсе ресурсы:\n";
        system.printAllResources();

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }

    return 0;
}