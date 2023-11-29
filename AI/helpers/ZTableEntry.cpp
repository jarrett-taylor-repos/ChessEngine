typedef unsigned long long U64;
enum { ExactValue = 0, UpperBound = 1, LowerBound = -1 };

class ZTableEntry {
	U64 zvalue;
	int depth;
	int score;
	int nodetype;

	public:
	ZTableEntry(){
		zvalue=0;
		depth=-1;
	};

	ZTableEntry(U64 zvaluei,int depthi,int scorei,int nodetypei) {
		zvalue = zvaluei;
		depth = depthi;
		score = scorei;
		nodetype = nodetypei;
	};

	ZTableEntry(const ZTableEntry& other) {
		this->zvalue = other.zvalue;
		this->depth = other.depth;
		this->score = other.score;
		this->nodetype = other.nodetype;
	};

	ZTableEntry& operator=(const ZTableEntry& other) {
		this->zvalue = other.zvalue;
		this->depth = other.depth;
		this->score = other.score;
		this->nodetype = other.nodetype;
		return *this;
	};

	void Update(int depthi,int scorei,int nodetypei) {
		depth = depthi;
		score = scorei;
		nodetype = nodetypei;
	};

	void Update(int zvaluei,int depthi,int scorei,int nodetypei) {
		zvalue = zvaluei;
		depth = depthi;
		score = scorei;
		nodetype = nodetypei;
	};

	U64 GetZvalue() { return zvalue; };
	int GetDepth() { return depth; };
	int GetScore() { return score; };
	int GetNodeType() { return nodetype; };

	bool isScoreGreaterThanOrEqual(int val) { return score>=val; };
	bool isScoreLessThanOrEqual(int val) { return score<=val; };
	bool isScoreGreaterThan(int val) { return score>val; };
	bool isScoreLessThan(int val) { return score<val; };

	bool isDepthLessThan(int val) { return depth<val; };

	bool isEqualToZvalue(U64 val) { return zvalue == val; };
	bool isEqualToNodeType(int val) { return nodetype == val; };	

	bool isNodeTypeExactValue() { return nodetype == ExactValue; };
	bool isNodeTypeLowerBound() { return nodetype == LowerBound; };
	bool isNodeTypeUpperBound() { return nodetype == UpperBound; };
};