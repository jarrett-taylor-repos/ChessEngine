typedef unsigned long long U64;
enum { ExactValue = 0, UpperBound = 1, LowerBound = -1, SearchFailed = -64 };

class Entry {
	U64 value;
	int depth;
	int score;
	int nodetype;

	public:
	Entry(){
		value=0;
		depth=-1;
	};

	Entry(U64 zvaluei,int depthi,int scorei,int nodetypei) {
		value = zvaluei;
		depth = depthi;
		score = scorei;
		nodetype = nodetypei;
	};

	Entry(const Entry& other) {
		this->value = other.value;
		this->depth = other.depth;
		this->score = other.score;
		this->nodetype = other.nodetype;
	};

	Entry& operator=(const Entry& other) {
		this->value = other.value;
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
		value = zvaluei;
		depth = depthi;
		score = scorei;
		nodetype = nodetypei;
	};

	U64 GetZvalue() { return value; };
	int GetDepth() { return depth; };
	int GetScore() { return score; };
	int GetNodeType() { return nodetype; };

	bool isScoreGreaterThanOrEqual(int val) { return score>=val; };
	bool isScoreLessThanOrEqual(int val) { return score<=val; };
	bool isScoreGreaterThan(int val) { return score>val; };
	bool isScoreLessThan(int val) { return score<val; };

	bool isDepthLessThan(int val) { return depth<val; };

	bool isValue(U64 val) { return value == val; };
	bool isNodeType(int val) { return nodetype == val; };	

	bool isNodeTypeExactValue() { return nodetype == ExactValue; };
	bool isNodeTypeLowerBound() { return nodetype == LowerBound; };
	bool isNodeTypeUpperBound() { return nodetype == UpperBound; };
};