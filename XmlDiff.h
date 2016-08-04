#pragma once
#include "Common.h"
#include <string>
#include <vector>
#include <hash_set>
#include "RapidXml/rapidxml.hpp"
#include <utility>
#include <list>
#include <set>
#include "diffui.h"

using namespace rapidxml;
using namespace std;

class XmlFile
{
public:
	bool Parse(std::string filename);
	rapidxml::xml_document<> doc;
};

struct DiffContext
{
	DiffContext(const std::vector<string>& stringVecL, const std::vector<string>& stringVecR):stringVecL(stringVecL),stringVecR(stringVecR)
	{
	}
	const std::vector<string>& stringVecL;
	const std::vector<string>& stringVecR;
	vector<DiffType> resL;
	vector<DiffType> resR;
	vector<vector<int>>	cache;
	int maxValue;
};

struct NodeMatchResult
{
	vector<DiffType> resL;
	vector<DiffType> resR;
};


class XmlDiff
{
public:
	XmlDiff(DiffUI* ui)
	{
		diffUIView = ui;
	}
	bool Diff( std::string file1, std::string file2);
	void RenderText();
	void test();
private:
	//bool DiffLevel( xml_node<> *nodeL, xml_node<> *nodeR, DiffNodeResult& result);
	//bool MatchNode( xml_node<> *nodeL, xml_node<> *nodeR);
	bool Compare(const char* strL,const char* strR);

	//DiffNodeResult DumpNode(xml_node<>* node, DiffType type);
	DiffNodeResult DiffMatchedNode( xml_node<> *nodeL, xml_node<> *nodeR);
	
	std::list<DiffNodeResult> DiffSibling( xml_node<> *nodeL, xml_node<> *nodeR);


	std::list<DiffNodeResult> DiffNodes( const std::vector<xml_node<>*>& nodeLVector, const std::vector<xml_node<>*>& nodeRVector);
	bool CompareNode(xml_node<> *nodeL, xml_node<> *nodeR);
	std::list<DiffNodeResult> ConvertTrace(const std::list<std::pair<int,int>>& trace, const std::vector<xml_node<>*>& nodeLVector, const std::vector<xml_node<>*>& nodeRVector);

	std::list<DiffNodeResult> DiffNodesAcceptModify( const std::vector<xml_node<>*>& nodeLVector, const std::vector<xml_node<>*>& nodeRVector);

	const char* GetDiffTypeString(DiffType type);
	void DumpResult(const std::list<DiffNodeResult>& diffNodeList,DiffUI* ui,int indent);
	void DumpResultLog(const std::list<DiffNodeResult>& diffNodeList);
	size_t NodeStringDistance(xml_node<> *nodeL, xml_node<> *nodeR);

	NodeMatchResult XmlDiff::DiffStringListAcceptModify( const std::vector<string>& stringVecL, const std::vector<string>& stringVecR);


	
	int MatchNode( DiffContext& context, int fromL, int fromR );
	void GenMatchResult(DiffContext& context, int fromL, int fromR);

private:
	DiffUI* diffUIView;
	std::list<DiffNodeResult> diffResult;
	XmlFile xmlFileL,xmlFileR;
};


