#include <bits/stdc++.h>
using namespace std;
string GetStdoutFromCommand(string cmd) {
    string data;
   
    const int max_buffer = 1024;
    char buffer[max_buffer];
    FILE* stream;
    cmd.append(" 2>&1"); 
    stream = popen(cmd.c_str(), "r");

   // FILE *stream  = 
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return data;
}
int main(){
    string ss; getline(cin,ss);
    string str = GetStdoutFromCommand(ss);
    cout << str.find_first_of("\n") <<' ' << str.find_last_of("\n");
    //cout << oneline << endl;
}
