#include "stdafx.h"
#include "XmlHelper.h"
#include <iosfwd>
#include <cmath>

using namespace std;

size_t StringDistance(const std::string_view s1, const std::string_view s2, int maxDistance)
{
	//return StringDistSift4(s1,s2);
	//int a = sift4(s1, s2);
	//int b = StringDistSift4(s1, s2);
	//if (a != b)
	//{
	//	assert(0);
	//}
	//assert(sift4(s1, s2) == StringDistSift4(s1, s2));
	return sift4(s1, s2, 5, maxDistance);
}

void split(const string &s, char delim, vector<string> &elems) {
	//stringstream ss(s);
	//string item;
	//while (getline(ss, item, delim)) {
	//	elems.push_back(item);
	//}

	for (size_t i = 0,beg = 0; i < s.length(); ++i)
	{
		if (s[i]==delim)
		{
			elems.push_back(s.substr(beg,i-beg));
			beg = i + 1;
		}
		if (i == s.length() - 1)
		{
			elems.push_back(s.substr(beg,i-beg));
		}
	}
}


void split(const string_view s, char delim, vector<string_view>& elems) {
	for (size_t i = 0, beg = 0; i < s.length(); ++i)
	{
		if (s[i] == delim)
		{
			elems.push_back(s.substr(beg, i - beg));
			beg = i + 1;
		}
		if (i == s.length() - 1)
		{
			elems.push_back(s.substr(beg, i - beg));
		}
	}
}


size_t StringDistBasedLine(rapidxml::xml_node<>* s1, rapidxml::xml_node<>* s2 )
{
	//vector<string_view> stringL,stringR;
	//split(s1->get_stringview(), '\n', stringL);
	//split(s2->get_stringview(), '\n', stringR);

	//the first attr is almost id or key, and the modification of the key is less often, so we give the nodes share the same key with more weight of similarity
	bool firstAttrIsSame = false;
	if (string_view(s1->name(),s1->name_size()) == string_view(s2->name(),s2->name_size()))
	{
		if (s1->first_attribute() && s2->first_attribute())
		{
			if (string_view(s1->first_attribute()->name(), s1->first_attribute()->name_size()) == string_view(s2->first_attribute()->name(), s2->first_attribute()->name_size()) &&
				string_view(s1->first_attribute()->value(), s1->first_attribute()->value_size()) == string_view(s2->first_attribute()->value(), s2->first_attribute()->value_size()))
			{
				firstAttrIsSame = true;
			}			
		}
	}

	std::string_view const s1view = (s1->name_size() == 0) ? std::string_view(s1->value(), s1->value_size()) : s1->get_stringview();
	std::string_view const s2view = (s2->name_size() == 0) ? std::string_view(s2->value(), s2->value_size()) : s2->get_stringview();

	return StringDistance(s1view, s2view,20) * (firstAttrIsSame ? 0.75 : 1);
	
	//#todo make sure move right value
	//return DiffLines(stringL,stringR) * firstAttrIsSame?0.75:1;
}


int DiffLines(std::vector<string_view> nodeLVec, std::vector<string_view> nodeRVec)
{
	int ret = 0;
	
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
			while ( xEnd < N && yEnd < M && (nodeLVec[xEnd] == nodeRVec[yEnd])) { xEnd++; yEnd++; snake++; }

			// save end point
			currV[k] = xEnd;

			// check for solution
			if ( xEnd >= N && yEnd >= M ) /* solution has been found */
			{
				std::list<std::pair<int,int>> trace = LCSTrace(VList,k,d);
				ret = CalcStringDist(trace,nodeLVec,nodeRVec);
				goto end;
			}
		}
	}

end:
	return ret;
}


std::list<std::pair<int,int>> LCSTrace( const vector<BiasArray>& VList, int k, int finalD)
{
	std::list<std::pair<int,int>> ret;
	for (int d = finalD ; d >= 0; --d)
	{
		int x = VList[d][k];
		int y = x - k;

		bool down = ( k == -d || ( k != d && VList[d-1][k-1] < VList[d-1][k+1] ) );
		if (down)
		{
			int prevX = (d==0)?0:VList[d-1][k+1];
			k = k + 1;

			while(x != prevX)
			{
				ret.push_back(std::make_pair(x,y));
				--x;
				--y;
			}
			continue;
		}
		else
		{
			int prevX = (d==0)?0:VList[d-1][k-1];
			int prevY = prevX - (k-1);
			k = k - 1;
			while(y != prevY)
			{
				ret.push_back(std::make_pair(x,y));
				--x;--y;
			}
			continue;
		}
	}
	return ret;
}

