#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

// ============================================
// Template Method Pattern (Behavioral Pattern)
// ============================================

class GameObject {
public:
    virtual ~GameObject() = default;
    
    void update() {
        preUpdate();
        doUpdate();
        postUpdate();
    }
    
    virtual void render() const = 0;
    virtual std::string getType() const = 0;
    
protected:
    virtual void preUpdate() {
    }
    
    virtual void doUpdate() = 0;
    
    virtual void postUpdate() {
    }
};

// ============================================
// Builder Pattern (Creational Pattern)
// ============================================

class Unit : public GameObject {
public:
    class Builder {
    private:
        std::string type = "Warrior";
        int health = 100;
        int damage = 10;
        int defense = 5;
        std::string faction = "Neutral";
        
    public:
        Builder& setType(const std::string& t) {
            type = t;
            return *this;
        }
        
        Builder& setHealth(int h) {
            health = h;
            return *this;
        }
        
        Builder& setDamage(int d) {
            damage = d;
            return *this;
        }
        
        Builder& setDefense(int df) {
            defense = df;
            return *this;
        }
        
        Builder& setFaction(const std::string& f) {
            faction = f;
            return *this;
        }
        
        std::unique_ptr<Unit> build() {
            return std::unique_ptr<Unit>(new Unit(type, health, damage, defense, faction));
        }
    };
    
    void render() const override {
        std::cout << "[" << faction << "] " << type << " (HP: " << health << ", DMG: " 
                  << damage << ", DEF: " << defense << ")" << std::endl;
    }
    
    std::string getType() const override {
        return type;
    }
    
    std::string getFaction() const {
        return faction;
    }
    
    int getHealth() const {
        return health;
    }
    
    int getDamage() const {
        return damage;
    }
    
    int getDefense() const {
        return defense;
    }
    
protected:
    void doUpdate() override {
        if (health < maxHealth) {
            health = std::min(health + 1, maxHealth);
        }
    }
    
private:
    Unit(const std::string& t, int h, int d, int df, const std::string& f)
        : type(t), health(h), maxHealth(h), damage(d), defense(df), faction(f) {}
    
    std::string type;
    int health;
    int maxHealth;
    int damage;
    int defense;
    std::string faction;
};

class Building : public GameObject {
public:
    class Builder {
    private:
        std::string type = "Barracks";
        int health = 500;
        int capacity = 10;
        std::string faction = "Neutral";
        
    public:
        Builder& setType(const std::string& t) {
            type = t;
            return *this;
        }
        
        Builder& setHealth(int h) {
            health = h;
            return *this;
        }
        
        Builder& setCapacity(int c) {
            capacity = c;
            return *this;
        }
        
        Builder& setFaction(const std::string& f) {
            faction = f;
            return *this;
        }
        
        std::unique_ptr<Building> build() {
            return std::unique_ptr<Building>(new Building(type, health, capacity, faction));
        }
    };
    
    void render() const override {
        std::cout << "[" << faction << "] " << type << " (HP: " << health 
                  << ", Capacity: " << capacity << ")" << std::endl;
    }
    
    std::string getType() const override {
        return type;
    }

    std::string getFaction() const {
        return faction;
    }
    
    int getHealth() const {
        return health;
    }
    
protected:
    void doUpdate() override {
        if (health < maxHealth) {
            health = std::min(health + 2, maxHealth);
        }
    }
    
private:
    Building(const std::string& t, int h, int c, const std::string& f)
        : type(t), health(h), maxHealth(h), capacity(c), faction(f) {}
    
    std::string type;
    int health;
    int maxHealth;
    int capacity;
    std::string faction;
};

// ============================================
// Composite Pattern (Structural Pattern)
// ============================================

class CompositeGameObject : public GameObject {
public:
    void add(std::unique_ptr<GameObject> obj) {
        objects.push_back(std::move(obj));
    }
    
    void remove(GameObject* obj) {
        objects.erase(
            std::remove_if(objects.begin(), objects.end(),
                [obj](const std::unique_ptr<GameObject>& ptr) {
                    return ptr.get() == obj;
                }),
            objects.end()
        );
    }
    
    void render() const override {
        std::cout << "=== Group contains " << objects.size() << " objects ===" << std::endl;
        for (const auto& obj : objects) {
            obj->render();
        }
        std::cout << "=== End of group ===" << std::endl;
    }
    
    std::string getType() const override {
        return "Composite";
    }
    
    size_t size() const {
        return objects.size();
    }
    
protected:
    void preUpdate() override {
        std::cout << "Updating composite object with " << objects.size() << " elements" << std::endl;
    }
    
    void doUpdate() override {
        for (const auto& obj : objects) {
            obj->update();
        }
    }
    
    void postUpdate() override {
    }
    
private:
    std::vector<std::unique_ptr<GameObject>> objects;
};

TEST(GameFramework, BuilderPatternUnitCreation) {
    Unit::Builder unitBuilder;
    auto warrior = unitBuilder
        .setType("Elf Archer")
        .setHealth(80)
        .setDamage(15)
        .setDefense(3)
        .setFaction("Elves")
        .build();
    
    EXPECT_EQ("Elf Archer", warrior->getType());
    EXPECT_EQ("Elves", warrior->getFaction());
    EXPECT_EQ(80, warrior->getHealth());
    EXPECT_EQ(15, warrior->getDamage());
    EXPECT_EQ(3, warrior->getDefense());
    
    auto defaultWarrior = Unit::Builder().build();
    EXPECT_EQ("Warrior", defaultWarrior->getType());
    EXPECT_EQ("Neutral", defaultWarrior->getFaction());
    EXPECT_EQ(100, defaultWarrior->getHealth());
}

TEST(GameFramework, BuilderPatternBuildingCreation) {
    Building::Builder buildingBuilder;
    auto barracks = buildingBuilder
        .setType("Advanced Barracks")
        .setHealth(800)
        .setCapacity(20)
        .setFaction("Humans")
        .build();
    
    EXPECT_EQ("Advanced Barracks", barracks->getType());
    EXPECT_EQ(800, barracks->getHealth());
    
    auto defaultBuilding = Building::Builder().build();
    EXPECT_EQ("Barracks", defaultBuilding->getType());
    EXPECT_EQ(500, defaultBuilding->getHealth());
}

TEST(GameFramework, CompositePattern) {
    auto group = std::make_unique<CompositeGameObject>();
    
    group->add(Unit::Builder().setType("Warrior").setFaction("Humans").build());
    group->add(Unit::Builder().setType("Archer").setFaction("Humans").build());
    group->add(Building::Builder().setType("Tower").setFaction("Humans").build());
    
    EXPECT_EQ(3, group->size());
    group->update();
    
    auto subGroup = std::make_unique<CompositeGameObject>();
    subGroup->add(Unit::Builder().setType("Knight").setFaction("Humans").build());
    subGroup->add(Unit::Builder().setType("Mage").setFaction("Humans").build());
    
    group->add(std::move(subGroup));
    EXPECT_EQ(4, group->size());
}
