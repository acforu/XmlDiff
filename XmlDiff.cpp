#include "stdafx.h"
#include "XmlDiff.h"
#include "windows.h"
#include "XmlHelper.h"
#include "StringBuff.h"

using namespace rapidxml;
using namespace std;

typedef  size_t AddressType;


const int XmlTextBuffSize = 10000000;


bool XmlDiff::Diff( std::string file1, std::string file2)
{
	if(!xmlFileL.Parse(file1))
		return false;

	if(!xmlFileR.Parse(file2))
		return false;

	diffResult = DiffSibling(xmlFileL.doc.first_node(),xmlFileR.doc.first_node());

	//static char buff[XmlTextBuffSize];
	//print(buff,*a.doc.first_node());
	//ui->AppendText(buff,TextSide_Left,TextColor_Modify);
	//ui->AppendText(buff,TextSide_Right,TextColor_Normal);

	//DumpResult(diffResult);

	//print(std::ostream_iterator<char>(cout), *a.doc.first_node(), 0);

	return true;
}


//bool XmlDiff::DiffLevel( xml_node<> *nodeL, xml_node<> *nodeR, DiffNodeResult& result )
//{
//	std::hash_map<int,bool> visited;
//	for (xml_node<> *fromNode = nodeL; fromNode; fromNode = fromNode->next_sibling())
//	{
//		bool matched = false;
//		xml_node<> *toNode = nodeR;
//		for (;toNode; toNode = toNode->next_sibling(fromNode->name()))
//		{
//			if (MatchNode(fromNode,toNode)) //node name ,first attr key and value are same
//			{
//				matched = true;
//				break;
//			}
//		}
//
//		if (matched)
//		{
//			visited[AddressType(toNode)] = true;
//			DiffNodeResult nodeResult = DiffMatchedNode(fromNode,toNode);
//			result.child.push_back(nodeResult);
//		}
//		else
//		{
//			DiffNodeResult nodeResult = DumpNode(fromNode,DiffType_Del);
//			result.child.push_back(nodeResult);
//		}
//	}
//
//
//	
//	for (xml_node<> *toNode = nodeR;toNode; toNode = toNode->next_sibling())
//	{
//		if (visited.find((AddressType)toNode) == visited.end())
//		{
//			DiffNodeResult nodeResult = DumpNode(toNode,DiffType_Add);
//			result.child.push_back(nodeResult);
//		}
//	}
//
//	return true;
//}

//node name ,first attr key and value are same

//bool XmlDiff::MatchNode( xml_node<> *nodeL, xml_node<> *nodeR )
//{
//	if (!Compare(nodeL->name(),nodeR->name()))
//		return false;
//
//	xml_attribute<> * attrL = nodeL->first_attribute();
//	xml_attribute<> * attrR = nodeR->first_attribute();
//
//	if (attrL == NULL && attrR == NULL)
//		return true;
//
//	if (attrL && attrR)
//	{
//		if (Compare(attrL->name(),attrR->name()) && 
//			Compare(attrL->value(),attrR->value()))
//		{
//			return true;
//		}
//	}
//
//	return false;
//}

bool XmlDiff::Compare( const char* strL,const char* strR )
{
	return std::string(strL) == strR;
}