int CalcStringDist( const std::list<std::pair<int,int>>& trace,const std::vector<string_view>& nodeLVec, const std::vector<string_view>& nodeRVec)
{
	int ret = 0;

	size_t indexL = 0 , indexR = 0;
	for(auto iter = trace.rbegin(); iter != trace.rend(); ++iter)
	{
		size_t sameIndexL = iter->first - 1;
		size_t sameIndexR = iter->second - 1;

		string nodesL;
		for (; indexL < sameIndexL; ++indexL)
		{
			nodesL.append(nodeLVec[indexL]);
		}

		string nodesR;
		for (; indexR < sameIndexR; ++indexR)
		{
			nodesR.append(nodeRVec[indexR]);
		}

#if SIMPLIFY_STRING_DIST_CALC 
		ret += StringDistSimplify(nodesL,nodesR);
#else
		ret += StringDistance(nodesL,nodesR);
#endif
		++indexL;
		++indexR;
	}

	string nodesL;
	for(;indexL < nodeLVec.size(); ++indexL)
	{
		nodesL.append(nodeLVec[indexL]);
	}
	string nodesR;
	for(;indexR < nodeRVec.size(); ++indexR)
	{
		nodesR.append(nodeRVec[indexR]);
	}

#if SIMPLIFY_STRING_DIST_CALC 
	ret += StringDistSimplify(nodesL,nodesR);
#else
	ret += StringDistance(nodesL,nodesR);
#endif
	return ret;
}

char* CopyString( const char *begin, char* out )
{
	while (*begin != '\0')
		*out++ = *begin++;
	return out;
}

int GetEnterCount( const char *begin )
{
	int ret = 0;
	while (*begin != '\0')
	{
		if(*begin++ == '\n')
			++ret;
	}
	return ret;
}

size_t StringDistSimplify( const std::string &s1, const std::string &s2 )
{
	return s1.size() + s2.size();
}

Profiler::Profiler()
{
	QueryPerformanceFrequency(&frequency);
	Start();
}

Profiler::~Profiler()
{
	Stop();
}

void Profiler::Stop()
{
	QueryPerformanceCounter(&t2);
	elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart / 1000;
	cout.precision(6);
	cout << "cost time: " << std::fixed << elapsedTime * 1000 << "ms" << endl;
}

void Profiler::Start()
{
	QueryPerformanceCounter(&t1);
}

namespace
{
	double round(double d)
	{
		return floor(d + 0.5);
	}
}

// Sift4 - simplest version
// online algorithm to compute the distance between two strings in O(n)
// maxOffset is the number of characters to search for matching letters

//https://siderite.dev/blog/super-fast-and-accurate-string-distance.html/#at1214062680

size_t StringDistSift4(const std::string_view s1, const std::string_view s2, int maxOffset)
{
	const int s1_size = s1.length();
	const int s2_size = s2.length();

	if (s1.empty() || !s1_size) {
		if (s2.empty()) {
			return 0;
		}
		return s2_size;
	}

	if (s2.empty() || !s2_size) {
		return s1_size;
	}

	int l1=s1.size();
	int l2=s2.size();

	int c1 = 0;  //cursor for string 1
	int c2 = 0;  //cursor for string 2
	int lcss = 0;  //largest common subsequence
	int local_cs = 0; //local common substring

	while ((c1 < l1) && (c2 < l2)) {
		if (s1[(c1)] == s2[(c2)]) {
			local_cs++;
		} else {
			lcss+=local_cs;
			local_cs=0;
			if (c1!=c2) {
				c1=c2=max(c1,c2); //using max to bypass the need for computer transpositions ('ab' vs 'ba')
			}
			for (int i = 0; i < maxOffset && (c1+i<l1 || c2+i<l2); i++) {
				if ((c1 + i < l1) && (s1[(c1 + i)] == s2[(c2)])) {
					c1+= i;
					local_cs++;
					break;
				}
				if ((c2 + i < l2) && (s1[(c1)] == s2[(c2 + i)])) {
					c2+= i;
					local_cs++;
					break;
				}
			}
		}
		c1++;
		c2++;
	}
	lcss+=local_cs;
	return round(max(l1,l2)- lcss);
}


struct sift_offset {
	int c1;
	int c2;
	bool trans;
};


