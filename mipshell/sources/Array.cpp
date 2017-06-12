// Array.cpp: implementation of the CArray class.
//
//////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <cstring>
#include "Array.h"

extern char* str2ell(const char* str,int& ell);
extern char* str2ell(const char* str,double& ell);
extern char* str2ell(const char* str, CIndex& ell);

void setZero(int& a)
{
	a=0;
}

void setZero(double& a)
{
	a=0.0;
}

void setZero(CIndex& a)
{
	a="";
}

void setZero(CSet<CIndex>& a)
{
	a.clear();
}

void setZero(CSet<int>& a)
{
	a.clear();
}

void setZero(CSet<double>& a)
{
	a.clear();
}

////////////////////////////////////////////////////
// CArray
// Construction/Destruction
////////////////////////////////////////////////////
template<typename Tell>
CArray<Tell>::CArray(): m_iCurIt(0)
{
}

template<typename Tell>
CArray<Tell>::CArray(const char* str): m_iCurIt(0)
{
	operator=(str);
}

template<typename Tell>
CArray<Tell>::CArray(CArray& ar): m_iCurIt(0)
{
	m_Map=ar.m_Map;
}

template<typename Tell>
CArray<Tell>::~CArray()
{
}
////////////////////////////

///// \cond NEWandDELoperators
//template <typename Tell>
//void* CArray<Tell>::operator new(size_t iSize) throw(CMemoryException)
//{
//	void *p=malloc(iSize);
//	if (!p) {
//		throw new CMemoryException("CArray<Tell>::operator new");
//	}
//	return p;
//}
//
//template <typename Tell>
//void CArray<Tell>::operator delete(void* pPtr) throw()
//{
//	free(pPtr);
//}
//
//template <typename Tell>
//void* CArray<Tell>::operator new[](size_t iSize) throw(CMemoryException)
//{
//	void *p=malloc(iSize);
//	if (!p) {
//		throw new CMemoryException("CArray<Tell>::operator new[]");
//	}
//	return p;
//}
//
//template <typename Tell>
//void CArray<Tell>::operator delete[](void* pPtr) throw()
//{
//	free(pPtr);
//}
///// \endcond

template <typename Tell>
Tell& CArray<Tell>::getZero()
{
	setZero(m_Zero);
	return m_Zero;
}

template <class Tell>
int CArray<Tell>::getSize()
{
	return static_cast<int>(m_Map.size());
}

////////////////////////////////////////////
// Iterators
//////////////
template <class Tell>
void CArray<Tell>::initIt()
{
	m_pIt[++m_iCurIt]=m_Map.begin();
}

template <class Tell>
bool CArray<Tell>::getNextIndex(CIndex& ind)
{
	bool flag=(m_pIt[m_iCurIt] != m_Map.end())? true: false;
	if (flag)
		ind=(m_pIt[m_iCurIt]++)->first;
	else --m_iCurIt;
	return flag;
}

template <class Tell>
bool CArray<Tell>::getNext(CIndex& ind)
{
	bool flag=(m_pIt[m_iCurIt] != m_Map.end())? true: false;
	if (flag)
		ind=(m_pIt[m_iCurIt]++)->first;
	else --m_iCurIt;
	return flag;
}

////////////////////////////////////////////
// assinment
/////////////
template<typename Tell> 
Tell& CArray<Tell>::operator[](const CIndex& ind)
{
	typename std::map<CIndex,Tell>::iterator it=m_Map.find(ind);
	return (it == m_Map.end())? getZero(): (*it).second;
}

template<typename Tell> 
Tell& CArray<Tell>::operator()(const CIndex& ind)
{
	typename std::map<CIndex,Tell>::iterator it=m_Map.find(ind);
	return (it == m_Map.end())? getZero(): (*it).second;
}

template<typename Tell> 
Tell& CArray<Tell>::operator()(const CIndex& ind0, const CIndex& ind1)
{
	CIndex ind(ind0,ind1);
	typename std::map<CIndex,Tell>::iterator it=m_Map.find(ind);
	return (it == m_Map.end())? getZero(): (*it).second;
}
	
template<typename Tell> 
Tell& CArray<Tell>::operator()(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2)
{
	CIndex ind(ind0,ind1,ind2);
	typename std::map<CIndex,Tell>::iterator it=m_Map.find(ind);
	return (it == m_Map.end())? getZero(): (*it).second;
}
	
