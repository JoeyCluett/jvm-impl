#include <iostream>
#include <ClassLoader.h>

using namespace std;

int main(int argc, char* argv[]) {

    if(argc < 2) {
        cout << "Usage:\n    " << argv[0] << " <class file>\n";
        return 1;
    }

    ClassFile cf(argv[1]);
    cf.load_class();

}
