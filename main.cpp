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
        35
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
        10
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
        25
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
        75
    ) {}

    virtual ~Annihilator_Cannon() {}

    std::string getName() const override {
        return "Annihilator Cannon";
    }
};

class Kevlar_Vest : public Item {
public:
    Kevlar_Vest() : Item("Armor", 20) {} 
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
    int hp;
    int maxHp;
    int attack;
    int defense;
    int agility;

    int level;
    int currentXp;
    int xpToNextLevel;
    int skillPoints;

    Inventory inventory;
    std::vector<int> gearSlots; // 0: Weapon Dmg, 1: Armor Def

public:
    Character(int h, int atk, int def, int agi, int lvl)
        : hp(h), maxHp(h), attack(atk), defense(def), agility(agi), level(lvl)
    {
        gearSlots = { 0, 0 };
        currentXp = 0;
        xpToNextLevel = 100;
        skillPoints = 0;
    }

    virtual ~Character() {}
    virtual std::string getName() const = 0;

    bool isAlive() const { return hp > 0; }
    int getHP() const { return hp; }
    int getMaxHP() const { return maxHp; }
    int getLevel() const { return level; }
    int getSkillPoints() const { return skillPoints; }

    // Геттеры "чистых" статов для расчета баланса врага
    int getRawTotalAttack() const { return attack + gearSlots[0]; }
    int getRawTotalDefense() const { return defense + gearSlots[1]; }

    void printStats() const {
        std::cout << "\n=== HERO SHEET: " << getName() << " ===" << std::endl;
        std::cout << " Lvl: " << level << " (" << currentXp << "/" << xpToNextLevel << " XP)" << std::endl;
        std::cout << " HP:  " << hp << "/" << maxHp << std::endl;
        std::cout << " Atk: " << attack << " (+" << gearSlots[0] << ")" << std::endl;
        std::cout << " Def: " << defense << " (+" << gearSlots[1] << ")" << std::endl;
        std::cout << " Agi: " << agility << std::endl;
        std::cout << " SP:  " << skillPoints << std::endl;
        std::cout << "=============================" << std::endl;
    }

    void levelUp() {
        currentXp -= xpToNextLevel;
        level++;
        skillPoints += 3;
        xpToNextLevel = static_cast<int>(xpToNextLevel * 1.4);
        hp = maxHp; // Полное лечение

        std::cout << "\n*** LEVEL UP! -> " << level << " ***" << std::endl;
    }

    void gainXp(int amount) {
        currentXp += amount;
        std::cout << " >> Gained " << amount << " XP." << std::endl;
        while (currentXp >= xpToNextLevel) {
            levelUp();
        }
    }

    void pickUp(Item* item) {
        inventory.addItem(item);
    }

    void checkInventory() {
        inventory.showItems();
        std::cout << "Actions: (e)quip <num>, (u)se <num>, (c)lose: ";
        char action;
        std::cin >> action;
        if (action == 'e') {
            int idx; std::cin >> idx;
            equipItem(idx);
        }
        else if (action == 'u') {
            heal();
        }
    }

    void equipItem(int inventoryIndex) {
        Item* item = inventory.getItem(inventoryIndex - 1);
        if (!item) {
            std::cout << "Invalid item." << std::endl;
            return;
        }

        if (item->type == "Attack") {
            gearSlots[0] = item->effect;
            std::cout << " >> Equipped Weapon: " << item->getName() << std::endl;
        }
        else if (item->type == "Armor") {
            gearSlots[1] = item->effect;
            std::cout << " >> Equipped Armor: " << item->getName() << std::endl;
        }
        else {
            std::cout << " >> Cannot equip this item." << std::endl;
        }
    }

    void distributeSkillPoints() {
        while (skillPoints > 0) {
            std::cout << "\n[UPGRADE] Points: " << skillPoints << std::endl;
            std::cout << "1. Strength (+2 Atk)\n2. Vitality (+15 HP)\n3. Agility (+2 Agi)\n4. Exit\n> ";
            int choice;
            std::cin >> choice;
            if (choice == 4) break;

            switch (choice) {
            case 1: attack += 2; skillPoints--; break;
            case 2: maxHp += 15; hp += 15; skillPoints--; break;
            case 3: agility += 2; skillPoints--; break;
            default: std::cout << "Invalid." << std::endl;
            }
        }
    }

    // Боевая механика
    int calculateTotalDamage() const {
        int baseDmg = attack + gearSlots[0];
        // Крит: каждые 5 ловкости = 1%
        int critChance = 5 + (agility / 5);
        if ((std::rand() % 100) < critChance) {
            std::cout << " [CRITICAL!] ";
            return static_cast<int>(baseDmg * 1.5);
        }
        return baseDmg;
    }

    int calculateTotalDefense() const {
        return defense + gearSlots[1];
    }

