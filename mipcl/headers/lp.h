///////////////////////////////////////////////////////////////
/**
 * \file lp.h Interface for `CLP` class
 *
 * |  __Author__  | N.N. Pisaruk                              |
 * |-------------:|:------------------------------------------|
 * |  __e-mail__  | nicolaipisaruk@gmail.com                  |
 * | __home page__| wwww.mipcl-cpp.appspot.com                |
 *
 *   \copyright __2015 Nicolai N. Pisaruk__
 */

 /*  This file is part of the Mixed Integer Class Library (MIPCL).
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
#ifndef __LP__H
#define __LP__H

#ifdef _WINDOWS
#ifndef MIP_API
#ifdef MIP_EXPORTS
#define MIP_API __declspec(dllexport)
#else
#define MIP_API __declspec(dllimport)
#endif
#endif
typedef __int64 __LONG; ///< For Visual C++, `long` means 32 bit integer; so we redefine it.
#else
#ifndef MIP_API
/// WINDOWS specifics.
/**
 * All files within this DLL are compiled with the MIP_EXPORTS
 * symbol defined on the command line. This symbol should not be defined on any project
 * that uses this DLL. This way any other project whose source files include this file see
 * MIP_API functions as being imported from a DLL, whereas this DLL sees symbols
 * defined with this macro as being exported.
 */
#define MIP_API
#endif
typedef long __LONG; ///< For GNU C++, `long` means 64 bin integer.
#endif

#include <fstream>
#include "thread.h"

class CException;
class CBinHeap;
class CPrepStack;
class CLU;

/// This class has been designed for solving Linear Programs (LPs)
/**
 * \f{align*}{
 *     & c^Tx \to \max,\\
 *     & b_1 \le Ax \le b_2,\\
 *     & l \le  x \le u,
 * \f}
 * by both, prime and dual, simplex methods.
 */
class MIP_API CLP 
{
    friend class CLU;
    friend class CMIP;
public:
    /**
     * If preprocessing is switched on (default option)
     * or columns are generated, the solver do not preserve the order
     * in which the variables were added to the matrix.
     * The handles are primarily used for mapping the variables in the solver memory onto user's variables.
     */
    typedef int tagHANDLE; ///< program type for handles of variables and constraints

    /// Matrix alignment.
    /**
     * The non-zero matrix entries may be listed either _column-wise_ or _row-wise_.
     * It is also possible that the entries are not ordered at all.
     * Furthermore, even if the non-zero entries are ordered before the solution procedure starts,
     * after adding to the matrix new rows and/or columns, the previous alignment will be lost.
     * In the latter case, we say that the matrix is _approximately aligned_ either column-wise or row-wise.
     */
    enum enAlign {
        ALIGN_NONE        = 0x00000000, ///< no alignment
        ALIGN_COLUMN_APPR = 0x00000001, ///< mainly by columns
        ALIGN_COLUMN      = 0x00000011, ///< by columns
        ALIGN_ROW_APPR    = 0x00000100, ///< mainly by rows
        ALIGN_ROW         = 0x00001100  ///< by rows
    };

	/// LP types of constraints.
	/**
	 * The type of a constraint is the bitwise OR of the members of `enCtrType`.
	 */
    enum enCtrType {
        CTR_ATTACHED     = 0x00000001, ///< constrains of this type cannot be removed from matrix
        CTR_LEFT         = 0x00000002, ///< constraints is _left bounded_ (its left hand side is finite)
        CTR_RIGHT        = 0x00000004, ///< constraints is _right bounded_ (its right hand side is finite)
        CTR_EQ           = 0x00000008, ///< constraint is equation
        CTR_REF          = 0x00000010 ///< flag is _privately_ used in `CLP`
    };

	/// LP types of variables.
	/**
	 * The type of a variable is the bitwise OR of the members of `enVarType`.
	 */
    enum enVarType {
        VAR_LEFT        = 0x10000000,  ///< variable is _lower bounded_ (its lower bound is finite)
        VAR_RIGHT       = 0x20000000,  ///< variable is _upper bounded_ (its upper bound is finite)
        VAR_FX          = 0x40000000,  ///< variable is _fixed_
        VAR_REF         = 0x80000000,  ///< is _privately_ used in `CLP`
        VAR_LB_IMPL     = 0x01000000,  ///< is _privately_  used in `CLP`
        VAR_UB_IMPL     = 0x02000000,  ///< is _privately_  used in `CLP`
		VAR_INT         = 0x00001000, ///< integral variable, meaningless in `CLP`.
   };

    /// This is the list of all possible ways to scale matrix.
    enum enScaling {
    	/**
    	 * A row (resp., column) scaling factor is the power of 2 that is closest to
    	 *  the average value of the minimum and maximum absolute values of coefficients in that row (resp., column).
    	 *  The min-max scaling procedure 10 times, in turn, first computes row scaling factors and multiplies all rows by their factors,
    	 *  and then computes column scaling factors  and multiplies all the columns by their factors.
    	 */
        SCL_MINMAX, ///< Min-max scaling.
        /**
         * The _ideal scaling_ procedure applies a complex algorithm to compute scaling factors such that, after scaling,
         *  the difference between the maximum and minimum of the absolute values of matrix coefficients is minimal.
         */
        SCL_IDEAL, ///< Ideal scaling.
        SCL_NO, ///< Switches off scaling.
        /**
         * A row (resp., column) scaling factor is the power of 2 that is closest to
         *  the geometric mean value of the absolute values of coefficients in that row (resp., column).
         *  The _geometric-mean scaling_ procedure 10 times, in turn, first computes column scaling factors and multiplies all columns by their factors,
         *  and then computes rows scaling factors  and multiplies all rows by their factors.
         */
        SCL_GM_ROWS, ///< Geometric-mean scaling, rows are scaled after columns.
        SCL_GM_COLUMNS, ///< Geometric-mean scaling, rows are scaled before columns (for details, see `SCL_GM_ROWS`).
        /**
         * A row (resp., column) scaling factor is the power of 2 that is closest to
         *  the maximum absolute values of coefficients in that row (resp., column).
         *  The _ max-value scaling_ procedure 10 times, in turn, first computes column scaling factors and multiplies all columns by their factors,
         *  and then computes rows scaling factors  and multiplies all rows by their factors.
         *
         */
        SCL_MAX_ROWS, ///< Max-value scaling, rows are scaled after columns.
        SCL_MAX_COLUMNS, ///< Max-value scaling, rows are scaled before columns (for details, see `SCL_MAX_ROWS`).
        NOT_SCALED=127 ///< When adding to the matrix a new constraint (or column), the value of scaling factor set to `NOT_SCALED` means that this constraint (or column) is to be scaled by the solver.
    };

    /// LP methods.
    enum enLPmethod {
        AUTO_DETECT, ///< it is left to the solver to decide what method to use for solving LPs,
        PRIME_SIMPLEX, ///< prime simplex is used for solving LPs,
        DUAL_SIMPLEX ///< dual simplex is used for solving LPs.
    };

    ///Separation rules.
    enum enSepRule {
        SEP_MOST_VIOLATED,  ///< most violated constraint is chosen
        SEP_ONLY_EQUATIONS, ///< violated constraint is chosen only among equations
        SEP_PARTIAL, ///< partial separation, i.e., most violated constraint is chosen from small predefined set of constraints
        SEP_STEEPEST_EDGE ///< steepest edge rule, i.e.,
        ///< violation of each constraint is divided by the norm of corresponding row,
        ///< and then constraint with minimum ratio is chosen
    };

    /// Pricing rules.
    enum enPricingRule {
        PRC_MOST_NEGATIVE, ///< variable with most negative reduced cost is chosen
        PRC_PARTIAL, ///< variable with most negative reduced cost is chosen from small predefined set of variables
        PRC_STEEPEST_EDGE ///< reduced cost of each variable is divided by the norm of corresponding column, and then variable with minimum ratio is chosen
    };

    /// Problem states.
    /**
     * Any current state of the problem being solved is composed as the bitwise OR of the members of `enProbState`.
     */
    enum enProbState {
        PROB_IN_MEMORY    = 0x00000001, ///< memory necessary to solve problem has been allocated
        PROB_PREPARED     = 0x00000002, ///< ready (memory allocated, matrix preprocessed and scaled) for optimizing
        PROB_SOLVED       = 0x00000004, ///< problem has been solved
        PROB_SOLUTION     = 0x00000008, ///< solution found
        PROB_OPTIMAL      = 0x00000010, ///< optimality proven
        PROB_INFEASIBLE   = 0x00000020, ///< inconsistency proven
        PROB_TIME_LIMIT   = 0x00000040, ///< time limit exceeded
        PROB_GAP_LIMIT    = 0x00000080, ///< required integrality gap attained
        PROB_SOLVER_FLAGS = PROB_SOLVED|PROB_SOLUTION|PROB_OPTIMAL|PROB_INFEASIBLE|PROB_TIME_LIMIT|PROB_GAP_LIMIT ///< bitwise OR of all `enProbState` masks
    };

    /**
     * A bitwise ORing of the members of `enRowColGenRule1` stored in `m_uRowColGenRule` determines whether rows and/or columns are generated.
     */
    enum enRowColGenRule1 {
    	ROW_GEN     	= 0x1, ///< rows/cuts can be generated by overloading `separate()`, `genCut1()`, or cut generating procedures implemented in `CMIP`
    	COL_GEN     	=0x2,  ///< columns can be generated by overloading `generateColumns()`
    	USER_SEP_PROC	=0x4 ///< if `separate()` has been overloaded
    };

// Constants
    static const double INF; ///< default value of infinity.
    static const double VAR_INF; ///< default value of infinity for variables.
    static const double MAX_BIG_M; ///< default value for big `M` used  in the one phase implementation of the prime simplex algorithm.
    static const int SHIFT; ///< equals to (std::numeric_limits<int>::max() >> 1).
    static const char msgNoSolution[]; ///< constant string for no-solution message.

    /**
     * \warning Each message must contain no more than `255` characters.
     */
    char m_sWarningMsg[256]; ///< character array for using as message strings.

protected:
	int m_iThread; ///< index of a thread in a multi-threaded MIP-application.
    char m_strProblemName[32]; ///< stores problem name.

    std::ostream* m_pLogStream; ///< LOG file.

//////  start M A T R I X
    bool m_bSense; ///< `true` - maximize, `false` - minimize.
    int m_iN; ///< number of variables in the currently processed LP.
    int m_iN0; ///< number of variables in the root LP; if columns are generated, `m_iN0 <= m_iN`.

    /*
     *  If an extra variable (above this limit) is added, memory for the matrix is reallocated.
     */
    int m_iNmax; ///< maximum number of variables.
    int m_iM;  ///< number of constraints in the currently processed LP.
    int m_iM0; ///< number of constraints in the root LP; if rows (cuts) are generated, `m_iM0 <= m_iM`.

    /**
     *  If an extra constraint (above this limit) is added, memory for the matrix is reallocated.
     */
    int m_iMmax; ///< maximum  number of constraints.
    int m_iNZ;  ///< number of nonzero entries in the currently processed matrix.
    int m_iNZ0; ///< number of nonzero entries in the root LP matrix.

    /**
     *  If an extra entry (above this limit) is added, memory for the matrix is reallocated.
     */
    int m_iNZmax; ///< maximum number of entries.

    double* m_dpC; ///< cost vector, array of size `m_iNmax`.

    /**
     * Cost vector `m_dpC` is multiplied by 2^{m_iObjScaleExp}.
     */
    int m_iObjScaleExp; ///< objective scaling factor.

    /**
     * Array of size `2*m_iMmax`, `m_dpB[i<<1]` and `m_dpB[(i<<)+1]` are left and right hand sides of constraint `i`.
     */
    double* m_dpB; ///< left and right hand sides of constraints.

    /*
     *  Array of size `2*m_iNmax`, `m_dpD[j<<1]` and `m_dpD[(j<<1)+1]` are lower and upper  bound of variable `j`.
     */
    double* m_dpD; ///< lower and upper bounds of variables.

private:
    char* m_cpMatrix; ///< pointer to the memory allocated for the matrix.
protected:
    int* m_ipPrevColEntry; ///< array of size `m_iNZmax`, `m_ipPrevColEntry[j]` is the entry in column `m_ipCol[j]` preceding entry `j`.
    int* m_ipPrevRowEntry; ///< array of size `m_iNZmax`, `m_ipPrevRowEntry[j]` is the entry in row `m_ipRow[j]` preceding entry `j`.
    int* m_ipRow; ///<  array of size `m_iNZmax`, `m_ipRow[j]` is row index of entry `j`.
    int* m_ipCol; ///<  array of size `m_iNZmax`, `m_ipCol[j]` is column index of entry `j`.
    double* m_dpVal; ///< array of size `m_iNZmax`, `m_dpVal[j]` is coefficient for entry `j`.
// Row lists
    int* m_ipLastRowEntry; ///< array of size `m_iMmax`, `m_ipLastRowEntry[i]` starts the linked list of entries in row `i`.
    int* m_ipRowSize; ///< array of size `m_iMmax`, `m_ipRowSize[i]` is number of entries in row `i`.
// Column Lists
    int* m_ipLastColEntry; ///< array of size `m_iNmax`, `m_ipLastColEntry[i]` starts the linked list of entries in column `i`.
    int* m_ipColSize; ///< array of size `m_iNZmax`, `m_ipColSize[i]` is number of entries in column `i`.

    /**
     * Array of size `m_iMmax`, `s=m_cpRowScale[i]` is a scale factor of row `i`, i.e., row `i` is multiplied by 2^s.
     */
    signed char* m_cpRowScale; ///< row factors.

    /**
     *  Array of size `m_iNmax`, `s=m_cpColScale[j]` is a scale factor of column `j`, i.e., column `j` is multiplied by 2^s.
     */
    signed char* m_cpColScale; ///< column factors.

    enAlign m_iAlign; ///< specifies how matrix entries are listed.

    /**
     *  Array of size `m_iMmax`,
     * `m_ipRowHd[i]` is _handle_ of row (or constraint) `i`,
     *     - if `m_ipRowHd[i] < 0`, constraint `i` is also stored in the pool;
     *     - if `m_ipRowHd[i] >= 0`, it is up to the user to assign a value to the handle, say,
     *       this may be an index specifying the location of a data structure describing constraint `i`.
     *
     * \attention each row handle must be unique.
     */
    tagHANDLE* m_ipRowHd; ///< row handles.

    /**
     *  Array of size `m_iNmax`,
     * `m_ipColHd[j]` is _handle_ of column (or variable) `j`,
     *     - if `m_ipColHd[j] < 0`, column `i` is also stored in the pool;
     *     - if `m_ipColHd[j] >= 0`, it is up to the user to assign a value to the handle, say,
     *       this may be an index specifying the location of a data structure describing column `j`.
     *
     * \attention each column handle must be unique.
     */
    tagHANDLE* m_ipColHd; ///< column handles.

