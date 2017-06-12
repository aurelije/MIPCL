// Problem.cpp: implementation of the CProblem class.
//
//////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <cmath>
#include <except.h>
#include "Var.h"
#include "Ctr.h"
#include "Vector.h"
#include "Function.h"
#include "DVar.h"
#include "Problem.h"

using std::ofstream;
using std::endl;

#ifndef NIL
#define NIL -1
#endif

extern void int2str(int, char*);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CProblem::init()
{
	m_iVarNum=m_iCtrNum=0;
	m_pLastVar=0;
	m_pLastCtr=0;
	m_pLastFunc=0;
	m_pLastDvar=0;
	m_pObj=0;
	m_bSense=true;
	m_dpVarVal=m_dpRedCost=m_dpPrice=0;
	m_iCutState=0;
	m_pLastCut=0;
	m_pSum = new CLinSum[10];
	for (int i=0; i < 10; ++i)
		m_pSum[i].makePermanent();
} // end of CProblem::init()

///// \cond NEWandDELoperators
//void* CProblem::operator new(size_t iSize) throw(CMemoryException)
//{
//	void *p=malloc(iSize);
//	if (!p) {
//		throw new CMemoryException("CProblem::operator new");
//	}
//	return p;
//}
//
//void CProblem::operator delete(void* pPtr) throw()
//{
//	free(pPtr);
//}
///// \endcond

CProblem::CProblem()
{
	m_strProblemName[0]='X'; m_strProblemName[1]='Y'; m_strProblemName[2]='Z'; m_strProblemName[3]=0;
	init();
}

CProblem::CProblem(const char* name): CMIP(name)
{
	init();
}

#ifndef __ONE_THREAD_
CProblem::CProblem(const CProblem &other, int thread): CMIP(other,thread)
{
	m_iVarNum=other.m_iVarNum;
	m_iCtrNum=other.m_iCtrNum;
	m_pLastVar=other.m_pLastVar;
	m_pLastCtr=other.m_pLastCtr;
	m_pLastFunc=other.m_pLastFunc;
	m_pLastDvar=other.m_pLastDvar;
	m_pObj=other.m_pObj;
	m_bSense=other.m_bSense;
	m_pLastCut=other.m_pLastCut;
	m_dpVarVal=other.m_dpVarVal;
	m_dpRedCost=other.m_dpRedCost;
	m_dpPrice=other.m_dpPrice;
	m_pSum = new CLinSum[10];
	for (int i=0; i < 10; ++i)
		m_pSum[i].makePermanent();
} // end of CProblem::CProblem(CProblem &other)

