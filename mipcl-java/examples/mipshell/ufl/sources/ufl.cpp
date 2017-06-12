#include "ufl.h"

Cufl::Cufl(const char* name): CProblem(name)
{
	readUFL(name);
}

#ifndef __ONE_THREAD_
Cufl::Cufl(const Cufl &other, int thread): CProblem(other,thread)
{
} // end of Cufl::Cufl(Cufl &other, int thread)

CMIP* Cufl::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cufl(*static_cast<Cufl*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cufl::~Cufl()
{
#ifndef __ONE_THREAD_
	if (!m_iThread) {
#endif
#ifndef __ONE_THREAD_
	}
#endif
// TODO: delete not-shared members
}
//////////////////// implementation
void Cufl::readUFL(const char* name)
{
	std::ifstream fin(name);
	fin >> c >> f >> b;
	fin >> q;
//	std::cerr << c << std::endl;
//	std::cerr << f << std::endl;
//	std::cerr << b << std::endl;
	fin.close();
}


#include "ufl.aux"

