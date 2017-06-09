#include "assembly.h"
#include <cstring>
#include <cmath>

Cassembly::Cassembly(const char* name): CProblem(name)
{
	m_ipTime=m_ipPrec=0;
	char fileName[128];
	strcpy(fileName,name);
	strcat(fileName,".txt");
	readData(fileName);
}

#ifdef __THREADS_
Cassembly::Cassembly(const Cassembly &other, int thread): CProblem(other,thread)
{
}

CMIP* Cassembly::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cassembly(*static_cast<Cassembly*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cassembly::~Cassembly()
{
#ifdef __THREADS_
	if (!m_iThread) {
#endif
		if (m_ipTime)
			delete[] m_ipTime;
		if (m_ipPrec)
			delete[] m_ipPrec;
#ifdef __THREADS_
	}
#endif
}

//////////////////// implementation
void Cassembly::readData(const char* fileName)
{
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw new CFileException("Cassembly::readData",fileName);
	}
	fin >> m_iOpNum >> m_iStNum >> m_iRelNum >> m_iCycleTime;
	if (m_ipTime = new(std::nothrow) int[m_iOpNum])
		if (m_ipPrec = new(std::nothrow) int[2*m_iRelNum])
			m_ipSucc=m_ipPrec+m_iRelNum;
	if (!m_ipTime || !m_ipPrec) {	
		throw CMemoryException("Cassembly::readData");
	}
	
	for (int i=0; i < m_iOpNum; ++i)
		fin >> m_ipTime[i];
	
	for (int i=0; i < m_iRelNum; ++i)
		fin >> m_ipPrec[i] >> m_ipSucc[i];
	
	fin.close();
} // end of Cgenassign::readData

void Cassembly::printSol(VAR_VECTOR &x)
{ 
	char fileName[128];
	getprobname(fileName);
	strcat(fileName,".sol");
	std::ofstream fout(fileName);
	if (!fout.is_open()) {
		throw new CFileException("Cassembly::ptintSol",fileName);
	}
	
	int k=static_cast<int>(floor(getobj()+1.0e-6));
	fout << "Number of stations = " << k
	     << "\nStation: operations\n";
	for (int i=0; i < k; ++i) {
		fout << i+1 << ":";
		for (int j=0; j < m_iOpNum; ++j)
			if (getval(x(i,j)) > 0.5)
				fout << " " << j+1;
		fout << std::endl;
	}
	fout.close();

} // end of Cassembly::printSol

#include "assembly.aux"

