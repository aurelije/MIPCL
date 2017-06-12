// Index.cpp: implementation for the CIndex class.
//
//////////////////////////////////////////////////////////////////////
#include <cstring>
#include <cstdlib>
#include "Index.h"

extern void int2str(int, char*);

CIndex::CIndex() 
{
	m_pStr[0]=0;
}
	
CIndex::CIndex(const CIndex& ind) 
{
	strcpy(m_pStr,ind.m_pStr);
}

CIndex::CIndex(const CIndex& ind0, const CIndex& ind1)
{
	char *str, ind[255];
	strcpy(str=ind,ind0.m_pStr);
	str+=strlen(str);
	*str++=',';
	strcpy(str,ind1.m_pStr);
	ind[MAX_INDEX_SIZE]=0;
	strcpy(m_pStr,ind);
}

CIndex::CIndex(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2)
{
	char *str, ind[255];
	strcpy(str=ind,ind0.m_pStr);
	str+=strlen(str);
	*str++=',';
	strcpy(str,ind1.m_pStr);
	str+=strlen(str);
	*str++=',';
	strcpy(str,ind2.m_pStr);
	ind[MAX_INDEX_SIZE]=0;
	strcpy(m_pStr,ind);
}

CIndex::CIndex(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2, const CIndex& ind3)
{
	char *str, ind[255];
	strcpy(str=ind,ind0.m_pStr);
	str+=strlen(str);
	*str++=',';
	strcpy(str,ind1.m_pStr);
	str+=strlen(str);
	*str++=',';
	strcpy(str,ind2.m_pStr);
	str+=strlen(str);
	*str++=',';
	strcpy(str,ind3.m_pStr);
	ind[MAX_INDEX_SIZE]=0;
	strcpy(m_pStr,ind);
}

CIndex::CIndex(int ind)
{
//	itoa(ind,m_pStr,10);
	int2str(ind,m_pStr);
}

CIndex::CIndex(const char* ind) 
{
	strncpy(m_pStr,ind,MAX_INDEX_SIZE);
}

CIndex::~CIndex() 
{
}
/////////////////////////
CIndex CIndex::operator()(int k)
{
	char s[MAX_INDEX_SIZE+1];
	char *ch, *str=m_pStr;
	for (int i=0; i < k; i++) {
		 str=strchr(str,',')+1; 
	}
	if (str) {
		if (ch=strchr(str,',')) *ch=0;
		strcpy(s,str);
		if (ch) *ch=',';
	}
	else *s=0;
	return CIndex(s);
}

int CIndex::split(int k, CIndex* ind)
{
	int q=0;
	char *ch, *str=m_pStr;
	for (; q < k && (ch=strchr(str,',')); str=ch+1) {
		*ch=0;
		ind[q++].setString(str);
		*ch=',';
	}
	if (q < k)
		if (strlen(str))
			ind[q++].setString(str);

	return q;
}

/////////////////////////
void CIndex::setString(const char* str)
{
	strncpy(m_pStr,str,MAX_INDEX_SIZE);
}

/////////////////////////
// operators
///////////////////////
bool CIndex::operator==(CIndex& ind)
{
	return (strcmp(m_pStr,ind.m_pStr))? false: true;
}

bool CIndex::operator==(int ind)
{
	char str[16];
//	itoa(ind,str,10);
	int2str(ind,str);
	return (strcmp(m_pStr,str))? false: true;
}

bool CIndex::operator==(const char* ind)
{
	return (strcmp(m_pStr,ind))? false: true;
}

bool operator==(int ind1, CIndex& ind2)
{
	char str[16];
//	itoa(ind1,str,10);
	int2str(ind1,str);
	return (strcmp(ind2.m_pStr,str))? false: true;
}

bool operator==(const char* ind1, CIndex& ind2)
{
	return (strcmp(ind2.m_pStr,ind1))? false: true;
}

bool CIndex::operator%(CIndex& ind)
{
	bool flag=false;
	char *ch, *str2=(char*)ind.getString(), *str=m_pStr;
	for (; !flag && (ch=strchr(str2,',')); str2=ch+1) {
		*ch=0;
		if (!strcmp(str2,str))
			flag=true;
		*ch=',';
	}
	if (!flag)
		if (strlen(str2))
			if (!strcmp(str2,str))
				flag=true;

	return flag;
}

bool operator%(int ind1, CIndex& ind2)
{
	bool flag=false;
	char *ch, *str2=(char*)ind2.getString(), str[16];
//	itoa(ind1,str,10);
	int2str(ind1,str);
	for (; !flag && (ch=strchr(str2,',')); str2=ch+1) {
		*ch=0;
		if (!strcmp(str2,str))
			flag=true;
		*ch=',';
	}
	if (!flag)
		if (strlen(str2))
			if (!strcmp(str2,str))
				flag=true;

	return flag;
}

