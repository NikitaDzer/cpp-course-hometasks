#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <sstream>

class Entity {
public:
    virtual ~Entity() = default;
    virtual void operation() const = 0;
};

template<typename T>
class Decorator : public virtual Entity, private T {
public:
    using T::T;
    
    void operation() const override {
        std::cout << "Decorator: before operation" << std::endl;
        T::operation();
        std::cout << "Decorator: after operation" << std::endl;
    }
};

class ConcreteEntity : public virtual Entity {
public:
    ConcreteEntity(const std::string& name) : name_(name) {}
    
    void operation() const override {
        std::cout << "ConcreteEntity " << name_ << ": performing operation" << std::endl;
    }
    
private:
    std::string name_;
};

class AnotherEntity : public virtual Entity {
public:
    AnotherEntity(int value) : value_(value) {}
    
    void operation() const override {
        std::cout << "AnotherEntity with value " << value_ << ": performing operation" << std::endl;
    }
    
private:
    int value_;
};

TEST(DecoratorPattern, BasicDecoration) {
    testing::internal::CaptureStdout();
    
    Decorator<ConcreteEntity> decorated("Test");
    decorated.operation();
    
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Decorator: before operation") != std::string::npos);
    EXPECT_TRUE(output.find("ConcreteEntity Test: performing operation") != std::string::npos);
    EXPECT_TRUE(output.find("Decorator: after operation") != std::string::npos);
}

TEST(DecoratorPattern, DifferentEntityTypes) {
    testing::internal::CaptureStdout();
    
    Decorator<AnotherEntity> decorated(42);
    decorated.operation();
    
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Decorator: before operation") != std::string::npos);
    EXPECT_TRUE(output.find("AnotherEntity with value 42: performing operation") != std::string::npos);
    EXPECT_TRUE(output.find("Decorator: after operation") != std::string::npos);
}

TEST(DecoratorPattern, EntityInterface) {
    testing::internal::CaptureStdout();
    
    std::unique_ptr<Entity> entity = std::make_unique<Decorator<ConcreteEntity>>("InterfaceTest");
    entity->operation();
    
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Decorator: before operation") != std::string::npos);
    EXPECT_TRUE(output.find("ConcreteEntity InterfaceTest: performing operation") != std::string::npos);
    EXPECT_TRUE(output.find("Decorator: after operation") != std::string::npos);
}

TEST(DecoratorPattern, MultipleInheritanceCheck) {
    std::unique_ptr<Decorator<ConcreteEntity>> decorated = std::make_unique<Decorator<ConcreteEntity>>("Test");
    Entity* entity = decorated.get();
    
    testing::internal::CaptureStdout();
    entity->operation();
    
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Decorator: before operation") != std::string::npos);
    EXPECT_TRUE(output.find("ConcreteEntity Test: performing operation") != std::string::npos);
    EXPECT_TRUE(output.find("Decorator: after operation") != std::string::npos);
}
