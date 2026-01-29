#include <iostream>
#include <string>
#include <iomanip> // Для красивого форматирования вывода
#include <ctime> // Для генерации рандомных числовых значений
#include <string>

class Character {
protected:
    // Характеристики персонажа
    // Используем protected, чтобы классы-наследники имели к ним доступ
    int hp;          // Здоровье
    int attack;      // Сила атаки
    int defense;     // Защита
    int agility;     // Ловкость
    double height;   // Рост
    double weight;   // Вес
    // | FOR GAME |
    int skills;
    int level;
public:
    // Конструктор с параметрами для инициализации
    Character(int h, int atk, int def, int agi, double he, double we, int sk, int lvl)
        : hp(h), attack(atk), defense(def), agility(agi), height(he), weight(we), skills(sk), level(lvl) {}

    // Виртуальный деструктор (обязателен для базовых классов)
    virtual ~Character() {}

    // Виртуальная функция (делает класс абстрактным)
    // Наследники обязаны реализовать этот метод
    virtual std::string getName() const = 0;

    // Метод вывода характеристик
    virtual void printStats() const {
        std::cout << "==========================" << std::endl;
        std::cout << "Hero: " << getName() << std::endl; // Используем чисто виртуальный метод
        std::cout << "==========================" << std::endl;
        std::cout << "HP:      " << hp << std::endl;
        std::cout << "Attack:  " << attack << std::endl;
        std::cout << "Defense: " << defense << std::endl;
        std::cout << "Agility: " << agility << std::endl;
        std::cout << "Height:  " << height << " cm" << std::endl;
        std::cout << "Weight:  " << weight << " kg" << std::endl;
        std::cout << "==========================" << std::endl;
        std::cout << "GAME STATS:" << std::endl;
        std::cout << "Skills: " << skills << std::endl;
        std::cout << "Level: " << level << std::endl;
    }
};

class Person : public Character {
public:
    Person() : Character(
        std::rand() % 50 + 50,
        std::rand() % 10 + 1,
        std::rand() % 10 + 1,
        std::rand() % 10 + 1,
        std::rand() % 100 + 100,
        std::rand() % 100 + 50,
        0,
        0
    ) {}
    
    virtual ~Person() {}

    std::string getName() const override {
        return "Mark Down";
    }
};

class Elfie : public Character {
public:
    Elfie() : Character(
        std::rand() % 100 + 100,
        std::rand() % 20 + 5,
        std::rand() % 20 + 5,
        std::rand() % 20 + 5,
        std::rand() % 100 + 100,
        std::rand() % 100 + 50,
        0,
        0
    ) {}

    virtual ~Elfie() {}

    std::string getName() const override {
        return "Elfie A";
    }
};

class Dwarves : public Character {
public:
    Dwarves() : Character(
       std::rand() % 200 + 200,
       std::rand() % 30 + 10,
       std::rand() % 30 + 10,
       std::rand() % 30 + 10,
       std::rand() % 100 + 100,
       std::rand() % 100 + 50,
        0,
        0
    ) {}

    virtual ~Dwarves() {}

    std::string getName() const override {
        return "Dwarves D";
    }
};

class Item {
protected:
    std::string type;
    int effect;
public:
    // Конструктор с параметрами для инициализации
    Item(std::string t, int eff)
        : type(t), effect(eff) {}

    // Виртуальный деструктор (обязателен для базовых классов)
    virtual ~Item() {}

    // Виртуальная функция (делает класс абстрактным)
    // Наследники обязаны реализовать этот метод
    virtual std::string getName() const = 0;

    // Метод вывода характеристик
    virtual void printStats() const {
        std::cout << "Item: " << getName() << std::endl;
        std::cout << "Type: " << type << std::endl;
    }
};

class Potion_Health : public Item {
public:
    Potion_Health() : Item(
        "Magic",
        25
    ) {}

    virtual ~Potion_Health() {}

    std::string getName() const override {
        return "Potion Health";
    }
};

class Coin : public Item {
public:
    Coin() : Item(
        "Attack",
        5
    ) {}

    virtual ~Coin() {}

    std::string getName() const override {
        return "Coin";
    }
};

class Glock_17 : public Item {
public:
    Glock_17() : Item(
        "Attack",
        100
    ) {}

    virtual ~Glock_17() {}

    std::string getName() const override {
        return "Glock 17";
    }
};

class Annihilator_Cannon : public Item {
public:
    Annihilator_Cannon() : Item(
        "Attack",
        500
    ) {}

    virtual ~Annihilator_Cannon() {}

    std::string getName() const override {
        return "Annihilator Cannon";
    }
};


class Inventory {
public:
    Inventory() {}
    ~Inventory() {};


};

int main()
{
    std::srand(static_cast<unsigned int>(std::time(0)));



    return 0;
}