#include <gtest/gtest.h>
#include <iostream>
#include <string>

class WarriorStrategy {
public:
    void execute() const {
        std::cout << "Warrior attacks with sword!" << std::endl;
    }
    
    std::string getType() const {
        return "Warrior";
    }
};

class MageStrategy {
public:
    void execute() const {
        std::cout << "Mage casts fireball!" << std::endl;
    }
    
    std::string getType() const {
        return "Mage";
    }
};

class ArcherStrategy {
public:
    void execute() const {
        std::cout << "Archer shoots arrow!" << std::endl;
    }
    
    std::string getType() const {
        return "Archer";
    }
};

template<typename T>
class Entity : private T {
public:
    using T::T;
    
    void performAction() const {
        T::execute();
    }
    
    std::string getType() const {
        return T::getType();
    }
};

class CustomWarrior {
public:
    CustomWarrior(const std::string& name, int strength) 
        : name_(name), strength_(strength) {}
    
    void execute() const {
        std::cout << name_ << " attacks with strength " << strength_ << "!" << std::endl;
    }
    
    std::string getType() const {
        return "CustomWarrior";
    }
    
private:
    std::string name_;
    int strength_;
};

class HealerStrategy {
public:
    HealerStrategy(int healingPower) : healingPower_(healingPower) {}
    
    void execute() const {
        std::cout << "Healer heals for " << healingPower_ << " HP!" << std::endl;
    }
    
    std::string getType() const {
        return "Healer";
    }
    
private:
    int healingPower_;
};

TEST(StaticStrategyPattern, BasicStrategies) {
    Entity<WarriorStrategy> warrior;
    Entity<MageStrategy> mage;
    Entity<ArcherStrategy> archer;
    
    EXPECT_EQ("Warrior", warrior.getType());
    EXPECT_EQ("Mage", mage.getType());
    EXPECT_EQ("Archer", archer.getType());
    
    testing::internal::CaptureStdout();
    warrior.performAction();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Warrior attacks with sword!") != std::string::npos);
    
    testing::internal::CaptureStdout();
    mage.performAction();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Mage casts fireball!") != std::string::npos);
    
    testing::internal::CaptureStdout();
    archer.performAction();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Archer shoots arrow!") != std::string::npos);
}

TEST(StaticStrategyPattern, ParameterizedStrategies) {
    Entity<CustomWarrior> customWarrior("Conan", 15);
    Entity<HealerStrategy> healer(50);
    
    EXPECT_EQ("CustomWarrior", customWarrior.getType());
    EXPECT_EQ("Healer", healer.getType());
    
    testing::internal::CaptureStdout();
    customWarrior.performAction();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Conan attacks with strength 15!") != std::string::npos);
    
    testing::internal::CaptureStdout();
    healer.performAction();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Healer heals for 50 HP!") != std::string::npos);
}
