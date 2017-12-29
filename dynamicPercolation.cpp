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

// Check if "number" is one of the three largest clusters
void DynCreation::changeMax(int number)
{
	if( threemax[0] < number )	threemax[0] = number;
    else if( threemax[1] < number )	threemax[1] = number;
    else if( threemax[2] < number )	threemax[2] = number;
}

// If none of the nodes on the link exist
void DynCreation::noneExists(string p, string q)
{
	clusNumber++;	// Create new cluster
	nocl[p] = clusNumber;
	nocl[q] = clusNumber;
	clSize[clusNumber] += 2;
	int tmpClSizeNocl = clSize[clusNumber];
	tmp_i += (long long int)tmpClSizeNocl*(long long int)tmpClSizeNocl;
  changeMax(tmpClSizeNocl);
}

// If only the first node exists
void DynCreation::onlyPExists(string p, string q)
{
	nocl[q] = nocl[p]; // Cited and citing nodes belong to the same cluster
    int tmpClSizeNocl = clSize[nocl[p]];
    tmp_i -= (long long int)tmpClSizeNocl*(long long int)tmpClSizeNocl;
    // Increase cluster size by 1
    clSize[nocl[p]] += 1;
    tmpClSizeNocl = clSize[nocl[p]];
    tmp_i += (long long int)tmpClSizeNocl*(long long int)tmpClSizeNocl;
    changeMax(tmpClSizeNocl);
}

// If only the second node exists
void DynCreation::onlyQExists(string p, string q)
{
	nocl[p]=nocl[q];    // Cited and citing nodes belong to the same cluster
    int tmpClSizeNocl = clSize[nocl[q]];
    tmp_i -= (long long int)tmpClSizeNocl*(long long int)tmpClSizeNocl;
    // Increase cluster size by 1
    clSize[nocl[q]] += 1;
    tmpClSizeNocl = clSize[nocl[q]];
    tmp_i += (long long int)tmpClSizeNocl*(long long int)tmpClSizeNocl;
    changeMax(tmpClSizeNocl);
}

// If both nodes exist
void DynCreation::bothExist(string p, string q)
{
	// Check if both nodes belong already to the same cluster
    if( nocl[q] == nocl[p] )    return;
    int temp1=nocl[p];
    int temp2=nocl[q];
    int tmpClSizeNocl = clSize[temp1];
    tmp_i -= (long long int)tmpClSizeNocl*(long long int)tmpClSizeNocl;
    tmpClSizeNocl = clSize[temp2];
    tmp_i -= (long long int)tmpClSizeNocl*(long long int)tmpClSizeNocl;
	int temp3 = 0, temp4 = 0;
	// Merge the two clusters, keeping(removing) the largest(smallest)
	if( clSize[temp1] >= clSize[temp2] )
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
        temp3 = temp1;	// Largest
        temp4 = temp2;
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
        temp3 = temp2;	// Largest
        temp4 = temp1;
	}
    tmpClSizeNocl = clSize[temp3];
    tmp_i += (long long int)tmpClSizeNocl*(long long int)tmpClSizeNocl;
    changeMax(tmpClSizeNocl);
    clSize.erase(temp4);
}

// Calculate reduced average cluster size
double DynCreation::idot()
{
	int maxSize = threemax[0];
	return (tmp_i - (long long int)maxSize*(long long int)maxSize) / (double)nodes.size();
}

void DynCreation::printState()
{
	if( threemax[2] == 0 )	return;
	string firstthree = to_string(threemax[0]) + " " + to_string(threemax[1]) + " " + to_string(threemax[2]);
	cout<<firstthree<<" "<<idot()<<" "<<nodes.size()<<" "<<noEdges<<" "<<n1<<" "<<n2<<endl;
}

void DynCreation::printState(ofstream& exf)
{
	if( threemax[2] == 0 )	return;
	string firstthree = to_string(threemax[0]) + " " + to_string(threemax[1]) + " " + to_string(threemax[2]);
	exf<<firstthree<<" "<<idot()<<" "<<nodes.size()<<" "<<noEdges<<" "<<n1<<" "<<n2<<endl;
}

void DynCreation::mProcess(string p, string q)
{
	insertEdge(p,q);
	bool pExists = nocl.find(p) != nocl.end();
	bool qExists = nocl.find(q) != nocl.end();
	if( !pExists and !qExists )		noneExists(p, q);
	else if( pExists and !qExists )	onlyPExists(p, q);
	else if( !pExists and qExists )	onlyQExists(p, q);
	else if( pExists and qExists )		bothExist(p, q);
}
