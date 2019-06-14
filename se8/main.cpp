#include <iostream>
#include <ClassFile.h>

using namespace std;

int main(int argc, char* argv[]) {

    if(argc < 2) {
        cout << "Usage:\n    " << argv[0] << " <class file>\n";
        return 1;
    }

    ClassFile cf;
    cf.load_file(argv[1]);
    cout << cf << endl;

}
