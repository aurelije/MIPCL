#include "shtermfin.h"
#include <cstring>
#include <cmath>

Cshtermfin::Cshtermfin(const char* name): CProblem(name)
{
	m_ipInitPortfolio=0;
	m_dpDemand=0;
	char fileName[128];
	strcpy(fileName,name);
	strcat(fileName,".txt");
	readData(fileName);

}

#ifndef __ONE_THREAD_
Cshtermfin::Cshtermfin(const Cshtermfin &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of Cshtermfin::Cshtermfin(Cshtermfin &other, int thread)

CMIP* Cshtermfin::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cshtermfin(*static_cast<Cshtermfin*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cshtermfin::~Cshtermfin()
{
#ifndef __ONE_THREAD_
	if (!m_iThread) {
#endif
		if (m_ipInitPortfolio)
			delete[] m_ipInitPortfolio;
		if (m_dpDemand)
			delete[] m_dpDemand;
#ifndef __ONE_THREAD_
	}
#endif
// TODO: delete not-shared members
}

//////////////////// implementation
void Cshtermfin::readData(const char* fileName)
{
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw new CFileException("Clotsize::readData",fileName);
	}
	int m,n;
	fin >> m_iT >> n >> m;
	m_iSecurityNum=n; m_iCreditLineNum=m;
	if (m_ipInitPortfolio = new(std::nothrow) int[n])
		m_dpDemand=new(std::nothrow) double[m_iT*(2+2*n+m)+m+n];
	if (!m_dpDemand) {
		if (m_ipInitPortfolio) {
			delete[] m_ipInitPortfolio;
			m_ipInitPortfolio=0;
		}
		throw CMemoryException("Clotsize::readData");
	}
	
	m_dpSaleCost=(m_dpMinCash=m_dpDemand+m_iT)+m_iT;
	m_dpPurchaseCost=m_dpSaleCost + n*(m_iT+1);
	m_dpLineCapacity=m_dpPurchaseCost + n*m_iT;
	m_dpLoanCost=m_dpLineCapacity+m;
		
	fin >> m_dInitCash;
	for (int i=0; i < n; ++i)
		fin >> m_ipInitPortfolio[i];
	
	for (int t=0; t < m_iT; ++t)
		fin >> m_dpDemand[t] >> m_dpMinCash[t];
	
	double *dpV=m_dpSaleCost;
	for (int i=0; i < n; ++i) {
		for (int t=0; t <= m_iT; ++t)
			fin >> *dpV++;
	}
	
	dpV=m_dpPurchaseCost;
	for (int i=0; i < n; ++i) {
		for (int t=0; t < m_iT; ++t)
			fin >> *dpV++;
	}
	
	dpV=m_dpLoanCost;
	for (int i=0; i < m; ++i) {
		fin >> m_dpLineCapacity[i];
		for (int t=0; t < m_iT; ++t)
			fin >> *dpV++;			
	}
	
	fin.close();
} // end of Cshtermfin::readData

void Cshtermfin::printSol(VAR_VECTOR &x, VAR_VECTOR &xs, VAR_VECTOR &xb,
		VAR_VECTOR &y, VAR_VECTOR &z)
{ 
	char fileName[128];
	getprobname(fileName);
	strcat(fileName,".sol");
	std::ofstream fout(fileName);
	if (!fout.is_open()) {
		throw new CFileException("Cassembly::ptintSol",fileName);
	}
	int val, T=m_iT, n=m_iSecurityNum, m=m_iCreditLineNum;
	fout << "Firm actives:\n  Cash - " << m_dInitCash << "\n  Portfolio:";
	for (int i=0; i < n; ++i)
		fout << " sec. " << i+1 << " - " << m_ipInitPortfolio[i] << ",";
	fout << "\n========================\n";
	for (int t=0; t < m_iT; ++t) {
		fout << "======= Period: " << t+1 << "\n  Cash - " << getval(y(t)) << "\n  Sold:";
		for (int i=0; i < n; ++i) {
			if ((val=(int)floor(getval(xs(i,t))+0.5)) > 0)
				fout << " sec. " << i+1 << " - " << val << ";";
		}
		fout << "\n  Baught:";
		for (int i=0; i < n; ++i) {
			if ((val=(int)floor(getval(xb(i,t))+0.5)) > 0)
				fout << " sec. " << i+1 << " - " << val << ";";
		}
		fout << "\n  Borroved:";
		for (int i=0; i < m; ++i) {
			if ((val=(int)floor(getval(z(i,t))+0.5)) > 0)
				fout << " line " << i+1 << " - " << val << ";";
		}
		fout << "\n  In portfolio:";
		for (int i=0; i < n; ++i) {
			if ((val=(int)floor(getval(x(i,t))+0.5)) > 0)
				fout << " sec. " << i+1 << " - " << val << ";";
		}
		fout << std::endl;
	}
	fout << "\n=================\n  Firm wealth at the end of time horizon - " 
	     << getobj() << std::endl;
	
	fout.close();
} // end of Cshtermfin::printSol

#include "shtermfin.aux"