//DiffNodeResult XmlDiff::DumpNode( xml_node<>* node, DiffType type )
//{
//	DiffNodeResult result;
//	if (node == NULL)
//		return result;
//
//	result.name = node->name();
//	for (xml_attribute<> * attr = node->first_attribute(); attr; attr = attr->next_attribute())
//	{
//		DiffAttrResult attrResult;
//		attrResult.type = type;
//		attrResult.name = attr->name();
//		attrResult.prev = attr->value();
//
//		result.attr.push_back(attrResult);
//	}
//
//	for (xml_node<> *fromNode = node->first_node(); fromNode; fromNode = fromNode->next_sibling())
//	{
//		DiffNodeResult childNodeResult = DumpNode(fromNode,type);
//		result.child.push_back(childNodeResult);
//	}
//	return result;
//}
//
DiffNodeResult XmlDiff::DiffMatchedNode( xml_node<> *nodeL, xml_node<> *nodeR )
{
#if ENABLE_LOG
	cout << "-------------------------------" <<endl;
	cout << "DiffMatchedNode: " <<endl;
	cout << "node1: ---------------------" <<endl;
	cout<< *nodeL << endl;
	cout << "node2: ---------------------" <<endl;
	cout<< *nodeR <<endl;
	cout << "-------------------------------" <<endl;
#endif

	DiffNodeResult result;

	result.node = nodeL;
	result.type = DiffType_Modify;
	std::hash_map<int,bool> visited;
	for (xml_attribute<> * attrL = nodeL->first_attribute(); attrL; attrL = attrL->next_attribute())
	{
		DiffAttrResult attrResult;
		attrResult.name = attrL->name();

		if(xml_attribute<> * attrR = nodeR->first_attribute(attrL->name()))
		{
			if (Compare(attrL->value(),attrR->value()))
			{
				attrResult.type = DiffType_Unchanged;
			}
			else
			{
				attrResult.type = DiffType_Modify;
			}
			attrResult.prev = attrL->value();
			attrResult.curr = attrR->value();
			visited[AddressType(attrR)] = true;
		}
		else
		{
			attrResult.type = DiffType_Del;
			attrResult.prev = attrL->value();
		}

		result.attr.push_back(attrResult);
	}

	for (xml_attribute<> * attrR = nodeR->first_attribute(); attrR; attrR = attrR->next_attribute())
	{
		if (visited.find(AddressType(attrR)) == visited.end())
		{
			DiffAttrResult attrResult;
			attrResult.type = DiffType_Add;
			attrResult.name = attrR->name();
			attrResult.prev = attrR->value();
			result.attr.push_back(attrResult);
		}
	}

	result.child = DiffSibling(nodeL->first_node(),nodeR->first_node());

	return result;
}

bool XmlFile::Parse( std::string filename )
{
	ifstream file (filename, ios::in|ios::binary|ios::ate);
	if (!file.is_open())
	{
		return false;
	}

	doc.clear();

	size_t size = (size_t)file.tellg();
	file.seekg (0, ios::beg);

	char* memblock = new char [size+1];
	memset(memblock,0,size+1);
	file.read (memblock, size);
	char * filecontent = doc.allocate_string(memblock,size+1);
	delete[] memblock;

	file.close();

	//cout << "the entire file content is in memory" << endl;

	try
	{
		doc.parse<parse_default>(filecontent);
	}
	catch(rapidxml::parse_error& error)
	{
		string err = filename + " " + error.what();
		//MessageBoxA(NULL, err.c_str(), "XML Parse Error", MB_OK|MB_ICONERROR|MB_SYSTEMMODAL);
		cout << "XML Parse Error: " << err << endl;
		return false;
	}

	return true;
}

std::list<DiffNodeResult> XmlDiff::DiffSibling( xml_node<> *nodeL, xml_node<> *nodeR )
{
	std::vector<xml_node<>*> nodeLVector;
	for (xml_node<> *fromNode = nodeL; fromNode; fromNode = fromNode->next_sibling())
	{
		nodeLVector.push_back(fromNode);
	}
	
	std::vector<xml_node<>*> nodeRVector;
	for (xml_node<> *toNode = nodeR;toNode; toNode = toNode->next_sibling())
	{
		nodeRVector.push_back(toNode);
	}

	return DiffNodes(nodeLVector,nodeRVector);
}


std::list<DiffNodeResult> XmlDiff::DiffNodes( const std::vector<xml_node<>*>& nodeLVec, const std::vector<xml_node<>*>& nodeRVec)
{
	std::list<DiffNodeResult> ret;

	int N = nodeLVec.size();
	int M = nodeRVec.size();
	int maxD = N + M;
	vector<BiasArray> VList;

	BiasArray stub(3);
	//stub.assign(2,0);
	//V[ 1 ] = 0;
	//VList.push_back(V);

	for ( int d = 0 ; d <= maxD ; d++ )
	{
		BiasArray prevV = (d == 0) ? stub : VList.back();

		{
			BiasArray currV(max(d*2+1,3));
			//currV.assign(d*2 + 1,0);
			//V[ 1 ] = 0;
			VList.push_back(currV);
		}

		BiasArray& currV = VList.back();


		for ( int k = -d ; k <= d ; k += 2 )
		{
			// down or right?
			bool down = ( k == -d || ( k != d && prevV[k - 1] < prevV[k + 1] ) );

			int kPrev = down ? k + 1 : k - 1;

			// start point
			int xStart = prevV[kPrev];
			int yStart = xStart - kPrev;

			// mid point
			int xMid = down ? xStart : xStart + 1;
			int yMid = xMid - k;

			// end point
			int xEnd = xMid;
			int yEnd = yMid;

			// follow diagonal
			int snake = 0;
			while ( xEnd < N && yEnd < M && CompareNode(nodeLVec[xEnd],nodeRVec[yEnd])) { xEnd++; yEnd++; snake++; }

			// save end point
			currV[k] = xEnd;

			// check for solution
			if ( xEnd >= N && yEnd >= M ) /* solution has been found */
			{
				std::list<std::pair<int,int>> trace = LCSTrace(VList,k,d);
				ret = ConvertTrace(trace,nodeLVec,nodeRVec);
				goto end;
			}
		}
	}

	end:
	return ret;
}

