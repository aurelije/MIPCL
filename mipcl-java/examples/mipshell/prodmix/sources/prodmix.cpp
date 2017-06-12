#include "prodmix.h"

Cprodmix::Cprodmix(const char* name): CProblem(name)
{
// TODO: Create your model
}

#ifndef __ONE_THREAD_
Cprodmix::Cprodmix(const Cprodmix &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of Cprodmix::Cprodmix(Cprodmix &other, int thread)

CMIP* Cprodmix::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cprodmix(*static_cast<Cprodmix*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cprodmix::~Cprodmix()
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

#include "prodmix.aux"

