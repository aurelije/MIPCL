/**
 * \file Array.h Interface for `CArray` class
 * |  __Author__  | N.N. Pisaruk                              |
 * |-------------:|:------------------------------------------|
 * |  __e-mail__  | nicolaipisaruk@gmail.com                  |
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
#ifndef __ARRAY__H
#define __ARRAY__H

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

#include <map>
#include <except.h>
#include "Set.h"

/**
 In __MIPshell__ dense arrays are called _vectors_,
 * while sparse arrays (with many zero/empty entries which are not stored) are simply called _arrays_
 * and are represented as objects of class `CArray`.
 * \sa \ref mipshell_arrays.
 */
template <typename Tell>
class MIPSHELL_API CArray: public CBasicSet
{
private:
	Tell m_Zero; ///< value of `zero`.
	std::map<CIndex,Tell> m_Map;

	typename std::map<CIndex,Tell>::iterator m_pIt[SET_MAX_IT_NUM]; ///< array of iterators.
	int m_iCurIt; ///< currently used iterator.

public:
	CArray(); ///< The default constructor.

	/**
	 * The constructor creates a new `CArray` object and initializes it from a string describing some array.
	 * An example of such a string is as follows.
	 *
	 *       {(A1,B2) => 4.0; (A7,B4) => 3.14; (21,3) => -12.4}
	 *
	 * \param[in] str string representing an array.
	 * \throw CDataException.
	 */
	CArray(const char* str);

	 /// The copy constructor.
	/**
	 * \param[in] ar array to be copied.
	 */
	CArray(CArray& ar);
	virtual ~CArray(); ///< The destructor.

// BasicSet operations
	int getSize(); ///< \return array size (number of elements).
	void initIt(); ///< initializes the current iterator to enumerate array elements.

	/**
	 * \param[in,out] ind on input, index of some element `e` in `*this` array;
	 *  on output, index of the element that immediately follows `e` if such one exists.
	 * \return `false` if `e` is the last element of the set; otherwise, `true`.
	 */
	bool getNextIndex(CIndex& ind);

	/**
	 * This function does the same as `getNextIndex()` does.
	 * \param[in,out] ind on input, index of some element `e` in `*this` array;
	 *  on output, index of the element that immediately follows `e` if such one exists.
	 * \return `false` if `e` is the last element of the set; otherwise, `true`.
	 */
	bool getNext(CIndex& ind);

	// operations
	/**
	 * The access operator returns the array element  with a given index.
	 * \param[in] ind index of array element.
	 * \return reference to the element in the array indexed by `ind`.
	 */
	Tell& operator[](const CIndex& ind);

	/**
	 * The access operator returns the array element  with a given index.
	 * \param[in] ind index of array element.
	 * \return reference to the element in the array indexed by `ind`.
	 */
	Tell& operator()(const CIndex& ind);

	/**
	 * The access operator returns the array element  with given indices.
	 * \param[in] ind0,ind1 indices of array element.
	 * \return reference to the element in the array indexed by `ind0,ind1`.
	 */
	Tell& operator()(const CIndex& ind0, const CIndex& ind1);

	/**
	 * The access operator returns the array element  with given indices.
	 * \param[in] ind0,ind1,ind2 indices of array element.
	 * \return reference to the element in the array indexed by `ind0,ind1,ind2`.
	 */
	Tell& operator()(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2);

	/**
	 * The access operator returns the array element  with given indices.
	 * \param[in] ind0,ind1,ind2,ind3 indices of array element.
	 * \return reference to the element in the array indexed by `ind0,ind1,ind2,ind3`.
	 */
	Tell& operator()(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2, const CIndex& ind3);

	/**
	 * The functions adds to `*this` array a new element with zero value and indexed by `ind`,
	 * \param[in] ind index of new element.
	 * \return reference to the element indexed by `ind`.
	 */
	Tell& add(const CIndex& ind);

	/**
	 * The functions adds to `*this` array a new element with zero value and indexed by `ind0,ind1`,
	 * \param[in] ind0,ind1 indices of new element.
	 * \return reference to the element indexed by `ind0,ind1`.
	 */
	Tell& add(const CIndex& ind0, const CIndex& ind1);