bool operator%(const char* ind1, CIndex& ind2)
{
	bool flag=false;
	char *ch, *str2=(char*)ind2.getString();
	for (; !flag && (ch=strchr(str2,',')); str2=ch+1) {
		*ch=0;
		if (!strcmp(str2,ind1))
			flag=true;
		*ch=',';
	}
	if (!flag)
		if (strlen(str2))
			if (!strcmp(str2,ind1))
				flag=true;

	return flag;
}


CIndex& CIndex::operator+=(CIndex& ind)
{
	int sz;
	if ((sz=static_cast<int>(strlen(m_pStr))) < MAX_INDEX_SIZE) {
		m_pStr[sz++]=',';
		strncpy(m_pStr+sz,ind.m_pStr,MAX_INDEX_SIZE-sz);
	}
	return *this;
}

CIndex& CIndex::operator+(CIndex& ind)
{
	int sz;
	CIndex* pInd = new CIndex(m_pStr);
	if ((sz=static_cast<int>(strlen(m_pStr))) < MAX_INDEX_SIZE) {
		pInd->m_pStr[sz++]=',';
		strncpy(pInd->m_pStr+sz,ind.m_pStr,MAX_INDEX_SIZE-sz);
	}
	return *pInd;
}

CIndex& operator+(const char* str, CIndex& ind)
{
	int sz;
	CIndex* pInd = new CIndex(str);
	if ((sz=static_cast<int>(strlen(pInd->m_pStr))) < MAX_INDEX_SIZE) {
		pInd->m_pStr[sz++]=',';
		strncpy(pInd->m_pStr+sz,ind.m_pStr,MAX_INDEX_SIZE-sz);
	}
	return *pInd;
}

CIndex& operator+(const int i, CIndex& ind)
{
	int sz;
	CIndex* pInd = new CIndex(i);
	if ((sz=static_cast<int>(strlen(pInd->m_pStr))) < MAX_INDEX_SIZE) {
		pInd->m_pStr[sz++]=',';
		strncpy(pInd->m_pStr+sz,ind.m_pStr,MAX_INDEX_SIZE-sz);
	}
	return *pInd;
}

bool CIndex::operator<(const CIndex& ind)
{
	return ((strcmp(m_pStr,ind.m_pStr) < 0)? true: false);

}

bool operator<(const CIndex& ind1, const CIndex& ind2)
{
	return ((strcmp(ind1.m_pStr,ind2.m_pStr) < 0)? true: false);

}
bool operator==(const CIndex& ind1, const CIndex& ind2)
{
	return (!strcmp(ind1.m_pStr,ind2.m_pStr))? true: false;
}

////////////////////////
// input/output
///////////////////////
MIPSHELL_API std::istream& operator>>(std::istream& in, CIndex& ind)
{
	int bad=0, sz=0;
	char ch=0;
	for (in >> ch; !in.eof() && (ch == ' ' || ch == '\t' || ch == '\n'); in >> ch);
	if (ch == '(') {
		for (in >> ch; !in.eof() && (sz < MAX_INDEX_SIZE) && ch != ')'; in >> ch)
			ind.m_pStr[sz++]=ch;
		if (in.eof())
			bad=2;
		else if (sz == MAX_INDEX_SIZE)
			bad=3;
		else ind.m_pStr[sz]=0;
	}
	else bad=1;
	if (bad) {
		char msg[64];
		strcpy(msg,"Error parsing an index: ");
		switch (bad) {
			case 1: strcat(msg,"( is expected"); break;
			case 2: strcat(msg,"unexpected end of file"); break;
			case 3: strcat(msg,"max length is exceeded"); break;
		}
		throw CDataException(msg);
	}
	return in;
}

MIPSHELL_API std::ostream& operator<<(std::ostream& out, const CIndex& ind)
{
	out << "(" << ind.m_pStr << ")";
	return out;
}

///// \cond NEWandDELoperators
//////////////////////
//// new and delete
//void* CIndex::operator new(size_t iSize) throw(CMemoryException)
//{
//	void *p=malloc(iSize);
//	if (!p) {
//		throw new CMemoryException("CIndex::operator new");
//	}
//	return p;
//}
//
//void CIndex::operator delete(void* pPtr) throw()
//{
//	free(pPtr);
//}
//
//void* CIndex::operator new[](size_t iSize) throw(CMemoryException)
//{
//	void *p=malloc(iSize);
//	if (!p) {
//		throw new CMemoryException("CIndex::operator new[]");
//	}
//	return p;
//}
//
//void CIndex::operator delete[](void* pPtr) throw()
//{
//	free(pPtr);
//}
///// \endcond