    /**
     *  Array of size `m_iNmax`, `m_ipVarType[j]` is a type of variable `j`,
     *  which is the bitwise OR of the members of two enumerations CLP::enVarType and CMIP::enVarType.
     */
    unsigned int* m_ipVarType;  ///< types of variables.

    /**
     * Array of size `m_iMmax`, `m_ipCtrType[i]` is a type of constraint `i`,
   	 * which is the bitwise OR of the members of two enumerations `CLP::enCtrType` and `CMIP::enCtrType`.
     */
    unsigned int* m_ipCtrType; ///< constraint types.

//////  end M A T R I X

    double m_dZeroEntry; ///< all numbers that are less than `m_dZeroEntry` are treated as zeroes.
    double m_dVarInf; ///< `-m_dVarInf <= x[j] <= m_dVarInf`, default value is `CLP::VAR_INF=1.0e+12`.
    double m_dCtrTol; ///< maximum violation allowed for constraints.
    double m_dVarTol; ///< maximum violation allowed for variables.
    double m_dShadowPriceTol; ///< maximum violation allowed for shadow prices (dual variables).
    double m_dRedCostTol; ///< maximum violation allowed for reduced costs.

private:
    double m_dPrimeRatioTol;  ///< tolerance value used when carrying out dual ratio test.
    double m_dDualRatioTol; ///< tolerance value used when carrying out dual ratio test.
    double m_dTolPiv; ///< minimum value for pivots.
    double m_dRelTolPiv;
    double m_dGoodPiv; ///< minimum value for "good" pivots.
    double m_dTolPivErr;
    double m_dRelTolPivErr;
    double m_dTolPrimeDeg;
    double m_dTolDualDeg;
    double m_dBigM; ///< maximum value for big Ms.

    /**
     * If a bound for a variable is violated by more than `m_dVarViolatThreshold`,
     * the separation procedure will not verify any non-trivial inequalities.
     */
    double m_dVarViolatThreshold;

    /**
     * If a dual variable is less than `-m_dShadowPriceViolatThreshold`,
     * the corresponding column is chosen as a pivot column;
 	 * the latter is not true for the steepest edge strategy.
     */
    double m_dShadowPriceViolatThreshold;

    // Memory for the matrix
    char *m_cpColMatrMem, *m_cpRowMatrMem;

    char *m_cpColSimplexMem, *m_cpRowSimplexMem;

protected:
    double m_dFixedCost; ///< total cost of all variables fixed during preprocessing.
    double m_dObjFactor; ///< original objective was divided by m_dObjFactor;
    double m_dBasicVarCost; ///< total cost of basic variables.
    double m_dObjVal; ///< current value of the objective
    double* m_dpX; ///< prime solution; array of size `m_iMmax.
    double* m_dpPi; ///< dual solution (potentials), array of size `m_iNmax`.

    /**
     * `m_iState` is the bitwise OR of the members of the enumeration `enProbState`.
     */
    int m_iState; ///< state of the problem.

    /**
     * Problem is
     *  - infeasible if m_iS != 0 && m_bDualFeasible,
	 *  - unbounded if m_iS >= 0 && m_bPrimeFeasible
     */
    int m_iS; ///< defines a state of the last LP solved.

// Below auxiliary arrays extensively used as temporal memory in different ways
    double *m_dpUd; ///< auxiliary array of size `m_iNmax`.
    double *m_dpUb; ///< auxiliary array of size `m_iMmax`.
    int* m_ipArray; ///< auxiliary array of size max{`m_iNmax,m_iMmax`}
    double* m_dpArray; ///< auxiliary array of size `m_iMmax+m_iNmax`
    double* m_dpW; ///< auxiliary array of size `2*m_iMmax`

    /**
     * Array of size max{`m_iNmax,m_iMmax+m_iMaxLuUpdateNum`},
     *  may be freely used in user cut and column generating procedures.
     */
    double *m_dpNorm; ///< row or column norms in the current basis.

///////////////////////////////////////////////////////////////
// F L A G S
/////////////
public:
    bool m_bPrimeFeasible; ///< if `true`, the current basic solution is prime feasible.
    bool m_bDualFeasible; ///< if `true`, the current basic solution is dual feasible.
    bool m_bRowNorm;  ///< if `true`, `m_dpNorm[i]` is the norm of `m_ipBasicRow[i]` in the current basis
    bool m_bColNorm; ///< if `true`, `m_dpNorm[i]` is the norm of `i`-th non-basic row.
private:
    enScaling m_eScaling; ///< scaling strategy.

    /**
     * If `m_iInfoMsgFlag & 0x1`, run time CLP-messages are blocked;
     * if `m_iInfoMsgFlag & 0x2`, run time branch-and-cut CMIP-messages are blocked;
     * if `m_iInfoMsgFlag & 0x4`, cut-statistics CMIP-messages are blocked;
     * if `m_iInfoMsgFlag & 0x10`, all run time messages are blocked.
     *
     * \sa `setInfoMsgState()` and `beSilent()`.
     */
    int m_iInfoMsgFlag;

    int m_iRowNormNum; ///< row norms are computed for all rows plus for those columns that are in current row basis.
    int m_iColNormNum;

    /**
     * This constant is used when implementing the steepest edge pricing and separating rules.
     * If the recomputed norm of a pivot row or column exceeds its actual value
     * by more than `BAD_NORM_FACTOR` times, all row or column norms are recomputed.
     */
    static const double BAD_NORM_FACTOR;

    /**
     *memory to store the objective and the  row and variable bounds
     *before applying perturbations to prevent cycling
     */
    char *m_cpPrtbMem; ///< only a pointer, memory is allocated in case of necessity.
    double m_dPrtbObjDec; ///< local variable, it is initialized to 0 in dualSimplex.

    /**
     * If an approximate norm (square of the norm) is BAD_NORM_FACTOR times bigger or smaller of
 	 * the exact norm, m_iBadNormNum is incremented
 	 * if `m_iBadNormNum > MAX_BAD_NORM_NUM`, all norms are recomputed.
     */
    int m_iBadNormNum; ///< Number of error encountered among the norms.

    /**
     * If m_iNormUpdatesNum > MAX_UPDATE_NORM_NUM, switch to Dancig rule.
     */
    int m_iNormUpdatesNum; ///< number of norm updates.
//
    double m_dPivot; ///< value of pivot element when pivot operation is performed.
    double m_dObjInc; ///< increase of prime or dual objective.
    double m_dPrimalStep; ///< step value when prime solution is updated.
    double m_dDualStep; ///< step value when dual solution is updated.

//////////////////////////////////////////////////////////////
// BASIS
    int m_iNoOfTrivial; ///< in current basis, there are `m_iNoOfTrivial` trivial (representing bounds for variables) rows among basic rows.

    int m_ipBasicRowSize; ///< total number of non-zeroes in basic rows.
    int m_ipBasicColSize; ///< total number of non-zeroes in basic columns.
protected:
    int m_iBasisSize; ///< number of nontrivial (b1 <= Ax <= b2) inequalities in the basis.

    /**
     * If `(r=m_ipBasicRow[i]) >= 0`, row `r` is the `i`-th row in the basis;
     * if `(r=m_ipBasis[i]) < 0`, row e_{-r-1} is the `i`-th row in the basis.
     */
    int *m_ipBasicRow;  ///< list of basic rows.

    /**
     * If `(k=m_ipRowMap[i]) < SHIFT`,  then `m_ipBasicRow[abs(k)-1]=i` and the value of the inequality is equal to its
     *   - left-hand-side if `k < 0`;
     *   - right-hand-side if `k > 0`.
     *
     *   If `k >= SHIFT`, then `m_ipBasicRow[k-SHIFT]=i`.
     *  \remark The pair (`m_ipRowMap`,`m_ipColMap`) is called a _short basis_.
     * \sa m_ipBasicRow.
     */
    int *m_ipRowMap;

    int *m_ipBasicColumn; ///< list of basic columns.

    /**
     * If `(k=m_ipColMap[j]) < SHIFT`,  then `m_ipBasicColumn[abs(k)-1]=j` and the value of the value of variable is equal to its
     *   - lower bound if `k < 0`;
     *   - upper bound if `k > 0`.
     *m_dRelTolPiv
     *   If `k >= SHIFT`, then `m_ipBasicColumn[k-SHIFT]=j`.
     *  \remark The pair (`m_ipRowMap`,`m_ipColMap`) is called a _short basis_.
     * \sa `m_ipBasicColumn`.
     */
    int *m_ipColMap;

    double *m_dpBeta; ///< values of non-basic rows, the value of row `i` is A_ix.

    enLPmethod m_eDefLPmethod; ///< LP method to solve the root LP.
    enLPmethod m_eLPmethod; ///< currently used LP method.

    // Parameters
    unsigned m_uRowColGenRule; ///< bitwise OR of the members of `enRowColGenRule`; by default, all flags are set to `1`.

    enSepRule m_eSepRule; ///< currently used separation rule.
    enPricingRule m_ePricingRule; ///< currently used pricing rule.
    enSepRule m_eLpSepRule; ///< separation rule used for solving pure LPs as well as root LPs when solving MIPs.
    enPricingRule m_eLpPricingRule; ///< pricing rule used for solving pure LPs as well as root LPs when solving MIPs.

// Partial pricing
    int m_iVarNum; ///< number of variables in the current LP when applying partial pricing.
    int m_iCtrNum; ///< number of constraints in the current LP when applying partial pricing.
private:
    bool m_bUpdateCandSet; ///< if `true`, the set of candidates inequalities (variables) for choosing them in separating (pricing).
    int m_iCandSetUpdateNum; ///< number of updates of the candidate set.
    int m_iMaxCandNum; ///< number of candidates (rows or columns) to enter the basis.
    CBinHeap *m_hpCand; ///< binary heap of candidates  to enter the basis.

protected:
    bool m_bPreproc; ///< if `true` preprocessing is on; default value is `true`.
    CPrepStack* m_pPrepStack; ///< pointer to the preprocessing stack.

    /**
     * Sometimes we need to solve an LP that is a slightly modified version of  previously another LP.
     * If those modification does not affect the basic submatrix and the objective,
     * one can apply the dual simplex algorithm to re-optimizing the LP starting from an optimal basis of the original LP.
     * But if the bounds of some non-basic variables were modified, we need to update an initial basic solution as well.
     */
    int m_iUpdatePrimeSol; ///< number of modified bounds of non-basic variables.

// Factoring the basis
    CLU* m_pLU; ///< pointer to an object of class CLU, that solves linear systems.
private:
    int m_iMaxLuUpdateNum; ///< maximal number of multiplicators allowed

// STATISTICS
    int m_iItNum; ///< number of iterates performed by `primeSimplex()` or `dualSimplex()`.
    int m_iDegItNum; ///< number of degenerate  iterates performed by `primeSimplex()` or `dualSimplex()`.
    int m_iLpItToInform; ///< number of LP iterations to print an info message
    int m_iPrtbNum; ///< number of objective(dual)/bounds(primal) perturbations
    int m_iPartitionNum; ///< number of times the basis was refactored during a call to `primeSimplex()` or `dualSimplex()`.

protected:
    __LONG m_lStartTime; ///< start time of solving problem.
    __LONG m_lSolTime; ///< solution time.

//////////////////////////////////////////////////////////////////////////
//                        Functions of general use                      //
//////////////////////////////////////////////////////////////////////////
private:
    /**
     * This function is called by the constructors to initialize all
     * the tolerance parameters; usually there is no reason to call it once again.
     */
    void setToleranceVars();

    void closeLogStream(); ///< closes LOG-file.

public:
    /**
     * The procedure opens a log file, which name is the value of `name` appended by `_mipcl.log`.
     *
     * \param[in] name log file name; if `name=0`, `m_strProblemName` is used as log file name.
     */
    void openLogStream(const char* name=0);

    /**
     * If the lower bound of a variable is less than `-val`, then the variable is assumed not bounded from below.
     * Similarly, if the upper bound of a variable is greater than `val`, then the variable is assumed not bounded from above.
     * \param[in] inf new value of "infinity" for variables.
     * \sa getVarInf().
     */
    void setVarInf(double inf)
        {m_dVarInf=inf;}

    /**
     * \return currently used value of "infinity" for variables.
     * \sa setVarInf().
     */
    double getVarInf() const
        {return m_dVarInf;}

    /**
     * __MIPCL__ writes to the LOG file, which name is the concatenation of the problem name and the extension ".log"
     * quite a lot of information describing all phases of the solution process. After having been analyzed the contents
     * of the LOG file, the user may adjust the parameters of the solver to improve its efficiency.
     * The user can also write to this LOG file any messages by calling `writeStrToLogStream()`.
     * \param[in] str message string.
     */
    void writeStrToLogStream(const char* str);


///////////////////////////////////////////////////////////////////////////////
    /**
     * Sets problem name.
     * \param[in] name problem name.
     * \remark The name is truncated to have at most 31 symbol.
     */
    void setProblemName(const char* name);

    /**
     * \param[out] name problem name.
     * \attention allocate for `name` at least 32 bytes.
     */
    void getProblemName(char* name) const;

//    std::ostream* getLogStreamPtr()
//        {return m_pLogStream;}
//    void setLogStreamPtr(std::ostream* pLogStream)
//        {m_pLogStream=pLogStream;}

/////////////////////////////////////
// Getting and setting tolerance variables
    /**
   	 * All numeric values that are less than `m_dZeroEntry` are considered as zeroes.
   	 * \param[in] zero new value of "zero".
   	 * \remark The default value is  1.0e-13.
   */
    void setZero(double zero)
        {m_dZeroEntry=zero;}

    /**
     * \return currently used value of "zero".
     */
    double getZero() const
        {return m_dZeroEntry;}

    /**
     * The absolute value of any pivot element must always be greater than `m_dTolPiv` (an internal MICL parameter),
     * and it also must be greater than either `m_dGoodPiv` or `m_dRelTolPiv * gamma`,
     * where gamma is the maximum absolute value of a nonzero entry in the pivot column
     * for the prime simplex method, or in the pivot row for the dual simplex method.
     * The other two parameters, `m_dTolPivErr` and `m_dRelTolPivErr`
     * are used to estimate absolute and relative errors when computing pivot elements.
     *
     * Default values:
     *     - 1.0e-9 for `m_dTolPiv`,
     *     - 1.0e-3 for `m_dGoodPiv`,
     *     - 1.0e-10 for `m_dTolPivErr`
     *     - 1.0e-7 for `m_dRelTolPiv` and `m_dRelTolPivErr`.
     *
     * \param[in] tolPiv new value for `m_dTolPiv`;
     * \param[in] relTolPiv new value for `m_dRelTolPiv`;
     * \param[in] goodPiv new value for `m_dGoodPiv`;
     * \param[in] tolPivErr new value for `m_dTolPivErr`;
     * \param[in] relTolPivErr new value for `m_dRelTolPivErr`;
     * \attention All these tolerance parameters are interdependent; therefore, tuning them is a subtle task.
     * \sa getPivTol(), getGoodPiv(), getRelPivTol(), getPivErrTol(), getRelPivErrTol()
     */
    void setPivTol(double tolPiv, double relTolPiv, double goodPiv,
                   double tolPivErr, double relTolPivErr);

