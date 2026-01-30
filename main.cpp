#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <vector>
#include <algorithm>

// === 1. ПРЕДМЕТЫ ===

class Item {
public:
    std::string type;
    int effect;
public:
    Item(std::string t, int eff) : type(t), effect(eff) {}
    virtual ~Item() {}
    virtual std::string getName() const = 0;

    virtual void printStats() const {
        std::cout << getName() << " [" << type << " +" << effect << "]";
    }
};

class Potion_Health : public Item {
public:
    Potion_Health() : Item("Magic", 35) {}
    std::string getName() const override { return "Health Potion"; }
};

class Coin : public Item {
public:
    Coin() : Item("Currency", 10) {}
    std::string getName() const override { return "Gold Coin"; }
};

class Iron_Sword : public Item {
public:
    Iron_Sword() : Item("Attack", 15) {}
    std::string getName() const override { return "Iron Sword"; }
};

class Great_Axe : public Item {
public:
    Great_Axe() : Item("Attack", 30) {}
    std::string getName() const override { return "Great Axe"; }
};

class Leather_Armor : public Item {
public:
    Leather_Armor() : Item("Armor", 10) {}
    std::string getName() const override { return "Leather Armor"; }
};

class Plate_Mail : public Item {
public:
    Plate_Mail() : Item("Armor", 25) {}
    std::string getName() const override { return "Plate Mail"; }
};

// Функция-фабрика для генерации случайного лута
Item* generateRandomLoot() {
    int chance = std::rand() % 100;
    if (chance < 40) return new Potion_Health();
    if (chance < 60) return new Coin();
    if (chance < 75) return new Leather_Armor();
    if (chance < 85) return new Iron_Sword();
    if (chance < 95) return new Plate_Mail();
    return new Great_Axe();
}

// === 2. ИНВЕНТАРЬ (с ограничением места) ===

class Inventory {
private:
    std::vector<Item*> items;
    const size_t CAPACITY = 5; // Ограничение инвентаря по ТЗ
public:
    Inventory() {}

    ~Inventory() {
        for (Item* item : items) delete item;
        items.clear();
    }

    bool isFull() const { return items.size() >= CAPACITY; }

    void addItem(Item* item) {
        if (isFull()) {
            std::cout << "\n[!] Inventory is FULL! Cannot pick up " << item->getName() << std::endl;
            std::cout << "Do you want to drop an item to take this one? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'y') {
                showItems();
                std::cout << "Enter number to drop (0 to cancel): ";
                int idx;
                std::cin >> idx;
                if (idx > 0 && idx <= items.size()) {
                    dropItem(idx);
                    items.push_back(item);
                    std::cout << "Took " << item->getName() << "." << std::endl;
                    return;
                }
            }
            std::cout << "You left the " << item->getName() << " behind." << std::endl;
            delete item; // Удаляем, так как не подобрали
        }
        else {
            items.push_back(item);
            std::cout << " [Loot] Added [" << item->getName() << "]." << std::endl;
        }
    }

    void showItems() const {
        std::cout << "\n=== INVENTORY (" << items.size() << "/" << CAPACITY << ") ===" << std::endl;
        if (items.empty()) {
            std::cout << " (Empty)" << std::endl;
        }
        else {
            for (int i = 0; i < items.size(); ++i) {
                std::cout << i + 1 << ". ";
                items[i]->printStats();
                std::cout << std::endl;
            }
        }
        std::cout << "============================" << std::endl;
    }

    void dropItem(int index) {
        int vecIdx = index - 1;
        if (vecIdx >= 0 && vecIdx < items.size()) {
            std::cout << "Dropped " << items[vecIdx]->getName() << "." << std::endl;
            delete items[vecIdx];
            items.erase(items.begin() + vecIdx);
        }
    }
Item* getItem(int index) {
        if (index >= 0 && index < items.size()) return items[index];
        return nullptr;
    }

    int consumeItemByType(std::string type) {
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i]->type == type) {
                int effect = items[i]->effect;
                std::cout << " [Used] " << items[i]->getName() << std::endl;
                Item* temp = items[i];
                items.erase(items.begin() + i);
                delete temp;
                return effect;
            }
        }
        return 0;
    }
};

// === 3. ПЕРСОНАЖИ ===

class Character {
protected:
    std::string raceName;
    int hp, maxHp;
    int attack;
    int defense;
    int agility;
    
