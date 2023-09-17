#ifndef EXTENSIONS_H
#define EXTENSIONS_H
#include <string> 
#include <vector> 

class Extensions {
    public:
    vector<string> Split(string, const char);
    bool TryParseCharToInt(char);
    int CharToInt(char);
};

#endif