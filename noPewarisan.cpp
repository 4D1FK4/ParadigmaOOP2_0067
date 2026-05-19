#include <iostream>
using namespace std;

class baseClass final{
public:
    virtual void perkenalan() {
        cout << "Hallo saya Function dari base class";
    }
};
class baseClass1 {
public:
    virtual void perkenalan() {
        cout << "Hallo saya dari Function dari base class";
    }
};