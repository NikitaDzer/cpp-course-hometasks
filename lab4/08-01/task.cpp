#include <iostream>
#include <cstring>

class Entity_v1 {
private:
    int value;
public:
    Entity_v1(int v) : value(v) {}
    void print() const {
        std::cout << "Entity_v1 value: " << value << std::endl;
    }
};

class Entity_v2 {
public:
    int value;
};

void fraudWithReinterpretCast(Entity_v1& obj, int newVal) {
    reinterpret_cast<Entity_v2&>(obj).value = newVal;
}

void fraudWithMemcpy(Entity_v1& obj, int newVal) {
    std::memcpy(&obj, &newVal, sizeof(int));
}

int main() {
    Entity_v1 obj(10);
    obj.print();

    fraudWithReinterpretCast(obj, 42);
    obj.print();

    fraudWithMemcpy(obj, 99);
    obj.print();

    return 0;
}