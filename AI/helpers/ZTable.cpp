#include "ZTableEntry.cpp"
#include <iostream>
#include <fstream>

class ZTable {
  private:
    int tableSize;
    ZTableEntry table[1000000];
  
  public:
    ZTable() {
        tableSize = 1000000;
        for (int i = 0; i<tableSize; i++) table[i] = ZTableEntry();
    }

    ZTableEntry GetEntry(U64 zvalue) { 
        return table[zvalue%tableSize];
    };

    int GetValue(int alpha, int beta, bool &shouldReturn, int depth, U64 zvalue, ofstream &log, string logtab, bool logging) {
        ZTableEntry entry = table[zvalue%tableSize];
        if (logging) {
            log<<logtab<<"getting value for zvalue "<<zvalue<<" which is entry # "<<zvalue%tableSize<<endl;
            logtab += "\t";
        }
        shouldReturn=false;

        if(!entry.isEqualToZvalue(zvalue)) {
            if (logging) log<<logtab<<"did not find corresponding entry"<<endl;
            return 0;
        }

        //checking depth
        if (entry.isDepthLessThan(depth) && depth>0) {
            if (logging) log<<logtab<<"entry depth of "<<entry.GetDepth()<<" is less than depth: "<<depth<<" not using entry"<<endl;
            shouldReturn=false;
            return 0;
        }

        //exact value
        if (entry.isNodeTypeExactValue()) {
            shouldReturn=true;
            if (logging) log<<logtab<<"found score of "<<entry.GetScore()<<" with node type 0 and depth: "<<entry.GetDepth();
            if (entry.isScoreGreaterThanOrEqual(beta)) { 
                if (logging) log<<". score is greater than beta, returning beta of "<<beta<<endl;
                return beta;
            }
            if (entry.isScoreLessThanOrEqual(alpha)) { 
                if (logging) log<<". score is less than alpha, returning alpha of "<<alpha<<endl;
                return alpha;
            }
            if (logging) log<<". returning score"<<endl;
            return entry.GetScore();
        }

        //lower bound
        if (entry.isEqualToNodeTypeLowerBound()) {
            if (logging) log<<logtab<<"found score of "<<entry.GetScore()<<" with node type -1 (lower bound) and depth: "<<entry.GetDepth();
            if (entry.isScoreGreaterThanOrEqual(beta)) {
                if (logging) log<<". score is greater than beta, returning beta of "<<beta<<endl;
                shouldReturn=true;
                return beta;
            }
            if (entry.isScoreGreaterThan(alpha)){
                if (logging) log<<". score is greater than alpha ("<<alpha<<"), changing alpha to score"<<endl;
                alpha = entry.GetScore();
            }
        }

        //upper bound
        if (entry.isEqualToNodeTypeUpperBound()) {
            if (logging) log<<logtab<<"found score of "<<entry.GetScore()<<" with node type -1 (lower bound) and depth: "<<entry.GetDepth();
            if (entry.isScoreLessThanOrEqual(alpha)) {
                if (logging) log<<". score is less than alpha, returning alpha of "<<alpha<<endl;
                shouldReturn=true;
                return alpha;
            }

            if (entry.isScoreGreaterThan(beta)) {
                if (logging) log<<". score is greater than beta ("<<beta<<"), changing beta to score"<<endl;
                beta = entry.GetScore();
            }
        }
    };

    void SetValue(U64 zvaluei, int depthi, int scorei, int nodetypei) { table[zvaluei%tableSize] = ZTableEntry(zvaluei, depthi, scorei, nodetypei); };

};