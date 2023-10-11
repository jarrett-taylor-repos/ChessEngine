#include <iostream>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <bitset>
#include <iterator>
#include <map>
#include <sstream>
#include <vector>
#include <stdint.h>
using namespace std;
#include "..\Extensions\U64Extentions.h"
#include <ctype.h>
#include <stdio.h>
#include <unordered_map>
using namespace U64Extensions;


typedef unsigned long long U64;
#define C64(constantU64) constantU64##ULL

int main () {
    U64 test = 0;
    SetBit(test, 45);
    Print(test, "set bit U64");
    ResetBit(test, 45);
    Print(test, "reset bit U64");
}