template<typename Tell> 
Tell& CArray<Tell>::operator()(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2, const CIndex& ind3)
{
	CIndex ind(ind0,ind1,ind2,ind3);
	typename std::map<CIndex,Tell>::iterator it=m_Map.find(ind);
	return (it == m_Map.end())? getZero(): (*it).second;
}

template<typename Tell> 
Tell& CArray<Tell>::add(const CIndex& ind)
{
	return m_Map[ind]=getZero();
}

template<typename Tell> 
Tell& CArray<Tell>::add(const CIndex& ind0, const CIndex& ind1)
{
	CIndex ind(ind0,ind1);
	return m_Map[ind]=getZero();
}
	
template<typename Tell> 
Tell& CArray<Tell>::add(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2)
{
	CIndex ind(ind0,ind1,ind2);
	return m_Map[ind]=getZero();
}
	
template<typename Tell> 
Tell& CArray<Tell>::add(const CIndex& ind0, const CIndex& ind1, 
						   const CIndex& ind2, const CIndex& ind3)
{
	CIndex ind(ind0,ind1,ind2,ind3);
	return m_Map[ind]=getZero();
}

template<typename Tell> 
CArray<Tell>& CArray<Tell>::operator=(const char* str)
{
	int bad=0;
	char *s=const_cast<char*>(str);
	Tell ell;
	CIndex ind;
	if (*(s+=strspn(s," \t\n")) == '{')
		++s;
	else bad=1;
	while (!bad) {
		s=str2ell(s,ind);
		s+=strspn(s," \t\n");
		if (*s != '=' || s[1] != '>')
			bad=2;
		else {
			s+=strspn(s+=2," \t\n");
			s=str2ell(s,ell);
			add(ind)=ell;
			s+=strspn(s," \t\n");
			if (*s == '}') {
				++s;
				break;
			}
			else if (*s == 0)
				bad=3;
			else if (*s == ',' || *s == ';')
				++s;
			else bad=4;
		}
	} // end while
	if (bad) {
		char msg[64];
		strcpy(msg,"Error parsing a set: ");
		switch (bad) {
		case 1: strcat(msg,"{ is expected"); break;
		case 2: strcat(msg,"=> is expected"); break;
		case 3: strcat(msg,"} is expected"); break;
		case 4: strcat(msg,", or ; is expected"); break;
		}
		throw CDataException(msg);
	}
	return *this;
}

int SkipHashSeparator(std::istream& in) 
{
	int ret_val=0;
	char ch;
	for (in >> ch; !in.eof() && (ch == ' ' || ch == '\t' || ch == '\n'); in >> ch);
	if (in.eof())
		ret_val=1;
	else if (ch != '=')
		ret_val = 3;
	else {
		in >> ch;
		if (ch != '>')
			ret_val = 3;
	}
	return ret_val;
}

void throwDataException(int bad)
{
	char msg[64];
	strcpy(msg,"Error when reading array: ");
	if (bad == 1)
		strcpy(msg,"{ is expected when reading a set");
	else if (bad == 2)
		strcpy(msg,", or ; or } is expected when reading a set");
	else
		strcpy(msg,"=> is expected when reading array");
	throw new CDataException(msg);
}

template<typename Tell> 
std::istream& operator>>(std::istream& in, CArray<Tell> &arr)
{
	int bad=0;
	char ch;
	CIndex ind;
	Tell ell;
	std::locale lc=in.imbue(std::locale());
	for (in >> ch; !in.eof() && (ch == ' ' || ch == '\t' || ch == '\n'); in >> ch);
	if (in.eof() || ch != '{')
		bad=1;
	while (!bad) {
// reading an index in the form (i1[,[i2,[i3,[i4]]])
		in >> ind;
// skip separator =>
		if (!(bad=SkipHashSeparator(in))) {
// read an element
			in >> ell;
			arr.add(ind)=ell;
			for (in >> ch; (ch == ' '|| ch == '\t' || ch == '\n') && !in.eof(); in >> ch);
			if (ch == '}')
				break;
			else if (in.eof() || (ch != ',' && ch != ';'))
				bad=2;
		}
	}
	in.imbue(lc);
	if (bad)
		throwDataException(bad);

	return in;
}

template<typename Tell>
std::ostream& operator<<(std::ostream& out, CArray<Tell>& arr)
{
	std::locale lc=out.imbue(std::locale());
	typename std::map<CIndex,Tell>::iterator it=arr.m_Map.begin(), 
		end=arr.m_Map.end();
	out << "{";
	if (it != end) {
		out << "\n" << (*it).first << " => " << (*it).second;
		for (++it; it != end; ++it) {
			out << ",\n" << (*it).first << " => " << (*it).second;
		}
	}
	out << "\n}";
	out.imbue(lc);
	return out;
}