    // Новые характеристики по ТЗ
    int height; // см
    int weight; // кг

    int level;
    int currentXp;
    int xpToNextLevel;
    int skillPoints;

    Inventory inventory;
    std::vector<int> gearSlots; // 0: Weapon, 1: Armor

public:
    Character(std::string rName, int h, int atk, int def, int agi, int hght, int wght)
        : raceName(rName), hp(h), maxHp(h), attack(atk), defense(def), agility(agi), height(hght), weight(wght)
    {
        level = 1;
        gearSlots = { 0, 0 };
        currentXp = 0;
        xpToNextLevel = 100;
        skillPoints = 0;

        // ТЗ: Рост и вес могут влиять на характеристики.
        // Реализация: Если вес большой (>90), снижаем ловкость, но повышаем макс ХП (выносливость)
        if (weight > 90) {
            this->agility -= 2;
            this->maxHp += 10;
            this->hp += 10;
        }
        // Если маленький рост, бонус к уклонению
        if (height < 150) {
            this->agility += 2;
        }
    }

    virtual ~Character() {}
    
    std::string getRace() const { return raceName; }
    bool isAlive() const { return hp > 0; }
    int getHP() const { return hp; }
    int getMaxHP() const { return maxHp; }
    int getLevel() const { return level; }
    int getSkillPoints() const { return skillPoints; }

    int getRawTotalAttack() const { return attack + gearSlots[0]; }
    int getRawTotalDefense() const { return defense + gearSlots[1]; }

    void giveStarterLoot() {
        inventory.addItem(new Potion_Health());
    }

    void printStats() const {
        std::cout << "\n=== " << raceName << " Character Sheet ===" << std::endl;
        std::cout << " Lvl: " << level << " (" << currentXp << "/" << xpToNextLevel << " XP)" << std::endl;
        std::cout << " HP:  " << hp << "/" << maxHp << std::endl;
        std::cout << " Atk: " << attack << " (+" << gearSlots[0] << ")" << std::endl;
        std::cout << " Def: " << defense << " (+" << gearSlots[1] << ")" << std::endl;
        std::cout << " Agi: " << agility << std::endl;
        std::cout << " Phys: " << height << "cm / " << weight << "kg" << std::endl;
        std::cout << " SP:  " << skillPoints << std::endl;
        std::cout << "=============================" << std::endl;
    }

