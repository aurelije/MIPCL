#include "primer.h"

Cprimer::Cprimer(const char* name): CProblem(name)
{
// TODO: Create your model
}

#ifdef __THREADS_
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

#include "primer.aux"

