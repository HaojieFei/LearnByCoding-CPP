//
//  test1.cpp
// 
//

#include <iostream>
#include "LeakDetector.hpp"

class Err {
public:
    Err(int n) {
        if(n == 0) throw 1000;
        data = new int[n];
    }
    ~Err() {
        delete[] data;
    }
private:
    int *data;
};

int main() {
    
    int *a = new int;
    int *b = new int[12];
    
    delete a;
    
    try {
        std::cout << sizeof(Err) << std::endl;
        Err* e = new Err(0);
        delete e;
    } catch (int &ex) {
        std::cout << "Exception " << ex << std::endl;
    };

    
    return 0;
    
}
