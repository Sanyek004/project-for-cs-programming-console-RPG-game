#include <iostream>
#include <string>
#include <iomanip> // Для красивого форматирования вывода
#include <ctime> // Для генерации рандомных числовых значений
#include <string>
#include <vector>
#include <algorithm>

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

    // Метод для извлечение предмета из инвентаря
    Item* detachItem(int index)
    {
        if (index >= 0 && index < items.size())
        {
            Item* itemToGive = items[index];
            // Удаляем предмет только из вектора
            items.erase(items.begin() + index);
            return itemToGive;
        }
        return nullptr;
    }

    // Ищет предмет определенного типа, удаляет его и возвращает эффект. Если нет - возвращает 0.
    int consumeItemByType(std::string type) {
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i]->type == type) {
                int effect = items[i]->effect;
                std::cout << " [Inventory] Used: " << items[i]->getName() << std::endl;
                delete items[i];
                items.erase(items.begin() + i);
                return effect;
            }
        }
        return 0;
    }
};

class Character {
protected:
    // Характеристики персонажа
    // Используем protected, чтобы классы-наследники имели к ним доступ
    int hp;
    int maxHp; // Добавлено чтобы не лечиться до бесконечности
    int attack;
    int defense;
    int agility;
    double height;
    double weight;
    int skills;
    int level;

    // Инвентарь как часть персонажа
    Inventory inventory;

    // Вектор слотов экипировки
    std::vector<int> gearSlots;
public:
    // Конструктор с параметрами для инициализации
    Character(int h, int atk, int def, int agi, double he, double we, int sk, int lvl)
        : hp(h), maxHp(h), attack(atk), defense(def), agility(agi), height(he), weight(we), skills(sk), level(lvl)
    {
        gearSlots = { 0, 0 };
    }

    // Виртуальный деструктор (обязателен для базовых классов)
    virtual ~Character() {}

    // Виртуальная функция (делает класс абстрактным)
    // Наследники обязаны реализовать этот метод
    virtual std::string getName() const = 0;

    bool isAlive() const { return hp > 0; }
    int getHP() const { return hp; }

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

    // Расчёт полного урона
    int calculateTotalDamage() const {
        int baseDmg = attack + gearSlots[0];

        // Шанс критического урона зависит от ловкости (каждые 10 ловкости = 1% крит. урона, минимум 5%)
        int critChance = 5 + (agility / 10);
        bool isCrit = (std::rand() % 100) < critChance;

        if (isCrit) {
            std::cout << "CRITICAL HIT! ";
            return static_cast<int>(baseDmg * 1.5);
        }
        return baseDmg;
    }

    // Расчёт полной защиты
    int calculateTotalDefense() const {
        return defense + gearSlots[1];
    }

    // Получение урона с учётом уклонение и брони
    // isDodging - флаг, пытается ли персонаж активно уклоняться
    void takeDamage(int incomingDamage, bool isDodging) {
        // Шанс уклонения
        // Базовый: Ловкость * 0.5
        // Если выбрано действие "Уклонение": Ловкость * 2 + 30% бонус
        int dodgeChance = static_cast<int>(agility * 0.5);
        if (isDodging) {
            dodgeChance = (agility * 2) + 30;
            std::cout << getName() << " takes a defensive stance!" << std::endl;
        }

        // Ограничим шанс уклонения 90%
        if (dodgeChance > 90) dodgeChance = 90;

        if ((std::rand() % 100) < dodgeChance) {
            std::cout << ">>> " << getName() << " DODGED the attack! (0 Damage)" << std::endl;
            return;
        }

        // Расчет чистого урона
        int mitigation = calculateTotalDefense();
        // Рандомный разброс защиты +/- 10%
        mitigation += (std::rand() % 5) - 2;

        int finalDamage = std::max(1, incomingDamage - mitigation); // Минимум 1 урон

        hp -= finalDamage;
        std::cout << ">>> " << getName() << " takes " << finalDamage << " damage! (Defended: " << mitigation << ")" << std::endl;
        std::cout << "HP Left: " << hp << "/" << maxHp << std::endl;
    }

    // Восстановление ХП
    void heal() {
        int healAmount = inventory.consumeItemByType("Magic");
        if (healAmount > 0) {
            hp += healAmount;
            if (hp > maxHp) hp = maxHp;
            std::cout << getName() << " healed for " << healAmount << " HP. Current HP: " << hp << std::endl;
        }
        else {
            std::cout << "No potions found in inventory!" << std::endl;
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

class Enemy : public Character {
private:
    int xpReward; // Опыт, который дается за убийство
public:
    // Конструктор генерирует случайного врага
    Enemy() : Character(
        std::rand() % 40 + 50,  // HP 
        std::rand() % 10 + 10,    // Attack
        std::rand() % 5 + 1,    // Defense
        std::rand() % 10 + 1,    // Agility
        170,                    // Height
        80,                     // Weight
        0,                      // Skills
        1                       // Level
    ) {}

    virtual ~Enemy() {}

    std::string getName() const override {
        return "Dark Horse";
    }

    // Логика атаки врага, принимающая во внимание состояние игрока
    void attackPlayer(Character* player, bool playerIsDodging) {
        std::cout << "\n--- Enemy's Turn ---" << std::endl;
        int enemyDmg = this->calculateTotalDamage();
        std::cout << getName() << " attacks with power " << enemyDmg << "!" << std::endl;

        // Враг наносит урон, игрок пытается защититься
        player->takeDamage(enemyDmg, playerIsDodging);
    }

};

// Функция боя
void battle(Character* player, Enemy* enemy) {
    std::cout << "\n!!! A WILD " << enemy->getName() << " APPEARED !!!" << std::endl;
    std::cout << "Enemy Stats -> HP: " << enemy->getHP() << std::endl;

    while (player->isAlive() && enemy->isAlive()) {
        std::cout << "\n========================================" << std::endl;
        std::cout << "Your HP: " << player->getHP() << " | Enemy HP: " << enemy->getHP() << std::endl;
        std::cout << "Choose action:\n1. Attack\n2. Use Potion\n3. Dodge\n4. Inventory\n> ";

        int choice;
        std::cin >> choice;

        bool playerIsDodging = false; // Флаг уклонения на этот ход

        // --- ХОД ИГРОКА ---
        switch (choice) {
        case 1: // Атака
        {
            int dmg = player->calculateTotalDamage();
            std::cout << "You attack for " << dmg << " raw damage!" << std::endl;
            enemy->takeDamage(dmg, false); // Враг не уклоняется (для простоты)
            break;
        }
        case 2: // Предмет
            player->heal();
            break;
        case 3: // Уклонение
            playerIsDodging = true;
            std::cout << "You focus on evading the next attack..." << std::endl;
            break;
        case 4:
            player->checkInventory();
            std::cout << "Enter item index to equip (or 0 to cancel): ";
            int eq;
            std::cin >> eq;
            if (eq > 0) player->equipItem(eq);
            continue; // Возвращаемся в начало цикла, не тратя ход на просмотр
        default:
            std::cout << "You stumbled and lost your turn!" << std::endl;
        }

        // Если враг умер от атаки игрока
        if (!enemy->isAlive()) {
            std::cout << "\nVICTORY! The enemy is defeated." << std::endl;
            break;
        }

        // --- ХОД ВРАГА ---
        // Враг атакует, передаем ему информацию, уклоняется ли игрок
        enemy->attackPlayer(player, playerIsDodging);

        if (!player->isAlive()) {
            std::cout << "\nGAME OVER... You died." << std::endl;
        }
    }
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(0)));



    return 0;
}