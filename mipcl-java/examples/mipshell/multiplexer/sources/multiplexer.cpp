#include "multiplexer.h"
#include <cstring>
#include <cmath>
#include <iomanip>

Cmultiplexer::Cmultiplexer(const char* name): CProblem(name)
{
	m_ipCardCost=0;
	m_paDemand=0;
	char fileName[128];
	strcpy(fileName,name);
	strcat(fileName,".txt");
	readData(fileName);
}

#ifndef __ONE_THREAD_
Cmultiplexer::Cmultiplexer(const Cmultiplexer &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of Cmultiplexer::Cmultiplexer(Cmultiplexer &other, int thread)

CMIP* Cmultiplexer::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cmultiplexer(*static_cast<Cmultiplexer*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cmultiplexer::~Cmultiplexer()
{
#ifndef __ONE_THREAD_
	if (!m_iThread) {
#endif
		if (m_ipCardCost)
			delete[] m_ipCardCost;
		if (m_paDemand)
			delete m_paDemand;
#ifndef __ONE_THREAD_
	}
#endif
// TODO: delete not-shared members
}

//////////////////// implementation
void Cmultiplexer::readData(const char* fileName)
{
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw new CFileException("Cmultiplexer::readData",fileName);
	}

	fin >> m_iNodeNum;
	fin.ignore(1000,'\n');
	fin >> m_iRingNum >> m_iRingCap;
	fin.ignore(1000,'\n');
	fin >> m_iMultCost >> m_iSlotNum;
	fin.ignore(1000,'\n');
	fin >> m_iCardNum;
	fin.ignore(1000,'\n');
	if (!(m_ipCardCost = new(std::nothrow) int[2*m_iCardNum])) {
		throw CMemoryException("Cmultiplexer::readData");		
	}
	m_ipCardCap=m_ipCardCost+m_iCardNum;
	for (int i=0; i < m_iCardNum; ++i)
		fin >> m_ipCardCost[i] >> m_ipCardCap[i];
	
	fin >> *(m_paDemand = new REAL_ARRAY());

	fin.close();
} // end of Cmultiplexer::readData

#include "multiplexer.aux"

