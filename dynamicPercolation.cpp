#include "dynamicPercolation.h"

using namespace std;

DynCreation::DynCreation()
{
	clusNumber = -1;
	noEdges = 0;
	threemax = new int[3]{0};
	tmp_i = 0;
}

// Adds nodes to the system and increases the number of edges
void DynCreation::insertEdge(string p, string q)
{
	nodes.insert(p);	n1 = p;
	nodes.insert(q);	n2 = q;
	noEdges++;
}

// Find the size of the three largest clusters
// The three largest numbers from clDist (Distribution of sizes)
void DynCreation::calculateMaxClusters()
{
	threemax = new int[3]{0};
	map<int,int>::reverse_iterator rit = clDist.rbegin();
	int i = 0;
	while( (i<3)  and (rit!=clDist.rend()) )
	{
		for ( int j=0; j<rit->second; ++j )
		{
			threemax[i] = rit->first;
			i++;
		}
		++rit;
	}
}

// If none of the nodes on the link exist
void DynCreation::noneExists(string p, string q)
{
	clusNumber++;			// Create new cluster number
	nocl[p] = clusNumber;	// Insert both nodes in the cluster
	nocl[q] = clusNumber;
	clSize[clusNumber] += 2;// Increase the size of the cluster by 2
	int tmpClSizeNocl = clSize[clusNumber];
	tmp_i += (long long int)tmpClSizeNocl*(long long int)tmpClSizeNocl;
	if ( clDist.find(tmpClSizeNocl) == clDist.end() )	clDist[tmpClSizeNocl] = 1;
	else	clDist[tmpClSizeNocl] += 1;
}

// If only the first node exists
void DynCreation::onlyPExists(string p, string q)
{
	nocl[q] = nocl[p]; // Cited and citing nodes belong to the same cluster
	int tmpClSizeNocl = clSize[nocl[p]];
	// Decrease the quantity of previous size from distribution
	clDist[tmpClSizeNocl] -= 1;
	if(clDist[tmpClSizeNocl]==0)    clDist.erase(tmpClSizeNocl);
	tmp_i -= (long long int)tmpClSizeNocl*(long long int)tmpClSizeNocl;
	// Increase cluster size by 1
	clSize[nocl[p]] += 1;
	tmpClSizeNocl = clSize[nocl[p]];
	tmp_i += (long long int)tmpClSizeNocl*(long long int)tmpClSizeNocl;
	clDist[tmpClSizeNocl] += 1;
}

// If only the second node exists
void DynCreation::onlyQExists(string p, string q)
{
	nocl[p]=nocl[q];    // Cited and citing nodes belong to the same cluster
	int tmpClSizeNocl = clSize[nocl[q]];
	clDist[tmpClSizeNocl] -= 1;
	if(clDist[tmpClSizeNocl]==0)    clDist.erase(tmpClSizeNocl);
	tmp_i -= (long long int)tmpClSizeNocl*(long long int)tmpClSizeNocl;
	// Increase cluster size by 1
	clSize[nocl[q]] += 1;
	tmpClSizeNocl = clSize[nocl[q]];
	tmp_i += (long long int)tmpClSizeNocl*(long long int)tmpClSizeNocl;
	clDist[tmpClSizeNocl] += 1;
}

// If both nodes exist
void DynCreation::bothExist(string p, string q)
{
	// Check if both nodes belong already to the same cluster
	if( nocl[q] == nocl[p] )    return;
	int temp1=nocl[p];
	int temp2=nocl[q];
	int tmp1ClSizeNocl = clSize[temp1];
	tmp_i -= (long long int)tmp1ClSizeNocl*(long long int)tmp1ClSizeNocl;
	clDist[tmp1ClSizeNocl] -= 1;
	if(clDist[tmp1ClSizeNocl]==0)    clDist.erase(tmp1ClSizeNocl);
	int tmp2ClSizeNocl = clSize[temp2];
	tmp_i -= (long long int)tmp2ClSizeNocl*(long long int)tmp2ClSizeNocl;
	clDist[tmp2ClSizeNocl] -= 1;
	if(clDist[tmp2ClSizeNocl]==0)    clDist.erase(tmp2ClSizeNocl);
	// Merge the two clusters, keeping(removing) the largest(smallest)
	if( tmp1ClSizeNocl >= tmp2ClSizeNocl )
	{
		for(auto& x: nocl)
		{
			if(x.second == temp2)
			{
				x.second = temp1;
				clSize[temp2]--;
				clSize[temp1]++;
			}
		}
	}
	else
	{
		for(auto& x: nocl)
		{
			if(x.second == temp1)
			{
				x.second = temp2;
				clSize[temp1]--;
				clSize[temp2]++;
			}
		}
	}
	int tmpClSizeNocl = (tmp1ClSizeNocl >= tmp2ClSizeNocl) ? clSize[temp1] : clSize[temp2];
	tmp_i += (long long int)tmpClSizeNocl*(long long int)tmpClSizeNocl;
	if ( clDist.find(tmpClSizeNocl) == clDist.end() )	clDist[tmpClSizeNocl] = 1;
	else	clDist[tmpClSizeNocl] += 1;
}

// Calculate reduced average cluster size
double DynCreation::idot()
{
	int maxSize = threemax[0];
	return (tmp_i - (long long int)maxSize*(long long int)maxSize) / (double)nodes.size();
}

// Console print
void DynCreation::printState()
{
	calculateMaxClusters();
	string firstthree = to_string(threemax[0]) + " " + to_string(threemax[1]) + " " + to_string(threemax[2]);
	cout<<firstthree<<" "<<idot()<<" "<<nodes.size()<<" "<<noEdges<<" "<<n1<<" "<<n2<<endl;
}

// File print
void DynCreation::printState(ofstream& exf)
{
	calculateMaxClusters();
	string firstthree = to_string(threemax[0]) + " " + to_string(threemax[1]) + " " + to_string(threemax[2]);
	exf<<firstthree<<" "<<idot()<<" "<<nodes.size()<<" "<<noEdges<<" "<<n1<<" "<<n2<<endl;
}

// Main process
void DynCreation::mProcess(string p, string q)
{
	bool pExists = nodes.find(p) != nodes.end();
	bool qExists = nodes.find(q) != nodes.end();
	if( !pExists and !qExists )			noneExists(p, q);
	else if( pExists and !qExists )		onlyPExists(p, q);
	else if( !pExists and qExists )		onlyQExists(p, q);
	else if( pExists and qExists )		bothExist(p, q);
	insertEdge(p,q);
}
