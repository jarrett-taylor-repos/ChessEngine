typedef unsigned long long U64;

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
	}

	ZTableEntry& operator=(const ZTableEntry& other) {
	this->zvalue = other.zvalue;
	this->depth = other.depth;
	this->score = other.score;
	this->nodetype = other.nodetype;
	return *this;
	}

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

	U64 GetZvalue() { return zvalue; };
	int GetDepth() { return depth; };
	int GetScore() { return score; };
	int GetNodeType() { return nodetype; };

	bool TestZValue(U64 val) { return val==zvalue; };
	bool TestNodeValue(int val) { return nodetype==val; };
};