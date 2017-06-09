#include <fstream>
#include <cstring>
#include "genassign.h"

Cgenassign::Cgenassign(const char* name): CProblem(name)
{
	char fileName[128];
	strcpy(fileName,name);
	strcat(fileName,".txt");
	readData(fileName);
}

#ifdef __THREADS_
Cgenassign::Cgenassign(const Cgenassign &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of Cgenassign::Cgenassign(Cgenassign &other, int thread)

CMIP* Cgenassign::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cgenassign(*static_cast<Cgenassign*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cgenassign::~Cgenassign()
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
void Cgenassign::readData(const char* fileName)
{
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw new CFileException("Cgenassign::readData",fileName);
	}
	fin >> l;
	fin >> c;
	fin >> p;
	fin.close();
} // end of Cgenassign::readData

#include "genassign.aux"

