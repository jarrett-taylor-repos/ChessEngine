#include <iostream> 
#include <string> 
#include <vector> 
#include <sstream> 
using namespace std;

vector<string> Split(string str, const char delim) {
    vector<string> out;
    stringstream ss(str); 

    string s; 
    while (getline(ss, s, delim)) { 
        out.push_back(s); 
    } 
    return out;
}

bool TryParseCharToInt(char ch) {
    bool result = false;
    try {
        string str(1, ch);
        stoi(str);
        return true;
    } catch (string ex) {

    }
    return result;
}

int CharToInt(char ch) {
    string str(1, ch);
    return stoi(str);
}