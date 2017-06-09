///////////////////////////////////////////////////////////////
/**
 * \file mipshell.h includes all the other __MIPshell__ headers
 * and provides a number of aliases for __MIPshell__ objects.
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

#ifndef __MIPSHELL__H
#define __MIPSHELL__H

#include "Problem.h"
#include <except.h>
#include "Var.h"
#include "Ctr.h"
#include "Vector.h"
#include "Function.h"
#include "DVar.h"
#include "VarArray.h"
#include "Array.h"
#include "Set.h"

///////////////////////////////////

typedef CIndex INDEX; ///< `INDEX` is an alias for `CIndex`.
typedef CVar VAR;  ///< `VAR` is an alias for `CVar`.
typedef CDvar DVAR; ///< `DVAR` is an alias for `CDvar`.

typedef CVector<int> INT_VECTOR; ///< `INT_VECTOR` is an alias for `CVector<int>`.
typedef CVector<double> REAL_VECTOR; ///< `REAL_VECTOR` is an alias for `CVector<double>`.

typedef CRange RANGE; ///< `RANGE` is an alias for `CRange`.

typedef CSet<int> INT_SET; ///< `INT_SET` is an alias for `CSet<int>`.
typedef CSet<double> REAL_SET; ///< `REAL_SET` is an alias for CSet<double>.
typedef CSet<CIndex> INDEX_SET; ///< `INDEX_SET` is an alias for `CSet<CIndex>`.

typedef CArray<int> INT_ARRAY; ///< `INT_ARRAY` is an alias for `CArray<int>`.
typedef CArray<double> REAL_ARRAY; ///< `REAL_ARRAY` is an alias for `CArray<double>`.
typedef CArray<CIndex> INDEX_ARRAY; ///< `INDEX_ARRAY` is an alias for `CArray<CIndex>`.

typedef CArray<CSet<int>* > INT_SET_ARRAY; ///< `INT_SET_ARRAY` is an alias for `CArray<CSet<int>* >`.
typedef CArray<CSet<double>* > REAL_SET_ARRAY; ///< `REAL_SET_ARRAY` is an alias for `CArray<CSet<double>* >`.
typedef CArray<CSet<CIndex>* > INDEX_SET_ARRAY; ///< `INDEX_SET_ARRAY` is an alias for `CArray<CSet<CIndex>* >`.

typedef CVarVector VAR_VECTOR; ///< `VAR_VECTOR` is an alias for `CVarVector`.
typedef CVarArray VAR_ARRAY; ///< `VAR_ARRAY` is an alias for `CVarArray`.

#endif // #ifndef __MIPSHELL__H
