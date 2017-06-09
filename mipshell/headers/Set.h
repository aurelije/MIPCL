///////////////////////////////////////////////////////////////
/**
 * \file Set.h interface for `CSet` class
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
#ifndef __SET__H
#define __SET__H

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

#pragma warning( disable : 4251)

#include <set>
#include <iostream>
#include <except.h>
#include "Index.h"

#define SET_MAX_IT_NUM 4 ///< maximum number of iterators based on a particular set.

/**
 * `CBasicSet` defines an interface for the other classes of set objects.
 */
class MIPSHELL_API CBasicSet
{
public:
	CBasicSet() {}; ///< The default constructor.
	virtual ~CBasicSet() {}; ///< The destructor.

	virtual int getSize()=0; ///< \return number of elements in the set.
	virtual void initIt()=0; ///< initializes the iterator to enumerate set elements.

	/**
	 * \param[in,out] ind on input, index of some element `e` in `*this` set;
	 *  on output, index of the element that immediately follows `e` if such one exists.
	 * \return `false` if `e` is the last element of the set; otherwise, `true`.
	 */
	virtual bool getNextIndex(CIndex& ind)=0;
};

/**
 * `CRange` extends `CBasicSet`, and represents simple sets of successive integers from a given interval.
 */
class MIPSHELL_API CRange: public CBasicSet
{
	int m_iStart, m_iEnd;
	/**
	 * Sets may be used as index sets.
	 * Since __MIPshell__ arrays may have at most `SET_MAX_IT_NUM` indices,
	 * each set object has `SET_MAX_IT_NUM` iterators, and `m_iIt[i]` stores the current position of iterator `i`.
	 */
	int m_iIt[SET_MAX_IT_NUM];
	int m_iCurIt; ///< index of current iterator.
public:
	CRange(); ///< The default constructor that creates a `CRange` object tha stores the empty set.
	 /// The copy constructor.
	/**
	 * \param[in] other reference to a `CRange` object to be copied.
	 */
	CRange(const CRange& other);

	/**
	 * The constructor that initializes a newly created `CRange` object with a given range set.
	 * \param[in] start,end define the set {start,strat+1,...,end}.
	 */
	CRange(int start, int end);

	virtual ~CRange(); ///< The destructor.

	/**
	 * The procedure resets `*this` range set to a new one.
	 * \param[in] start,end define new range set `{start,start+1,...,end}`.
	 */
	void reset(int start, int end)
		{m_iStart=start; m_iEnd=end;}

	int getSize()
	{return m_iEnd-m_iStart+1;} ///< \return size (cardinality) of the set.

	void initIt(); ///< initializes the iterator to enumerate set elements.

	/**
	 * The function return the value of `*this` set element that immediately follows
	 *  the current element, which index is stored internally.
	 * \param[out] val value of the next element.
	 * \return `false` if the current element is the last in the set; otherwise, `true`.
	 */
	bool getNext(int& val);

	/**
	 * The function return the index of the set element that immediately follows
	 *  the current element, which index is stored internally.
	 * \param[out] ind index of the next element if such one exists.
	 * \return `false` if the current element is the last in the set; otherwise, `true`.
	 */
	bool getNextIndex(CIndex& ind);

	bool In(int el); ///< return `true`, if `el` belongs to `*this` object.

	/**
	 * The operator prints to a given output stream the string representation of a given range set.
	 * \param[in] out output stream;
	 * \param[in] r range object to be printed.
	 * \return reference to `out`.
	 * \sa \ref mipshell_inputOutput.
	 */
	friend MIPSHELL_API std::ostream& operator<<(std::ostream& out, CRange& r);

};

/**
 * `CSet` implements basic operations with the sets.
 * \sa \ref mipshell_sets.
 */
template <typename Tell> 
class MIPSHELL_API CSet: public CBasicSet  
{
private:
	/**
	 * Sets may be used index sets.
	 * Since __MIPshell__ arrays may have at most `SET_MAX_IT_NUM` indices,
	 * each set object has `SET_MAX_IT_NUM` iterators, and `m_pIt[i]` stores the current position of iterator `i`.
	 */
	typename std::set<Tell>::iterator m_pIt[SET_MAX_IT_NUM];
	int m_iCurIt; ///< index of current iterator.
	std::set<Tell> m_Set; ///< stores set elements.
public:
	CSet(); ///< The default constructor.