    void takeDamage(int incomingDamage, bool isDodging) {
        // Шанс уклонения
        int dodgeChance = static_cast<int>(agility * 0.5);
        if (isDodging) {
            dodgeChance = (agility * 2) + 25;
            std::cout << " (" << getName() << " rolls to dodge...) ";
        }
        if (dodgeChance > 85) dodgeChance = 85; // Кап уклонения

        if ((std::rand() % 100) < dodgeChance) {
            std::cout << ">>> MISS! " << getName() << " dodged (" << dodgeChance << "% chance)!" << std::endl;
            return;
        }

        int myDef = calculateTotalDefense();
        // Защита не может блокировать более 80% урона, чтобы игра не зависала в 0 урона
        int maxBlock = static_cast<int>(incomingDamage * 0.8);
        int actualBlock = std::min(myDef, maxBlock);

        // Добавим немного рандома в блок (+/- 2)
        actualBlock += (std::rand() % 5) - 2;
        if (actualBlock < 0) actualBlock = 0;

        int finalDamage = std::max(1, incomingDamage - actualBlock);

        hp -= finalDamage;
        std::cout << ">>> " << getName() << " hit for " << finalDamage << " dmg! (Armor blocked " << actualBlock << ")" << std::endl;
    }

    void heal() {
        int healAmount = inventory.consumeItemByType("Magic");
        if (healAmount > 0) {
            hp += healAmount;
            if (hp > maxHp) hp = maxHp;
            std::cout << " >> Healed. HP: " << hp << "/" << maxHp << std::endl;
        }
        else {
            std::cout << " >> No potions!" << std::endl;
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
        std::rand() % 100 + 100
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
        std::rand() % 100 + 100
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
       std::rand() % 100 + 100
    ) {}

    virtual ~Dwarves() {}

    std::string getName() const override {
        return "Dwarves D";
    }
};

class Enemy : public Character {
private:
    int xpReward;
public:
    // Конструктор принимает указатель на игрока для балансировки
    Enemy(Character* player) : Character(0, 0, 0, 0, player->getLevel()) {

        int pLvl = player->getLevel();

        // Враг должен жить примерно 3-4 хода при текущем уроне игрока.
        int playerDmg = std::max(5, player->getRawTotalAttack());
        this->maxHp = (playerDmg * 3) + (std::rand() % playerDmg);
        // Минимум ХП для врага (чтобы на 1 уровне не было слишком мало)
        if (this->maxHp < 40 + (pLvl * 10)) this->maxHp = 40 + (pLvl * 10);
        this->hp = this->maxHp;

        // Враг должен пробивать защиту игрока и наносить ощутимый урон.
        // Цель: снимать ~1/5 или 1/6 здоровья игрока за удар "чистыми".
        int playerDef = player->getRawTotalDefense();
        int desiredDmg = player->getMaxHP() / 6;
        if (desiredDmg < 5) desiredDmg = 5;

        // Атака врага = Защита игрока + Желаемый чистый урон + Рандом
        this->attack = playerDef + desiredDmg + (std::rand() % 4);

        // Баланс ловкости и защиты
        this->defense = pLvl * 2; // Символическая защита
        this->agility = 5 + pLvl;

        // Награда
        this->xpReward = 25 + (pLvl * 15);
    }

    std::string getName() const override { return "Monster"; }
    int getXpReward() const { return xpReward; }

    void attackPlayer(Character* target, bool dodging) {
        std::cout << "\n[ENEMY TURN] Monster attacks!" << std::endl;
        int dmg = calculateTotalDamage();
        target->takeDamage(dmg, dodging);
    }
};

// Функция боя
void battle(Character* player, Enemy* enemy) {
    std::cout << "\n!!! COMBAT STARTED !!!" << std::endl;
    while (player->isAlive() && enemy->isAlive()) {
        // Отображение ХП
        std::cout << "\n================= BATTLE STATS =================" << std::endl;
        std::cout << " [PLAYER] " << player->getName() << ": \t"
            << player->getHP() << " / " << player->getMaxHP() << " HP" << std::endl;
        std::cout << " [ENEMY]  " << enemy->getName() << ": \t"
            << enemy->getHP() << " / " << enemy->getMaxHP() << " HP" << std::endl;
        std::cout << "================================================" << std::endl;

        std::cout << "\nChoose: 1.Attack 2.Heal 3.Dodge 4.Inventory\n> ";
        int choice;
        std::cin >> choice;
        bool dodging = false;

        switch (choice) {
        case 1:
            enemy->takeDamage(player->calculateTotalDamage(), false);
            break;
        case 2:
            player->heal();
            break;
        case 3:
            dodging = true;
            break;
        case 4:
            player->checkInventory();
            break;
        default: std::cout << "Missed turn!" << std::endl;
        }

        if (enemy->isAlive()) {
            enemy->attackPlayer(player, dodging);
        }
        else {
            std::cout << "\nVICTORY! Enemy defeated." << std::endl;
        }
    }
}

class Room {
protected:
   
public:
    virtual ~Room() {}

