#pragma once
#include "Common.h"
#include <string>
#include <vector>
//#include <hash_set>
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
	size_t FileSize() const { return fileSize; }

private:
	size_t fileSize;

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

	bool CompareNullTerminatedStr( const char* strL,const char* strR );
	bool Compare(const char* strL,size_t sizeL,const char* strR,size_t sizeR);

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

	void FormatAttr(StringBuff& buff,const XmlString& name, const XmlString& value);

	struct DumpContext
	{
		std::list<DiffAttrResult>::const_iterator iter;
		DiffUI* ui;
		bool singleAttr;
		DiffType prevDiffType;
		int attrLineCol;
	};
	void HandleAddAttr(DumpContext& context,int indent);
	void HandleDelAttr(DumpContext& context,int indent);
	void HandleModifyAttr(DumpContext& context,int indent);
	void HandleUnchangeAttr(const std::list<DiffAttrResult>::const_iterator iter,DiffUI* ui,int indent,bool attrChanged,DiffType prevDiffType,int& attrLineCol);
	void HandleSingleValue(const DiffSingleValueResult& diffSingleValue,DiffUI* ui,int indent);

	size_t CalcStringBuffMaxSize();
private:
	DiffUI* diffUIView;
	std::list<DiffNodeResult> diffResult;
	XmlFile xmlFileL,xmlFileR;

	bool hideUnchangedNode;
};


