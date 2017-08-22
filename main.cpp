#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <algorithm>
#include "dem.h"
#include "Node.h"
#include "utils.h"
#include <time.h>
#include <list>
#include <unordered_map>

using namespace std;
using std::cout;
using std::endl;
using std::string;
using std::getline;
using std::fstream;
using std::ifstream;
using std::priority_queue;
using std::binary_function;


typedef std::vector<Node> NodeVector;
typedef std::priority_queue<Node, NodeVector, Node::Greater> PriorityQueue;


void FillDEM(char* inputFile, char* outputFilledPath);
//compute stats for a DEM
void calculateStatistics(const CDEM& dem, double* min, double* max, double* mean, double* stdDev)
{
	int width = dem.Get_NX();
	int height = dem.Get_NY();

	int validElements = 0;
	double minValue, maxValue;
	double sum = 0.0;
	double sumSqurVal = 0.0;
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			if (!dem.is_NoData(row, col))
			{
				double value = dem.asFloat(row, col);
				
				if (validElements == 0)
				{
					minValue = maxValue = value;
				}
				validElements++;
				if (minValue > value)
				{
					minValue = value;
				}
				if (maxValue < value)
				{
					maxValue = value;
				}

				sum += value;
				sumSqurVal += (value * value);
			}
		}
	}

	double meanValue = sum / validElements;
	double stdDevValue = sqrt((sumSqurVal / validElements) - (meanValue * meanValue));
	*min = minValue;
	*max = maxValue;
	*mean = meanValue;
	*stdDev = stdDevValue;
}


int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout<<"Fill DEM usage: fill inputfileName outputfileName"<<endl;
		cout<<"\nFor example, fill dem.tif dem_filled.tif"<<endl;
		return 1;
	}
	
	string path(argv[1]);
	string outputFilledPath(argv[2]);
	size_t index = path.find(".tif");
	if (index ==string::npos) {
		cout<<"Input file name should have an extension of '.tif'"<<endl;
		return 1;
	}
	char* method=argv[1];
	string strFolder = path.substr(0, index);
	FillDEM(&path[0], &outputFilledPath[0]); 

	return 0;
}

