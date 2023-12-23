#include "Entry.cpp"
using namespace std;

const int BYTES = 1024 * 1024;

class TranspositionTable {
  private:
    int tableSize;
    Entry entries[];
  
  public:
    TranspositionTable() {
        tableSize = 64 * BYTES;
        for (int i = 0; i<tableSize; i++) entries[i] = Entry();
    }

    TranspositionTable(int sizeMB) {
        tableSize = sizeMB * BYTES;
        for (int i = 0; i<tableSize; i++) entries[i] = Entry();
    }

    ~TranspositionTable() {
        delete[] entries;
    };

    Entry GetEntry(U64 zvalue) { 
        return entries[zvalue%tableSize];
    };

    int GetValue(int alpha, int beta, bool &shouldReturn, int depth, U64 zvalue) {
        Entry entry = entries[zvalue%tableSize];
        shouldReturn=false;

        if(!entry.isValue(zvalue)) return 0;

        //checking depth
        if (entry.isDepthLessThan(depth) && depth>0) {
            shouldReturn=false;
            return 0;
        }

        //exact value
        if (entry.isNodeType(ExactValue)) {
            shouldReturn=true;
            if (entry.isScoreGreaterThanOrEqual(beta)) return beta;
            if (entry.isScoreLessThanOrEqual(alpha)) return alpha;

            return entry.GetScore();
        }

        //lower bound
        if (entry.isNodeType(LowerBound)) {
            if (entry.isScoreGreaterThanOrEqual(beta)) {
                shouldReturn=true;
                return beta;
            }

            if (entry.isScoreGreaterThan(alpha)) alpha = entry.GetScore();
        }

        //upper bound
        if (entry.isNodeType(UpperBound)) {
            if (entry.isScoreLessThanOrEqual(alpha)) {
                shouldReturn=true;
                return alpha;
            }

            if (entry.isScoreGreaterThan(beta)) beta = entry.GetScore();
        }
    };

    void SetValue(U64 zvaluei, int depthi, int scorei, int nodetypei) { 
        entries[zvaluei%tableSize] = Entry(zvaluei, depthi, scorei, nodetypei); 
    };

};