// Ctr.cpp: implementation of the CCtr class.
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "Var.h"
#include "Ctr.h"
#include "Problem.h"

#define INF (CLP::INF)
#define ZERO (1.0e-13)

extern void int2str(int, char*);

//////////////////////////////////////////////
// class  CTerm
//////////////////////////////////////////////
CTerm::CTerm()
{
	m_pVar=0;
	m_pPrev=0;
}

CTerm::CTerm(CVar* var,	double coeff)
{
	m_dCoeff=coeff;
	m_pVar=var;
	m_pPrev=0;
}

CTerm::CTerm(CTerm& other)
{
	m_dCoeff=other.m_dCoeff;
	m_pVar=other.m_pVar;
	m_pPrev=0;
}

CTerm::~CTerm()
{
}

// operators
CTerm::operator CLinSum()
{
	CLinSum* lsum=new CLinSum();
	lsum->setLastTerm(this);
	return *lsum;
}

CTerm& operator*(double coeff, CVar& var)
{
	return *(new CTerm(&var,coeff));
}

CLinSum& CTerm::operator+(CTerm& term)
{
	CLinSum* lsum=new CLinSum();
	lsum->makePermanent();
	lsum->addTerm(this);
	lsum->addTerm(&term);
	return *lsum;
}
CLinSum& CTerm::operator-(CTerm& term)
{
	CLinSum* lsum=new CLinSum();
	lsum->makePermanent();
	lsum->addTerm(this);
	term.m_dCoeff=-term.m_dCoeff;
	lsum->addTerm(&term);
	return *lsum;
}

CLinSum& CTerm::operator+(CVar& var)
{
	return operator+(*(new CTerm(&var,1.0)));
}

CLinSum& CTerm::operator-(CVar& var)
{
	return operator+(*(new CTerm(&var,-1.0)));
}

CLinSum& CTerm::operator+(double a)
{
	return operator+(*(new CTerm(0,a)));
}

CLinSum& CTerm::operator-(double a)
{
	return operator-(*(new CTerm(0,a)));
}
	
CLinSum& operator+(double a,CTerm& term)
{
	return (new CTerm(0,a))->operator+(term);
}

CLinSum& operator-(double a,CTerm& term)
{
	return (new CTerm(0,a))->operator-(term);
}


CLinSum& CTerm::operator+(CLinSum& lsum)
{
	CTerm *pTerm=lsum.getLastTerm();
	if (pTerm) {
		for (CTerm* pTerm1=pTerm; pTerm1; pTerm1=(pTerm=pTerm1)->getPrev());
		pTerm->m_pPrev=this;
	}
	else {
		lsum.setLastTerm(this);
	}
	return lsum;
}

CLinSum& CTerm::operator-(CLinSum& lsum)
{
	CTerm *pTerm=lsum.getLastTerm();
	if (pTerm) {
		for (CTerm* pTerm1=pTerm; pTerm1; pTerm1=(pTerm=pTerm1)->getPrev())
			pTerm1->m_dCoeff=-pTerm1->m_dCoeff;
		pTerm->m_pPrev=this;
	}
	else {
		lsum.setLastTerm(this);
	}
	return lsum;
}

CCtr& CTerm::operator<=(CVar& var)
{
	return (*this - var <= 0.0);
}

CCtr& CTerm::operator>=(CVar& var)
{
	return (*this - var >= 0.0);
}

CCtr& CTerm::operator==(CVar& var)
{
	return (*this - var == 0.0);
}

CCtr& CTerm::operator<=(CTerm& term)
{
	return (*this - term <= 0.0);
}

CCtr& CTerm::operator>=(CTerm& term)
{
	return (*this - term >= 0.0);
}

CCtr& CTerm::operator==(CTerm& term)
{
	return (*this - term == 0.0);
}

CCtr& CTerm::operator<=(CLinSum& lsum)
{
	return (lsum - *this >= 0.0);
}

