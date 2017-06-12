// Set.cpp: implementation of the CRange and CSet classes.
//
//////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <cstring>

#include "Set.h"

extern void int2str(int, char*);

// next 6 functions are defined in ellstr.cpp
extern char* str2ell(const char* str, int& ell);
extern char* str2ell(const char* str, double& ell);
extern char* str2ell(const char* str, CIndex& ell);
extern void ell2str(char*& str, const int& ell);
extern void ell2str(char*& str, const double& ell);
extern void ell2str(char*& str, const CIndex& ell);

/////////////////////////////////////
// CRange
CRange::CRange(): m_iStart(0), m_iEnd(0), m_iCurIt(0) {}

CRange::CRange(const CRange& other)
{
	m_iCurIt=0;
	m_iStart=other.m_iStart;
	m_iEnd=other.m_iEnd;
}

CRange::CRange(int start, int end): m_iStart(start), m_iEnd(end), m_iCurIt(0) {}
CRange::~CRange() {}

void CRange::initIt() {
	m_iIt[++m_iCurIt]=m_iStart;
}
	
bool CRange::getNext(int& val)
{
	bool flag;
	if (m_iIt[m_iCurIt] <= m_iEnd) {
		val=m_iIt[m_iCurIt]++;
		flag=true;
	}
	else {
		--m_iCurIt;
		flag=false;
	}
	return flag;
}

bool CRange::getNextIndex(CIndex& ind)
{
	char* str=(char*)ind.getString();
	bool flag;
	if (m_iIt[m_iCurIt] <= m_iEnd) {
//		itoa(m_iIt[m_iCurIt]++,str,10);
		int2str(m_iIt[m_iCurIt]++,str);
		flag=true;
	}
	else {
		--m_iCurIt;
		flag=false;
	}
	return flag;
}

bool CRange::In(int el)
{
	return (el >= m_iStart && el < m_iEnd)? true: false;
}

MIPSHELL_API std::ostream& operator<<(std::ostream& out, CRange& r)
{
	return (out << "[" << r.m_iStart << "," << r.m_iEnd << ")");
}

template <typename Tell>
CSet<Tell>::CSet()
{
	m_iCurIt=-1;
}

template <typename Tell>
CSet<Tell>::CSet(const char* str)
{
	m_iCurIt=-1;
	joinString(str);
}

template <typename Tell>
CSet<Tell>::~CSet()
{
}

template <typename Tell>
int CSet<Tell>::getSize()
{
	return static_cast<int>(m_Set.size());
}

template <typename Tell>
void CSet<Tell>::clear()
{
	m_Set.clear();
}

template <typename Tell>
void CSet<Tell>::initIt()
{
	m_pIt[++m_iCurIt]=m_Set.begin();
}

template <typename Tell>
bool CSet<Tell>::getNext(Tell& val)
{
	bool flag=(m_pIt[m_iCurIt] != m_Set.end())? true: false;
	if (flag)
		val=*(m_pIt[m_iCurIt]++);
	else --m_iCurIt;
	return flag;
}

template <typename Tell>
bool CSet<Tell>::getNextIndex(CIndex& ind)
{
	char* str=(char*)ind.getString();
	bool flag=(m_pIt[m_iCurIt] != m_Set.end())? true: false;
	if (flag)
		ell2str(str,*(m_pIt[m_iCurIt]++));
	else --m_iCurIt;
	return flag;
}

template <typename Tell>
bool CSet<Tell>::In(Tell el)
{
	return (m_Set.find(el) != m_Set.end())? true: false;
}

template <typename Tell>
void CSet<Tell>::add(const Tell& el)
{
	m_Set.insert(el);
}

template <typename Tell>
char* CSet<Tell>::joinString(const char* str, bool flag)
{
	int bad=0;
	char *st=const_cast<char*>(str);
	Tell ell;
	if (*(st+=strspn(st," \t\n")) != '{')
		bad=1;
	++st;
	while (!bad) {
		st=str2ell(st,ell);
		if (flag)
			m_Set.insert(ell);
		else 
			m_Set.erase(ell);
		st+=strspn(st," \t\n");
		if (*st == ',' || *st == ';')
			st+=strspn(++st," \t\n");
		else if (*st == '}') {
			++st;
			break;
		}
		else
			bad=2;
	} // while
	if (bad) {
		char msg[64];
		strcpy(msg,"Error parsing a set: ");
		if (bad == 1)
			strcat(msg,"{ is expected");
		else
			strcat(msg,", or ; or } is expected");
		throw CDataException(msg);
	}
	return st;
}

template <typename T>
char* str2ell(const char* str, CSet<T>& ell)
{
	return ell.joinString(str);
}

