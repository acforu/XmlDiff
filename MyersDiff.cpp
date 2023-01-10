#include "stdafx.h"
#include "MyersDiff.h"

std::vector<std::pair<int, int>> MyersDiff::LCSTrace(const std::vector<BiasArray>& VList, int k, int finalD)
{
	std::vector<std::pair<int, int>> ret;
	for (int d = finalD; d >= 0; --d)
	{
		int x = VList[d][k];
		int y = x - k;

		bool down = (k == -d || (k != d && VList[d - 1][k - 1] < VList[d - 1][k + 1]));
		if (down)
		{
			int prevX = (d == 0) ? 0 : VList[d - 1][k + 1];
			k = k + 1;

			while (x != prevX)
			{
				ret.push_back(std::make_pair(x, y));
				--x;
				--y;
			}
			continue;
		}
		else
		{
			int prevX = (d == 0) ? 0 : VList[d - 1][k - 1];
			int prevY = prevX - (k - 1);
			k = k - 1;
			while (y != prevY)
			{
				ret.push_back(std::make_pair(x, y));
				--x; --y;
			}
			continue;
		}
	}
	return ret;
}