CCtr& CTerm::operator>=(CLinSum& lsum)
{
	return (lsum - *this <= 0.0);
}

CCtr& CTerm::operator==(CLinSum& lsum)
{
	return (lsum - *this == 0.0);
}

/// \cond debugMIPshell
void CTerm::print()
{
	if (m_dCoeff >= 0)
		std::cerr << "+";
	std::cerr << m_dCoeff;
	if (m_pVar)
		std::cerr << "*" << m_pVar->getName();
}
/// \endcond

//////////////////////////////////////////
// class CLinSum
//////////////////////////////////////////
/// \cond NEWandDELoperators
void* CLinSum::operator new(size_t iSize) throw(CMemoryException)
{
	void *p=malloc(iSize);
	if (!p) {
		throw new CMemoryException("CLinSum::operator new");
	}
	return p;
}

void CLinSum::operator delete(void* pPtr) throw()
{
	free(pPtr);
}
/// \endcond

CLinSum::CLinSum(): m_bTemp(true), m_pLast(0)
{}

CLinSum::CLinSum(CVar& var): m_bTemp(true)
{
	setLastTerm(new CTerm(&var,1.0));
}

CLinSum::CLinSum(CTerm& term): m_bTemp(true), m_pLast(0)
{
	setLastTerm(&term);
}

CLinSum::~CLinSum()
{
	for (CTerm* pTerm1, *pTerm=m_pLast; pTerm; pTerm=pTerm1) {
		pTerm1=pTerm->getPrev();
		delete pTerm;
	}
}

void CLinSum::addTerm(CTerm* pTerm)
{
	pTerm->setPrev(m_pLast);
	m_pLast=pTerm;
}

CLinSum& CLinSum::operator=(CLinSum& lsum)
{
	m_pLast=lsum.m_pLast;
	lsum.m_pLast=0;
	if (lsum.m_bTemp)
		delete &lsum;
	return *this;
}

CLinSum& CLinSum::operator*=(double a)
{
	for (CTerm* pTerm=m_pLast; pTerm; pTerm=pTerm->getPrev())
		pTerm->setCoeff(a*pTerm->getCoeff());
	return *this;
}

CLinSum& CLinSum::operator*(double a)
{
	return *this*=a;
}

CLinSum& operator*(double a, CLinSum& lsum)
{
	return lsum*=a;
}

CLinSum& operator+(double a, CLinSum& lsum)
{
	lsum+=a;
	return lsum; 
}

CLinSum& operator-(double a, CLinSum& lsum)
{
	CTerm *pTerm=lsum.getLastTerm();
	if (pTerm) {
		for (CTerm* pTerm1=pTerm; pTerm1; pTerm1=(pTerm=pTerm1)->getPrev())
			pTerm1->changeSign();

		pTerm->setPrev(new CTerm(0,a));
//		pTerm->changeSign();
	}
	else {
		lsum.setLastTerm(new CTerm(0,a));
	}
	return lsum;
}

CLinSum& CLinSum::operator+=(CTerm& term)
{
	if (fabs(term.getCoeff()) > ZERO) {
		term.setPrev(m_pLast);
		m_pLast=&term;
	}
	else
		delete &term;
	return *this;
}

CLinSum& CLinSum::operator-=(CTerm& term)
{
	if (fabs(term.getCoeff()) > ZERO) {
		term.setPrev(m_pLast);
		m_pLast=&term;
		term.changeSign();
	}
	else
		delete &term;
	return *this;
}

CLinSum& CLinSum::operator+=(CVar& var)
{
	return operator+=(*(new CTerm(&var,1.0)));
}

CLinSum& CLinSum::operator-=(CVar& var)
{
	return operator+=(*(new CTerm(&var,-1.0)));
}

CLinSum& CLinSum::operator+=(double w)
{
	return operator+=(*(new CTerm(0,w)));
}

CLinSum& CLinSum::operator-=(double w)
{
	return operator+=(*(new CTerm(0,-w)));
}

