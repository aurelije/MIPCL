#include "refining.h"

Crefining::Crefining(const char* name): CProblem(name)
{
// TODO: Create your model
}

#ifndef __ONE_THREAD_
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
#ifndef __ONE_THREAD_
	if (!m_iThread) {
#endif
// TODO: delete shared members 
#ifndef __ONE_THREAD_
	}
#endif
// TODO: delete not-shared members
}

//////////////////// implementation

#include "refining.aux"

