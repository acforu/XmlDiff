#pragma once

using namespace std;

enum PatchType
{
	PatchType_Add,
	PatchType_Del,
	PatchType_Modify,
};

struct XmlPatchAttr
{
	uint16_t index;
	PatchType type;
	XmlString atrrName;
	XmlPatchAttr value;
};

struct XmlPatchSingleValue
{
	XmlString prev;
	XmlString curr;
};

struct XmlPatchNode
{
	uint16_t index;
	PatchType type;
	bool isSingleValue;
	union 
	{
		XmlPatchSingleValue singleValue;
		std::list<XmlPatchAttr> attr;
	};
	
	std::list<XmlPatchNode> child;
};

class XmlPatch
{
public:
	XmlPatch();
	~XmlPatch();

	

};

