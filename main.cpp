#include "universal_container.hpp"
class Test {
private:
    int t_ = 0;
    std::string str_;
public:
    Test(int tt, std::string s) :t_(tt), str_(s) {
    }
    void test() {
        std::cout << "test:" << t_ << ":" << str_ << std::endl;
    }
};
class Tool {
public:
    virtual void work() = 0;
};
class wrench : public Tool {
public:
    virtual void work() override {
        std::cout << "wrench!" << std::endl;
    }
};
class hammer : public Tool {
public:
    virtual void work() override {
        std::cout << "hammer!" << std::endl;
    }
};
class worker {
private:
    Tool& tool_;
public:
    worker(Tool& tool) : tool_(tool) {}
    void work() {
        tool_.work();
    }
};
int main() {
    universal_container uc;
    // register a object
    std::string key = "test";
    uc.register_key<Test, int, std::string>(key, 12, "hello World!");
    uc.resolve_key<Test>(key)->test();
    uc.release_key<Test>(key);
    // register a string
    key = "string";
    uc.register_key<std::string, std::string>(key, "hello World!");
    std::cout << *(uc.resolve_key<std::string>(key)) << std::endl;
    uc.release_key<std::string>(key);
    // register mutiple objects
    key = "wrench";
    uc.register_key<wrench>(key);
    key = "hammer";
    uc.register_key<hammer>(key);

    key = "wrench";
    worker worker1(*(uc.resolve_key<wrench>(key)));
    worker1.work();

    key = "hammer";
    worker worker2(*(uc.resolve_key<hammer>(key)));
    worker2.work();

    key = "wrench";
    uc.release_key<wrench>(key);
    key = "hammer";
    uc.release_key<hammer>(key);
    
    return 0;
}