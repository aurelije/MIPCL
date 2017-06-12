///////////////////////////////////////////////////////////////
/**
 * \file Ctr.h Interfaces for  `CTerm`, `CLinSum`, and `CCtr` classes
 *
 * |  __Author__  | N.N. Pisaruk                              |
 * |-------------:|:------------------------------------------|
 * |  __e-mail__  | pisaruk@yandex.ru                         |
 * | __home page__| http://pisaruk.narod.ru                   |
 *
 *   \copyright __2015 Nicolai N. Pisaruk__
 */

 /*  This file is part of the mixed integer class library (MIPCL).
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
#ifndef __CTR__H
#define __CTR__H

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

#define CTR_NAME_LENGTH 23 ///< maximum length (number of characters) of any constraint name.

#include <except.h>

class CVar;
class CProblem;
class CLinSum;
class CCtr;

/**
* A _constraint_ is a linear sum with lower and upper bounds.
* A _linear sum_ (object of type `CLinSum`) is a sum of linear terms,
*  while a _linear term_ is either a real constant or the product of a real constant and  a variable.
*  In __MIPSHELL__, linear terms, linear sums, and constraints
*  are implemented as objects of types `CTerm`, `CLinSum`, and `CCtr`, respectively.
*/

class MIPSHELL_API CTerm
{
	CVar* m_pVar; ///< variable which is part of the term.
	double m_dCoeff; ///< coefficient.
	CTerm* m_pPrev; ///< previous term in the linked list of some linear sum.

public:
	CTerm(); ///< The default constructor.

	/**
	 * The constructor creates a `CTerm` object and initializes it with the term `coff * var`.
	 * \param[in] var variable;
	 * \param[in] coeff real constant.
	 */
	CTerm(CVar* var, double coeff=1.0);

	/// The copy constructor.
	/**
	 * \param[in] other `CTerm` object to be copied.
	 */
	CTerm(CTerm& other);

	virtual ~CTerm(); ///< The destructor.

// operators
	operator CLinSum(); ///< converts `*this` `CTerm` object into a `CLinSum` object.

	/**
	 * The operator creates a `CLinSum` object to represent the linear sum `*this + term`.
	 * \param[in] term linear term.
	 * \return reference to the created `CLinSum` object.
	 */
	CLinSum& operator+(CTerm& term);

	/**
	 * The operator creates a `CLinSum` object to represent the linear sum `*this - term`.
	 * \param[in] term linear term.
	 * \return reference to the created `CLinSum` object.
	 */
	CLinSum& operator-(CTerm& term);

	/**
	 * The operator creates a `CLinSum` object to represent the linear sum `*this + 1.0*var`.
	 * \param[in] var variable.
	 * \return reference to the created `CLinSum` object.
	 */
	CLinSum& operator+(CVar& var);

	/**
	 * The operator creates a `CLinSum` object to represent the linear sum `*this - 1.0*var`.
	 * \param[in] var variable.
	 * \return reference to the created `CLinSum` object.
	 */
	CLinSum& operator-(CVar& var);

	/**
	 * The operator creates a `CLinSum` object to represent the linear sum `*this + lsum`.
	 * \param[in] lsum linear sum.
	 * \return reference to the created `CLinSum` object.
	 */
	CLinSum& operator+(CLinSum& lsum);

	/**
	 * The operator creates a `CLinSum` object to represent the linear sum `*this - lsum`.
	 * \param[in] lsum linear sum.
	 * \return reference to the created `CLinSum` object.
	 */
	CLinSum& operator-(CLinSum& lsum);

	/**
	 * The operator creates a `CLinSum` object to represent the linear sum `*this + a`.
	 * \param[in] a real number.
	 * \return reference to the created `CLinSum` object.
	 */
	CLinSum& operator+(double a);

	/**
	 * The operator creates a `CLinSum` object to represent the linear sum `*this - a`.
	 * \param[in] a real number.
	 * \return reference to the created `CLinSum` object.
	 */
	CLinSum& operator-(double a);

	/**
	 * The operator creates a `CLinSum` object to represent the linear sum `a + term`.
	 * \param[in] a real number;
	 * \param[in] term linear term.
	 * \return reference to the created `CLinSum` object.
	 */
	MIPSHELL_API friend CLinSum& operator+(double a,CTerm& term);

