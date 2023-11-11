class ZTableEntry {
  public:
  U64 zvalue;
  int depth;
  int score;
  int nodetype;

  ZTableEntry(){
    zvalue=0;
    depth=-1;
  };

  ZTableEntry(int zvaluei,int depthi,int scorei,int nodetypei) {
    zvalue = zvaluei;
    depth = depthi;
    score = scorei;
    nodetype = nodetypei;
  };

  void update(int depthi,int scorei,int nodetypei) {
    depth = depthi;
    score = scorei;
    nodetype = nodetypei;
  };

  void update(int zvaluei,int depthi,int scorei,int nodetypei) {
    zvalue = zvaluei;
    depth = depthi;
    score = scorei;
    nodetype = nodetypei;
  };
};