CLinSum& CLinSum::operator+(double w)
{
	return *this+=w;
}

CLinSum& CLinSum::operator-(double w)
{
	return *this-=w;
}

CLinSum& CLinSum::operator+(CTerm& term)
{
	term.setPrev(m_pLast);
	m_pLast=&term;
	return *this;
}

CLinSum& CLinSum::operator-(CTerm& term)
{
	term.changeSign();
	term.setPrev(m_pLast);
	m_pLast=&term;
	return *this;
}

CLinSum& CLinSum::operator+(CVar& var)
{
	return operator+(*(new CTerm(&var,1.0)));
}

CLinSum& CLinSum::operator-(CVar& var)
{
	return operator+(*(new CTerm(&var,-1.0)));
}

CLinSum& CLinSum::plusSum(CLinSum& lsum)
{
	if (lsum.m_pLast) {
		CTerm* pTerm=lsum.m_pLast;
		for (CTerm* pTerm1=pTerm; pTerm1; pTerm=pTerm1, pTerm1=pTerm1->getPrev());
		pTerm->setPrev(m_pLast);
		m_pLast=lsum.m_pLast;
		lsum.m_pLast=0;
	}
	if (lsum.m_bTemp)
		delete &lsum;
	return *this;
}

CLinSum& CLinSum::minusSum(CLinSum& lsum)
{
	if (lsum.m_pLast) {
		CTerm* pTerm=lsum.m_pLast;
		for (CTerm* pT=pTerm; pT; pT=(pTerm=pT)->getPrev())
			pT->changeSign();
		pTerm->setPrev(m_pLast);
		m_pLast=lsum.m_pLast;
		lsum.m_pLast=0;
	}
	if (lsum.m_bTemp)
		delete &lsum;
	return *this;
}

CLinSum& CLinSum::operator-() // unary minus
{
	for (CTerm* pT=m_pLast; pT; pT=pT->getPrev())
		pT->changeSign();
	return *this;
}

CLinSum& CLinSum::operator+=(CLinSum& lsum)
{
	this->plusSum(lsum);
	return *this;
}

CLinSum& CLinSum::operator-=(CLinSum& lsum)
{
	this->minusSum(lsum);
	return *this;
}

CLinSum& CLinSum::operator+(CLinSum& lsum)
{
	this->plusSum(lsum);
	return *this;
}

CLinSum& CLinSum::operator-(CLinSum& lsum)
{
	this->minusSum(lsum);
	return *this;
}

CCtr& CLinSum::operator<=(double b)
{
	CCtr* ctr=new CCtr();
	ctr->setRHS(b);
	ctr->setLastTerm(m_pLast);
	m_pLast=0;
	if (m_bTemp)
		delete this;
	return *ctr;
}

CCtr& CLinSum::operator>=(double b)
{
	CCtr* ctr=new CCtr();
	ctr->setLHS(b);
	ctr->setLastTerm(m_pLast);
	m_pLast=0;
	if (m_bTemp)
		delete this;
	return *ctr;
}

CCtr& CLinSum::operator==(double b)
{
	CCtr* ctr=new CCtr();
	ctr->setLHS(b);
	ctr->setRHS(b);
	ctr->setLastTerm(m_pLast);
	m_pLast=0;
	if (m_bTemp)
		delete this;
	return *ctr;
}

//CCtr& operator==(CLinSum &lsum, double b)
//{
//	CCtr* ctr=new CCtr();
//	ctr->setLHS(b);
//	ctr->setRHS(b);
//	ctr->setLastTerm(lsum.m_pLast);
//	lsum.m_pLast=0;
//	if (lsum.m_bTemp)
//		delete &lsum;
//	return *ctr;
//}


CCtr& CLinSum::operator<=(CVar& var)
{
	CCtr* ctr=new CCtr();
	ctr->setRHS(0.0);
	*this-=var;
	ctr->setLastTerm(m_pLast);
	m_pLast=0;
	if (m_bTemp)
		delete this;
	return *ctr;
}