	/**
	 * The operator creates a `CLinSum` object to represent the linear sum `a - term`.
	 * \param[in] a real number;
	 * \param[in] term linear term.
	 * \return reference to the created `CLinSum` object.
	 */
	MIPSHELL_API friend CLinSum& operator-(double a,CTerm& term);


	/**
	 * The operator creates a `CCtr` object representing the constraint `*this - 1.0*var <= 0.0`.
	 * \param[in] var variable.
	 * \return reference to the created `CCtr` object.
	 */
	CCtr& operator<=(CVar& var);

	/**
	 * The operator creates a `CCtr` object representing the constraint `0.0 <= *this - 1.0*var`.
	 * \param[in] var variable.
	 * \return reference to the created `CCtr` object.
	 */
	CCtr& operator>=(CVar& var);


	/**
	 * The operator creates a `CCtr` object representing the constraint `0.0 <= *this - 1.0*var <= 0.0`.
	 * \param[in] var variable.
	 * \return reference to the created `CCtr` object.
	 */
	CCtr& operator==(CVar& var);

	/**
	 * The operator creates a `CCtr` object representing the constraint `*this - term <= 0.0`.
	 * \param[in] term linear term.
	 * \return reference to the created `CCtr` object.
	 */
	CCtr& operator<=(CTerm& term);

	/**
	 * The operator creates a `CCtr` object representing the constraint `0.0 <= *this - term`.
	 * \param[in] term linear term.
	 * \return reference to the created `CCtr` object.
	 */
	CCtr& operator>=(CTerm& term);

	/**
	 * The operator creates a `CCtr` object representing the constraint `0.0 <= *this - term <= 0.0`.
	 * \param[in] term linear term.
	 * \return reference to the created `CCtr` object.
	 */
	CCtr& operator==(CTerm& term);

	/**
	 * The operator creates a `CCtr` object representing the constraint `*this - lsum <= 0.0`.
	 * \param[in] lsum linear sum.
	 * \return reference to the created `CCtr` object.
	 */
	CCtr& operator<=(CLinSum& lsum);

	/**
	 * The operator creates a `CCtr` object representing the constraint `0.0 <= *this - lsum`.
	 * \param[in] lsum linear sum.
	 * \return reference to the created `CCtr` object.
	 */
	CCtr& operator>=(CLinSum& lsum);

	/**
	 * The operator creates a `CCtr` object representing the constraint `0.0 <= *this - lsum <= 0.0`.
	 * \param[in] lsum linear sum.
	 * \return reference to the created `CCtr` object.
	 */
	CCtr& operator==(CLinSum& lsum);

// operations
	/**
	 * The function sets a value of term coefficient.
	 * \param[in] val coefficient value.
	 */
	void setCoeff(double val)
		{m_dCoeff=val;}

	double getCoeff()
		{return m_dCoeff;} ///< \return value of term coefficient.

	/**
	 * The function sets a term variable.
	 * \param[in] var pointer to variable.
	 */
	void setVar(CVar* var)
		{m_pVar=var;}

	CVar* getVar()
		{return m_pVar;} ///< \return term variable.

	void changeSign()
		{m_dCoeff=-m_dCoeff;} ///< changes the sign of term coefficient.

	/**
	 * The function is called when `*this` term is appended to some linear sum.
	 * \param[in] pTerm term that immediately precedes `*this` term in linear sum.
	 */
	void setPrev(CTerm* pTerm)
		{m_pPrev=pTerm;}

	CTerm* getPrev()
		{return m_pPrev;} ///< \return previous term in the linked list of the linear sum to which the term belongs to.
	/// \cond debugMIPshell
// debugging
	void print(); ///< prints the term.
	/// \endcond
};
//////////////////////////////////////////

/**
 * A _linear sum_ is a sum of linear terms (objects of type `CTerm`).
  *  In __MIPSHELL__, linear sums are implemented as objects of type `CLinSum`.
 */
class MIPSHELL_API CLinSum
{
	bool m_bTemp; ///< = true if the object is not temporal
	CTerm* m_pLast; ///< starts linked list of liner sum terms.
public:
	CLinSum(); ///< The default constructor.

	/**
	 * The constructor creates a new `CLinSum` object representing the linear sum `1.0 * v`.
	 * \param[in] var variable.
	 */
	CLinSum(CVar& var);

	/**
	 * The constructor creates a new `CLinSum` object representing the linear sum `term`.
	 * \param[in] term linear term.
	 */
	CLinSum(CTerm& term);

