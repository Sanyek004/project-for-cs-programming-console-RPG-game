#include <iostream>
#include <string>
#include <iomanip> // Для красивого форматирования вывода
#include <ctime> // Для генерации рандомных числовых значений
#include <string>
#include <vector>
#include <windows.h>

class Item {
public:
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
        "Currency",
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

class Kevlar_Vest : public Item {
public:
    Kevlar_Vest() : Item("Armor", 150) {} // 150 - прочность
    ~Kevlar_Vest() {}
    std::string getName() const override { return "Kevlar Vest"; }
};


class Inventory {
private:
    // Храним указатели на базовый класс Item поскольку предметы
    // могут быть разными
    std::vector<Item*> items;
public:
    Inventory() {}

    ~Inventory() {
        // В Деструкторе очищаем память, так как храним указатели
        for (Item* item : items)
        {
            delete item;
        }
        items.clear();
    }

    void addItem(Item* item)
    {
        items.push_back(item);
        std::cout << "Added [" << item->getName() << "] to inventory. " << std::endl;
    }

    void showItems() const
    {
        std::cout << "=== INVENTORY ===" << std::endl;
        if (items.empty())
        {
            std::cout << "Inventory is Empty" << std::endl;
        }
        else
        {
            for (int i = 0; i < items.size(); ++i)
            {
                std::cout << i + 1 << ". ";
                items[i]->printStats();
            }
            std::cout << "----------------------------" << std::endl;
        }
    }

    void dropItem(int index)
    {
        // "index - 1" так как пользователь вводит число от 1, а вектор с 0
        int vectorIndex = index - 1;

        if (vectorIndex >= 0 && vectorIndex < items.size())
        {
            std::cout << "Dropped [" << items[vectorIndex]->getName() << "]" << std::endl;

            delete items[vectorIndex]; // Удаляем объект из памяти
            items.erase(items.begin() + vectorIndex); // Удаляем указатель из вектора
        }
        else
        {
            std::cout << "Invalid item number!" << std::endl;
        }
    }

    // Получение количество предметов
    int getSize() const
    {
        return items.size();
    }

    // Метод для получения указателя на предмет по индексу (чтобы надеть на экипировку что-то)
    Item* getItem(int index)
    {
        if (index >= 0 && index < items.size())
        {
            return items[index];
        }
        return nullptr;
    }

};

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

    // Инвентарь как часть персонажа
    Inventory inventory;

    // Вектор слотов экипировки
    std::vector<int> gearSlots;
public:
    // Конструктор с параметрами для инициализации
    Character(int h, int atk, int def, int agi, double he, double we, int sk, int lvl)
        : hp(h), attack(atk), defense(def), agility(agi), height(he), weight(we), skills(sk), level(lvl) 
    {
        gearSlots = { 0,0 };
    }

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

    // Метод подбора предмета
    void pickUp(Item* item)
    {
        inventory.addItem(item);
    }

    // Метод просмотра инвентаря
    void checkInventory() const
    {
        std::cout << getName() << " checks their inventory: " << std::endl;
        inventory.showItems();
    }

    // Метод выбрасывания предмета
    void throwAwayItem(int index)
    {
        inventory.dropItem(index);
    }

    // Метод надевания предмета на экипировку
    void equipItem(int inventoryIndex)
    {
        // Получаем индекс массива
        Item* item = inventory.getItem(inventoryIndex - 1);

        if (item == nullptr)
        {
            std::cout << "No such item to equip!" << std::endl;
            return;
        }

        if (item->type == "Attack")
        {
            gearSlots[0] = item->effect; // Записываем сколько урона будет давать опред. вид оружие в нулевой слот
            std::cout << " >> Equipped Weapon: " << item->getName() << " ( +" << item->effect << " Damage)" << std::endl;
        }
        else if (item->type == "Armor")
        {
            gearSlots[1] = item->effect; // Записываем числовое значение брони в первый слот
            std::cout << ">> Equipped Armor: " << item->getName() << " (" << item->effect << " Durability)" << std::endl;
        }
        else
        {
            std::cout << ">> Cannot equip item of type: " << item->type << std::endl;
        }
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

class Room {
protected:
   
public:
    // Конструктор с параметрами для инициализации
    Room() {}

    // Виртуальный деструктор (обязателен для базовых классов)
    virtual ~Room() {}

    // Виртуальная функция (делает класс абстрактным)
    // Наследники обязаны реализовать этот метод
    virtual std::string getName() const = 0;

    // Метод вывода характеристик
    virtual void printStats() const 
    {

    }
};

class Room_Relax : public Room {

public:
    Room_Relax() : Room()
    {
        std::cout << "There is Room Relax. Time break, bro!" << std::endl;
    }

    virtual ~Room_Relax() {}

    std::string getName() const override {
        return "Room Relax";
    }
};

class Room_Chest : public Room {

public:
    Room_Chest() : Room()
    {
        std::cout << "There is Room Chest. It's time to take items!" << std::endl;

    }

    virtual ~Room_Chest() {};

    std::string getName() const override {
        return "Room Chest";
    }

    // Самый главный метод с сундуком
    void loot(Character* player)
    {
        std::cout << "You see a chest opening..." << std::endl;
        Sleep(3000);

        // Генерируем случайное число от 0 до 4
        int randomChance = std::rand() % 5;

        Item* newItem = nullptr;

        switch (randomChance)
        {
        case 0:
            newItem = new Potion_Health();
            break;

        case 1:
            newItem = new Coin();
            break;

        case 2:
            newItem = new Glock_17();
            break;

        case 3:
            newItem = new Annihilator_Cannon();
            break;

        case 4:
            newItem = new Kevlar_Vest();
            break;


        default:
            newItem = new Coin();
            break;
        }

        if (newItem != nullptr)
        {
            std::cout << "Luck in on your side! You found: " << newItem->getName() << std::endl;
            // Добавляем предмет найденный в инвентарь персонажа
            player->pickUp(newItem);
        }
    }
};

int main()
{
    std::srand(static_cast<unsigned int>(std::time(0)));



    return 0;
}