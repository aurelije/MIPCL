///////////////////////////////////////////////////////////////
/**
 * \file Index.h interface for `CIndex` class
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

#ifndef __INDEX__H
#define __INDEX__H

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

#define MAX_INDEX_SIZE 63 ///< maximum index size (in characters).

#include <iostream>
#include <except.h>

/**
 * In __MIPshell__ an index is just a string.
 * A compound index is a string of other indices (sub-indices) separated by commas;
 * there may be at most four sub-indices.
 * \sa \ref mipshell_sets.
 */
class MIPSHELL_API CIndex
{
private:
	 char m_pStr[MAX_INDEX_SIZE+1];
public:
	CIndex(); ///< The default constructor.

	/// The copy constructor.
	/**
	 * \param[in] ind object to be copied.
	 */
	CIndex(const CIndex& ind);

	/**
	 * The constructor that creates a compound index from two sub-indices.
	 * \param[in] ind0 first sub-index;
	 * \param[in] ind1 second sub-index.
	 */
	CIndex(const CIndex& ind0, const CIndex& ind1);

	/**
	 * The constructor that creates a compound index from three sub-indices.
	 * \param[in] ind0 first sub-index;
	 * \param[in] ind1 second sub-index;
	 * \param[in] ind2 third sub-index.
	 */
	CIndex(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2);

	/**
	 * The constructor that creates a compound index from three sub-indices.
	 * \param[in] ind0 first sub-index;
	 * \param[in] ind1 second sub-index;
	 * \param[in] ind2 third sub-index;
	 * \param[in] ind3 fourth sub-index.
	 */
	CIndex(const CIndex& ind0, const CIndex& ind1, const CIndex& ind2, const CIndex& ind3);

	/**
	 * The constructor that creates an index translating an integer into the string.
	 * \param[in] ind integer-index.
	 */
	CIndex(int ind);


	/**
	 * The constructor that creates an index from a given string.
	 * \param[in] ind string-index.
	 */
	CIndex(const char* ind);

	virtual ~CIndex(); ///< The destructor.
/////////////////////////
// operations
////////////////////////
	const char* getString() const {return m_pStr;} ///< \return Pointer to the string representing the index.

	/**
	 * The procedure resets the value of the existing index to the value stored in the input string.
	 * \param[in] str string representing new index.
	 */
	void setString(const char* str);

	/**
	 * The procedure splits composite index into `r` single indices stored in the array `ind`.
	 * \param[in] k maximum number of sub-indices to be extracted;
	 * \param[out] ind array of sub-indices.
	 * \return number of sub-indices stored in `ind`.
	 */
	int split(int k, CIndex* ind); 

// RETURN VALUE: r

/////////////////////////
// operators
///////////////////////
	CIndex operator()(int k); ///< return the k-th sub-index; \param k position of sub-index to be returned.

	/**
	 * The operator compares `*this` index with another index `ind`.
	 * \param[in] ind index to compare with.
	 * \return `true` if two indices, `*this` and `ind`, are identical.
	 */
	bool operator==(CIndex& ind);

	/**
	 * The operator compares `*this` index with an integer-index `ind`.
	 * \param[in] ind integer-index to compare with.
	 * \return `true` if two indices, `*this` and `ind`, are identical.
	 */
	bool operator==(int ind);

	/**
	 * The operator compares `*this` index with a string-index `ind`.
	 * \param[in] ind string-index to compare with.
	 * \return `true` if two indices, `*this` and `ind`, are identical.
	 */
	bool operator==(const char* ind);

	/**
	 * The operator compares an integer-index `ind1` with another index `ind2`.
	 * \param[in] ind1 integer-index;
	 * \param[in] ind2 index.
	 * \return `true` if two indices, `*this` and `ind`, are identical.
	 */
	friend MIPSHELL_API bool operator==(int ind1, CIndex& ind2);

	/**
	 * The operator compares a string-index `ind1` with another index `ind2`.
	 * \param[in] ind1 string-index;
	 * \param[in] ind2 index.
	 * \return `true` if two indices, `ind1` and `ind2`, are identical.
	 */
	friend MIPSHELL_API bool operator==(const char* ind1, CIndex& ind2);