bool XmlDiff::CompareNode( xml_node<> *nodeL, xml_node<> *nodeR )
{
	//static char buffL[XmlTextBuffSize];
	//static char buffR[XmlTextBuffSize];

	//ZeroMemory(buffL,ARRAYSIZE(buffL));
	//ZeroMemory(buffR,ARRAYSIZE(buffR));

	//if (!Compare(nodeL->name(),nodeR->name()))
	//{
	//	return false;
	//}

	static StringBuff buffL;
	static StringBuff buffR;

	buffL.Clear();
	print(buffL.Begin(),*nodeL);
	buffL.AppendNull();
	//*out = '\0';
	
	buffR.Clear();
	print(buffR.Begin(),*nodeR);
	buffR.AppendNull();

	return strcmp(buffL.Data(),buffR.Data())==0;
}


void XmlDiff::test()
{
	 //std::vector<xml_node<>*> L;
	 //L.push_back((xml_node<>*)1);
	 //L.push_back((xml_node<>*)2);
	 //L.push_back((xml_node<>*)3);
	 //L.push_back((xml_node<>*)4);

	 //std::vector<xml_node<>*> R;
	 //R.push_back((xml_node<>*)2);
	 //R.push_back((xml_node<>*)4);

	 //DiffNodes(L,R);

	//std::vector<string> A,B;
	//A.push_back("222");
	//A.push_back("3");
	//A.push_back("4");

	//B.push_back("31");
	//B.push_back("41");

	//DiffNodesAcceptModify(A,B);
}

const char* XmlDiff::GetDiffTypeString(DiffType type)
{
	switch(type)
	{
	case DiffType_Add: return "Add";
	case DiffType_Del: return "Del";
	case DiffType_Modify: return "Modify";
	case DiffType_Unchanged: return "Unchanged";
	}
	return "Invalid type";
}



void XmlDiff::DumpResultLog( const std::list<DiffNodeResult>& diffNodeList)
{
	FOR_EACH(iter,diffNodeList)
	{
		if (iter->type == DiffType_Unchanged)
		{
			print(std::ostream_iterator<char>(cout), *iter->node, 0);
		}
		else
		{
			if (iter->type != DiffType_Modify)
			{
				cout << GetDiffTypeString(iter->type) << endl;
			}

			cout << "<" << iter->node->name();
			rapidxml::internal::print_attributes(std::ostream_iterator<char>(cout), iter->node, 0);
			cout << ">" << endl;


			for (auto attrIt = iter->attr.begin(); attrIt!=iter->attr.end(); ++attrIt)
			{
				if (attrIt->type == DiffType_Add)
				{
					cout<< attrIt->name << " ";
					cout << "add " << attrIt->prev << endl;
				}
				else if (attrIt->type == DiffType_Del)
				{
					cout<< attrIt->name << " ";
					cout << "del " << attrIt->prev << endl;
				}
				else if (attrIt->type == DiffType_Modify)
				{
					cout<< attrIt->name << " ";
					cout << "modify " <<attrIt->prev << " " << attrIt->curr << endl;
				}
			}
			DumpResultLog(iter->child);
		}
		//cout <<  *iter->node << endl;
	}
}



