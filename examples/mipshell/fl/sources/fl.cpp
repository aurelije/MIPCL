#include "fl.h"
#include <cmath>
#include <cstring>

Cfl::Cfl(const char* name): CProblem(name)
{
	m_ipCost=0;
	char fileName[128];
	strcpy(fileName,name);
	strcat(fileName,".txt");
	readData(fileName);
}

#ifdef __THREADS_
Cfl::Cfl(const Cfl &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of Cfl::Cfl(Cfl &other, int thread)

CMIP* Cfl::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cfl(*static_cast<Cfl*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cfl::~Cfl()
{
#ifdef __THREADS_
	if (!m_iThread) {
#endif
	if (m_ipCost)
		delete[] m_ipCost;
#ifdef __THREADS_
	}
#endif
// TODO: delete notshared members 
}

//////////////////// implementation
void Cfl::readData(const char* fileName)
{
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw new CFileException("Clotsize::readData",fileName);
	}
	int m, n;
	fin >> n >> m >> m_iQ; 
	m_iLocationNum=n; m_iSiteNum=m;
	if (!(m_ipCost = new(std::nothrow) int[m*(n+2)+n])) {
		throw CMemoryException("Clotsize::readData");
	}
	m_ipPopulation=(m_ipCapacity=(m_ipFixedCost=m_ipCost+m*n)+m)+m;

	for (int j=0; j < n; ++j)
		fin >> m_ipPopulation[j];
	
	for (int i=0; i < m; ++i)
		fin >> 	m_ipFixedCost[i] >> m_ipCapacity[i];

	int *ipCost=m_ipCost;
	for (int i=0; i < m; ++i) {
		for (int j=0; j < n; ++j)
			fin >> 	ipCost[j];
		ipCost+=n;
	}

	fin.close();
} // end of Cfl::readData

void Cfl::printSol(VAR_VECTOR &x, VAR_VECTOR &y)
{ 
	char fileName[128];
	getprobname(fileName);
	strcat(fileName,".sol");
	std::ofstream fout(fileName);
	if (!fout.is_open()) {
		throw new CFileException("Cassembly::ptintSol",fileName);
	}
	int m=m_iSiteNum, n=m_iLocationNum;
	fout << "Overall travel distance is " << std::floor(getobj()+0.5) << std::endl;
	for (int k, i=0; i < m; ++i) {
		if (getval(y(i)) > 0.5) {
			fout << "===== Facility at site " << i+1 << " serves customers:\n";
			for (int j=k=0; j < n; ++j) {
				if ((k=getval(x(i,j))) > 0.5) {
					fout << "\t" << k << "\tfrom loc. " << j+1 << std::endl;
				}
			}
			fout << "=============================================\n";
		}
	}
	fout.close();
} // end of Cfl::printSol

#include "fl.aux"

