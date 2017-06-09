#include "lotsize.h"
#include <cstring>
#include <cmath>
#include <iomanip>

Clotsize::Clotsize(const char* name): CProblem(name)
{
	m_ipPrice=0;
	char fileName[128];
	strcpy(fileName,name);
	strcat(fileName,".txt");
	readData(fileName);
}

#ifdef __THREADS_
Clotsize::Clotsize(const Clotsize &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of Clotsize::Clotsize(Clotsize &other, int thread)

CMIP* Clotsize::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Clotsize(*static_cast<Clotsize*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Clotsize::~Clotsize()
{
#ifdef __THREADS_
	if (!m_iThread) {
#endif
		if (m_ipPrice)
			delete[] m_ipPrice;
#ifdef __THREADS_
	}
#endif
// TODO: delete notshared members 
}

//////////////////// implementation
void Clotsize::readData(const char* fileName)
{
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw new CFileException("Clotsize::readData",fileName);
	}
	fin >> m_iT >> m_iInitStock;
	if (!(m_ipPrice = new(std::nothrow) int[5*m_iT])) {
		fin.close();
		throw CMemoryException("Clotsize::readData");
	}
	m_ipDemand=(m_ipCapacity=(m_ipHoldCost=(m_ipFixedCost=m_ipPrice+m_iT)+m_iT)+m_iT)+m_iT;

	for (int t=0; t < m_iT; ++t)
		fin >> 	m_ipPrice[t] >> m_ipFixedCost[t] >> m_ipHoldCost[t]
		    >> m_ipCapacity[t] >> m_ipDemand[t];

	fin.close();
} // end of Clotsize::readData

void Clotsize::printSol(VAR_VECTOR &x, VAR_VECTOR &s)
{ 
	char fileName[128];
	getprobname(fileName);
	strcat(fileName,".sol");
	std::ofstream fout(fileName);
	if (!fout.is_open()) {
		throw new CFileException("Clotsize::ptintSol",fileName);
	}
	int val, prodCost=0, fxCost=0, holdCost=0;
	fout << "+---------------------+\n";
	fout << "| Period | Production |\n";
	fout << "+---------------------+\n";
	for (int t=0; t < m_iT; ++t) {
		if ((val=(int)floor(getval(x(t))+0.5)) > 0) {
			fout << "| " << std::setw(6) << t << " | " << std::setw(10) << val << " |\n";
			prodCost+=m_ipPrice[t]*val;
			fxCost+=m_ipFixedCost[t];
		}
		holdCost+=m_ipHoldCost[t]*((int)floor(getval(s(t))));
	}
	fout << "+---------------------+\n";
	fout << "Production cost - " << prodCost << std::endl;
	fout << "Fixed cost      - " << fxCost << std::endl;
	fout << "Inventory cost  - " << holdCost << std::endl;
	
	fout.close();
} // end of Clotsize::printSol

#include "lotsize.aux"

