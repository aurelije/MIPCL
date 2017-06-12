///////////////////////////////////////////////////////////////
/**
 * \file Problem.h interface for `CProblem` class
 * |  __Author__  | N.N. Pisaruk                              |
 * |-------------:|:------------------------------------------|
 * |  __e-mail__  | nicolaipisaruk@gmail.com                  |
 * | __home page__| http://pisaruk.narod.ru                   |
 *
 *   \copyright __2015 Nicolai N. Pisaruk__
 */

/*
 *  This file is part of the mixed integer class library (MIPCL).
 *
 *  MIPCL is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  MIPCL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with MIPCL; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __PROBLEM__H
#define __PROBLEM__H

#ifdef _WINDOWS
#ifndef MIPSHELL_API
#ifdef MIPSHELL_EXPORTS
#define MIPSHELL_API __declspec(dllexport)
#else
#define MIPSHELL_API __declspec(dllimport)
#endif
#endif
#else
#ifndef MIPSHELL_API
#define MIPSHELL_API
#endif
#endif

#include <cmip.h>
#include "Set.h"
#include "Var.h"

//class CVar;
class CCtr;
class CFunction;
class CDvar;
class CTerm;
class CLinSum;

/// `CProblem` represents MIP instances.
class MIPSHELL_API CProblem: public CMIP 
{
	int m_iVarNum; ///< number of variables
	int m_iCtrNum; ///< number of constraints
	CVar* m_pLastVar; ///< starts linked list of variables
	CCtr* m_pLastCtr; ///< starts linked list of constraints
	CFunction* m_pLastFunc; ///< starts linked list of functions
	CDvar* m_pLastDvar; ///< starts linked list of discrete variables
	CLinSum* m_pObj; ///< objective
	double *m_dpVarVal; ///< only pointer used to extract variable value
	double *m_dpRedCost, *m_dpPrice; ///< only pointers used to extract reduced costs and shadow prices
	int m_iCutState; ///< 0 - constraint, 1 - global cut, 2 - local cut
	CCtr* m_pLastCut; ///< starts a chain of cuts
public:
	CLinSum *m_pSum; ///< 10 pointers are used to allocate memory for 10 CLinSum objects
private:
	void init(); ///< initialized members of this class
public:
	CProblem(); ///< The default constructor.

	/**
		The most commonly used constructor.
		\param[in] name name of the problem being solved.
	 */
	CProblem(const char* name);
#ifndef __ONE_THREAD_
	/**
	 * The clone constructor and the clone function are used in multithreaded applications.
	 * Their implementation is standard in all __MIPSHELL__ programs.
	 * \param[in] other `CProblem` object to be cloned;
	 * \param[in] thread order of newly created object in the list of `CMIP` objects of multithreaded application.
	 */
	CProblem(const CProblem &other, int thread);

	/**
	 * The clone function simply calls the clone constructor to create a new object of a class derived from `CMIP`
	 * (in our case `CProblem` object) are used.
	 * \param[in] pMip pointer to object to be cloned;
	 * \param[in] thread order of newly created object in the list of `CMIP` objects of multithreaded application.
	 */
	virtual CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~CProblem(); ///< The destructor.

//	/// \cond NEWandDELoperators
//	/// The new operator.
//	/**
//	 * Initially, the new and delete operators were overloaded to overcome a WINDOWS bug.
//	 * When __MIPschell__ and __MIPCL__ libraries allocate memory in different heaps,
//	 * deleting in __MIPCL__ objects created by __MIPschell__ cased an ERROR.
//	 * \param[in] iSize size (in bytes) of required memory block.
//	 * \return pointer to allocated memory block.
//	 * \throws CMemoryException.
//	 * \sa `operator delete()`
//	 */
//	void* operator new(size_t iSize) throw(CMemoryException);
//
//	/// The delete operator.
//	/**
//	 * \param[in] pPtr pointer to memory block to bee freed.
//	 * \sa `operator new()`
//	 */
//	void operator delete(void* pPtr) throw();
//	/// \endcond

