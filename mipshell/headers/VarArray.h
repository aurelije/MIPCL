///////////////////////////////////////////////////////////////
/**
 * \file VarArray.h interface for `CVarArray class`
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

#ifndef __VARARRAY__H
#define __VARARRAY__H

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
 * symbol defined on the command line. this symbol should not be defined on any project
 * that uses this DLL. This way any other project whose source files include this file see
 * MIP_API functions as being imported from a DLL, whereas this DLL sees symbols
 * defined with this macro as being exported.
 */
#define MIPSHELL_API
#endif
#endif

#include <map>
#include <except.h>
#include "Var.h"
#include "Index.h"

class CBasicSet;

typedef CVar *CVarPtr; ///< `CVarPtr` is an alias for `CVar`.

/**
 * In __MIPshell__ there are two type of arrays of variables: _dense_, represented as object of class `CVarVector`,
 * and _sparse_, represented as objects of class `CVarArray`.
 * Both may be multidimensional. The maximum dimension of an array of variables is 4.
 * Indices of a `CVarVector` object are consecutive integers starting from `0`.
 * Indices of a `CVarArray` object are elements of a given sets, index sets for different dimensions may be different.
 * Besides, it is not necessary that each tuple of indices maps to a variable in the `CVarArray` object.
 * \sa \ref mipshell_arraysOfVariables.
 */
class MIPSHELL_API CVarArray
{
	char m_strName[VARARR_NAME_LENGTH+1]; ///< array name.
	int m_iDim; ///< dimension of array, `1 <= m_iDim <= MAX_VARARR_SIZE`.
	CVar* m_pVars; ///< array to store variables.
	std::map<CIndex,CVarPtr> *m_pMap; ///< maps indices onto variables .

public:
	CVarArray() {}; ///< The default constructor.

	/**
	 * The constructor creates a one-dimensional array of variables, and then adds it to a given problem.
	 * \param[in] prob problem to which newly created array must be added;
	 * \param[in] name array name;
	 * \param[in] type extended type of array variables;
	 * \param[in] s set of indices.
	 */
	CVarArray(CProblem* prob, const char* name, int type, CBasicSet& s);

	/**
	 * The constructor creates a 2-dimensional array of variables, and then adds it to a given problem.
	 * \param[in] prob problem to which newly created array must be added;
	 * \param[in] name array name;
	 * \param[in] type extended type of array variables;
	 * \param[in] s0,s1 two sets of indices.
	 */
	CVarArray(CProblem* prob, const char* name, int type, CBasicSet& s0, CBasicSet& s1);

	/**
	 * The constructor creates a 3-dimensional array of variables, and then adds it to a given problem.
	 * \param[in] prob problem to which newly created array must be added;
	 * \param[in] name array name;
	 * \param[in] type extended type of array variables;
	 * \param[in] s0,s1,s2 three sets of indices.
	 */
	CVarArray(CProblem* prob, const char* name, int type, CBasicSet& s0, CBasicSet& s1, CBasicSet& s2);

	/**
	 * The constructor creates a 4-dimensional array of variables, and then adds it to a given problem.
	 * \param[in] prob problem to which newly created array must be added;
	 * \param[in] name array name;
	 * \param[in] type extended type of array variables;
	 * \param[in] s0,s1,s2,s3 four sets of indices.
	 */
	CVarArray(CProblem* prob, const char* name, int type, CBasicSet& s0, CBasicSet& s1, CBasicSet& s2, CBasicSet& s3);

	virtual ~CVarArray(); ///< The destructor.
// operators
	/**
	 * The access operator returns the array element (variable) with a given index.
	 * \param[in] ind index of array variable.
	 * \return reference to the variable in the array indexed by `ind`.
	 * \throw "Bad index" exception when in the array there is no variable indexed by `ind`.
	 */
	CVar& operator[](const CIndex& ind);

	/**
	 * The access operator returns the array element (variable) with a given index.
	 * \param[in] ind index of array variable.
	 * \return reference to the variable in the array indexed by `ind`.
	 * \throw "Bad index" exception when in the array there is no variable indexed by `ind`.
	 */
	CVar& operator()(const CIndex& ind);

	/**
	 * The access operator returns the array element (variable) with given indices.
	 * \param[in] ind0,ind1 indices of array variable.
	 * \return reference to the variable in the array indexed by the compound index `(ind0,ind1)`.
	 * \throw "Bad index" exception when in the array there is no variable indexed by `(ind0,ind1)`.
	 */
	CVar& operator()(const CIndex& ind0, const CIndex& ind1);

	/**
	 * The access operator returns the array element (variable) with given indices.
	 * \param[in] ind0,ind1,ind2 indices of array variable.
	 * \return reference to the variable in the array indexed by the compound index `(ind0,ind1,ind2)`.
	 * \throw "Bad index" exception when in the array there is no variable indexed by `(ind0,ind1,ind2)`.
	 */
	CVar& operator()(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2);

	/**
	 * The access operator returns the array element (variable) with given indices.
	 * \param[in] ind0,ind1,ind2,ind3 indices of array variable.
	 * \return reference to the variable in the array indexed by the compound index `(ind0,ind1,ind2,ind3)`.
	 * \throw "Bad index" exception when in the array there is no variable indexed by `(ind0,ind1,ind2,ind3)`.
	 */
	CVar& operator()(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2, const CIndex& ind3);

	/// \cond NEWandDELoperators
	void* operator new(size_t iSize) throw(CMemoryException);
	void operator delete(void* pPtr) throw();
	void* operator new[](size_t iSize) throw(CMemoryException);
	void operator delete[](void* pPtr) throw();
	/// \endcond

// attributes
	const char* getName() const
		{return m_strName;} ///< \return array name.

	/**
	 * The function resets the array name.
	 * \param[in] name new array name.
	 */
	void setName(const char* name);

	int getSize()
		{return (int)m_pMap->size();} ///< \return number of elements in the array (its size).
};

#endif // #ifndef __VARARRAY__H
