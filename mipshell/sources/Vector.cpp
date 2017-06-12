// Vector.cpp: implementation of the CVector class.
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include <limits>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <except.h>
#include "Vector.h"

#define MAX_STR_LENGTH 256

extern void int2str(int, char*);

char* getStrInd(int ind, int dim, int* ipSize, int iTotSize, char* str)
{
	int q;
	char *sub_str=str;
	str[0]='(';
	for (int i=0; i < dim; i++) {
		iTotSize/=ipSize[i];
		q=ind/iTotSize;
		ind%=iTotSize;
//		itoa(q,++sub_str,10);
		int2str(q,++sub_str);
		sub_str+=strlen(sub_str);
		sub_str[0]=(dim-i > 1)? ',': ')';
	}
	sub_str[1]=0;
	return str;
}

////////////////////////////////////////////////
// CVector
////////////////////////////////////////////////
///// \cond NEWandDELoperators
//template <typename Tell>
//void* CVector<Tell>::operator new(size_t iSize) throw(CMemoryException)
//{
//	void *p=malloc(iSize);
//	if (!p) {
//		throw new CMemoryException("CVector<Tell>::operator new");
//	}
//	return p;
//}
//
//template <typename Tell>
//void CVector<Tell>::operator delete(void* pPtr) throw()
//{
//	free(pPtr);
//}
//
//template <typename Tell>
//void* CVector<Tell>::operator new[](size_t iSize) throw(CMemoryException)
//{
//	void *p=malloc(iSize);
//	if (!p) {
//		throw new CMemoryException("CVector<Tell>::operator new[]");
//	}
//	return p;
//}
//
//template <typename Tell>
//void CVector<Tell>::operator delete[](void* pPtr) throw()
//{
//	free(pPtr);
//}
///// \endcond

template <typename Tell>
void CVector<Tell>::reallocMem(int size, int newSize)
{
	Tell* pVal=new(std::nothrow) Tell[newSize];
	if (!pVal) {
		throw new CMemoryException("CVector<Tell>::reallocMem");
	}
	if (size) {
		memcpy(pVal,m_pVal,size*sizeof(Tell));
		delete[] m_pVal;
	}
	m_pVal=pVal;
}

template <typename Tell>
CVector<Tell>::CVector()
{
	m_iDim=0;
	m_pVal=0;
}

template <typename Tell>
CVector<Tell>::CVector(int size0, int size1, int size2, int size3)
{
	m_iDim=0;
	int sz=1;
	if (size0 > 0) {
		sz*=(m_ipSize[0]=size0);
		m_iDim++;
		if (size1 > 0) {
			sz*=(m_ipSize[1]=size1);
			m_iDim++;
			if (size2 > 0) {
				sz*=(m_ipSize[2]=size2);
				m_iDim++;
				if (size3 > 0) {
					sz*=(m_ipSize[3]=size3);
					m_iDim++;
				}
			}
		}
	}
	if (!m_iDim)
		m_pVal=0;
	else if (!(m_pVal= new(std::nothrow) Tell[sz])) {
		throw new CMemoryException("CVector<Tell>::CVector");
	}
}

template <typename Tell>
void CVector<Tell>::setDim(int size0, int size1, int size2, int size3)
{
	m_iDim=0;
	int sz=1, sz2=getSize();
	if (size0 > 0) {
		sz*=(m_ipSize[0]=size0);
		m_iDim++;
		if (size1 > 0) {
			sz*=(m_ipSize[1]=size1);
			m_iDim++;
			if (size2 > 0) {
				sz*=(m_ipSize[2]=size2);
				m_iDim++;
				if (size3 > 0) {
					sz*=(m_ipSize[3]=size3);
					m_iDim++;
				}
			}
		}
	}
	if (sz > sz2)
		reallocMem(sz2,sz);
}

template <typename Tell>
CVector<Tell>::~CVector()
{
	if (m_pVal)
		delete[] m_pVal;
}

template <typename Tell>
int CVector<Tell>::getSize(int arg)
{
	if (!m_iDim)
		return 0;
	else if (arg >= 0)
		return m_ipSize[arg];
	else {
		int sz=1;
		for (int i=0; i < m_iDim; i++)
			sz*=m_ipSize[i];
		return sz;
	}
}

//////////////////////////////////
// usefull functions

template <typename Tell>
Tell CVector<Tell>::min()
{
	Tell m=std::numeric_limits<Tell>::max();
	int n=getSize();
	for (int i=0; i < n; ++i) {
		if (m_pVal[i] < m)
			m=m_pVal[i];
	}
	return m;
}

