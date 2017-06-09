///////////////////////////////////////////////////////////////
/**
 * \file DVar.h interfaces for `CDvar` class
 *
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
#ifndef __DVAR__H
#define __DVAR__H

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

#define DVAR_NAME_LENGTH 31 ///< maximum name length of `CDvar` constraints.

#include <except.h>
#include "Set.h"

class CVar;

/**
 * A _discrete variable_ is a real variable restricted to take values from a given set of reals.
 * In __MIPshell_ such variables are represented as objects of class `CDvar`.
 * \sa \ref mipshell_discreteVariables.
 */
class MIPSHELL_API CDvar  
{
	char m_strName[DVAR_NAME_LENGTH+1]; ///< constraint namee.
	int m_iHd; ///< constraint handle.
	CVar *m_pVar; ///< pointer to discrete variable.
	CSet<double>* m_pSet; ///< set of values of discrete variable.
	CDvar* m_pPrev; ///< preceding `CDvar` object in the linked list of some problem discrete variables.
	bool m_bMemHere; ///< if `true`, `&this` object allocated memory for the set of values.

public:
	/**
	 * The constructor creates an empty discrete variable constraint, and assigns it a name.
	 * \param[in] name
	 */
	CDvar(const char* name=0);

	/**
	 * The constructor creates a new `CDvar` object to represent a discrete variable given by the parameters.
	 * \param[in] var variable that is declared by `*this` constraint to be discrete;
	 * \param[in] values string representation of the set of values of `var`.
	 */
	CDvar(CVar& var, const char* values);

	/**
	 * The constructor creates a new `CDvar` object to represent a discrete variable given by the parameters.
	 * \param[in] var variable that is declared by `*this` constraint to be discrete;
	 * \param[in] values set of values of `var`.
	 */
	CDvar(CVar& var, CSet<double>& values);
	virtual ~CDvar(); ///< the destructor.
// operations
	///////////////////////////////////
	/// \cond NEWandDELoperators
	void* operator new(size_t iSize) throw(CMemoryException);
	void operator delete(void* pPtr) throw();
	void* operator new[](size_t iSize) throw(CMemoryException);
	void operator delete[](void* pPtr) throw();
	/// \endcond

	/**
	 * The procedure creates a new `CDvar` object to represent a discrete variable given by the parameters.
	 * \param[in] var variable that is declared by `*this` constraint to be discrete;
	 * \param[in] values string representation of the set of values of `var`.
	 *  \return reference to the created `CDvar` object.
	 */
	CDvar& newDvar(CVar& var, const char* values);

	/**
	 * The procedure creates a new `CDvar` object to represent a discrete variable given by the parameters.
	 * \param[in] var variable that is declared by `*this` constraint to be discrete;
	 * \param[in] values set of values of `var`.
	 *  \return reference to the created `CDvar` object.
	 */
	CDvar& newDvar(CVar& var, CSet<double>& values);
	
	/**
	 * The function assigns a name to `*this` constraint.
	 * \param[in] name constraint name.
	 */
	void setName(const char* name);
	const char* getName()
		{return m_strName;} ///< \return name of `*this` constraint.

	int getSize()
		{return m_pSet->getSize();} ///< \return number of values of the discrete variable given by `*this` constraint.
	/**
	 * The procedure sets the preceding `CDvar` object in the linked list of some problem discrete variables.
	 * \param[in] dvar pointer to preceding `CDVar` object.
	 */
	void setPrev(CDvar* dvar)
		{m_pPrev=dvar;}
	CDvar* getPrev()
		{return m_pPrev;} ///< \return preceding `CDvar` object in the linked list of some problem discrete variables.

	CVar* getVar()
		{return m_pVar;} ///< \return discrete variable.

	/**
	 * The function assigns a variable to `*this` constraint.
	 * \param[in] pVar pointer to a `CVar` object representing a variable that is declared by `*this` constraint to be discrete.
	 */
	void setVar(CVar* pVar)
		{m_pVar=pVar;}

	CSet<double>* getSet()
		{return m_pSet;} ///< \return pointer to the set of values of `*this` discrete variable.

	/**
	 * The procedure sets the set of values for the discrete variable represented by `*this` object.
	 * \param[in] pSet pointer to a set of reals.
	 */
	void setSet(CSet<double>* pSet)
		{m_pSet=pSet;}

	/**
	 * When a number of discrete variables take the same values, it is reasonable to declare only one set of values,
	 * and use that set in many discrete variable constraints.
	 * \param[in] flag if `true` the set of values of `*this` discrete variables will be destroyed by the destructor.
	 */
	void setMemFlag(bool flag)
	{m_bMemHere=flag;}
};

#endif // #ifndef __DVAR__H