	/**
	 * The functions adds to `*this` array a new element with zero value and indexed by `ind0,ind1,ind2`,
	 * \param[in] ind0,ind1,ind2 indices of new element.
	 * \return reference to the element indexed by `ind0,ind1,ind2`.
	 */
	Tell& add(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2);

	/**
	 * The functions adds to `*this` array a new element with zero value and indexed by `ind0,ind1,ind2,ind3`,
	 * \param[in] ind0,ind1,ind2,ind3 indices of new element.
	 * \return reference to the element indexed by `ind0,ind1,ind2,ind3`.
	 */
	Tell& add(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2, const CIndex& ind3);

	/**
	 * The assignment operator initializes `*this` array from a string describing some array.
	 * An example of such a string is as follows.
	 *
	 *       {(A1,B2) => 4.0; (A7,B4) => 3.14; (21,3) => -12.4}
	 *
	 * \param[in] str string representing an array.
	 * \return reference to `*this`.
	 * \throw CDataException.
	 */
	CArray& operator=(const char* str);

	/**
	 * The operator initializes a given array with the array from a given input stream.
	 * \param[in] in input stream;
	 * \param[in,out] arr array object to be initialized.
	 * \return reference to `in`.
	 * \sa \ref mipshell_inputOutput.
	 */
	template<typename T> friend std::istream& operator>>(std::istream& in, CArray<T> &arr);

	/**
	 * The operator prints to a given output stream the string representation of a given array.
	 * \param[in] out output stream;
	 * \param[in] arr array to be printed.
	 * \return reference to `out`.
	 * \sa \ref mipshell_inputOutput.
	 */
	template<typename T> friend std::ostream& operator<<(std::ostream& out, CArray<T>& arr);

	/// \cond NEWandDELoperators
	void* operator new(std::size_t iSize) throw(CMemoryException);
	void operator delete(void* pPtr) throw();
	void* operator new[](std::size_t iSize) throw(CMemoryException);
	void operator delete[](void* pPtr) throw();
	void* operator new(std::size_t , void* pPtr)
		{return pPtr;}
	void operator delete(void* pPtr, void*) {}
	/// \endcond

// usefull functions
	Tell& max(); ///< \return array element of maximum value (key).
	Tell& min(); ///< \return array element of minimum value (key).
private:
	Tell& getZero(); ///< value of zero for `Tell`.
};

/////////////////////////////////////////////////
/**
 *  Specialization `CArray<void*>` is used when implementing `CArray<Tell>` for pointers.
 */
template<>
class MIPSHELL_API CArray<void*>
{
private:
	std::map<CIndex,void*> m_Map;

	std::map<CIndex,void*>::iterator m_pIt[SET_MAX_IT_NUM]; ///< itertors to enumerate `*this` array elements.
	int m_iCurIt; ///< current iterator.

public:
	CArray(); ///< The defaul constructor.
	CArray(CArray& ar); ///< The copy constructor.
	virtual ~CArray(); ///< The destructor.

// BasicSet operations
	int getSize(); ///< \return array size (number of elements).
	void initIt(); ///< initializes the current iterator to enumerate array elements.

	/**
	 * \param[in,out] ind on input, index of some element `e` in `*this` array;
	 *  on output, index of the element that immediately follows `e` if such one exists.
	 * \return `false` if `e` is the last element of the set; otherwise, `true`.
	 */
	bool getNextIndex(CIndex& ind);

	/**
	 * This function does the same as `getNextIndex()` does.
	 * \param[in,out] ind on input, index of some element `e` in `*this` array;
	 *  on output, index of the element that immediately follows `e` if such one exists.
	 * \return `false` if `e` is the last element of the set; otherwise, `true`.
	 */
	bool getNext(CIndex& ind);

	/**
	 * Three functions `getFirstPtr()`, `getNextPtr()`, and `getCurIndex()` are internally
	 * used when implementing `CArray<Tell*>` to initialize and later to delete array elements.
	 */
	void* getFirstPtr(); ///< \return the first pointer in the array.