template <typename Tell>
bool operator%(Tell el, CSet<Tell>& s)
{
	return (s.m_Set.find(el) != s.m_Set.end())? true: false;
}

// operators
template <typename Tell>
CSet<Tell>& CSet<Tell>::operator=(CSet<Tell>& s)
{
	m_Set=s.m_Set;
	return *this;
}
template <typename Tell>
CSet<Tell>& CSet<Tell>::operator+=(CSet<Tell>& s)
{
	typename std::set<Tell>::iterator i=s.m_Set.begin(), e=s.m_Set.end();
	for (; i != e; i++)
		m_Set.insert(*i);

	return *this;
}

template <typename Tell>
CSet<Tell>& CSet<Tell>::operator-=(CSet<Tell>& s)
{
	typename std::set<Tell>::iterator i=s.m_Set.begin(), e=s.m_Set.end();
	for (; i != e; i++)
		m_Set.erase(*i);

	return *this;
}

/*
template <typename Tell>
CSet<Tell>& CSet<Tell>::operator+=(Tell el)
{
	insert(el);
	return *this;
}

template <typename Tell>
CSet<Tell>& CSet<Tell>::operator-=(Tell el)
{
	erase(el);
	return *this;
}
*/
template <typename Tell>
CSet<Tell>& CSet<Tell>::operator*=(CSet<Tell>& s)
{
	typename std::set<Tell>::iterator i=s.m_Set.begin(), e=s.m_Set.end();
	for (; i != e; i++)
		if (m_Set.find(*i) != m_Set.end())
			m_Set.erase(*i);

	return *this;
}

template <typename Tell>
CSet<Tell>& CSet<Tell>::operator+(CSet<Tell>& s)
{
	CSet<Tell> *pSet=new CSet<Tell>();
	*pSet=*this;
	*pSet+=s;
	return *pSet;
}

template <typename Tell>
CSet<Tell>& CSet<Tell>::operator-(CSet<Tell>& s)
{
	CSet<Tell> *pSet=new CSet<Tell>();
	std::set<Tell> &s3=pSet->m_Set, &s2=s.m_Set, &s1=m_Set;
	typename std::set<Tell>::iterator end1=s1.end(), end2=s2.end();
	for (typename std::set<Tell>::iterator it=s1.begin(); it != end1; it++)
		if (s2.find(*it) == end2)
			s3.insert(*it);

	return *pSet;
}

template <typename Tell>
CSet<Tell>& CSet<Tell>::operator*(CSet<Tell>& s)
{
	CSet<Tell> *pSet=new CSet<Tell>();
	for (typename std::set<Tell>::iterator it=m_Set.begin(); it != m_Set.end(); it++)
		if (s.m_Set.find(*it) != s.m_Set.end())
			pSet->m_Set.insert(*it);

	return *pSet;
}

//////////////////////////
// working with strings
//////////////////////////
template <typename Tell>
CSet<Tell>& CSet<Tell>::operator=(const char* str)
{
	joinString(str);
	return *this;
}

template <typename Tell>
CSet<Tell>& CSet<Tell>::operator+=(const char* str)
{
	joinString(str);
	return *this;
}

template <typename Tell>
CSet<Tell>& CSet<Tell>::operator-=(const char* str)
{
	joinString(str,false);
	return *this;
}

template <typename Tell>
CSet<Tell>& CSet<Tell>::operator*=(const char* str)
{
	CSet<Tell> s(str);
	return *this*=s;
}

template <typename Tell>
CSet<Tell>& CSet<Tell>::operator+(const char* str)
{
	CSet<Tell> *pSet = new CSet<Tell>(str);
	return *pSet+=*this;
}

template <typename Tell>
CSet<Tell>& CSet<Tell>::operator-(const char* str)
{
	CSet<Tell> *pSet=new CSet<Tell>();
	*pSet=*this;
	*pSet-=str;
	return *pSet;
}

template <typename Tell>
CSet<Tell>& CSet<Tell>::operator*(const char* str)
{
	CSet<Tell> *pSet=new CSet<Tell>(str);
	return *pSet*=*this;
}

template <typename Tell>
CSet<Tell>& operator+(const char* str, CSet<Tell>& s)
{
	return s+str;
}

template <typename Tell>
CSet<Tell>& operator-(const char* str, CSet<Tell>& s)
{
	CSet<Tell> *pSet=new CSet<Tell>(str); 
	return *pSet-=s;
}

template <typename Tell>
CSet<Tell>& operator*(const char* str, CSet<Tell>& s)
{
	return s * str;
}

template <typename Tell>
bool operator%(const char* str, CSet<Tell>& s)
{
	Tell el;
	str2ell(str,el);
	return (s.m_Set.find(el) != s.m_Set.end())? true: false;
}

