#pragma once
#include <list>
#include <iostream>
#include <vector>
#include "RapidXml/rapidxml.hpp"

#define SIMPLIFY_STRING_DIST_CALC 0
#define ENABLE_LOG 0
#define ENABLE_CONSOLE 0

#define FOR_EACH(iter,container) for (auto iter = container.begin(); iter!=container.end(); ++iter)

enum DiffType
{
	DiffType_Add,
	DiffType_Del,
	DiffType_Modify,
	DiffType_Unchanged,
};

struct DiffAttrResult
{
	DiffType type;
	const char* name;
	const char* prev;
	const char* curr;
};

struct DiffNodeResult
{
	DiffNodeResult()
	{
		node = nullptr;
		//targetNode = nullptr;
	}


	DiffNodeResult(rapidxml::xml_node<>* node, DiffType type)
	{
		this->node = node;
		this->type = type;
	}

	/*void print()
	{
		using namespace std;
		if (name)
		{
			cout<<name<<endl;
		}
		for (auto iter = attr.begin(); iter!=attr.end(); ++iter)
		{
			if (iter->type == DiffType_Add)
			{
				cout<< iter->name << " ";
				cout << "add " << iter->prev << endl;
			}
			else if (iter->type == DiffType_Del)
			{
				cout<< iter->name << " ";
				cout << "del " << iter->prev << endl;
			}
			else if (iter->type == DiffType_Modify)
			{
				cout<< iter->name << " ";
				cout << "modify " <<iter->prev << " " << iter->curr << endl;
			}
		}

		for (auto iter = child.begin(); iter!=child.end(); ++iter)
		{
			iter->print();
		}
	}*/
	//const char* name;
	rapidxml::xml_node<>* node;
//	rapidxml::xml_node<>* targetNode;
	DiffType type;
	std::list<DiffAttrResult> attr;
	std::list<DiffNodeResult> child;	
};


struct LineMatchResult
{
	LineMatchResult()
	{
		LeftLineBeg = LeftLineEnd = RightLineBeg = RightLineEnd = 0;
		same = false;
	}
	int LeftLineBeg;
	int LeftLineEnd;

	int RightLineBeg;
	int RightLineEnd;

	bool same;
};

class BiasArray
{
public:
	BiasArray(int length)
	{
		assert(length > 0);
		vec.assign(length,0);
		bias = length / 2 ;
		this->length = length;
	}

	const int operator [] (int index) const
	{
		return vec[bias + index];
	}

	int& operator [] (int index)
	{
		return vec[bias + index];
	}
	
	int bias;
	int length;
	std::vector<int> vec;
};

