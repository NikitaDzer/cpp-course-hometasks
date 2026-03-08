# Exceptions analysis

### Task
The task is to analyze possible exceptions in next function:

```c++
void test(const Person &person) {
    std::cout << "test : " << person.name() << '\n';
    if (person.grade() == 10 || person.salary() > 1'000'000) {
        save(Status::success, person.id());
    } else {
        save(Status::failure, person.id());
    }
}
```

### Answer

* `person.name()`;
* `person.grade()`;
* `person.salary()`;
* `person.id()`;
* `save(Status::success, person.id())`;
* `save(Status::failure, person.id())`;
* `operator<<`;