void XmlDiff::DumpResult( const std::list<DiffNodeResult>& diffNodeList,DiffUI* ui,int indent )
{
	//static StringBuff strBuff;
	FOR_EACH(iter,diffNodeList)
	{
		if (iter->type == DiffType_Unchanged)
		{
			StringBuff strBuff;
			rapidxml::internal::print_node(strBuff.Begin(), iter->node, 0,indent);
			ui->AppendText(strBuff,TextSide_Left,TextColor_Normal);
			ui->AppendText(strBuff,TextSide_Right,TextColor_Normal);
		}
		else if (iter->type == DiffType_Add)
		{
			ui->ModifyMarkBegin();
			{
				StringBuff strBuff;
				rapidxml::internal::print_node(strBuff.Begin(), iter->node, 0,indent);
				ui->AppendText(strBuff,TextSide_Right,TextColor_Modify);
				int enterCount = strBuff.Total('\n');
				strBuff.Clear();
				strBuff.FillChars('\n',enterCount);
				ui->AppendText(strBuff,TextSide_Left,TextColor_Normal);
			}
			ui->ModifyMarkEnd();
		}
		else if(iter->type == DiffType_Del)
		{
			ui->ModifyMarkBegin();
			{
				StringBuff strBuff;
				rapidxml::internal::print_node(strBuff.Begin(), iter->node, 0,indent);
				ui->AppendText(strBuff,TextSide_Left,TextColor_Modify);
				int enterCount = strBuff.Total('\n');
				strBuff.Clear();
				strBuff.FillChars('\n',enterCount);
				ui->AppendText(strBuff,TextSide_Right,TextColor_Normal);
			}
			ui->ModifyMarkEnd();
		}
		else
		{
			{
				StringBuff strBuff;
				strBuff.FillChars('\t',indent);
				strBuff.AppendChar('<');
				strBuff.AppendStr(iter->node->name(),iter->node->name() + iter->node->name_size());
				ui->AppendText(strBuff,TextSide_Left,TextColor_Normal);
				ui->AppendText(strBuff,TextSide_Right,TextColor_Normal);
			}

			bool attrChanged = false;
			DiffType prevDiffType = DiffType_Unchanged;
			for (auto attrIt = iter->attr.begin(); attrIt!=iter->attr.end(); ++attrIt)
			{
				if (attrIt->type == DiffType_Unchanged)
				{
					StringBuff strBuff;
					strBuff.AppendChar(' ');
					FormatAttr(strBuff,attrIt->name,attrIt->prev);
					ui->AppendText(strBuff,TextSide_Left,TextColor_Normal);
					ui->AppendText(strBuff,TextSide_Right,TextColor_Normal);

				}
				else
				{
					attrChanged = true;
					if (prevDiffType == DiffType_Unchanged)
					{
						ui->AppendNewLine();
					}
				}

				if (attrIt->type == DiffType_Add)
				{

					ui->ModifyMarkBegin();


					StringBuff strBuff;
					strBuff.FillChars('\t',indent);
					FormatAttr(strBuff,attrIt->name,attrIt->prev);
					strBuff.AppendChar('\n');

					ui->AppendText(strBuff,TextSide_Right,TextColor_Modify);


					int enterCount = strBuff.Total('\n');
					strBuff.Clear();
					strBuff.FillChars('\n',enterCount);

					ui->AppendText(strBuff,TextSide_Left,TextColor_Normal);

					ui->ModifyMarkEnd();


				}
				else if (attrIt->type == DiffType_Del)
				{
					ui->ModifyMarkBegin();

					StringBuff strBuff;
					strBuff.FillChars('\t',indent);
					FormatAttr(strBuff,attrIt->name,attrIt->prev);
					strBuff.AppendChar('\n');

					ui->AppendText(strBuff,TextSide_Left,TextColor_Modify);

					int enterCount = strBuff.Total('\n');
					strBuff.Clear();
					strBuff.FillChars('\n',enterCount);
					ui->AppendText(strBuff,TextSide_Right,TextColor_Normal);

					ui->ModifyMarkEnd();

				}
				else if (attrIt->type == DiffType_Modify)
				{
					ui->ModifyMarkBegin();

					StringBuff strBuff;
					strBuff.FillChars('\t',indent);
					FormatAttr(strBuff,attrIt->name,attrIt->prev);
					strBuff.AppendChar('\n');

					ui->AppendText(strBuff,TextSide_Left,TextColor_Modify);
					int leftEnterCount = strBuff.Total('\n');

					strBuff.Clear();
					strBuff.FillChars('\t',indent);
					FormatAttr(strBuff,attrIt->name,attrIt->curr);
					strBuff.AppendChar('\n');

					ui->AppendText(strBuff,TextSide_Right,TextColor_Modify);
					int RightEnterCount = strBuff.Total('\n');

					if (leftEnterCount < RightEnterCount)
					{
						strBuff.Clear();
						strBuff.FillChars('\n',RightEnterCount-leftEnterCount);
						ui->AppendText(strBuff,TextSide_Left,TextColor_Modify);
					}
					else if(leftEnterCount > RightEnterCount)
					{
						strBuff.Clear();
						strBuff.FillChars('\n',leftEnterCount-RightEnterCount);
						ui->AppendText(strBuff,TextSide_Right,TextColor_Modify);
					}

					ui->ModifyMarkEnd();

				}

				prevDiffType = attrIt->type;

			}


			if(attrChanged)
			{
				//StringBuff strBuff;
				//strBuff.AppendChar('\n');
				//strBuff.AppendNull();

				//ui->AppendText(strBuff.Data(),TextSide_Left,TextColor_Normal);
				//ui->AppendText(strBuff.Data(),TextSide_Right,TextColor_Normal);

				ui->AppendNewLine();
			}

			StringBuff strBuff;
			if(attrChanged)
			{
				strBuff.FillChars('\t',indent);
			}
			strBuff.AppendChar('>');
			strBuff.AppendChar('\n');

			ui->AppendText(strBuff,TextSide_Left,TextColor_Normal);
			ui->AppendText(strBuff,TextSide_Right,TextColor_Normal);


			DumpResult(iter->child,ui,indent+1);

			strBuff.Clear();
			strBuff.FillChars('\t',indent);
			strBuff.AppendChar('<');
			strBuff.AppendChar('\\');
			strBuff.AppendStr(iter->node->name(), iter->node->name() + iter->node->name_size());
			strBuff.AppendChar('>');
			strBuff.AppendChar('\n');

			ui->AppendText(strBuff,TextSide_Left,TextColor_Normal);
			ui->AppendText(strBuff,TextSide_Right,TextColor_Normal);
		
		}
	}
}