CCtr& CLinSum::operator>=(CVar& var)
{
	CCtr* ctr=new CCtr();
	ctr->setLHS(0.0);
	*this-=var;
	ctr->setLastTerm(m_pLast);
	m_pLast=0;
	if (m_bTemp)
		delete this;
	return *ctr;
}

CCtr& CLinSum::operator==(CVar& var)
{
	CCtr* ctr=new CCtr();
	ctr->setLHS(0.0);
	ctr->setRHS(0.0);
	*this-=var;
	ctr->setLastTerm(m_pLast);
	m_pLast=0;
	if (m_bTemp)
		delete this;
	return *ctr;
}
CCtr& CLinSum::operator<=(CTerm& term)
{
	CCtr* ctr=new CCtr();
	ctr->setRHS(0.0);
	*this-=term;
	ctr->setLastTerm(m_pLast);
	m_pLast=0;
	if (m_bTemp)
		delete this;
	return *ctr;
}

CCtr& CLinSum::operator>=(CTerm& term)
{
	CCtr* ctr=new CCtr();
	ctr->setLHS(0.0);
	*this-=term;
	ctr->setLastTerm(m_pLast);
	m_pLast=0;
	if (m_bTemp)
		delete this;
	return *ctr;
}

CCtr& CLinSum::operator==(CTerm& term)
{
	CCtr* ctr=new CCtr();
	ctr->setLHS(0.0);
	ctr->setRHS(0.0);
	*this-=term;
	ctr->setLastTerm(m_pLast);
	m_pLast=0;
	if (m_bTemp)
		delete this;
	return *ctr;
}

CCtr& CLinSum::operator<=(CLinSum& lsum)
{
	CCtr* ctr=new CCtr();
	ctr->setRHS(0.0);
	this->minusSum(lsum);
	ctr->setLastTerm(m_pLast);
	m_pLast=0;
	if (m_bTemp)
		delete this;
	return *ctr;
}

CCtr& CLinSum::operator>=(CLinSum& lsum)
{
	CCtr* ctr=new CCtr();
	ctr->setLHS(0.0);
	this->minusSum(lsum);
	ctr->setLastTerm(m_pLast);
	m_pLast=0;
	if (m_bTemp)
		delete this;
	return *ctr;
}

CCtr& CLinSum::operator==(CLinSum& lsum)
{
	CCtr* ctr=new CCtr();
	ctr->setLHS(0.0);
	ctr->setRHS(0.0);
	this->minusSum(lsum);
	ctr->setLastTerm(m_pLast);
	m_pLast=0;
	if (m_bTemp)
		delete this;
	return *ctr;
}

CCtr& operator<=(double b, CLinSum& lsum)
{
	CCtr* ctr=new CCtr();
	ctr->setLHS(b);
	ctr->setLastTerm(lsum.getLastTerm());
	lsum.setLastTerm(0);
	if (lsum.isTemporal())
		delete &lsum;
	return *ctr;
}

CCtr& operator>=(double b, CLinSum& lsum)
{
	CCtr* ctr=new CCtr();
	ctr->setRHS(b);
	ctr->setLastTerm(lsum.getLastTerm());
	lsum.setLastTerm(0);
	if (lsum.isTemporal())
		delete &lsum;
	return *ctr;
}

CCtr& operator==(double b, CLinSum& lsum)
{
	CCtr* ctr=new CCtr();
	ctr->setRHS(b);
	ctr->setLHS(b);
	ctr->setLastTerm(lsum.getLastTerm());
	lsum.setLastTerm(0);
	if (lsum.isTemporal())
		delete &lsum;
	return *ctr;
}

/// \cond debugMIPshell
void CLinSum::print()
{
	for (CTerm* pTerm=m_pLast; pTerm; pTerm=pTerm->getPrev())
		pTerm->print();
}
/// \endcond

