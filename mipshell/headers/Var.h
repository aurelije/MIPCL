///////////////////////////////////////////////////////////////
/**
 * \file Var.h interface for CVar class
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

#ifndef __VAR__H
#define __VAR__H

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

#ifndef VAR_NAME_LENGTH
#define VAR_NAME_LENGTH 31 ///< maximum name length of variable.
#endif

#ifndef VARARR_NAME_LENGTH
#define VARARR_NAME_LENGTH 15 ///< maximum name length of variable in `CVarArray` objects.
#endif

#ifndef MAX_ARR_DIM
#define MAX_ARR_DIM 4 ///< maximum dimension of any array.
#endif


#include <except.h>

class CTerm;
class CLinSum;
class CCtr;
class CProblem;

/// Types of variables.
enum enMipshellVarType {
	REAL     = 0x00000000, ///< variable that may take any real value.
	REAL_GE  = 0x00010000, ///< variable that may take any non-negative real value.
	REAL_LE  = 0x00020000, ///< variable that may take any non-positive real value.
	INTEGER  = 0x00100000, ///< variable that may take any integer value.
	INT_GE   = 0x00110000, ///< variable that may take any non-negative integer value.
	INT_LE   = 0x00120000, ///< variable that may take any non-positive integer value.
	BIN      = 0x00200000  ///< binary variable that takes only two values, 0 or 1.
};

/**
 * In __MIPshell__ models, objects of type `CVar` represent variables.
 */
class MIPSHELL_API CVar  
{
	char m_strName[VAR_NAME_LENGTH+1]; ///< name, which is used when printing the solution.
	double m_dLb; ///< lower bound.
	double m_dUb; ///< upper bound.
//	double m_dVal; ///< value (it is computed only if a solution has been found).
	int m_iHd; ///< handle.
	int m_iType; ///< type.
	CVar* m_pPrev; ///< previous variable in the linked list of problem variables.
private:
	void init();
public:
	CVar(); ///< The default constructor.

	/**
	 * The constructor that creates a new variable (`CVar` object), and adds that variable to a specified problem.
	 * \param[in] prob problem to which newly created variable is added;
	 * \param[in] name variable name;
	 * \param[in] type variable type.
	 */
	CVar(CProblem* prob, const char* name, enMipshellVarType type=REAL_GE);

	/// The copy constructor.
	/**
	 * \param[in] other `Cvar` object to be copied.
	 */
	CVar(CVar& other);

	virtual ~CVar(); ///< The destructor.

// operators
	operator CLinSum(); ///< converts `*this` `CVar` object into a `CLinSum` object.

	/**
	 * The operator creates a linear term (object of type `CTerm`), which is the _product_ of a real coefficient and a variable.
	 * \param[in] coeff coefficient;
	 * \param[in] var variable.
	 * \return reference to created `CTerm` object representing the product `coeff * var`.
	 */
	MIPSHELL_API friend CTerm& operator*(double coeff, CVar& var);

	/**
	 * The operator creates a linear term (object of type `CTerm`) that is the _product_ of a real coefficient and `*this` variable.
	 * \param[in] coeff coefficient;
	 * \return reference to created `CTerm` object representing the product `coeff * (*this)`.
	 */
	CTerm& operator*(double coeff);

	/**
	 * The operator creates a linear sum (object of type `CLinSum`)
	 *  that is the _sum_ of variable `*this` and a linear term.
	 * \param[in] term linear term;
	 * \return reference to created `CLinSum` object representing the sum `*this + term`.
	 */
	CLinSum& operator+(CTerm& term);

	/**
	 * The operator creates a linear sum (object of type `CLinSum`)
	 *  that is the _difference_ of variable `*this` and a linear term.
	 * \param[in] term linear term;
	 * \return reference to created `CLinSum` object representing the difference `*this - term`.
	 */
	CLinSum& operator-(CTerm& term);

	/**
	 * The operator creates a linear sum (object of type `CLinSum`)
	 *  that is the _sum_ of variable `*this` and a variable.
	 * \param[in] var variable;
	 * \return reference to created `CLinSum` object representing the sum `*this + var`.
	 */
	CLinSum& operator+(CVar& var);

	/**
	 * The operator creates a linear sum (object of type `CLinSum`)
	 *  that is the _difference_ of variable `*this` and a variable.
	 * \param[in] var variable;
	 * \return reference to created `CLinSum` object representing the difference `*this - var`.
	 */
	CLinSum& operator-(CVar& var);