//template<typename Tell>
//Tell& CArray<Tell>::max()
//{
//	typename std::map<CIndex,Tell>::iterator imax, i,e;
//	imax=i=m_Map.begin();
//	e=m_Map.end();
//	for (++i; i != e; ++i) {
//		if ((*imax).second < (*i).second)
//			imax=i;
//	}
//	return (*imax).second;
//}
//
//template<typename Tell>
//Tell& CArray<Tell>::min()
//{
//	typename std::map<CIndex,Tell>::iterator imin, i,e;
//	imin=i=m_Map.begin();
//	e=m_Map.end();
//	for (++i; i != e; ++i) {
//		if ((*imin).second < (*i).second)
//			imin=i;
//	}
//	return (*imin).second;
//}
////////////////////////////////////////////////
// CArray<void*> implementation

CArray<void*>::CArray()
{
	m_iCurIt=0;	
}

CArray<void*>::CArray(CArray& ar)
{
	m_iCurIt=0;
	m_Map=ar.m_Map;
}

CArray<void*>::~CArray()
{
}

///// \cond NEWandDELoperators
////////////////////////////
//void* CArray<void*>::operator new(size_t iSize)
//{
//	return malloc(iSize);
//}
//
//void CArray<void*>::operator delete(void* pPtr)
//{
//	free(pPtr);
//}
//
//void* CArray<void*>::operator new[](size_t iSize)
//{
//	return malloc(iSize);
//}
//
//void CArray<void*>::operator delete[](void* pPtr)
//{
//	free(pPtr);
//}
///// \endcond

int CArray<void*>::getSize()
{
	return static_cast<int>(m_Map.size());
}

////////////////////////////////////////////
// Iterators
//////////////
void CArray<void*>::initIt()
{
	m_pIt[++m_iCurIt]=m_Map.begin();
}

bool CArray<void*>::getNextIndex(CIndex& ind)
{
	bool flag=(m_pIt[m_iCurIt] != m_Map.end())? true: false;
	if (flag)
		ind=(m_pIt[m_iCurIt]++)->first;
	else --m_iCurIt;
	return flag;
}

bool CArray<void*>::getNext(CIndex& ind)
{
	bool flag=(m_pIt[m_iCurIt] != m_Map.end())? true: false;
	if (flag)
		ind=(m_pIt[m_iCurIt]++)->first;
	else --m_iCurIt;
	return flag;
}
//////////////////////////////////
// 2 function for using in delete
void* CArray<void*>::getFirstPtr()
{
	return (*m_pIt=m_Map.begin())->second;
}

void* CArray<void*>::getNextPtr()
{
	void *p=0;
	if (*m_pIt != m_Map.end())
		if (++(*m_pIt) != m_Map.end())
			p=(*m_pIt)->second; 
	return p;
}

const CIndex& CArray<void*>::getCurIndex()
{
	return (*m_pIt)->first;
}

////////////////////////////////////////////
// assinment
/////////////
void* CArray<void*>::operator[](const CIndex& ind)
{
	std::map<CIndex,void*>::iterator it=m_Map.find(ind);
	return (it == m_Map.end())? 0: (*it).second;
}
 
void* CArray<void*>::operator()(const CIndex& ind)
{
	std::map<CIndex,void*>::iterator it=m_Map.find(ind);
	return (it == m_Map.end())? 0: (*it).second;
}
 
void* CArray<void*>::operator()(const CIndex& ind0, const CIndex& ind1)
{
	CIndex ind(ind0,ind1);
	std::map<CIndex,void*>::iterator it=m_Map.find(ind);
	return (it == m_Map.end())? 0: (*it).second;
}
	
void* CArray<void*>::operator()(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2)
{
	CIndex ind(ind0,ind1,ind2);
	std::map<CIndex,void*>::iterator it=m_Map.find(ind);
	return (it == m_Map.end())? 0: (*it).second;
}
	
void* CArray<void*>::operator()(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2, const CIndex& ind3)
{
	CIndex ind(ind0,ind1,ind2,ind3);
	std::map<CIndex,void*>::iterator it=m_Map.find(ind);
	return (it == m_Map.end())? 0: (*it).second;
}

void CArray<void*>::add(void *pEll, const CIndex& ind)
{
	m_Map[ind]=pEll;
}
 
