#include "multlotsize.h"
#include <cstring>
#include <cmath>

Cmultlotsize::Cmultlotsize(const char* name): CProblem(name)
{
	m_ipDur=0;
	m_dpCost=0;

	char fileName[128];
	strcpy(fileName,name);
	strcat(fileName,".txt");
	readData(fileName);
}

#ifdef __THREADS_
Cmultlotsize::Cmultlotsize(const Cmultlotsize &other, int thread): CProblem(other,thread)
{
}

CMIP* Cmultlotsize::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cmultlotsize(*static_cast<Cmultlotsize*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cmultlotsize::~Cmultlotsize()
{
#ifdef __THREADS_
	if (!m_iThread) {
#endif
		if (m_ipDur)
			delete[] m_ipDur;
		if (m_dpCost)
			delete[] m_dpCost;
#ifdef __THREADS_
	}
#endif
} // end of Cmultlotsize::~Cmultlotsize()

//////////////////// implementation
void Cmultlotsize::readData(const char* fileName)
{
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw CFileException("Cmultlotsize::readData",fileName);
	}
	int n,m,T;
	fin >> n >> m >> T;
	m_iProdNum=n; m_iMachTypeNum=m; m_iT=T;

	if (m_ipDur=new(std::nothrow) int[T + 3*n+ n*T + m*T]) {
		if (m_dpCost=new(std::nothrow) double[2*n*T+m*T+m*n+n*n]) {
			m_ipFinalStock=(m_ipInitStock=(m_ipStockCap=m_ipDur+T)+n)+n;
			m_ipMachNum=(m_ipDemand=m_ipFinalStock+n)+n*T;
			m_dpFixedCost=(m_dpHoldingCost=m_dpCost+n*T)+n*T;
			m_dpRho=(m_dpTau=m_dpFixedCost+m*T)+m*n;
		}
	}
	if (!m_dpCost) {
		throw new CMemoryException("Cmultlotsize::readData");
	}
	
	for (int j=0; j < n; ++j) {
		fin >> m_ipStockCap[j] >> m_ipInitStock[j] >> m_ipFinalStock[j];
	}
	double *c=m_dpCost, *f=m_dpFixedCost, *h=m_dpHoldingCost,
		*tau=m_dpTau, *rho=m_dpRho;
	int k, *d=m_ipDemand, *mn=m_ipMachNum;
	for (int t=0; t < T; ++t) {
		fin >> k >> m_ipDur[t];
		for (int j=0; j < n; ++j)
			fin >> *d++ >> *c++ >> *h++;
		for (int i=0; i < m; ++i)
			fin >> *f++ >> *mn++;
	}
	
	for (int i=0; i < m; ++i) {
		for (int j=0; j < n; ++j)
			fin >> *tau++;
	}

	for (int j=0; j < n; ++j) {
		for (int k=0; k < n; ++k)
			fin >> *rho++;
	}
	
	fin.close();
} // end of Cmultlotsize::readData()

#include "multlotsize.aux"

