#include "proclayout.h"
#include <cstring>
#include <cmath>
#include <iomanip>

Cproclayout::Cproclayout(const char* name): CProblem(name)
{
	m_ipMovingCost=0;
	char fileName[128];
	strcpy(fileName,name);
	strcat(fileName,".txt");
	readData(fileName);
}

#ifdef __THREADS_
Cproclayout::Cproclayout(const Cproclayout &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of Cproclayout::Cproclayout(Cproclayout &other, int thread)

CMIP* Cproclayout::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cproclayout(*static_cast<Cproclayout*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cproclayout::~Cproclayout()
{
#ifdef __THREADS_
	if (!m_iThread) {
#endif
		if (m_ipMovingCost)
			delete[] m_ipMovingCost;
#ifdef __THREADS_
	}
#endif
// TODO: delete notshared members 
}

//////////////////// implementation
void Cproclayout::readData(const char* fileName)
{
	int k,m,n;
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw new CFileException("Cproclayout::readData",fileName);
	}
	fin >> n >> m;
	m_iN=n; m_iM=m;
	if (!(m_ipMovingCost = new(std::nothrow) int[m*n + m*(m-1)/2 + n*(n-1)/2])) {
		fin.close();
		throw CMemoryException("Cproclayout::readData");
	}
	m_ipFlow=(m_ipDist=m_ipMovingCost+(n*n))+m*(m-1)/2;

	k=m*n;
	for (int i=0; i < k; ++i)
		fin >> 	m_ipMovingCost[i];
	
	k=m*(m-1)/2;
	for (int i=0; i < k; ++i)
		fin >> 	m_ipDist[i];

	k=n*(n-1)/2;
	for (int i=0; i < k; ++i)
		fin >> 	m_ipFlow[i];

	fin.close();
} // end of Cproclayout::readData

void Cproclayout::printSol(VAR_VECTOR &x)
{ 
	char fileName[128];
	getprobname(fileName);
	strcat(fileName,".sol");
	std::ofstream fout(fileName);
	if (!fout.is_open()) {
		throw new CFileException("Cproclayout::ptintSol",fileName);
	}
	int n=m_iN, m=m_iM;
	fout << "Total expenses: " << (int)floor(getobj()+0.5) << std::endl;
	fout << "i => s  means that department i moves to site s\n";
	for (int i=0; i < n; ++i) for (int s=0; s < m; ++s) {
		if (getval(x(i,s)) > 0.5)
			fout << std::setw(4) << i+1 << " => " << s+1 << std::endl;
	}
	fout.close();
} // end of Cproclayout::printSol

int Cproclayout::c(int i,int j, int s, int r)
{
	if (s == r)
		return 0;
	int k;
//	if (i > j) {
//		k=i; i=j; j=k;
//	}
	if (s > r) {
		k=s; s=r; r=k;
	}
	return m_ipDist[s*m_iM -s*(1+s)/2 + r-s-1]*m_ipFlow[i*m_iN-i*(i+1)/2 + j-i-1];
}

///////////////////////////////

#include "proclayout.aux"

