#include "stdafx.h"
#include "XmlHelper.h"
#include <iosfwd>


using namespace std;

size_t StringDistance(const std::string &s1, const std::string &s2)
{
	const size_t m(s1.size());
	const size_t n(s2.size());

	if( m==0 ) return n;
	if( n==0 ) return m;

	size_t *costs = new size_t[n + 1];

	for( size_t k=0; k<=n; k++ ) costs[k] = k;

	size_t i = 0;
	for ( std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i )
	{
		costs[0] = i+1;
		size_t corner = i;

		size_t j = 0;
		for ( std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j )
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

	return result;
}

void split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
}

size_t StringDistBasedLine( const std::string &s1, const std::string &s2 )
{
	vector<string> stringL,stringR;
	split(s1,'\n',stringL);
	split(s2,'\n',stringR);

	return DiffLines(stringL,stringR);
}


int DiffLines( const std::vector<string>& nodeLVec, const std::vector<string>& nodeRVec)
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

int CalcStringDist( const std::list<std::pair<int,int>>& trace,const std::vector<string>& nodeLVec, const std::vector<string>& nodeRVec)
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

		ret += StringDistance(nodesL,nodesR);
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

	ret += StringDistance(nodesL,nodesR);

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
