#include "optportfolio.h"
#include <cstring>
#include <cmath>

Coptportfolio::Coptportfolio(const char* name): CProblem(name)
{
	m_dTol=0.0001;
	m_dZero=1.0e-6;
	m_dV=0.01;
	m_dpL=0;
	char fileName[128];
	strcpy(fileName,name);
	strcat(fileName,".txt");
	readData(fileName);
	computeParameters(0.9);
}

#ifdef __THREADS_
Coptportfolio::Coptportfolio(const Coptportfolio &other, int thread): CProblem(other,thread)
{
	m_dTol=other.m_dTol;
	m_dZero=other.m_dZero;
	m_iN=other.m_iN;
	m_iT=other.m_iT;
	m_iQ=other.m_iQ;
	m_dV==other.m_dV;
	m_dpRet=other.m_dpRet;
	m_dpMu=other.m_dpMu;
	m_dpP=other.m_dpP;
	m_dpL=other.m_dpL;
	m_pY=other.m_pY;
} // end of Coptportfolio::Coptportfolio(Coptportfolio &other, int thread)

CMIP* Coptportfolio::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Coptportfolio(*static_cast<Coptportfolio*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Coptportfolio::~Coptportfolio()
{
#ifdef __THREADS_
	if (!m_iThread) {
#endif
		if (m_dpL)
			delete[] m_dpL;
#ifdef __THREADS_
	}
#endif
} // end of Coptportfolio::~Coptportfolio

//////////////////// implementation
void Coptportfolio::readData(const char* fileName)
{
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw new CFileException("Coptportfolio::readData",fileName);
	}

	int n, T, q;

	fin >> n >> m_iQ >> T;
	m_iN=n; m_iT=T;
	if (!(m_dpL = new(std::nothrow) double[n*(T+2)+T])) {
		throw CMemoryException("Coptportfolio::readData");
	}
	m_dpP=(m_dpMu=m_dpL+n)+n;
	m_dpRet=m_dpP+T;
	
	for (int j=0; j < n; ++j)
		fin >> m_dpL[j];
	
	for (int t=0; t < T; ++t) {
		fin >> q;
		for (int j=0; j < n; ++j)
			fin >> m_dpRet[j*T+t];
	}
	fin.close();
} // end of Coptportfolio::readData

double Coptportfolio::getMean(const double p, const int T, const double* dpR)
{
	double nom=0.0, den=0.0, f=1.0; 
	for (int t=T; --t >= 0;) {
		nom+=f*log(dpR[t]);
		den+=f;
		f*=p;
	}
	return exp(nom/den);
} // end of Coptportfolio::getMean

void Coptportfolio::computeParameters(const double p)
{
	int n=m_iN, T=m_iT;
	double *dpR=m_dpRet, *dpP=m_dpP;
	double d=0.0, f=1.0;
	for (int j=0; j < n; ++j) {
		m_dpMu[j]=getMean(p,T,m_dpRet+j*T);
	}
	for (int t=T; --t >= 0;) {
		dpP[t]=f;
		d+=f;
		f*=p;
	}
	for (int t=0; t < T; ++t) {
		dpP[t]/=d;
	}
	for (int j=0; j < n; ++j) {
		d=0.0;
		for(int tau=0; tau < T; ++tau) {
			d+=dpP[tau]*dpR[tau];
		}
		for(int t=0; t < T; ++t) {
			dpR[t]-=d;
		}
		dpR+=T;
	}
} // end of Coptportfolio::computeParameters

#include "optportfolio.aux"