    /**
     * \return minimum absolute value of pivots.
     * \sa setPivTol().
     */
    double getPivTol() const
        {return m_dTolPiv;}

    /**
     * \return minimum absolute value of pivots that are considered as "good".
     * \sa setPivTol().
     */
   double getGoodPiv() const
        {return m_dGoodPiv;}

   /**
    * \return minimum relative pivot value in pivot row and column.
     * \sa setPivTol().
    */
    double getRelPivTol() const
        {return m_dRelTolPiv;}

    /**
     * \return tolerance for relative pivot errors.
      * \sa setPivTol().
     */
    double getPivErrTol() const
        {return m_dTolPivErr;}

    /**
     * \return tolerance for pivot errors.
      * \sa setPivTol().
     */
    double getRelPivErrTol() const
        {return m_dRelTolPivErr;}

    /**
     * \param[in] tol any variable may maximum violation allowed.
     * \sa getVarTol().
     */
   void setVarTol(double tol);

   /**
    * \return maximum violation allowed for bounds on variables.
    */
   double getVarTol() const
        {return m_dVarTol;}

    /**
     * \param[in] tol maximum violation allowed for regular constraints (not cuts).
     * \sa getCtrTol().
     */
    virtual void setCtrTol(double tol);

    /**
     * \return maximum violation allowed for regular constraints (not cuts).
     */
    double getCtrTol() const
        {return m_dCtrTol;}

    /**
     * _Shadow prices_ (dual variables) are considered as non-negative if their values are not less than `-m_dShadowPriceTol`.
     * \param[in] tol new value for `m_dShadowPriceTol`.
     * \sa getShadowPriceTol().
    */
    void setShadowPriceTol(double tol);

    /**
     * \return currently used value for shadow price tolerance.
     * \sa setShadowPriceTol().
    */
    double getShadowPriceTol() const
        {return m_dShadowPriceTol;}

    /**
     * _Reduced costs_ of variables are considered as non-negative if their values are not less than `-m_dRedCostTol`.
     * \param[in] tol new value for `m_dRedCostTol`.
     * \sa getRedCostTol().
    */
    void setRedCostTol(double tol);

    /**
     * \return currently used value for reduced cost tolerance.
     * \sa setRedCostTol().
    */
    double getRedCostTol() const
        {return m_dRedCostTol;}

// setting/getting parameters
    /**
     * The function sets _degeneracy tolerance_ values for both, prime and dual` simplex procedures.
     * \param[in] primeDegTol  if increase of objective after performing any iterate of `primeSimplex()`
     *  is less than `primeDegTol` this iterate is considered as _degenerate_.
     * \param[in] dualDegTol  if decrease of objective after performing any iterate of `dualSimplex()`
     *  is less than `dualDegTol` this iterate is considered as _degenerate_.
     *  \remark If one of two input parameters has a negative value, the corresponding tolerance value remains unaffected.
     *  \sa getPrimeDegTol(), getDualDegTol().
     */
    void setDegenTol(double primeDegTol, double dualDegTol);

    /**
     * \return degeneracy tolerance values for prime simplex algorithm.
     * \sa setDegenTol().
     */
    double getPrimeDegTol() const
        {return m_dTolPrimeDeg;}

    /**
     * \return degeneracy tolerance values for dual simplex algorithm.
     * \sa setDegenTol().
     */
    double getDualDegTol() const
        {return m_dTolDualDeg;}
    
    /**
     * If a bound for a variable is violated by more than a given value,
     * the separation procedure will not verify any non-trivial inequalities.
     * \param[in] th tolerance value.
     * \sa getVarViolatThreshold().
     */
    void setVarViolatThreshold(double th)
    	{m_dVarViolatThreshold=th;}

    /**
     * If a bound for a variable is violated by more than a given value, called variable violation threshold,
     * the separation procedure will not verify any non-trivial inequalities.
     * \return variable violation threshold.
     * \sa setVarViolatThreshold().
     */

    double getVarViolatThreshold() const
    	{return m_dVarViolatThreshold;}

    /**
     * If a dual variable is less than a given value,
     * the corresponding column is chosen as a pivot column;
 	 * the latter is not true for the steepest edge strategy.
     * \param[in] th tolerance value.
     */
    void setShadowPriceToleranceThreshold(double th)
    	{m_dShadowPriceViolatThreshold=th;}

    /**
     * \param[in] scalingMethod new scaling method.
     * \sa enScaling.
     */
    void setScaling(enScaling scalingMethod)
        {m_eScaling=scalingMethod;}

    /**
     * Call this function to switch off all preprocessing actions, excluding scaling the matrix.
     */
    void preprocOff()
        {m_bPreproc=false;}

    /**
     * \return `true` if CLP run time messages are to be printed.
     * \sa switchLpInfoMsg()
     */
    bool lpInfoMsg() const
    {return (!(m_iInfoMsgFlag & 0x11))? true: false;}

    /**
	 * The function is called to switch on or off printing CLP run time messages.
     * \param[in] flag if set to `true`, run time CLP-messages will be printing; otherwise, not,
     */
    void switchLpInfoMsg(bool flag)
    {
    	if (!flag) m_iInfoMsgFlag|=0x1; else m_iInfoMsgFlag&=~0x1;
    }

    /**
     * \param[in] flag if set to `true`, all run time messages are blocked.
     */
    void beSilent(bool flag=true)
    {if (flag) m_iInfoMsgFlag|=0x10; else m_iInfoMsgFlag&=~0x10;}

    bool isSilent() const
    {return (m_iInfoMsgFlag & 0x10)? true: false;} ///< \return `true` if silent mode has been set.

    /**
     * \param[in] fr CLP-messages will be printed after those LP-iterates which index is a multiple of `fr`.
     * \sa switchPrintingInfoMsg().
     */
    void setFrequencyForInfoMsg(int fr)
        {if (fr > 0) m_iLpItToInform=fr;}

//    /// \param[out] str running time.
//    void getRunTime(char* str);

    /**
     * Some modification of the matrix may make the currently optimal basis either prime or dual infeasible.
     * Therefore, after having done any such modifications, the user must call this function to
     * change the status of the basis.
     * \param[in] primeFeasible if `false`, the basis becomes prime infeasible;
     * \param[in] dualFeasible if `false, the basis becomes dual infeasible;
     */
    void setOptFlags(bool primeFeasible, bool dualFeasible)
        {m_bPrimeFeasible=primeFeasible; m_bDualFeasible=dualFeasible;}

    /**
     * \param[in] dpC array of size `m_iN` of objective coefficients;
     * \param[in] sense  if `true`, the goal is to _maximize_ the objective function; otherwise, to _minimize_;
     * \param[in] scale if `true`, the objective is given in the scaled variables; otherwise, in non-scaled.
     */
    void setObjective(const double *dpC, bool sense=true, bool scale=true);

    /**
     * \param[in] i if `i < SHIFT`,  the objective is row `i`; otherwise,
     *  if `i >= SHIFT`, the objective is to optimize the (i-SHIFT)-th variable;
     * \param[in] sense  if `true`, the goal is to _maximize_ the objective function; otherwise, to _minimize_.
     */
    void setObjective(int i, bool sense=true);

    /**
     * \param[in] sense  if `true`, the goal is to _maximize_ the objective function; otherwise, to _minimize_.
     */
    void setObjSense(bool sense)
    	{m_bSense=sense;}

    /**
     * \return `true` if the goal is to _maximize_ the objective function,
     *  or `false` if the goal is to _minimize__ the objective.
     */
    bool getObjSense() const
    {return m_bSense;}

//////////////////////////////////////////////////////////////////////////
//                           Matrix attributes                          //
//////////////////////////////////////////////////////////////////////////

    /// \return number of variables.
    int getVarNum() const
        {return  m_iN;}

    /// \return number of constraints.
    int getCtrNum() const
        {return m_iM;}

    /// \return number of nonzero entries in the matrix.
    int getNonZerosNum() const
        {return m_iNZ;}

// Separation/Pricing rule
    /** \return `true` if rows can be generated.
     * \sa `enRowGenRule`.
     */
    bool isRowGen() const
    {return (m_uRowColGenRule & ROW_GEN)? true: false;}

    /** \return `true` if columns can be generated.
     * \sa `enRowGenRule`.
     */
   bool isColGen()
    {return (m_uRowColGenRule & COL_GEN)? true: false;}

protected:
    /**
     * \param[in] i constraint index.
     * \return maximum violation allowed for this particular constraint.
     */
    virtual double getCtrTol(int i) const
    	{return m_dCtrTol;}

    /**
     * \return `true` if the problem is an LP, otherwise, `false`.
     * \remark Default implementation always returns `true`.
     */
    virtual bool isPureLP() const
        {return true;}
//////////////////////////////////////////////////////////////////////////
//                       C O N S T R U C T O R S                        //
//////////////////////////////////////////////////////////////////////////
private:
    void init(); ///< Initializes class members.
public:
    CLP(); ///< Default constructor.

    /**
     *  This constructor does the same as the default one, plus it sets the problem name to `name`.
     *  \param[in] name problem name.
     */
    CLP(const char* name);
#ifndef __ONE_THREAD_
    /// Clone constructor.
    /**
     * This constructor is used in multithreaded MIP applications.
     * \param[in] other reference to `CLP` object to be cloned;
     * \param[in] thread thread index (positive integer).
     * \note The clone constructors in any derived class first must call the clone constructor of the base class.
     * \sa `CMIP::CMIP(const CMIP &other, int thread)`, `CMIP::clone()`.
     */
    CLP(const CLP &other, int thread);
#endif
    virtual ~CLP(); ///< The destructor.

//    void* operator new(size_t iSize) throw(); ///< Operator `new`.
//    void operator delete(void* pPtr) throw(); ///< Operator `delete`.
//	void* operator new[](size_t iSize) throw(); ///< Operator `new[]`.
//    void operator delete[](void* pPtr) throw(); ///< Operator `delete[]`.

/////////////////////////////////////////////////
//             Operations with matrix
/////////////////////////////////////////////////

    /**
     * Allocates memory for an LP problem of the required  size.
     * \param[in] m number of rows;
     * \param[in] n number of columns;
     * \param[in] nz number of non-zeroes in the matrix;
     * \param[in] bRowGen `true` if generation of rows is assumed;
     * \param[in] bColGen `true` if generation of columns is assumed;
     * \param[in] mMax maximum number of rows;
     * \param[in] nMax maximum number of columns;
     * \param[in] nzMax maximum number of non-zeroes in the matrix.
     * \remark If `bRowGen=true` and `mMax=0`, `openMatrix()` will estimate `mMax`;
     *   if `bColGen=true` and `nMax=0`, `openMatrix()` will estimate `nMax`.
     * \throws CMemoryException lack of memory.
     */
    virtual void openMatrix(int m, int n, int nz,
                     bool bRowGen=true, bool bColGen=false, 
                     int mMax=0, int nMax=0, int nzMax=0);

    /**
     * Does all preparations for solving the problem: allocates memory, does preprocessing, scales the matrix.
     */
    virtual void closeMatrix();

    /**
     * The function adds a constraint (empty row) to the matrix.
     * \param[in] hd handle of the constraint;
	 * \param[in] type type of the constraint;
	 * \param[in] lhs,rhs  respectively, left (LHS) and right (RHS) hand sides of constraint.
 	 * \return constraint index.
 	 * \throws CMemoryException lack of memory.
     * \sa addRow(), safeAddRow().
     */
    int addCtr(tagHANDLE hd, unsigned type, double lhs, double rhs);

    /**
     * This function is used to add a row (constraint) to the closed matrix.
     * \param[in] hd handle of row (constraint);
	 * \param[in] type type of constraint;
	 * \param[in] lhs,rhs respectively, left (LHS) and right (RHS) hand sides of constraint;
 	 * \param[in] sz number of non-zero entries;
	 * \param[in] dpVal, ipCol references to arrays of size `sz`, `dpVal[i]` is the coefficient at variable `ipCol[i]`;
	 * \param[in] bSort if `true`, row entries are sorted in increasing order of their column indices.
 	 * \return constraint index.
 	 * \throws CMemoryException lack of memory.
 	 * \sa addCtr(), safeAddRow(), getRow().
     */
    int addRow(tagHANDLE hd, unsigned type, double lhs, double rhs,int sz, double* dpVal, int* ipCol, bool bSort=true);

    /**
     * This function is a safer version of addRow().
     * If `dpVal` and  `ipCol` are references to internal MIPCL arrays such as `m_dpFb` or `m_ipArray`,
     * memory for such arrays may be reallocated during the call to `addRow()`,
     * and, as a consequence, the pointers `dpVal` and `ipCol` becomes not valid.
     * \param[in] hd,type,lhs,rhs,sz,dpVal,ipCol,bSort have the same meanings as parameters of `addRow()`.
 	 * \return constraint index.
     * \throws CMemoryException lack of memory.
     * \sa addCtr(), addRow(), getRow().
    */
    int safeAddRow(tagHANDLE hd, unsigned type, double lhs, double rhs, int sz, double* &dpVal, int* &ipCol, bool bSort=true);

    /**
     * The function adds a variable (empty column) to the matrix.
     * \param[in] hd handle of variable; if hd < 0, then you must overload
     *  `getColumn()` (version with 11 parameters);
     * \param[in] type type of variable;
     * \param[in] cost objective coefficient;
     * \param[in] l,u lower and upper bounds of variable;
     * \return index of added column (variable).
     * \throws CMemoryException lack of memory.
     */
    int addVar(tagHANDLE hd, unsigned type, double cost,double l, double u);

    /**
     * The function adds column to the matrix.
     * \param[in] hd handle of the variable; if hd >= 0, then you must overload
     *  `getColumn()` (version with 11 parameters);
     * \param[in] type type of variable;
     * \param[in] cost objective coefficient;
     * \param[in] l,u lower and upper bounds of variable;
     * \param[in] sz number of nonzero entries;
     * \param[in] dpVal,ipRow arrays of size `sz`; `dpVal[i]` is coefficient in row `ipRow[i]`;
 	 * \param[in] bSort if `true`, column entries are sorted in increasing order of their row indices.
     * \return index of added column (variable).
     * \throws CMemoryException lack of memory.
     */
    int addColumn(tagHANDLE hd, unsigned type, double cost,double l, double u,
        int sz, double* dpVal, int* ipRow, bool bSort=true);

