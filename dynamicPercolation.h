#ifndef DYNAMICPERCOLATION_H_
#define DYNAMICPERCOLATION_H_

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

class DynCreation
{
	private:
		int clusNumber, noEdges, *threemax;
		long long int tmp_i;		// Temporary Variable for reduced average cluster size
		map<string, int> nocl;	// Number of cluster for each node
		map<int, int> clSize;		// Size of each cluster
		set<string> nodes;
		string n1, n2;
		void changeMax(int number);
		void insertEdge(string, string);
		void noneExists(string, string);
		void onlyPExists(string, string);
		void onlyQExists(string, string);
		void bothExist(string, string);
		double idot();
	public:
		DynCreation();	// Constructor
		void printState();
		void printState(ofstream&);
		void mProcess(string, string);
};
#endif
