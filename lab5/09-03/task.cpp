#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

void test_builder_pattern() {
    std::cout << "=== Builder Pattern ===" << std::endl;
    
    struct Entity {
        int x = 0, y = 0;
    };
    
    class Builder {
    public:
        virtual ~Builder() = default;
        
        std::unique_ptr<Entity> make_entity() {
            m_entity = std::make_unique<Entity>();
            set_x();
            set_y();
            return std::move(m_entity);
        }
        
        virtual void set_x() const = 0;
        virtual void set_y() const = 0;
        
    protected:
        std::unique_ptr<Entity> m_entity;
    };
    
    class Builder_Client : public Builder {
    public:
        void set_x() const override { m_entity->x = 1; }
        void set_y() const override { m_entity->y = 1; }
    };
    
    class Builder_Server : public Builder {
    public:
        void set_x() const override { m_entity->x = 2; }
        void set_y() const override { m_entity->y = 2; }
    };
    
    std::unique_ptr<Builder> builder = std::make_unique<Builder_Client>();
    std::unique_ptr<Entity> entity = builder->make_entity();
    assert(entity->x == 1);
    assert(entity->y == 1);
    std::cout << "Builder pattern test passed" << std::endl;
}

void test_abstract_factory() {
    std::cout << "=== Abstract Factory Pattern ===" << std::endl;
    
    class Entity {
    public:
        virtual ~Entity() = default;
        virtual int get_id() const = 0;
    };
    
    class Client : public Entity {
    public:
        int get_id() const override { return 1; }
    };
    
    class Server : public Entity {
    public:
        int get_id() const override { return 2; }
    };
    
    class Factory {
    public:
        virtual ~Factory() = default;
        virtual std::unique_ptr<Entity> make_entity() const = 0;
    };
    
    class Factory_Client : public Factory {
    public:
        std::unique_ptr<Entity> make_entity() const override {
            return std::make_unique<Client>();
        }
    };
    
    class Factory_Server : public Factory {
    public:
        std::unique_ptr<Entity> make_entity() const override {
            return std::make_unique<Server>();
        }
    };
    
    std::unique_ptr<Factory> factory = std::make_unique<Factory_Client>();
    std::unique_ptr<Entity> entity = factory->make_entity();
    assert(entity->get_id() == 1);
    std::cout << "Abstract factory test passed" << std::endl;
}

void test_prototype_pattern() {
    std::cout << "=== Prototype Pattern ===" << std::endl;
    
    class Entity {
    public:
        virtual ~Entity() = default;
        virtual std::unique_ptr<Entity> copy() const = 0;
        virtual int get_id() const = 0;
    };
    
    class Client : public Entity {
    public:
        std::unique_ptr<Entity> copy() const override {
            return std::make_unique<Client>(*this);
        }
        int get_id() const override { return 1; }
    };
    
    class Server : public Entity {
    public:
        std::unique_ptr<Entity> copy() const override {
            return std::make_unique<Server>(*this);
        }
        int get_id() const override { return 2; }
    };
    
    class Prototype {
    public:
        Prototype() {
            m_entities.push_back(std::make_unique<Client>());
            m_entities.push_back(std::make_unique<Server>());
        }
        
        std::unique_ptr<Entity> make_client() { 
            return m_entities.at(0)->copy(); 
        }
        std::unique_ptr<Entity> make_server() { 
            return m_entities.at(1)->copy(); 
        }
        
    private:
        std::vector<std::unique_ptr<Entity>> m_entities;
    };
    
    Prototype prototype;
    std::unique_ptr<Entity> client = prototype.make_client();
    assert(client->get_id() == 1);
    std::cout << "Prototype pattern test passed" << std::endl;
}

void test_composite_pattern() {
    std::cout << "=== Composite Pattern ===" << std::endl;
    
    class Entity {
    public:
        virtual ~Entity() = default;
        virtual int test() const = 0;
    };
    
    class Client : public Entity {
    public:
        int test() const override { return 1; }
    };
    
    class Server : public Entity {
    public:
        int test() const override { return 2; }
    };
    
    class Composite : public Entity {
    public:
        void add(std::unique_ptr<Entity> entity) {
            m_entities.push_back(std::move(entity));
        }
        
        int test() const override {
            int x = 0;
            for (const auto& entity : m_entities) {
                if (entity) {
                    x += entity->test();
                }
            }
            return x;
        }
        
    private:
        std::vector<std::unique_ptr<Entity>> m_entities;
    };
    
    auto make_composite = [](std::size_t size_1, std::size_t size_2) {
        auto composite = std::make_unique<Composite>();
        for (std::size_t i = 0; i < size_1; ++i) {
            composite->add(std::make_unique<Client>());
        }
        for (std::size_t i = 0; i < size_2; ++i) {
            composite->add(std::make_unique<Server>());
        }
        return composite;
    };
    
    auto composite = std::make_unique<Composite>();
    for (std::size_t i = 0; i < 5; ++i) {
        composite->add(make_composite(1, 1));
    }
    
    assert(composite->test() == 15);
    std::cout << "Composite pattern test passed" << std::endl;
}

void test_observer_pattern() {
    std::cout << "=== Observer Pattern ===" << std::endl;
    
    class Observer {
    public:
        virtual ~Observer() = default;
        virtual void test(int x) const = 0;
    };
    
    class Entity {
    public:
        void add(std::shared_ptr<Observer> observer) {
            m_observers.push_back(observer);
        }
        
        void set(int x) {
            m_x = x;
            notify_all();
        }
        
        void notify_all() const {
            for (const auto& observer : m_observers) {
                if (observer) {
                    observer->test(m_x);
                }
            }
        }
        
    private:
        int m_x = 0;
        std::vector<std::shared_ptr<Observer>> m_observers;
    };
    
    class Client : public Observer {
    public:
        void test(int x) const override {
            std::cout << "Client::test : x = " << x << std::endl;
        }
    };
    
    class Server : public Observer {
    public:
        void test(int x) const override {
            std::cout << "Server::test : x = " << x << std::endl;
        }
    };
    
    Entity entity;
    entity.add(std::make_shared<Client>());
    entity.add(std::make_shared<Server>());
    
    for (int i = 0; i < 2; ++i) {
        entity.set(i + 1);
    }
    std::cout << "Observer pattern test passed" << std::endl;
}

int main() {
    test_builder_pattern();
    test_abstract_factory();
    test_prototype_pattern();
    test_composite_pattern();
    test_observer_pattern();
    
    std::cout << "\n=== All tests passed ===" << std::endl;
    
    return 0;
}