// operations
	/**
	 * The procedure loads the problem, i.e. builds the matrix, does preprocessing, performs scaling, and etc.
	 * \throws CMemoryException.
	 */
	void load();

	/**
	 * The procedure solves the problem (the problem must be previously loaded).
	 * \param[in] solFile if not `0`, each new best solution is  written to file named `solFile`.
	 * \throws CMemoryException and CDegenException.
	 */
	void solve(const char* solFile=0);

	/**
	 * The procedure loads and solves the problem (`load()` + `solve()`).
	 * \param[in] solFile if not `0`, each new best solution is  written to file named `solFile`.
	 */
	void optimize(const char* solFile=0);

	/**
	 * The procedure prints the solution to the file.
	 * \param[in] fileName  file name; if `fileName=0`, then the
	 *   output file name is the problem name appended with the extension ".sol".
	 * \throws CFileException.
	 */
	void printFinalSolution(const char* fileName=0);
#define printsol printFinalSolution ///< alias for `CProblem::printFinalSolution()`

	double getObjective(); ///< \return objective value of optimal solution.
#define getobj getObjective ///< alias for `CProblem::getObjective()`

	/**
	 * \param[in] var variable which value is inquired.
	 * \return value of variable `var`.
	 */
	double getValue(CVar& var);
#define getval getValue ///< alias for `CProblem::getValue()`

	/**
	 * Use this function only for LPs.
	 * \param[in] var variable which reduced cost is inquired.
	 * \return reduced cost of variable `var`.
	 */
	double getReducedCost(CVar& var);
#define getredcost getReducedCost ///< alias for `CProblem::getReducedCost()`

	/**
	 * Use this function only for LPs.
	 * \param[in] ctr constraint which shadow price is inquired.
	 * \return shadow price of constraint `ctr`.
	 */
	double getShadowPrice(CCtr& ctr);
#define getprice getShadowPrice ///< alias for `CProblem::getShadowPrice()`
	
#define preprocoff preprocOff ///< alias for `CLP::preprocOff()`
#define setcutpattern setAutoCutPattern ///< alias for `CMIP::setAutoCutPattern()`
	
protected:
    /**
     *  `CProblem` privately overloads separating procedure, `CMIP::separate()`, of the base class `CMIP`.
     *  This overloaded function - `CProblem::separate()` with four arguments -
     *   first calls `CProblem::separate()` (without arguments) to generate cuts,
     *   and then adds generated cuts (if any one was produced) to the matrix, which is __MIPCL__ representation of the problem being solved.
     *   \return `true` if at least one cut was generated.
     * \sa  `CLP::separate()`, `CMIP::separate()`.
     */
	virtual bool separate()
		{return false;}

    /**
     *  `CProblem` privately overloads separating procedure, `CMIP::genCut1()`, of the base class `CMIP`.
     *  This overloaded function, `CProblem::genCut1()`, first calls `CProblem::gencut()`,
     *   and then adds generated cuts (if any one was produced) to the matrix.
     *   \return `true` if at least one cut was generated.
     * \sa `CMIP::genCut1()`.
     */
	virtual bool gencut()
		{return false;}

	/**
	 * Any inequality added to the problem is treated as
	 *  - ordinary constraint if `m_iCutState=0`;
	 *  - global cut if  `m_iCutState=1`;
	 *  - local cut if `m_iCutState=2`.
	 *  \return value of `m_iCutState`.
	 */
	const int getCutState()
		{return m_iCutState;}

	/**
	 * The function sets `m_iCutState=2` what means that, until `m_iCutState` is reset,
	 * all inequalities added to the problem will be considered as _local_ cuts.
	 * \sa `getCutState()`.
	 */
	void setCutsLocal()
		{m_iCutState=2;}

	/**
	 * The function sets `m_iCutState=2` what means that, until `m_iCutState` is reset,
	 * all inequalities added to the problem will be considered as _global_ cuts.
	 * \sa `getCutState()`.
	 */
	void setCutsGlobal()
		{m_iCutState=1;}

	/**
	 * This function overloads the function of the base class `CMIP` to store solutions
	 * in a more appropriate way.
	 * \param[in] fileName name of the file to store solutions; if `fileName=`0`,
	 * the solver makes up the file name by appending the extension ".sol" to the name of the problem being solved.
	 */
	virtual void printSolution(const char* fileName=0);

	/**
	 * The procedure rewrites solution to an internal `MIPCL` array so that
	 * the value of any `MIPshell` variable `x` can be accessed via calls `x.getVal()` or `getval(x)`.
	 * In multithreaded applications,
	 * we cannot store the value of a variables as member `CVar` class
	 * because the object representing variables are shared between threads.
	 * \param[in] n number of variables;
	 * \param[in] dpVal,ipHd arrays of size `n` that store solution components:
	 * `dpVal[in]` is value of variable indexed by `ipHd[i]`;
	 * if all three parameters, `n`, `dpVal` and `ipCol` are zeroes,
	 * `MIPshell` assigns their values and then writes solution components to these arrays;
	 * \param[in] bLocal if `true`, solution of currently processing node LP is needed;
	 * otherwise, global (record) solution is required.
	 */
	void setSolution(int n, double *dpVal, int *ipHd, bool bLocal=true);