std::list<DiffNodeResult> XmlDiff::ConvertTrace( const std::list<std::pair<int,int>>& trace,const std::vector<xml_node<>*>& nodeLVec, const std::vector<xml_node<>*>& nodeRVec)
{
	std::list<DiffNodeResult>  ret;
	FOR_EACH(iter,trace)
	{
		cout << iter->first - 1 << " " << iter->second - 1 <<endl;
	}

	size_t indexL = 0 , indexR = 0;
	for(auto iter = trace.rbegin(); iter != trace.rend(); ++iter)
	{
		size_t sameIndexL = iter->first - 1;
		size_t sameIndexR = iter->second - 1;

		vector<xml_node<>*> nodesL;
		for (; indexL < sameIndexL; ++indexL)
		{
			//DiffNodeResult diffNode;
			//diffNode.node = nodeLVec[indexL];
			//diffNode.type = DiffType_Del;
			//ret.push_back(diffNode);
			nodesL.push_back(nodeLVec[indexL]);
		}

		vector<xml_node<>*> nodesR;
		for (; indexR < sameIndexR; ++indexR)
		{
			//DiffNodeResult diffNode;
			//diffNode.node = nodeLVec[indexR];
			//diffNode.type = DiffType_Add;
			//ret.push_back(diffNode);
			nodesR.push_back(nodeRVec[indexR]);
		}

		std::list<DiffNodeResult> diff = DiffNodesAcceptModify(nodesL,nodesR);
		ret.insert(ret.end(),diff.begin(),diff.end());

		ret.push_back(DiffNodeResult(nodeLVec[indexL],DiffType_Unchanged));

		++indexL;
		++indexR;
	}

	vector<xml_node<>*> nodesL;
	for(;indexL < nodeLVec.size(); ++indexL)
	{
		//DiffNodeResult diffNode;
		//diffNode.node = nodeLVec[indexL];
		//diffNode.type = DiffType_Del;
		//ret.push_back(diffNode);
		nodesL.push_back(nodeLVec[indexL]);
	}
	vector<xml_node<>*> nodesR;
	for(;indexR < nodeRVec.size(); ++indexR)
	{
		//DiffNodeResult diffNode;
		//diffNode.node = nodeLVec[indexR];
		//diffNode.type = DiffType_Add;
		//ret.push_back(diffNode);
		nodesR.push_back(nodeRVec[indexR]);
	}
	std::list<DiffNodeResult> diff = DiffNodesAcceptModify(nodesL,nodesR);
	ret.insert(ret.end(),diff.begin(),diff.end());

	return ret;
}

