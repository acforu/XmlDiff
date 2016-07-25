#pragma once
#include "Common.h"


class XmlHelper
{
	
};



std::list<std::pair<int,int>> LCSTrace( const std::vector<BiasArray>& VList, int k, int finalD);

size_t StringDistance(const std::string &s1, const std::string &s2);
size_t StringDistBasedLine(const std::string &s1, const std::string &s2);

int DiffLines( const std::vector<std::string>& nodeLVec, const std::vector<std::string>& nodeRVec);
int CalcStringDist( const std::list<std::pair<int,int>>& trace,const std::vector<std::string>& nodeLVec, const std::vector<std::string>& nodeRVec);

char* CopyString(const char *begin, char* out);
int GetEnterCount(const char *begin);
