#include <iostream>
#include <BinaryFileReader.h>
#include <ClassFile.h>

using namespace std;

int main(int argc, char* argv[]) {
    cout << "SE8 v2" << endl;

    if(argc != 2) {
        cout << "usage: " << argv[0] << " <input file>\n";
        return 0;
    }

    ClassFile toplevel;
    toplevel.init(argv[1]);

    return 0;
}
