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
      for (int i; i<tableSize; i++) {
        table[i] = ZTableEntry();
      }
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
      if (entry.zvalue==zvalue) {
        //checking depth
        if (depth>entry.depth && depth>0) {
          if (logging) log<<logtab<<"entry depth of "<<entry.depth<<" is less than depth: "<<depth<<" not using entry"<<endl;
          shouldReturn=false;
          return 0;
        }

        //exact value
        if (entry.nodetype==0) {
          shouldReturn=true;
          if (logging) log<<logtab<<"found score of "<<entry.score<<" with node type 0 and depth: "<<entry.depth;
          if (entry.score>=beta) {
            if (logging) log<<". score is greater than beta, returning beta of "<<beta<<endl;
            return beta;
            };
          if (entry.score<=alpha) {
            if (logging) log<<". score is less than alpha, returning alpha of "<<alpha<<endl;
            return alpha;
          }
          if (logging) log<<". returning score"<<endl;
          return entry.score;
        }

        //lower bound
        if (entry.nodetype==-1) {
          if (logging) log<<logtab<<"found score of "<<entry.score<<" with node type -1 (lower bound) and depth: "<<entry.depth;
          if (entry.score>=beta) {
            if (logging) log<<". score is greater than beta, returning beta of "<<beta<<endl;
            shouldReturn=true;
            return beta;
          }
          if (entry.score>alpha){
            if (logging) log<<". score is greater than alpha ("<<alpha<<"), changing alpha to score"<<endl;
            alpha = entry.score;
          }
        }

        //upper bound
        if (entry.nodetype==1) {
          if (logging) log<<logtab<<"found score of "<<entry.score<<" with node type -1 (lower bound) and depth: "<<entry.depth;
          if (entry.score<=alpha) {
            if (logging) log<<". score is less than alpha, returning alpha of "<<alpha<<endl;
            shouldReturn=true;
            return alpha;
          }

          if (entry.score>beta) {
            if (logging) log<<". score is greater than beta ("<<beta<<"), changing beta to score"<<endl;
            beta = entry.score;
          }
        }
      } else {
        if (logging) log<<logtab<<"did not find corresponding entry"<<endl;
      }
    };

    void setValue(U64 zvaluei,int depthi,int scorei,int nodetypei) {
      
      table[zvaluei%tableSize] = ZTableEntry(zvaluei, depthi, scorei, nodetypei);

      // ZTableEntry entry = getEntry(zvaluei);
      // if (depthi>=entry.depth || entry.zvalue == 0) {
      //   table[zvaluei%tableSize].update(zvaluei, depthi, scorei, nodetypei);
      // }
    };

};