    /**
     * This function is a safer version of addColumn().
     * If `dpVal` and  `ipCol` are references to internal MIPCL arrays such as `m_dpFb` or `m_ipArray`,
     * memory for such arrays may be reallocated during the call to `addColumn()`,
     * and, as a consequence, the pointers `dpVal` and `ipRow` becomes not valid.
     * \param[in] hd,type,cost,l,u,sz,dpVal,ipRow,bSort have the same meanings as parameters of `addColumn()`.
     * \return index of added column (variable).
     * \throws CMemoryException lack of memory.
     * \sa addVar(), addColumn(), getColumn().
    */
    int safeAddColumn(tagHANDLE hd, unsigned type, double cost,double l, double u,
        int sz, double* &dpVal, int* &ipRow, bool bSort=true);

    /**
     * The functions adds to the matrix the entry of value `val` into row `i` and column `j`;
     * if `i < 0`, the function sets the cost of variable `j`, `m_dpC[j]=val`.
     * \param[in] val entry value;
     * \param[in] i row index;
     * \param[in] j column index.
     * \throws CMemoryException lack of memory.
     */
    void addEntry(double val, int i, int j);

    /**
     * The function changes the matrix entry in row `i` and column `j`;
     * if `i < 0`, the function changes the cost of variable `j`, setting `m_dpC[j]=val`.
     * \param[in] val new entry value;
     * \param[in] i row index;
     * \param[in] j column index.
     * \throws CMemoryException lack of memory.
     */
    void changeEntry(double val, int i, int j);
    
    /**
     * \return number of non-zero entries in row `i`.
     */
    int getRowSize(int i) const
    	{return m_ipRowSize[i];}

    /**
     * \return number of non-zero entries in column `j`.
     */
    int getColumnSize(int j) const
    	{return m_ipColSize[j];}

    /**
     * \param[in] i constraint index.
     * \return left hand side of constraint `i`.
     * \sa  getRHS().
     */
    double getLHS(int i) const;

    /**
     * \param[in] i constraint index.
     * \return right hand side of constraint `i`.
     * \sa  getLHS().
     */
    double getRHS(int i) const;

    /**
     * \param[in] j index of a variable.
     * \return lower bound of variable `j`.
     * \sa  getVarUpBound().
     */
    double getVarLoBound(int j) const;

    /**
     * \param[in] j index of a variable.
     * \return upper bound of variable `j`.
     * \sa  getVarLoBound().
     */
    double getVarUpBound(int j) const;

    /**
     * \param[in] j index of a variable.
     * \return cost of variable `j`.
     * \sa setObjCoeff().
     */
    double getObjCoeff(int j) const;

    /**
     * The function sets (or changes) the cost of a variable.
     * \param[in] j index of a variable.
     * \param[in] val cost of variable `j`.
     * \sa getObjCoeff(), addEntry(), changeEntry().
     */
    void setObjCoeff(int j, double val);

protected:

    /**
     * The function multiplies the constraint coefficients by a given factor.
     * \param[in] i constraint index;
     * \param[in] factor multiplier.
     */
    void multiplyCtr(int i, double factor);

    /**
     * The functions extends the type of variable `j`
     *  by  bitwise ORing its current type with the flags stored in the parameter `type`.
     * \param[in] j index of variable;
     * \param[in] type is bitwise OR of members of `enVarType` and `CMIP::enVarType`.
     * \sa enVarType, CMIP::enVarType.
     */
    virtual void extendVarType(int j, unsigned type)
        {m_ipVarType[j]|=type;}

    /**
     * The functions extends the type of constraint `i`
     *  by  bitwise ORing its current type with the flags stored in the parameter`type`.
     * \param[in] i index of constraint;
     * \param[in] type is bitwise OR of members of `enCtrType` and `CMIP::enCtrType`.
     * \sa enCtrType, CMIP::enCtrType.
     */
    virtual void extendCtrType(int i, unsigned type)
        {m_ipCtrType[i]|=type;}

//    /**
//     * To be overloaded in `CMIP`.
//     * \sa CMIP::isVarIntegral().
//     */
//    virtual bool isVarIntegral(int /*j*/)
//        {return false;}

   	/**
   	 * \param[in] j variable index.
   	 * \return `true` if variable `j` is integer-valued.
   	 */
    bool isVarIntegral(int j) const
    {
        return (m_ipVarType[j] & VAR_INT)? true: false;
    }

    /**
     * To be overloaded in `CMIP`.
     * \sa CMIP::isVarStrongIntegral().
     */
    virtual bool isVarStrongIntegral(int /*j*/) const
        {return false;}

    /**
     * To be overloaded in `CMIP`.
     * \sa CMIP::isVarBinary().
     */
    virtual bool isVarBinary(int /*j*/) const
        {return false;}

    /**
     * To be overloaded in `CMIP`.
     * \sa CMIP::isVarScalable().
     */
    virtual bool isVarScalable(int /*j*/) const
        {return true;}

    /**
     * To be overloaded in `CMIP`.
     * \sa CMIP::isVarDeletable().
     */
    virtual bool isVarDeletable(int /*j*/) const
        {return true;}

    /**
     * To be overloaded in `CMIP`.
     * \sa CMIP::isVarModifyable().
     */
    virtual bool isCtrModifyable(int /*i*/) const
        {return true;}

    /**
     * \param[in] row row index;
     * \param[out] dpVal,ipCol arrays of size `k`, where `k` is return value;
     * `dpVal[i]` is coefficient in column `ipCol[i]`;
     * \param[in] bScaled if `true`, row of scaled matrix is returned; otherwise, row of non-scaled matrix.
     * \return number of non-zero coefficients in row `row`.
     */
    int getRow(int row, double* dpVal, int* ipCol, bool bScaled=false);

    /**
     * \param[in] col column index;
     * \param[out] dpVal,ipRow arrays of size `k`, where `k` is return value;
     * `dpVal[i]` is the coefficient in row `ipRow[i]`;
     * \param[in]  bScaled if `true`, column of scaled matrix is returned; otherwise, column of non-scaled matrix.
     * \return number of non-zero coefficients in column `col`.
     */
    int getColumn(int col, double* dpVal, int* ipRow, bool bScaled=false);

    /**
     * \param[in] recomputeBasicVarsCost if `true`,
     * total cost of non-basic variables stored in `m_dBasicVarCost` is recomputed.
     * \return cost of of current basic solution.
     */
    double computeObjValue(bool recomputeBasicVarsCost=false);

////////////////////////////////////////
//       Preprocessing
///////////////////////////////////////
protected:
    /**
     * \param[in] a,b two integers (although written as double reals).
     * \return global common divisor of input integers.
     */
    static double GCD(double a, double b);

////////
private:
    bool preprocessCtr(int ind, int& last,double& lhs, double& rhs, double& loBd, double& upBd);
    int deleteRedundantRows(bool flag=false); ///< deletes redundant (with INFINITY bounds) and singleton rows.

    void deleteFixedVar(int col); ///< deletes fixed variable indexed by `col`.
    int deleteFixedVars(); ///< iteratively calls `deleteFixedVar()`.

    /**
     * The function is called from `delFixedVars()`.
     * \return  `true` if variable `col` has been fixed by duality reasoning.
     */
    bool dualFixVar(int col);

    int processParallelColumns(); ///< seeks parallel columns to break possible symmetries.
    int processParallelRows(); ///< combines any pairs of parallel constraints into one two-sided constraint.

    /**
     * The procedure compares two rows: a row is less than another one if the value of the first row is less than the value of the second.
     * \param[in] r1,r2 row indices.
     * \return number of fixed variables.
     * \remark Each row `r` is associated with a flag `cpRowPassiv[r]` non-zero value of which indicates that
     *  row `r` is dominated (less or equal) by another row. So, if `compareRows()` establishes that
     *  row `r1` is less or equal than row `r2` or vice versa, it assigns the value of `1` to either `cpRowPassiv[r]` or `cpRowPassiv[r2]'.
     */
    int compareRows(int r1, int r2);

    /**
     *  The procedure eliminates all constraints that dominate (or are dominated by) a given constrain;
     *  \param[in] constraint index.
     *  \return number of fixed variables plus number of removed rows.
     */
    int removeDominatedRows(int row);

    /**
     *  The procedure eliminates all dominated constraints; does its job by calling `removeDominatedRows()`.
     *  \param[in] timeLimit time limit.
     *  \return number of fixed variables plus number of removed rows.
     */
    int processDominatedRows(__LONG timeLimit);

    /**
     *  The procedure deletes a given constraint with single variable, and simultaneously updates the bounds on that variable.
     *  \param[in] row constraint index.
     */
    void deleteSingletonRow(int row);

    /**
     * The function, in the equality indexed by `row`, seeks for a variable to be substituted for
     * the linear expression induced from equality `row` so that the number of fill-ins (additional non-zero entries in the matrix)
     * does not exceeds some predefined value.
     * \param[in] row index of some equality constraint;
     * \param[in] flag if `flag=0`, only real variables are considered; if `flag=1`, only integer variables are considered,
     * and if `flag=2`, only binary variables are considered.
     * \returns index of variable to be substituted, or `-1` if no variable is good for substituting.
     */
    int estimateFillIns(int row, int flag);

    /**
     * Only small size equalities are used when substituting variables.
     * \param[in] row - row index of equality constraint `\sum_{i\in I} a_{row,i} x_i = b_{row}`
     * \return `-1` if no variable has been chosen to be substituted;
     *   otherwise, variable `j` to be substituted for expression
     *       `b_{row} - sum(i in I\ j) a_{row,i} x_i`.
     */
    int getBestFillInColumn(int row);

    /**
     * The procedure substitute variable `x_j` for the expression
     *         `b_{row} - sum(i in I\ j} a_{row,j} x_j`.
     * \param[in] row - row index of equality constraint `\sum_{i in I} a_{row,i} x_i = b_{row}`;
     * \param[in] j  index from `I`.
     */
    void substitute(int row, int j);

    void alignMatrixByRows(); ///< non-zeroes of any column j are followed by non-zeroes of column j+1

    void alignMatrixByColumns(); ///< non-zeroes of any row j are followed by non-zeroes of row i+1

    /**
     * After the matrix has been closed, any new constraint
     * `b1 <= sum(i=0,...,sz-1) dpVal[i]*x[ipCol[i]] <= b2`,
     * before adding to the matrix, must be preprocessed.
     * \param[in] n number of variables in initial (n0n-preprocessed) matrix;
     * \param[in,out] b1,b2 left and right hand sides;
     * \param[in,out] sz number of non-zero entries (size);
     * \param[in,out] dpVal array of coefficients (of size `sz`);
     * \param[in,out] ipCol array of column indices (of size `sz`).
     */
    void preprocNewCtr(int n, double& b1, double& b2, int& sz, double* dpVal, int* ipCol);

    void labelActiveCtrs(int col, int& last);

    /**
     *  The procedure applies standard preprocessing techniques to a given list of constraints.
     *  \param[in] last number of constraint indices written in array `ipCtrLst=m_ipArray+m_iN`.
     *  \return `true` if no contradictions have been detected; otherwise, `false`.
     *
     */
    bool preprocessMatrix(int last);

    /**
     * The procedure does some preparations for calling `preprocessMatrix(int&)`.
     */
    bool preprocessMatrix();

    int seekIncludedCtrs();

protected:
    /**
     * The procedures replaces bounds `[d1[j],d2[j]]` for `[0,d2[j]-d1[j]]`) for those variables, `j`, which lower bounds `d1[j] <!= 0`.
     * \return number of changed bounds.
     */
    int shiftBounds();

    /**
     * Just a skeleton. To be overloaded in `CMIP`.
     * \return `true`.
     * \remark Default implementations does nothing.
     */
    virtual bool preprocessInit() {return true;}

    /**
     * Just a skeleton. To be overloaded in `CMIP`.
     * \return `true`.
     * \remark Default implementations does nothing.
     */
    virtual bool preprocessPlus() {return true;}

    /**
     * Preprocessing without probing.
     * \param[in] bDominant if `true`, dominated constraints are detected and processed.
     * \return total number of variables fixed and constraints eliminated.
     */
    int basicPreprocess(bool bDominant=true);

    /**
     * The procedure reformulate an LP (or MIP) by calling, in turn,
     *      - `preprocessInit()`,
     *      - `basicPreprocess()`,
     *      - `preprocessPlus()`.
     * \return `false` if inconsistency has been detected; otherwise, `true`.
     */
    bool preprocess();

////////////////////////////////////////
// S C A L I N G
private:
    void SCL_ShiftScaleFactors(); ///< The procedure is used only in `SCL_scaleMatrix()`.

    /**
     * \param[out] minExp minimum exponent among matrix entries.
     *  \return the maximum absolute value among exponents of matrix entries.
     */
    int SCL_getMaxEntryExponent(int &minExp);
    int SCL_HScaleRow(int row, bool GMflag);
    int SCL_HScaleColumn(int col, bool GMflag);
    void SCL_HScaleRows(bool GMflag);
    void SCL_HScaleColumns(bool GMflag);

    /**
     * The procedure iteratively scales rows an columns processing each of them `roundNum` times.
     * \param[in] roundNum number of iterates (rounds);
     * \param[in] bGMflag  if `true`, geometric mean scaling is applied; otherwise, min-max scaling.
     * \param[in] bRowAlign if true, rows are scaled after columns.
     */
    void SCL_HScale(int roundNum, bool bGMflag, bool bRowAlign);


    bool SCL_computeLevels(double dMaxExp, int& iNodeNum,
        int* &ipNodeSep,int* &ipNode, int* &ipRowNode, int* &ipColNode, 
        int* &ipNewNodeSep,int* &ipNewNode, int* &ipNewRowNode, int* &ipNewColNode, 
        int* ipNum, int* ipLevel, int* ipLowLink, int* ipStack, int* ipParent,
        int *ipCurRow, int* ipCurCol, int* ipCurNode,
        double *dpRowScale, double *dpColScale); ///< is used internally `SCL_IdealScaling()`.
    /**
     * The procedure implements the "ideal scaling".
     *
     * \param[in] maxExp maximum exponent among matrix entries.
     */
    void SCL_IdealScaling(int maxExp); ///< ideal scaling.


    /**
     * The procedure is used internally in `SCL_MinMaxScale()`.
     * \remark set `flag` to `true` only for the last call to the procedure.
     */
    bool SCL_estimateExponent(int q, int *ipS, int* ipT, bool flag=false);

    /**
     * The procedure implements min-max scaling, i.e.,
     * in each line (row or column) the mean value of the minimum and maximum absolute coefficient values are computed,
     * and then the line is multiplied by the power of 2 nearest to that mean value.

     * \param[in] maxExp maximum exponent among matrix entries.
    */
    void SCL_MinMaxScale(int maxExp);

protected:
    void SCL_scaleObj(); ///<  Scales objective with previously computed column scale factors.
    void SCL_scaleMatrix(); ///< Scales matrix with previously computed scale factors.

