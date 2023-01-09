#pragma once
#include "Common.h"
#include "RapidXml\rapidxml.hpp"
#include "StringBuff.h"


class XmlHelper
{
	
};


std::list<std::pair<int,int>> LCSTrace( const std::vector<BiasArray>& VList, int k, int finalD);

size_t StringDistance(const std::string &s1, const std::string &s2);
size_t DataDistance(const std::vector<char>& s1, const std::vector<char>& s2);
size_t StringDistBasedLine(rapidxml::xml_node<>* s1, rapidxml::xml_node<>* s2);
size_t StringDistSimplify(const std::string &s1, const std::string &s2);
size_t StringDistSift4(const std::string &s1, const std::string &s2, int maxOffset = 5);

int DiffLines(std::vector<std::string_view> nodeLVec, std::vector<std::string_view> nodeRVec);
int CalcStringDist( const std::list<std::pair<int,int>>& trace,const std::vector<std::string_view>& nodeLVec, const std::vector<std::string_view>& nodeRVec);

char* CopyString(const char *begin, char* out);
int GetEnterCount(const char *begin);

void Print2Buff(rapidxml::xml_node<> *node, StringBuff& buff);
bool CompareData(const char* begL,const char* endL,const char* begR,const char* endR);

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