	/**
	 * The operator creates a linear sum (object of type `CLinSum`)
	 *  that is the _sum_ of variable `*this` and a linear sum.
	 * \param[in] lsum linear sum;
	 * \return reference to created `CLinSum` object representing the sum `*this + lsum`.
	 */
	CLinSum& operator+(CLinSum& lsum);

	/**
	 * The operator creates a linear sum (object of type `CLinSum`)
	 *  that is the _difference_ of variable `*this` and a linear sum.
	 * \param[in] lsum linear sum;
	 * \return reference to created `CLinSum` object representing the difference `*this - lsum`.
	 */
	CLinSum& operator-(CLinSum& lsum);

	/**
	 * The operator creates a linear sum (object of type `CLinSum`)
	 *  that is the _sum_ of variable `*this` and a constant.
	 * \param[in] a real constant;
	 * \return reference to created `CLinSum` object representing the sum `*this + a`.
	 */
	CLinSum& operator+(double a);

	/**
	 * The operator creates a linear difference (object of type `CLinSum`)
	 *  that is the _difference_ of variable `*this` and a constant.
	 * \param[in] a real constant;
	 * \return reference to created `CLinSum` object representing the difference `*this - a`.
	 */
	CLinSum& operator-(double a);


	/**
	 * The operator creates a linear sum (object of type `CLinSum`)
	 *  that is the _sum_ of a constant and a variable.
	 * \param[in] a real constant;
	 * \param[in] var variable to be added to real constant.
	 * \return reference to created `CLinSum` object representing the sum `*a + var`.
	 */
	MIPSHELL_API friend CLinSum& operator+(double a,CVar& var);

	/**
	 * The operator creates a linear sum (object of type `CLinSum`)
	 *  that is the _difference_ of a constant and a variable.
	 * \param[in] a real constant;
	 * \param[in] var variable to be subtracted from real constant.
	 * \return reference to created `CLinSum` object representing the sum `*a + var`.
	 */
	MIPSHELL_API friend CLinSum& operator-(double a,CVar& var);

	/**
	 * The operator sets a new _upper_ bound for `*this` variable.
	 * \param[in] d value of new upper bound.
	 * \return `*this`.
	 */
	CVar& operator<=(double d)
		{m_dUb=d; return *this;}

	/**
	 * The operator sets a new _lower_ bound for `*this` variable.
	 * \param[in] d value of new lower bound.
	 * \return `*this`.
	 */
	CVar& operator>=(double d)
		{m_dLb=d; return *this;}

	/**
	 * The operator _fixes_ `*this` variable (sets its _lower_ and _upper_ bounds to the same value).
	 * \param[in] d value of new lower and lower bounds.
	 * \return reference to `*this`.
	 */
	CVar& operator==(double d)
		{m_dLb=m_dUb=d; return *this;}

	/**
	 * The operator sets a new _lower_ bound for a given variable.
	 * \param[in] d value of new lower bound;
	 * \param[in] var variable which lower bound be changed.
	 * \return reference to `var`.
	 */
	/*MIPSHELL_API*/ friend CVar& operator<=(double d, CVar& var)
		{var.m_dLb=d; return var;}

	/**
	 * The operator sets a new _upper_ bound for a given variable.
	 * \param[in] d value of new upper bound;
	 * \param[in] var variable which upper bound be changed.
	 * \return reference to `var`.
	 */
	/*MIPSHELL_API*/ friend CVar& operator>=(double d, CVar& var)
		{var.m_dUb=d; return var;}

	/**
	 * The operator _fixes_ variable `var` (sets its _lower_ and _upper_ bounds to the same value).
	 * \param[in] d value of new lower and lower bounds.
	 * \param[in] var variable to be fixed.
	 * \return reference to `var`.
	 */
	/*MIPSHELL_API*/ friend CVar& operator==(double d, CVar& var)
		{var.m_dLb=var.m_dUb=d; return var;}

	/**
	 * The operator creates a new constraint `*this <= var`.
	 * \param[in] var variable.
	 * \return reference to newly created constraint.
	 */
	CCtr& operator<=(CVar& var);

	/**
	 * The operator creates a new constraint `*this >= var`.
	 * \param[in] var variable.
	 * \return reference to newly created constraint.
	 */
	CCtr& operator>=(CVar& var);

