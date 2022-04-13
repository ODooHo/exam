#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
using namespace std;


void writeGPIO(string path, string filename, string value){
    fstream fs;
    fs.open((path + filename).c_str(),fstream::out);
    fs << value;
    fs.close();
}

int main(int argc, char* argc[]){
    if(argc!=2){
        cout << "Usage is makeLED and one of : "<<endl;
    }
}