private:
	bool separate(int n, const double* X, const tagHANDLE* colHd, bool genFlag);
	bool genCut1(int n, const double* X, const tagHANDLE* colHd);

	void loadCuts(); ///< sends cuts to the solver
	void deleteCuts(); ///< delete all cuts

	char* getCtrName(tagHANDLE rowHd, char *name);
	char* getVarName(tagHANDLE colHd, char *name);

	void setObj(int col, double cost);
	void setLb(int col, double lb);
	void setUb(int col, double ub);
 // Don't use the above three function for preprocessed problems

public:
	/**
	 * Sets given linear expression as the objective for the problem being constructed.
	 * \param[in] lsum linear expression;
	 * \param[in] bSense  if `true`, optimization goal is to maximize objective;
	 *  otherwise, objective is to be minimized.
	 */
	void setObj(CLinSum *lsum, bool bSense);

	bool isSolution(); ///< \return `true` if a solution (not necessary optimal) has been found already.
#define is_soluion isSolution ///< alias for `CProblem::isSolution()`

#define getprobname	getProblemName ///< alias for `CLP::getProblemName()`

	/**
	 * The procedure returns the `i`-th linear expression.
	 * \param[in] i index of linear expression.
	 * \return reference to linear expression indexed by `i`.
	 */
	CLinSum& getSum(int i);

	/**
	 * The function declares that the objective is to maximize a given linear expression.
	 * \param[in] lsum objective linear expression.
	 */
	void maximize(CLinSum& lsum);

	/**
	 * The function declares that the objective is to maximize a given linear term.
	 * \param[in] term objective linear term.
	 */
	void maximize(CTerm& term);

	/**
	 * The function declares that the objective is to maximize the value of given variable.
	 * \param[in] var objective variable.
	 */
	void maximize(CVar& var);

	/**
	 * The function declares that the objective is to minimize a given linear expression.
	 * \param[in] lsum objective linear expression.
	 */
	void minimize(CLinSum& lsum);

	/**
	 * The function declares that the objective is to minimize a given linear term.
	 * \param[in] term objective linear term.
	 */
	void minimize(CTerm& term);

	/**
	 * The function declares that the objective is to minimize the value of given variable.
	 * \param[in] var objective variable.
	 */
	void minimize(CVar& var);
	
	/**
	 * The procedure adds a given variable to the linked list of problem variables.
	 * \param[in] var variable to be added.
	 * \return reference to `var`.
	 * \sa \ref mipshell_variables.
	 */
	CVar& addVar(CVar& var);

	/**
	 * The procedure adds a given linear constraint to the linked list of problem constraints.
	 * \param[in] ctr linear constraint to be added.
	 * \return reference to `ctr`.
	 *  \sa \ref mipshell_constraints.
	 */
	CCtr& addCtr(CCtr& ctr);

	/**
	 *  This function is needed only to simplify preprocessor.
	 *  \param[in] var reference to `CVar` object.
	 *  \return reference to `var`.
	 *  \sa \ref mipshell_constraints.
	 */
	CVar& addCtr(CVar& var)
		{return var;}

	/**
	 * The procedure adds a given function constraint to the linked list of problem functions.
	 * \param[in] func function constraint to be added.
	 * \return reference to `func`.
	 */
	CFunction& addCtr(CFunction& func);

	/**
	 * The procedure adds a given discrete variable constraint to the linked list of problem discrete variables.
	 * \param[in] dvar discrete variable constraint to be added.
	 * \return reference to `dvar`.
	 * \sa \ref mipshell_discreteVariables.
	 */
	CDvar& addDvar(CDvar& dvar);

	/**
	 * The procedure sets the type of a variable.
	 * \param[in] var variable which type to be set;
	 * \param[in] type type of variable.
	 * \sa `enMipshellVarType`, \ref mipshell_variables.
	 */
	void settype(CVar& var, enMipshellVarType type);

	/**
	 * To improve branching, the user can assign higher priorities to "most important" integer variables.
	 * \param[in] var variable which priority be changed;
	 * \param[in] pri new priority.
	 */
	void setpriority(CVar& var, int pri);

	/**
	 * The procedure sets the value of objective coefficient for a given variable.
	 * \param[in] var variable which objective coefficient be changed;
	 * \param[in] cost value of objective coefficient.
	 */
	void setobj(CVar& var,double cost);

	/**
	 * The procedure sets upper bound of a given variable.
	 * \param[in] var variable which upper bound be changed;
	 * \param[in] ub new upper bound.
	 */
	void setub(CVar& var,double ub);

	/**
	 * The procedure sets lower bound of a given variable.
	 * \param[in] var variable which lower bound be changed;
	 * \param[in] lb new lower bound.
	 */
	void setlb(CVar& var,double lb);

