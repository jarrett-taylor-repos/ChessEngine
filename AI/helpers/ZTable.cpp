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
        for (int i; i<tableSize; i++) table[i] = ZTableEntry();
    }

    ZTableEntry getEntry(U64 zvalue) { //TODO: change to return pointer to stuff?
        return table[zvalue%tableSize];
    };

    int getValue(int &alpha, int &beta, bool &shouldReturn, int depth, U64 &zvalue, ofstream &log, string logtab, bool logging) {
        ZTableEntry entry = table[zvalue%tableSize];
        if (logging) {
            log<<logtab<<"getting value for zvalue "<<zvalue<<" which is entry # "<<zvalue%tableSize<<endl;
            logtab += "\t";
        }
        shouldReturn=false;
        

        if (entry.TestZValue(zvalue)) {
            //checking depth
            if (entry.isDepthLessThan(depth) && depth) {
                if (logging) log<<logtab<<"entry depth of "<<entry.GetDepth()<<" is less than depth: "<<depth<<" not using entry"<<endl;
                shouldReturn=false;
                return 0;
            }

        
            //exact value
            if (entry.TestNodeValue(0)) {
                shouldReturn=true;
                if (logging) log<<logtab<<"found score of "<<entry.GetScore()<<" with node type 0 and depth: "<<entry.GetDepth();
                if (entry.isScoreGreaterThan(beta)) {
                    if (logging) log<<". score is greater than beta, returning beta of "<<beta<<endl;
                    return beta;
                    };
                if (entry.isScoreLessThanOrEqual(alpha)) {
                    if (logging) log<<". score is less than alpha, returning alpha of "<<alpha<<endl;
                    return alpha;
                }
                if (logging) log<<". returning score"<<endl;
                return entry.GetScore();
            }

            //lower bound
            if (entry.TestNodeValue(-1)) {
                if (logging) log<<logtab<<"found score of "<<entry.GetScore()<<" with node type -1 (lower bound) and depth: "<<entry.GetDepth();
                if (entry.isScoreGreaterThan(beta)) {
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
            if (entry.TestNodeValue(1)) {
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
        } else {
            if (logging) log<<logtab<<"did not find corresponding entry"<<endl;
        }
    };

    void setValue(U64 zvaluei,int depthi,int scorei,int nodetypei) { table[zvaluei%tableSize] = ZTableEntry(zvaluei, depthi, scorei, nodetypei); };

};