	/**
	 * This constructor creates a set object, and then initializes it with the set stored in string `str`.
	 * \param[in] str string that stores a set.
	 * \sa \ref mipshell_sets.
	 */
	CSet(const char* str);
	virtual ~CSet(); ///< The destructor.
public:
//	int getIndex(Tell el);
// RETURN VALUE: index of el;  < 0 if el does not belong to the set 
//	std::set<Tell>& getSet() {return m_Set;}
	// operations
	void clear(); ///< makes `*this` the empty set.

	int getSize(); ///< \return number of elements in the set.

	void initIt(); ///< initializes the iterator to enumerate set elements.

	/**
	 * The function return the value of the set element that immediately follows
	 *  the current element, which index is stored internally.
	 * \param[out] val value of the next element.
	 * \return `false` if the current element is the last in the set; otherwise, `true`.
	 */
	bool getNext(Tell& val);

	/**
	 * The function return the index of the set element that immediately follows
	 *  the current element, which index is stored internally.
	 * \param[out] ind index of the next element.
	 * \return `false` if the current element is the last in the set; otherwise, `true`.
	 */
	bool getNextIndex(CIndex& ind);

	bool In(Tell el); ///< return `true`, if `el` belongs to `*this` object.

	void add(const Tell& el); ///< adds `el` to the set `*this`.

	/**
	 * The procedure parses a given string a set of type `T`,
	 *  splits it into substrings separated by commas or semicolons,
	 * translates those substrings into elements of type `T`, and, depending  on the value
	 * of `flag` adds (`flag=true`) them to or deletes (`flag=false`) them from the set `*this`.
	 * \param[in] str string representing a set of type `T`;
	 * \param[in] flag if `true`, elements extracted from the string are added to the set `*this`;
	 *  otherwise, the extracted elements are deleted from the set `*this`.
	 *  \return number of parsed characters until the set is extracted or an error is detected.
	 */
	char* joinString(const char* str, bool flag=true);

// opertators
	/**
	 * The operator assigns to `*this` the set `s`.
	 * \param[in] s set.
	 * \return `*this`.
	 */
	CSet& operator=(CSet& s);

	/**
	 * The operator computes the _union_ of `*this` and `s`, and the result is assigned to `*this`.
	 * \param[in] s set.
	 * \return `*this`.
	 */
	CSet& operator+=(CSet& s);	/**

	/**
	 * The operator computes the _difference_ of `*this` and `s`, and the result is assigned to `*this`.
	 * \param[in] s set.
	 * \return `*this`.
	 */
	CSet& operator-=(CSet& s);

	/**
	 * The operator computes the _intersection_ of `*this` and `s`, and the result is assigned to `*this`.
	 * \param[in] s set.
	 * \return `*this`.
	 */
	CSet& operator*=(CSet& s);

	/**
	 * The operator computes the _union_ of `*this` and `s`, and the result is assigned to `*this`.
	 * \param[in] s set.
	 * \return `*this`.
	 */
	CSet& operator+(CSet& s);

	/**
	 * The operator computes the _difference_ of `*this` and `s`.
	 * \param[in] s set.
	 * \return difference of `*this` and `s`.
	 */
	CSet& operator-(CSet& s); ///< \return difference of `*this` and `s`.

	/**
	 * The operator computes the _intersection_ of `*this` and `s`.
	 * \param[in] s set.
	 * \return intersection of `*this` and `s`.
	 */
	CSet& operator*(CSet& s); ///< \return intersection of `*this` and `s`.

	/**
	 * The operator assigns to `*this` the set stored in `str`.
	 * \param[in] str string that stores a set.
	 * \return `*this`.
	 */
	CSet& operator=(const char* str);

	/**
	 * The operator computes the _union_ of `*this` and the set stored in `str`, the result is assigned to `*this`.
	 * \param[in] str st.
	 * \return `*this`.
	 */
	CSet& operator+=(const char* str); ///< adds to `*this` the set stored in `str`. \return `*this`.

	/**
	 * The operator computes the _difference_ of `*this` and the set stored in `str`, the result is assigned to `*this`.
	 * \param[in] str string that stores a set.
	 * \return `*this`.
	 */
	CSet& operator-=(const char* str);

	/**
	 * The operator computes the _intersection_ of `*this` and the set stored in `str`, the result is assigned to `*this`.
	 * \param[in] str string that stores a set.
	 * \return `*this`.
	 */
	CSet& operator*=(const char* str);