	/**
	 * The operator creates a new constraint `*this == var`.
	 * \param[in] var variable.
	 * \return reference to newly created constraint.
	 */
	CCtr& operator==(CVar& var);

	/**
	 * The operator creates a new constraint `*this <= term`.
	 * \param[in] term linear term.
	 * \return reference to newly created constraint.
	 */
	CCtr& operator<=(CTerm& term);

	/**
	 * The operator creates a new constraint `*this >= term`.
	 * \param[in] term linear term.
	 * \return reference to newly created constraint.
	 */
	CCtr& operator>=(CTerm& term);

	/**
	 * The operator creates a new constraint `*this == term`.
	 * \param[in] term linear term.
	 * \return reference to newly created constraint.
	 */
	CCtr& operator==(CTerm& term);

	/**
	 * The operator creates a new constraint `*this <= lsum`.
	 * \param[in] lsum linear sum.
	 * \return reference to newly created constraint.
	 */
	CCtr& operator<=(CLinSum& lsum);

	/**
	 * The operator creates a new constraint `*this >= lsum`.
	 * \param[in] lsum linear sum.
	 * \return reference to newly created constraint.
	 */
	CCtr& operator>=(CLinSum& lsum);

	/**
	 * The operator creates a new constraint `*this == lsum`.
	 * \param[in] lsum linear sum.
	 * \return reference to newly created constraint.
	 */
	CCtr& operator==(CLinSum& lsum);

	/// \cond NEWandDELoperators
	void* operator new(size_t iSize) throw(CMemoryException);
	void operator delete(void* pPtr) throw();
	void* operator new[](size_t iSize) throw(CMemoryException);
	void operator delete[](void* pPtr) throw();
	/// \endcond

// operations
	const char* getName() const
		{return m_strName;} ///< \return name of `*this` variable.

	/**
	 * The function resets the name of `*this` variable.
	 * \param name new name.
	 */
	void setName(const char* name);

	/**
	 * The function resets the handle of `*this` variable.
	 * \param hd new handle.
	 */
	void setHandle(int hd)
		{m_iHd=hd;}

	int getHandle()
		{return m_iHd;} ///< \return handle of `*this` variable.

	double getLb()
		{return m_dLb;} ///< \return lower bound  of `*this` variable.

	double getUb()
		{return m_dUb;} ///< \return upper bound  of `*this` variable.

	/**
	 * The function sets the lower bound of `*this` variable.
	 * \param[in] Lb new lower bound.
	 */
	void setLb(double Lb)
		{m_dLb=Lb;}

	/**
	 * The function sets the upper bound of `*this` variable.
	 * \param[in] Ub new upper bound.
	 */
	void setUb(double Ub)
		{m_dUb=Ub;}

	/**
	 * The function sets the priority of `*this` variable.
	 * \param[in] pr new priority.
	 */
	void setPriority(int pr)
		{m_iType = (m_iType & 0xFFFFF000) | (pr & 0x00000FFF);}
	
	int getPriority()
		{return (m_iType & 0x00000FFF);} ///< \return priory of `*this` variable.

	bool isInteger()
		{return (m_iType & (INTEGER | BIN))? true: false;} ///< return `true` if variable is integer (of type `INTEGER`) or binary (of type `BIN`).
	void setInteger()
		{m_iType=(m_iType & 0x0000FFFF) | INTEGER;} ///< declares variable as being integer.
	bool isBinary()
		{return (m_iType & BIN)? true: false;} ///< return `true` if variable is binary (of type `BIN`).
	void setBinary()
		{m_iType&=(m_iType & 0x0000FFFF) | BIN;}///< declares variable as being binary.

	/**
	 * The function sets the type of `*this` variable.
	 * \param[in] type new type.
	 */
	void setType(int type);

	/**
	 * The function declares variable `var` as previous to `*this`
	 * in the linked list of problem variables when variable `*this` is added to the list.
	 * \param[in]  var previous variable.
	 */
	void setPrev(CVar* var)
		{m_pPrev=var;}

	CVar* getPrev()
		{return m_pPrev;} ///< \return previous variable in the linked list of problem variables.
};

/**
 * Any object of type `CVarVector` represents a list of variables of the same type.
 * The variables in the list are numbered from `0` to `n-1`,
 *  where `n` is the number of variables in the vector.
 */