void CArray<void*>::add(void *pEll, const CIndex& ind0, const CIndex& ind1)
{
	CIndex ind(ind0,ind1);
	m_Map[ind]=pEll;
}
	 
void CArray<void*>::add(void *pEll, const CIndex& ind0, const CIndex& ind1, const CIndex& ind2)
{
	CIndex ind(ind0,ind1,ind2);
	m_Map[ind]=pEll;
}
	 
void CArray<void*>::add(void *pEll, const CIndex& ind0, const CIndex& ind1, 
						   const CIndex& ind2, const CIndex& ind3)
{
	CIndex ind(ind0,ind1,ind2,ind3);
	m_Map[ind]=pEll;
}
//////////////////////////////////////////////
// CArray<Tell*> implementation

// Construction/Destruction
////////////////////////////////////////////////////
template<typename Tell>
CArray<Tell*>::CArray(): Base() {}

template<typename Tell>
CArray<Tell*>::CArray(const char* str): Base()
{
	operator=(str);
}

template<typename Tell>
CArray<Tell*>::CArray(CArray& ar): Base()
{
	for (void *p=ar.getFirstPtr(); p; p=ar.getNextPtr()) {
		add(ar.getCurIndex())=*(new Tell(*static_cast<Tell*>(p)));
	}
}

template<typename Tell>
CArray<Tell*>::~CArray()
{
	for (Tell *p=static_cast<Tell*>(getFirstPtr()); p; p=static_cast<Tell*>(getNextPtr())) {
		delete p;
	}
}
////////////////////////////
template <typename Tell>
Tell& CArray<Tell*>::getZero()
{
	setZero(m_Zero);
	return m_Zero;
}

///// \cond NEWandDELoperators
//t=emplate <typename Tell>
//void* CArray<Tell*>::operator new(size_t iSize)
//{
//	return malloc(iSize);
//}
//
//template <typename Tell>
//void CArray<Tell*>::operator delete(void* pPtr)
//{
//	free(pPtr);
//}
//
//template <typename Tell>
//void* CArray<Tell*>::operator new[](size_t iSize)
//{
//	return malloc(iSize);
//}
//
//template <typename Tell>
//void CArray<Tell*>::operator delete[](void* pPtr)
//{
//	free(pPtr);
//}
///// \endcond

////////////////////////////////////////////
// assinment
/////////////
template<typename Tell> 
Tell& CArray<Tell*>::operator[](const CIndex& ind)
{
	Tell* pEll = static_cast<Tell*>(Base::operator()(ind));
	return pEll? *pEll: getZero();
}

template<typename Tell> 
Tell& CArray<Tell*>::operator()(const CIndex& ind)
{
	Tell* pEll = static_cast<Tell*>(Base::operator()(ind));
	return pEll? *pEll: getZero();
}

template<typename Tell> 
Tell& CArray<Tell*>::operator()(const CIndex& ind0, const CIndex& ind1)
{
	Tell* pEll = static_cast<Tell*>(Base::operator()(ind0,ind1));
	return pEll? *pEll: getZero();
}
	
template<typename Tell> 
Tell& CArray<Tell*>::operator()(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2)
{
	Tell* pEll = static_cast<Tell*>(Base::operator()(ind0,ind1,ind2));
	return pEll? *pEll: getZero();
}
	
template<typename Tell> 
Tell& CArray<Tell*>::operator()(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2, const CIndex& ind3)
{
	Tell* pEll = static_cast<Tell*>(Base::operator()(ind0,ind1,ind2,ind3));
	return pEll? *pEll: getZero();
}

template<typename Tell> 
Tell& CArray<Tell*>::add(const CIndex& ind)
{
	Tell *pEll=new Tell;
	Base::add(pEll,ind);
	return *pEll;
}

template<typename Tell> 
Tell& CArray<Tell*>::add(const CIndex& ind0, const CIndex& ind1)
{
	Tell *pEll=new Tell;
	Base::add(pEll,ind0,ind1);
	return *pEll;
}
	
template<typename Tell> 
Tell& CArray<Tell*>::add(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2)
{
	Tell *pEll=new Tell;
	Base::add(pEll,ind0,ind1,ind2);
	return *pEll;
}
	
template<typename Tell> 
Tell& CArray<Tell*>::add(const CIndex& ind0, const CIndex& ind1, 
						   const CIndex& ind2, const CIndex& ind3)
{
	Tell *pEll=new Tell;
	Base::add(pEll,ind0,ind1,ind2,ind3);
	return *pEll;
}

