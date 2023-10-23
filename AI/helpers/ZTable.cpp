#include "ZTableEntry.cpp"

class ZTable {
  private:
    int tableSize = 10000;
    ZTableEntry table[10000];
  public:

    ZTable() {
    }

    ZTableEntry getEntry(unsigned long long zvalue) { //TODO: change to return pointer to stuff?
      return table[zvalue%tableSize];
    };

    int getValue(int &alpha, int &beta, bool &shouldReturn, int depth, unsigned long long zvalue, ofstream log, bool logging) {
      ZTableEntry entry = table[zvalue%tableSize];
      shouldReturn=false;
      if (entry.zvalue==zvalue) {
        //checking depth
        if (depth<entry.depth && depth>0) {
          shouldReturn=false;
          return 0;
        }

        //exact value
        if (entry.nodetype==0) {
          shouldReturn=true;
          if (entry.score>=beta) return beta;
          if (entry.score<=alpha) return alpha;
          return entry.score;
        }

        //lower bound
        if (entry.nodetype==-1) {
          if (entry.score>=beta) {
            shouldReturn=true;
            return beta;
          }
          if (entry.score>alpha){
            alpha = entry.score;
          }
        }

        //upper bound
        if (entry.nodetype==1) {
          if (entry.score<=alpha) {
            shouldReturn=true;
            return alpha;
          }

          if (entry.score>beta) {
            beta = entry.score;
          }
        }
      }
    };

    void setValue(unsigned long long zvaluei,int depthi,int scorei,int nodetypei) {
      ZTableEntry entry = getEntry(zvaluei);
      if (depthi>=entry.depth || entry.zvalue == 0) {
        table[zvaluei%tableSize].update(zvaluei, depthi, scorei, nodetypei);
      }
    };

};