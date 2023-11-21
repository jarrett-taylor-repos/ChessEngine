#include "..\Constants\Constants.h"
using namespace Constants;

namespace CastlingExtensions {
    int SetCastlingRightsHelper(char ch) {
        switch (ch) {
            case 'K': return wk; 
            case 'Q': return wq;
            case 'k': return bk; 
            case 'q': return bq;
            default: return 0;
        }
    }

    int SetCastlingRights(string str) {
        int result = 0;
        if(str == "" || str == "-") return 0;
        for(int i = 0; i < str.size(); i++) {
            char ch = str[i];
            result |= SetCastlingRightsHelper(ch);
        }
        return result;
    }
    
    string CastlingRightsString(int castling) {
        if(castling == 0) return "-";

        string temp = "";
        if(castling & wk) temp += "K";
        if(castling & wq) temp += "Q";
        if(castling & bk) temp += "k";
        if(castling & bq) temp += "q";
        return temp;
    }
}