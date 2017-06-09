#include "timeindex.h"
#include <cstring>
#include <limits>
#include <iomanip>

Ctimeindex::Ctimeindex(const char* name): CProblem(name)
{
	m_ipRelease=m_ipDead=m_ipProcTime=m_ipStart=0;
	char fileName[128];
	strcpy(fileName,name);
	strcat(fileName,".txt");
	readData(fileName);
	if (!(m_ipStart = new(std::nothrow) int[m_iJobNum])) {
		throw CMemoryException("Ctimeindex::Ctimeindex");		
	}
}

#ifdef __THREADS_
Ctimeindex::Ctimeindex(const Ctimeindex &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of Ctimeindex::Ctimeindex(Ctimeindex &other, int thread)

CMIP* Ctimeindex::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Ctimeindex(*static_cast<Ctimeindex*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Ctimeindex::~Ctimeindex()
{
#ifdef __THREADS_
	if (!m_iThread) {
#endif
		if (m_ipRelease)
			delete[] m_ipRelease;
		if (m_dpWeight)
			delete[] m_dpWeight;
		if (m_ipStart)
			delete[] m_ipStart;
#ifdef __THREADS_
	}
#endif
// TODO: delete notshared members 
}

//////////////////// implementation
void Ctimeindex::readData(const char* fileName)
{
	int m,n;
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw CFileException("Ctimeindex::readData",fileName);
	}
	fin.ignore(256,'\n');
	fin >> m >> n;
	m_iMachineNum=m; m_iJobNum=n;
	if (m_ipRelease = new(std::nothrow) int[(2+m)*n])
		m_dpWeight = new(std::nothrow) double[n];
	if (!m_ipRelease || !m_dpWeight) {
		throw CMemoryException("Ctimeindex::readData");
	}
	m_ipProcTime=(m_ipDead=m_ipRelease+n)+n;
	
	int *p=m_ipProcTime;

	for (int j=0; j < n; j++) {
		fin >> m_ipRelease[j] >> m_ipDead[j] >> m_dpWeight[j];
		for (int i=0; i < m; ++i)
				fin >> *p++;
	}
	
}

#include "timeindex.aux"

void Ctimeindex::printSol(VAR_ARRAY &x)
{
	char fileName[128];
	getProblemName(fileName);
	strcat(fileName,".sol");
	std::ofstream fout(fileName);
	if (!fout.is_open()) {
		throw CFileException("Ctimeindex::printSol",fileName);
	}
	int m=m_iMachineNum, n=m_iJobNum;
	fout << "+----------------------------------+\n";
	fout << "|  Job | Machine | Starts |   Ends |\n";
	fout << "+----------------------------------+\n";
	for(int i=0; i < m; ++i) {
		for(int j=0; j < n; ++j) {
			for (int t=r(j); t <= d(j)-p(i,j); ++t)
				if (getval(x(i,j,t)) > 0.5) {
					fout << "| " << std::setw(4) << j+1 <<" | "
						<< std::setw(7) << i+1 << " | "
						<< std::setw(6) << t << " | "
						<< std::setw(6) << t+p(i,j) << " |\n";
				break;
			}
		}
	}
	fout << "+----------------------------------+\n";
} // end of Ctimeindex::PrintSol