	void* getNextPtr(); ///< \return the next to the current pointer. \sa `getFirstPtr()`.
	const CIndex& getCurIndex(); ///< the index of the current pointer. \sa `getFirstPtr()`.

protected:
	// operations
	/**
	 * The access operator returns the pointer with a given index.
	 * \param[in] ind index of required pointer.
	 * \return pointer indexed by `ind`.
	 */
	void* operator[](const CIndex& ind);

	/**
	 * The access operator returns the pointer with a given index.
	 * \param[in] ind index of required pointer.
	 * \return pointer indexed by `ind`.
	 */
	void* operator()(const CIndex& ind);

	/**
	 * The access operator returns the pointer with given indices.
	 * \param[in] ind0,ind1 indices of required pointer.
	 * \return pointer indexed by `ind0,ind1`.
	 */
	void* operator()(const CIndex& ind0, const CIndex& ind1);

	/**
	 * The access operator returns the pointer with given indices.
	 * \param[in] ind0,ind1,ind2 indices of required pointer.
	 * \return pointer indexed by `ind0,ind1,ind2`.
	 */
	void* operator()(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2);

	/**
	 * The access operator returns the pointer with given indices.
	 * \param[in] ind0,ind1,ind2,ind3 indices of required pointer.
	 * \return pointer indexed by `ind0,ind1,ind2,ind3`.
	 */
	void* operator()(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2, const CIndex& ind3);

	/**
	 * The functions adds to `*this` array a given pointer to be indexed with a given index,
	 * \param[in] pEll pointer;
	 * \param[in] ind index of `pEll`.
	 */
	void add(void *pEll, const CIndex& ind);

	/**
	 * The functions adds to `*this` array a given pointer to be indexed with given indices,
	 * \param[in] pEll pointer;
	 * \param[in] ind0,ind1 indices of `pEll`.
	 */
	void add(void *pEll, const CIndex& ind0, const CIndex& ind1);

	/**
	 * The functions adds to `*this` array a given pointer to be indexed with given indices,
	 * \param[in] pEll pointer;
	 * \param[in] ind0,ind1,ind2 indices of `pEll`.
	 */
	void add(void *pEll, const CIndex& ind0, const CIndex& ind1, const CIndex& ind2);

	/**
	 * The functions adds to `*this` array a given pointer to be indexed with given indices,
	 * \param[in] pEll pointer;
	 * \param[in] ind0,ind1,ind2,ind3 indices of `pEll`.
	 */
	void add(void *pEll, const CIndex& ind0, const CIndex& ind1, const CIndex& ind2, const CIndex& ind3);

	/// \cond NEWandDELoperators
	void* operator new(std::size_t iSize);
	void operator delete(void* pPtr);
	void* operator new[](std::size_t iSize);
	void operator delete[](void* pPtr);

	void* operator new(std::size_t , void* pPtr)
	{return pPtr;}
	void operator delete(void* pPtr, void*) {}
	/// \endcond
};

////////////////////////////////////////////////////
/// Specialization of `CArray<Tell>` for pointers.
/**
 * This specialization is useful in those cases when array elements are big in size
 * and they were previously stored in some other data structure.
 * It would be inefficient to copy those elements into the array.
 * Instead, it is better to store in the array only pointers to the elements.
 */
template <typename Tell>
class MIPSHELL_API CArray<Tell*>: CArray<void*>
{
	typedef CArray<void*> Base; ///< `Base` is alias for `CArray<void*>`.
private:
	Tell m_Zero; ///< zero element value.
public:
	CArray();///< The default constructor.

	/**
	 * The constructor creates a new `CArray` object and initializes it from a string describing some array.
	 * An example of such a string is as follows.
	 *
	 *       {(A1,B2) => 4.0; (A7,B4) => 3.14; (21,3) => -12.4}
	 *
	 * \param[in] str string representing an array.
	 * \throw CDataException.
	 */
	CArray(const char* str);

	 /// The copy constructor.
	/**
	 * \param[in] ar array to be copied.
	 */
	CArray(CArray& ar);