	virtual ~CLinSum(); ///< The destructor.

// operators
	/**
	 * The assignment operator copies to `*this` object the linear sum stored in another `CLinSum` object.
	 * \param[in] lsum linear sum to be copied.
	 */
	CLinSum& operator=(CLinSum& lsum);

	/**
	 * The operator multiplies `*this` linear sum by a constant and stores the result in `*this` object.
	 * \param[in] a real number.
	 * \return reference to `*this`.
	 */
	CLinSum& operator*=(double a);

	/**
	 * The operator multiplies `*this` linear sum by a constant and assigns the result to a newly created `CLinSum` object.
	 * \param[in] a real number.
	 * \return reference to the created object.
	 */
	CLinSum& operator*(double a);

	/**
	 * The operator multiplies linear sum by a constant and assigns the result to a newly created `CLinSum` object.
	 * \param[in] a real number;
	 * \param[in] lsum linear sum to be multiplied.
	 * \return reference to the created object.
	 */
	MIPSHELL_API friend CLinSum& operator*(double a, CLinSum& lsum);

	/**
	 * The operator adds to `*this` linear sum another linear sum and stores the result in `*this` object.
	 * \param[in] lsum linear sum to be added.
	 * \return reference to `*this`.
	 */
	CLinSum& operator+=(CLinSum& lsum);

	/**
	 * The operator subtracts from `*this` linear sum another linear sum and stores the result in `*this` object.
	 * \param[in] lsum linear sum to be subtracted.
	 * \return reference to `*this`.
	 */
	CLinSum& operator-=(CLinSum& lsum);

	/**
	 * The operator adds to `*this` linear sum another linear sum and assigns the result to a newly created `CLinSum` object.
	 * \param[in] lsum linear sum to be added.
	 * \return reference to the created object.
	 */
	CLinSum& operator+(CLinSum& lsum);

	/**
	 * The operator subtracts from `*this` linear sum another linear sum and assigns the result to a newly created `CLinSum` object.
	 * \param[in] lsum linear sum to be subtracted.
	 * \return reference to `*this`.
	 */
	CLinSum& operator-(CLinSum& lsum);


	/**
	 * The operator adds to `*this` linear sum a linear term and assigns the result to a newly created `CLinSum` object.
	 * \param[in] term linear term to be added.
	 * \return reference to the created object.
	 */
	CLinSum& operator+(CTerm& term);

	/**
	 * The operator subtracts from `*this` linear sum a linear term and assigns the result to a newly created `CLinSum` object.
	 * \param[in] term linear term to be added.
	 * \return reference to the created object.
	 */
	CLinSum& operator-(CTerm& term);


	/**
	 * The operator adds to `*this` linear sum the linear term `1.0 * var` and assigns the result to a newly created `CLinSum` object.
	 * \param[in] var variable.
	 * \return reference to the created object.
	 */
	CLinSum& operator+(CVar& var);

	/**
	 * The operator subtracts from `*this` linear sum the linear term `1.0 * var` and assigns the result to a newly created `CLinSum` object.
	 * \param[in] var variable.
	 * \return reference to the created object.
	 */
	CLinSum& operator-(CVar& var);

	/**
	 * The _unary minus_ operator changes the signs of all term coefficients of `*this` linear sum (multilies `*this` by `-1.0`).
	 * \return reference to `*this`.
	 */
	CLinSum& operator-();
	
	/**
	 * The operator adds to `*this` linear sum a linear term and stores the result in `*this` object.
	 * \param[in] term linear term to be added.
	 * \return reference to `*this`.
	 */
	CLinSum& operator+=(CTerm& term);

	/**
	 * The operator subtracts from `*this` linear sum a linear term and stores the result in `*this` object.
	 * \param[in] term linear term to be added.
	 * \return reference to `*this`.
	 */
	CLinSum& operator-=(CTerm& term);

	/**
	 * The operator adds to `*this` linear sum the linear term `1.0 * var` and stores the result in `*this` object.
	 * \param[in] var variable.
	 * \return reference to `*this`.
	 */
	CLinSum& operator+=(CVar& var);

	/**
	 * The operator subtracts from `*this` linear sum the linear term `1.0 * var` and stores the result in `*this` object.
	 * \param[in] var variable.
	 * \return reference to `*this`.
	 */
	CLinSum& operator-=(CVar& var);

