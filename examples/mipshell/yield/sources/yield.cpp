#include "yield.h"

Cyield::Cyield(const char* name): CProblem(name)
{
	m_pNode=0;
	m_pPlane=0;
	m_dpCost=0;
	readData(name);
}

#ifdef __THREADS_
Cyield::Cyield(const Cyield &other, int thread): CProblem(other,thread)
{
	m_iT=other.m_iT;
	m_iPlaneTypeNum=other.m_iPlaneTypeNum;
    m_pPlane=other.m_pPlane;
	m_iNodeNum=other.m_iNodeNum;
	m_iLeafNum=other.m_iLeafNum;
	m_pNode=other.m_pNode;
	m_iOptNum=other.m_iOptNum;
	m_dpCost=other.m_dpCost;
} // end of Cyield::Cyield(Cyield &other, int thread)

CMIP* Cyield::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cyield(*static_cast<Cyield*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cyield::~Cyield()
{
#ifdef __THREADS_
	if (!m_iThread) {
#endif
		if (m_pNode) {
			sNode *pNode=m_pNode;
			for (int i=1; i < m_iNodeNum; ++i, ++pNode) {
				delete[] pNode->demand;
			}
			delete[] m_pNode;
		}
		if (m_pPlane)
			delete[] m_pPlane;
		if (m_dpCost)
			delete[] m_dpCost;
#ifdef __THREADS_
	}
#endif
} // end of Cyield::~Cyield()

//////////////////// implementation
void Cyield::readParameters()
{
	std::ifstream fin("param.txt");
	if (!fin.is_open()) {
		throw new CFileException("Cyild::ReadParameters","param.txt");
	}
	
	int n;
	fin >> m_iT >> n;

	if (!(m_pPlane = new(std::nothrow) sPlane[m_iPlaneTypeNum=n])) {
		throw new CMemoryException("Cyild::ReadParameters");
	}

	sPlane *plane=m_pPlane;
	for (int k=0; k < n; ++k, ++plane) {
		fin >> plane->num >> plane->cost
	    	>> plane->q1 >> plane->r1
	    	>> plane->q2 >> plane->r2
	    	>> plane->q3 >> plane->r3;
	}
	fin.close();
} // end of Cyield::readParameters()

void Cyield::readOptions()
{
	std::ifstream fin("options.txt");
	if (!fin.is_open()) {
		throw new CFileException("Cyild::ReadOptions","options.txt");
	}
	
	int m;
	double *cost;
	fin >> m;
	m_iOptNum=m;
	if (!(m_dpCost=cost=new(std::nothrow) double[3*m*m_iT])) {
		fin.close();
		throw new CMemoryException("Cyild::ReadOptions");
	}

	m*=(3*m_iT);
	for (int i=0; i < m; ++i) {
		fin >> cost[i];
	}
	
	fin.close();
} // end of Cyield::readOptions()

void Cyield::readTree()
{
	std::ifstream fin("tree.txt");
	if (!fin.is_open()) {
		throw new CFileException("Cyild::ReadTree","tree.txt");
	}
	fin >> m_iNodeNum;
	if (!(m_pNode = new(std::nothrow) sNode[++m_iNodeNum])) {
		throw new CMemoryException("Cyild::ReadTree (1)");
	}
	m_pNode[0].ind=0;
	m_pNode[0].parent=-1;
	m_pNode[0].prob=1;
	m_pNode[0].period=0;
	m_pNode[0].demand=0;
	double prob;
	int nodeNum, leafNum, n, *d;
	sNode *pNode=m_pNode+1;
	leafNum=0;
	nodeNum=m_iNodeNum;
	for (int t,j=1; j < nodeNum; ++j, ++pNode) {
		fin >> pNode->ind >> pNode->parent >> prob;
		pNode->period=t=m_pNode[pNode->parent].period+1;
		pNode->prob=m_pNode[pNode->parent].prob * prob;
		if (t == m_iT)
			++leafNum;
		n=m_iOptNum;
		if (!(pNode->demand=d=new(std::nothrow) int[3*n])) {
			fin.close();
			throw new CMemoryException("Cyild::ReadTree (2)");
		}
		n*=3;
		for (int i=0; i < n; ++i)
			fin >> d[i];		
	}
	m_iLeafNum=leafNum;
	fin.close();
} // end of Cyield::readTree()

void Cyield::readData(const char* name)
{
	readParameters();
	readOptions();
	readTree();
} // end of Cyield::readData()


#include "yield.aux"

