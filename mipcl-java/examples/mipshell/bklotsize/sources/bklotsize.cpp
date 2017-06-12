#include "bklotsize.h"
#include <cstring>
#include <cmath>
#include <iomanip>


Cbklotsize::Cbklotsize(const char* name): CProblem(name)
{
	m_ipDemand=0;
	char fileName[128];
	strcpy(fileName,name);
	strcat(fileName,".txt");
	readData(fileName);	
}

#ifndef __ONE_THREAD_
Cbklotsize::Cbklotsize(const Cbklotsize &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of Cbklotsize::Cbklotsize(Cbklotsize &other, int thread)

CMIP* Cbklotsize::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cbklotsize(*static_cast<Cbklotsize*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cbklotsize::~Cbklotsize()
{
#ifndef __ONE_THREAD_
	if (!m_iThread) {
#endif
		if (m_ipDemand)
			delete[] m_ipDemand;
// TODO: delete shared members 
#ifndef __ONE_THREAD_
	}
#endif
// TODO: delete notshared members 
}

//////////////////// implementation
void Cbklotsize::readData(const char* fileName)
{
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw new CFileException("Cbklotsize::readData",fileName);
	}
	fin >> m_iT >> m_iTp;
	if (!(m_ipDemand = new(std::nothrow) int[m_iT+2*m_iTp + m_iTp*m_iT])) {
		fin.close();
		throw CMemoryException("Cbklotsize::readData");
	}
	m_ipCost=(m_ipFxCost=(m_ipCapacity=m_ipDemand+m_iT)+m_iTp)+m_iTp;

	for (int t=0; t < m_iT; ++t)
		fin >> 	m_ipDemand[t];

	for (int t=0; t < m_iTp; ++t)
		fin >> 	m_ipCapacity[t];

	for (int t=0; t < m_iTp; ++t)
		fin >> 	m_ipFxCost[t];

	int *c=m_ipCost;
	for (int tau=0; tau < m_iTp; ++tau) {
		for (int t=0; t < m_iT; ++t)
			fin >> c[t];
		c+=m_iT;
	}
	
	fin.close();
} // end of Cbklotsize::readData

void Cbklotsize::printSol(VAR_VECTOR &x, VAR_VECTOR &y)
{ 
	char fileName[128];
	getprobname(fileName);
	strcat(fileName,".sol");
	std::ofstream fout(fileName);
	if (!fout.is_open()) {
		throw new CFileException("Clotsize::ptintSol",fileName);
	}
	int k,prod,val, prodCost=0, fxCost=0;
	fout << "+--------+----------+------------+\n";
	fout << "| Prod.  | produces | for demand |\n";
	fout << "| period | (units)  |   period   |\n";
	fout << "+--------+----------+------------+\n";
	for (int tau=0; tau < m_iTp; ++tau) {
		prod=k=0;
		if (getval(y(tau)) > 0.5)
			fxCost+=m_ipFxCost[tau];
		for (int t=0; t < m_iT; ++t) {
			if ((val=(int)floor(getval(x(tau,t))+0.5)) > 0) {
				prod+=val;
				prodCost+=m_ipCost[tau*m_iT+t]*val;
				if (!k++) {
					fout << "|  " << std::setw(4) << std::setprecision(4)
						<< tau << "  | ";
				}
				else {
					fout << "|        | ";
				}
				fout << std::setw(8) << std::setprecision(8) << val << " |    " 
					<< std::setw(4) << std::setprecision(4)
					<< t+1 << "    |\n";
			}
		}
		fout << "+--------+----------+------------+\n";
	}
	fout << "Production cost - " << prodCost << std::endl;
	fout << "Fixed cost      - " << fxCost << std::endl;
	fout.close();
} // end of Cbklotsize::printSol

#include "bklotsize.aux"