    /**
     * \remark The method used for computing factors is determined by the current value of `m_eScaling`,
     *  which is set by `setScaling()`.
     */
    void scaleMatrix(); ///< Compute factors and then scales matrix.
    void unscaleMatrix(); ///< Recovers matrix to initial state.

/////////////////////////////////////////////////////////////////////

    /**
     * The function deletes variable (column) `j` from the matrix and adds its expression
     *       x(j) = b +sum(i=0,...,sz-1} dpVal[i]*x(ipCol[i])
     * to the preprocessing stack.
     * \param[in] j index of variable;
     * \param[in] hd handle of deleted variable, or `hd in {-1,-2}`;
     *   if `hd = -1`, then `sz = 1` and variable `x[ipCol[0]]` is substituted for `x[ipCol[0]]*dpVal[0]+b`;
     *   if `hd = 2`, then `sz = 1`, and, for `c=reinterpret_cast<int*>(dpVal)[0]`,
     *   if `x[ipCol[0]] >= 0.0`, then `x[c]=0.0`; otherwise, `x[c]=-x[ipCol[0]]` and `x[ipCol[0]]=0.0`;
     * \param[in] b free term of expression;
     * \param[in] sz number of variables in expression
     * \param[in] ipCol list of `sz` variables (columns) in expression;
     * \param[in] dpVal list of `sz` coefficients in expression.
     */
    virtual void deleteVariable(int j, int hd,  double b, int sz, int *ipCol, double *dpVal);

    /**
     * The procedure deletes from the matrix all the rows and columns that were deleted during preprocessing.
     * This procedure is also used in `CMIP` to delete from the matrix those cuts that are not "tight" for optimal node LP-solutions.
     * \param[in] m0 initial row to start search for unneeded rows;
     * \param[in] n0 initial column to start search for unneeded columns;
     * \param[in] prepFlag if `true`, expressions in preprocessing stack are also updated;
     * \param[in] bEntry is used internally in `CMIP`.
     * \attention Never set both, `prepFlag` and `bEntry`, flags to `false` in your applications.
     */
    void compressMatrix(int m0=0, int n0=0, bool prepFlag=true, bool bEntry=true);

    /**
     * The function is called when a set of constraints is deleted from the matrix.
     * Default `CLP`-implementation does nothing.
     * \sa CMIP::setCtrsInactive().
     */
    virtual void setCtrsInactive(int /*sz*/, int* /*ipCtr*/) {};

    /**
     * The function is called when a set of columns is deleted from the matrix.
     * Default `CLP`-implementation does nothing.
     * \sa CMIP::setColumnsInactive().
     */
    virtual void setColumnsInactive(int /*sz*/, int* /*ipCtr*/) {};

    /**
     * The procedure deletes all "_non-tight_" inequalities and columns previously generated during the solution process,
     * and which indices `>=` m for rows, and `>= n` for columns.
     * \param[in] m only rows which index is not less than `m` are considered as candidates for deletion;
     * \param[in] n only columns which index is not less than `n` are considered as candidates for deletion;
     * \param[in] bFull if `true`, matrix must be compressed and solution be updated;
     * \param[in] tight inequality (resp, column) is "_tight_" if its _slack_ (resp., _reduced cost_)
     *  is less than value of `tight`.
     */
    void deleteNonBasicLines(int m, int n, bool bFull, double tight);

    /**
     * The function is used to express constraints in scaled variables.
     * \param[in] sz size (number of variables) of constraint;
     * \param[in,out] dpVal,ipCol arrays of size `sz`; in constraint being scaled,
     *  `dpVal[i]` is coefficient at variable `ipCol[i]`.
     *  \sa scaleCtr().
     */
	void scaleRow(int sz, double* dpVal, int* ipCol);

	/**
	 * When implementing a separation, or cut generation procedure,
	 *  the user knows nothing about transformations of the matrix
	 * that will be done by the solver. Therefore, in user defined procedures inequalities (cuts) are generated
	 * assuming that the matrix had not been changed.
	 *  Before adding such inequalities to the matrix, they must be previously scaled.
	 *
     * Given a constraint expressed in scaled variables,
     *  the function computes _scaling factor_ and multiplies that constraint by this factor.
     * \param[in,out] lhs,rhs left and right sides of constraint being scaled;
     * \param[in] sz size (number of variables) of constraint;
     * \param[in,out] dpVal,ipCol arrays of size `sz`; in constraint being scaled,
     *  `dpVal[i]` is coefficient at variable `ipCol[i]`.
     * \return scaling factor.
     *  \sa scaleRow().
     */
    int scaleCtr(double& lhs, double& rhs, int sz, double* dpVal, int* ipCol);

    /**
     * The function multiplies each coefficient (including the cost) in the column
     * by the factor of that row the coefficient belongs to.
     * \param[in,out] cost cost of column being scaled;
     * \param[in] sz size (number of rows with non-zero coefficients) of column;
     * \param[in] dpVal,ipCol arrays of size `sz`; in column being scaled,
     *  `dpVal[i]` is coefficient in row `ipRow[i]`.
     *  \sa scaleColumn().
     */
	void scaleColumn(double &cost, int sz, double* dpVal, int* ipCol);

	/**
	 * When implementing a column generating procedure, the user knows nothing about transformations of the matrix
	 * that will be done by the solver. Therefore, in user defined procedures columns are generated assuming
	 * that the matrix had not been changed. Before adding such columns to the matrix, they must be previously scaled.
	 *
     * Given a variable (cost coefficient, column of the matrix, and lower and upper bounds),
     * `scaleVar()` computes _scaling factor_ and multiplies that column by this factor.
     * \param[in,out] cost cost of column being scaled;
     * \param[in,out] l,u lower and upper bounds that corresponds to column being scaled;
     * \param[in] sz size (number of variables) of constraint;
     * \param[in,out] dpVal,ipRow arrays of size `sz`; in column being scaled,
     *  `dpVal[i]` is coefficient in row `ipRow[i]`.
     * \return scaling factor.
     *  \sa scaleColumn().
     */
    int scaleVar(double &cost, double& l, double& u, int sz, double* dpVal, int* ipRow);

//////////////////////////////////////////////////////////////////////////
//                   Initialization/memory allocation                   //
//////////////////////////////////////////////////////////////////////////
    /**
     * The procedure allocates memory needed for both, prime and dual, simplex algorithms.
     * \throws CMemoryException lack of memory
     */
    void allocMemForSimplex();

    /**
     * The function is used internally in `CMIP`.
     * It builds linked lists of both, rows and columns.
     */
    void buildRowColLists();

private:
    void allocColMemForSimplex(); ///< The procedure allocates memory for "column" arrays used in simplex procedures.
    void allocRowMemForSimplex(); ///< The procedure allocates memory for "row" arrays used in simplex procedures.

    /**
     * The procedure reallocates memory for "column" arrays used in simplex procedures.
     *
     * If the number of columns (`m_iN`) approaches its limit (`m_iNmax`),
     * this limit is increased 1/3-d of its initial value, and then memory is reallocated.
     *
     */
    void reallocColMemForSimplex();

    /**
     * The procedure reallocates memory for "row" arrays used in simplex procedures.
     *
     * If the number of rows (`m_iM`) approaches its limit (`m_iMmax`),
     * this limit is increased 1/3-d of its initial value, and then memory is reallocated.
     *
     */
    void reallocRowMemForSimplex();

    void allocMemForNorms(); ///< The procedure allocates memory for arrays storing row or column norms.
    void reallocMemForNorms(); ///< The procedure reallocates memory for arrays storing row or column norms.

    void allocMemForEntries(); ///< The procedure allocates memory for the matrix.
protected:
    /**
     * The function reallocates memory for the matrix.
     *
     * If the number of nonzero entries (`m_iNZ`) approaches its limit (`m_iNZmax`),
     * this limit is increased 1/3-d of its initial value, and then memory is reallocated.
     *
     * \param[in] nz maximum number of entries.
     * \remark This function is overloaded in CMIP to make it thread safe.
     * \attention The procedure should not be called in user applications.
     */
    virtual void reallocMemForEntries(int nz);

    /**
     * The function is called when the number of rows (constraints) exceeds `m_iMmax`.
     * \attention `incMaxRowNumber()` should not be called in user applications.
     * \remark This function is overloaded in CMIP to make it thread safe.
     */
    virtual void incMaxRowNumber();

    /**
     * The function is called when the number of columns (variables) exceeds `m_iNmax`.
     * \attention `incMaxColumnNumber()` should not be called in user applications.
     * \remark This function is overloaded in CMIP to make it thread safe.
     */
    virtual void incMaxColumnNumber();

    /**
     * The function allocates memory for three auxiliary arrays,
     *  `m_dpArray` and `m_ipArray`,  both of size `max{m_iNmax,m_iMmax}`,
     *  and `dpW` of size `2*m_iMmax`.
     * `allocMemForAuxArrays()` should not be called in user applications.
     * \attention `reallocMemForAuxArrays()` should not be called in user applications.
     * \remark This function is overloaded in `CMIP` to allocate memory for two additional arrays.
     * \sa reallocMemForAuxArrays().
     */
    virtual void allocMemForAuxArrays();

    /**
     * The function allocates memory for three auxiliary arrays,
     *  `m_dpArray` and `m_ipArray`,  both of size `max{m_iNmax,m_iMmax}`,
     *  and `dpW` of size `2*m_iMmax`. This function is called when either the number of columns
     *  exceeds `m_iNmax`, or the number of rows exceeds `m_iMmax`.
     *
     * \param[in] rowMem maximum number of entries.
     * \attention `reallocMemForAuxArrays()` should not be called in user applications.
     * \sa allocMemForAuxArrays().
     */
    void reallocMemForAuxArrays(bool rowMem, bool /*colMem*/);

private:
    void allocMemForProblem(); ///<  \throws CMemoryException
    void allocMemForColumns(); ///<  \throws CMemoryException
    void reallocMemForColumns(); ///<  \throws CMemoryException
    void allocMemForRows(); ///<  \throws CMemoryException
    void reallocMemForRows(); ///<  \throws CMemoryException

    int saveShortBasis(int* mem);
    int saveIntBasis(int* mem);
    int saveBasisBitwise(unsigned* mem);
///
    void restoreShortBasis(int* mem);
    void restoreIntBasis(int* mem);
    void restoreBasisBitwise(unsigned* mem);
protected:
    /**
     * The function packs the basis into a memory buffer (pointed to by `mem`).
     * \param[out] mem pointer to memory array of size at least `m_iN`.
     * \return size (in integer words) of stored data.
     * \sa restoreBasis().
     */
    int saveBasis(int* mem);

    /**
     * The function restores basis previously stored by `saveBasis()`.
     * \param[in] mem pointer to memory array with stored basis.
     * \sa saveBasis().
     */
    void restoreBasis(int* mem);

    /**
     * The function restores full basis from its short version given by two arrays `m_ipRowMap` and `m_ipColMap`.
     * \param[in] ipRowMap,ipColMap pointer to memory array with stored basis  in short form;
     *  usually, `ipRowMap` and `ipColMap` are copies of `m_ipRowMap` and `m_ipColMap`.
     *  \remark The function is used internally in `CMIP`.
     * \sa `m_ipRowMap` and `m_ipColMap`.
     */
    void restoreBasis(int* ipRowMap, int* ipColMap);
/////////////////////////////////////////////////////////////

// SEPARATION

    /**
     * \param[in] i constraint index;
     * \param[in] scaled if `true`, slack for scaled matrix is returned; otherwise, for not-scaled.
     * \return slack value of right part of constraint `i` for basic solution currently stored in memory.
     */
	double getCtrRightSlack(const int i, bool scaled=true) const;

    /**
     * \param[in] i constraint index;
     * \param[in] scaled if `true`, slack for scaled matrix is returned; otherwise, for not-scaled.
     * \return slack value of left part of constraint `i` for basic solution currently stored in memory.
     */
	double getCtrLeftSlack(const int i, bool scaled=true) const;

private:
	/**
	 * Multiplies row `i` by vector stored in `X`, assuming that components in `X` are listed so as in `m_ipBasicColumn`.
	 * \param[in] i row index;
	 * \param[in] X array of size `m_iN`.
	 * \return   \sum_{0 \le j < sz} A(j,m_iBasicColumn[j])*X[j].
	 */
    double getBasicLeftHand(int i, double* X) const;

    bool checkInEq(int i, double& beta, bool& side) const;
    int getViolatedEquation(double& beta, bool& side) const;
    void switchSeparation(enSepRule eSepRule, bool bUpdateFlag=false);


    void updateDualCandSet(); ///< updates candidate set of constraints used when doing partial separation.

    int rowSeparate(int& r, bool& side, double& edg);

    /**
     * The procedure separates the current basic solution stored in `m_dpX`.
     * \param[out] side `true` if left side of inequality `r`, where `r` is return value, is violated; otherwise, `false`.
     * \return index of violated inequality.
     */
    int innerSeparate(bool& side);

/////////////////////////////////////////////////////////////
// PRICING
    void switchPricing(enPricingRule ePricingRule, bool bUpdateFlag=false);
    void updatePrimeCandSet();
    int pricingColumns(int& r, double edg);
    int pricing();
//////////////////////////////////////////////////////////////////////////
//                          Pivoting Functions                          //
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * The functions sets the pricing rule to be used for solving LPs and root LPs of MIPs.
     * \param[in] pricingRule new pricing rule.
     * \sa enPricingRule, getPricingRule().
     */
    void setLpPricingRule(enPricingRule pricingRule)
    {
        m_eLpPricingRule=pricingRule;
    }

    /**
     * \return currently used pricing rule.
     */
    enPricingRule getPricingRule() const
    {return m_ePricingRule;}

    /**
     * The function sets the __separaion rule__ applied when solving LPs,
     *  and is also used when solving root LPs of MIPs.
     *  \param[in] sepRule new separation rule.
     * \sa enSepRule, getSepRule(), setMipSepRule().
     */
    void setLpSepRule(enSepRule sepRule)
    {
    	if (sepRule != SEP_ONLY_EQUATIONS)
    		m_eLpSepRule=sepRule;
    }

    /**
     * \return currently used _separation rule_.
     */
    enSepRule getSepRule() const
    {return m_eSepRule;}