	virtual ~CArray(); ///< The destructor.

// operations
	/**
	 * The access operator returns the reference to the array element  with a given index.
	 * \param[in] ind index of array element.
	 * \return reference to the element in the array indexed by `ind`.
	 */
	Tell& operator[](const CIndex& ind);

	/**
	 * The access operator returns the reference to the array element  with a given index.
	 * \param[in] ind index of array element.
	 * \return reference to the element in the array indexed by `ind`.
	 */
	Tell& operator()(const CIndex& ind);

	/**
	 * The access operator returns the reference to the array element  with a given indices.
	 * \param[in] ind0,ind1 indices of array element.
	 * \return reference to the element in the array indexed by `ind0,ind1`.
	 */
	Tell& operator()(const CIndex& ind0, const CIndex& ind1);

	/**
	 * The access operator returns the reference to the array element  with a given indices.
	 * \param[in] ind0,ind1,ind2 indices of array element.
	 * \return reference to the element in the array indexed by `ind0,ind1,ind2`.
	 */
	Tell& operator()(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2);

	/**
	 * The access operator returns the reference to the array element  with a given indices.
	 * \param[in] ind0,ind1,ind2,ind3 indices of array element.
	 * \return reference to the element in the array indexed by `ind0,ind1,ind2,ind3`.
	 */
	Tell& operator()(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2, const CIndex& ind3);

	/**
	 * The functions adds to `*this` array a pointer to newly created element indexed by `ind`,
	 * \param[in] ind index of new element.
	 * \return reference to the element indexed by `ind`.
	 */
	Tell& add(const CIndex& ind);

	/**
	 * The functions adds to `*this` array a pointer to newly created element indexed by `ind0,ind1`,
	 * \param[in] ind0,ind1 indices of new element.
	 * \return reference to the element indexed by `ind0,ind1`.
	 */
	Tell& add(const CIndex& ind0, const CIndex& ind1);

	/**
	 * The functions adds to `*this` array a pointer to newly created element indexed by `ind0,ind1,ind2`,
	 * \param[in] ind0,ind1,ind2 indices of new element.
	 * \return reference to the element indexed by `ind0,ind1,ind2`.
	 */
	Tell& add(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2);

	/**
	 * The functions adds to `*this` array a pointer to newly created element indexed by `ind0,ind1,ind2,ind3`,
	 * \param[in] ind0,ind1,ind2,ind3 indices of new element.
	 * \return reference to the element indexed by `ind0,ind1,ind2,ind3`.
	 */
	Tell& add(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2, const CIndex& ind3);

	/**
	 * This assign operator creates a new element to store the array represented by a given string.
	 * \param[in] str string representing an array.
	 * \return reference to `*this` array.
	 */
	CArray<Tell*>& operator=(const char* str);

	/**
	 * The operator initializes a given array with the array from a given input stream.
	 * \param[in] in input stream;
	 * \param[in,out] arr array object to be initialized.
	 * \return reference to `in`.
	 * \sa \ref mipshell_inputOutput.
	 */
	template<typename T> friend std::istream& operator>>(std::istream& in, CArray<T*> &arr);

	/**
	 * The operator prints to a given output stream the string representation of a given array.
	 * \param[in] out output stream;
	 * \param[in] arr array to be printed.
	 * \return reference to `out`.
	 * \sa \ref mipshell_inputOutput.
	 */
	template<typename T> friend std::ostream& operator<<(std::ostream& out, CArray<T*>& arr);

	/// \cond NEWandDELoperators
	void* operator new(std::size_t iSize);
	void operator delete(void* pPtr);
	void* operator new[](std::size_t iSize);
	void operator delete[](void* pPtr);

	void* operator new(std::size_t , void* pPtr)
		{return pPtr;}
	void operator delete(void* pPtr, void*) {}
	/// \endcond

// usefull functions
	Tell& max(); ///< \return pointer to an element of maximum value.
	Tell& min(); ///< \return pointer to an element of minimum value.
private:
	Tell& getZero(); ///< \return value of zero.
};

#endif //#ifndef __ARRAY__H
