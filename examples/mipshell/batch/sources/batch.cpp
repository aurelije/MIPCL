#include "batch.h"
#include <cstring>

Cbatch::Cbatch(const char* name): CProblem(name)
{
	char fileName[128];
	strcpy(fileName,name);
	strcat(fileName,".txt");
	readSTN(fileName);
}

#ifdef __THREADS_
Cbatch::Cbatch(const Cbatch &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of Cbatch::Cbatch(Cbatch &other, int thread)

CMIP* Cbatch::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cbatch(*static_cast<Cbatch*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cbatch::~Cbatch()
{
#ifdef __THREADS_
	if (!m_iThread) {
#endif
// TODO: delete shared members 
#ifdef __THREADS_
	}
#endif
// TODO: delete notshared members 
} // end of Cbatch::~Cbatch

//////////////////// implementation
void Cbatch::readSTN(const char* fileName)
{
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw new CFileException("Cbatch::readSTN",fileName);
	}
	char str[256];
	int w,q;
	INDEX state, tsk, unit;

	fin >> H;
// reading tasks
	for (fin.getline(str,255); strcmp(str,"begin(tasks)"); fin.getline(str,255));
	for (fin >> str; strcmp(str,"end(tasks)"); fin >> str) {
		TASKS.add(tsk=str);
		fin >> K.add(tsk);
		fin >> Sin.add(tsk);
		for (Sin(tsk).initIt(); Sin(tsk).getNext(state);)
			fin >> rho.add(state,tsk);
		fin >> Sout.add(tsk);
		w=0;
		for (Sout(tsk).initIt(); Sout(tsk).getNext(state);) {
			fin >> rho.add(tsk,state) >> q;
			p.add(tsk,state)=q;
			if (q > w) w=q;
		}
		dur.add(tsk)=w;
	}
// reading states
	for (fin.getline(str,255); strcmp(str,"begin(states)"); fin.getline(str,255));
	for (fin >> str; strcmp(str,"end(states)"); fin >> str) {
		STATES.add(state=str);
		Tin.add(state);
		Tout.add(state);
		fin >> u.add(state) >> z0.add(state) >> cs.add(state) >> cp.add(state);
	}
	for (TASKS.initIt(); TASKS.getNext(tsk);) {
		for (Sin(tsk).initIt(); Sin(tsk).getNext(state);)
			Tout(state).add(tsk);
		for (Sout(tsk).initIt(); Sout(tsk).getNext(state);)
			Tin(state).add(tsk);
	}
// reading units
	for (fin.getline(str,255); strcmp(str,"begin(units)"); fin.getline(str,255));
	for (fin >> str; strcmp(str,"end(units)"); fin >> str) {
		UNITS.add(unit=str);
		fin >> I.add(unit) >> Vmin.add(unit) >> Vmax.add(unit);
	}
	fin.close();
} // end of Cbatch::readSTN

void Cbatch::printSolution(VAR_ARRAY& y, VAR_ARRAY& z)
{
	char fileName[128];
	getprobname(fileName);
	strcat(fileName,".sol");
	std::ofstream fout(fileName);
	if (!fout.is_open()) {
		throw new CFileException("Cbatch::ptintSol",fileName);
	}
	
	INDEX state, tsk, unit;
	int t;
	
	fout << "Objective = " << getobj() << "\nStock: ";
	for (STATES.initIt(); STATES.getNext(state);)
		fout << getval(z(state,H)) << " ";
	fout << "\nSchedule:\n";
	for (UNITS.initIt(); UNITS.getNext(unit);) {
		fout << unit << ":\n";
		for (t=0; t < H; ++t)
			for (TASKS.initIt(); TASKS.getNext(tsk);)
				if (getval(y(tsk,unit,t)) > 1.0e-7)
			fout << "batch of task " << tsk
				 << ": starts at " << t 
				 << ", size = " << getval(y(tsk,unit,t)) << std::endl;
	}
	fout.close();
} // end of Cbatch::printSolution

#include "batch.aux"