	/**
	 * The operator adds to the single constant term (without a variable) of `*this` linear sum a real constant and stores the result in `*this` object.
	 * \param[in] w real number to be added.
	 * \return reference to `*this`.
	 */
	CLinSum& operator+=(double w);

	/**
	 * The operator subtracts from the single constant term (without a variable) of `*this` linear sum a real constant and stores the result in `*this` object.
	 * \param[in] w real number to be subtracted.
	 * \return reference to `*this`.
	 */
	CLinSum& operator-=(double w);

	/**
	 * The operator adds to `*this` linear sum the constant term `w` and stores the result in `*this` object.
	 * \param[in] w constant to be added.
	 * \return reference to `*this`.
	 */
	CLinSum& operator+(double w);

	/**
	 * The operator subtructs from `*this` linear sum the constant term `w` and stores the result in `*this` object.
	 * \param[in] w constant to be subtructed.
	 * \return reference to `*this`.
	 */
	CLinSum& operator-(double w);

	/**
	 * The operator adds to the single constant term (without a variable) of a given linear sum `lsum`
	 * a real constant `a` and stores the result in the `lsum` object.
	 * \param[in] a real number;
	 * \param[in] lsum linear sum.
	 * \return reference to `lsum`.
	 */
	MIPSHELL_API friend CLinSum& operator+(double a, CLinSum& lsum);

	/**
	 * The operator subtracts from the single constant term (without a variable) of a given linear sum `lsum`
	 * a real constant `a` and stores the result in the `lsum` object.
	 * \param[in] a real number;
	 * \param[in] lsum linear sum.
	 * \return reference to `lsum`.
	 */
	MIPSHELL_API friend CLinSum& operator-(double a, CLinSum& lsum);

	/**
	 * The operator creates a new `CCtr` object that represents the constraint `*this <= b`.
	 * \param[in] b value of right hand side.
	 * \return reference to the created constraint.
	 */
	CCtr& operator<=(double b);

	/**
	 * The operator creates a new `CCtr` object that represents the constraint `b <= *this`.
	 * \param[in] b value of left hand side.
	 * \return reference to the created constraint.
	 */
	CCtr& operator>=(double b);

	/**
	 * The operator creates a new `CCtr` object that represents the constraint `b <= *this <= b`.
	 * \param[in] b value of left and right hand sides.
	 * \return reference to the created constraint.
	 */
	CCtr& operator==(double b);

	/**
	 * The operator creates a new `CCtr` object that represents the constraint `*this - var <= 0.0`.
	 * \param[in] var variable.
	 * \return reference to the created constraint.
	 */
	CCtr& operator<=(CVar& var);

	/**
	 * The operator creates a new `CCtr` object that represents the constraint `0.0 <= *this - var`.
	 * \param[in] var variable.
	 * \return reference to the created constraint.
	 */
	CCtr& operator>=(CVar& var);

	/**
	 * The operator creates a new `CCtr` object that represents the constraint `0.0 <= *this - var <= 0.0`.
	 * \param[in] var variable.
	 * \return reference to the created constraint.
	 */
	CCtr& operator==(CVar& var);

	/**
	 * The operator creates a new `CCtr` object that represents the constraint `*this - term <= 0.0`.
	 * \param[in] term linear term.
	 * \return reference to the created constraint.
	 */
	CCtr& operator<=(CTerm& term);

	/**
	 * The operator creates a new `CCtr` object that represents the constraint `0.0 <= *this - term`.
	 * \param[in] term linear term.
	 * \return reference to the created constraint.
	 */
	CCtr& operator>=(CTerm& term);

	/**
	 * The operator creates a new `CCtr` object that represents the constraint `0.0 <= *this - term <= 0.0`.
	 * \param[in] term linear term.
	 * \return reference to the created constraint.
	 */
	CCtr& operator==(CTerm& term);

	/**
	 * The operator creates a new `CCtr` object that represents the constraint `*this - lsum <= 0.0`.
	 * \param[in] lsum linear sum.
	 * \return reference to the created constraint.
	 */
	CCtr& operator<=(CLinSum& lsum);

	/**
	 * The operator creates a new `CCtr` object that represents the constraint `0.0 <= *this - lsum`.
	 * \param[in] lsum linear sum.
	 * \return reference to the created constraint.
	 */
	CCtr& operator>=(CLinSum& lsum);