	/**
	 * The operator computes the _union_ of `*this` and the set stored in `str`.
	 * \param[in] str string that stores a set.
	 * \return _union_ of `*this` and the set stored in `str`.
	 */
	CSet& operator+(const char* str);

	/**	 *  initializes a given set with the set from a given input stream.
	 *
	 * The operator computes the _difference_ of `*this` and the set stored in `str`.
	 * \param[in] str string that stores a set.
	 * \return _difference_ of `*this` and the set stored in `str`.
	 */
	CSet& operator-(const char* str);

	/**
	 * The operator computes the _intersection_ of `*this` and the set stored in `str`.
	 * \param[in] str string that stores a set.
	 * \return _intersection_ of `*this` and the set stored in `str`.
	 */
	CSet& operator*(const char* str);

	/**
	 * The operator compares two sets `*this` and `s`.
	 * \param[in] s set.
	 * \return `true` if set `*this` includes set `s`.
	 */
	bool operator<(const CSet& s);

	/**
	 * The operator creates a `CSet` object to store the _union_ of the set stored in `str` and the set `s`.
	 * \param[in] str string that stores a set;
	 * \param[in] s set.
	 * \return reference to the newly created `CSet` object that represents the _union_ of the set stored in `str` and the set `s`.
	 */
	template<typename T> friend CSet<T>& operator+(const char* str, CSet<T>& s);

	/**
	 * The operator creates a `CSet` object to store the _difference_ of the set stored in `str` and the set `s`.
	 * \param[in] str string that stores a set;
	 * \param[in] s set.
	 * \return reference to the newly created `CSet` object that represents the _difference_ of the set stored in `str` and the set `s`.
	 */
	template<typename T> friend CSet<T>& operator-(const char* str, CSet<T>& s);

	/**
	 * The operator creates a `CSet` object to store the _intersection_ of the set stored in `str` and the set `s`.
	 * \param[in] str string that stores a set;
	 * \param[in] s set.
	 * \return reference to the newly created `CSet` object that represents the _intersection_ of the set stored in `str` and the set `s`.
	 */
	template<typename T> friend CSet<T>& operator*(const char* str, CSet<T>& s);

	/**
	 * \param[in] el element of type `T`;
	 * \param[in] s set.
	 * \return `true` if `el` belongs to `s`.
	 */
	template<typename T> friend bool operator%(T el, CSet<T>& s);

	/**
	 * \param[in] el string representation of an element;
	 * \param[in] s set.
	 * \return `true` if `el` belongs to `s`.
	 * \remark This operator first converts string into element `e` of type `T`,
	 *  and then checks whether `e` belongs to `s`.
	 */
	template<typename T> friend bool operator%(const char* el, CSet<T>& s);

	/**
	 * The operator initializes a given set with the set from a given input stream.
	 * \param[in] in input stream;
	 * \param[in,out] s set object to be initialized.
	 * \return reference to `in`.
	 * \sa \ref mipshell_inputOutput.
	 */
	template<typename T> friend std::istream& operator>>(std::istream& in, CSet<T>& s);

	/**
	 * The operator prints to a given output stream the string representation of a given set.
	 * \param[in] out output stream;
	 * \param[in] s set object to be printed.
	 * \return reference to `out`.
	 * \sa \ref mipshell_inputOutput.
	 */
	template<typename T> friend std::ostream& operator<<(std::ostream& out, CSet<T>& s);

	/// \cond NEWandDELoperators
	/**
	 * The new operator allocates a memory block of a given size.
	 * \param[in] iSize size of memory block(in bytes).
	 * \return pointer to allocated memory block.
	 * \throws CMemoryException.
	 */
	void* operator new(size_t iSize) throw(CMemoryException);

	/**
	 * The delete operator frees a memory block previously allocated by the new operator.
	 * \param[pPtr] address of memory block to be freed.
	 */
	void operator delete(void* pPtr) throw();

	/**
	 * The new operator allocates a memory block for an array of some objects.
	 * \param[in] iSize size of memory block(in bytes).
	 * \return pointer to allocated memory block.
	 * \throws CMemoryException.
	 */
	void* operator new[](size_t iSize) throw(CMemoryException);

	/**
	 * The delete operator frees a memory block previously allocated for an array of some objects by the new operator.
	 * \param[pPtr] address of memory block to be freed.
	 */
	void operator delete[](void* pPtr) throw();
	/// \endcond
};

#endif // #ifndef __SET__H
