#include "unitcom.h"
#include <cstring>
#include <cmath>
#include <iomanip>

Cunitcom::Cunitcom(const char* name): CProblem(name)
{
	m_ipDemand=0;
	char fileName[128];
	strcpy(fileName,name);
	strcat(fileName,".txt");
	readData(fileName);
}

#ifndef __ONE_THREAD_
Cunitcom::Cunitcom(const Cunitcom &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of Cunitcom::Cunitcom(Cunitcom &other, int thread)

CMIP* Cunitcom::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cunitcom(*static_cast<Cunitcom*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cunitcom::~Cunitcom()
{
#ifndef __ONE_THREAD_
	if (!m_iThread) {
#endif
	if (m_ipDemand)
		delete[] m_ipDemand;
#ifndef __ONE_THREAD_
	}
#endif
}

//////////////////// implementation
void Cunitcom::readData(const char* fileName)
{
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw new CFileException("Cunitcom::readData",fileName);
	}
	int n;
	fin >> n >> m_iT >> m_dQ;
	m_iGenNum=n;
	if (!(m_ipDemand = new(std::nothrow) int[m_iT+7*n])) {
		throw CMemoryException("Cunitcom::readData");
	}
	
	m_ipUpCapacity=(m_ipLoCapacity=m_ipDemand+m_iT)+n;
	m_ipRPlus=(m_ipRminus=m_ipUpCapacity+n)+n; 
	m_ipUnitCost=(m_ipFixedCost=(m_ipStartUpCost=m_ipRPlus+n)+n)+n;

	for (int t=0; t < m_iT; ++t)
		fin >> 	m_ipDemand[t];
	
	for (int i=0; i < n; ++i)
		fin >> m_ipLoCapacity[i] >> m_ipUpCapacity[i]
		    >> m_ipRminus[i] >> m_ipRPlus[i]
		    >> m_ipStartUpCost[i] >> m_ipFixedCost[i] >> m_ipUnitCost[i];  

	fin.close();
} // end of Cunitcom::readData

void Cunitcom::printSol(VAR_VECTOR &y)
{ 
	char fileName[128];
	getprobname(fileName);
	strcat(fileName,".sol");
	std::ofstream fout(fileName);
	if (!fout.is_open()) {
		throw new CFileException("Cassembly::ptintSol",fileName);
	}
	for (int i=0; i < m_iGenNum; ++i) {
		fout << "Generator " << i+1 << ":\nPeriod: ";
		for (int t=0; t < m_iT; )
			fout << std::setw(6) <<  ++t;
		fout << "\n Power: ";
		for (int t=0; t < m_iT; ++t) {
			fout << std::setw(6) << ((floor(getval(y(i,t))+1.0e-6)*1000)/1000.0);
		}
		fout << "\n================================================================================\n";
	}
	fout.close();
} // end of Cunitcom::printSol

#include "unitcom.aux"