    void levelUp() {
        currentXp -= xpToNextLevel;
        level++;
        skillPoints += 3;
        xpToNextLevel = static_cast<int>(xpToNextLevel * 1.5);
        hp = maxHp; 
        std::cout << "\n* LEVEL UP! -> " << level << " *" << std::endl;
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

    void manageInventory() {
        inventory.showItems();
        std::cout << "Actions: (e)quip <num>, (u)se <num>, (d)rop <num>, (c)lose: ";
        char action;
        std::cin >> action;
        
        if (action == 'c') return;

        int idx;
        std::cin >> idx;

        if (action == 'e') equipItem(idx);
        else if (action == 'u') healPotion();
        else if (action == 'd') inventory.dropItem(idx);
    }
void equipItem(int inventoryIndex) {
        Item* item = inventory.getItem(inventoryIndex - 1);
        if (!item) return;

        if (item->type == "Attack") {
            gearSlots[0] = item->effect;
            std::cout << " >> Equipped Weapon: " << item->getName() << std::endl;
        }
        else if (item->type == "Armor") {
            gearSlots[1] = item->effect;
            std::cout << " >> Equipped Armor: " << item->getName() << std::endl;
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

    int calculateTotalDamage() const {
        int baseDmg = attack + gearSlots[0];
        // Влияние ловкости на крит
        int critChance = 5 + (agility / 3);
        if ((std::rand() % 100) < critChance) {
            std::cout << " [CRITICAL!] ";
            return static_cast<int>(baseDmg * 1.5);
        }
        return baseDmg;
    }

    void takeDamage(int incomingDamage, bool isDodging) {
        int dodgeChance = static_cast<int>(agility * 0.5);
        if (isDodging) {
            dodgeChance += 30;
            std::cout << " (Dodging...) ";
        }
        if (dodgeChance > 80) dodgeChance = 80;

        if ((std::rand() % 100) < dodgeChance) {
            std::cout << ">>> MISS! You dodged!" << std::endl;
            return;
        }

        int myDef = defense + gearSlots[1];
        int actualBlock = std::min(myDef, static_cast<int>(incomingDamage * 0.9));
        int finalDamage = std::max(1, incomingDamage - actualBlock);

        hp -= finalDamage;
        std::cout << ">>> Hit for " << finalDamage << " dmg! (Armor blocked " << actualBlock << ")" << std::endl;
    }

    void healPotion() {
        int healAmount = inventory.consumeItemByType("Magic");
        if (healAmount > 0) {
            hp += healAmount;
            if (hp > maxHp) hp = maxHp;
            std::cout << " >> HP Restored. " << hp << "/" << maxHp << std::endl;
        } else {
            std::cout << "No potions!" << std::endl;
        }
    }

    void restoreFull() {
        hp = maxHp;
        std::cout << "You are fully rested." << std::endl;
    }
};

// === 4. РАСЫ (с учетом ТЗ по характеристикам) ===

class Human : public Character {
public:
    Human() : Character(
        "Human",
        100 + (std::rand() % 20), // HP
        10 + (std::rand() % 5),   // Atk
        5 + (std::rand() % 3),    // Def
        10 + (std::rand() % 5),   // Agi
        170 + (std::rand() % 20), // Height (cm)
        70 + (std::rand() % 20)   // Weight (kg)
    ) { giveStarterLoot(); }
};

class Elf : public Character {
public:
    Elf() : Character(
        "Elf",
        80 + (std::rand() % 20),
        8 + (std::rand() % 5),
        3 + (std::rand() % 3),
        15 + (std::rand() % 10), // Высокая ловкость
        180 + (std::rand() % 15), // Высокий рост
        60 + (std::rand() % 10)   // Легкий вес
    ) { giveStarterLoot(); }
};

class Dwarf : public Character {
public:
    Dwarf() : Character(
        "Dwarf",
        120 + (std::rand() % 30), // Много HP
        12 + (std::rand() % 5),
        8 + (std::rand() % 5),    // Много защиты
        5 + (std::rand() % 5),    // Мало ловкости
        130 + (std::rand() % 10), // Низкий рост
        80 + (std::rand() % 20)   // Тяжелый вес
    ) { giveStarterLoot(); }
};

// === 5. ВРАГИ ===
class Enemy : public Character {
private:
    int xpReward;
public:
    // Враг скалируется от уровня игрока И номера этажа (ТЗ п.6)
    Enemy(Character* player, int floor) 
        : Character("Monster", 0,0,0,0,0,0) // Базовые нули, перезапишем
    {
        int pLvl = player->getLevel();
        
        // Сложность растет с этажом
        double difficultyMult = 1.0 + (floor * 0.2); 

        int playerDmg = std::max(5, player->getRawTotalAttack());
        
        this->maxHp = static_cast<int>((playerDmg * 3 * difficultyMult) + (std::rand() % 10));
        this->hp = this->maxHp;

        this->attack = static_cast<int>((player->getRawTotalDefense() + 5) * difficultyMult);
        this->defense = floor; 
        this->agility = 5 + floor;

        this->xpReward = static_cast<int>(25 * difficultyMult);
        this->raceName = "Dungeon Beast";
    }

    void attackPlayer(Character* target, bool dodging) {
        std::cout << "\n[ENEMY] Beast attacks!" << std::endl;
        int dmg = attack; // У монстров простая атака
        target->takeDamage(dmg, dodging);
    }

    int getXpReward() const { return xpReward; }
};

// === 6. СИСТЕМА БОЯ ===

void battle(Character* player, Enemy* enemy) {
    std::cout << "\n!!! COMBAT STARTED !!!" << std::endl;
    while (player->isAlive() && enemy->isAlive()) {
        std::cout << "\n[PLAYER HP]: " << player->getHP() << " | [ENEMY HP]: " << enemy->getHP() << std::endl;
        
        std::cout << "1.Attack 2.Potion 3.Dodge 4.Inventory\n> ";
        char choice;
        std::cin >> choice;
        bool dodging = false;

        switch (choice) {
            case '1': enemy->takeDamage(player->calculateTotalDamage(), false); break;
            case '2': player->healPotion(); break;
            case '3': dodging = true; break;
            case '4': player->manageInventory(); break;
            default: std::cout << "Missed turn!" << std::endl;
        }

        if (enemy->isAlive()) {
            enemy->attackPlayer(player, dodging);
        } else {
            std::cout << "\nVICTORY!" << std::endl;
            player->gainXp(enemy->getXpReward());
            
            // ТЗ п.8: Лут с врагов
            std::cout << "Enemy dropped loot!" << std::endl;
            player->pickUp(generateRandomLoot());
        }
    }
}

// === 7. КОМНАТЫ И ГЕНЕРАЦИЯ ===

class Room {
public:
    virtual ~Room() {}
    virtual std::string getType() const = 0;
    virtual void enter(Character* player, int floor) = 0;
};

class Room_Relax : public Room {
public:
    std::string getType() const override { return "Safe Campfire"; }
    void enter(Character* player, int floor) override {
        std::cout << "\nYou rest at the campfire." << std::endl;
        player->restoreFull();
        // ТЗ п.3.1: Распределение очков только в комнатах отдыха (или можно всегда, но тут атмосфернее)
        if (player->getSkillPoints() > 0) {
            std::cout << "You have skill points!" << std::endl;
            player->distributeSkillPoints();
        }
    }
};

class Room_Chest : public Room {
public:
    std::string getType() const override { return "Treasure Room"; }
    void enter(Character* player, int floor) override {
        std::cout << "You found a Chest!" << std::endl;
        player->pickUp(generateRandomLoot());
    }
};

class Room_Enemy : public Room {
public:
    std::string getType() const override { return "Monster Lair"; }
    void enter(Character* player, int floor) override {
        std::cout << "\n--- FLOOR " << floor << " ENCOUNTER ---" << std::endl;
        Enemy* enemy = new Enemy(player, floor);
        battle(player, enemy);
        delete enemy;
    }
};

Room* generateRandomRoom() {
    int r = std::rand() % 100;
    if (r < 25) return new Room_Relax();
    if (r < 50) return new Room_Chest();
    return new Room_Enemy();
}

// === 8. ГЛАВНОЕ МЕНЮ И ЦИКЛ ===

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));
std::cout << "=== TEXT RPG DUNGEON ===" << std::endl;
    