    /**
     * At each iterate both, prime and dual, simplex algorithms solves at least two linear systems, `B x = b` and `B^Ty=c`.
     * Here $B$ denotes basic matrix, vector $b$ is composed from the basic components of the right or left hand side vectors,
     * and vector $c$ is composed from the basic components of the upper or lower bound vectors.
     * To solve the systems `B x = b` and `B^Ty=c`, `CLP` _factors_ the basic matrix, i.e., computes an LU-partition: `B=LU`,
     * where `L` is lower, and `U` upper triangular matrices. Since computing LU-partition is time-consuming operation,
     * it is computed only in those simplex-iterations that are multiple of some predefined number (say, 50).
     * In all other iterations, `CLP` updates the partition of the previous iteration.
     * Although updating LU-partitions is less costly than re-factoring the basic matrix,
     * such updates introduce additional errors in the solutions. To eliminate these the solver has to re-factor the basis
     * after carrying out a predefined number of updates, or in case the solver detects computational instability.
     *
     * `setMaxLuUpdateNum()` sets a new limit on the number of consecutive updates of the LU-partition.
     * \param[in] maxUpdateNum maximum number of LU-updates.
     */
    void setMaxLuUpdateNum(int maxUpdateNum)
        {m_iMaxLuUpdateNum=maxUpdateNum;}

    /**
     * _Partial pricing_ (resp., _separating_) assumes selecting violated column (resp., constraint)
     * among a set of previously selected candidate columns (resp., inequalities).
     * This set of candidate columns (resp., inequalities) is updated if during a few last simplex-iterates
     * the objective has increased (resp., decreased) "not-substantially".
     *
     * Use `setMaxCandNum()` to change the candidate-set size.
     * \param[in] maxCandNum maximal size of candidate set.
     */
    void setMaxCandNum(int maxCandNum)
        {m_iMaxCandNum=maxCandNum;}
private:
//         Steepest Edge
//////////////////////////////////////////////////////////////////////////
    bool isCtrRef(int i) const ///< \return `true` if constraint `i` is in reference framework
    {return (m_ipCtrType[i] & CTR_REF)? true: false;}
    bool isVarRef(int i) const ///< \return `true` if variable ``i is in reference framework
    {return (m_ipVarType[i] & VAR_REF)? true: false;}

    void ctr2Ref(int i) const
    {m_ipCtrType[i] |= CTR_REF;}
    void var2Ref(int i) const
    {m_ipVarType[i] |= VAR_REF;}

    void clearCtrRef(int i)
    {m_ipCtrType[i] &= ~CTR_REF;}
    void clearVarRef(int i)
    {m_ipVarType[i] &= ~VAR_REF;}

    void DUAL_rectify(); ///< initializes constraints norms and reference framework

    double computeColumnNorm(int i); // computes norm for column `m_ipBasicColumn[i]`
    void computeColumnNorms(int k1=0, int k2=0); /// computes norms for columns `m_ipBasicColumn[k1},...,m_ipBasicColumn[k2-1]`
    bool extendUdAndColNormUpdates(int t);
    void updateColumnNorms(int t);

    /**
     * The function computes the norm of a given row.
     * \input[in] i row index.
     * \return norm value.
     */
    double computeRowNorm(int i);

    /**
     * The procedure the computes norms for all rows.
     */
    void computeRowNorms();

    void updateRowNorms(int s, double ur, double tau);

//////////////////////////////////////////////////////////////////////////
    bool PRIME_getPivotVal(int s, int t);
    bool computeUb(int col); ///< is called only in `primeRatioTest()`
    void makePrimeVarFeasible(int j, bool side, double delta); ///< is called when variable `j` is infeasible
    void makePrimeCtrFeasible(int j, bool side, double delta); ///< is called when constraint `j` is infeasible
    int primeRatioTest(int t, bool& side, bool& bDegen, bool bSafeMode);
    void primeSearchPivot(int& s, int& t, bool& side, bool& bDegen);

    int computeUd(int s, bool bSafeMode, double& dMaxVal); ///< used only in `dualRatioTest()`
    int computeUdAndRowNormUpdates(int s, bool bSafeMode, double& dMaxVal);///< called only in `dualRatioTest()`
    void makeDualFeasible(int i); ///< is called if j-th dual variable is infeasible, i.e., (negative for right or positive for left inequality)
    int dualRatioTest(int s, bool side, bool& degen, bool bSafeMode);
    double dualGetPivotVal(int s, int t, double* U);
    void dualSeekPivot(int& s, int& t, bool& side, bool& degen, bool needSol=true);

private:
    void unpackRow(int i, double* d);
    void unpackRowForBasis(int s, double* U);
    void unpackColumnForBasis(int s, double* U, int ExtraRow=-1, double* v=0);

    /**
     * if bSafeMode == true, LU::Factor is called in the safe mode,
     * i.e. with (full) column permutations to reduce rounding off errors
    */
    void updatePartition(bool bSafeMode=false);

    void updateDualSolution(int r, double delta);
    void updatePrimalSolution(double delta);

    /**
     * \param[in,out] y on input, if not zero, vector of basic components of dual solution; on output, full (including reduced costs) dual solution;
     * \param[in] how =1 for column base extension, =-1 for row base extension, = 0 to be decided in the procedure;
     * \param[in] bWithFixed if `false`, the reduced costs of fixed variables are not computed.
     */
    void extendDualSolution(double* y=0, int how=0, bool bWithFixed=false);

    void extendPrimeSolution(double* x=0, int col=-1, int how=0); ///< Set how = 1 in case of partial separation

    int substituteRowForRow(int r, int s, bool side);
    int substituteRowForColumn(int r, int s, bool side);
    void swapNonBasicColumns(int c1, int c2);
    void swapNonBasicRows(int r1, int r2);
    /**
     * The procedure performs the pivot operation when a column is substituted for a row.
     * \param[in,out] r column `c=m_ipBasicColumn[r]` is to be replaced by row `s`;
     *  since `substituteColumnForRow()` swaps columns, on return, `r` gives a new position of `c`
     *  in array `m_ipBasicColumn`;
     *  \param[in] s row to enter basis;
     *  \param[in] side if `true` right hand side constraint given by `row` enters basis;
     *   otherwise, right hand side constraint enters basis.
     *   \return `1` if the row can be added as a multiplicator, otherwize, `-1`.
     */
    int substituteColumnForRow(int& r, int s, bool side);

    int substituteColumnForColumn(int r, int s, bool side);
    void doPivot(int s, int r, bool side);

protected:
    /**
     * The function transforms a non-scaled basic solution for the LP in the memory
     * into a solution of the original LP or MIP. To do its job, the function uses expressions
     * in the preprocessing stack to recover the values of those variables that were eliminated
     *  from the matrix during preprocessing.
     * \param[in] dpBasicX basic solution;
     * \param[out] dpX array of size `m_iN` that sores extended solution;
     * `dpX[i]` is value of variable with handle `m_ipColHd[i]`.
     * \return
     */
    int getPrimeSolution(double* dpBasicX, double* dpX);

    /**
     * \param[in] j index of variable;
     * \return value of variable `j`.
     */
    double getVarValue(int j) const;

    /**
     * \param[in] i row index;
     * \return value of row `i`.
     */
    double getRowValue(int i) const;

public:
    /**
     * \param[in] i row index;
     * \param[in] dpX array of size `m_iN`.
     * \return the scalar product of the constraint vector of row `i` and the vector stored in `dpX`.
     */
    double getRowValue(int i, double * dpX) const;

protected:

    /**
     * The function computes the expression of a given vector in the current basis.
     * If `col >= 0`, then the expression (in current basis) of the unit vector with 1 in position `col` is computed.
     * \param[in,out] x array of size `m_iN` storing vector.
     * \param[in] col column index.
     * \remark This function is used internally in `CLP` and `CMIP`.
     * \sa computeY().
     */
    void computeX(double* x=0, int col=-1);

    /**
     * The function computes the expression of a given dual vector in the current basis.
     * \param[in,out] y array of size `m_iN` storing vector.
     * \param[in] withFixedVars if `false` reduced costs of fixed variables are not computed.
     * \remark This function is used internally in `CLP` and `CMIP`.
     * \sa computeX().
     */
    void computeY(double* y=0, bool withFixedVars=false);

    /**
     * The function re-factors the basis, and then recomputes both, prime and dual, basic solutions.
     */
    void updateSolution();

    /**
     * The function computes the vector weighted sum of all non-basic columns,
     *  i.e. each column is first multiplied by the value of corresponding variable.
     *  \sa incColumnSum().
     */
    void computeBasicColumnSum();

    /**
     * The function adds a given matrix column, `col`, multiplied by `delta` to the sum of basic columns.
     * \param[in] col column index;
     * \param[in] delta multiplier.
     */
    void incColumnSum(int col, double delta);

private:
//////////////////////////////////////////////////////////////////////////
//                      Prime and Dual Simplex Methods                  //
//////////////////////////////////////////////////////////////////////////
    /**
     * \param[in] col column index;
     * \param[in] dCost column cost,
 	 * \param[in] dpPi vector of size `m_iBasisSize`, row basic potentials.
     * \return reduced cost of variable indexed with `col`.
     */
    double getReducedCost(int col, double dCost, double* dpPi);

    /**
     * The procedure deletes from the matrix all rows and columns for previously deleted constraints and variables.
     */
    void compressBasis();

    /**
     * The procedure deletes from the matrix all columns corresponding to slack variables.
     */
    void deleteSlackVars();

    /**
     * THe procedure extends the basis to make it prime feasible by adding slack variables
     * \param[in] dpX array of size `m_iN`, basic solution to be extended.
     */
    void addSlackVars(double bigM, double* dpX);


//////////////////////////////////////////////////////////////////////////
// Perturbation of the objective, row and column bounds to prevent Cycling
/////////////////////
    virtual bool storeObj();
    virtual void restoreObj(bool updateY);
    void prtbObj(); ///< randomly perturbs basic column variables.

    bool storeBounds();
    void prtbBounds();
    void restoreBounds(bool updateX);
///////////////////////////////////////////////////////////////////////////

public:
    /**
     * The function sets the solution algorithm to be used for solving LPs and root LPs of MIPs.
     * \param[in] method solution algorithm.
     * \sa enLPmethod, getCurrentLPmethod().
     */
    void setLPmethod(enLPmethod method)
        {m_eDefLPmethod=method;}

    /**
     * \return currently used LP algorithm.
     */
    enLPmethod getCurrentLPmethod() const
        {return m_eLPmethod;}

    /**
     * This procedure does a lot of useful things: allocates memory, applies different preprocessing techniques,
     * scales the matrix, and so on. As a results, in many cases, we can get substantially simple (for solving)
     * problem, which is still equivalent to the original (user) problem.
     * The latter means that, given an optimal solution to the transformed problem, we can
     * easily compute an optimal solution to the original problem.
     * \return  `false` if inconsistency has been detected; otherwise, `true`.
     * \throws CMemoryException lack of memory.
    */
    virtual bool prepare();

    /**
     * The procedure implements a prime simplex algorithm.
     * \param[in] upperBound procedure stops if objective value exceeds `upperBound`;
     * \param[in] maxItNum  procedure stops after carrying out at most `maxItNum` iterations;
     * \param[in] timeLimit procedure stops after at most `timeLimit` seconds; `timeLimit=0` means no time limitation.
     * \param[in] degCheckInt,degPrc degeneracy test is activated after `degCheckInt` consecutive iterations;
     * if more than `degPrc`% iterations among the last `degCheckInt` ones were degenerated, bounds on variables are perturbated.
     * \return
     *   - `0` solution found;
     *   - `1` objective function upper bound, `upperBound`, exceeded;
     *   - `2` limit on number of iterates,  `maxItNum`, exceeded;
     *   - `3` time limit, `timeLimit`, exceeded.
     *   \throws CMemoryException lack of memory.
     *   \sa dualSimplex().
     */
    int primeSimplex(double upperBound=INF, int maxItNum=1000000000,
    		__LONG timeLimit=0l, int degCheckInt=200, int degPrc=30);

    /**
     * The procedure implements a dual simplex algorithm.
     * \param[in] lowerBound procedure stops if objective value becomes less than `lowerBound`
     * \param[in] maxItNum  procedure stops after carrying out at most `maxItNum` iterates;
     * \param[in] timeLimit procedure stops after at most `timeLimit` seconds; `timeLimit=0` means no time limitation;
     * \param[in] needSol if set to `true` and if solution has been found, on termination basic matrix is re-factored to eliminate computational errors.
     * \param[in] inconsistSertificate if set to `true`, on termination, even if problem has been proven to be infeasible, basic matrix is re-factored to eliminate computational errors.
     * \param[in] degCheckInt,degPrc degeneracy test is activated after `degCheckInt` consecutive iterations;
     * if more than `degPrc`% iterations among the last `degCheckInt` ones were degenerated, objective is perturbed.
     * \return
     *   - `0` solution found;
     *   - `1` inconsistency detected;
     *   - `2` limit on number of iterates,  `maxItNum`, exceeded;
     *   - `3` objective function is less than `lowerBound`;
     *   - `4` time limit, `timeLimit`, exceeded.
     *   \throws CMemoryException lack of memory.
     *   \sa primeSimplex().
     */
    int dualSimplex(double lowerBound = -INF, int maxItNum=1000000000,
             __LONG timeLimit=0l, bool needSol=true, bool inconsistSertificate=false,
             int degCheckInt=200, int degPrc=30);

private:
    /**
     * The repeatedly calls `dualSimplex()` and `primeSimplex()` until prime and dual feasibility of the basis is achived.
     * \throws CMemoryException lack of memory
     */
    int solveProblem(bool bRowGen=false, bool bColGen=false);

    /**
     * 1) calls PRIME_InitSolution() to get an initial prime feasible basis;
     * 2)   solve LP by the prime simplex method;
     * 3) deletes all slack variables introduced by PRIME_InitBasis()
     */
    int PRIME_solve();

///////////////////////////////////////////////////////
// Initial Basis
///////////////////////////////////////////////////////
    bool INIT_changeSide(int col);
    double getGain(int row, bool side);
    int lpGreedy(int row, bool side);
    void crackBasis();

    /**
     * The procedure adds all equation constraints to the initial dual basis formed from the bounds on variables.
     */
    void EquationsToDualBasis();
    virtual void PRIME_InitBasis(); ///< Builds an initial LP prime feasible basis by introducing slack variables.
    virtual void DUAL_initBasis(); ///< Builds an initial LP dual feasible basis formed from the bounds on variables.

protected:
    /**
     * The procedure first computes an initial basis, and then implements an iterative procedure
     * that, until the current basic solution is prime and dual feasible,
     * applies either `primeSimplex()` or `dualSimplex()`.
     * \param[in] genFlag if `false`, both, inequalities and columns, are not generated.
     * \return 0 in case a solution has been found; otherwise, the return value is that of
     * `primeSimplex()` or `dualSimplex()` depending which of them returned a non-zero value.
     * \sa optimize().
     */
    int solveLP(bool genFlag=true);
///////////////////////////////////////////////////////
//       S E P A R A T I O N
///////////////////////////////////////////////////////
public:
    /**
     * Call this function to prevent generation of any inequalities.
     * \attention The function should be called before `openMatrix()`, which allocates memory.
     * Otherwise, unnecessary additional amount of memory will be allocated.
     */
    void switchOffRowGen()
        {m_uRowColGenRule&=COL_GEN;}

