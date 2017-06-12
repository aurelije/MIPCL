#include "fcnf.h"
#include <fstream>
#include <cstring>

Cfcnf::Cfcnf(const char* name): CProblem(name)
{
	m_ipTail=m_ipHead=0;
	m_dpCapacity=m_dpFixedCost=m_dpCost=m_dpDemand=0;
	char fileName[128];
	strncpy(fileName,name,123);
	strcat(fileName,".txt");
	readNet(fileName);
}

#ifndef __ONE_THREAD_
Cfcnf::Cfcnf(const Cfcnf &other, int thread): CProblem(other,thread)
{
} // end of Cfcnf::Cfcnf(Cfcnf &other, int thread)

CMIP* Cfcnf::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cfcnf(*static_cast<Cfcnf*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cfcnf::~Cfcnf()
{
#ifndef __ONE_THREAD_
	if (!m_iThread) {
#endif
		if (m_ipTail)
			delete[] m_ipTail;
		if (m_dpCapacity)
			delete[] m_dpCapacity; 
#ifndef __ONE_THREAD_
	}
#endif
} // end of Cfcnf::~Cfcnf

void Cfcnf::readNet(const char* fileName)
{
	int n,m;
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw CFileException("Cfcnf::ReadNet",fileName);
	}
	fin >> n >> m;
	m_iVertNum=n; m_iEdgeNum=m;
	if (m_ipTail = new(std::nothrow) int[2*m])
		m_dpCapacity = new(std::nothrow) double[3*m+n];
	if (!m_dpCapacity) {
		if (!m_ipTail)
			delete[] m_ipTail;
		throw CMemoryException("Cfcnf::ReadNet");
	}
	m_dpDemand=(m_dpCost=(m_dpFixedCost=m_dpCapacity+m)+m)+m;
	m_ipHead = m_ipTail+m;
	
	for (int i=0; i < n; ++i) {
		fin >> m_dpDemand[i];
	}
	for (int i=0; i < m; ++i) {
		fin >> m_ipTail[i] >> m_ipHead[i] >> m_dpCapacity[i] 
			>> m_dpFixedCost[i] >> m_dpCost[i];
	}
	fin.close();
} // end of Cfcnf::readNet  

void Cfcnf::printSolution(VAR_VECTOR &flow)
{
	int m=m_iEdgeNum;
	char fileName[128];
	getProblemName(fileName);
	std::strcat(fileName,".sol");
	std::ofstream fout(fileName);
	if (isSolution()) {
		fout << "Nonzero flows:\n";
		for(int e=0; e < m; ++e) {
			if (getval(flow(e)) > 0.5)  {
				fout << "flow(" << m_ipTail[e] << "," 
					<< m_ipHead[e] << ")="
					<< getval(flow(e)) << std::endl;
			}
		}
	}
	else {
		fout << "Problem has no solution!\n";
	}
	fout.close();
} // end of Cfcnf::printSolution

//////////////////// implementation

#include "fcnf.aux"

