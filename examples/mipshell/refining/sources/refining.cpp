#include "refining.h"

Crefining::Crefining(const char* name): CProblem(name)
{
// TODO: Create your model
}

#ifdef __THREADS_
Crefining::Crefining(const Crefining &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of Crefining::Crefining(Crefining &other, int thread)

CMIP* Crefining::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Crefining(*static_cast<Crefining*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Crefining::~Crefining()
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

#include "refining.aux"

