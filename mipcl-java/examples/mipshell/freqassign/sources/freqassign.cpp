#include "freqassign.h"
#include <cstring>
#include <cmath>
#include <iomanip>

Cfreqassign::Cfreqassign(const char* name): CProblem(name)
{
	m_ipStation1=0;
	char fileName[128];
	strcpy(fileName,name);
	strcat(fileName,".txt");
	readData(fileName);
}

#ifndef __ONE_THREAD_
Cfreqassign::Cfreqassign(const Cfreqassign &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of Cfreqassign::Cfreqassign(Cfreqassign &other, int thread)

CMIP* Cfreqassign::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cfreqassign(*static_cast<Cfreqassign*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cfreqassign::~Cfreqassign()
{
#ifndef __ONE_THREAD_
	if (!m_iThread) {
#endif
		if (m_ipStation1)
			delete[] m_ipStation1;
#ifndef __ONE_THREAD_
	}
#endif
// TODO: delete not-shared members
}

//////////////////// implementation
void Cfreqassign::readData(const char* fileName)
{
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw new CFileException("Cfreqassign::readData",fileName);
	}
	int m;
	fin >> m_iK >> m_iN >> m;
	m_iM=m;
	if (!(m_ipStation1 = new(std::nothrow) int[3*m])) {
		throw CMemoryException("Cfreqassign::readData");
	}
	m_ipInterference=((m_ipStation2=m_ipStation1+m))+m;

	
	for (int i,j, e=0; e < m; ++e) {
		fin >> i >> j >> m_ipInterference[e];
		m_ipStation1[e]=i-1;
		m_ipStation2[e]=j-1;
	}

	fin.close();
} // end of Cfreqassign::readData

void Cfreqassign::printSol(VAR_VECTOR &f)
{ 
	char fileName[128];
	getprobname(fileName);
	strcat(fileName,".sol");
	std::ofstream fout(fileName);
	if (!fout.is_open()) {
		throw new CFileException("Cfreqassign::ptintSol",fileName);
	}
	fout << "+---------------------+\n";
	fout << "| Station | Frequency |\n|         | assigned  |\n";
	fout << "+---------------------+\n";
	for (int i=0; i < m_iN; ++i) {
		fout << "| " << std::setw(7) << i+1 << " | " 
			<< std::setw(9) << (int)floor(getval(f(i))) << " |\n";
	}
	fout << "+---------------------+\n";

	fout.close();
} // end of Cfreqassign::printSol


#include "freqassign.aux"

