#include <cmath>
#include <cstdlib>
#include <cstring>
#include "Set.h"
//#include "ellstr.h"
//#include <Except.h>

extern void int2str(int, char*);

char* str2ell(const char* str, int& ell)
{
	char *s;
	ell=strtol(str,&s,10);
	return s;
}

char* str2ell(const char* str, double& ell)
{
	char *s;
	ell=strtod(str,&s);
	return s;
}

char* str2ell(const char* str, CIndex& ell)
{
	char *s1, *s2;
	if ((s1=strchr(const_cast<char*>(str),'('))) {
		s2=strchr(const_cast<char*>(str),')');
		*s2=0;
		++s1;
		ell=s1;
		*s2=')';
		s2++;
	}
	else {
		ell=str;
		s2=const_cast<char*>(str)+strlen(str);
	}
	return s2;
}

void ell2str(char*& str, const int& ell)
{
//	itoa(ell,str,10);
	int2str(ell,str);
}

void ell2str(char*& str, const double& ell)
{
#ifdef _WIN32
	_gcvt(ell,MAX_INDEX_SIZE,str);
#else
	gcvt(ell,MAX_INDEX_SIZE,str);
#endif
}

void ell2str(char*& str, const CIndex& ell)
{
	strcpy(str,ell.getString());
}