	/**
	 * \param[in] ind index;
	 * \return `true` if `ind` is a sub-index of `*this`.
	 */
	bool operator%(CIndex& ind);

	/**
	 * \param[in] ind1 integer-index;
	 * \param[in] ind2 index;
	 * \return `true` if `ind1` is a sub-index of `*this`.
	 */
	friend MIPSHELL_API bool operator%(int ind1, CIndex& ind2);


	/**
	 * \param[in] ind1 string-index;
	 * \param[in] ind2 index;
	 * \return `true` if `ind1` is a sub-index of `*this`.
	 */
	friend MIPSHELL_API bool operator%(const char* ind1, CIndex& ind2);

	/**
	 * The operator extends *this` index by appending to it another index `ind`.
	 * \param[in] ind index to be appended.
	 * \return `*this`.
	 */
	CIndex& operator+=(CIndex& ind);

	/**
	 * The operator creates a new index by concatenating `*this` index with another index `ind`.
	 * \param[in] ind index to be appended.
	 * \return reference to newly created index.
	 */
	CIndex& operator+(CIndex& ind);

	/**
	 * The operator creates a new index by concatenating a string-index `str` with another index `ind`.
	 * \param[in] str string-index to be extended;
	 * \param[in] ind index.
	 * \return reference to newly created index.
	 */
	friend CIndex& operator+(const char* str, CIndex& ind);

	/**
	 * The operator creates a new index by concatenating an integer-index `i` with another index `ind`.
	 * \param[in] i integer-index to be extended.
	 * \param[in] ind index.
	 * \return reference to newly created index.
	 */
	friend CIndex& operator+(const int i, CIndex& ind);

	/**
	 * The operator compares `*this` index with another index `ind`.
	 * \param[in] ind index to compare with.
	 * \return `true` if the string representation of `*this` index is lexicographically smaller
	 * than the string representation of index `ind`.
	 */
	bool operator<(const CIndex& ind);

	/**
	 * The operator compares two indices.
	 * \param[in] ind1,ind2 indices to be compared.
	 * \return `true` if the string representation of index `ind1` is lexicographically smaller
	 * than the string representation of index `ind2`.
	 */
	friend bool operator<(const CIndex& ind1, const CIndex& ind2);

	/**
	 * The operator compares two indices.
	 * \param[in] ind1,ind2 indices to be compared.
	 * \return `true` if two indices are identical.
	 * \remark This operator is needed to close all possible cases of comparing two indices.
	 * If there is no operator for comparing a pair of indices of particular types,
	 * appropriate constructors are used to convert those indices to `CIndex` objects,
	 *  and then created objects are compared.
	 */
	friend bool operator==(const CIndex& ind1, const CIndex& ind2);

	/**
	 * The operator initializes a given index with a string-index from a given input stream.
	 * \param[in] in input stream;
	 * \param[in,out] ind index to be initialized.
	 * \return reference to `in`.
	 * \sa \ref mipshell_inputOutput.
	 */
	friend MIPSHELL_API std::istream& operator>>(std::istream& in, CIndex& ind);

	/**
	 * The operator prints to a given output stream the string representation of a given index.
	 * \param[in] out output stream;
	 * \param[in] ind index to be printed.
	 * \return reference to `out`.
	 * \sa \ref mipshell_inputOutput.
	 */
	friend MIPSHELL_API std::ostream& operator<<(std::ostream& out, const CIndex &ind);

	/// @cond NEWandDELoperators
	////////////////////
// new and delete
/////////////////////////
	void* operator new(size_t iSize) throw(CMemoryException);
	void operator delete(void* pPtr) throw();
	void* operator new[](size_t iSize) throw(CMemoryException);
	void operator delete[](void* pPtr) throw();

	void* operator new(size_t , void* pPtr)
		{return pPtr;}

	void operator delete(void* pPtr, void*) {}
	/// @endcond
};
#endif // #ifndef __INDEX__H
