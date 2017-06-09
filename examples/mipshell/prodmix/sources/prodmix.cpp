#include "prodmix.h"

Cprodmix::Cprodmix(const char* name): CProblem(name)
{
// TODO: Create your model
}

#ifdef __THREADS_
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

#include "prodmix.aux"