CMIP* CProblem::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new CProblem(*static_cast<CProblem*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

CProblem::~CProblem()
{
#ifndef __ONE_THREAD_
	if (!m_iThread) {
#endif	
	if (m_pObj)
		delete m_pObj;
	for (CCtr *pCtr1, *pCtr=m_pLastCtr; pCtr; pCtr=pCtr1) {
		pCtr1=pCtr->getPrev();
		delete pCtr;
	}

	for (CFunction *pFunc1, *pFunc=m_pLastFunc; pFunc; pFunc=pFunc1) {
		pFunc1=pFunc->getPrev();
		delete pFunc;
	}
	for (CDvar *pDvar1, *pDvar=m_pLastDvar; pDvar; pDvar=pDvar1) {
		pDvar1=pDvar->getPrev();
		delete pDvar;
	}
#ifndef __ONE_THREAD_
	}
#endif
	if (m_pSum)
		delete[] m_pSum;
}

void CProblem::setObj(CLinSum *lsum, bool bSense)
{
	m_bSense=bSense;
	m_pObj=lsum;
	lsum->makePermanent();
}

void CProblem::setObj(int col, double cost)
{
	if (!m_bSense)
		cost=-cost;
	m_dpC[col]=ldexp(cost,m_cpColScale[col]);
	m_bDualFeasible=false;
}

void CProblem::setUb(int col, double ub)
{
	if (ub < m_dVarInf)
		ub=ldexp(ub,m_cpColScale[col]);
	setVarUpBound(col,ub);
	m_bPrimeFeasible=false;
}

void CProblem::setLb(int col, double lb)
{
	if (lb > -m_dVarInf)
		lb=ldexp(lb,m_cpColScale[col]);
	setVarLoBound(col,lb);
	m_bPrimeFeasible=false;
}

void CProblem::load()
{
	char *str=m_sWarningMsg;
	int sz,col,r1,r2,r3,n, m, nz;
	CVar* pVar;
	CCtr* pCtr;
	CTerm* pTerm;
	CFunction* pFunc;
	CDvar* pDvar;
	m=nz=0;

	for (pCtr=m_pLastCtr; pCtr; pCtr=pCtr->getPrev()) {
		++m;
		for (pTerm=pCtr->getLastTerm(); pTerm; pTerm=pTerm->getPrev()) {
			++nz;
		}
	}
	n=m_iVarNum;
	m_iCtrNum=m;
	for (pFunc=m_pLastFunc; pFunc; pFunc=pFunc->getPrev()) {
		m+=3;
		n+=(sz=pFunc->getSize());
		nz+=(3*sz+2);
	}
	for (pDvar=m_pLastDvar; pDvar; pDvar=pDvar->getPrev()) {
		m+=2;
		n+=(sz=pDvar->getSize());
		nz+=(2*sz+1);
	}
	openMatrix(m,n,nz);
	n=m=0;
	for (pVar=m_pLastVar; pVar; pVar=pVar->getPrev()) {
		nz=0;
		if (pVar->isInteger()) {
			nz=VAR_INT;
			if (pVar->isBinary())
				nz|=VAR_BIN;
		}
		col=CLP::addVar(n,nz,0.0,pVar->getLb(),pVar->getUb());
		if (nz)
			setVarPriority(col,pVar->getPriority());
		pVar->setHandle(n++);
	}

	int *ipCol=m_ipArray, *ipInd=(int*)m_dpC;
	double l,u,w,*dpVal=m_dpArray;

	for (int i=0; i < n; ++i) {
		ipInd[i]=NIL;
	}

	strcpy(str,"ctr_");
	for (pCtr=m_pLastCtr; pCtr; pCtr=pCtr->getPrev()) {
		if (pCtr->getName()[0] == 0) { // set name
//			itoa(m,str+4,10);
			int2str(m,str+4);
			pCtr->setName(str);
		}
		l=pCtr->getLHS();
		u=pCtr->getRHS();
        sz=0;
		for (pTerm=pCtr->getLastTerm(); pTerm; pTerm=pTerm->getPrev()) {
			w=pTerm->getCoeff();
			if ((pVar=pTerm->getVar())) {
				if (ipInd[col=pVar->getHandle()] == NIL) {
					ipCol[ipInd[col]=sz]=col;
					dpVal[sz++]=w;
				}
				else
					dpVal[ipInd[col]]+=w;
			}
			else {
				if (l > -INF)
					l-=w;
				if (u < INF)
					u-=w;
			}
		} // for (pTerm=pCtr->getLastTerm()
		pCtr->setHandle(m);
		if (sz)
			safeAddRow(m++,0,l,u,sz,dpVal,ipCol);
		for (int i=0; i < sz; ++i) {
			ipInd[ipCol[i]]=NIL;
		}
	} // end of for (pCtr=m_pLastCtr
// functions
	for (pFunc=m_pLastFunc; pFunc; pFunc=pFunc->getPrev()) {
		col=n;
		sz=pFunc->getSize();
		for (int i=0; i < sz; ++i) {
			CLP::addVar(n++,0,0.0,0.0,1.0);
		}
		CLP::addCtr(r1=m++,0,0,0);
		CLP::addCtr(r2=m++,0,0,0);
		CLP::addCtr(r3=m++,CTR_SOS2,1.0,1.0);
//		pFunc->setHd(r1);
		for (int i=0; i < sz; ++i) {
			pFunc->getPoint(i,l,u);
			addEntry(l,r1,col);
			addEntry(u,r2,col);
			addEntry(1.0,r3,col++);
		}
		addEntry(-1.0,r1,pFunc->getX()->getHandle());
		addEntry(-1.0,r2,pFunc->getY()->getHandle());
	}
// discrete variables
	if (m_pLastDvar) {
		CSet<double>* s;
		for (pDvar=m_pLastDvar; pDvar; pDvar=pDvar->getPrev()) {
			s=pDvar->getSet();
			sz=s->getSize();
			col=n;
			for (int i=0; i < sz; ++i) {
				CLP::addVar(n++,0,0.0,0.0,1.0);
			}
			CLP::addCtr(r1=m++,0,0,0);
			CLP::addCtr(r2=m++,CTR_GUB,1.0,1.0);
//			pDvar->setHd(r1);
			for (s->initIt(); s->getNext(w);) {
				addEntry(w,r1,col);
				addEntry(1.0,r2,col++);
			}
			addEntry(-1.0,r1,pDvar->getVar()->getHandle());
		}
	}
	memset(m_dpC,0,n*sizeof(double));
	if (m_pObj) {
		for (pTerm=m_pObj->getLastTerm(); pTerm; pTerm=pTerm->getPrev()) {
			m_dpC[pTerm->getVar()->getHandle()]=(m_bSense)? pTerm->getCoeff(): -pTerm->getCoeff();
		}
	}
	closeMatrix();
} // end of CProblem::load

void sortByHandles(int n, double* dpX, int* ipHd, double* dpMem=0)
{
	bool flag=false;
	if (!dpMem) {
		dpMem=new double[n];
		if (!dpMem) {
			throw new CMemoryException("CProblem::sortByHandles");
		}
		flag=true;
	}
	memcpy(dpMem,dpX,n*sizeof(double));
	for (int i=0; i < n; ++i) {
		dpX[ipHd[i]]=dpMem[i];
	}
	if (flag)
		delete[] dpMem;
} // sortByHandles

void CProblem::setSolution(int n, double *dpVal, int *ipHd, bool bLocal)
{
//	int *ipHdToCol=(m_iN)? reinterpret_cast<int*>(m_dpArray): reinterpret_cast<int*>(m_ipVarType);
	if (!ipHd)
		n=(bLocal)? CLP::getSolution(dpVal,ipHd): CMIP::getSolution(dpVal,ipHd);
	m_dpVarVal=dpVal;
//	for (int i=0; i < n; i++) {
//		ipHdToCol[ipHd[i]]=i;
//	}
	sortByHandles(n,dpVal,ipHd,m_dpArray);
//	for (CVar* pVar=m_pLastVar; pVar; pVar=pVar->getPrev()) {
//		pVar->setVal(dpVal[ipHdToCol[pVar->getHandle()]]);
//	}
}

void CProblem::solve(const char* solFile)
{
	m_dpVarVal=m_dpPrice=m_dpRedCost=0;
	if (isPureLP()) { // LP problem
		CLP::optimize();
		if (CLP::isSolution()) {
			setSolution(0,0,0,true);
			if (!CLP::m_bPreproc) {
				int k, *ipHd=0;
				k=CLP::getReducedCosts(m_dpRedCost,ipHd); // m_dpRedCost=m_dpUd
				sortByHandles(k,m_dpRedCost,ipHd,m_dpArray);
				ipHd=0;
				k=CLP::getShadowPrices(m_dpPrice,ipHd); // m_dpPrice=m_dpUb
				sortByHandles(k,m_dpPrice,ipHd,m_dpArray);
			}
		}
		else if (isLpInfeasible()) {
		}
	}
	else {
		CMIP::optimize(10000000l,0.0,solFile);
		if (CMIP::isSolution()) {
			setSolution(0,0,0,false);
		}
	}
} // end CProblem::solve()

double CProblem::getValue(CVar& var)
{
	return (m_dpVarVal)? m_dpVarVal[var.getHandle()]: CLP::INF;;
}

double CProblem::getReducedCost(CVar& var)
{
	return (m_dpRedCost)? m_dpRedCost[var.getHandle()]: CLP::INF;
}

double CProblem::getShadowPrice(CCtr& ctr)
{
	return (m_dpPrice)? m_dpPrice[ctr.getHandle()]: CLP::INF;
}

void CProblem::printSolution(const char* fileName)
{
	char *str=m_sWarningMsg;
	if (fileName)
		strcpy(str,fileName);
	else {
		strcpy(str,m_strProblemName);
		strcat(str,".sol");
	}
	ofstream fout(str);
	if (fout.fail()) {
		throw new CFileException("CProblem::printSolution",str);
	}
	setSolution(0,0,0,false);
	fout << "Objective Value = " << getObjective() << endl;
	fout << "Variables\n                    Name            Value\n";
	for (CVar* pVar=m_pLastVar; pVar; pVar=pVar->getPrev()) {
		sprintf(str,"%24s  %16.6lf\n",pVar->getName(),getval(*pVar));
		fout << str;
	}
	fout.close();
} // CProblem::printSolution()

double CProblem::getObjective()
{
	return (isPureLP())? CLP::getObjVal(): CMIP::getObjVal();
}

char* CProblem::getCtrName(tagHANDLE rowHd, char *name)
{
	if (rowHd < m_iCtrNum)
		strcpy(name,((CCtr**)m_dpArray)[rowHd]->getName());
	else
		CLP::getCtrName(rowHd,name);
	return name;
}

char* CProblem::getVarName(tagHANDLE colHd, char *name)
{
	if (colHd < m_iVarNum)
		strcpy(name,((CVar**)m_ipArray)[colHd]->getName());
	else
		CLP::getVarName(colHd,name);
	return name;
}

bool CProblem::isSolution()
{
	return (isPureLP())?
		CLP::isSolution(): CMIP::isSolution();
}

void CProblem::printFinalSolution(const char* fileName)
{
	char *str=m_sWarningMsg;
	if (!fileName) {
		fileName=str;
		strcpy(str,m_strProblemName);
		strcat(str,".sol");
	}
	if (isSolution()) {
		ofstream fout(fileName);
		if (fout.fail()) {
			throw new CFileException("CProblem::printFinalSolution",fileName);
		}
		fout << "Objective Value = " << getObjective() << endl;
		fout << "Variables\n";
		fout << "                    Name             Value";
		if (m_dpRedCost)
			fout << "      Reduced Cost";
		fout << endl;
		for (CVar* pVar=m_pLastVar; pVar; pVar=pVar->getPrev()) {
			sprintf(str,"%24s  %16.6lf",pVar->getName(),getval(*pVar));
			if (m_dpRedCost)
				sprintf(str+42,"  %16.6lf",getredcost(*pVar));
			fout << str << endl;
		}
		if (m_dpPrice) {
			fout << "Constraints\n";
			fout << "                    Name        Shadow price\n";
			for (CCtr* pCtr=m_pLastCtr; pCtr; pCtr=pCtr->getPrev()) {
				sprintf(str,"%24s    %16.6lf\n",pCtr->getName(),getprice(*pCtr));
				fout << str;
			}
		}
		fout.close();
	}
	else {
		bool bInf=isLpInfeasible(),
			bUnb=isLpUnbounded();
		if (bInf || bUnb) {
			CVar** pVars=(CVar**)m_ipArray;
			for (CVar* pVar=m_pLastVar; pVar; pVar=pVar->getPrev()) {
				pVars[pVar->getHandle()]=pVar;
			}
			if (bInf) {
				CCtr** pCtrs=(CCtr**)m_dpArray;
				for (CCtr* pCtr=m_pLastCtr; pCtr; pCtr=pCtr->getPrev()) {
					pCtrs[pCtr->getHandle()]=pCtr;
				}
				showWhyLpInfeasible(fileName);
			}
			else
				showWhyLpUnbounded(fileName);
		}
		else {
			ofstream fout(fileName);
			if (fout.fail()) {
				throw new CFileException("CProblem::printFinalSolution",fileName);
			}
			fout << "The problem has no solution!\n";
			fout.close();
		}
	}
} // CProblem::printFinalSolution

////////////////////////////////////////////////////////////

void CProblem::maximize(CLinSum& lsum)
{
	setObj(new CLinSum(lsum),true);
//	if (lsum.IsTemporal())
		lsum.setLastTerm(0);
}

void CProblem::maximize(CTerm& term)
{
	setObj(new CLinSum(term),true);
}

void CProblem::maximize(CVar& var)
{
	setObj(new CLinSum(var),true);
}

void CProblem::minimize(CLinSum& lsum)
{
	setObj(new CLinSum(lsum),false);
//	if (lsum.IsTemporal())
		lsum.setLastTerm(0);
}

void CProblem::minimize(CTerm& term)
{
	setObj(new CLinSum(term),false);
}

void CProblem::minimize(CVar& var)
{
	setObj(new CLinSum(var),false);
}

CVar& CProblem::addVar(CVar& var)
{
	var.setPrev(m_pLastVar);
	m_pLastVar=&var;
	++m_iVarNum;
	return var;
}

CCtr& CProblem::addCtr(CCtr& ctr)
{
	int state=getCutState();
	if (state) {
		ctr.setPrev(m_pLastCut);
		m_pLastCut=&ctr;
		if (state > 1)
			ctr.setLocal();
	}
	else {
		ctr.setPrev(m_pLastCtr);
		m_pLastCtr=&ctr;
	}
	return ctr;
} // end of CProblem::addCtr

CFunction& CProblem::addCtr(CFunction& func)
{
	func.setPrev(m_pLastFunc);
	m_pLastFunc=&func;
	return func;
}

CDvar& CProblem::addDvar(CDvar& dvar)
{
	dvar.setPrev(m_pLastDvar);
	m_pLastDvar=&dvar;
	return dvar;
}

void CProblem::optimize(const char* solFile)
{
	load();
	solve(solFile);
}

void CProblem::settype(CVar& var, enMipshellVarType type)
	{var.setType(type);}

void CProblem::setobj(CVar& var, double cost)
{
	setObj(var.getHandle(),cost);
}

void CProblem::setub(CVar& var,double ub)
{
	setUb(var.getHandle(),ub);
}

void CProblem::setlb(CVar& var,double lb)
{
	setLb(var.getHandle(),lb);
}

void CProblem::setpriority(CVar& var, int pr)
{
	var.setPriority(pr);
}

CLinSum& CProblem::getSum(int i)
	{return m_pSum[i];}

//////////////////////////////////////////////////////////////
// S E P A R A T I O N
///////////////////////
void CProblem::loadCuts()
{
	double l,u,w;
	double *dpVal;
	CVar* pVar;
	int	*ipHdToCol, *ipCol, *ipInd;
	int col,i,sz,n;

	n=m_iVarNum;
	dpVal=m_dpFd;
	ipCol=reinterpret_cast<int*>(dpVal+n);
	ipInd=ipCol+n;
	ipHdToCol=reinterpret_cast<int*>(m_dpNorm);

	for (i=0; i < n; i++) {
		ipInd[i]=NIL;
		ipHdToCol[m_ipColHd[i]]=i;
	}

	for (CCtr *pCtr=m_pLastCut; pCtr; pCtr=pCtr->getPrev()) {
		l=pCtr->getLHS();
		u=pCtr->getRHS();
        sz=0;
		for (CTerm* pTerm=pCtr->getLastTerm(); pTerm; pTerm=pTerm->getPrev()) {
			w=pTerm->getCoeff();
			if ((pVar=pTerm->getVar())) {
				if (ipInd[col=ipHdToCol[pVar->getHandle()]] == NIL) {
					ipCol[ipInd[col]=sz]=col;
					dpVal[sz++]=w;
				}
				else
					dpVal[ipInd[col]]+=w;
			}
			else {
				if (l > -INF)
					l-=w;
				if (u < INF)
					u-=w;
			}
		} // for (pTerm=pCtr->getLastTerm()
		if (isPureLP())
			addNewRow(NIL,0,l,u,sz,dpVal,ipCol,false,NOT_SCALED,n);
		else
			addCut(-2,pCtr->isGlobal()? 0: CTR_LOCAL,l,u,sz,dpVal,ipCol,false,NOT_SCALED,n);
		ipHdToCol=reinterpret_cast<int*>(m_dpNorm); // because of possible memory reallocation
		for (i=0; i < sz; i++) {
			ipInd[ipCol[i]]=NIL;
		}
	}
} // end of CProblem::loadCuts

void CProblem::deleteCuts()
{
	for (CCtr *pCtr1, *pCtr=m_pLastCut; pCtr; pCtr=pCtr1) {
		pCtr1=pCtr->getPrev();
		delete pCtr;
	}
	m_pLastCut=0;
}

bool CProblem::separate(int n, const double* X, const tagHANDLE* colHd, bool genFlag)
{
	bool flag;
	setSolution(n,m_dpVarVal=const_cast<double*>(X),const_cast<int*>(colHd),true);
	m_iCutState=1;
	if ((flag=separate())) {
		if (genFlag) {
			loadCuts();
			deleteCuts();
		}
	}
	m_dpVarVal=0; // values of variables are not available if `m_dpVarVal=0`
	m_iCutState=0;
	return flag;
}

bool CProblem::genCut1(int n, const double* X, const tagHANDLE* colHd)
{
	bool flag;
	setSolution(n,m_dpVarVal=const_cast<double*>(X),const_cast<int*>(colHd),true);
	m_iCutState=1;
	if ((flag=gencut())) {
		loadCuts();
		deleteCuts();
	}
	m_dpVarVal=0; // values of variables are not available if `m_dpVarVal=0`
	m_iCutState=0;
	return flag;
}

////////////////////////////////
// modeling
////////////
CFunction& CProblem::curve(CVar& x, CVar& y, const char* points)
{
	int n=CFunction::getPointNum(points);
	if (!n) {
		throw new CDataException("CProblem:function");
	}
	CFunction* pFunc=new CFunction(n);
	if (!pFunc) {
		throw new CMemoryException("CProblem::function");
	}
	pFunc->m_pX=&x;
	if (&x != &y) {
		pFunc->m_pY=&y;
		addCtr(*pFunc);
	}
	else
		pFunc->m_pY=0;
//	char *ch, *str;
	char *ch, *str;
	bool flag=true;
	double *dpVal=pFunc->m_dpPoint;
	int sz=0;

	str=m_sWarningMsg;
	strcpy(str,points);
	for (str=strchr(str,'('); flag && str;) {
//	for (str=const_cast<char*>(strchr(points,'(')); flag && str;) {
		if ((ch=strchr(++str,','))) {
			if (sz >= n) {
				flag=false;
				break;
			}
			*ch=0;
			*dpVal=atof(str);
			++dpVal;
			*ch=',';
			if ((ch=strchr(str=ch+1,')'))) {
				*ch=0;
				*dpVal=atof(str);
				++dpVal;
				++sz;
				*ch=')';
				if ((str=strchr(ch+1,',')))
					if (!(str=strchr(ch+2,'(')))
						flag=false;
			}
			else flag=false;
		}
		else flag=false;
	}
	if (!flag) {
		throw new CDataException("CProblem::function");
	}
	pFunc->m_iSize=sz;
	return *pFunc;
}

CFunction& CProblem::curve(CVar& x, CVar& y, std::string& points)
{
	return curve(x,y,points.c_str());
}

CFunction& CProblem::function(CVar& x, const char* points)
{
	return curve(x,x,points);
}

CFunction& CProblem::function(CVar& x, std::string& points)
{
	return function(x,points.c_str());
}

////////////////////////
// Debugging
///////////////////////
void CProblem::printMatrix(const char *fileName, bool bScaled)
{
	CVar** pVars=(CVar**)m_dpArray;
	for (CVar* pVar=m_pLastVar; pVar; pVar=pVar->getPrev()) {
		pVars[pVar->getHandle()]=pVar;
	}
	CCtr** pCtrs=(CCtr**)m_dpW;
	for (CCtr* pCtr=m_pLastCtr; pCtr; pCtr=pCtr->getPrev()) {
		pCtrs[pCtr->getHandle()]=pCtr;
	}
	CMIP::printMatrix(fileName,bScaled);
}