    void switchOnColGen()
        {m_uRowColGenRule&=~COL_GEN;} ///<  Call this function to allow generation of new columns.

protected:
    /**
     * This function is used to add new constraints to the closed matrix.
     * \param[in] hd handle of constraint;
	 * \param[in] type type of constraint;
	 * \param[in] b1,b2  respectively, left (LHS) and right (RHS) hand sides of constraint;
 	 * \param[in] sz number of non-zero entries;
	 * \param[in] dpVal, ipCol references to arrays of size `sz`, `dpVal[i]` is coefficient at variable `ipCol[i]`;
	 * \param[in] bVarScaled  if `true`, constraint is expressed in scaled variables;
 	 * \param[in] factor scale factor; `NOT_SCALED` value means that constraint is to be scaled by __MIPCL__ itself;
 	 * \param[in] n if non-zero, number of columns in original (non-preprocessed) problem;
 	 * in such a case the expressions in the preprocessing stack are used to substitute the variables `m_iN,...,n-1`;
 	 * \param[in] toBasis if `true`, data structures representing basis are slightly modified;
 	 * in rare cases when the matrix has been closed already, but optimization has not been started yet,
 	 *  `  toBasis` must be set to `false`.
 	 *  \return index of newly created constraint.
 	 * \throws CMemoryException lack of memory.
 	 * \attention The procedure adds the constraint to the matrix but not to the MIP pool.
 	 * \sa addRow(), getRow(), CMIP::genCut1(), CMIP::genCut2().
     */
    int addNewRow(tagHANDLE hd, unsigned type, double b1, double b2,
                  int sz, double* &dpVal, int* &ipCol,
                  bool bVarScaled=true, int factor=NOT_SCALED, int n=0,
                  bool toBasis=true);

    /**
     * The function may be useful for adding to the matrix several very similar constraints.
     */
    int dublicateRow(int row); ///< Adds to the matrix another copy of row `row`.

    /**
     *
     * \param[in] hd handle of the variable; if `hd >= 0`, then you must overload
     *  `getColumn()` (version with 11 parameters);
     * \param[in] type type of variable;
     * \param[in] cost objective coefficient;
     * \param[in] l,u lower and upper bounds of variable;
     * \param[in] sz number of nonzero entries;
     * \param[in] dpVal,ipRow arrays of size `sz`; `dpVal[i]` is coefficient in row `ipRow[i]`;
     * \param[in] side  if `true`, then in current basis variable takes value of `u`;
     *           otherwise, variable takes value of `l`;
     * \param[in] scaled if `true`, column is given in scaled variables;
     * \param[in] factor   - scale factor, set `factor=NOT_SCALED` if column must be scaled;
     * \param[in] flag set it to `true`, only if `addNewColumn()` is called within `generateColumns()`;
     * \return index of newly created column (variable).
     * \throws CMemoryException lack of memory.
     */
    int addNewColumn(tagHANDLE hd, unsigned type, double cost, double l, double u,
                      int sz, double* &dpVal, int* &ipRow, 
                      bool side, bool scaled=false, int factor=0, bool flag=false);

    /**
     * This function is helpful in branch-and-price algorithms when pricing columns that are not in the matrix.
     * \param[in] sz number of non-zero coefficients in column;
     * \param[in] dpVal,ipRow arrays of size `sz`; `dpVal[i]` is coefficient in row `ipRow[i]`.
     * \return if `c` is the cost coefficient of corresponding variable and `q` denotes return value, then
     * `c-q` is reduced cost of that variable.
     */
    double estimateCol(int sz, double* dpVal, int* ipRow);

    /**
     * Usually, this function is used for generating (adding to the matrix) strong inequalities, i.e.,
     * those that are a part of problem formulation, or those that are facet-defining cuts, and so on.
     * \param[in] n number of variables;
     * \param[in] dpX,ipColHd arrays of size `n`; `dpX[i]` is value of variable having handle `ipColHd[i]`;
     * \param[in] genFlag if `true`, all generated inequalities are added to matrix by calling `addNewRow()`
     *  (or `CMIP::addCut()` in MIP applications); otherwise,
     * none inequality should be added;
     * \return
     *   - `genFlag==true`: `true` if at least one inequality were generated; otherwise, `false`;
     *   - `genFlag==false`: `false` (!!!) if solution presented in `dpX,ipColHd`  is feasible,
 	 * or `true` if it is infeasible.
 	 * \throws CMemoryException lack of memory.
     * \remark Default implementation does nothing.
 	 * \sa addNewRow(), CMIP::genCut1(), CMIP::genCut2().
     */
    virtual bool separate(int n, const double* dpX, const tagHANDLE* ipColHd, bool genFlag);
//////////////////////////////////////////////////////////////////////////
//                           Column Generation                          //
//////////////////////////////////////////////////////////////////////////
    /**
     * This is used when working with preprocessed matrix;
     * otherwise, `getShadowPrices()` should be used.
     * \param[in] z vector of potentials;
     * \param[out] y array of size `m_iM`, `y[i]` dual variable for constraint `i`;
     * \param[in] bScaled if `true`, output `y` is computed for scaled problem; otherwise, for not-scaled.
     * \sa getShadowPrices().
     */
    void getDualRowVars(double* y, double* z, bool bScaled);

    /**
     * This function must be overloaded in any derived class that generates columns and stores them
     * in its own pool. If a new column is added to the matrix with a positive handle,
     * later, the solver may call `getColumn()` to reload this column to the matrix.
     * \param[in] hd handle of column to be returned;
     * \param[in] m number of rows in matrix;
     * \param[in] ipHd array of size `n`; `ipHd[i]` is handle of row `i`;
     * \param[out] type type of corresponding variable;
     * \param[out] cost objective coefficient;
     * \param[out] l,u  lower and upper bounds of corresponding variable;
     * \param[out] sz number of non-zero coefficients in column;
     * \param[out] dpVal, ipRow arrays of size `sz`; `dpVal[i]` is coefficient in row `ipRow[i]`.
      * \return `true` if column with handle `hd` was successfully restored; otherwise, `false`.
     * \sa addNewColumn().
     */
    virtual bool getColumn(tagHANDLE hd, int m, const tagHANDLE* ipHd,
                unsigned& type, double& cost, double& l, double& u,
                int& sz, double* dpVal, int* ipRow)
        {return false;}

    /**
     * This function must be overloaded in any derived class that generates rows (cuts) and stores them
     * in its own pool. If a new inequality is added to the matrix with a positive handle,
     * later, the solver may call `getRow()` to reload this inequality to the matrix.
     * \param[in] hd handle of constraint (row) to be returned;
     * \param[in] n number of columns in matrix;
     * \param[in] ipHd array of size `n`; `ipHd[j]` is handle of column `j`;
     * \param[out] type type of constraint;
     * \param[out] lhs,rhs  LHS and RHS;
     * \param[out] sz number of variables in constraint;
     * \param[out] dpVal, ipCol arrays of size `sz`; `dpVal[i]` is coefficient in column `ipCol[i]`.
     * \param[out] scaled if `true` the constraint (row) is returned in scaled variables; otherwise, `false`.
     * \return `true` if constraint with handle `hd` was successfully restored; otherwise, `false`.
     * \sa addNewRow().
     */
    virtual bool getRow(tagHANDLE hd, int n, const tagHANDLE* ipHd,
                        unsigned& type, double& lhs, double& rhs,
                        int& sz, double* dpVal, int* ipCol, bool &scaled)
        {return false;}

    /**
     * The function must be overloaded in user applications generating columns.
     * The functions is to produce a number of columns (presently not in the matrix)
     * with negative reduced costs defined to be the cost of the column minus the value
     * returned by `estimateColumn()`.
     * \param[in] m number of rows in matrix;
     * \param[in] ipRowHd,dpY arrays of size `m`; `dpY[i] ` is shadow price (value of dual variable)
     *  of constraint (row) having handle `ipRowHd`.
     * \return `true` if at least one column was generated; otherwise, `false`.
     * \attention The procedure must add columns to the matrix by calling `addNewColumn()`
     *  with parameter `flag` set to `false`.
     */
    virtual bool generateColumns(int m, const tagHANDLE* ipRowHd, const double* dpY);

    tagHANDLE getVarHandle(int j) const
        {return m_ipColHd[j];} ///< \return handle of variable indexed by `j`.

    tagHANDLE getCtrHandle(int i) const
        {return m_ipRowHd[i];} ///< \return handle of constraint indexed by `i`.

    /**
     * Overload this function to allow `CLP` to use symbolic constraint names when storing solutions.
     * \param[in] hd constraint (row) handle;
     * \param[out] name memory to store returned name;
     * \return pointer to `name` parameter.
     * \remark Default implementation composes the names as the concatenation
     *  of string "ctr_" with the value of `hd`.
     * \sa getVarName().
     */
    virtual char* getCtrName(tagHANDLE hd, char* name) const;

    /**
     * Overload this function to allow `CLP` to use symbolic names of variables when storing solutions.
     * \param[in] hd handle of variable (column);
     * \param[out] name memory to store returned name;
     * \return pointer to `name` parameter.
     * \remark Default implementation represents the name as the string "x(q)",
     *  where `q` is the value of `hd`.
     * \sa getCtrName().
     */
    virtual char* getVarName(tagHANDLE hd, char* name) const;

public:
    /**
     * \param[in] j index (not handle) of variable;
     * \param[in] l,u new lower and upper bounds for variable `j`.
     * \attention Changing bounds for variables may make the basis prime or dual infeasible.
     */
    void setVarBounds(int j, double l, double u);

    /**
     * \param[in] j index of a variable;
     * \param[in] l new lower bound for variable `j`.
     * \attention Changing bounds for variables may make the basis prime or dual infeasible.
     */
    void setVarLoBound(int j, double l);

    /**
     * \param[in] j index of a variable;
     * \param[in] u new upper bound for variable `j`.
     * \attention Changing bounds for variables may make the basis prime or dual infeasible.
     */
    void setVarUpBound(int j, double u);

    /**
     * The procedure removes both, lower and upper, bounds.
     * Sets the lower bound to minus infinity, and the upper to infinity.
     * \attention Removing  bounds for variables may make the basis dual infeasible.
     * \sa setVarBounds().
     */
    void setVarFree(int j)
        {m_dpD[j<<1]=-(m_dpD[(j<<1)+1]=m_dVarInf); m_ipVarType[j]&=~(VAR_LEFT | VAR_RIGHT | VAR_FX);}

    /**
     * The procedure sets both, lower and upper, bounds for a given constraint.
     * \param[in] i constraint index (not handle);
     * \param[in] lhs,rhs new left and right hand sides for constraint `i`.
     * \attention Changing bounds for constraints may make the basis prime or dual infeasible.
     */
    void setCtrBounds(int i, double lhs, double rhs);

    /**
     * The procedure sets the lower (left hand side) bound for a given constraint.
     * \param[in] i constraint index (not handle);
     * \param[in] lhs new left hand side for constraint `i`.
     * \attention Changing bounds for constraints may make the basis prime or dual infeasible.
     */
    void setLHS(int i, double lhs);

    /**
     * The procedure sets the upper (right hand side) bound for a given constraint.
     * \param[in] i constraint index (not handle);
     * \param[in] rhs new right hand side for constraint `i`.
     * \attention Changing bounds for constraints may make the basis prime or dual infeasible.
     */
    void setRHS(int i, double rhs);

    /**
     * The procedure sets the left hand side to minus infinity, and the right hand side to infinity.
     * \attention Removing  bounds for constraints may make the basis dual infeasible.
     * \sa setCtrBounds().
     */
    void setCtrFree(int i)
        {m_dpB[i<<1]=-(m_dpB[(i<<1)+1]=INF); m_ipCtrType[i]&=~(CTR_LEFT | CTR_RIGHT | CTR_EQ);}

protected:
    /**
     * \param[in] j index of variable;
     * \return `true` if variable `j` is _fixed_ (its lower and upper bounds are equal), otherwise, `false`.
     */
    bool isVarFixed(int j) const
        {return (m_ipVarType[j] & VAR_FX)? true: false;}

    /**
     * \param[in] j index of variable;
     * \return `true` if variable `j` is _free_, otherwise, `false`.
     */
    bool isVarFree(int j) const
        {return (m_ipVarType[j] & (VAR_LEFT | VAR_RIGHT))? false: true;}

    /**
     * \param[in] j index of variable;
     * \return `true` if variable `j` is _bounded_ (both its bounds, lower and upper, are finite), otherwise, `false`.
     */
    bool isVarBounded(int j) const
        {return ((m_ipVarType[j] & VAR_LEFT) && (m_ipVarType[j] & VAR_RIGHT))? true: false;}

    /**
     * The function is used to verify whether a variable is lower bounded.
     * \param[in] j index of variable;
     * \return `true` if variable `j` is _lower bounded_ (its lower bound is finite), otherwise, `false`.
     */
    bool isVarLoBounded(int j) const
        {return (m_ipVarType[j] & VAR_LEFT)? true: false;}  

    /**
     * The function is used to verify whether a variable is upper bounded.
     * \param[in] j index of variable;
     * \return `true` if variable `j` is _upper bounded_ (its upper bound is finite), otherwise, `false`.
     */
    bool isVarUpBounded(int j) const
        {return (m_ipVarType[j] & VAR_RIGHT)? true: false;}

    /**
     * The function is used internally in `CLP` and is overloaded in `CMIP`.
     * \param[in] j index of variable.
     * \return `true` if lower bound of variable `j` is _implied_, i.e., if it follows from constraints and other bounds on variables.
     */
    virtual bool isVarLbImplied(int j) const
        {return (m_ipVarType[j] & VAR_LB_IMPL)? true: false;}

    /**
     * The function is used internally in `CLP` and is overloaded in `CMIP`.
     * \param[in] j index of variable.
     * \return `true` if upper bound of variable `j` is _implied_, i.e., if it follows from constraints and other bounds on variables.
     */
    virtual bool isVarUbImplied(int j) const
        {return (m_ipVarType[j] & VAR_UB_IMPL)? true: false;}

    /**
     * The function resets all the variable bounds as non-implied,
     *  and then recomputes all implications from scratch.
    */
    void setImpliedVarBounds();

/////////////////////////////////////////////////////////////////////////////
    /**
     * The function is used to verify whether a constraint is an equality constraint.
     * \param[in] i index of constraint;
     * \return `true` if constrain `i` is _equality_ (its left  and right hand are equal), otherwise, `false`.
     */
   bool isCtrEq(int i) const
        {return (m_ipCtrType[i] & CTR_EQ)? true: false;}

   /**
    * The function is used to verify whether a constraint is _free_.
    * \param[in] i index of constraint;
    * \return `true` if constrain `i` is _free_, otherwise, `false`.
    */
    bool isCtrFree(int i) const
        {return (m_ipCtrType[i] & (CTR_LEFT | CTR_RIGHT))? false: true;}