class MIPSHELL_API CVarVector
{
	char m_strName[VARARR_NAME_LENGTH+1];
	int m_iDim; ///< dimension, 1 <= m_iDim <= MAX_VARARR_SIZE
	int m_ipSize[MAX_ARR_DIM];
	CVar* m_pVars; ///< array of variables.

public:
	CVarVector(); ///< The default constructor.

	/**
	 * The constructor creates a vector of variables of given type,
	 *  and then adds all the vector variable to a given problem.
	 *  The maximum dimension (number of indices) of a vector is 4.
	 *  \param[in] prob problem to which vector variables are added;
	 *  \param[in] name vector name;
	 *  \param[in] type extended type of all vector variables;
	 *  \param[in] size0,size1,size2,size3 sizes of each of four dimensions;
	 *  to declare a vector of, say, dimension 2, the value of `sise2` and `size3` must be omitted.
	 *  \remark The name of a particular variable is the concatenation of the vector name and
	 *  the list of its indices. For example, if we have a 3-dimensional array named `xxx`, then
	 *  the variable indexed by `(2,4,3)` is named `xxx(2,4,3)`.
	 */
	CVarVector(CProblem* prob, char const* name, int type,
		int size0, int size1=0, int size2=0, int size3=0);

	virtual ~CVarVector(); ///< The destructor.

	/**
	 * The function initializes `*this` vector, i.e., sets its name, type of its variables,
	 * and the dimension.
	 *  \param[in] prob problem to which array variables be added;
	 *  \param[in] name array name;
	 *  \param[in] type extended type of variables;
	 *  \param[in] size0,size1,size2,size3 sizes for each of four possible dimensions.
	 */
	void init(CProblem* prob, const char* name, int type,
		int size0, int size1=0, int size2=0, int size3=0);

// operators
	/**
	 * The access operator returns the vector element (variable) with a given index.
	 * \param[in] ind index of vector variable.
	 * \return reference to the vector variable indexed by `ind`.
	 */
	CVar& operator[](int ind)
		{return m_pVars[ind];}

	/**
	 * The access operator returns the vector element (variable) with a given index.
	 * \param[in] ind index of vector variable.
	 * \return reference to the vector variable indexed by `ind`.
	 */
	CVar& operator()(int ind)
		{return m_pVars[ind];}

	/**
	 * The access operator returns the vector element (variable) with given indices.
	 * \param[in] ind0,ind1 indices of vector variable.
	 * \return reference to the vector variable indexed by `ind0,ind1`.
	 */
	CVar& operator()(int ind0, int ind1)
		{return m_pVars[ind0*m_ipSize[1]+ind1];}

	/**
	 * The access operator returns the vector element (variable) with given indices.
	 * \param[in] ind0,ind1,ind2 indices of vector variable.
	 * \return reference to the vector variable indexed by `ind0,ind1,ind2`.
	 */
	CVar& operator()(int ind0, int ind1, int ind2)
		{return m_pVars[(ind0*m_ipSize[1]+ind1)*m_ipSize[2]+ind2];}

	/**
	 * The access operator returns the vector element (variable) with given indices.
	 * \param[in] ind0,ind1,ind2,ind3 indices of vector variable.
	 * \return reference to the vector variable indexed by `ind0,ind1,ind2,ind3`.
	 */
	CVar& operator()(int ind0, int ind1, int ind2, int ind3)
	{return m_pVars[((ind0*m_ipSize[1]+ind1)*m_ipSize[2]+ind2)*m_ipSize[3]+ind3];}

	/// \cond NEWandDELoperators
	void* operator new(size_t iSize);
	void operator delete(void* pPtr);
	/// \endcond

// attributes
	const char* getName() const
		{return m_strName;} ///< \return vector name.

	/**
	 * The function resets the vector name, type of its variables,
	 *  and then adds all the array variables to a given problem.
	 *  \param[in] prob problem to which array variables be added;
	 *  \param[in] name new array name;
	 *  \param[in] type extended type of vector variables.
	 */
	void setName(CProblem* prob, const char* name, int type=REAL);

	/**
	 * The procedure the dimension and the size of each dimension of `*this` vector.
	 * \param[out] ipSize array of size `dim`, where `dim` is return value;
	 * `ipSize[i]` is size for dimension `i`.
	 * \return dimension (number of indices) of `*this` vector.
	 */
	int getSize(int*& ipSize)
		{ipSize=m_ipSize; return m_iDim;}

	int getSize();  ///< \return the array size (number of variables in it).

};

#endif // #ifndef __VAR__H
