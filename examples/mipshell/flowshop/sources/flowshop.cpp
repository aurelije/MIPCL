#include "flowshop.h"
#include <cstring>

Cflowshop::Cflowshop(const char* name): CProblem(name)
{
	char fileName[128];
	strcpy(fileName,name);
	strcat(fileName,".txt");
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw new CFileException("Cflowshop::Cflowshop",fileName);
	}
	fin >> p;
	fin.close();
}

#ifdef __THREADS_
Cflowshop::Cflowshop(const Cflowshop &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of Cflowshop::Cflowshop(Cflowshop &other, int thread)

CMIP* Cflowshop::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cflowshop(*static_cast<Cflowshop*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cflowshop::~Cflowshop()
{
#ifdef __THREADS_
	if (!m_iThread) {
#endif
// TODO: delete shared members 
#ifdef __THREADS_
	}
#endif
// TODO: delete notshared members 
}

//////////////////// implementation

#include "flowshop.aux"

