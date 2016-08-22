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

class StringBuff;

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
		hideUnchangedNode = false;
	}
	bool Diff( std::string file1, std::string file2);
	void RenderText();
	void test();

	bool HideUnchangedNode() const { return hideUnchangedNode; }
	void HideUnchangedNode(bool val) { hideUnchangedNode = val; }

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
	void DumpResult(const std::list<DiffNodeResult>& diffNodeList,DiffUI* ui,int indent,bool parentChanged = false);
	void DumpNodeAttr(const std::list<DiffNodeResult>::const_iterator iter,DiffUI* ui,int indent,bool parentChanged);
	void DumpResultLog(const std::list<DiffNodeResult>& diffNodeList);
	size_t NodeStringDistance(xml_node<> *nodeL, xml_node<> *nodeR);

	NodeMatchResult XmlDiff::DiffStringListAcceptModify( const std::vector<string>& stringVecL, const std::vector<string>& stringVecR);


	
	int MatchNode( DiffContext& context, int fromL, int fromR );
	void GenMatchResult(DiffContext& context, int fromL, int fromR);

	void FormatAttr(StringBuff& buff,const char* name, const char* value);

	void HandleAddAttr(const std::list<DiffAttrResult>::const_iterator iter,DiffUI* ui,int indent);
	void HandleDelAttr(const std::list<DiffAttrResult>::const_iterator iter,DiffUI* ui,int indent);
	void HandleModifyAttr(const std::list<DiffAttrResult>::const_iterator iter,DiffUI* ui,int indent);
	void HandleUnchangeAttr(const std::list<DiffAttrResult>::const_iterator iter,DiffUI* ui,int indent,bool attrChanged,DiffType prevDiffType,int& attrLineCol);
	void HandleSingleValue(const DiffSingleValueResult& diffSingleValue,DiffUI* ui,int indent);
private:
	DiffUI* diffUIView;
	std::list<DiffNodeResult> diffResult;
	XmlFile xmlFileL,xmlFileR;

	bool hideUnchangedNode;
};