template <typename Tell>
Tell CVector<Tell>::max()
{
	Tell m=std::numeric_limits<Tell>::min();
	int n=getSize();
	for (int i=0; i < n; ++i) {
		if (m_pVal[i] > m)
			m=m_pVal[i];
	}
	return m;
}

template <typename Tell>
Tell CVector<Tell>::Sum()
{
	Tell s=0;
	int n=getSize();
	for (int i=0; i < n; ++i) {
		s+=m_pVal[i];
	}
	return s;
}

template <typename Tell>
Tell CVector<Tell>::absSum()
{
	Tell s=0;
	int n=getSize();
	for (int i=0; i < n; ++i) {
		if (m_pVal[i] > 0)
			s+=m_pVal[i];
		else
			s-=m_pVal[i];
	}
	return s;
}

////////////////////////////////////////////////////

template <typename Tell>
void scanArray(std::istream& in, int dim, int* ipSize, Tell* pVal)
{
	char ch;
	int sz=ipSize[0]-1;
	for (in >> ch; ch != '['; in >> ch);
	if (dim == 1) {
		for (int i=0; i <= sz; i++) {
			in >> pVal[i];
			if (i < sz)
				for (in >> ch; ch != ','; in >> ch);			
		}
	}
	else {
		int i,sz1=1;
		for (i=1; i < dim; i++)
			sz1*=ipSize[i];
		for (i=0; i <= sz; i++, pVal+=sz1) {
			scanArray(in,dim-1,ipSize+1,pVal);
			if (i < sz)
				for (in >> ch; ch != ','; in >> ch);			
		}
	}
	for (ch=in.get(); ch != ']'; ch=in.get());			
}

template <typename Tell>
void printArray(std::ostream& out, int dim, int* ipSize, Tell* pVal)
{
	int sz=ipSize[0]-1;
	out << "[";
	if (dim == 1) {
		for (int i=0; i < sz; i++) {
			out << pVal[i] << ",";
		}
		if (sz >= 0)
			out << pVal[sz];
	}
	else {
		int i,sz1=0;
		for (i=1; i < dim; i++)
			sz1+=ipSize[i];
		for (i=0; i <= sz; i++, pVal+=sz1) {
			printArray(out,dim-1,ipSize+1,pVal);
			if (i < sz)
				out << ",\n";
		}
	}
	out << "]";
}

template <typename T>
std::istream& operator>>(std::istream& in, CVector<T>& v)
{
	char ch, *c1, *c2, str[64];
	int dim=0,sz=0;
	std::locale lc=in.imbue(std::locale());
	for (in >> ch; ch != ':'; in >> ch) {
		if (ch != ' ' && ch != '\t' && ch != '\n')
			str[sz++]=ch;
		if (sz == 63)
			break;
	}
	str[sz--]=0;
	if (ch != ':' || strncmp(str,"dim(",4) || str[sz] != ')') {
		throw new CDataException("operator>>(istream&, CVector<Tell>&)");		
	}
	c1=str+4;
	str[sz]=0;
	sz=v.getSize();
	for (c2=strchr(c1,','); c2; c2=strchr(c1=c2+1,',')) {
		*c2=0;
		v.m_ipSize[dim++]=atoi(c1);
	}
	v.m_ipSize[dim++]=atoi(c1);
	v.m_iDim=dim;
	if (v.getSize() > sz)
		v.reallocMem(sz,v.getSize());
	scanArray(in,dim,v.m_ipSize,v.m_pVal);
	in.imbue(lc);
	return in;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, CVector<T>& v)
//ostream& Write(ostream& out, CVector<Tell>& v)
{
	int sz=v.m_iDim-1;
	std::locale lc=out.imbue(std::locale());
	out << "dim(";
	for (int i=0; i < sz; i++) {
		out << v.m_ipSize[i] << ","; 
	}
	out << v.m_ipSize[sz] << "):\n";
	printArray(out,v.m_iDim,v.m_ipSize,v.m_pVal);
	out.imbue(lc);
	return out;
}

//////////////////////////////////
// Instantiation
//////////////////

template class MIPSHELL_API CVector<int>;
template class MIPSHELL_API CVector<double>;

template MIPSHELL_API std::istream& operator>>(std::istream& in, CVector<double>& v);
template MIPSHELL_API std::ostream& operator<<(std::ostream& out, CVector<double>& v);
template MIPSHELL_API std::istream& operator>>(std::istream& in, CVector<int>& v);
template MIPSHELL_API std::ostream& operator<<(std::ostream& out, CVector<int>& v);

