// VarArray.cpp: implementation of the CVarArray class.
//
//////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cstring>
#include "Set.h"
#include "VarArray.h"
#include "Problem.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////
// CVarArray implementation
/////////////////////////////////////

CVarArray::CVarArray(CProblem* prob, const char* name, int type, CBasicSet& s)
{
	if (!(m_pMap = new(std::nothrow) std::map<CIndex,CVarPtr>)) {
		throw new CMemoryException("CVarArray::CVarArray");
	}
	CIndex ind0;
	int sz;
	char *str, VarName[80];
	CVar* pVar;
	m_pVars=0;
	m_pVars=pVar=new CVar[s.getSize()];
	m_iDim=1;
	strcpy(VarName,name);
	sz=static_cast<int>(strlen(VarName));
	VarName[sz++]='(';
	str=VarName+sz;
	for (s.initIt(); s.getNextIndex(ind0);) {
		m_pMap->insert(map<CIndex,CVarPtr>::value_type(ind0,pVar));
		strcpy(str,ind0.getString());
		sz=static_cast<int>(strlen(str));
		str[sz++]=')';
		str[sz]=0;
		pVar->setName(VarName);
		pVar->setType(type);
		prob->addVar(*pVar);
		pVar++;
	}
}

CVarArray::CVarArray(CProblem* prob, const char* name, int type, CBasicSet& s0, CBasicSet& s1)
{
	if (!(m_pMap = new(std::nothrow) std::map<CIndex,CVarPtr>)) {
		throw new CMemoryException("CVarArray::CVarArray");
	}
	CIndex ind,ind0, ind1;
	int sz;
	char *str,VarName[80];
	CVar* pVar;
	m_pVars=0;
	m_pVars=pVar=new CVar[s0.getSize()*s1.getSize()];
	m_iDim=2;
	strcpy(VarName,name);
	sz=static_cast<int>(strlen(VarName));
	VarName[sz++]='(';
	str=VarName+sz;
	for (s0.initIt(); s0.getNextIndex(ind0);) {
		for (s1.initIt(); s1.getNextIndex(ind1);) {
			ind=ind0+ind1;
			m_pMap->insert(map<CIndex,CVarPtr>::value_type(ind,pVar));
			strcpy(str,ind.getString());
			sz=static_cast<int>(strlen(str));
			str[sz++]=')';
			str[sz]=0;
			pVar->setName(VarName);
			pVar->setType(type);
			prob->addVar(*pVar);
			pVar++;
		}
	}
}

CVarArray::CVarArray(CProblem* prob, const char* name, int type, CBasicSet& s0, CBasicSet& s1, CBasicSet& s2)
{
	if (!(m_pMap = new(std::nothrow) std::map<CIndex,CVarPtr>)) {
		throw new CMemoryException("CVarArray::CVarArray");
	}
	CIndex ind, ind0, ind1, ind2;
	int sz;
	char *str, VarName[80];
	CVar* pVar;
	m_pVars=0;
	m_pVars=pVar=new CVar[s0.getSize()*s1.getSize()*s2.getSize()];
	m_iDim=3;
	strcpy(VarName,name);
	sz=static_cast<int>(strlen(VarName));
	VarName[sz++]='(';
	str=VarName+sz;
	for (s0.initIt(); s0.getNextIndex(ind0);) {
		for (s1.initIt(); s1.getNextIndex(ind1);) {
			for (s2.initIt(); s2.getNextIndex(ind2);) {
				ind=ind0+ind1+ind2;
				m_pMap->insert(map<CIndex,CVarPtr>::value_type(ind,pVar));
				strcpy(str,ind.getString());
				sz=static_cast<int>(strlen(str));
				str[sz++]=')';
				str[sz]=0;
				pVar->setName(VarName);
				pVar->setType(type);
				prob->addVar(*pVar);
				pVar++;
			}
		}
	}
}