std::list<DiffNodeResult> XmlDiff::DiffNodesAcceptModify( const std::vector<xml_node<>*>& nodeLVector, const std::vector<xml_node<>*>& nodeRVector )
{
#if ENABLE_LOG 
	cout << "-------------------------------" <<endl;
	cout << "DiffNodesAcceptModify: " <<endl;
	cout << "node1: -----------" <<endl;
	FOR_EACH(iter,nodeLVector)
	{
		cout << **iter;
	}
	cout << "node2: -----------" <<endl;
	FOR_EACH(iter,nodeRVector)
	{
		cout << **iter;
	}

	cout << "-------------------------------" <<endl;
#endif



	//std::list<DiffNodeResult> ret;
	//size_t i = 0;
	//for (; i < nodeLVector.size() && i < nodeRVector.size(); ++i)
	//{
	//	DiffNodeResult result = DiffMatchedNode(nodeLVector[i],nodeRVector[i]);
	//	cout << "dist " << NodeStringDistance(nodeLVector[i],nodeRVector[i]) << endl;
	//	ret.push_back(result);
	//}

	//for (;i < nodeLVector.size(); ++i)
	//{
	//	ret.push_back(DiffNodeResult(nodeLVector[i],DiffType_Del));
	//}

	//for (;i < nodeRVector.size(); ++i)
	//{
	//	ret.push_back(DiffNodeResult(nodeRVector[i],DiffType_Add));
	//}
	//return ret;

	//static char buff[XmlTextBuffSize];		
	static StringBuff buff;

	vector<string> stringVecL,stringVecR;
	FOR_EACH(iter,nodeLVector)
	{
		//ZeroMemory(buff,ARRAYSIZE(buff));
		buff.Clear();
		print(buff.Begin(),**iter,print_attributes_separate_by_enter);
		buff.AppendNull();
		//*out = '\0';
		stringVecL.push_back(buff.Data());
	}
	FOR_EACH(iter,nodeRVector)
	{
		//ZeroMemory(buff,ARRAYSIZE(buff));
		buff.Clear();
		print(buff.Begin(),**iter,print_attributes_separate_by_enter);
		buff.AppendNull();
		//*out = '\0';
		stringVecR.push_back(buff.Data());
	}
	
	std::list<DiffNodeResult> ret;

	NodeMatchResult result = DiffStringListAcceptModify(stringVecL,stringVecR);
	auto& resL = result.resL;
	auto& resR = result.resR;

	assert(resL.size() == nodeLVector.size());
	assert(resR.size() == nodeRVector.size());

	size_t indexL = 0, indexR = 0;
	while(1)
	{
		if (indexL == resL.size() && indexR == resR.size())
		{
			break;
		}

		while(indexL < resL.size() && resL[indexL] != DiffType_Modify)
		{
			DiffNodeResult item(nodeLVector[indexL],DiffType_Del);
			ret.push_back(item);
			++indexL;
		}

		while(indexR < resR.size() && resR[indexR] != DiffType_Modify)
		{
			DiffNodeResult item(nodeRVector[indexR],DiffType_Add);
			ret.push_back(item);
			++indexR;
		}

		while(indexL < resL.size() 
			&& indexR < resR.size() 
			&& resL[indexL] == DiffType_Modify
			&& resR[indexR] == DiffType_Modify)
		{
			DiffNodeResult item = DiffMatchedNode(nodeLVector[indexL],nodeRVector[indexR]);
			ret.push_back(item);
			++indexL;
			++indexR;
		}
	}

	return ret;
}


size_t XmlDiff::NodeStringDistance( xml_node<> *nodeL, xml_node<> *nodeR )
{
	static char buffL[2000];
	static char buffR[2000];

	//ZeroMemory(buffL,ARRAYSIZE(buffL));
	//ZeroMemory(buffR,ARRAYSIZE(buffR));

	char* out = print(buffL,*nodeL);
	*out = '\0';
	out = print(buffR,*nodeR);
	*out = '\0';

	if (Compare(buffL,buffR))
		return 0;
	else
		return StringDistance(buffL,buffR);
}