/////////////////////////////////////////
// class CCtr
/////////////////////////////////////////
/// \cond NEWandDELoperators
void* CCtr::operator new(size_t iSize) throw(CMemoryException)
{
	void *p=malloc(iSize);
	if (!p) {
		throw new CMemoryException("CCtr::operator new");
	}
	return p;
}

void CCtr::operator delete(void* pPtr) throw()
{
	free(pPtr);
}

void* CCtr::operator new[](size_t iSize) throw(CMemoryException)
{
	void *p=malloc(iSize);
	if (!p) {
		throw new CMemoryException("CCtr::operator new[]");
	}
	return p;
}

void CCtr::operator delete[](void* pPtr) throw()
{
	free(pPtr);
}
/// \endcond

void CCtr::init()
{
	m_strName[0]=0;
	m_pLastTerm=0;
	m_iHd=0; // when constraint is a cut, m_iHd=0 means that the cut is clobal 
	m_dLHS=-(m_dRHS=INF);
}

CCtr::CCtr()
{
	init();
}

CCtr::CCtr(const char* name)
{
	init();
	if (name)
		strncpy(m_strName,name,CTR_NAME_LENGTH);
}

CCtr::~CCtr()
{
	for (CTerm* pTerm1, *pTerm=m_pLastTerm; pTerm; pTerm=pTerm1) {
		pTerm1=pTerm->getPrev();
		delete pTerm;
	}
}

CCtr& CCtr::operator=(CCtr& ctr)
{
	if (ctr.m_strName)
		strncpy(m_strName,ctr.m_strName,CTR_NAME_LENGTH);
	else
		m_strName[0]=0;
	m_iHd=ctr.m_iHd;
	m_pLastTerm=ctr.m_pLastTerm;
	m_dLHS=ctr.m_dLHS;
	m_dRHS=ctr.m_dRHS;
	ctr.m_pLastTerm=0;
	delete &ctr;
	return *this;
}

void CCtr::setName(const char* name, int ind1, int ind2, int ind3)
{
	if (name) {
		strncpy(m_strName,name,CTR_NAME_LENGTH);
		if (ind1 >= 0) {
			char *str, index[CTR_NAME_LENGTH];
			*(str=index)='(';
//			itoa(ind1,++str,10);
			int2str(ind1,++str);
			if (ind2 >= 0) {
				*(str+=strlen(str))=',';
//				itoa(ind2,++str,10);
				int2str(ind2,++str);
				if (ind3 >= 0) {
					*(str+=strlen(str))=',';
//					itoa(ind3,++str,10);
					int2str(ind3,++str);
				}
			}
			*(str+=strlen(str))=')';
			*(str+1)=0;
			strncat(m_strName,index,CTR_NAME_LENGTH-strlen(name));
		}
	}
	else
		m_strName[0]=0;
}

CCtr& CCtr::operator<=(double b)
{
	m_dRHS=b;
	return *this;
}

CCtr& CCtr::operator>=(double b)
{
	m_dLHS=b;
	return *this;
}

CCtr& CCtr::operator==(double b)
{
	m_dLHS=m_dRHS=b;
	return *this;
}

CCtr& operator<=(double b, CCtr& ctr)
{
	ctr.m_dLHS=b;
	return ctr;
}

CCtr& operator>=(double b, CCtr& ctr)
{
	ctr.m_dRHS=b;
	return ctr;
}

CCtr& operator==(double b, CCtr& ctr)
{
	ctr.m_dLHS=ctr.m_dRHS=b;
	return ctr;
}

/// \cond debugMIPshell
void CCtr::print()
{
	if (m_dLHS > -INF)
		std::cerr << m_dLHS << " <= ";
	for (CTerm* pTerm=m_pLastTerm; pTerm; pTerm=pTerm->getPrev())
		pTerm->print();

	if (m_dRHS < INF)
		std::cerr << " <= " << m_dRHS;
	std::cerr << std::endl;
}
/// \endcond