    virtual std::string getType() const = 0; // Для предпросмотра (что видит игрок)
    virtual void enter(Character* player) = 0; // Что происходит внутри
};

class Room_Relax : public Room {
public:
    std::string getType() const override { return "Campfire (Safe)"; }
    void enter(Character* player) override {
        std::cout << "\nYou found a safe campfire." << std::endl;
        std::cout << "You rest and recover 20 HP." << std::endl;
        int healAmount = 20;

        // --- ПРОВЕРКА НА ПРОКАЧКУ ---
        if (player->getSkillPoints() > 0) {
            std::cout << "\n(!) You have " << player->getSkillPoints() << " unspent skill points!" << std::endl;
            std::cout << "Would you like to train? (y/n): ";
            char ans;
            std::cin >> ans;
            if (ans == 'y') {
                player->distributeSkillPoints();
            }
        }
        else {
            std::cout << "You have no skill points to spend right now." << std::endl;
        }
    }
};

class Room_Chest : public Room {

public:

    std::string getType() const override { return "Room Chest"; }

    void enter(Character* player) override {
        std::cout << "There is Room Chest. It's time to take items!" << std::endl;
        std::cout << "You found a chest!" << std::endl;

        int chance = std::rand() % 100;
        Item* loot = nullptr;

        if (chance < 40) loot = new Coin();
        else if (chance < 70) loot = new Potion_Health();
        else if (chance < 85) loot = new Glock_17();
        else if (chance < 95) loot = new Kevlar_Vest();
        else loot = new Annihilator_Cannon();

        std::cout << "You obtained: " << loot->getName() << std::endl;
        player->pickUp(loot);
    }
};

class Room_Enemy : public Room {
public:
    std::string getType() const override { return "Monster Lair"; }
    void enter(Character* player) override {
        std::cout << "\n--- DANGER ---" << std::endl;
        std::cout << "Something moves in the dark..." << std::endl;

        // Передаем игрока в конструктор врага для балансировки
        Enemy* enemy = new Enemy(player);
        battle(player, enemy);
        delete enemy;
    }
};



// Функция для генерации случайной комнаты
Room* generateRandomRoom() {
    int r = std::rand() % 100;
    if (r < 30) return new Room_Relax(); // 30%
    if (r < 60) return new Room_Chest(); // 30%
    return new Room_Enemy();             // 40%
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(0)));

    std::cout << "Welcome to the ENDLESS DUNGEON." << std::endl;
    std::cout << "Try to survive as long as possible.\n" << std::endl;

    Character* player = new Person();

    // Первая комната всегда пустая для разгона
    Room* currentRoom = new Room_Relax();
    int roomsCleared = 0;

    while (player->isAlive()) {
        // 1. Вход в текущую комнату
        currentRoom->enter(player);

        if (!player->isAlive()) break; // Если умер в комнате

        roomsCleared++;
        std::cout << "\nRooms cleared: " << roomsCleared << std::endl;

        // 2. Генерация развилки
        Room* leftRoom = generateRandomRoom();
        Room* rightRoom = generateRandomRoom();

        // 3. Механика видимости (50% шанс увидеть)
        bool canSee = (std::rand() % 2) == 0;

        std::cout << "\nThere is a fork in the path..." << std::endl;
        if (canSee) {
            std::cout << "[Visibility: HIGH] You can see down the corridors:" << std::endl;
            std::cout << "LEFT:  " << leftRoom->getType() << std::endl;
            std::cout << "RIGHT: " << rightRoom->getType() << std::endl;
        }
        else {
            std::cout << "[Visibility: ZERO] It is too dark to see what's ahead." << std::endl;
            std::cout << "LEFT:  ???" << std::endl;
            std::cout << "RIGHT: ???" << std::endl;
        }

        // 4. Выбор игрока
        char choice;
        while (true) {
            std::cout << "Choose direction (l/r) or (i)nventory: ";
            std::cin >> choice;
            if (choice == 'l' || choice == 'r') break;
            if (choice == 'i') player->checkInventory();
        }

        // 5. Переход и очистка памяти
        delete currentRoom; // Удаляем старую комнату

        if (choice == 'l') {
            std::cout << "You turn LEFT..." << std::endl;
            currentRoom = leftRoom;
            delete rightRoom; // Удаляем ту, в которую не пошли
        }
        else {
            std::cout << "You turn RIGHT..." << std::endl;
            currentRoom = rightRoom;
            delete leftRoom; // Удаляем ту, в которую не пошли
        }
    }

    std::cout << "\n===================================" << std::endl;
    std::cout << "GAME OVER" << std::endl;
    std::cout << "You cleared " << roomsCleared << " rooms." << std::endl;
    std::cout << "===================================" << std::endl;

    delete currentRoom;
    delete player;

    return 0;
}