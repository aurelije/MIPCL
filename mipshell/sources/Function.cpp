// Function.cpp: implementation of the CFunction class.
//
//////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cstring>
#include "Var.h"
#include "Problem.h"
#include "Function.h"

using std::strcpy;
using std::strncpy;
using std::strcat;
using std::strchr;
using std::malloc;
using std::free;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CFunction::setName(const char* name)
{
	if (name)
		strncpy(m_strName,name,FUNC_NAME_LENGTH);
	else m_strName[0]=0;
}

CFunction::CFunction(const int n): m_iMaxSize(n)
{
	m_iSize=0;
	m_strName[0]=0;
	m_pX=m_pY=0;
	if (!(m_dpPoint = new double[2*n])) {
		throw new CMemoryException("CFunction::CFunction");
	}
}

CFunction::~CFunction()
{
	if (m_dpPoint)
		delete m_dpPoint;
}

///// \cond NEWandDELoperators
//void* CFunction::operator new(size_t iSize) throw(CMemoryException)
//{
//	void *p=malloc(iSize);
//	if (!p) {
//		throw new CMemoryException("CFunction::operator new");
//	}
//	return p;
//}
//
//void CFunction::operator delete(void* pPtr) throw()
//{
//	free(pPtr);
//}
//
//void* CFunction::operator new[](size_t iSize) throw(CMemoryException)
//{
//	void *p=malloc(iSize);
//	if (!p) {
//		throw new CMemoryException("CFunction::operator new[]");
//	}
//	return p;
//}
//
//void CFunction::operator delete[](void* pPtr) throw()
//{
//	free(pPtr);
//}
///// \endcond

/*
double& CFunction::operator()(double arg)
{
	return (*m_pVal)[arg]=0.0;
}
*/

CFunction& CFunction::operator==(CVar& var){
	m_pY=&var;
	return *this;
}

CFunction& operator==(CVar& var, CFunction& func)
{
	func.setY(&var);
	return func;
}

int CFunction::getPointNum(const char* points)
{
	int k=0;
	char *str=const_cast<char*>(points);
	while ((str=strchr(str,')'))) {
		++str;
		++k;
	}
	return k;
}

void CFunction::getPoint(int ind, double& x, double& y)
{
	x=m_dpPoint[ind<<1];
	y=m_dpPoint[(ind<<1) + 1];
}
