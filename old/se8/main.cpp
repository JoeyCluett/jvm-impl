#include <iostream>
#include <cstdlib>
#include <ClassFile.h>
#include <jre/InstructionMnemonics.h>

/*

    For the purposes of this JVM implementation. specify the location of a 
    valid collection of Java class files. I extracted mine from 
    /usr/lib/jvm/default-java/lib/ct.sym

    The folder with the extracted libs is at 
    $HOME/jvmextractedlibs/META-INF/sym/rt.jar/

*/

using namespace std;

int main(int argc, char* argv[]) {

    if(argc < 2) {
        cout << "Usage:\n    " << argv[0] << " <class file>\n";
        return 1;
    }

    std::string HOME = getenv("HOME");
    cout << "HOME: " << HOME << endl;
    
    ClassFile cf;
    cf.load_file(argv[1]);
    //cout << cf << endl;

}
