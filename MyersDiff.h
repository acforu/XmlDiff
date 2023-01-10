#pragma once
#include "Common.h"
#include <functional>
#include <vector>

using namespace std;

namespace MyersDiff
{
	template<typename T>
	struct DiffBlock
	{
		DiffType type;
		std::vector<T> first;
		std::vector<T> second;
	};

	template<typename T>
	struct DiffResult
	{
		void Add(std::vector<T>&& t)
		{
			results.emplace_back(DiffBlock{DiffType_Add , std::move(t)});
		}

		void Del(std::vector<T>&& t)
		{
			results.emplace_back(DiffBlock{DiffType_Del , std::move(t)});
		}

		void Modify(std::vector<T>&& prev, std::vector<T>&& next)
		{
			results.emplace_back(DiffBlock{DiffType_Modify , std::move(prev), std::move(next)});
		}

		void Unchange(std::vector<T>&& t)
		{
			results.emplace_back(DiffBlock<T>{ DiffType_Unchanged , std::move(t)});
		}
		
		void Handle(std::vector<T>&& prev, std::vector<T>&& next)
		{
			if (prev.size() > 0 && next.size() > 0)
			{
				Modify(std::move(prev),std::move(next));
			}
			else if (prev.size() > 0 && next.size() == 0)
			{
				Del(std::move(prev));
			}
			else if (prev.size() == 0 && next.size() > 0)
			{
				Add(std::move(next));
			}
			else
			{
				
			}
		}


		std::vector<DiffBlock<T>> results;
	};



	template<typename T>
	DiffResult<T> DiffNodes(const std::vector<T>& nodeLVec, const std::vector<T>& nodeRVec, std::function<bool(const T&, const T&)> CompareNodeFunc);

	std::vector<std::pair<int, int>> LCSTrace(const std::vector<BiasArray>& VList, int k, int finalD);

	template<typename T>
	DiffResult<T> ConvertTrace(const std::vector<std::pair<int, int>>& trace, const std::vector<T>& nodeLVec, const std::vector<T>& nodeRVec);

	template<typename T>
	DiffResult<T> DiffNodes(const std::vector<T>& nodeLVec, const std::vector<T>& nodeRVec, std::function<bool(const T&,const T&)> CompareNodeFunc)
	{
		DiffResult<T> ret;

		int N = nodeLVec.size();
		int M = nodeRVec.size();
		int maxD = N + M;
		vector<BiasArray> VList;

		BiasArray stub(3);
		//stub.assign(2,0);
		//V[ 1 ] = 0;
		//VList.push_back(V);

		for (int d = 0; d <= maxD; d++)
		{
			BiasArray prevV = (d == 0) ? stub : VList.back();

			{
				BiasArray currV(max(d * 2 + 1, 3));
				//currV.assign(d*2 + 1,0);
				//V[ 1 ] = 0;
				VList.push_back(currV);
			}

			BiasArray& currV = VList.back();


			for (int k = -d; k <= d; k += 2)
			{
				// down or right?
				bool down = (k == -d || (k != d && prevV[k - 1] < prevV[k + 1]));

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
				while (xEnd < N && yEnd < M && CompareNodeFunc(nodeLVec[xEnd], nodeRVec[yEnd])) { xEnd++; yEnd++; snake++; }

				// save end point
				currV[k] = xEnd;

				// check for solution
				if (xEnd >= N && yEnd >= M) /* solution has been found */
				{
					auto trace = MyersDiff::LCSTrace(VList, k, d);
					ret = MyersDiff::ConvertTrace<T>(trace, nodeLVec, nodeRVec);
					goto end;
				}
			}
		}

	end:
		return ret;
	}



	template<typename T>
	DiffResult<T> ConvertTrace(const std::vector<std::pair<int, int>>& trace, const std::vector<T>& nodeLVec, const std::vector<T>& nodeRVec)
	{
		DiffResult<T>  ret;

		size_t indexL = 0, indexR = 0;
		for (auto iter = trace.rbegin(); iter != trace.rend(); ++iter)
		{
			size_t sameIndexL = iter->first - 1;
			size_t sameIndexR = iter->second - 1;

			{
				vector<T> nodesL;
				for (; indexL < sameIndexL; ++indexL)
				{
					nodesL.push_back(nodeLVec[indexL]);
				}

				vector<T> nodesR;
				for (; indexR < sameIndexR; ++indexR)
				{
					nodesR.push_back(nodeRVec[indexR]);
				}

				ret.Handle(std::move(nodesL),std::move(nodesR));
			}

			
			ret.Unchange({ nodeLVec.at(indexL)});

			++indexL;
			++indexR;
		}


		{
			vector<T> nodesL;
			for (; indexL < nodeLVec.size(); ++indexL)
			{
				nodesL.push_back(nodeLVec[indexL]);
			}
			vector<T> nodesR;
			for (; indexR < nodeRVec.size(); ++indexR)
			{
				nodesR.push_back(nodeRVec[indexR]);
			}

			ret.Handle(std::move(nodesL),std::move(nodesR));
		}
		return ret;
	}

}

