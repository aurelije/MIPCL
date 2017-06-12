#include "IndexFund.h"
#include <cstring>
#include <iomanip>

CIndexFund::CIndexFund(const char* name): CProblem(name)
{
	char fileName[128];
	strcpy(fileName,name);
	strcat(fileName,".txt");
	readData(fileName);
}

#ifndef __ONE_THREAD_
CIndexFund::CIndexFund(const CIndexFund &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of CIndexFund::CIndexFund(CIndexFund &other, int thread)

CMIP* CIndexFund::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new CIndexFund(*static_cast<CIndexFund*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

CIndexFund::~CIndexFund()
{
#ifndef __ONE_THREAD_
	if (!m_iThread) {
#endif
// TODO: delete shared members 
#ifndef __ONE_THREAD_
	}
#endif
// TODO: delete not-shared members
}

//////////////////// implementation
void CIndexFund::readData(const char* fileName)
{
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw new CFileException("CIndexFund::readData",fileName);
	}
	fin >> m_iQ;
	fin >> Val;
	fin >> rho;
	fin.close();
} // end of CIndexFund::readData

void CIndexFund::printSol(VAR_VECTOR &x, VAR_VECTOR &y)
{ 
	char fileName[128];
	getprobname(fileName);
	strcat(fileName,".sol");
	std::ofstream fout(fileName);
	if (!fout.is_open()) {
		throw new CFileException("CIndexFund::ptintSol",fileName);
	}
	int n=rho.getSize(0);
	double stockVal, val;
	stockVal=0.0;
	for (int i=0; i < n; ++i)
		stockVal+=Val(i);
	
	fout << "+---------------+\n";
	fout << "| Asset | Share |\n";
	fout << "+---------------+\n";

	for (int j=0; j < n; ++j) {
		if (getval(y(j)) > 0.5) {
			val=0.0;
			for (int i=0; i < n; ++i) {
				if (getval(x(i,j)) > 0.5)
					val+=Val(i);
			}
			val*=100.0/stockVal;
			fout << "| " << std::setw(5) << j << " | " 
				<< std::fixed << std::setprecision(2) << std::setw(5) << val << " |\n";   
		}		
	}
	fout << "+---------------+\n";

	fout.close();
} // end of CIndexFund::printSol

#include "IndexFund.aux"

