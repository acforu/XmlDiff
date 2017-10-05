#pragma once
#include "Common.h"
#include "XmlFile.h"
using namespace std;

enum PatchType
{
	PatchType_None,
	PatchType_Add,
	PatchType_Del,
	PatchType_Modify,
};

struct XmlPatchNode;
struct XmlPatchAttr
{
	uint16_t index;
	PatchType type;
	XmlString atrrName;
	XmlString value;
};

struct XmlPatchSingleValue
{
	XmlString prev;
	XmlString curr;
};

struct XmlPatchAddNode
{
	XmlString curr;
};

//#limit only support value modification, name modification will mark the whole node to be delete and create a new one
struct XmlPatchModifyNode
{
	XmlPatchSingleValue singleValue; //#todo opt
	std::list<XmlPatchAttr> attr;
	std::list<XmlPatchNode> child;
};

struct XmlPatchDelNode
{

};

struct XmlPatchNode
{
	XmlPatchNode():
		index(0),
		type(PatchType_None)
	{}
	~XmlPatchNode() {}
	uint16_t index;
	PatchType type;
	union 
	{
		XmlPatchAddNode add;
		XmlPatchModifyNode modify;
		XmlPatchDelNode del;
	};
};

class XmlPatch
{
public:
	XmlPatch();
	~XmlPatch();

	bool LoadXml(const string& filename);
	bool ApplyPatch();

private:
	shared_ptr<XmlFile> srcXmlFile;
	std::list<XmlPatchNode> xmlPatchRoot;
};

