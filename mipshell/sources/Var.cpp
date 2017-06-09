// Var.cpp: implementation of the CVar class.
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "Ctr.h"
#include "Var.h"
#include "Problem.h"

#define VAR_INF (1.0e12) //(getVarInf())

extern void int2str(int, char*);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CVar::init()
{
	m_iHd=-2; // -2 -- variable has not been arrached to any problem 
	m_strName[0]=0;
	m_dLb=-VAR_INF;
	m_dUb=VAR_INF;
	m_iType=REAL;
}

CVar::CVar()
{
	init();
}

CVar::CVar(CProblem* prob, const char* name, enMipshellVarType type)
{
	init();
	if (name) {
		strncpy(m_strName,name,VAR_NAME_LENGTH);
		m_strName[VAR_NAME_LENGTH]=0;
	}
	setType(type);
	prob->addVar(*this);
}

CVar::CVar(CVar& other)
{
	m_iHd=-2;
	m_dLb=other.m_dLb;
	m_dUb=other.m_dUb;
//	m_dVal=other.m_dVal;
	strcpy(m_strName,other.m_strName);
}

CVar::~CVar()
{
}

/// \cond NEWandDELoperators
void* CVar::operator new(size_t iSize) throw(CMemoryException)
{
	void *p=malloc(iSize);
	if (!p) {
		throw new CMemoryException("CVar::operator new");
	}
	return p;
}

void CVar::operator delete(void* pPtr) throw()
{
	free(pPtr);
}

void* CVar::operator new[](size_t iSize) throw(CMemoryException)
{
	void *p=malloc(iSize);
	if (!p) {
		throw new CMemoryException("CVar::operator new[]");
	}
	return p;
}

void CVar::operator delete[](void* pPtr) throw()
{
	free(pPtr);
}
/// \endcond

#define GE   0x00010000
#define LE   0x00020000

void CVar::setType(int type)
{
	m_iType=type;
	if (type & GE)
		m_dLb=0.0;
	else if (type & BIN) {
		m_dLb=0.0; m_dUb=1.0;
	}
	else if (type & LE)
		m_dUb=0.0;
}

#undef GE
#undef LE

void CVar::setName(const char* name)
{
	if (name) {
		strncpy(m_strName,name,VAR_NAME_LENGTH);
		m_strName[VAR_NAME_LENGTH]=0;
	}
	else
		m_strName[0]=0;
}

CVar::operator CLinSum()
{
	CLinSum* lsum=new CLinSum();
	lsum->setLastTerm(new CTerm(this,1.0));
	return *lsum;
}

CTerm& CVar::operator*(double coeff)
{
	return *(new CTerm(this,coeff));
}

CLinSum& CVar::operator+(CTerm& term)
{
	return (new CTerm(this,1.0))->operator+(term);
}

CLinSum& CVar::operator-(CTerm& term)
{
	return (new CTerm(this,1.0))->operator-(term);
}

CLinSum& CVar::operator-(CVar& var)
{
	return (new CTerm(this,1.0))->operator+(*new CTerm(&var,-1.0));
}

CLinSum& CVar::operator+(CVar& var)
{
	return (new CTerm(this,1.0))->operator+(*new CTerm(&var,1.0));
}

CLinSum& CVar::operator+(CLinSum& lsum)
{
	return (new CTerm(this,1.0))->operator+(lsum);
}

CLinSum& CVar::operator-(CLinSum& lsum)
{
	return (new CTerm(this,1.0))->operator-(lsum);
}

CLinSum& CVar::operator+(double a)
{
	return (new CTerm(this,1.0))->operator+(a);
}

CLinSum& CVar::operator-(double a)
{
	return (new CTerm(this,1.0))->operator-(a);
}

CLinSum& operator+(double a,CVar& var)
{
	return (new CTerm(&var,1.0))->operator+(a);
}

CLinSum& operator-(double a,CVar& var)
{
	return (new CTerm(&var,-1.0))->operator+(a);
}

