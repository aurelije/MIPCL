#include "primer.h"

Cprimer::Cprimer(const char* name): CProblem(name)
{
// TODO: Create your model
}

#ifndef __ONE_THREAD_
Cprimer::Cprimer(const Cprimer &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of Cprimer::Cprimer(Cprimer &other, int thread)

CMIP* Cprimer::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cprimer(*static_cast<Cprimer*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cprimer::~Cprimer()
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

#include "primer.aux"