#define setcutpattern setAutoCutPattern ///< alias for `CMIP::setAutoCutPattern()`.
	
//////////////////////////////////////
// modeling
//
	/**
	 * When a variable `y` is a function of another real variable `x`,
	 * we can approximate that function by a piecewise-linear function
	 * given by an ordered list of its breakpoints.
	 * \param[in] x independent  variable;
	 * \param[in] points string of breakpoints, where x-values go in increasing order.
	 * \return reference to a `CFunction` object.
	 * \sa \ref mipshell_piecewiseFunctions.
	 */
	CFunction& function(CVar& x, const char* points);

	/**
	 * Similar to `CFunction& function(CVar& x, const char* points)`.
	 * \param[in] x independent  variable;
	 * \param[in] points string of breakpoints, where x-values go in increasing order.
	 * \return reference to a `CFunction` object.
	 * \sa \ref mipshell_piecewiseFunctions.
	 */
	CFunction& function(CVar& x, std::string& points);

	/**
	 * Similar to `CFunction& function(CVar& x, const char* points)`,
	 * but in the list of breakpoints,  x-values may go in arbitrary order.
	 * \param[in] x,y dependent  variables;
	 * \param[in] points string of breakpoints.
	 * \return reference to a `CFunction` object.
	 * \sa \ref mipshell_piecewiseFunctions.
	 */
	CFunction& curve(CVar& x, CVar& y, const char* points);

	/**
	 * Similar to `CFunction& curve(CVar& x, CVar& y, const char* points).
	 * \param[in] x,y dependent variables;
	 * \param[in] points string of breakpoints.
	 * \return reference to a `CFunction` object.
	 * \sa \ref mipshell_piecewiseFunctions.
	 */
	CFunction& curve(CVar& x, CVar& y, std::string& points);
	
////////////////////////
// Debugging
///////////////////////
	/**
	 * The procedure prints the problem matrix to the text file.
	 * The problem must be previously loaded by calling `load()`.
	 * \param[in] fileName file name;
	 * \param[in] bScaled __MIPCL_ scales variables and inequalities to make computations more numerically stable;
	 *  if `bScaled=false`, the matrix is printed in the original (not scaled) variables.
	 * \throws CFileException.
	 */
	void printMatrix(const char *fileName, bool bScaled=false);
#define printmatrix printMatrix ///< `printmatrix` is an  alias for `printMatrix()`.
};

#endif // #ifndef __PROBLEM__H