	/**
	 * The operator creates a new `CCtr` object that represents the constraint `0.0 <= *this - lsum <= 0.0`.
	 * \param[in] lsum linear sum.
	 * \return reference to the created constraint.
	 */
	CCtr& operator==(CLinSum& lsum);

	/**
	 * The operator creates a new `CCtr` object that represents the constraint `b <= lsum`.
	 * \param[in] b value of left hand side.
	 * \param[in] lsum linear sum.
	 * \return reference to the created constraint.
	 */
	MIPSHELL_API friend CCtr& operator<=(double b, CLinSum& lsum);

	/**
	 * The operator creates a new `CCtr` object that represents the constraint `lsum <= b`.
	 * \param[in] b value of right hand side.
	 * \param[in] lsum linear sum.
	 * \return reference to the created constraint.
	 */
	MIPSHELL_API friend CCtr& operator>=(double b, CLinSum& lsum);

	/**
	 * The operator creates a new `CCtr` object that represents the constraint `b <= lsum <= b`.
	 * \param[in] b value of left and right hand sides.
	 * \param[in] lsum linear sum.
	 * \return reference to the created constraint.
	 */
	MIPSHELL_API friend CCtr& operator==(double b, CLinSum& lsum);

//	/// \cond NEWandDELoperators
//	void* operator new(size_t iSize) throw(CMemoryException);
//	void operator delete(void* pPtr) throw();
//	/// \endcond

// operations
	/**
	 * Constraints added to a problem are considered as _permanent_; otherwise, they are _temporal_.
	 * The function makes `*this` constraint permanent.
	 * \sa `makeTemporal()`.
	 */
	void makePermanent()
		{m_bTemp=false;}

	/**
	 * Constraints added to a problem are considered as _permanent_; otherwise, they are _temporal_.
	 * The function makes `*this` constraint temporal.
	 * \sa `makePermanent()`.
	 */
	void makeTemporal()
		{m_bTemp=true;}

	bool isTemporal()
		{return m_bTemp;} ///< return `true` if `*this` constraint is temporal. \sa `makeTemporal()`.


	/**
	 * As soon as  a constraint has been added to some problem, its `CCtr` object can be reset
	 * and used for building another constraint.
	 */
	void reset()
		{m_pLast=0;}

	CTerm* getLastTerm()
		{return m_pLast;} ///< return pointer to last `CTerm` object in the linked list of `*this` linear sum linear terms.

	/**
	 * The function sets a given linear term `*pTerm` to be the last one in the linked list of `*this` linear sum linear terms.
	 * \param[in] pTerm pointer to linear term.
	 */
	void setLastTerm(CTerm* pTerm)
		{m_pLast=pTerm;}

	/**
	 * The function extends linear sum `*this` by adding to it a new term.
	 * \param[in] pTerm pointer to linear term to be added.
	 */
	void addTerm(CTerm* pTerm);

	/**
	 * The function extends linear sum `*this` by adding to it all the terms of linear sum `lsum`.
	 * \param[in] lsum linear sum to be added.
	 * \return reference to `*this`.
	 */
	CLinSum& plusSum(CLinSum& lsum);

	/**
	 * The function extends linear sum `*this` by adding to it all the terms of linear sum `-lsum`.
	 * \param[in] lsum linear sum to be subtracted.
	 * \return reference to `*this`.
	 */
	CLinSum& minusSum(CLinSum& lsum);

	/// \cond debugMIPshell
// debugging
	void print();
	/// \endcond
};
/////////////////////////////////////////////
/**
 * A _constraint_ is a linear sum (object of type `CLinSum`) with lower and upper bounds.
 * In __MIPSHELL__, constraints are implemented as objects of type `CCtr`.
 */
class MIPSHELL_API CCtr
{
	double m_dLHS; ///< left hand side.
	double m_dRHS; ///< right hand side.
	CTerm* m_pLastTerm; ///< starts linked list of terms constituting linear sum of this constraint.
	CCtr* m_pPrev; ///< points to the constraint that precedes this constraint in the linked list of problem constraints.
	int m_iHd; ///< constraint handle.
	char m_strName[CTR_NAME_LENGTH+1]; ///< constraint name.

	void init(); ///< initializes an empty constraint.
public:
	CCtr(); ///< The default constructor.

	/**
	 * The constructor initializes an empty constraint and assigns it a name.
	 * \param[in] name constraint name.
	 */
	CCtr(const char* name);