NodeMatchResult XmlDiff::DiffStringListAcceptModify( const std::vector<string>& stringVecL, const std::vector<string>& stringVecR )
{
	int maxValue = 1000;
	int n = stringVecL.size();
	int m = stringVecR.size();

	DiffContext context(stringVecL,stringVecR);
	context.resL.assign(n,DiffType_Unchanged);
	context.resR.assign(m,DiffType_Unchanged);
	context.cache.resize(n+1);
	context.maxValue = maxValue;

	FOR_EACH(iter,context.cache)
	{
		iter->assign(m+1,maxValue);
	}

	MatchNode(context,0,0);
	GenMatchResult(context,0,0);



#if ENABLE_LOG
	FOR_EACH(iter,context.resL)
	{
		cout << GetDiffTypeString(*iter) <<endl;
	}

	FOR_EACH(iter,context.resR)
	{
		cout << GetDiffTypeString(*iter) <<endl;
	}
#endif

	NodeMatchResult ret;
	ret.resL = context.resL;
	ret.resR = context.resR;
	return ret;
}

int XmlDiff::MatchNode( DiffContext& context, int fromL, int fromR )
{
	int& cacheValue = context.cache[fromL][fromR];
	if (cacheValue != context.maxValue)
	{
		return cacheValue;
	}

	if (fromL == context.stringVecL.size())
	{
		int ret = 0;
		for (size_t i = fromR; i < context.stringVecR.size(); ++i)
		{
			ret += context.stringVecR[i].length();
		}
		cacheValue = ret;
		return ret;
	}


	if (fromR == context.stringVecR.size())
	{
		int ret = 0;
		for (size_t i = fromL; i < context.stringVecL.size(); ++i)
		{
			ret += context.stringVecL[i].length();
		}
		cacheValue = ret;
		return ret;
	}

	int valueMatch = MatchNode(context,fromL+1,fromR+1) + StringDistBasedLine(context.stringVecL[fromL],context.stringVecR[fromR]);
	int valueKickA = context.stringVecL[fromL].length() + MatchNode(context,fromL+1,fromR);
	int valueKickB = context.stringVecR[fromR].length() + MatchNode(context,fromL,fromR+1);

	cacheValue = min(valueMatch,min(valueKickA,valueKickB));
	cout << "write cache:" << fromL << " " << fromR << " "<< cacheValue <<endl;
	//if (cacheValue == valueMatch)
	//{
	//	context.resL[fromL]	= DiffType_Modify;
	//	context.resR[fromR]	= DiffType_Modify;
	//}
	//else if (cacheValue == valueKickA)
	//{
	//	context.resL[fromL]	= DiffType_Add;
	//}
	//else
	//{
	//	context.resR[fromR]	= DiffType_Del;
	//}

	return cacheValue;
}

void XmlDiff::GenMatchResult( DiffContext& context, int fromL, int fromR )
{
	//int valueMatch = MatchNode(context,fromL+1,fromR+1) + StringDistance(context.stringVecL[fromL],context.stringVecR[fromR]);
	if (fromL == context.stringVecL.size())
	{
		for (size_t i = fromR; i < context.stringVecR.size(); ++i)
		{
			context.resR[i] = DiffType_Add;
		}
		return;
	}

	if (fromR == context.stringVecR.size())
	{
		for (size_t i = fromL; i < context.stringVecL.size(); ++i)
		{
			context.resL[i] = DiffType_Del;
		}
		return;
	}

	int value = context.cache[fromL][fromR];
	int valueKickA = context.stringVecL[fromL].length() + context.cache[fromL+1][fromR];
	int valueKickB = context.stringVecR[fromR].length() + context.cache[fromL][fromR+1];

	//if (cacheValue == valueMatch)
	//{
	//	context.resL[fromL]	= DiffType_Modify;
	//	context.resR[fromR]	= DiffType_Modify;
	//}
	if (value == valueKickA)
	{
		context.resL[fromL]	= DiffType_Del;
		GenMatchResult(context,fromL+1,fromR);
	}
	else if(value == valueKickB)
	{
		context.resR[fromR]	= DiffType_Add;
		GenMatchResult(context,fromL,fromR+1);
	}
	else
	{
		context.resL[fromL] = context.resR[fromR] = DiffType_Modify;
		GenMatchResult(context,fromL+1,fromR+1);
	}
}

void XmlDiff::RenderText()
{
	diffUIView->BeginEditBlock();
	DumpResult(diffResult,diffUIView,0);
	diffUIView->EndEditBlock();
}

void XmlDiff::FormatAttr( StringBuff& buff,const char* name, const char* value )
{
	buff.AppendStr(name);
	buff.AppendChar('=');
	buff.AppendChar('\"');
	buff.AppendStr(value);
	buff.AppendChar('"');
}