size_t sift4(const std::string_view s1, const std::string_view s2, int maxOffset, int maxDistance)
{
	const int s1_size = s1.length();
	const int s2_size = s2.length();

	if (s1.empty() || !s1_size) {
		if (s2.empty()) {
			return 0;
		}
		return s2_size;
	}

	if (s2.empty() || !s2_size) {
		return s1_size;
	}

	int l1 = s1_size;
	int l2 = s2_size;

	int c1 = 0; //cursor for string 1
	int c2 = 0; //cursor for string 2
	int lcss = 0; //largest common subsequence
	int local_cs = 0; //local common substring
	int trans = 0; //number of transpositions ('ab' vs 'ba')
	std::vector < sift_offset > offset_arr; //offset pair array, for computing the transpositions
	offset_arr.reserve(10);

	while ((c1 < l1) && (c2 < l2)) {
		if (s1[c1] == s2[c2]) {
			local_cs++;
			bool isTrans = false;
			//see if current match is a transposition
			int i = 0;
			while (i < offset_arr.size()) {
				sift_offset ofs = offset_arr[i];
				if (c1 <= ofs.c1 || c2 <= ofs.c2) {
					// when two matches cross, the one considered a transposition is the one with the largest difference in offsets
					isTrans = std::abs(c2 - c1) >= std::abs(ofs.c2 - ofs.c1);

					if (isTrans) {
						trans++;
					}
					else {
						if (!ofs.trans) {
							ofs.trans = true;
							trans++;
						}
					}
					break;
				}
				else {
					if (c1 > ofs.c2 && c2 > ofs.c1) {
						offset_arr.erase(offset_arr.begin() + i);

					}
					else {
						i++;
					}
				}
			}
			offset_arr.push_back({
			  c1,
			  c2,
			  isTrans
				});

			static int s = 0;
			if (offset_arr.size() > s)
			{
				s = offset_arr.size();
				cout << "offset_arr.size() " << offset_arr.size() << endl;
			}

		}
		else {
			lcss += local_cs;
			local_cs = 0;
			if (c1 != c2) {
				c1 = c2 = (int)min(c1, c2); //using min allows the computation of transpositions
			}
			//if matching characters are found, remove 1 from both cursors (they get incremented at the end of the loop)
			//so that we can have only one code block handling matches 
			for (int i = 0; i < maxOffset && (c1 + i < l1 || c2 + i < l2); i++) {
				if ((c1 + i < l1) && (s1[c1 + i] == s2[c2])) {
					c1 += i - 1;
					c2--;
					break;
				}
				if ((c2 + i < l2) && (s1[c1] == s2[c2 + i])) {
					c1--;
					c2 += i - 1;
					break;
				}
			}
		}
		c1++;
		c2++;
		if (maxDistance) {
			int temporaryDistance = (int)max(c1, c2) - lcss + trans;
			if (temporaryDistance >= maxDistance)
			{
				return std::round(temporaryDistance);
			}
		}

		// this covers the case where the last match is on the last token in list, so that it can compute transpositions correctly
		if ((c1 >= l1) || (c2 >= l2)) {
			lcss += local_cs;
			local_cs = 0;
			c1 = c2 = (int)min(c1, c2);
		}
	}
	lcss += local_cs;
	return std::round((int)max(l1, l2) - lcss + trans); //add the cost of transpositions to the final result
}



void Print2Buff( rapidxml::xml_node<> *node, StringBuff& buff )
{
	if (node->get_text_beg()!=nullptr && node->get_text_end()!=nullptr)
	{
		buff.AppendStr(node->get_text_beg(),node->get_text_end());
	}
	else
	{
		print(buff.Begin(),*node);
	}
}

bool CompareData( const char* begL,const char* endL,const char* begR,const char* endR )
{
	size_t length = endL - begL;
	if (endR - begR != length)
	{
		return false;
	}

	for (size_t i = 0; i < length ; ++i)
	{
		if (begL[i]!=begR[i])
		{
			return false;
		}
	}
	return true;
}

size_t DataDistance( const std::vector<char>& s1, const std::vector<char>& s2 )
{
	const size_t m(s1.size());
	const size_t n(s2.size());

	if( m==0 ) return n;
	if( n==0 ) return m;

	if (abs(int(m-n)) > 60)
	{
		return abs(int(m+n));
	}

	size_t *costs = new size_t[n + 1];

	for( size_t k=0; k<=n; k++ ) costs[k] = k;

	size_t i = 0;
	for ( auto it1 = s1.begin(); it1 != s1.end(); ++it1, ++i )
	{
		costs[0] = i+1;
		size_t corner = i;

		size_t j = 0;
		for ( auto it2 = s2.begin(); it2 != s2.end(); ++it2, ++j )
		{
			size_t upper = costs[j+1];
			if( *it1 == *it2 )
			{
				costs[j+1] = corner;
			}
			else
			{
				size_t t(upper<corner?upper:corner);
				costs[j+1] = (costs[j]<t?costs[j]:t)+1;
			}

			corner = upper;
		}
	}

	size_t result = costs[n];
	delete [] costs;

	//qDebug() << "result " << result << endl;
	//qDebug() << "swift " << StringDistSift4(s1,s2) << endl;

	return result;
}
