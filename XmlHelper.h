#pragma once
#include "Common.h"


class XmlHelper
{
	
};



std::list<std::pair<int,int>> LCSTrace( const std::vector<BiasArray>& VList, int k, int finalD);

size_t StringDistance(const std::string &s1, const std::string &s2);
size_t StringDistBasedLine(const std::string &s1, const std::string &s2);
size_t StringDistSimplify(const std::string &s1, const std::string &s2);
size_t StringDistSift4(const std::string &s1, const std::string &s2, int maxOffset = 5);

int DiffLines( const std::vector<std::string>& nodeLVec, const std::vector<std::string>& nodeRVec);
int CalcStringDist( const std::list<std::pair<int,int>>& trace,const std::vector<std::string>& nodeLVec, const std::vector<std::string>& nodeRVec);

char* CopyString(const char *begin, char* out);
int GetEnterCount(const char *begin);

class Profiler
{
public:
	Profiler();
	~Profiler();
	void Start();
	void Stop();

private:
	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER t1, t2;           // ticks
	double elapsedTime;
};