template <typename Tell>
std::istream& operator>>(std::istream& in, CSet<Tell>& s)
{
	Tell el;
	int bad=0;
	char ch=0;
	std::locale lc=in.imbue(std::locale());
	for (in >> ch; !in.eof() && ch != '{'; in >> ch);
	if (ch != '{')
		bad=1;
	while (!bad) {
		in >> el;
		s.add(el);
		for (in >> ch; !in.eof() && (ch == ' ' || ch == '\t' || ch == '\n'); in >> ch);
		if (in.eof())
			bad=2;
		else if (ch == '}')
			break;
		else if (ch != ',' && ch != ';')
			bad=2;
	}
	in.imbue(lc);
	if (bad) {
		char msg[64];
		if (bad == 1)
			strcpy(msg,"{ is expected when reading a set");
		else
			strcpy(msg,", or ; or } is expected when reading a set");
		throw new CDataException(msg);
	}
	return in;
}

template <typename Tell>
std::ostream& operator<<(std::ostream& out, CSet<Tell>& s)
{
	typename std::set<Tell>::const_iterator i,e;
	std::locale lc=out.imbue(std::locale());
	out << "{";
	i=s.m_Set.begin();
	e=s.m_Set.end();
	if (i != e) {
		out << *i;
		for (; ++i != e;)
			out << "," << *i;
	}
	out << "}";
	out.imbue(lc);
	return out;
}

//////////////////////////////////
// Comparizons
///////////////
template <typename Tell>
bool CSet<Tell>::operator<(const CSet<Tell>& s)
{
	return std::lexicographical_compare(m_Set.begin(),m_Set.end(),s.m_Set.begin(),s.m_Set.end());
}

///// \cond NEWandDELoperators
///////////////////////////////////////////////////////
//template <typename Tell>
//void* CSet<Tell>::operator new(size_t iSize) throw(CMemoryException)
//{
//	void *p=malloc(iSize);
//	if (!p) {
//		throw new CMemoryException("CSet<Tell>::operator new");
//	}
//	return p;
//}
//
//template <typename Tell>
//void CSet<Tell>::operator delete(void* pPtr) throw()
//{
//	free(pPtr);
//}
//
//template <typename Tell>
//void* CSet<Tell>::operator new[](size_t iSize) throw(CMemoryException)
//{
//	void *p=malloc(iSize);
//	if (!p) {
//		throw new CMemoryException("CSet<Tell>::operator new[]");
//	}
//	return p;
//}
//
//template <typename Tell>
//void CSet<Tell>::operator delete[](void* pPtr) throw()
//{
//	free(pPtr);
//}
///// \endcond

////////////////////////////////
// CSet Instantiations
template class MIPSHELL_API CSet<int>;
template class MIPSHELL_API CSet<double>;
template class MIPSHELL_API CSet<CIndex>;

///////////////////////////////
// CSet friends instantiations
/////////////
template MIPSHELL_API CSet<int>&  operator+<int>(const char* str, CSet<int>& s);
template MIPSHELL_API CSet<int>& operator-(const char* str, CSet<int>& s);
template MIPSHELL_API CSet<int>& operator*(const char* str, CSet<int>& s);
template MIPSHELL_API bool operator%(int el, CSet<int>& s);
template MIPSHELL_API bool operator%(const char* el, CSet<int>& s);
template MIPSHELL_API std::istream& operator>>(std::istream& in, CSet<int>& s);
template MIPSHELL_API std::ostream& operator<<(std::ostream& out, CSet<int>& s);

template MIPSHELL_API CSet<double>& operator+(const char* str, CSet<double>& s);
template MIPSHELL_API CSet<double>& operator-(const char* str, CSet<double>& s);
template MIPSHELL_API CSet<double>& operator*(const char* str, CSet<double>& s);
template MIPSHELL_API bool operator%(double el, CSet<double>& s);
template MIPSHELL_API bool operator%(const char* el, CSet<double>& s);
template MIPSHELL_API std::istream& operator>>(std::istream& in, CSet<double>& s);
template MIPSHELL_API std::ostream& operator<<(std::ostream& out, CSet<double>& s);

template MIPSHELL_API CSet<CIndex>& operator+(const char* str, CSet<CIndex>& s);
template MIPSHELL_API CSet<CIndex>& operator-(const char*, CSet<CIndex>& s);
template MIPSHELL_API CSet<CIndex>& operator*(const char*, CSet<CIndex>& s);
template MIPSHELL_API bool operator%(const char* el, CSet<CIndex>& s);
template MIPSHELL_API std::istream& operator>>(std::istream& in, CSet<CIndex>& s);
template MIPSHELL_API std::ostream& operator<<(std::ostream& out, CSet<CIndex>& s);
