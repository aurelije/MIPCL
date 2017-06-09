///////////////////////////////////////////////////////////////
/**
 * \file Vector.h interface for `CVector` class
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

#ifndef __VECTOR__H
#define __VECTOR__H

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

#ifndef MAX_ARR_DIM
#define MAX_ARR_DIM 4 ///< maximum dimension of any array.
#endif

#include <iostream>
#include <except.h>

/**
 * In __MIPshell__ a vector is multidimensional array which indices are integers starting from `0`.
 * \sa \ref mipshell_vectors.
 */
template <typename Tell>
class MIPSHELL_API CVector
{
protected:
	int m_iDim; ///< dimension, `1 <= m_iDim <= MAX_VARARR_SIZE`
	int m_ipSize[MAX_ARR_DIM]; ///< `m_ipSize[i]` size of dimension `i`.
	Tell* m_pVal; ///< values of vector arrays.
public:
	CVector(); ///< The default constructor.

	/**
	 * The constructor creates an empty array which dimension is the number
	 * of its arguments with positive value. For example, the declaration
	 *  \code
	 *    CVector<double> a(3,2);
	 *  \endcode
	 *  creates a real 2-dimension vector of size `2x3`.
	 *  \param[in] size0,size1,size2,size3 sizes of each of four possible dimensions; zero sizes may be skiped.
	 */
	CVector(int size0, int size1=0, int size2=0, int size3=0);

	virtual ~CVector(); ///< The destructor.
// operations
	/**
	 * The access operator returns the value of vector element with a given index.
	 * \param[in] ind index of vector element.
	 * \return reference to the vector element indexed by `ind`.
	 */
	Tell& operator[](int ind)
		{return m_pVal[ind];}

	/**
	 * The access operator returns the value of vector element with a given index.
	 * \param[in] ind index of vector element.
	 * \return reference to the vector element indexed `ind`.
	 */
	Tell& operator()(int ind)
		{return m_pVal[ind];}

	/**
	 * The access operator returns the value of vector element with given indices.
	 * \param[in] ind0,ind1 indices of vector element.
	 * \return reference to the vector element indexed by `ind0,ind1`.
	 */
	Tell& operator()(int ind0, int ind1)
		{return m_pVal[ind0*m_ipSize[1]+ind1];}

	/**
	 * The access operator returns the value of vector element with given indices.
	 * \param[in] ind0,ind1,ind2 indices of vector element.
	 * \return reference to the vector element indexed by `ind0,ind1,ind2`.
	 */
	Tell& operator()(int ind0, int ind1, int ind2)
		{return m_pVal[(ind0*m_ipSize[1]+ind1)*m_ipSize[2]+ind2];}

	/**
	 * The access operator returns the value of vector element with given indices.
	 * \param[in] ind0,ind1,ind2,ind3 indices of vector element.
	 * \return reference to the vector element indexed by `ind0,ind1,ind2,ind3`.
	 */
	Tell& operator()(int ind0, int ind1, int ind2, int ind3)
		{return m_pVal[((ind0*m_ipSize[1]+ind1)*m_ipSize[2]+ind2)*m_ipSize[3]+ind3];}

	/**
	 * The operator initializes a given vector from a given input stream.
	 * \param[in] in input stream;
	 * \param[in,out] v vector to be initialized.
	 * \return reference to `in`.
	 * \sa \ref mipshell_inputOutput.
	 */
	template<class T> friend std::istream& operator>>(std::istream& in, CVector<T>& v);

	/**
	 * The operator prints a given vector to a given output stream.
	 * \param[in] out output stream;
	 * \param[in] v vector.
	 * \return reference to `out`.
	 * \sa \ref mipshell_inputOutput.
	 */
	template<class T> friend std::ostream& operator<<(std::ostream& out, CVector<T>& v);

	/// \cond NEWandDELoperators
	void* operator new(size_t iSize) throw(CMemoryException);
	void operator delete(void* pPtr) throw();
	void* operator new[](size_t iSize) throw(CMemoryException);
	void operator delete[](void* pPtr) throw();
	/// \endcond

// attributes
	int getDim()
		{return m_iDim;} ///< \return dimension of `*this` vector.

	/**
	 * The procedure resets the dimension of `*this` vector, and, if necessary, reallocates memory.
	 *  \param[in] size0,size1,size2,size3 sizes of each of four possible dimensions; zero sizes may be skiped.
	 */
	void setDim(int size0, int size1=0, int size2=0, int size3=0);
	
	/**
	 * \param[in] dim one of vector dimensions ore `-1`.
	 * \return the size of dimension `dim` if `0 <= dim < getDim()`, or the total number of elements in the vector if `dim=-1`.
	 */
	int getSize(int dim=-1);

	const Tell* getValArray()
		{return m_pVal;} ///< \return pointer to the array of the values of vector elements.
	
	Tell min(); ///< \return vector element of minimum value.
	Tell max(); ///< \return vector element of maximum value.
	Tell Sum(); ///< \return the sum of all vector elements.
	Tell absSum(); ///< \return the sum of the absolute values of all vector elements.
private:
	/**
	 * The procedure reallocates memory for storing the values of vector elements.
	 * \param[in] current size (number of elements);
	 * \param[in] newSize new size.
	 */
	void reallocMem(int size, int newSize);
};

#endif // #ifndef __VECTOR__H