CCtr& CVar::operator<=(CVar& var)
{
	return (*this - var <= 0.0);
}

CCtr& CVar::operator>=(CVar& var)
{
	return (*this - var >= 0.0);
}

CCtr& CVar::operator==(CVar& var)
{
	return (*this - var == 0.0);
}

CCtr& CVar::operator<=(CTerm& term)
{
	return (*this - term <= 0.0);
}

CCtr& CVar::operator>=(CTerm& term)
{
	return (*this - term >= 0.0);
}

CCtr& CVar::operator==(CTerm& term)
{
	return (*this - term == 0.0);
}

CCtr& CVar::operator<=(CLinSum& lsum)
{
	return (lsum - *this >= 0.0);
}

CCtr& CVar::operator>=(CLinSum& lsum)
{
	return (lsum - *this <= 0.0);
}

CCtr& CVar::operator==(CLinSum& lsum)
{
	return (lsum - *this == 0.0);
}

//////////////////////////////////////////////
// class CVarVector
//////////////////////////////////////////////
/// \cond NEWandDELoperators
void* CVarVector::operator new(size_t iSize)
{
	return malloc(iSize);
}

void CVarVector::operator delete(void* pPtr)
{
	free(pPtr);
}
/// \endcond

CVarVector::CVarVector()
{
	m_strName[0]=0;
	m_iDim=0;
	m_pVars=0;
}

void CVarVector::setName(CProblem* prob, const char* name, int type)
{
	if (name)
		strncpy(m_strName,name,VARARR_NAME_LENGTH);
	else
		m_strName[0]=0;
	if (m_pVars) {
		int i, j, k, q, ipInd[MAX_ARR_DIM];
		char *ch, *ch0,str[VAR_NAME_LENGTH+1];
		for (k=1, i=0; i < m_iDim; i++) {
			k*=m_ipSize[i];
			ipInd[i]=0;
		}
		j=0; q=m_iDim-1;
		strcpy(str,m_strName);
		ch0=str+strlen(str);
		*ch0='('; ch0++;
		for (;; ) {
			ch=ch0;
			for (int r=0; r < m_iDim; r++) {
//				itoa(ipInd[r],ch,10);
				int2str(ipInd[r],ch);
				ch+=strlen(ch);
				*ch=','; ch++;
			}
			*(ch-1)=')'; *ch=0;
			prob->addVar(m_pVars[j]);
			m_pVars[j].setType(type);
			m_pVars[j++].setName(str);
			k=q;
			while (++ipInd[k] == m_ipSize[k]) {
				ipInd[k--]=0;
				if (k < 0)
					return;
			}
		}
	}
} // end of CVarVector::setName

void CVarVector::init(CProblem* prob, const char* name, int type,
					 int size0, int size1, int size2, int size3)
{
	m_iDim=0;
	if (name)
		strncpy(m_strName,name,VARARR_NAME_LENGTH);
	else
		m_strName[0]=0;
	int sz=1;
	if (size0 > 0) {
		sz*=(m_ipSize[0]=size0);
		++m_iDim;
		if (size1 > 0) {
			sz*=(m_ipSize[1]=size1);
			m_iDim++;
			if (size2 > 0) {
				sz*=(m_ipSize[2]=size2);
				++m_iDim;
				if (size3 > 0) {
					sz*=(m_ipSize[3]=size3);
					++m_iDim;
				}
			}
		}
	}
	m_pVars = (m_iDim)? new CVar[sz]: 0;
	setName(prob,name,type);
}

CVarVector::CVarVector(CProblem* prob, char const* name, int type,
					 int size0, int size1, int size2, int size3)
{
	init(prob,name,type,size0,size1,size2,size3);
}

CVarVector::~CVarVector()
{
	if (m_pVars)
		delete[] m_pVars;
}

int CVarVector::getSize()
{
	int sz=1;
	for (int i=0; i < m_iDim; ++i)
		sz*=m_ipSize[i];
	return sz;
}

