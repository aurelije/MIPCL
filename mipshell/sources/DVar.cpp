// Dvar.cpp: implementation of the CDvar class.
//
//////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <cstring>
#include "Var.h"
#include "Vector.h"
#include "Set.h"
#include "Problem.h"
#include "DVar.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CDvar::setName(const char* name)
{
	if (name)
		strncpy(m_strName,name,DVAR_NAME_LENGTH);
	else m_strName[0]=0;
}

CDvar::CDvar(const char* name): m_bMemHere(false), m_pVar(0), m_pSet(0)
{
	setName(name);
}

CDvar::CDvar(CVar& var, const char* values): m_bMemHere(false), m_pVar(&var)
{
	bool flag=(values)? true: false;
	if (flag)
		if (!values[0]) flag=false;
	if (!flag) {
		throw new CDataException("CDvar::operation in");
	}
	if (!(m_pSet = new CSet<double>(values))) {
		throw new CMemoryException("CDvar::function");
	}
	m_bMemHere=true;
}

CDvar::CDvar(CVar& var, CSet<double>& values): m_bMemHere(false), m_pVar(&var), m_pSet(&values)
{
}

CDvar::~CDvar()
{
	if (m_bMemHere && m_pSet)
		delete m_pSet;
}

///// \cond NEWandDELoperators
//void* CDvar::operator new(size_t iSize) throw(CMemoryException)
//{
//	void *p=malloc(iSize);
//	if (!p) {
//		throw new CMemoryException("CDVar::operator new");
//	}
//	return p;
//}
//
//void CDvar::operator delete(void* pPtr) throw()
//{
//	free(pPtr);
//}
//
//void* CDvar::operator new[](size_t iSize) throw(CMemoryException)
//{
//	void *p=malloc(iSize);
//	if (!p) {
//		throw new CMemoryException("CDVar::operator new[]");
//	}
//	return p;
//}
//
//void CDvar::operator delete[](void* pPtr) throw()
//{
//	free(pPtr);
//}
///// \endcond