CVarArray::CVarArray(CProblem* prob, const char* name, int type, CBasicSet& s0, CBasicSet& s1, CBasicSet& s2, CBasicSet& s3)
{
	if (!(m_pMap = new(std::nothrow) std::map<CIndex,CVarPtr>)) {
		throw new CMemoryException("CVarArray::CVarArray");
	}
	CIndex ind,ind0,ind1,ind2,ind3;
	int sz;
	char *str, VarName[80];
	CVar* pVar;
	m_pVars=0;
	m_pVars=pVar=new CVar[s0.getSize()*s1.getSize()*s2.getSize()*s3.getSize()];
	m_iDim=4;
	strcpy(VarName,name);
	sz=static_cast<int>(strlen(VarName));
	VarName[sz++]='(';
	str=VarName+sz;
	for (s0.initIt(); s0.getNextIndex(ind0);) {
		for (s1.initIt(); s1.getNextIndex(ind1);) {
			for (s2.initIt(); s2.getNextIndex(ind2);) {
					for (s3.initIt(); s3.getNextIndex(ind3);) {
					ind=ind0+ind1+ind2+ind3;
					m_pMap->insert(map<CIndex,CVarPtr>::value_type(ind,pVar));
					strcpy(str,ind.getString());
					sz=static_cast<int>(strlen(str));
					str[sz++]=')';
					str[sz]=0;
					pVar->setName(VarName);
					pVar->setType(type);
					prob->addVar(*pVar);
					pVar++;
				}
			}
		}
	}
}

CVarArray::~CVarArray()
{
	if (m_pMap)
		delete m_pMap;
	if (m_pVars)
		delete[] m_pVars;
}

void CVarArray::setName(const char* name)
{
	if (name) {
		strncpy(m_strName,name,VARARR_NAME_LENGTH);
		m_strName[VARARR_NAME_LENGTH]=0;
	}
	else
		m_strName[0]=0;
}

//////////////////////////////////
// Operators
/////////////////
CVar& CVarArray::operator[](const CIndex& ind)
{
	map<CIndex,CVarPtr>::iterator it=m_pMap->find(ind);
	if (it == m_pMap->end()) {
		throw "Bad index when indexing var array ";// << m_strName";
	}
	return *((*it).second);
}

CVar& CVarArray::operator()(const CIndex& ind)
{
	map<CIndex,CVarPtr>::iterator it=m_pMap->find(ind);
	if (it == m_pMap->end()) {
		throw "Bad index when indexing var array";// << m_strName";
	}
	return *((*it).second);
}

CVar& CVarArray::operator()(const CIndex& ind0, const CIndex& ind1)
{
	CIndex ind(ind0,ind1);
	map<CIndex,CVarPtr>::iterator it=m_pMap->find(ind);
	if (it == m_pMap->end()) {
		throw "Bad index when indexing var array";// << m_strName";
	}
	return *((*it).second);
}
CVar& CVarArray::operator()(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2)
{
	CIndex ind(ind0,ind1,ind2);
	map<CIndex,CVarPtr>::iterator it=m_pMap->find(ind);
	if (it == m_pMap->end()) {
		throw "Bad index when indexing var array";// << m_strName";
	}
	return *((*it).second);
}

CVar& CVarArray::operator()(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2, const CIndex& ind3)
{
	CIndex ind(ind0,ind1,ind2,ind3);
	map<CIndex,CVarPtr>::iterator it=m_pMap->find(ind);
	if (it == m_pMap->end()) {
		throw new CDataException("CVarArray::operator()[Bad index when indexing var array]");
	}
	return *((*it).second);
}

///// \cond NEWandDELoperators
//void* CVarArray::operator new(size_t iSize) throw(CMemoryException)
//{
//	void *p=malloc(iSize);
//	if (!p) {
//		throw new CMemoryException("CVarArray::operator new");
//	}
//	return p;
//}
//
//void CVarArray::operator delete(void* pPtr) throw()
//{
//	free(pPtr);
//}
//
//void* CVarArray::operator new[](size_t iSize)  throw(CMemoryException)
//{
//	void *p=malloc(iSize);
//	if (!p) {
//		throw new CMemoryException("CVarArray::operator new[]");
//	}
//	return p;
//}
//
//void CVarArray::operator delete[](void* pPtr) throw()
//{
//	free(pPtr);
//}
///// \endcond
