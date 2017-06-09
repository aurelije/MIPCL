///////////////////////////////////////////////////////////////
/**
 * \file Function.h Interface for `CFunction` class
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

#ifndef __FUNCTION__H
#define __FUNCTION__H

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
/// WINDOWS specifics.
/**
 * The following ifdef block is the standard way of creating macros which make exporting
 * from a DLL simpler. All files within this DLL are compiled with the MIP_EXPORTS
 * symbol defined on the command line. This symbol should not be defined on any project
 * that uses this DLL. This way any other project whose source files include this file see
 * MIP_API functions as being imported from a DLL, whereas this DLL sees symbols
 * defined with this macro as being exported.
 */
#define MIPSHELL_API
#endif
#endif

#define FUNC_NAME_LENGTH 31 ///< maximum number of characters in function names.

#include <map>
#include <string>
#include <except.h>

/**
 * Class `CFunction` is used to represent the relations `y=f(x)`,
 *  where `f` is piecewise linear functions. In what follows,
 *  `x` and `y` are called, respectively, _independent_ and _dependent_ variables.
 * \sa \ref mipshell_piecewiseFunctions.
 */
class MIPSHELL_API CFunction
{
	friend class CProblem;
//
	int m_iMaxSize; ///< maximum number of breakpoints.
	int m_iSize; ///< number of breakpoints.
	char m_strName[FUNC_NAME_LENGTH+1]; ///< constraint name.
	CVar *m_pX; ///< pointer to independent variable.
	CVar *m_pY; ///< pointer to dependent variable.
	double *m_dpPoint; ///< list of breakpoints; `(m_dpPoint[i<<1],m_dpPoint[(i<<1)+1])` is breakpoint `i`.
	CFunction* m_pPrev; ///< pointer to preceding object in the linked list of problem function constraints.
public:
	/**
	 * The constructor creates a `CFunction` that can represent
	 * piecewise functions with up to `n` breakpoints.
	 * \param[in] n  maximum number of break-points.
	 */
	CFunction(const int n);
	virtual ~CFunction(); ///< The destructor.
// operators
	/// \cond NEWandDELoperators
	void* operator new(size_t iSize) throw(CMemoryException);
	void operator delete(void* pPtr) throw();
	void* operator new[](size_t iSize) throw(CMemoryException);
	void operator delete[](void* pPtr) throw();
	/// \endcond

	/**
	 * The operator reassigns the dependent variable.
	 * \param[in] var dependent variable.
	 *
	 */
	CFunction& operator==(CVar& var);

	/**
	 * The operator reassigns the dependent variable for a given `CFunction` object.
	 * \param[in] var dependent variable;
	 * \param[in] func `CFunction` object which dependent variable be reassigned.
	 *
	 */
	friend MIPSHELL_API CFunction& operator==(CVar& var, CFunction& func);

// operations
	bool isOK() {return (m_iSize > 0)? true: false;} ///< \return `true` if `*this` object has been initialized already.

	const char* getName() const
		{return m_strName;} ///< \return function constraint name.

	/**
	 * The procedure assigns the name for `*this` function object.
	 * \param name name of `*this` function.
	 */
	void setName(const char* name);

	int getSize()
		{return m_iSize;} ///< \return number of breakpoints.

	/**
	 * The procedure is called assign the pointer to preceding object in the linked list of problem function constraints.
	 * \param[in] fun prededing `CFunction` object.
	 */
	void setPrev(CFunction* fun)
		{m_pPrev=fun;}

	CFunction* getPrev()
		{return m_pPrev;} ///< \return pointer to preceding object in the linked list of problem function constraints.

	/**
	 * The procedure reassigns the independent variable for `*this` function object.
	 * \param[in] pVar new independent variable.
	 */
	void setX(CVar* pVar)
		{m_pX=pVar;}

	CVar* getX()
		{return m_pX;} ///< \return pointer to independent variable.

	/**
	 * The procedure reassigns the dependent variable for `*this` function object.
	 * \param[in] pVar new dependent variable.
	 */
	void setY(CVar* pVar)
		{m_pY=pVar;}

	CVar* getY()
		{return m_pY;}  ///< \return pointer to dependent variable.

	/**
	 * The procedure returns a particular breakpoint.
	 * \param[in] ind index of breakpoint;
	 * \param[out] x `x`-coordinate of breakpoint;
	 * \param[out] y `y`-coordinate of breakpoint.
	 */
	void getPoint(int ind, double& x, double& y);
	
	/**
	 * The procedure computes the number of breakpoints in a given string of breakpoints.
	 * \param[in] points string of breakpoints.
	 * \return number of breakpoints in the input string.
	 */
	static int getPointNum(const char* points);

};

#endif // #ifndef __FUNCTION__H 