    // ТЗ п.2.1: Выбор расы
    Character* player = nullptr;
    while (player == nullptr) {
        std::cout << "Select Race:\n1. Human (Balanced)\n2. Elf (Agile, Frail)\n3. Dwarf (Tough, Slow)\n> ";
        int choice;
        std::cin >> choice;
        if (choice == 1) player = new Human();
        else if (choice == 2) player = new Elf();
        else if (choice == 3) player = new Dwarf();
        else std::cout << "Invalid choice." << std::endl;
    }

    std::cout << "\nCharacter Created!" << std::endl;
    player->printStats();

    Room* currentRoom = new Room_Relax();
    int roomsCleared = 0;
    int currentFloor = 1;

    // ТЗ п.5.1: Игровой цикл
    while (player->isAlive()) {
        currentRoom->enter(player, currentFloor);

        if (!player->isAlive()) break;

        roomsCleared++;
        // ТЗ п.6: Каждые N (5) комнат - новый этаж
        if (roomsCleared % 5 == 0) {
            currentFloor++;
            std::cout << "\n>>> DESCENDING TO FLOOR " << currentFloor << " (Enemies are stronger!) <<<" << std::endl;
        }

        Room* leftRoom = generateRandomRoom();
        Room* rightRoom = generateRandomRoom();

        // ТЗ п.5.3: Видимость (рандом)
        bool canSee = (std::rand() % 2) == 0;

        std::cout << "\n--- Fork in the path ---" << std::endl;
        if (canSee) {
            std::cout << "[Light] You see: (L) " << leftRoom->getType() << " | (R) " << rightRoom->getType() << std::endl;
        } else {
            std::cout << "[Darkness] You cannot see what lies ahead." << std::endl;
        }

        char choice;
        while (true) {
            std::cout << "Go (l)eft, (r)ight, (i)nventory, (s)tats: ";
            std::cin >> choice;
            if (choice == 'l' || choice == 'r') break;
            if (choice == 'i') player->manageInventory();
            if (choice == 's') player->printStats();
        }

        delete currentRoom;

        if (choice == 'l') {
            currentRoom = leftRoom;
            delete rightRoom;
        } else {
            currentRoom = rightRoom;
            delete leftRoom;
        }
    }

    std::cout << "\nGAME OVER. You reached Floor " << currentFloor << "." << std::endl;
    delete player;
    delete currentRoom;

    return 0;
}