	virtual ~CCtr(); ///< The destructor.
// operators
	/// The assignment operator.
	/**
	 * \param[in] ctr constraint to be assigned.
	 * \return reference to the newly created `CCtr` object.
	 */
	CCtr& operator=(CCtr& ctr);

	/** The operator sets a new value of the right hand side of this constraint.
	 * \param[in] b right hand side value.
	 * \return `*this`.
	 */
	CCtr& operator<=(double b);

	/** The operator sets a new value of the left hand side of this constraint.
	 * \param[in] b left hand side value.
	 * \return `*this`.
	 */
	CCtr& operator>=(double b);

	/** The operator declares this constraint as equality constraint.
	 * \param[in] b value of both, left and right, hand sides.
	 * \return `*this`.
	 */
	CCtr& operator==(double b);


	/** The operator sets a new value of the left hand side of constraint `ctr`.
	 * \param[in] b left hand side value;
	 * \param[in] ctr constraint.
	 * \return `*this`.
	 */
	MIPSHELL_API friend CCtr& operator<=(double b, CCtr& ctr);

	/** The operator sets a new value of the right hand side of constraint `ctr`.
	 * \param[in] b right hand side value;
	 * \param[in] ctr constraint.
	 * \return reference to `ctr`.
	 */
	MIPSHELL_API friend CCtr& operator>=(double b, CCtr& ctr);

	/** The operator declares constraint `ctr` as equality constraint.
	 * \param[in] b value of both, left and right, hand sides;
	 * \param[in] ctr constraint.
	 * \return reference to `ctr`.
	 */
	MIPSHELL_API friend CCtr& operator==(double b, CCtr& ctr);

//	/// \cond NEWandDELoperators
//	void* operator new(std::size_t iSize) throw(CMemoryException);
//	void operator delete(void* pPtr) throw();
//	void* operator new[](std::size_t iSize) throw(CMemoryException);
//	void operator delete[](void* pPtr) throw();
//	/// \endcond
	
// operations
	const char* getName() const
		{return m_strName;} ///< \return name of this constraint.

	/**
	 * Very often, it is convenient to assign to constraints "indexed" names such as
	 * the following ones: `capacity(i)`, `balance(i,j)`, where `i` and `j` are nonnegative integers.
	 * `setName()` is used to assign indexed names with at most three indices.
	 * \param[in] name base name (without indices);
	 * \param[in] ind1,ind2,ind3 indices; indices with negative values are skiped.
	 */
	void setName(const char* name, int ind1=-1, int ind2=-1, int ind3=-1);

	/**
	 * The function sets the handle of this constraint.
	 * \param[in] hd handle value.
	 */
	void setHandle(int hd)
		{m_iHd=hd;}

	int getHandle()
		{return m_iHd;} ///< handle of this constraint.

	void setLocal()
		{m_iHd=-1;} ///< marks this constraint as being local.

	bool isGlobal()
		{return (m_iHd >= 0)? false: true;} ///< \return `true` if this constraint is global.

	/**
	 * The function sets the value of left hand side of this constraint.
	 * \param[in] b left hand side value.
	 */
	void setLHS(double b)
		{m_dLHS=b;}

	double getLHS()
		{return m_dLHS;}  ///< \return left hand side of this constraint.

	/**
	 * The function sets the value of right hand side of this constraint.
	 * \param[in] b right hand side value.
	 */
	void setRHS(double b)
		{m_dRHS=b;}

	double getRHS()
		{return m_dRHS;} ///< \return right hand side of this constraint.

	/**
	 * The function is called when this constraint is added to the linked list of problem constraints.
	 * \param[in] ctr pointer to constraint that precedes this constraint.
	 */
	void setPrev(CCtr* ctr)
		{m_pPrev=ctr;}

	CCtr* getPrev()
		{return m_pPrev;} ///< \return predecessor of this constraint in the linked list of constraints of some problem.

	/**
	 * The function adds a term to the linked list of terms of the linear term of this constraint.
	 * \param[in] pTerm `CTerm` object being added.
	 */
	void setLastTerm(CTerm* pTerm)
		{m_pLastTerm=pTerm;}

	CTerm* getLastTerm()
		{return m_pLastTerm;} ///< \return last term in the linked list of linear sum of this constraint.

	/// \cond debugMIPshell
// debugging
	void print();
	/// \endcond
};

#endif // #ifndef __CTR__H