template<typename Tell> 
CArray<Tell*>& CArray<Tell*>::operator=(const char* str)
{
	int bad=0;
	char *s=const_cast<char*>(str);
	Tell *ell;
	CIndex ind;
	if (*(s+=strspn(s," \t\n")) == '{')
		++s;
	else bad=1;
	while (!bad) {
		s=str2ell(s,ind);
		s+=strspn(s," \t\n");
		if (*s != '=' || s[1] != '>')
			bad=2;
		else {
			s+=strspn(s+=2," \t\n");
			s=(ell = new Tell())->joinString(s);
			add(ind)=*ell;
			s+=strspn(s," \t\n");
			if (*s == '}') {
				++s;
				break;
			}
			else if (*s == 0)
				bad=3;
			else if (*s == ',' || *s == ';')
				++s;
			else bad=4;
		}
	} // end while
	if (bad) {
		char msg[64];
		strcpy(msg,"Error parsing a set: ");
		switch (bad) {
		case 1: strcat(msg,"{ is expected"); break;
		case 2: strcat(msg,"=> is expected"); break;
		case 3: strcat(msg,"} is expected"); break;
		case 4: strcat(msg,", or ; is expected"); break;
		}
		throw CDataException(msg);
	}
	return *this;
}

template<typename Tell>
std::istream& operator>>(std::istream& in, CArray<Tell*> &arr)
{
	int bad=0;
	char ch;
	CIndex ind;
	Tell *ell;
	std::locale lc=in.imbue(std::locale());
	for (in >> ch; !in.eof() && (ch == ' ' || ch == '\t' || ch == '\n'); in >> ch);
	if (in.eof() || ch != '{')
		bad=1;
	while (!bad) {
// reading an index in the form (i1[,[i2,[i3,[i4]]])
		in >> ind;
// skip separator =>
		if (!(bad=SkipHashSeparator(in))) {
// read an element
			ell = new Tell;
			in >> *ell;
			arr.add(ind)=*ell;
			for (in >> ch; (ch == ' '|| ch == '\t' || ch == '\n') && !in.eof(); in >> ch);
			if (ch == '}')
				break;
			else if (in.eof() || (ch != ',' && ch != ';'))
				bad=2;
		}
	}
	in.imbue(lc);
	if (bad)
		throwDataException(bad);

	return in;
}

template<typename Tell>
std::ostream& operator<<(std::ostream& out, CArray<Tell*>& arr)
{
	std::locale lc=out.imbue(std::locale());
	out << "{";
	Tell *p=static_cast<Tell*>(arr.getFirstPtr());
	if (p) {
		out << "\n" << arr.getCurIndex() << " => " << *p;
		while (p=static_cast<Tell*>(arr.getNextPtr())) {
			out << ",\n" << arr.getCurIndex() << " => " << *p;
		}
	}
	out << "\n}";
	out.imbue(lc);
	return out;
}

////////////////////////////////////////////////
// Instantiation
//////////////////

template class MIPSHELL_API CArray<int>;
template class MIPSHELL_API CArray<double>;
template class MIPSHELL_API CArray<CIndex>;

template MIPSHELL_API std::istream& operator>>(std::istream& in, CArray<int>& arr);
template MIPSHELL_API std::ostream& operator<<(std::ostream& out, CArray<int>& arr);
template MIPSHELL_API std::istream& operator>>(std::istream& in, CArray<double>& arr);
template MIPSHELL_API std::ostream& operator<<(std::ostream& out, CArray<double>& arr);
template MIPSHELL_API std::istream& operator>>(std::istream& in, CArray<CIndex>& arr);
template MIPSHELL_API std::ostream& operator<<(std::ostream& out, CArray<CIndex>& arr);

template class MIPSHELL_API CArray<CSet<int>*>;
template class MIPSHELL_API CArray<CSet<double>*>;
template class MIPSHELL_API CArray<CSet<CIndex>*>;

template MIPSHELL_API std::istream& operator>>(std::istream& in, CArray<CSet<int>*>& arr);
template MIPSHELL_API std::ostream& operator<<(std::ostream& out, CArray<CSet<int>*>& arr);
template MIPSHELL_API std::istream& operator>>(std::istream& in, CArray<CSet<double>*>& arr);
template MIPSHELL_API std::ostream& operator<<(std::ostream& out, CArray<CSet<double>*>& arr);
template MIPSHELL_API std::istream& operator>>(std::istream& in, CArray<CSet<CIndex>*>& arr);
template MIPSHELL_API std::ostream& operator<<(std::ostream& out, CArray<CSet<CIndex>*>& arr);
