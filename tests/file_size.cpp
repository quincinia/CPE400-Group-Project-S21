// https://stackoverflow.com/questions/2409504/using-c-filestreams-fstream-how-can-you-determine-the-size-of-a-file

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
    long begin, end;
    ifstream myfile(argv[1]);
    begin = myfile.tellg();
    myfile.seekg(0, ios::end);
    end = myfile.tellg();
    myfile.close();
    cout << "size: " << (end - begin) << " bytes." << endl;
}