    /**
     * The function is used to verify whether a constraint is two side bounded.
     * \param[in] i index of constraint;
     * \return `true` if constrain `i` is _bounded_ (its left  and right hand are finite), otherwise, `false`.
     */
    bool isCtrBounded(int i) const
        {return ((m_ipCtrType[i] & CTR_LEFT) && (m_ipCtrType[i] & CTR_RIGHT))? true: false;}

    /**
     * The function is used to verify whether a constraint is lower bounded.
     * \param[in] i index of constraint;
     * \return `true` if constrain `i` is _lower bounded_ (its left hand is finite), otherwise, `false`.
     */
    bool isCtrLoBounded(int i) const
        {return (m_ipCtrType[i] & CTR_LEFT)? true: false;}

    /**
     * The function is used to verify whether a constraint is upper bounded.
     * \param[in] i index of constraint;
     * \return `true` if constrain `i` is _upper bounded_ (its right hand is finite), otherwise, `false`.
     */
    bool isCtrUpBounded(int i) const
        {return (m_ipCtrType[i] & CTR_RIGHT)? true: false;}

private:
    /**
     * We say that a bound (lower or upper) on variable is _implied_ by a constraint
     * if it can be derived by preprocessing this constraint.
     * For a given constraint, the function computes all implied bounds, and,
     * if for  some variable the implied bound bound is better than the current bound on this variable,
     * the variable bound is replacesed by the implied bound.
     * \param[in] row index of constraint.
     */
    void setCtrImpliedVarBounds(int row);

/////////////////////////////////////////////////
//   Access the solution
/////////////////////////////////////////////////
public:
    /**
     * The function resumes the state in which the problem was before the solution procedure has started.
     */
    virtual void reset();

    /**
     * The procedure solves LPs.
     * \param[in] timeLimit limit on solution time (in seconds);
     * \param[in] gap integrality gap;
     * \param[in] solFileName pointer to string with file name for storing intermediate solutions.
     * \remark When solving LPs, input parameters are not used.
     * \sa CMIP::optimize().
     */
    virtual void optimize(__LONG timeLimit=1000000l, double gap=0.0, const char *solFileName=0);

    /**
     * \return `true` if LP (MIP) in memory has been prepared for optimization.
     */
    bool isPrepared() const
    {return (m_iState & PROB_PREPARED)? true: false;}

    /**
     * \return `true` if LP (or MIP) in memory has been solved.
     */
    bool isSolved() const
    {return (m_iState & PROB_SOLVED)? true: false;}
    
    /**
     * \return `true` if LP in memory has been solved and optimal solution found.
     * \sa CMIP::isSolution().
     */
    virtual bool isSolution() const
        {return (m_iState & PROB_SOLVED) && m_bPrimeFeasible && m_bDualFeasible;}

    /**
     * \return `true` if LP in memory has been solved and proven to be _infeasible_.
     * \sa whyLpInfeasible(), showWhyLpInfeasible().
     */
    bool isLpInfeasible() const
    {return (m_iS != 0) && m_bDualFeasible;}

    /**
     * \return `true` if LP in memory has been solved and proven to be _unbounded_.
     * \sa whyLpUnbounded(), showWhyLpUnbounded().
     */
    bool isLpUnbounded() const
        {return (m_iS >= 0) && m_bPrimeFeasible;}

    /**
     * This function is usually called when the problem has been solved already.
     * \return optimal objective value of solved LP.
     */
    double getObjVal() const;

    /**
     * The function returns two pointers to the internal MIPCL arrays storing the LP solution.
     * \param[out] dpX,ipHd `dpX[j]` is value of variable whose handle is `ipHd[j]`, `j=1,...,n`,
     *    where `n` is return value.
     * \return number of variables.
     * \attention Do not modify the values of both arrays  `dpX` and `ipHd`.
     * \sa CMIP::getSolution().
     */
    int getSolution(double* &dpX, int* &ipHd);

    /**
     * The function returns _reduced costs_ of variables for the solved LP.
     * \param[out]  dpC,ipHd - arrays of size `n`, where `n` denotes return value;
     * `dpC[i]` is reduced cost of variable with handle `ipHd[i]`.
     * \return number of variables.
     * \remark If `getReducedCosts()` is called with `dpC` and/or `ipHd` set to zero,
     * on return `dpC` and/or `ipHd` are pointers to internal `CLP` arrays.
     */
    int getReducedCosts(double* &dpC, int* &ipHd);

    /**
     * The function returns constraint _shadow prices_ (optimal values of dual variables).
     * \param[out] dpP,ipHd arrays of size `m`, where `m` is return value;
     *  `dpP[i]` is shadow price of constraint with handle `ipHd[i].
     * \return number of constraints.
     */
    int getShadowPrices(double* &dpP, int* &ipHd);

    /**
     * According to Farkas Lemma, a system of inequalities is infeasible
     * if can get the wrong inequality `0 < -1` by multiplying these inequalities by some numbers (multiplies)
     * --- of course,if we multiply an inequality by a negative number, we also have to change the sign of that inequality) ---
     * an then sum up the results. A collection of such multipliers is known as a _sertificate_ of inconsistency.
     *
     * `whyLpInfeasible()` returns a certificate of inconsistency.
     *
     * \param[out] m number of constraints;
     * \param[out] dpYctr,ipRowHd arrays of size `m`; if `dpYctr[i] > 0` (`dpYctr[i] < 0`),
     * then right (left) part of constraint with handle `ipRowHd[i]`
     * belongs to the set of contradicting constraints;
     * \param[out] n number of variables;
     * \param[out] dpYbd,ipColHd arrays of size `n`;
     * if `dpYbd[i] > 0` (`dpYbd[i] < 0`), then right (left) bound of variable
     * with handle `ipColHd[i]` belongs to the set of contradicting constraints.
     * \sa isLpInfeasible(), showWhyLpInfeasible().
     * \attention 1.  Call `whyLpInfeasible()` only if `isLpInfeasible()` returns `true`.
     *
     * 2. If the value of any input parameter is `0`,
     * then the output value of this parameter points to an internal MIPCL buffer.
     * Therefore, calling two function in sequence, keep in mind that the second call
     * may override the values returned by the first call.
     * This is not true for the handle-parameters, you can always
     * set initial values for these parameters to `0`.
     *
     * If you do not allocate memory to some input parameter and its value is not `0`,
     * then the behavior of the calling function is not predicted!!!
     */
    void whyLpInfeasible(int &m, int* &ipRowHd, double* &dpYctr,
                          int &n, int* &ipColHd, double* &dpYbd);

    /**
     * The function simply writes to the output stream the result returned by `whyLpInfeasible()`.
     * \param[in] out output file name.
     * \sa whyLpInfeasible(), showWhyLpInfeasible(const char* fileName).
     * \attention Call `showWhyLpInfeasible()` only if `isLpInfeasible()` returns `true`.
     */
    void showWhyLpInfeasible(std::ostream &out);

    /**
     * The function simply writes to the file the result returned by `whyLpInfeasible()`.
     * \param[in] fileName output file name.
     * \sa whyLpInfeasible(), showWhyLpInfeasible(std::ostream &out).
     * \attention Call `showWhyLpInfeasible()` only if `isLpInfeasible()` returns `true`.
     */
    void showWhyLpInfeasible(const char* fileName);

    /**
     * An LP is unbounded if the there exists a ray such that all its points are
     *  feasible solutions to the system of LP inequalities.
     * This procedure returns such a ray.
     * \param[out] n number of columns;
     * \param[out] dpX,dpRay arrays of size `n`;
     * `dpX + lambda * dpRay` is feasible point for all `lambda >= 0`;
     * \param[out] ipColHd arrays of size `n`; variable with handle `ipColHd[i]` has index `i`.
     * \sa whyLpInfeasible().
     * \attention Call `whyLpUnbounded()` only if `isLpUnbounded()` returns `true`.
     */
    void whyLpUnbounded(int &n, double* &dpX, double* &dpRay, int* &ipColHd);

    /**
     * The function simply writes to the file the result returned by `whyLpUnbounded()`.
     * \param[in] fileName output file name.
     * \sa whyLpUnbounded().
     * \attention Call `showWhyLpInfeasible()` only if `isLpUnbounded()` returns `true`.
     */
    void showWhyLpUnbounded(const char* fileName);

	/**
	 * The function writes LP solutions into the file.
	 * The user can overload this function to store solutions in an appropriate way.
	 * \param[in] fileName name of the file to store solutions; if `fileName=`0`,
	 * the solver makes up the file name by appending the extension ".sol" to the name of the problem being solved.
	 * \throws CFileException.
	 */
    virtual void printSolution(const char *fileName=0);

protected:
    /**
     * The function is usually called in MIP applications when an MIP is being solved.
     * Even when solving LPs, the values returned by `getObjVal()` and `getLPObjVal()` will differ
     * in sign if the objective is minimized. This happens because, in cases when the objective is minimized,
     *  the solver changes the signs of all objective coefficients.
     * \return non-scaled objective value for current basic solution.
     * \sa getObjVal().
     */
//    double getLPObjVal();

    /**
     * Usually, the function is used in MIP applications.
     * \param[in] val objective value of some solution in original (not-scaled) variables.
     * \return objective value of that solution in scaled variables.
     * \sa getNotScaledObjVal().
     */
    double getScaledObjVal(double val) const;

    /**
     * Usually, the function is used in MIP applications.
     * \param[in] val objective value of some solution in scaled variable.
     * \return objective value of that solution in original (not-scaled) variables..
     * \sa getScaledObjVal().
     */
    double getNotScaledObjVal(double val) const;

    /**
     * The function is used privately in CMIP.
     * \return the number of times the basic matrix has been refactored
     *  during last call to `primeSimplex()` or `dualSimplexthis()`.
     */
    int getPartitionNum() const
    	{return m_iPartitionNum;}


///////////////////////////////////////////////////
//  Serialization
///////////////////////////////////////////////////
private:
    void serializeBasis(std::fstream& ar, bool is_storing);
    virtual void serializeMatrix(std::fstream& ar, bool is_storing);
    virtual void serializeTolVars(std::fstream& ar, bool is_storing);
    virtual void serializeFlags(std::fstream& ar, bool is_storing);

protected:
    /**
     * The function stores into (or restores from) the stream `ar`
     * CLP objects (all its member storing permanent data).
     * Derived classes may overload this function to store additional information.
     * In such a case, call first `serialize()` of the base class.
     * \param[in] ar reference to a stream;
     * \param[in] is_storing if `true`, the object is written to the stream;
     *  otherwise, the object is restored from the stream.
     *  \sa CMIPP::serialize().
     */
    virtual void serialize(std::fstream& ar, bool is_storing);

/////////////////////////////////////////////////////
// Statistics
/////////////////////////////////////////////////////
    /**
     * \return number of iterates performed by `primeSimplex()` or `dualSimplex()`.
     */
    int getLPItNum() const
        {return m_iItNum;}

    /**
     * \return number of degenerate iterates performed by `primeSimplex()` or `dualSimplex()`.
     */
    int getDegItNum() const
        {return m_iDegItNum;}

    /**
     * The function is to compose a string describing the problem being solved.
     * \param[out] str memory for output string;
     * \return pointer to output string.
     * \remark If `str=0` when calling `getProbStatStr()`, on return `str` points to
     * an internal `CLP` array.
     */
    virtual char* getProbStatStr(char *str=0);

    /**
     * The function prints messages to the standard output stream.
     * This function should be overloaded in applications with graphic user interfaces (GUI).
     * \param[in] msg message string;
     * \param[in] level if set to 1) 0 -  info message; 2) 1 - warning message; 3) 2 - error message.
     * \remark Warning and error messages are also printed to LOG stream.
     */
    virtual void infoMessage(const char* msg, int level=0);

    /**
     * The function prints into the standard output stream a message string
     * which describes the current state of the solution process when
     * running `primeSimplex()` or `dualSimplex()`.
     * When developing an application with a GUI interface,
     * the user may wish to overload this function.
     * \param[in] method string describing simplex method that is currently running;
     * \param[in] time string representation of time elapsed since application start;
     * \param[in] itNum number of simplex-iterates;
     * \param[in] degItNum number of degenerate simplex-iterates;
     * \param[in] objVal current objective value.
     */
    virtual void lpInfo(const char *method, const char *time, int itNum, int degItNum, double objVal);

private:
    /**
     * The function prepares parameter for calling `lpInfo()` and then calls it.
     */
    void __lpInfo();

public:
// Debugging tools

    /**
     * The function is useful for debugging. It prints a given row of the matrix to the standard error stream.
     * \param[in] i row index;
     * \param[in] scaled if `true`, row of scaled matrix is printed; otherwise, not-scaled;
     * \param[in] varValues if `true`, current values of variables that occur in row are also printed.
     */
   void printRow(int i, bool scaled=true, bool varValues=false);

   /**
    * The function is used for debugging. It prints the constraint `sum(i=0,..,sz-1) dpVal[i]*x(ipCol[i]) <= (>=) b`
    *  given given by its argument to the standard error stream.
    * \param[in] sz size of arrays `dpVal` and `ipCol`;
    * \param[in] dpVal array of size `sz` storing coefficients;
    * \param[in] ipCol array of size `sz` storing indices of variables;
    * \param[in] b depending on value of `side`, right-hand-side or left-hand-side of inequality to be printed;
    * \param[in] side if `true`, the inequality sign is `<=`; otherwise, the sign is `>=`.
    */
   void printCtr(int sz, double* dpVal, int* ipCol, double b,  bool side=true);

   /**
    * The function is useful for debugging. It prints a given column of the matrix to the standard error stream.
    * \param[in] j column index.
    */
    void printColumn(int j);

    /**
     * The function is useful for debugging. Prints a the matrix to the file.
     * \param[in] fileName file name;
     * \param[in] scaled if `true`, scaled matrix is printed; otherwise, not-scaled;
     * \throws CFileException.
     */
    void printMatrix(const char* fileName, bool scaled=false);

#ifdef __PYTHON_MODULE_
    /**
     * To make communication between __Python__ and __MIPCL__
     * easier, `setSolution()` lists components of the prime and dual solutions
     * in order of their handles (initial indices).
     */
    void setSolution();

    /**
     *  When the problem has been proven to be inconsistent,
     * this procedure is called to set an inconsistency certificate.
     *
     */
    void setInconsistCertificate();

    /**
     * This function must be called only after calling `setSolution()`.
     * \param[in] ind index (handle) of variable.
     * \return value of variable indexed by `ind`.
     */
    double getOptVarValue(int ind) const;

    /**
     * This function must be called only after calling `setSolution()`.
     * \param[in] ind constraint index.
     * \return shadow price of constraint indexed by `ind`.
     */
    double getShadowPrice(int ind) const;

#endif


};

#endif // #ifndef __LP__H
