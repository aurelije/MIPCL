///////////////////////////////////////////////////////////////
/**
 * \file cmip.h Interface for `CMIP` class
 * |  __Author__  | N.N. Pisaruk                              |
 * |-------------:|:------------------------------------------|
 * |  __e-mail__  | nicolaipisaruk@gmail.com                  |
 * | __home page__| wwww.mipcl-cpp.appspot.com                |
 *
 * \copyright __2015 Nicolai N. Pisaruk__
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

#ifndef __CMIP__H
#define __CMIP__H

#include "lp.h"

#ifdef _WINDOWS
#ifndef MIP_API
#ifdef MIP_EXPORTS
#define MIP_API __declspec(dllexport)
#else
#define MIP_API __declspec(dllimport)
#endif // MIP_EXPORTS
#endif // MIP_API
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
#endif // MIP_API
typedef long __LONG; ///< For GNU C++, `long` means 64 bit integer.
#endif // _WINDOWS

#ifndef __PSEUDOCOSTS_
#define __PSEUDOCOSTS_ ///< switches on computing pseudocosts.
#endif

class CPool;
class CNode;
class CTree;
class CRecord;
class CImpl;

 /// Class `CMIP` has been designed for solving Mixed Integer Programs (MIPs)
 /**
 * \f{align*}{
 *     & c^Tx \to \max,\\
 *     & b_1 \le Ax \le b_2,\\
 *     & l \le  x \le u,\\
 *     & x_i\in\mathbb{Z}\quad\forall\; i\in I,
 * \f}
 * by the branch-and-cut or branch-and-price method.
 *
 * __Examples__ of how to use `CMIP` class:
 *    - \ref primer;
 *    - \ref fcnf;
 *    - \ref infLP;
 *    - \ref subgraph;
 *    - \ref MSched;
 *    - \ref gap;
 *    - \ref tsp
 */
class MIP_API CMIP: public CLP
{
	friend class CTree;
	friend class CImpl;

public:
	/// MIP types of constraints.
	/**
	 * The type of a constraint is the bitwise OR of the members of two enumerations CLP::enCtrType and CMIP::enCtrType.
	 * A constraint is of a particular type if it can be transformed to that type by complementing some binary variables.
	 */
	enum enCtrType {
		CTR_LOCAL            = 0x00000020, ///< The constraint is _local_ for node being processed.
		CTR_INT_VARS         = 0x00000040, ///< All variables and coefficients are integral in this constraint.
		CTR_INT_COEFF        = 0x00000080, ///< All coefficients are integral in this constraint.
		CTR_INT              = CTR_INT_VARS|CTR_INT_COEFF, ///< The constraint with all variables and coefficients being integral.
		CTR_VAR              = 0x00000100, ///< _Variable bound_, i.e. \f$z_i \le (\ge) a_jx_j +b,\quad z_i\in\mathbb{R},\; x_j\in \{0,1\}\f$.
		CTR_BINPACK          = 0x00000200, ///< _Bin packing_, i.e., \f$\sum_{i\in I} a_i x_i + a_j x_j \le a_j,\quad j\not\in I, x_j\in\{0,1\},\; x_i\in\{0,1\}\;\forall\: i\in I\f$.
		CTR_KNAPSACK         = 0x00000400, ///< \f$\sum_{i\in I} a_i x_i \le b,\quad x_i\in\{0,1\}\;\forall\: i\in I\f$.
		CTR_MX_KNAPSACK      = 0x00000800, ///< \f$\sum_{i\in I} a_i x_i + \sum_{j\in J} q_j z_j \le b,\quad x_i\in\{0,1\}\;\forall\: i\in I\f$.
		CTR_PACKING          = 0x00001000, ///< \f$\sum_{i\in I} x_i \le 1,\quad x_i\in\{0,1\}\;\forall\: i\in I\f$.
		CTR_INV_KNAPSACK     = 0x00002000, ///< \f$\sum_{i\in I} x_i \le b,\quad x_i\in \{0,1\}\;\forall\: i\in I\f$.
		CTR_COVERING         = CTR_INV_KNAPSACK, ///< \f$\sum_{i\in I} x_i \ge 1,\quad x_i\in\{0,1\}\;\forall\: i\in I\f$.
		CTR_CARDINALITY      = 0x00004000, ///< \f$\sum_{i\in I} x_i \le k,\quad x_i\in\{0,1\}\;\forall\: i\in I\f$.
		CTR_GUB              = CTR_PACKING|CTR_CARDINALITY, ///< \f$\sum_{i\in I} x_i \le 1,\quad x_i\in\{0,1\}\;\forall\: i\in I\f$.
		/**
		 * A __SOS1__-constraint is a __GUB__-constraint (of type `CTR_GUB`) such that
		 *  a) all its variables are present in exactly the same other constraints,
		 *  and b) in any of those constraints, if we list the coefficients of SOS1-variables in the order
		 *  these variables are present in the base SOS1-constraint, we get an increasing or decreasing sequence.
		 */
		CTR_SOS1             = 0x00008000, ///< \f$\sum_{i\in I} a_i z_i \le (=) b,\quad z_i \ge 0\;\forall\: i\in I \text{ and only one } z_i \text{ takes non-zero value} \f$.
		CTR_SOS2             = 0x00010000, ///< \f$\sum_{i\in I} a_i z_i \le (=) b,\quad z_i \ge 0\;\forall\: i\in I,\text{ and if } z_i> 0,\: z_j>0, \text{ then } |i-j| = 1\f$.
		CTR_01FLOW           = 0x00020000, ///< \f$\sum_{i\in I} a_i z_i \le (=) b,\quad 0\le z_i \le \alpha_ix_i +\beta_i \text{ with } x_i\in \{0,1\},\; i\in I\f$.
		CTR_WITH_VAR_BOUNDS  = 0x00040000, ///< \f$\sum_{i\in I} a_i z_i \le (=) b,\quad 0\le z_i \le a_ix_i +b_i \text{ with } x_i\in\{0,1\},\; i\in J\subset I\f$.
		CTR_WITH_UNIQUE      = 0x00080000, ///< Any constraint with only one non-binary variable.
		CTR_PARITY           = 0x00100000, ///< Any integer constraint with more than two odd coefficients.
		CTR_MX_01            = 0x00200000, ///< The constraint has binary and non-binary variables.
		CTR_MX_INT           = 0x00400000, ///< The constraint has integer and non-integer variables.
		CTR_GEN              = 0x00400000, ///< All other constraints (currently not used).
		CTR_NOT_INV          = 0x00800000, ///< Absolute coefficient values of integer variables are not of equal value.
		CTR_BRANCHING_INV    = 0x01000000, ///< Invariant knapsack used in balancing branching.
		CTR_WITH_DEP_BINS    = 0x02000000, ///< There are implication relations, \f$x_i=a\; \Rightarrow\; x_j=b,\; (a,b \in \{0,1\}\f$), between their binary variables.
		CTR_NOT_STABLE       = 0x04000000, ///< With too big or too small coefficients, or has been derived from non-stable constraints.
		CTR_IN_POOL          = 0x08000000, ///< The constraint is stored in pool.
//		CTR_DISJUNCTION      = 0x10000000, ///< The constraint contains binary variables setting which to one of its bounds (0 or 1) makes this constraint _free_.
		CTR_OBJ              = 0x20000000, ///< The constraint represents the objective, its lhs and rhs are lower and upper bounds on the optimal objective value.
        CTR_LB_UNBOUNDED     = 0x40000000, ///< The flag is _privately_ used in preprocessing subroutines.
    	CTR_UB_UNBOUNDED     = 0x80000000, ///<The flag is _privately_ used in preprocessing subroutines.
		CTR_BINARY           = CTR_BINPACK | CTR_KNAPSACK | CTR_INV_KNAPSACK | CTR_PACKING | CTR_COVERING | CTR_CARDINALITY, ///< all variables are binaries.
		CTR_WITH_INT_VARS    = CTR_INT_VARS | CTR_MX_01 | CTR_MX_INT | CTR_MX_KNAPSACK, ///The constraint includes integer variables.
		CTR_MIR              = CTR_MX_INT|CTR_MX_01|CTR_MX_KNAPSACK|CTR_WITH_VAR_BOUNDS|CTR_01FLOW|CTR_WITH_UNIQUE|CTR_KNAPSACK|CTR_BINPACK, ///< Mixed Integer Rounding (MIR) constraint
		CTR_FULLY_CLASSIFIED = CTR_VAR|CTR_BINPACK|CTR_KNAPSACK |CTR_INV_KNAPSACK|CTR_PACKING|CTR_COVERING|CTR_PARITY|CTR_SOS1|CTR_SOS2, ///< mask of all fully classified constraints
		CTR_CLASSIFIED = CTR_MIR|CTR_01FLOW|CTR_INT|CTR_VAR|CTR_INV_KNAPSACK |CTR_PACKING|CTR_COVERING|CTR_CARDINALITY|CTR_PARITY|CTR_BINPACK|CTR_BRANCHING_INV|CTR_WITH_DEP_BINS|CTR_NOT_INV ///< mask of automatically assigning flags
	};

	/// MIP types of variables.
	/**
	 * The type of a variable is the bitwise OR of the members of two enumerations, `CLP::enVarType` and `CMIP::enVarType`.
	 */
	enum enVarType {
//		VAR_INT           = 0x00001000, ///< Integral variable.
		VAR_BIN           = 0x00002000, ///< Binary variable.
		VAR_4_INT         = 0x00004000, ///< Both bounds, lower and upper, are in [0,15].
		VAR_8_INT         = 0x00008000, ///< Both bounds, lower and upper,  are in [0,255].
		VAR_IN_VAR_LB     = 0x00010000, ///< The non-binary variable occurs in some lower variable bound constraint.
		VAR_0_IN_VAR_CTR  = VAR_IN_VAR_LB, ///< If binary variable is set to `0`, then some variable bound constraint becomes a restriction.
		VAR_IN_VAR_UB     = 0x00020000, ///< The variable occurs in some upper variable bound constraint.
		VAR_1_IN_VAR_CTR  = VAR_IN_VAR_UB, ///< If the binary variable is set to `1`, then some variable bound constraint becomes a restriction.
		VAR_IN_VAR_CTR    = VAR_IN_VAR_UB | VAR_IN_VAR_LB, ///< The variable occurs in either lower, or upper variable bound constraint.  \sa CTR_VAR.
		VAR_IN_GUB        = 0x00040000, ///< The variable occurs in _GUB_ constraints. \sa CTR_GUB.
		VAR_IN_PACKING    = 0x00800000, ///< The variable occurs in _packing_ constraints. \sa CTR_PACKING.
		VAR_SOS2          = 0x00080000, ///< The variable occurs in _SOS2_ constraint. \sa CTR_SOS2.
		VAR_UNIQUE        = 0x00100000, ///< The non-binary variable that occurs in constraints which all other variables are binary.
		VAR_MON_UP        = 0x00200000, ///< The variable can be increased up to its upper bound without violating feasibility.
		VAR_MON_DOWN      = 0x00400000, ///< The variable can be decreased up to its lower bound without violating feasibility.
		VAR_MONOTONE      = VAR_MON_UP | VAR_MON_DOWN, ///< The variable is monotone down or up.
		VAR_BINPACK       = 0x00000000, ///< The variable occurs in _bin-packing_ constraint (currently is not used). \sa CTR_BINPACK.
		VAR_IN_POOL       = 0x08000000, ///< The column corresponding to this variable is stored in the pool.
//		VAR_DISJUNCTION   = 0x08000000, ///< The binary variable such that setting it to one of its bounds (`0` or `1`) makes _free_ at least one constraint. \sa CTR_DISJUNCTION1.
		VAR_CLASSIFIED    = VAR_IN_VAR_CTR|VAR_IN_PACKING|VAR_IN_GUB|VAR_MONOTONE|VAR_UNIQUE|VAR_IN_POOL|VAR_BINPACK, ///< mask of automatically assigned flags.
		VAR_PRI_MIN       = -50, ///< The priority of any variable is an integer from `VAR_PRI_MIN` to `VAR_PRI_MAX`.
		VAR_PRI_MAX       = 50 ///< The priority of any variable is an integer from `VAR_MIN_PRI` to `VAR_MAX_PRI`.
	};

	/// Branching rules.
	/**
	 * For each integer-valued variable \f$j\f$ with fractional value \f$x_j\f$, we estimate from below
	 *    - \f$\lambda^{\mathrm{down}}_j\f$: decrease  of the objective if we branch by this variable down;
	 *    - \f$\lambda^{\mathrm{up}}_j\f$: decrease  of the objective if we branch by this variable up.
	 *
	 * Then we compute the score \f$\lambda_j\f$ as a function of \f$\lambda^{\mathrm{down}}_j\text{ and } \lambda^{\mathrm{up}}_j\f$,
	 * say, \f$\lambda_j=\lambda^{\mathrm{down}}_j \cdot \lambda^{\mathrm{up}}_j\f$, or \f$\lambda_j=\min{\lambda^{\mathrm{down}}_j,\lambda^{\mathrm{up}}_j}\f$.
	 * A variable \f$j^*\f$ with maximum score is selected for doing branching.
	 */
	enum enBranchRule {
		/// Strong branching (default strategy).
		/**
		 * A candidate set of fractional variables is selected;
		 * for every candidate \f$x_j\f$, for each of 2 branches a fixed number (approximately 10)
		 * of dual iterates are performed and the decrease of the objective value is fixed;
		 * the product of these two decreases is the score.
		 */
		STRONG_BR,
		/// Score branching.
		/**
		 * Let\f$\lambda^{\mathrm{down}}_j\f$ (resp., \f$\lambda^{\mathrm{up}}_j\f$) be the decrease of the objective
		 * after performing one dual iterate; the minimum of these two decreases is the score.
		 */
		MAX_SCORE
	};

	/// Rounding rules.
	/** Specifies all possible values of m_eRoundType, which determines
	 * how non-integral solutions are rounded.
	 * if m_eRoundType != ROUND_NONE, at each node, an optimal solution is rounding
	 * in hope to get a feasible solution.
	 */
	enum enRoundType {
		ROUND_NONE, ///< none of rounding procedures is called
		ROUND_OFF, ///< round off values of all integral variables
		ROUND_DOWN, ///< round down values of all integral variables
		ROUND_UP, ///< round up values of all integral variables
		ROUND_USER ///< user defined procedure is called for rounding integer variables
	};

	/// Cut types.
	enum enCutType {
		CUT_TYPE_NUM           =13, ///< Number of cut types (classes).
		_CLICK                  =0, ///< Click cuts.
		_ODD_CYCLE              =1, ///< Odd cycle cuts.
		_KNAPSACK               =2, ///< Cuts generated for knapsack constraints.
		_MX_KNAPSACK            =3, ///< Generalized cover cuts generated for mixed-knapsacks.
		 _MIR                   =4, ///< MIR-cuts.
		_FLOW_COVER             =5, ///< Flow-cover cuts generated by 0,1-flow constraints.
		_SPARSE_MOD2            =6, ///< Sparse (0,1/2)-Chvatal-Gomory (mod2) cuts.
		_DENSE_MOD2             =7, ///< Dense (0,1/2)-Chvatal-Gomory (mod2) cuts.
		_SPARSE_GOMORY          =8, ///< Sparse mixed-integer Gomory cuts.
		_DENSE_GOMORY           =9, ///< Dense mixed-integer Gomory cuts.
		_PARITY                =10, ///< Parity cuts.
		_VAR_BOUND             =11, ///< Implied variable bounds.
		_SIMPLE_DJ             =12, ///< Simple disjunctions.
		CUTS_TO_POOL	       = 0  ///< The mask of cuts send to the pool.
	};

	/// Extension of `enRowColGenRule`.
    enum enRowColGenRule2 {
    	USER_GEN1_PROC	=0x8, ///< If `genCut1()` has been overloaded.
    	USER_GEN2_PROC	=0x10, ///< If `genCut2()` has been overloaded.
    	STOP_AUTO_CUTS  =0x20, ///< If the flag is set, generation of  auto cuts is stopped.
    	WITH_AUTO_CUTS  =0x40, ///< `autoCuts()` was not called at the previous call to `generateCuts()`.
    	NO_SOLVER_DECISIONS = 0x80 ///< If user resets any parameter affecting cut generation, _MIPCL_ does not change standard settings of all parameter affecting cut generation.
    };


/////////////////////////////////////////////////////////////
protected:
	/**
	 * This array may be freely used in user defined cut or column generating procedures.
	 */
	double *m_dpFd; ///< Auxiliary array of size max{`2*m_iNmax`}.
	int *m_ipFd; ///< Auxiliary array that shares memory with `m_dpFd'.
	/**
	 * This array may be freely used in user defined cut or column generating procedures.
	 */
	double *m_dpFb; ///< Auxiliary array of size `m_iMax+m_iNmax`.
private:
	static const char m_cpCutName[CUT_TYPE_NUM][16]; ///< names of cuts

    enSepRule m_eNodeSepRule; ///< Separation rule used when solving not-root node LPs.
    enPricingRule m_eNodePricingRule; ///<< Pricing rule used when solving not-root node LPs..

    int m_iVarLbNum;///< Number of variable lower bounds.
    int m_iVarUbNum; ///< Number of variable upper bounds.
    int m_iIntNum;  ///< Number of integer variables.
    int m_iBinNum; ///< Number of binary variables.
    int m_iFracVarNum; ///< Number of fractional components in the current basic solution.
    int m_iParityNum; ///< Number of parity constraints.

    int m_iProbingDepth; ///< Depth of probing search, default value is `PROBING_DEPTH`.
    int m_iProbingDepthAtNodes; ///< Default value `PROBING_DEPTH_AT_NODES`.

    CImpl *m_pImpl; ///< Class that supports logical implications.
////////////////////////////////////////////////////
    /**
     * Array of size `2*m_iN`,
     * `m_dpD0[j<<1]` and `m_dpD0[(j<<1)+1]` are lower and upper bound of variable `j`.
     */
	double *m_dpD0; ///< Bounds of variables at the root node.

//////// Branching ///////////////////////////////////////////////////
    enBranchRule m_eBranchRule; ///< Currently used branching rule.

    int m_iBrCol; ///< Variable selected for branching.
    double m_dBrVarVal; ///< Value of variable selected for branching.

    int m_iBrRow; ///< _GUB_-constraint selected for branching.
	int m_iFxNum; ///< number of variables fixed in `strongBranching`.
	int *m_ipFxVar; ///< array of size 4*STRONG_BR_LOW_NODE_CAND_NUM used in `GUBbranching` and `strongBranching`.

#ifdef __PSEUDOCOSTS_
    /**
     * `m_dpBrDec[j<<1]` (resp., `m_dpBrDec[(j<<1)+1]`) is total relative objective decrease
     * when branching by variable `j` rounding it down (resp., up)
     */
	double *m_dpBrDec;

	/**
	 * An array of size `3*m_iN`;
     * For `j=0,...,m_iN-1`,
     *     - `m_ipBrNum[3*j]`: two lower (upper) bytes store the number of consistent (node LP remains consistent) branchings when variable `j` was rounded down (resp., up);
     *     - `m_ipBrNum[3*j+1]`: two lower (upper) bytes store the number of successful branchings (node LP remains consistent and objective has increased)
     *     - `m_ipBrNum[3*j+2]`: two lower (upper) bytes store the number inconsistent branchings (node LP became inconsistent) when variable `j` was rounded down (resp., up);
     *       (node LPs were inconsistent or the objective increased when variable `j` was rounded down (resp., up).
	 */
	unsigned *m_ipBrNum;
#endif

////////  Diving   ///////////////////////////////////////////////////
	int m_iDivingCalls; ///< Number of times `dive()` has been called so far.
	int m_iToughDivingCalls; ///< If positive, number of times `dive()` exceeds time limit; negative value indicates `dive()` is called rarely.
//////////////////////////////////////////////////////////////////////////
//                        POOL to store generated cuts                  //
//////////////////////////////////////////////////////////////////////////
    CPool* m_pPool; ///< Pointer to the pool that stores cuts.
    bool m_bPool; ///< If set to _false_, memory for the pool is not allocated.
	double m_dPoolCtrTol; ///< Maximum violation allowed for constraints in the pool.

// ////////////////////////////////////////////////////////////////////////
//                           Branch And Cut TREE                        //
// ////////////////////////////////////////////////////////////////////////
    CTree* m_pTree; ///< Pointer to the root node of the search tree.
    int m_iParentNode; ///< Index of the parent node.
    int m_iNode; ///< Index of the currently processed node.
    int m_iHeight; ///< Height of the currently processed node.

    int m_iObjSize; ///< Number of non-zero coefficients in the objective.
	bool m_bObjInt; ///< `true` if optimal objective value is integral.
	/**
	 * If `0 <= m_iObjRow < m_iM`, then row `m_iObjRow` represents the objective,
	 * and LHS and RHS of constrains `m_iObjRow` are lower and upper bounds for the optimal objective value.
	 * If `m_iObjRow >= SHIFT`, then the objective is to maximize variable `m_iObjRow-SHIFT`.
	 * If `m_iObjRow=-1`, then the objective is not represented by any constraint.
	 */
	int m_iObjRow;
	/**
	 * If `m_dObjVal < m_dMipObjVal + m_dAbsObjTol`, the node is pruned.
     */
    double m_dAbsObjTol; ///< Tolerance value for MIP objective
    /**
     * If the difference between some value and its nearest integer is less than `m_dIntTol`,
     * then this value is treated as an integer.
     */
    double m_dIntTol;

	CRecord *m_pRecord; ///< Pointer to structure that stores the best MIP solution found so far.
    double m_dLoBound; ///< equals `m_pRecord->m_dLoBound` when when the currently processed node (subproblem) was restored.

    char *m_strSolFileName; ///< Pointer to string with file name for storing intermediate solutions; if `m_strSolFileName=0`, intermediate solutions are not stored.

    enRoundType m_eRoundType; ///< Specifies how non-integer solutions are rounded.

    __LONG m_luTimeLimit; ///< Branch-and-cut stops after `m_luTimeLimit` seconds.
    double m_dDualGap; ///< Branch-and-cut stops if the dual gap is less than `m_dDualGap`.

///// STATISTICS ////////////////////////////////////////
    unsigned int m_iBranchAndCutNodes; ///< Number of branch-and-cut nodes processed.
    unsigned int m_iDifficultNodes; ///< Number of nodes where numerical difficulties were met.

///// CUT GENERATION ///////////////////////////////////
	int m_iM1; ///< Number of rows when a new round of cut generation starts.
    int m_iLastKnapsack0; ///< Index of the last knapsack constraint in the root-node matrix.
    int m_iLastKnapsack; ///< Starts linked list of knapsack constraints.
    int m_iLast01Flow0; ///< Index of the last flow constraint in the root-node matrix.
    int m_iLast01Flow; ///< Starts linked list of variable bound constraints in the currently processed matrix.
    int m_iLastVarBound0; ///< Number of variable bound constraints in the root matrix.
    int m_iLastVarBound;  ///< Starts linked list of variable bounds in the currently processed matrix.
    int m_iLastGUB;  ///< Starts linked list of GUB-constraints in the currently processed matrix.
    int m_iLastGUB0; ///< Index of the last GUB-constraint in the root-node matrix.
    int m_iLastPacking0; ///< Index of the last packing constraint in the root-node matrix.
    int m_iLastPacking;  ///< Starts linked list of packing constraints.
    int m_iLastMixedKnapsack0; ///< Index of the last mixed-knapsack constraint in the root-node matrix.
    int m_iLastMixedKnapsack;  ///< Starts linked list of mixed-knapsack constraints.
    int m_iLastParity0; ///< Index of the last parity constraint in the root-node matrix.
    int m_iLastParity;  ///< Starts linked list of parity constraints.
//    int m_iLastSimpleDJ0; ///< Index of the last simple-disjunction constraint in the root-node matrix.
//    int m_iLastSimpleDJ;  ///< Starts linked list of simple-disjunction constraints.
//    int m_iLastBranchingInv0; ///< Index in the root-node matrix of the last invariant knapsacks, which all variables are in variable bound.
//    int m_iLastBranchingInv;  ///< Starts linked list of invariant knapsacks, which all variables are in variable bound; such invariant knapsacks are used in balanced branching.

    int* m_ipCtrLink; ///< Linked lists of constraint classes.

// constraint statistics
    int m_iAutoCutRound; ///< Current round of generating cuts.
    int m_iMaxAutoCutRoundsForThisNode; ///< Maximum number of rounds (iterates) during one call to `generateCuts()` for currently processed node.
    int m_iMinAutoCutRoundsForThisNode; ///< Minimum number of rounds (iterates) during one call to `generateCuts()` for currently processed node.
    int m_iMaxAutoCutRounds; ///< Number of rounds for generating cuts per node.
    int m_iMaxAutoCutRoundsAtRoot; ///< Number of rounds for generating cuts at root node.
    int m_iMaxCutSize; ///< Maximum cut size.

    int m_iUserCutPattern; ///< Determines cuts which patterns cannot be changed by the solver.

    /**
     * The solver automatically generates cuts for
     *     - all nodes of height less or equal than `m_iAutoCutNodeHeight`;
     *     - first `m_iAutoCutNodes` processed nodes.
     *
     */
    int m_iAutoCutNodes, m_iAutoCutNodeHeight;

    /**
     * If after one-round-cut generation the objective decreases less than
 	 * by m_dCutRoundPrc (resp., m_dCutRoundPrcAtRoot) time its value without cuts, then we stop generating cuts.
     */
	double  m_dCutRoundPrcAtRoot, m_dCutRoundPrc;

	int m_iMinAutoCutRoundsAtRoot; ///< Minimum round to generate cuts at root node.
	int m_iMinAutoCutRoundsAtNodes;  ///< Minimum round to generate cuts at not-root nodes.

	bool m_bCutsForThisNode; ///< If `true`, at least one cut has been generated for node being processed.

	/*
	 *  If bit `i` in `m_uiCutsToPool` is set to `1`, then
     * corresponding cuts are added to the pool regardless they are tight or not at the optimum LP solution.
	 */
	unsigned m_uiCutsToPool;

    int m_ipCutRound[CUT_TYPE_NUM]; ///< `m_ipCutRound[i]` is maximum number of times `generateCuts()` call procedure that generates cuts of type `i`.
    int m_ipMaxCutRounds[CUT_TYPE_NUM]; ///< The solver calls at most `m_ipMaxCutRounds[i]` times cut generating procedure for cuts of type `i`.
    int	m_ipMaxCutsPerRound[CUT_TYPE_NUM]; ///< At most `m_ipMaxCutsPerRound[i]` cuts are generated during each call of cut generating procedure for cuts of type `i`.
    int m_ipCutNodes[CUT_TYPE_NUM]; ///< `m_ipCutNodes[i]` is number of nodes for which cuts of type `i` are generated.
    int m_ipCutNodeHeight[CUT_TYPE_NUM]; ///< `m_ipCutNodeHeight[i]` is minimum height of a node for which cuts of type `i` are generated.
  	int m_ipMaxCutPrc[CUT_TYPE_NUM];  ///< For cuts of type `i`, only cuts of size not greater than `(m_ipMaxCutPrc[i]*m_iN)/100` are generated.
    double m_dpCutTol[CUT_TYPE_NUM]; ///< `m_dpCutTol[i]` is tolerance value for cuts of type `i`.

    int	m_iGomoryMinDenseCutSize; ///< If a cut size <= m_iGomoryMinDenseCutSize, the cut is treated as dense cut.
    int m_iMod2MinDenseCutSize; ///< If cut size is not greater than `m_iMod2MinDenseCutSize`, the cut is treated as dense cut.
	int m_iMirMinDenseCutSize; ///< Cuts of size not greater than `m_iMirMinDenseCutSize` are never rejected as dense ones.

// Statistics of using cuts
  	int m_ipCurrentRoundCutNum[CUT_TYPE_NUM];
  	int *m_ipTotalCutNum;

  	/// \return number of cuts of type `type` generated.
  	int getCutGenerated(int type) {return m_ipTotalCutNum[type<<1];}

  	/// \return number of cuts of type `type` used.
  	int getCutUsed(int type) {return m_ipTotalCutNum[(type<<1)+1];}

    int m_iCurrentMxTemplate; ///< used internally to smoothly use templates when generating  MIR cuts.

////// SOS constraints ////////////////////////////
    bool m_bSOS1branching; ///< If `true`, SOS1-branching is applied; default value is `false`.
    int m_iSOS1num; ///< Number of SOS1 constraints.
    int m_iSOS2num; ///< Number of SOS2 constraints.
    int m_iLastSOS1;  ///< Starts linked list of SOS1-constraints in the currently processed matrix.
    int m_iLastSOS10; ///< Index of the last SOS1-constraint in the root-node matrix.
    int m_iLastSOS2; ///< Starts list of SOS2 constraints.

//////////////////////////////////////////////////
#ifndef __ONE_THREAD_
// Multithreading
    int m_iCoreNum; ///< Number of physical cores.
    int m_iThreadNum; ///< Thread number used when solving MIPs,; by default, `m_iThreadNum=m_iCoreNum`.
	_RWLOCK *m_rwStatLock; ///< Locks changing statistics attributes: `m_ipTotalCutNum`.
	_MUTEX *m_pMemMutex; ///< Locks any memory reallocations.

#ifdef __PSEUDOCOSTS_
	_RWLOCK *m_rwBrLock; ///< Locks changing arrays `m_dpBrDec` and `m_ipBrNum` used in score branching.
#endif

static int getNumberOfCores(); ///<The function  returns number of physical cores.

	/**
	 * The function starts a new thread.
	 * \param[in] pointer to `CMIP` objects.
	 * \return always `0`.
	 */
#ifdef _WIN32
	static unsigned int __stdcall startThread(void* param);
#else
	static void* startThread(void* param);
#endif

protected:
	/// The clone constructor.
	/**
	 * The function clones an instance of `CMIP` class.
	 * \param[in] pMip a pointer to an instance of CMIP class to be cloned;
	 * \param[in] thread index from {1,2,...,__\_\_MAX_THREAD_NUM}.
	*/
	virtual CMIP* clone(const CMIP *pMip, int thread);

	int getThreadNum() const
	{return m_iThreadNum;} ///< \return number of threads currently used.

	/**
	 * In multi-threaded application a separate CMIP object is created for each thread.
	 * The threads share common memory, which is allocated and released by the object of thread 0.
	 * \return thread index of calling object.
	 * \see CLP::m_iThread
	 */
	int getThreadIndex() const
	{return m_iThread;}
public:
	/**
	 * If `threadNum > 0`  and `threadNum < __MAX_THREAD_NUM`,
	 * the function sets the number of threads to threadNum;
	 *  otherwise, the function does nothing.
	 *  \param[in] threadNum number of threads.
	 */
	void setThreadNum(int threadNum);
#endif
//////////////////////////////////////////
//     Functions of general use
//////////////////////////////////////////

    /**
     * The function sets the _separaion rule_ used when not-root node LPs of MIPs.
     *  \param[in] sepRule new separation rule.
     * \sa enSepRule, getSepRule(), CMIP::setMipSepRule().
     */
    void setNodeSepRule(enSepRule sepRule)
    {
    	if (sepRule == SEP_MOST_VIOLATED || sepRule == SEP_STEEPEST_EDGE)
    		m_eSepRule=m_eNodeSepRule=sepRule;
    }

    /**
     * The functions sets the _pricing rule_ to be used for solving not-root node LPs.
     * \param[in] pricingRule new pricing rule.
     * \sa enPricingRule, getPricingRule(), setMipPricingRule().
     */
    void setNodePricingRule(enPricingRule pricingRule)
    {
    	if (pricingRule == PRC_MOST_NEGATIVE || pricingRule == PRC_STEEPEST_EDGE)
    		m_ePricingRule=m_eNodePricingRule=pricingRule;
    }
protected:
    /**
     *  /return `true` if the problem being solved is an LP (has no integer variable).
     */
    bool isPureLP() const;

    /**
     * The function returns the tolerance value of given constraint.
     * \param[in] i constraint index.
     * \return maximum violation allowed for this particular constraint.
     */
    double getCtrTol(int i)
        {return ((i >= m_iM0 && m_ipRowHd[i] < 0 && !(m_ipCtrType[i] & CTR_ATTACHED))? m_dPoolCtrTol: m_dCtrTol);}

public:
    /**
     * The function sets the tolerance value for regular (not cuts) constraints.
     * \param[in] ctrTol maximum violation allowed for all constraints (excluding cuts).
     * \sa CLP::setCtrTol(), CLP::getCtrTol().
     */
    void setCtrTol(double ctrTol);

    /**
     *  \param[in] i constraint index.
     *  \return `true` if constraint `i` is global; otherwise, `false`.
     */
    bool isCtrGlobal(int i) const
    {
        return (m_ipCtrType[i] & CTR_LOCAL)? false: true;
    }

    /**
     * \return number of integer variables.
     */
    int getIntegerVarNum() const
        {return m_iIntNum;}

    /**
     * \return number of binary variables.
     */
    int getBinaryVarNum() const
        {return m_iBinNum;}

    /**
     * \return number of real valued variables.
     */
    int getRealVarNum() const
        {return m_iN-m_iIntNum;}

 /**
  * \return maximum number of rounds when probing binary variables.
  * \sa setProbingDepth(), setProbingRoundNum().
  */
    int getProbingDepth() const
        {return m_iProbingDepth;}

/**
 * \param[in] depth new maximum number of rounds when probing binary variables.
 * \sa getProbingDepth(), setProbingRoundNum().
 */
    void setProbingDepth(int depth)
        {m_iProbingDepth=depth;}

/** Integer variables can be assigned priorities, i.e., integer numbers from `VAR_PRI_MIN` to `VAR_PRI_MAX`,
 * which are used when selecting a variable for branching.
 * The higher priority the more chances for a fractional variable to be chosen.
 * \param[in] j index of variable which priority to be changed
 * \param[in] pr new priority
 * \sa incVarPriority(), getVarPriority().
*/
    void setVarPriority(int j, int pr);

 /**
  * The function increases priority of variable `j` by `inc`.
  * \param[in] j variable index;
  * \param[in] inc value of increase.
  * \sa setVarPriority(), getVarPriority().
  */
    void incVarPriority(int j, int inc);

 /**
  * \param[in] j variable index.
  *  \return priority of variable `j`.
  *  \sa setVarPriority(), incVarPriority().
  */
    int getVarPriority(int j) const;

    /**
     * The function sets the objective tolerance value:
     * If the difference between the upper and lower bounds on the optimal objective value
     *  is less than the objective tolerance value, the solver returns the best solution found
     *  as a good approximate solution.
     * \param[in] tol tolerance value.
     * \sa getAbsObjTol().
     */
    void setAbsObjTol(double tol)
        {if (tol >= 0.0) m_dAbsObjTol=tol;}

    /**
     * \return objective tolerance value.
     * \sa setAbsObjTol().
     */
    double getAbsObjTol() const
        {return m_dAbsObjTol;}

protected:
    /**
     * This function overload the function of the base class.
     * The functions extends the type of variable `j`
     *  by  bitwise ORing its current type with the flags stored in the parameter `type`.
     * \param[in] j index of variable;
     * \param[in] type bitwise OR of members of `enVarType` and `CMIP::enVarType`.
     * \sa CLP::enVarType, CMIP::enVarType, and `CLP::extendVarType()`.
     */
    void extendVarType(int j, unsigned type);

///////////////////////////////////////////
//   Overloaded matrix functions
///////////////////////////////////////////
public:
    /**
     * The function overloads the function of the base class `CLP`.
     * \sa CLP::openMatrix().
     */
	void openMatrix(int m, int n, int nz,
			bool bRowGen=true, bool bColGen=false,
			int mMax=0, int nMax=0, int nzMax=0);

    /**
     * The function overloads the function of the base class `CLP`.
     * \sa CLP::closeMatrix().
     */
    void closeMatrix();

    /**
     * The function adds a new cut to the matrix.
     * \param[in] hd handle of the constraint,
     *           - if `hd >= 0`, you must overload getRow function;
     *           - if `hd == -1`, the constraint is added to the pool immediately;
     *           - if `hd < -1` (most common option), the constraint is added to the pool if
     *            it is tight (holds as equality) for an optimal LP solution;
     * \param[in] type type of the constraint;
     * \param[in] b1,b2 left hand side (LHS) and right hand side (RHS), respectively;
     * \param[in] sz number of nonzero entries;
     * \param[in] dpVal,ipCol arrays of size `sz`, `dpVal[i]` is coefficient in column `ipCol[i]`;
     * \param[in] n if `n > m_iN`, then `n` is the number of variables in the original (non-preprocessed)
     * problem; in this case the constraint must be preprocessed;
     * \param[in] bVarScaled is `true` if cut is written in scaled variables;
     * \param[in] factor the constraint has been already scaled (multiplied) by `2^{factor}`,
     *         if `factor==NOT_SCALED`, __MIPCL__ will scale this constraint.
     * \return index of the newly created constraint.
     * \throws CMemoryException lack of memory.
     *
     */
    int addCut(tagHANDLE hd, unsigned type, double b1, double b2,
             int sz, double* dpVal, int* ipCol,
             bool bVarScaled=true, int factor=NOT_SCALED, int n=0);

    /**
     * This function is a safer version of `addCut()`.
     * If `dpVal` and  `ipCol` are references to __ MIPCL__ internal arrays such as `m_dpFb` or `m_ipArray`,
     * memory for such arrays may be reallocated during the call to `addCut()`,
     * and, as a consequence, the pointers `dpVal` and `ipCol` becomes not valid.
     * \param[in] hd handle of the constraint,
     *           - if `hd >= 0`, you must overload getRow function;
     *           - if `hd == -1`, the constraint is added to the pool immediately;
     *           - if `hd < -1` (most common option), the constraint is added to the pool if
     *            it is tight (holds as equality) for an optimal LP solution;
     * \param[in] type inequality type (bitwise OR of `enVarType` members);
     * \param[in] b1,b2 left hand side (LHS) and right hand side (RHS), respectively;
     * \param[in] sz number of nonzero entries;
     * \param[in] dpVal,ipCol arrays of size `sz`, `dpVal[i]` is coefficient in column `ipCol[i]`;
     * \param[in] n if `n > m_iN`, then `n` is the number of variables in the original (non-preprocessed)
     * problem; in this case the constraint must be preprocessed;
     * \param[in] bVarScaled is `true` if cut is written in scaled variables;
     * \param[in] factor the constraint has been already scaled (multiplied) by `2^{factor}`,
     *         if `factor==NOT_SCALED`, __MIPCL__ will scale this constraint.
     * \return index of the newly created constraint.
     * \throws CMemoryException lack of memory.
     * \sa addCut().
    */
    int safeAddCut(tagHANDLE hd, unsigned type, double b1, double b2,
             int sz, double* &dpVal, int* &ipCol,
             bool bVarScaled=true, int factor=NOT_SCALED, int n=0);

private:
    /**
     * The function compute a hash value `(hash1,hash2)` for a given constraint
     *     `b1 <= sum(i in 0..sz-1) dpVal[i]*x(ipCol[i]) <= b2`.
     * This function is used to prevent adding identical cuts.
     *
     * \param[in] sz number of entries;
     * \param[in] ipCol column indices, list of size 'sz';
     * \param[in] dpVal coefficients, list of size 'sz';
     * \param[in] b1,b2 left and right hand sides;
     * \param[in] factor input inequality is to be multiplied by `2^{factor}`.
     * \return 'true' if cut is new; otherwise, 'false'.
     * \remark 'm_dpW' is used to store hash values.
     */
    bool isNewCut(int sz, const int* ipCol, const double* dpVal, double b1, double b2, int factor=0);

//////////////////////////////////////////////////////////////////////////
//     C O N S T R U C T O R S / D E S T R U C T O R                    //
//////////////////////////////////////////////////////////////////////////
    void init(); ///< The procedure is called from the constructors to initialize most of the class members.
public:
    /// Default constructor.
    /**
     *  This constructor initializes many of the class members, but not allocates any memory.
     */
    CMIP();

    /**
     *  This constructor does the same as the default one, plus it sets the problem name to `name`.
     *  \param[in] name problem name.
     */
    CMIP(const char* name);

#ifndef __ONE_THREAD_
    /// Clone constructor.
	/**
	 * It is  used in multi-threaded applications.
	 * The cloned object shares many members with the object being cloned.
	 * \param[in] other reference to a `CMIP` object being cloned;
	 * \param[in]  thread thread-index.
     * \throws `CMemoryException` lack of memory.
     */
    CMIP(const CMIP &other, int thread);
#endif


    virtual ~CMIP(); ///< The destructor.

// ////////////////////////////////////////////////////////////////////////
//                   Initialization/memory allocation                   //
// ////////////////////////////////////////////////////////////////////////
    /**
     * Call this function only in case when cuts of all types are not supposed to be generated.
     * In such a case you can still overload separate(), genCut1(), and genCut2() procedures
     * to generate problem specific constraints. In such a case, you have to overload
     * getRow() function from the base class LP.
     */
    void doNotUsePool()
        {m_bPool=false;}

private:
    void allocMemForBC(); ///< allocates memory for the search tree, pool, and for some working arrays.
    /**
     * The function increases the maximum number of columns in the matrix,
     * and then reallocates memory for all structures which size depend on the number of columns.
     * \throws CMemoryException lack of memory.
     * \sa CLP::incMaxColumnNumber().
     */
    void incMaxColumnNumber();

    /**
     * The function increases the maximum number of rows in the matrix,
     * and then reallocates memory for all structures which size depend on the number of rows.
     * \throws CMemoryException lack of memory.
     * \sa CLP::incMaxColumnNumber().
     */
    void incMaxRowNumber();
   	void reallocMemForEntries(int nz); ///< thread safe version of the base-class-function.

   	/**
   	 *  First calls `CLP::allocMemForAuxArrays()`, and then allocates memory for `m_dpFd` and `m_dpFb`.
   	 *  \throws CMemoryException  lack of memory.
   	 */
   	void allocMemForAuxArrays();

   	/**
   	 * The function is called within `incMaxColumnNimber()` to reallocate memory for `m_dpFd` and `m_dpFb.
   	 * \param[in] rowMem if `true`, number of rows has been increased;
   	 * \param[in] colMem if `true`, number of columns has been increased.
   	 * \remark This function does not overload `CLP::reallocMemForAuxArrays()`.
   	 * \throws CMemoryException  lack of memory.
   	 */
   	void reallocMemForAuxArrays(bool rowMem, bool colMem);

   	bool storeObj(); ///< overloads `CLP::storeObj()` to save a lit bit memory.
   	void restoreObj(bool flag);  ///< overloads `CLP::restoreObj()`.

protected:
    /**
   	 * \param[in] j variable index.
   	 * \return `true` if variable `j` is binary.
     */
    bool isVarBinary(int j) const
    {
        return (m_ipVarType[j] & VAR_BIN)? true: false;
    }

   	/**
   	 * Using different preprocessing techniques one can deduce that some variables initially not declared as integer-valued
   	 * may take only integer values. The solver declares such variables as integral
   	 * (this may help the solver in generating stronger cuts) but assigns them the lowest priority to avoid branching by such variables.
   	 * It worth noting, that, for technical reasons, __MIPCL__ assigns to variables of the lowest priority the maximum possible priority value.
   	 * \param[in] j variable index.
   	 * \return `true` if variable `j` is integer-valued.
   	 * \sa isVarUsedForBranching().
   	 */
    bool isVarStrongIntegral(int j) const
    {
        return ((m_ipVarType[j] & VAR_INT) && (getVarPriority(j) > VAR_PRI_MIN))? true: false;
    }

    /**
     * We say that a variable is _used for branching_ if its lower and upper bounds
     *  may be changed during the branching process.
     * Let us also note that real-valued variables (say, those in SOS2-constraints) can be fixed during the branching process.
     * So, they are used for branching as well as strong integral variables.
   	 * \param[in] j variable index.
   	 * \return `true` if variable `j` can be chosen for branching.
   	 * \sa isVarUsedForBranching().
     * \sa isVarStrongIntegral(), isVarSOS2(), startBranching().
     */
    bool isVarUsedForBranching(int j) const;

   /**
    * \param[in] j variable index.
   	 * \return `true` if variable `j` appears in a SOS2-constraint.
   	 * \sa isVarUsedForBranching().
    */
    bool isVarSOS2(int j) const
    {
        return (m_ipVarType[j] & VAR_SOS2)? true: false;
    }

    /**
     * Strong integral and SOS2 variables are _not scalable_.
    * \param[in] j variable index.
   	 * \return `true` if variable `j` is _scalable_.
   	 * \sa isVarStrongIntegral(), isVarSOS2().
    */
    bool isVarScalable(int j) const
    {
        return (m_ipVarType[j] & (VAR_INT | VAR_SOS2))? false: true;
    }

    /**
     *  Constraint is declared as being _integral_ if all its variables are integer-valued as well as all its coefficients are integers.
     *  The slack of an inequality (defined as the difference between its right and left hand sides) takes only integral value;
     *  this property may be useful in cut generating procedures.
     *  \param[in] i constraint index.
     *  \return `true` if constraint `i` is integral; otherwise, `false`.
     */
    bool isCtrIntegral(int i) const
    {
        return ((m_ipCtrType[i] & CTR_INT_VARS) && (m_ipCtrType[i] & CTR_INT_COEFF))? true: false;
    }

    /**
     * The objective is _integral_ if its optimal value is an integer.
     * \return `true` if the objective is integral; otherwise, `false`.
     */
    bool isObjIntegral() const
    {return m_bObjInt;}

    /**
     * For a not-root node, a lower variable bound for a variable is _local_ if its value differs from that at the root node.
     * \param[in] j variable index.
     * \return `true` if the lower bound of variable `j` is local; otherwise, `false`.
     * \sa isVarLoBoundLocal().
     */
    bool isVarUpBoundLocal(int j) const;

    /**
     * For a not-root node, an upper  bound for a variable is _local_ if its value differs from that at the root node.
     * \param[in] j variable index.
     * \return `true` if the lower bound of variable `j` is local; otherwise, `false`.
     * \sa isVarUpBoundLocal().
     */
    bool isVarLoBoundLocal(int j) const;

	/**
	 * A variable is _monotone up_ if, given any solution of the relaxation LP,
	 *   we can increase the value of the considered variable without violating any constraint other than
	 *   the upper bound of this variable.
	 * \param[in] j variable index.
	 * \return `true` if variable `j` is monotone up; otherwise, `false`.
	 * \sa isVarMonotoneDown(), isVarMonotone().
	 */
    bool isVarMonotoneUp(int j) const
        {return (m_ipVarType[j] & VAR_MON_UP)? true: false;}

	/**
	 * A variable is _monotone down_ if, given any solution of the relaxation LP,
	 *   we can decrease the value of the considered variable without violating any constraint other than
	 *   the lower bound of this variable.
	 * \param[in] j variable index.
	 * \return `true` if variable `j` is monotone down; otherwise, `false`.
	 * \sa isVarMonotoneUp(), isVarMonotone().
	 */
    bool isVarMonotoneDown(int j) const
        {return (m_ipVarType[j] & VAR_MON_DOWN)? true: false;}

	/**
	 * A variable is _monotone_ if it is either monotone up or monotone down.
	 * Usually, a monotone variable is among the last candidates for choosing it as the branching variable
	 * since it is always possible to round off such a variable.
	 * \param[in] j variable index.
	 * \return `true` if variable `j` is monotone; otherwise, `false`.
	 * \sa isVarMonotoneUp(), isVarMonotone().
	 */
    bool isVarMonotone(int j) const
        {return (m_ipVarType[j] & (VAR_MON_UP | VAR_MON_DOWN))? true: false;}

private:
    /**
     * \param[in] j index of variable.
     * \return `true` if variable `x[j]` is in a variable bound constraint.
     */
    bool isVarInVarBound(int j) const
    {return (!(m_ipVarType[j] & VAR_BIN) && (m_ipVarType[j] & VAR_IN_VAR_CTR))? true: false;}

    /**
     * \param[in] j index of variable.
     * \return `true` if variable `x[j]` is in a variable upper bound constraint.
     */
    bool isVarInVarUpperBound(int j) const
    {return (!(m_ipVarType[j] & VAR_BIN) && (m_ipVarType[j] & VAR_IN_VAR_UB))? true: false;}

    /**
     * \param[in] j index of variable.
     * \return `true` if variable `x[j]` is in a variable lower bound constraint.
     */
    bool isVarInVarLowerBound(int j) const
    {return (!(m_ipVarType[j] & VAR_BIN) && (m_ipVarType[j] & VAR_IN_VAR_LB))? true: false;}

    /**
     * \param[in] i index of constraint.
     * \return `true` if constraint indexed by `i`  is a variable upper bound.
     */
	bool isCtrVarUb(int i) const
	{return  ((m_ipCtrType[i] & CTR_VAR) && (m_ipCtrType[i] & CTR_RIGHT))? true: false;}

    /**
     * \param[in] i index of constraint.
     * \return `true` if constraint indexed by `i`  is a variable lower bound.
     */
	bool isCtrVarLb(int i) const
	{return  ((m_ipCtrType[i] & CTR_VAR) && (m_ipCtrType[i] & CTR_LEFT))? true: false;}

// overloaded functions of the base class CLP
	bool isVarLbImplied(int j) const;
	bool isVarUbImplied(int j) const;

	/**
	 *  All but SOS2-variables (i.e., those included in SOS2-constraints) can be deleted from the matrix by the solver.
	 *  \param[in] j index of variable.
     * \return `true` if variable `j` can be deleted.
	 */
    bool isVarDeletable(int j) const
    {
        return (m_ipVarType[j] & VAR_SOS2)? false: true;
    }

    /**
     *  All but SOS2-constraints can be modified by the solver.
     * \param[in] i constraint index.
     * \return `true` if constraint `i` can be modified.
     */
    bool isCtrModifyable(int i) const
    {
    	return (m_ipCtrType[i] & CTR_SOS2)? false: true;
    }

    /**
     * \param[in] i constraint index.
     * \return `true` if constraint `i` is a GUB-constraint.
     */
    bool isCtrGUB(int i) const
    {
    	return ((m_ipCtrType[i] & CTR_PACKING) && (m_ipCtrType[i] & CTR_CARDINALITY))? true: false;
    }

///////////////////////////////////////////////////////
//  Branch and Cut attributes
//////////////////////////////////////////////////////
public:

    /**
     * Any MIP is transformed into the form when the objective is maximized.
     * The function returns the transformed objective value of the currently best solution.
     * It is usually called when the problem is being solved.
     * \return objective value of the best MIP solution found.
     * \sa getObjVal(), safeGetObjLowerBound().
     */
    double getObjLowerBound() const;

    /**
     * A thread safe version of getObjLowerBound().
     * \return objective value of the best MIP solution found.
     * \sa getObjVal(), safeGetObjLowerBound().
     * \remark this function locks for reading `m_pRecord`.
     */
    double safeGetObjLowerBound();
public:
    /**
     * This function is usually called when the problem has been solved already.
     * \return original objective value of the best MIP solution found.
     * \sa getObjLowerBound(), safeGetObjLowerBound().
     */
    double getObjVal() const;

    /**
     * \return number of MIP solutions found so far.
     * \sa safeGetSolNum().
     */
    int getSolNum() const;

    /**
     * Thread safe version getSolNum().
     * \return number of MIP solutions found so far.
     * \sa  getSolNum().
     */
    int safeGetSolNum();

    /**
     * \return index of the currently processed node.
     * \sa getCurrentNodeHeight().
     */
    int getCurrentNode() const
        {return m_iNode;}

    /**
     * \return height of the currently processed node.
     * \sa getCurrentNode().
     */
    int getCurrentNodeHeight() const;

    /**
     * An integer variable is assumed to be integral if the absolute difference
     * between its value and the nearest integer to this value is less than the integrality tolerance.
     * \param[in] intTol new integrality tolerance.
     * \sa getIntTol().
     */
    void setIntTol(double intTol)
        {m_dIntTol=intTol;}

    /**
     * \return integrality tolerance.
     * \sa setIntTol().
     */
    double getIntTol() const
        {return m_dIntTol;}

   /**
    * This function sets a new search rule.
    * \param[in] rule search rule to for low-height nodes;
    * \sa enBranchRule.
    */
    void setBranchingRule(enBranchRule rule)
    {
    	m_eBranchRule=rule;
    }

    /**
     * The solver automatically generates cuts for first `m_iAutoCutNodes` processed nodes,
     * and also for nodes of height less or equal than `m_iAutoCutNodeHeight`.
     * \param[in] nodeNum number of nodes for which cuts are generated;
     * \param[in] height cuts are generated for nodes of height `<= height`.
     * \sa setCutTypePattern(), setAutoCutRounds().
     */
    void setAutoCutPattern(int nodeNum, int height)
	{
		m_iAutoCutNodes=nodeNum;
		m_iAutoCutNodeHeight=height;
	}

    /**
     * The solver automatically generates cuts of type `cutType` only for first `m_ipCutNodes[cutType]` processed nodes.
     * \param[in] cutType cut type;
     * \param[in] nodeNum number of nodes for which cuts of type `cutType` are generated;
     * \param[in] height cuts of type `i` are generated for nodes of height `<= height`.
     * \sa enCutType, setAutoCutPattern();
    */
    void setCutTypePattern(enCutType cutType, int nodeNum, int height)
    {
    	m_iUserCutPattern|= (1 << cutType);
    	m_ipCutNodes[cutType]=nodeNum;
    	m_ipCutNodeHeight[cutType]=height;
    }

    /**
     * At any node the solver calls cut generating procedures a restricted number of times.
     * \param[in] atRoot at root node, solver repeatedly calls its cut generating procedure at most `atRoot` times.
     * \param[in] atNodes at not-root nodes, solver repeatedly calls its cut generating procedure at most `atNodes` times.
     * \sa setAutoCutPattern().
     */
    void setAutoCutRounds(int atRoot, int atNodes)
        {m_iMaxAutoCutRoundsAtRoot=atRoot; m_iMaxAutoCutRounds=atNodes;}

    /**
     * The solver adds to the matrix as well as to the pool cuts of restricted size.
     * \param[in] size new value of maximum size.
     */
    void setMaxCutSize(int size)
    {m_iMaxCutSize=size;}

    /**
     * The rules that are used by the solver
     *  to choose a right moment to stop generating cuts are complicated.
     *  The progress in decreasing the upper bound
     *  on the optimal objective value is the key factor affecting the whole cut generation process.
     *  setObjDecFracPerCutRoundAtRoot() sets a new minimum relative decrease of the upper bound (objective value of the relaxation LP)
     *  per cut round for the root node.
     * \param[in] prc minimum relative decrease of the upper bound per cut round for the root node.
     * \sa getRelObjDecPerCutRoundAtRoot(), setRelObjDecPerCutRoundAtNodes().
     */
	void  setRelObjDecPerCutRoundAtRoot(double prc)
	{
		if (prc > 1.0e-6) m_dCutRoundPrcAtRoot=prc;
	}

	/**
     * \return prc minimum relative decrease of the upper bound per cut round at the root node.
     * \sa setRelObjDecPerCutRoundAtRoot().
	 */
    double  getRelObjDecPerCutRoundAtRoot() const
	{return m_dCutRoundPrcAtRoot;}

    /**
     *  This function sets a new minimum relative decrease of the upper bound (objective value of the relaxation LP)
     *  per cut round for not-root nodes.
     * \param[in] prc minimum relative decrease of the upper bound per cut round for not-root nodes.
     * \sa getRelObjDecPerCutRoundAtNodes(), setRelObjDecPerCutRoundAtRoot().
     */
	void  setRelObjDecPerCutRoundAtNodes(double prc)
	{
		if (prc > 1.0e-6)
			m_dCutRoundPrc=prc;
	}

	/**
     * \return prc minimum relative decrease of the upper bound per cut round for not-root nodes.
     * \sa setRelObjDecPerCutRoundAtNodes().
	 */
	double getRelObjDecPerCutRoundAtNodes() const
	{return m_dCutRoundPrc;}

	/**
	 * The procedure sets minimum numbers of rounds to generate cuts at the root and not-root nodes.
	 * \param[in] atRoot minimum number of rounds to generate cuts at root node;
	 * \param[in] atNodes minimum number of rounds to generate cuts at not-root nodes.
	 */
	void setMinCutRounds(int atRoot, int atNodes)
		{m_iMinAutoCutRoundsAtRoot=atRoot; m_iMinAutoCutRoundsAtNodes=atNodes;}

	/**
	 * The solver calls the cut generating procedure for a particular cut type a limited number of times.
	 * This function reestablishes that limit.
     * \param[in] cutType cut type;
     * \param[in] roundNum number of rounds.
     * \sa enCutType, setMaxCutsPerRound().
	 */
    void setMaxCutRoundNum(enCutType cutType, int roundNum)
        {m_ipMaxCutRounds[cutType]=roundNum;}

	/**
	 * At each cut generation round, the cut generating procedure for a particular cut type
	 * generates no more than a predefined number of cuts.
	 * This function reestablishes that limit.
     * \param[in] cutType cut type;
     * \param[in] roundNum number of cuts.
     * \sa enCutType, setMaxCutRoundNum().
	 */
    void setMaxCutsPerRound(enCutType cutType, int roundNum)
    	{m_ipMaxCutsPerRound[cutType]=roundNum;}

    /**
     * \param[in] cutType cut type;
     * \param[in] tol cut inequality is assumed as being violated, and then added to the matrix,
     *  if the inequality is violated by at least `tol`.
     * \sa enCutType, getToleranseForCut().
     */
    void setToleranseForCut(enCutType cutType, double tol)
        {m_dpCutTol[cutType]=tol;}

    /**
     * \param[in] cutType cut type.
     * \return tolerance value for the cuts of type `cutType`.
     * \sa enCutType, setToleranseForCut().
     */
    double getToleranseForCut(enCutType cutType) const
        {return m_dpCutTol[cutType];}

    /**
     * Usually, cuts are sent to the pool
     * if they are in the matrix when the processed subproblem is added to the search tree as a new leaf.
     * Call this function to immediately send to the pool all newly generated cuts of type `cutType`.
     * \param[in] cutType cut type.
     * \sa enCutType, areCutsSentToPool.
     */
    void sendCutsToPool(enCutType cutType)
    	{m_uiCutsToPool|=(1 << cutType);}

    /**
     * \param[in] cutType cut type.
     * \return `true` if cuts of type `cutType` are immediately sent to the pool.
     * \sa enCutType, sendCutsToPool.
     */
    bool areCutsSentToPool(enCutType cutType) const
    	{return (m_uiCutsToPool & (1 << cutType))? true: false;}

    /**
     * Mod2-cuts (or (0,1/2)-Chvatal-Gomory cuts) are considered as sparse
     *  if the number of variables in them is less than `sparsePrc*m_iN/100`
     * (let us remember that `m_iN` is the number of a` variables),
     * and as dense if the number of variables in them is less than `sdensePrc*m_iN`.
     * Any cut of size greater than `sdensePrc*m_iN/100` is considered as super-dense and is rejected (not generated).
     * \param[in] sparsePrc percentage (relative to the total number of variables in the problem) of variable in sparse cuts;
     * \param[in] densePrc percentage of variable in dense cuts.
     */
    void setMod2CutDensity(int sparsePrc, int densePrc)
        {m_ipMaxCutPrc[_SPARSE_MOD2]=sparsePrc; m_ipMaxCutPrc[_DENSE_MOD2]=densePrc;}

    /**
     * Gomory cuts are considered as sparse if the number of variables in them is less than `sparsePrc*m_iN/100`
     * (let us remember that `m_iN` is the number of all variables),
     * and as dense if the number of variables in them is less than `sdensePrc*m_iN`.
     * Any cut of size greater than `sdensePrc*m_iN/100` is considered as super-dense and is rejected (not generated).
     * \param[in] sparsePrc percentage (relative to the total number of variables in the problem) of variable in sparse cuts;
     * \param[in] densePrc percentage of variable in dense cuts.
     */
    void setGomoryCutDensity(int sparsePrc, int densePrc)
        {m_ipMaxCutPrc[_SPARSE_GOMORY]=sparsePrc; m_ipMaxCutPrc[_DENSE_GOMORY]=densePrc;}
////////////////////////////////////////////////////////

private:
////////////////////////////////////////////////////////
//  Classification Reformulation, and preprocessing   //
////////////////////////////////////////////////////////
    /**
     *  The function marks all variables present in SOS2-constraints as SOS2-variables.
     *  \see `CTR_SOS2` and `VAR_SOS2`.
     */
    void selectSOS2vars();

    ///< The function marks integer-valued variables with lower and upper bounds, respectively, `0` and `1` as _binary_.
    void selectBinaries();

    ///< The procedure seeks for flow structures to classify flow variables as integer ones.
	void seekFlowStructures();

    ///< The procedure looks for flow structures to classify flow variables as integer ones.
	void checkTrFlowStructure();

	/**
	 * A task is defined to be a GUB such that, for each pair of its columns,
	 * the column of bigger cost "uses more resources", i.e, has bigger or the same
	 * coefficients in all rows (it is assumed the all constraints are `<=` inequalities).
	 *
	 * \return number of fixed variables.
	 */
	int seekMonotoneTasks();


// next functions are used to remove symmetry
	int seekParallelMachines(); ///< tries to find constrains describing capacities of parallel machines.
	int extractBlocks(int m, int q, int &rowSz, int &colSz);
	int breakCtrSymmetry(int symNum, int ctrNum, int colNum, int *ipCol);
	void breakSymmetry(int firstBlk, int lastBlk, int colNum, int *ipCol);
	int checkSymBlocks(int m, int k, int rowSz, int colSz, int *ipRow, int *ipCol);
	int seekSymBlocks();
//
	int seekIdenticalCols(); ///< looks for pairs of identical columns.
	int seekIncludedInvKnapsacks();

// Constraint classification
	/**
	 * The procedure detects SOS2-constraints.
	 *
	 * \return number of SOS2-constraits detected.
	 * \sa isSOS2().
	 */
    int seekSOS2();

    /**
     * The functions verifies whether a given row (together with some other rows) represents a __SOS2__ constraint.
     * \param[in] r row index.
     * \return `true` if row `r` represents a __SOS2__ constraint.
     * \remark the function has not the `const` attribute since it may alter internal `MIPCL` arrays.
     */
    bool isSOS2(int r);

    /**
     * \param[in] r row index.
     * \return `true` if row `r` represents a _parity_ constraint.
     */
    bool isParityCtr(int r) const;

    /**
     * The procedure tries to multiply a given constraint, in turn, by `1,...,10,100,1000,10000`
     * until all coefficients are integral. If such a multiplier exists, let us denote it by `a`,
     * and the constraint contains at most one non-integral variable,
     * then this constraint is multiplied by `a`.
     * \param[in] row constraint index;
     * \param[in] maxFactor integer in {10,100,1000,10000}; say, if `maxFactor=1000`,
     *  then coefficient are multiplied by `1,...,10,1000`.
     */
    void makeCtrCoeffIntegral(int row, int maxFactor=1000);

    /**
     * The procedure does for cuts the same as `makeCtrCoeffIntegral()` does for matrix constraints.
     * \param[in] sz size of arrays `dpVal` and `ipCol`;
     * \param[in,out] dpVal,ipCol `sum(i=0,..,sz-1) dpVal[i]*x[ipCol[i]] <= b` is input inequality;
     * \param[in,out] b right hand side of input inequality;
     * iparam[in] maxFactor integer in {10,100,1000,10000}; say, if `maxFactor=100`,
     *  then coefficient are multiplied by `1,...,10,100`.
     * \sa makeCtrCoeffIntegral().
     * \return `true` if all coefficient (`dpVal[i]`) are integral.
     */
    bool makeCutCoeffIntegral(int sz, double *dpVal, int *ipCol, double &b,  int maxFactor=100);

    /**
     * The procedure establishes additional properties of constraints that are not fully classified.
     * \param[in] constraint index.
     * \return bitwise OR of `enVarType` members.
     */
    unsigned classifyCtrPlus(int ind);

    /**
     * The function classifies constraint `row`.
     * \param[in] row index of constraint.
     */
    void classifyCtr(int row);

    /**
     * The procedure classifies constraints and variables.
     * \param[in] bPlus when applied within preprocessing step with `bPlus` set to `false`,
     *  some properties of constraints, e.g. those that are used only when cuts are generated,
     *  are skipped to be detected.
     */
    void classify(bool bPlus=true);

    /**
     * The procedure builds linked lists of constraint indices
     *  that are written in the array `m_ipCtrLink`.
     * For example, the linked list of all knapsack constraints
     *  starts with the constraint indexed by `i(0)=m_iLastKnapsack`,
     *  the next knapsack constraints are `i(1)=m_ipCtrLink[i(0)],...,i(k)=m_ipCtrLink[i(k-1)]`
     *  with `m_ipCtrLink[i(k)=-1` indicating the last constraint in the list.
     *  If `m_iLastKnapsack=-1`, then there is no knapsack constraint.
     *  \param[in] bUpdate if set to `true`, currently existing list is updated;
     *  otherwise, the list is build from scratch.
     */
    void buildCtrLists(bool bUpdate=false);

///// PREPROCESSING //////////////////////////////////////

    /**
     * Substitutes variable `x[col]` with the expression `b + sum(j in 0..sz-1) dpVal[j]*x[ipCol[j]]`.
     * \param[in] j variable to be deleted;
     * \param[in] hd handle of deleted variable, or `hd in {-1,-2}`;
     *   if `hd = -1`, then `sz = 1` and variable `x[ipCol[0]]` is substituted for `x[ipCol[0]]*dpVal[0]+b`;
     *   if `hd = 2`, then `sz = 1`, and, for `c=reinterpret_cast<int*>(dpVal)[0]`,
     *   if `x[ipCol[0]] >= 0.0`, then `x[c]=0.0`; otherwise, `x[c]=-x[ipCol[0]]` and `x[ipCol[0]]=0.0`;
     * \param[in] b free term in the substitution
     * \param[sz] sz number of variables in the substitution;
     * \param[in] ipCol list of `sz` variables;
     * \param[in] dpVal list of `sz` coefficients.
     */
    void deleteVariable(int j, int hd, double b, int sz, int *ipCol, double *dpVal);

    /**
     * The function calls `reduceCoefficients()` to tighten the constraint given by its arguments,
     * and then classifies the resulting inequality.
     * \param[in,out] sz number of variables in constraint to be classified; on output, `sz` may be smaller because some coefficients may be zeroes;
     * \param[in,out] dpVal,ipCol arrays of size `sz`, `dpVal[i]` is coefficient for variable `ipCol[i]`;
     * \param[in,out] b left or right hand side of constraint;
     * \param[in] side if `true` (resp., `false`), `b` is right (resp., left) hand side;
     * \param[in] makeIntCoeff if `true`, `makeCutCoeffIntegral()` is called.
     * \return constraint type.
     */
	int classifyAndReduceCoeff(int &sz, double* dpVal, int* ipCol, double&  b,  bool side, bool makeIntCoeff=false);

	/**
	 * The function tries to make integral the coefficients of the cut given by first four arguments:
	 *     `sum(i in 0..sz) dpVal[i]*x[ipCol[i]] <= (>=) b`.
	 *
     * \param[in] size number of variables in constraint to be classified;
     * \param[in,out] dpVal,ipCol arrays of size `sz`, `dpVal[i]` is coefficient for variable `ipCol[i]`;
     * \param[in,out] b right hand side of constraint;
     * \param[in] side if `true`, the sign in of the input inequality is `<=`; otherwise, the sign is `>=`.
     * \return constraint type (bitwise OR of `enVarType` members).
	 */
	unsigned simplifyCut(int &size, double *dpVal, int *ipCol, double &b, bool side=true);

//////////////// P R O B I N G
	/**
	 * To speed up probing of binary variables, not all constraint
	 * are processed at probing rounds. Usually, only constraints of small size
	 * and some structured constraints - say, packing constraints - are useful
	 * when probing binary variables.
	 * \param[in] r row index;
	 * \param[in] dpBd for row `r`, `dpBd[r<<1]` and `dpBd[(r<<1)+1]` are, respectively,
	 *            lower and upper bounds on left-hand-side value of constraint `r`.
	 * \return `true` is the constraint indexed by `r` is to be probed.
	 */
		bool isCtrUsedInProbing(int r, double * dpBd);

	/**
	 * When a binary variable is fixed, this procedure is called in probing subroutines
	 * to derive implications from the implication matrix.
	 *
	 * \param[in] row row in implication matrix;
	 * \param[in,out] last last constraint in list (queue) of active and non-processed constraints;
	 * \param[in,out] ipCtrLst list of active and non-processed constraints;
	 * \param[in,out] cpActiveCtr for row `r`, `cpActiveCtr[r]` is `true` if `r` is in `ipCtrLst`;
	 * \param[in,out] dpBd for row `r`, `dpBd[r<<1]` and `dpBd[(r<<1)+1]` are, respectively,
	 *            lower and upper bounds on left-hand-side value of constraint `r`;
	 * \param[in,out] dpD, for column `j`, `dpD[j<<1]` and `dpD[(j<<1)+1]` are, respectively,
	 *            lower and upper bounds for variable `j`;
	 * \param[in] round probing round;
	 * \param[in,out] cpRound for column `j`, `cpRound[j<<1]` and `cpRounf[(j<<1)+1]` are
	 *       last probing rounds when, respectively, lower and upper bounds for variable `j` changed.
	 * \return `false` if propagation results in any inconsistency; otherwise, `true`.
	 */
	bool propagateImplications(int row, int& last, int *ipCtrLst, char *cpActiveCtr,
			double* dpBd, double* dpD, int round, unsigned char *cpRound);

	/**
	 * The procedure is called only within `PROBE_preprocessCtr()` when the lower or upper bound on variable `col` was changed.
	 * \param[in] m number of rows in preprocessed submatrix;
	 * \param[in] col index of variables whose bound was changed;
	 * \param[in] side1 if `true`, lower bound of variable `col was changed;
	 * \param[in] side2 if `true`, upper bound of variable `col was changed;
	 * \param[in] delta1 value by which lower bound of `col` was changed;
	 * \param[in] delta2 value by which upper bound of `col` was changed;
	 * \param[in] round iterate (round) of `probe()`;
	 * \param[in,out] last index of last constraint in list of active (to be preprocessed) constraints;
	 * \param[in,out] ipCtrLst list (of maximum size `m`) of active constraints;
	 * \param[in,out] cpActiveCtr constraint `r` is active if `cpActiveCtr[r]==1`;
	 * \param[in,out] dpBd array of size `m`; `dpBd[r<<1]` and `dpBd[(r<<1)+1]` are respectively lower and upper values of constraint `r`.
	 * \return `false` if any inconsistency has been detected; otherwise, `true`.
	 */
	bool correctBounds(int m, int col, bool side1, bool side2, double delta1, double delta2,
			 int round, int& last, int *ipCtrLst, char *cpActiveCtr, double* dpBd);

	/**
	 * The procedure is called from `probe()` to preprocess a given constraint, i.e.,
	 * to improve bounds on variables and the constraint being processed.
	 * \param[in] ind index of constraint;
	 * \param[in,out] last constraint in queue (given by array `ipCtrLst`) of  constraints to be preprocessed;
	 * \param[in,out] ipCtrLst queue of  constraints to be preprocessed;
	 * \param[in,out] cpActiveCtr if `cpActiveCtr[r]`, then inequality `r` belongs to
	 *  queue of  constraints to be preprocessed;
	 * \param[in,out] dpBd `dpBd[r<<1]` and `dpBd[(r<<1)+1]` are, respectively, lower and upper bound on value of constraint `r`;
	 * \param[in,out] dpD `dpD[j<<1]` and `dpD[(j<<1)+1]` are, respectively, lower and upper bound on value of variable `j`;
	 * \param[in] round preprocessing round;
	 * \param[in,out] cpRound value of `cpRound[j<<1]` (resp., `cpRound[(j<<1)+1]`) is last round at which
	 * lower (resp., upper) bound on variable `j` has been changed.
	 * \return `false` if constraint `ind` is infeasible subject to the bounds on variables written in `dpD`;
	 * otherwise, the procedure returns `true`.
	 */
	bool PROBE_preprocessCtr(int ind, int& last, int *ipCtrLst, char *cpActiveCtr,
        double* dpBd, double* dpD, int round, unsigned char *cpRound);

	/**
	 * The procedure at each preprocessing _round_ repeatedly calls `PROBE_preprocessCtr()`
	 *  to preprocess constraints from the queue given by its three first parameters.
	 *  `PROBE_preprocessCtr()` adds to the queue new constraints to be preprocessed in the next processing round.
	 * \param[in,out] last constraint in queue (given by array `ipCtrLst`) of  constraints to be preprocessed;
	 * \param[in,out] ipCtrLst queue of  constraints to be preprocessed;
	 * \param[in,out] cpActiveCtr if `cpActiveCtr[r]`, then inequality `r` belongs to
	 *  queue of  constraints to be preprocessed;
	 * \param[in,out] dpBd `dpBd[r<<1]` and `dpBd[(r<<1)+1]` are, respectively, lower and upper bound on value of constraint `r`;
	 * \param[in,out] dpD `dpD[j<<1]` and `dpD[(j<<1)+1]` are, respectively, lower and upper bound on value of variable `j`;
	 * \param[in,out] cpRound value of `cpRound[j<<1]` (resp., `cpRound[(j<<1)+1]`) is last round at which
	 * lower (resp., upper) bound on variable `j` has been changed;
	 * \param[in] implRow row in implication matrix.
	 * \return `false` if constraint `ind` is infeasible subject to the bounds on variables written in `dpD`;
	 * otherwise, the procedure returns `true`.
	 */
	bool probe(int& last, int *ipCtrLst, char *cpActiveCtr,
		double *dpD, double *dpBd, unsigned char *cpRound, int implRow=-1);

	/**
	 * The procedure prepares a `CImpl` object for storing implications between binary variables,
	 * as well as variable bounds.
	 * \param[in] impl pointer to `CImpl` object.
	 */
	void probeInit(CImpl *impl);

	void updateRowBds(int c, bool side, double delta, double *dpBd);

	/**
	 * This procedure is called after probing a binary variable to tighten constraints.
	 * \param[in] c column index of binary variable having been probed;
	 * \param[in] side if `true`, variable `c` was fixed to `1`; otherwise, to `0`.
	 * \return number of tightened constraints
	 */
	int tightenCtrs(int c, bool side);

	/**
	 * After probing a binary variable `c1`, the procedure adds a new lower variable bound, `x[c2]-a*x[c1] >= b`, on variable `c2`.
	 * \param[in] c1 variable having been probed;
	 * \param[in] c2 dependent variable whose lower bound has been increased when probing variable `c1`;
	 * \param[in] side `true` if `x[c1]=1` when probing; otherwise, value of `side` is `false`.
	 */
	void addNewLoVarBound(int c1, int c2, bool side);

	/**
	 * After probing a binary variable `c1`, the procedure adds a new upper variable bound, `x[c2]-a*x[c1] <= b`, on variable `c2`.
	 * \param[in] c1 variable having been probed;
	 * \param[in] c2 dependent variable whose upper bound has been increased when probing variable `c1`;
	 * \param[in] side `true` if `x[c1]=1` when probing; otherwise, value of `side` is `false`.
	 */
	void addNewUpVarBound(int c1, int c2, bool side);

	/**
	 * This procedure adds to the matrix inequalities for implications derived when probing a binary variable.
	 * \param[in] c column index of binary variable having been probed;
	 * \param[in] side if `true`, variable `c` was fixed to `1`; otherwise, to `0`;
	 * \param[out] varBdAdded  number of variable bounds added to constraint matrix;
	 * \param[out] implAdded number of implications generated.
	 * \remark the procedure is used only in rare cases when storing implications in a `CImpl` object requires to much memory.
	 */
	void processImplications(int c, bool side, int &varBdAdded, int &implAdded);

	/**
	 * This procedure is called to further process implications derived when probing a binary variable.
	 * \param[in] c column index of binary variable having been probed;
	 * \param[in] side if `true`, variable `c` was fixed to `1`; otherwise, to `0`;
	 * \param[in] impl pointer to `CImpl` object that stores implications.
	 * \param[out] varBdAdded  number of variable bounds added to implication matrix;
	 * \param[out] implAdded number of implications added to implication matrix.
	 * \return  number of variable bounds added.
	 */
	void processImplications(int c, bool side, CImpl *impl, int &varBdAdded, int &implAdded);

	/**
	 * The procedure implements probing down or up a given variable.
	 * \param[in] c column index of binary variable having been probed;
	 * \param[in] side if `true`, variable `c` was fixed to `1`; otherwise, to `0`;
	 * \param[out] varBdAdded number of variable bounds added;
	 * \param[out] ctrTightened number of constraints having been tightened;
	 * \param[out] implAdded number of implications added to implication matrix.
	 * \return `false` if some inconsistencies were detected (constraints are infeasible); otherwise, `true`.
	 */
	bool probeVar(int c, bool side, int &varBdAdded, int &ctrTightened, int &implAdded);

	/**
	 * The procedure substitutes variable `x[k]` for the expression `a*x[j]+b`.
	 * \param[in] k,j indices of variables;
	 * \param[in] a,b coefficients in expression `a*x[j]+b`.
	 */
	void substituteVar(int k, int j, double a, double b);

	/**
	 * \param[in] round current preprocessing iterate;
	 * \param[in] timeLimit maximum running time (in seconds);
	 * \param[out] probeVarNum number of variables having been probed;
	 * \param[out] varFixed number of fixed variables;
     * \param[out] ctrTightened number of modified constraints;
     * \param[out] varBdAdded number of variable bounds added;
     * \param[out] implAdded number of implications added to implication matrix.
	 * \return `false` if problem domain is empty.
     */
	bool probeVars(int round,__LONG timeLimit, int &probeVarNum,
			int &varFixed, int &ctrTightened, int &varBdAdded, int &implAdded);

	/**
	 * The procedure selects a list of variables to be probed.
	 * \param[in] round current preprocessing iterate;
	 * \param[out] ipVars list of `sz` variables to be preprocessed, where `sz` denotes return value.
	 * \return number of variables to be preprocessed.
	 */
	int selectVarsForProbing(int round, int *ipVars);

	bool probePackingCtr(int row, bool rowSide); ///< The procedure checks whether a given packing inequality can be turned into equality constraint.

	int probePackings(); ///< The procedure calls `probePackingCtr()` for each packing inequality.

	/**
	 * The procedure calls `selectVarsForProbing()` to select a list of variables to be probed,
	 * and then calls `probeVars()` to probe variables from that list.
	 * \param[in] timeLimit maximum running time (in seconds);
	 * \param[in] timeLimitPerRound maximum running time (in seconds) for each probing round.
	 * \return `false` if some inconsistencies were detected (constraints are infeasible); otherwise, `true`.
	 */
	bool probing(__LONG timeLimit, __LONG timeLimitPerRound);

protected:
	/**
	 * The procedure displays preprocessing statistics.
	 * \param[in] timeStr string representation of time elapsed since MIPCL have been started;
	 * \param[in] round probing round; if `round < 0`, round field should not be displayed;
	 *            if `round==0`, only header is displayed;
	 * \param[in] probeVarNum number of variables having been probed;
	 * \param[in] varFixed number of variables having been fixed;
	 * \param[in] ctrTightened number of constraints having been tightened;
	 * \param[in] varBdAdded number of new variable bounds having been added;
	 * \param[in] implications number of entries in implication matrix.
	 */
	virtual void probingInfo(char* timeStr, int round,
			int probeVarNum,int varFixed,int ctrTightened,int varBdAdded, int implications);

private:
///////////////////////////////////////////
	/*
	 * Columns are parallel (opposite) if their entries are identical (of different signs)
	 * in all the rows but variable bounds.
	 * \param[in] col1,col2 column indices.
	 * \return `1` if columns are parallel; `-1` if columns are opposite; otherwise, `0`.
	 * \see detectParallelColumns().
	 */
	int areColumnsParallel(int col1, int col2);

	/**
	 * The procedure computes hash values for every matrix column,
	 * and then, for each pair of columns with equal hash values,
	 * calls `areColumnsParallel()` to detect parallel columns.
	 * \return number of columns (variables) deleted.
	 */
	int detectParallelColumns();

	int detectRowsWithParallelFracParts();

	int processMonotoneVars();

	/**
	 * The procedure _disaggregates_ a given knapsack inequality:
	 *      - sum(i in 0..ctrSize-1) dpVal[i]*x[ipCol[i]] <= b.
	 * In other words, the input inequality is substitutes for a number of inequalities
	 * that together imply the input inequality.
	 *
	 * For example, let us consider the knapsack inequality
	 *     - x_1 + 3x_2 + 4x_3 + 2x_4 <= 8
	 * If we divide it by `k=2` (`k` must be a divider of `b=8`),
	 * we see that the sum of fractional coefficients is not greater than `1`;
	 * therefore, the input inequality is a convex combination of `k` valid constraints
	 *     - x_1 + x_2 + 2x_3 + x_4 <= 4 and 2x_2 + 2x_3 + x_4 <= 4.
	 * Further, the latter inequality is simplified to x_2 + x_3 + x_4 <= 2.
	 *
	 * \param[in] ctrSize number of entries in input inequality;
	 * \param[in] dpVal,ipCol arrays of size `ctrSize`,
	 * \param[in] b right hand side.
	 * \return `true` if input constraint has been disaggregated.
	 */
	bool disaggregateKnapsack(int ctrSize, double *dpVal, int *ipCol, double b, char *cpReversed);

	/**
	 * The procedure just calls `disaggregateKnapsack()` for each knapsack constraint.
	 * \return number of _disaggregated_ knapsack constraints.
	 */
	int disaggregateKnapsacks();

	int roundToMakeStronger();

	/**
	 * The procedure just calls `makeCoeffIntegral()` for each row.
	 * \return always `true`.
	 */
	bool preprocessInit();

	/**
	 * The procedure reformulate a MIP by applying a number of preprocessing techniques (including probing).
	 * \return `false` in case the problem being solved has been proven to be inconsistent.
	 */
	bool preprocessPlus();

	void BranchAndCutWorker(); ///< This procedure implements a branch-and-cut algorithm.

	/**
	 * When `BranchAndCut()` terminates, it calls `fixSolutionState()`
	 * to assign a value to `m_iState`, which is the bitwise OR of the members of the enumeration `enProbState`.
	 */
    void fixSolutionState();
	//////
public:
    /**
     * This procedure does a lot of useful things: allocates memory, applies different preprocessing techniques,
     * scales the matrix, and so on. In many cases, we can get substantially simpler (for solving)
     * problem, which is still equivalent to the original (user) problem.
     * The latter means that, given an optimal solution to the transformed problem, we can
     * easily compute an optimal solution to the original problem.
     * \return `false` if inconsistency has been detected; otherwise, `true`.
     * \throws CMemoryException lack of memory.
    */
    bool prepare();

    /**
     * The function resumes the state in which the problem was before the solution procedure has started.
     */
    void reset();

    /**
     * The procedure creates a number of threads each of which calls `BranchAndCutWorker()`
     * that implements the branch-and-cut/price algorithm.
     * Each instance of BranchAndCutWorker() stops if duality (integrality) gap
     * (i.e., difference between upper and lower bounds on the optimal objective value) is less than the value of `gap`.
     * \param[in] timeLimit limit on solution time (in seconds);
     * \param[in] gap duality gap.
     * \sa optimize(), setDualGap().
     */
    void BranchAndCut(__LONG timeLimit=10000000l, double gap=0.0);

    /**
     * The procedure solves the root LP, and then, if its solution is not integral, calls `BranchAndCut()`.
     * \param[in] timeLimit limit on solution time (in seconds);
     * \param[in] gap integrality gap;
     * \param[in] solFileName pointer to string with file name for storing intermediate solutions; if `solFileName=0`, intermediate solutions are not stored.
     * \sa CLP::optimize(), BranchAndCut(), setDualGap().
     */
    void optimize(__LONG timeLimit=10000000l, double gap=0.0, const char *solFileName=0);

    /**
     * The function returns two pointers to the internal __MIPCL__ arrays storing the solution found so far.
     * \param[out] dpX,ipHd `dpX[j]` is the value of variable whose handle is `ipHd[j]`, `j=1,...,n`,
     *    where `n` is the return value. Do not modify the values of both arrays  `dpX` and `ipHd`.
     * \return number of variables.
     * \sa CLP::getSolution().
     */
    int getSolution(double* &dpX, int* &ipHd);

    /**
     * \return `true` if at least one feasible solution has been found so far.
     */
    bool isSolution() const;

    /**
     * First, `isSolution()` is called; if it returns `true`, then
     * we can call this function to verify whether
     * the solution found by __MIPCL__ is optimal.
     * \return `true`, if an optimal solution has been found.
     */
    bool isSolutionOptimal() const
	{return (m_iState & PROB_OPTIMAL)? true: false;}

    /**
     * \return `true` if this MIP is infeasible (has no solution).
     */
	bool isInfeasible() const
	{return (m_iState & PROB_INFEASIBLE)? true: false;}

    /**
     * \return `true` if solution procedure stopped after exceeding given time limit.
     */
	bool timeLimitStop() const
	{return (m_iState & PROB_TIME_LIMIT)? true: false;}

    /**
     * \return currently upper bound on the objective value defined to be
     * the largest value of the nodes LPs among all search tree leaves.
     */
    double getObjBound() const;

	/**
	 * The procedure writes MIP solutions into the file.
	 * The user can overload this function to store solutions in an appropriate way.
	 * \param[in] fileName name of the file to store solutions; if `fileName=`0`,
	 * the solver makes up the file name by appending the extension ".sol" to the name of the problem being solved.
	 */
    virtual void printSolution(const char* fileName=0);

protected:
    /**
     * To generate problem specific cuts, __MIPCL__ allows the user to overload any of three virtual functions:
     * `separate()` (a member of CLP class), `genCut1()`, and `genCut2()`.
     * The solver in turn calls `separate()`, `genCut1()`,
     * its internal cut generating procedure, and then `genCut2()`.
     * Both, `genCut1()` and `genCut2()` are called only if their predecessors failed to produce any cut.
     *
     * Rule of Thumb:
     *  - `separate()` produces all the inequalities that constitute a part of the problem formulation
     *    (for example, TSP cut inequalities);
     *  - strong (facet defining and etc.) cuts are generated by `genCut1()`
     *    (for example, TSP comb inequalities);
     *  - less stronger cuts must be generated by `genCut2()`.
     *
     * \param[in] n number of variables;
     * \param[in] dpX,ipColHd arrays of size `n` that represent a solution to cut off;
     *     `dpX[i]` is the value of the variable with handle `ipColHd[i]`.
     * \return `true` if at least one cut has been detected
     * \sa  `genCut2()`, `CLP::separate()`.
     */
    virtual bool genCut1(int n, const double* dpX, const tagHANDLE* ipColHd);

    /**
     * Similar to `genCut1()`, except that `genCut2()` is called only if all the other cut generating
     * procedures failed to produce any cut.
     * \sa genCut1().
     */
    virtual bool genCut2(int n, const double* dpX, const tagHANDLE* ipColHd);

    /**
     * This function must be overloaded in any derived class
     *  that implements its own pool for storing generated inequalities.
     * When a new node is added to the search tree,
     *  `lockCtr()` is called for any constraint
     *  generated by the derived class cut generating procedures (i.e. `separate()`, `genCut1()`, and `genCut2()`)
     *  unless that constraint was sent to the __MIPCL__ pool.
     *  Properly implemented two functions, `lockCtr()` and `unlockCtr()`, allow derived classes
     *  to delete, if necessary, unused cuts from their pools.
     *  \param[in] hd constraint handle.
     *  \sa unlockCtr(), delNodeLocalCtrs().
     */
    virtual void lockCtr(tagHANDLE hd)
        {}

    /**
     * This function must be overloaded in any derived class
     *  that implements its own pool for storing generated inequalities.
     * When a new node is extracted from the search tree,
     *  `unlockCtr()` is called for any constraint
     *  generated by the derived class cut generating procedures (i.e. `separate()`, `genCut1()`, and `genCut2()`)
     *  unless that constraint was sent to the __MIPCL__ pool.
     *  Properly implemented two functions, `lockCtr()`, `unlockCtr()`, and `delNodeLocalCtrs()` allow derived classes
     *  to delete, if necessary, unused cuts from their pools.
     *  \param[in] hd constraint handle.
     *  \sa lockCtr(), delNodeLocalCtrs().
     */
	virtual void unlockCtr(tagHANDLE hd)
        {}

    /**
     * This function must be overloaded in any derived class
     *  that implements its own pool for storing generated columns.
     * When a new node is added to the search tree,
     *  `lockColumn()` is called for any column
     *  generated by the derived class column generating procedure (i.e., generateColumns()
     *  unless that column was sent to the __MIPCL__ pool.
     *  Properly implemented two functions, `lockColumn()`, `unlockColumn()`, and `delNodeLocalColumns()`
     *  allow derived classes to delete, if necessary, unused columns from their pools.
     *  \param[in] hd column handle.
     *  \sa unlockColumn(), delNodeLocalColumns().
     */
    virtual void lockColumn(tagHANDLE hd)
        {}

    /**
     * This function must be overloaded in any derived class
     *  that implements its own pool for storing generated columns.
     * When a new node is extracted from the search tree,
     *  `unlockColumn()` is called for any column
     *  generated by the derived class column generating procedure (i.e., `generateColumns()`
     *  unless that column was sent to the __MIPCL__ pool.
     *  Properly implemented two functions, `lockColumn()` and `unlockColumn()`, and `delNodeLocalColumns()`
     *  allow derived classes to delete, if necessary, unused columns from their pools.
     *  \param[in] hd column handle.
     *  \sa lockColumn(), delNodeLocalColumns().
     */
    virtual void unlockColumn(tagHANDLE hd)
        {}

    /**
     * This function must be overloaded in any derived class
     *  that implements its own pool for storing generated inequalities.
     * When a node is pruned, `delNodeLocalCtrs()` is called to allow the derived class
     * to eliminate, if necessary, all the constraints that are local for node `nd`.
     * \param[in] nd pruned node.
     * \sa lockCtr(), unlockCtr().
     */
	virtual void delNodeLocalCtrs(int nd) {};

	/**
     * This function must be overloaded in any derived class
     *  that implements its own pool for storing generated columns.
     * When a node is pruned, `delNodeLocalColumns()` is called to allow the derived class
     * to eliminate, if necessary, all the columns that are local for node `nd`.
     * \param[in] nd pruned node.
     * \sa lockColumn(), unlockColumn().
     */
	virtual void delNodeLocalColumns(int nd) {};

//////////////////////////////////////////////////////////////////////////
//                    SOS feasibility and branching                     //
//////////////////////////////////////////////////////////////////////////
public:
	/**
	 * The function is called to switch on/off SOS1-branching. Initially, SOS1-branching is switched off.
	 * \param[in] flag if `flag` set to `true`, SOS1-branching is activated; otherwise, SOS1-branching is switched off.
	*/
    void switchSOS1branching(bool flag=true)
        {m_bSOS1branching=flag;}

private:
    /**
     * \param[out] row violated SOS1-constraint;
     * \param[out] l position in row `row`; in left branch all variables in positions preceding `l` are set to `0`,
     * while in right branch  all variables in positions following `l` are set to `0`.
     * \return `true` if violated SOS1-constraint has been detected.
     */
    bool SOS1_Branching(int& row, int& l);

    /**
     * The procedure decides on which SOS2-constraint to perform branching.
     * \param[out] l if return value, say `r` is non-negative,
     *  `l` is position in row `r`; in left branch all variables in positions preceding `l` are set to `0`,
     * while in right branch  all variables in positions following `l` are set to `0`.
     * \return if non-negative, index of violated SOS2-constraint; otherwise, all SOS2-constraints are satisfied.
     */
    int SOS2_Branching(int& l);

    /**
     * \param[in] X current node LP solution; i.e., `X[i]` is value of variable with handle `m_ipColHd[i]`.
     * \returns  `true` if all SOS2-constraints are satisfied.
     */
    bool isSOS2Feasible(double* X);

    /**
     * The function picks up a free node in the search tree.
     * \param[out] height of node being returned.
     * \return node index.
     */
    int getNode(int &height);

protected:
    /**
     * The procedure decides how to branch at the node being processed.
     * It is called by the solver when it starts processing a new node.
     * Overloading `startBranching()` together with updateBranch(), allows the user
     * to take full control on the branching process.
     * \param[in] nodeHeight height (in the search tree) of the currently processed node.
     * \return number of branches.
     * \sa updateBranch().
     */
    virtual int startBranching(int nodeHeight);

    /**
     * The function sets new bounds for integer variables or/and adds new constraints to the matrix
     * to transform the parent node problem into the problem of branch `i`.
     * \param[in] i branch index, when `i` is nonnegative integer that is less than the value returned by `startBranching()`.
     * \return `false` if inconsistency has been detected; otherwise, `true`.
     * \sa startBranching().
     */
    virtual bool updateBranch(int i);

    /**
     * There are practical applications with some requirements that are difficult to be expressed
     * by means of inequalities. As alternative, we can implement
     * a special branching strategy that eliminates all the solutions not valid for those difficult constraints.
     * In such a case, we need to overload three functions: `startBranching()`, `updateBranch()`, and `isFeasible()`.
     *
     * Any time when a new solution has been computed that satisfies all the constraints known to the solver,
     * `isFeasible()` is called to verify whether that solution is really feasible for the problem being solved.
     * \param[in] n number of variables;
     * \param[in] dpX,ipColHd for `j=1,...,n`, `dpX[j]` is the value of variable with handle `ipColHd[j]`.
     * \return `true` if the solution given by `dpX` and `ipColHd` is feasible; otherwise, `false`.
     * \sa startBranching(), updateBranch().
     */
    virtual bool isFeasible(int n, const double* dpX, const tagHANDLE* ipColHd)
        {return true;}


private:
    /**
     * The function first calls `updateBranch()` and then tries to propagate changes done by `updateBranch()`.
     * \param[in] i branch number (numbering starts from `0`).
     */
    bool _updateBranch(int i);

#ifdef __PSEUDOCOSTS_
    /**
     * The procedure is called from `strongBranching()` to build a list of integral variables with fractional values.
     *     - If there are variables on which branchings have been done more that 4 times, and if during those branchings
     *       either the objective had increased or inconsistency had been detected,
     *       one of such variables with maximum score (computed based on pseudocosts) is selected for branching.
     *     - If for all variables on which branchings have been done more that 4 times,
     *       neither the objective had increased nor inconsistency had been detected,
     *            + a list of variables on which branchings have been done less that 4 times
     *              is generated;
     *            + if for all variables branchings have been done more that 4 times,
     *              one of variables with maximum score (computed based on the current basis) is selected for branching.
     * \param[in] maxSize maximum number of variables in return list;
     * \param[out] ipLst list of `k` integral variables taking fractional value, where `k` is return value.
     * \returns size of returned list.
     */
    int selectFracListUsingPseudocosts(int maxSize, int *ipLst);

    /**
     * The procedure does not use pseudocosts to build a list of fractional variables.
     * In particular, this procedure is used in place of `selectFracListUsingPseudocosts()`
     * when columns are generated.
     * \param[in] maxSize maximum number of variables in return list;
     * \param[out] ipLst list of `k` integral variables taking fractional value, where `k` is return value.
     * \returns size of returned list.
     */
    int selectFracListWithoutPseudocosts(int maxSize, int *ipLst);
#endif

    /**
     * The procedure is used in `strongBranching()` to build a list of fractional variables.
     * \param[in] maxSize maximum number of variables in return list;
     * \param[out] ipLst list of `k` integral variables taking fractional value, where `k` is return value.
     * \returns size of returned list.
     */
	int selectFracList(int maxSize, int *ipLst);

	/**
	 * The procedure implements strong branching.
	 * \param[in] nodeHeight height of processed node.
	 * \return `0` if everything was OK.
	 */
	int strongBranching(int nodeHeight);

#ifdef __PSEUDOCOSTS_
	/**
	 * An implementation of `getFractional()` when pseudocosts are used.
	 * \param[out] d current value of variable `j` which is return value.
	 * \return variable `j` to split its domain into two parts, `[m_dpD[j<<1],d]` and `[d+1,m_dpD[(j<<1)+1]]`
     * if `j` is negative, then no fractional variable has been detected.
	 */
	int getFractionalUsingPseudocosts(double& d);

	/**
	 * An implementation of `getFractional()` when pseudocosts are not used.
	 * \param[out] d current value of variable `j` which is return value.
	 * \return variable `j` to split its domain into two parts, `[m_dpD[j<<1],d]` and `[d+1,m_dpD[(j<<1)+1]]`
     * if `j` is negative, then no fractional variable has been detected.
	 */
	int getFractionalWithoutPseudocosts(double& d);
#endif

protected:
	/**
	 * The procedure selects a variable to branch on it.
	 * \param[out] d current value of variable `j` which is return value.
	 * \return variable `j` to split its domain into two parts, `[m_dpD[j<<1],d]` and `[d+1,m_dpD[(j<<1)+1]]`
     * if `j` is negative, then no fractional variable has been detected.
	 */
    virtual int getFractional(double& d);

    ///< User defined prime heuristic.
    /**
     * The function is used to round a non-integer solution stored in `dpX` and `ipColHd`.
     * \param[in] n size of `dpX` and `ipColHd` (number of variables);
     * \param[in,out] dpX `dpX[i]` is the value of the variable with handle `ipColHd[i]`;
     * \param[in] ipColHd stores handles of the variables;
     * \param[in,out] objVal on input, upper (for maximization problem) or lower (minimization) bound on objective value;
     * \return `true` if the procedure succeeded in constructing
     * a feasible solution ( stored in `dpX` and `ipColHd`) with objective value better than input value of `objVal`.
     */
    virtual bool roundSolution(double& objVal, int n, double* dpX, const int* ipColHd)
    {return false;}

public:
    /**
     * The function specifies a way of rounding optimal LP solutions.
     * \param[in] dir direction of rounding fractional components;
     * if `dir==ROUND_USER`, `roundSolution()` with four argument has to
     * be overloaded to implement problem specific rounding strategy.
     * \sa enRoundType, getRoundingType().
     */
    void setRoundingType(enRoundType dir)
        {m_eRoundType=dir;}

    /**
     * \return type (direction )of rounding fractional components.
     * \sa enRoundType, setRoundingType().
     */
    enRoundType getRoundingType() const
        {return m_eRoundType;}
private:
    /**
     * The function is called in` roundSolution()` to round the LP solution of currently processed node.
     * \param[out] n size of solution (+ variables from propagation stack) stored in `m_dpUd`.
     * \param[out] objVal objective value of new solution found.
     * \returns `true` if a new record solution has been  found .
     */
    bool roundX(int& n, double& objVal);

    /**
     * The function calls a user defined `roundSolution()` (with four arguments)
     * if `m_eRoundType == ROUND_USER`, otherwise, `roundX()` is called.
     * \param[out] objVal objective value of solution found (if any);
     * \param[out] n number of variables.
     * \sa `enRoundType`, `setRoundingDir()`, `roundX()`.
     */
    bool roundSolution(double& objVal, int& n);
    
    /**
     * The procedure uses dual variables to change bounds of the prime variables.
     * \return number of fixed binary variables.
     */
    int propagateByDualVars();

    /**
     * The procedure uses reduced costs and shadow prices to reduce lower and upper bounds
     * for variables and constraints when an upper bound on the objective is known.
     * The procedure is used only in `propagate()`.
     * \param[in,out] dpD array of size `2*m_iN`, where `dpD[j<<1], dpD[(j<<1)+1` are, respectively,
     * lower and upper bounds on variable `j`;
     * \param[in,out] dpB array of size `2*m_iM`, where `dpD[r<<1], dpD[(r<<1)+1` are, respectively,
     * lower and upper bounds for constraint `r`.
     *
     */
    void propagateByDualVars(double *dpD, double *dpB);
    
protected:
    /**
     * The user can overload this function to implement some problem specific propagation algorithm.
     * To access variable bounds, one can call the following functions:
     *  `getVarNum()`, `getVarHd()`, `getVarLoBound()`, `getVarUpBound()`.
     * Do not change variable bounds directly correcting values in `m_dpD`, instead,
     * use the functions `setVarLoBound()`, `setVarUpBound()`, or `setVarBounds()`.
     * \return `false` if inconsistency has been detected; otherwise, `true`.
     * \sa CLP::setVarLoBound(), CLP::setVarUpBound(), CLP::setVarBounds().
     */
    virtual bool propagate();
      
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// D I V I N G
///////////////////////
private:
    /**
     * The procedure is called in `dive()` to list all violated SOS2-constraints
     * in the arrays `m_ipArray` and `m_dpArray`.
     * \param[in,out] sz on input, starting position for the list in `ipCol`;  `sz-1` - position of the last element in the list;
     * \param[out] ipCol list of variables to be extended;
     * \param[out] dpVal weights assigned to variables listed in `ipCol`, `dpVal[ipCol[i]]` is weight assigned to variable `ipCol[i]`.
     */
    void SOS2_violations(int& sz, int *ipCol, double *dpVal);

    /**
     * The procedure is called in `dive()` for setting to zero all but 2 successive variables of SOS2-constraint `i`.
     * \param[in] i SOS2-constraint index.
     */
    void SOS2_round(int i);

    /**
     * The procedure is called in `dive()` when inconsistency is detected.
     * It tries to prevent inconsistency  detected in `dive()`
     * by reverting the value of a single fixed binary variable.
     * \param[in] maxObjDec only variables having reduced costs not greater than `maxObjDec` are
     * considered.
     * \return `true` in case of success.
     */
    bool DIVE_backTrace(double maxObjDec);

    /**
     * This procedure is similar to `DIVE_backTrace()` but its goal is
     * to change fixings of binary variable to reduce the objective of the LP being solved
     * at least by a given value.
     * \param[in] gap value by which objective be reduced.
     * \return `true` in case of success.
     */
    bool DIVE_worthObjBackTrace(double gap);

    /**
     * The procedure is called within `dive()` to fix the values of those integral
     * variables that are closed to their lower or upper bounds.
     * \return if non-negative, number of variables fixed, or `-1` in case of failure.
     * \remark procedure is used only in `dive()`.
     */
    int DIVE_fixVars();

    /**
     * Fixing an increasingly large number of variables until either
     *      - a new, best integral solution is found, a new incumbent,
     *      - or the fixings make the result LP infeasible,
     *      - or an objective value worse than the current incumbent.
     * \param[in] timeLimit limit on running time (in seconds).
     * \return `false` if the node was proven to be infeasible; otherwise, `true`.
     */
    bool dive(__LONG timeLimit);
// end of D I V I N G ///////////////////////////////

protected:
    /**
     * The function is called to prepare information for storing in the node of the search tree.
     * This function can be overloaded to exploit problem specifics and store node data more efficiently.
     * \param[out] ipVal array to write down node data.
     * \return number of bytes stored in `ipVal`.
     * \sa restoreNodeData().
     */
    virtual int storeNodeData(int* ipVal);

    /**
     * Taking as input the data stored by `storeNodeData()`,
     * the function must restore the node to be processed.
     * \param[in] ipVal data needed to restore the node.
     */
    virtual void restoreNodeData(int* ipVal);

    /**
     * The procedure is called any time a group of constraints (inequalities) are removed from the matrix.
     * This procedure must be overloaded in any derived class that generates inequalities and stores them in its own pool.
     * \param[in] sz number of constraints;
     * \param[in] ipCtr list of `sz` columns.
     */
    virtual void setCtrsInactive(int sz, int* ipCtr);

    /**
     * The procedure is called any time a group of columns are removed from the matrix.
     * This procedure must be overloaded in any derived class that generates columns and stores them in its own pool.
     * \param[in] sz number of columns;
     * \param[in] ipCtr list of `sz` constraints.
     */
    virtual void setColumnsInactive(int sz, int* ipCtr);

private:
    /**
     * When a new node starts processing,
     * the procedure adds constraints from the pool to the constant part (comprising first `m_iM0` rows) of the matrix.
     * \throws CMemoryException in multithreaded applications.
     */
    void restoreMatrix();

	/**
	 * The procedure is used only in `preprocessObjective()`.
	 * \param[in] j variable index;
	 * \param[in,out] dpD array of size `2*m_iN`, where `dpD[i<<1]` and `dpD[(i<<1)+1]`
	 * are lower and upper bounds on variable indexed by `i`;
	 * \param[in,out] objUpBound upper bound on objective value.
	 */
	void propagateVarBound(int j, double *dpD, double &objUpBound);

    /**
     * The procedure preprocesses the constraint `loBd <= c^Tx` (defined by the objective)
     * to reduce lower and upper bounds of variables.
     * \param[in] loBd current lower bound value.
     */
    void preprocessObjective(double loBd);

    /**
     * The procedure restores the LP for a given node in the search tree.
     * \param[in] iNode search-tree node to be restored;
     * \param[in] flag if `false`, only basis is restored.
     */
    void restoreSubproblem(int iNode, bool flag=true);

    /**
     * For a given node in the search tree, the procedure "unlocks" cuts and columns from the pool
     * that were used by the node. Unlocking cut (or column) means decrementing the counter of users (that are nodes)
     * of this cut (or column).
     * \param[in] ind index of node in search tree.
     */
	void releaseNode(int ind);

	/**
	 * The procedure deletes from the pool all the cuts that are local for a given search tree node.
	 * \param[in] ind index of search tree node.
	 */
	void delNodeLocals(int ind);

	void freeCurrentNode();

    void resetNodeParam(); ///< Resets some parameters for a new node of the search tree.

    void addCtrToPool(int i); ///< Adds constraint `i` to the pool.
    void addColumnToPool(int col);  ///< Adds column `col` to the pool.

    int resolveLP(double loBd); ///< truncated version of `solveNodeLP()` without cut generation and deleting rows.
    int solveNodeLP(); ///< The procedure is called in `processNode()` to solve the LP for the processing node.

#ifdef __PSEUDOCOSTS_
    /**
     * The procedure is called in `processNode()` and `_updateBranch()` to update `m_dpBrDec` and `m_ipBrNum` arrays.
     * \param[in] j column (variable) index;
     * \param[in] side if `true`, variable `j` was rounded up; otherwise, variable `j` was rounded down;
     * \param[in] bFeasible if `true`, node LP remained consistent after branching; otherwise, node LP became inconsistent;
     * \param[in] objInc objective increase.
     */
    void updateBranchingStats(int j, bool side, bool bFeasible, double objInc);
#endif

    /**
     * This procedure solves node LPs.
     */
    void processNode();

    /**
     * This procedure solves the root node LP.
     */
    void processRootNode();
    
    /**
     * \param[in] dpX array of size `m_iN`; `dpX[i]` is value of variable having handle `m_ipColHd[i]`.
     * \return `true` if solution `dpX` is integral.
     */
    bool isIntegral(double* dpX);

    /**
     * \return number of fractional components in the LP solution of the currently processed node.
     */
    int getFracVarNum() const;

    /**
     * The function does the same as `CLP:getPrimeSolution()` does, plus rounds off integer components.
     * \param[in] dpBasicX `dpBasicX[m_ipBasicColumn[i]]` value of basic variable `m_ipBasicCol[i]`, `i=0,...,m_BasisSize-1`;
     * \param[out] dpX array of size `n`, where `n` is return value, and `dpX[j]` is value of variable with handle `m_ipHd[j]`.
     * \return number of variables in the original (non-preprocessed) problem.
     */
    int getLpSolution(double* dpBasicX, double* dpX);

	/**
	 * The function is a wrapper to call `changeRecord()`, which may be overloaded in derived classes.
	 * \sa changeRecordVal().
	 */
	void _changeRecord(double objVal,
				int n, const double* dpX, const tagHANDLE* ipHd);
public:
	/**
	 * When a new solution is found which is better than the record solution,
	 *  `changeRecord()` procedure is called to replace the record solution for a new one.
	 *  Sometimes, e.g. when an initial solution is known, it is needed to change only the bound
	 *  on the objective value. In such cases, `changeObjBound()` should be called.
	 * \param[in] objVal new lower (for maximization problem) or upper (for minimization problem) bound,
	 *  which is usually objective value of best solution found so far.
	 * \sa changeRecord().
	 */
	void changeObjBound(double objVal);

	/**
	 * The best solution found so far is called a _record solution_, and its objective value is known as a _record_,
	 * which is a _lower_ or _upper_ bound on the optimal objective value.
	 * The procedure is called when a new best solution is found to store it as the record solution.
	 *  When solving structured problems (such as TSP), the user can overload `changeRecord()`
	 *  to store record solutions more efficiently.
	 * \param[in]  objVal objective value;
	 * \param[in] n number of variables;
	 * \param[in] dpX,ipHd solution, `dpX[j]` is value of variable with handle `ipHd[j]`, `j=1,...,n`.
	 * \sa changeObjBound().
	 */
	virtual void changeRecord(double objVal,
		int n, const double* dpX, const tagHANDLE* ipHd);

///////////////////////////////////////////////////////////////
//               CUT Generation
///////////////////////////////////////////////////////////////
protected:
	/**
	 * The procedure reduces size of the cut by removing some variables taking their bound values;
	 * this may make the cut local.
	 * \param[in] maxSize maximally allowed cut size;
	 * \param[in,out] sz size of arrays `ipCol` and `dpVal` (size of cut);
	 * \param[in,out] ipCol,dpVal,b inequality `sum(i=0..sz-1) dpVal[i]*x(ipCol[i]) <= b` represents both, input and output, cuts;
	 * \param[out] iFactor `sum(i=0..sz-1) r(i)`, where `frexp(fabs(m_dpVal[i]),r(i))`;
	 * \param[in,out] bLocal is `true` if resulting cut is local.
	 */
	void shrinkCut(int maxSize, int &sz, int *ipCol, double *dpVal, double &b, int &iFactor, bool &bLocal);

private:
	/**
	 * The procedure preprocesses a constraint previously generated as a cut.
	 * \param[in] row constraint index.
	 * \return number of fixed variables.
	 */
	int preprocSingleCtr(int row);

	/**
	 * The procedure preprocesses all the constraints previously generated as cuts.
	 * \param[in] r1 index of first constraint to be preprocessed;
	 * \param[in] r2 index of last constraint to be preprocessed;
	 * \return number of fixed variables.
	 */
	int preprocCtrs(int r1, int r2);

	/**
	 * The procedure extracts from the pool all the cuts that are valid for the currently processed node,
	 * and that are violated by the optimal solution found for this node.
	 *
	 * \return number of cuts extracted.
	 */
    int separateFromPool();

    /**
     * The procedure generate click cuts based on the implication matrix (a member of the `CImpl` class)
     * that is build when calling `probing()`.
     * \return number of cuts generated.
     */
	int genImpliedClicks();

///////////////////////////////////////////////////////////////////
// Variable Bounds
//////////////////////
    /**
     * The procedure generate variable lower and upper bound cuts stored in `m_pImpl`.
     * \return number of cuts generated.
     */
	int genImplVarBounds();

    /**
     * For a given non-binary variable, `getTightLoVarBound()` verifies
     * whether there exists a lower variable bound that is tight for
     * an optimal solution of the node LP being processed.
     *
     * \param[in] j index of non-binary variable;
     * \param[out] val,side if return value `c` is not `-1`, `x[j] <= val*x[c]` if `side==true`,
     * and `x[j] <= val*(1-x[c])` if `side==false`;
     * \param[out] local takes `true`-value only if the return variable bound is local.
     * \return index of binary variable, or `-1`.
     */
    int getTightLoVarBound(int j, double& val, bool &side, bool &local);

    /**
     * For a given non-binary variable, `getTightUpVarBound()` verifies
     * whether there exists an upper variable bound that is tight for
     * an optimal solution of the node LP being processed.
     * \param[in] j index of non-binary variable;
     * \param[out] val,side if return value `c` is not `-1`, `x[j] >= val*x[c]` if `side==true`,
     * and `x[j] >= val*(1-x[c])` if `side==false`;
     * \param[out] local takes `true`-value only if the return variable bound is local.
     * \return index of binary variable, or `-1`.
     */
    int getTightUpVarBound(int j, double& val, bool &side, bool &local);

    ////////////////////////////////////////////////////////////////////
//  K N A P S A C K  cuts
//////////////////////////
// !!!m_dpArray and m_ipArray are used when building cuts
    /**
     * The procedure implements an algorithm for lifting cover inequalities.
     * For the idea of lifting, see the description of `LCI()`.
     */
	void liftCover(int& sz, int n, int *pi,
		double b, double* dpA, int* ipAlpha);

	/**
	 * To find a cover inequality that separates a given point from a given knapsack set,
	 * `LCI()` solves a 0,1-knapsack problem; this computation may be time consuming.
	 * For knapsack sets of big size, `simpleLCI()` solves that knapsack problem approximately
	 * but much faster.
	 * \param[in] sz,dpA,ipCol,b input knapsack inequality `sum(i=0,...,sz-1) dpA[i]*x[ipCol[i]] <= b`;
	 * \param[in] dpX `dpX[j]` is value of variable `j`;
	 * \param[in] bLocal if `true`, input inequality is local;
	 * \param[out] rhs right hand side of returned inequality;
	 * \param[out] type type of returned inequality (bitwise OR of `enVarType` members);
	 * \return size of generated cut (let us denote it by 'cutSize)`, or '0' if no cut has been found.
	 * \remark The returned inequality (cut) is given as
	 *     'sum(i in 0..cutSize-1) m_dpArray[i]*x(m_ipArray[i]) <= rhs'.
	 */
	int simpleLCI(int sz, double* dpA, int* ipCol, double* dpX, double b, bool bLocal, double &rhs, unsigned &type);

	/**
	 * The procedure builds LCI cut based on the mixed knapsack
	 *      'sum(i in 0..sz-1) dpA[i]*x(ipCol[i]) <= b'.
	 * \param[in] sz size of mixed knapsack;
	 * \param[in] dpA constraint coefficients, array of size 'sz';
	 * \param[in] ipCol column indices, array of size 'sz';
	 * \param[in] dpX array of size `sz`, `dpX[i]` is value of variable `ipCol[i]`;
	 * \param[in] bLocal 'true` if input constraint is local; otherwise, 'false';
	 * \param[in,out] b right hand side value;
	 * \param[out] type type of returned constraint (bitwise OR of `enVarType` members).
	 * \return cut size, or '0' if no cut has been found.
	 */
	int mxSimpleLCI(int sz, double* dpA, int* ipCol, double* dpX, bool bLocal, double &b, unsigned &type);

	bool LGCI(int n, int n0, int n1, int nf,
			double* A, double* X, double b, int &iBeta, int *ipAlpha);

	/**
	 * For a given knapsack inequality, the procedure builds a lifted cover inequality.
	 *
	 * For the knapsack set `X={x in {0,1}^n: sum(j=1,...,n) a(j)*x(j) <= b}` with all `a(j)` greater than zero,
	 * a subset `I` from `{1,...,n}` is a _cover_ if `sum(j \in I) a(j) > b`;
	 * then the _cover_ inequality `sum(j in I) x(j) <= |I|-1` is valid for `X`.
	 * One can strengthen this cover inequality by _lifting_ it.
	 *
	 * The idea of lifting is as follows.
	 * Given an inequality `sum(j=1,...,n) u(j)*x(j) <= q` (some `u(j)` may be zeros) that are valid for `X`,
	 * if `sum(j=1,...,n) u(j)*x(j) <= s < q` for all `x in X` such that `x(j)=1`,
	 * then we can increase `u(j)` by `q-s`.
	 *
	 * A lifting algorithm is implemented in `liftCover()`.
	 *
	 * \param[in] i constraint index;
	 * \param[in] side if `true`, right hand side inequality of constraint `i` is used; otherwise,
	 * left hand side inequality is used;
	 * \param[out] rhs right hand side of returned inequality;
	 * \param[out] type type of returned inequality (bitwise OR of `enVarType` members).
	 * \return size of generated cut (let us denote it by 'cutSize)`, or '0' if no cut has been found.
	 * \remark The returned inequality (cut) is given as
	 *     'sum(i in 0..cutSize-1) m_dpArray[i]*x(m_ipArray[i]) <= rhs'.
	 * \sa liftCover(), GUB_LCI().
	 */
	int LCI(int i, bool side, double &rhs, unsigned &type);

	/**
	 * For a given constraint, the procedure builds a lifted GUB-cover inequality.
	 *
	 * Given a `X={x in {0,1}^n: sum(j=1,...,n) a(j)*x(j) <= b; sum(j\in J_i) x(j) <=1 for i=1,...,k}`,
	 * where all `a(j)` are greater than zero and `J_1 U J_2 U ... U J_k = {1,...,n}`.
	 * A subset `I` from `{1,...,n}` is a _GUB-cover_ if `sum(i=1,...,k) q(i) > b`,
	 * where `q(i)=max{a(j): j in I and j in J_i} if the intersection of `I` and `J_i` is not empty;
	 * otherwise, `q(i)=0`. Then the GUB-cover inequality
	 *     `sum(j in I) x_j <= (sum(i=1,...,k) sign(q(i))) -1`
	 * is valid for `X`.
	 *
	 * \param[in] i constraint index;
	 * \param[in] side if `true`, right hand side inequality of constraint `i` is used; otherwise,
	 * left hand side inequality is used;
	 * \param[out] rhs right hand side of returned inequality;
	 * \param[out] type type of returned inequality (bitwise OR of `enVarType` members).
	 * \return size of generated cut (let us denote it by 'cutSize)`, or '0' if no cut has been found.
	 * \remark The returned inequality (cut) is given as
	 *     'sum(i in 0..cutSize-1) m_dpArray[i]*x(m_ipArray[i]) <= rhs'.
	 * \sa LCI().
	 */
	int GUB_LCI(int i, bool side, double &rhs, unsigned &type);

	/**
	 * The procedure implements an iterator through the list of knapsack constraints.
	 * The iterator is used in `knapsackCuts()`.
	 * \param[in] row index of currently used knapsack constraint;
	 * to start search, set `row=-1`;
	 * \param[in,out] family if `1`, return value is index of knapsack constraint
	 * that is a parity constraint as well;
	 *  otherwise, return value is `0`.
	 * \return index of knapsack constraint that follows constraint with index `row`,
	 * negative return value indicates that `row` is the last knapsack constraint.
	 */
	int getNextKnapsack(int row, int &family);

	/**
	 * For each knapsack inequality, the procedure calls `LCI()` to generate a lifted cover inequality;
	 * in case of failure (`LCI()` returns `false`), `GUB_LCI()` is called to generate a GUB-cover inequality.
	 * \return number of cuts generated.
	 */
    int knapsackCuts();

	int lpCover(int n, double* A, double b, double* X, int* ipCov, double& lambda);

    /**
     * The procedure generate mixed knapsack cuts for sets
     * \f$X =\{(y,s)\in \{0,1\}^N\times R^2_+: \sum_{j\in N} a_j y_j - s_1 + s_2\le b\}\f$,
     * with \f$a_j > 0\text{ for }j\in N\f$.
     *
     * Let _C_ be a cover, \f$\sum_{j\in C} a_j = b + \lambda\f$,
     * $\bar{a} > \lambda > 0$, where  $\bar{a} = \max_{j\in C} a_j$.
     * Let \f$E(C) = C \cup \{j\in N\setminus C: a_j > \bar{a}\}\f$ be the extension of _C_.
     * Then the _mixed cover inequality_
     * \f$\sum_{j\in E(C)} \min(a_j,\lambda) y_j - s_1 \le -lambda + \sum{j\in C} \min(a_j,\lambda)\f$
     * is valid for \f$\mathrm{conv}(X)\f$.
     *
     * \param[in] szA number of variables in template constraint;
     * \param[in] szP number of rows used to derive template constraint;
     * \param[in,out] rhs right-hand side of template constraint, or returned cut;
     * \param[out] type type of cut inequality (bitwise OR of `enVarType` members);
     * \param[in] bLocal if `true`, template constraint is local; otherwise, it is global;
     * \return `true` size of cut, or '0' if no cut has been found.
     */
    int mixedKnapsackCut(int szA, int szP, double &rhs, unsigned &type, bool bLocal);

////////////////////////////////
/// Parity cuts
    /**
     * The procedure generate parity cuts by calling `parityCut()` for every parity constraint.
 	 * A _parity constraint_ is any constraint from which we can deduce
 	 * a _parity condition_ that the sum \f$\sum_{j \in S} x_j\f$ of binary variables is odd (even).
 	 * This parity condition is expressed by the following family of inequalities:
     *  \f{align*}{
     *     \sum_{j\in J} x_j - \sum_{j\in S\setminus J} x_j \le |J|-1,\quad \forall\; J\subseteq S,\; |J|\text{ is even (odd)}.
     * \f}
     *
     * \return number of cuts generated.
     */
    int parityCuts();

    /**
     * \param[in] gap initial gap;
     * \param[in] sz size of arrays `ipCol` and `dpVal`;
     * \param[in,out] ipCol,dpVal represent left part of either input inequality or output cut:
     *  `sum(i=0,...,sz-1) dpVal[i]*x(ipCol[i]) <= b`;
     * \param[in,out] b right-hand side of either input inequality or output cut.
     * \param[in,out] parity on input, parity of constraint (1 - even, 0 - odd);
     *  on output, type of cut generated.
     *  \return `true` if cut has been generated.
     */
    bool parityIneq(unsigned &parity, double gap, int sz, int *ipCol, double *dpVal, int &b);

    /**
     * The procedure tries to produce a parity cut for a given parity inequality.
     * \param[in] sz size of arrays `ipCol` and `dpVal`;
     * \param[in,out] ipCol,dpVal represent left part of either input inequality or output cut:
     *  `sum(i=0,...,sz-1) dpVal[i]*x(ipCol[i]) <= rhs`;
     * \param[in,out] rhs right-hand side of either input inequality or output cut;
     * \param[out] cutType type of returned inequality (bitwise OR of `enVarType` members).
     *  \return number of cuts generated.
     */
    int parityCut(int sz, int *ipCol, double *dpVal, double &rhs, unsigned &cutType);

////////////////////////////////////////////////////////////////////
// Disjunctions
//////////////////////////
    /*
     * Given an inequality `sum(i=0,...,sz-1) dpA[ipCol[i]] dpX[ipCol[i]] >= 1`, where `0 <= dpX[j] <= 1` for all `j`.
     * Let P denote the solution set of the given inequality, and let `x^*=dpX`.
     * `computeAlpha_P()` finds the value of `alpha_P` defined to be `min{u^Tx^*: u^Tx \ge 1 for all x\in P, u \ge 0}`.
     * If `alpha_P < 1`, then, for any `u` such that `u^Tx^*=alpha_P`,
     * the inequality `u^T x \ge 1` is valid for `P` and cuts off `x^*` from `P`.
     * \param[in] sz number of variables with non-zero coefficients;
     * \param[in] ipCol array of `sz` variable indices;
     * \param[in] dpA array of `sz` coefficients;
     * \param[in] dpX point `x^*`;
     * \param[out] alpha `alpha_P`;
     * \param[out] q see description of return value;
     * \return size `k` of support of `u`, non-zero coefficients of `u` are `dpA[ipCol[i]]/q`, `i=0,...,k-1`,
     * where the entries of `ipCol` have been permuted so that `dpX[ipCol[0]] <= dpX[ipCol[1]] <= ... <= dpX[ipCol[sz-1]]`.
     */
    int computeAlpha_P(int sz, int *ipCol, double * dpA, double *dpX, double &alpha, double &q);

    /*
     * The procedure computes the most violated inequality (if any)
     * that separates a given point from the union of two sets
     *  `X_1 ={x: sum(i=0,...,sz1-1)} dpA[ipCol1[i]]*x(ipCol1[i]) >= 1, 0 <= x(ipCol1[i]) <= 1, i=0,...,sz1-1}` and
     *  `X_2 ={x: sum(i=0,...,sz2-1)} dpA[ipCol1[i]]*x(ipCol2[i]) >= 1, 0 <= x(ipCol2[i]) <= 1, i=0,...,sz2-1}`.
     *  \param[in,out] sz1 size of set `X_1`; on output, `sz1` is number of variables from `X_1` in separating inequality;
     *  \param[in,out] ipCol1 variable indices of set `X_1`;
     *  \param[in,out] sz2 size of set `X_2`; on output, `sz2` is number of variables from `X_2` in separating inequality;
     *  \param[in] dpA array of coefficients;
     *  \param[in] dpX components of point to be separated;
     *  \return if non-zero, size `k` of separating inequality that is written as
     *  `sum(i=0,...,k-1) m_dpArray[i]*x(m_ipArray[i]) >= 1`.
     */
    int disjunctionCut(int &sz1, int *ipCol1, double * dpA1, double *dpX1,
    			int &sz2, int *ipCol2, double * dpA2, double *dpX2);

    /*
     * The procedure calls `disjunctionCut()` to build a disjunction cut base on a disjunction derived from a given inequality.
     *
     * Given an inequality
     *       `sum(j\in B) a[j]*x[j] + sum(j in N) a[j]*x[j] >= b`,
     * where
     *     - `B` is a subset of binary variables,
     *     - `b` and all `a[j]` are nonnegative,
     *     - all variables take nonnegative values,
     * then the following disjunction is valid:
     *      `sum(j in J) x_j \ge 1  or sum(j in B\J) a[j]*x[j] + sum(j in N) a[j]*x[j] >= b - max{a[j]: j\in J}`.
     * \param[in] row constraint index;
     * \param[in] side if `true`, left hand side inequality is used to derive disjunction;
     * otherwise, right hand side inequality is used;
     * \param[out] type type of returned cut (bitwise OR of `enVarType` members);
     * \param[out] lhs left hand side of returned constraint.
     * \return cut size, or `0` if no cut has been found.
     */
    int oneRowDisjunction(int row, bool side, unsigned& type, double& lhs);

    /**
     * This procedure is similar to `oneRowDisjunction()` but uses variable bounds to strengthen cuts.
     * \param[in] row constraint index;
     * \param[in] side if `true`, left hand side inequality is used to derive disjunction;
     * otherwise, right hand side inequality is used.
     * \param[out] type type of returned cut (bitwise OR of `enVarType` members);
     * \param[out] lhs left hand side of returned constraint.
     * \return cut size, or `0` if no cut has been found.
     */
    int oneRowDisjunctionForCtrWithVarBounds(int row, bool side, unsigned& type, double& lhs);

    /*
     * The procedure calls `oneRowDisjunction()` or `oneRowDisjunctionForCtrWithVarBounds()`
     *  for each tight constraint of type `CTR_KNAPSACK|CTR_MX_KNAPSACK|CTR_MX_01`.
     */
    int oneRowDisjunctions();

    /**
     * The procedure tries to build a cut based on the disjunction
     * that is induced by two constraints when a given binary variable is rounded up and down.
     * \param[in] col variable (column) index;
     * \param[in] row1 constraint index;
     * \param[in] side1 if `true` right hand side of constraint `row1` to be used;
     * \param[in] row2 constraint index;
     * \param[in] side2 if `true` right hand side of constraint `row2` to be used;
     * \param[out] lhs left hand side of returned inequality;
     * \param[out] type type of returned inequality (bitwise OR of `enVarType` members).
     * \return size (denote it by 'k') of the returned inequality, or '0' if no cut has been generated.
     * \remark 1) The following restriction `m_ipRowSize[row1]+m_ipRowSize[row2] <= m_iM+m_iN`
     *            on the row sizes must be valid.
     *     2) The returned inequality is given in the following way:
     *          `sum(i in 0..k-1) m_dpArray[i]*x(m_ipArray[i]) >= lhs`.
     *
     */
    int twoRowDisjunction(int col, int row1, bool side1, int row2, bool side2, double &lhs, unsigned &type);

    /**
     * The procedure selects rows (constraints) that are violated
     * when a given binary variable is set in turn to `0` and to `1`.
     * \param[in] col variable index;
     * \param[in] mode if `0`, disjunctions are formed by setting binary variable `x[j=col]` in turn to `0` and to `1`;
     * if `1`, `x[col]=(side0)? 0: 1` is an implication when another binary variable, `x[j]`, is set to `1`;
     * if `-1`, `x[col]=(side0)? 0: 1` is an implication when another binary variable, `x[j]`, is set to `0`;
     * \param[in,out] size number or disjunctive constraints found so far;
     * \param[in,out] size0 number or disjunctive constraints induced by setting `x[j]` to `0` that have been found so far;
     * \param[in,out] size1 number or disjunctive constraints induced by setting `x[j]` to `1` that have been found so far;
     * \param[in,out] ipDJrow list of disjunctive constraints; if, for `i=0,...,size-1`, `ipDJrow[i<<1]=r+1`, then, when `x[j]` is set to `1`,
     * left hand side of constraint  `r` is violated if `ipDJrow[(i<<1)+1]=-1`, otherwise (if `ipDJrow[(i<<1)+1]=1`)
     * left hand side of constraint  `r` is violated;
     * if `ipDJrow[i<<1]=-r-1`, then, when `x[j]` is set to `0`,
     * left hand side of constraint  `r` is violated if `ipDJrow[(i<<1)+1]=-1`, otherwise (if `ipDJrow[(i<<1)+1]=1`)
     * left hand side of constraint  `r` is violated;
     * \param[in,out] ipRowPos array of maximum size `m_iM`; if row `r` is in list `ipDjRow`, then `abs(ipDJrow[ipRowPos[r]<<1])-1=r`;
     * \param[in] side0 if `true`, `x[col]` can be set to `0`;
     * \param[in] side1 if `true`, `x[col]` can be set to `1`.
     */
    void twoRowDJ_selectRows(int col, int mode,
    		int &size, int &size0, int &size1, int *ipDJrow, int *ipRowPos,
    		bool side0, bool side1);

    /**
     * For each binary variable, the procedure seeks for row pairs that can induce a disjunction,
     * and then calls `twoRowDisjunction()` to produce a cut.
     * \return number of cuts generated.
     */
    int twoRowDisjunctions();

////////////////////////////////////////////////////////////////////
//  P A C K I N G  cuts
//////////////////////////
    /**
     * \param[in] sz size (number of variables) of click;
     * \param[in,out] ipClick on input array of size `sz` with list of variables in initial click constraint,
     * on output `ipClick` contains list of variables in extended click of size `k`, where `k` is return value;
     * \param[in] dWeight sum of values of all variables in click;
     * \param[in,out] bGlobal if `true` on input, click has been derived from global constraints
     * \param[in] candNum number of variables written in `m_ipArray` that are candidates for adding to click.
     * \return number of variables in extended click, or `0` if extended click is not violated by current LP solution.
     */
	int extendClick(int sz, int *ipClick, double dWeight, bool &bGlobal, int candNum);

	/**
	 * \param[in] row index of packing constraint;
	 * \param[in] candNum number of previously generated click cuts;
	 * \param[in] cutNum number of click cuts generated earlier;
	 *  any time a new click cut is generated, it is added to matrix only if
	 *  its hash value does not match any hash value of click cut generated earlier.
	 * \return `true` if at least one cut was generated.
	 *
	 */
    bool clickCut(int row, int candNum, int cutNum);

    /**
     * The procedure call `clickCut()` for all tight packing constraints.
     * \returns number of generated cuts.
     */
    int clickCut();
    
///////////////////////////////////////////////////////////////
//  MIR inequalities: Chvatal-Gomory, Residual Capacity cuts
///////////////////////
    /**
     * Given a valid inequality that is a linear combination of problem constraints,
     * the procedure looks for another problem inequality to eliminate from the ininial inequality
     * one variables which current value is far from its bounds.
     * \param[in] round number of inequalities used to obtain initial inequality;
     * \param[in] m0 only first `m0` matrix rows are considered as candidates for mixing;
     * \param[in] sz size of arrays `dpA` and `ipCol`;
     * \param[in] dpA,ipCol `sum(i=0,...,sz-1) dpA[i]*x(ipCol[i])` is right part of initial inequality;
     * \param[in] ipRowPos only if `ipRowPos[i]==0`, inequality `i` can be used for mixing;
     * \param[out] row index of inequality to be mixed with initial one;
     * \param[out] col index of variable to be eliminated;
     * \param[out] pivot inequality indexed by `row` must first be multiplied by `pivot` and then added to initial inequality.
     * \return `true` is appropriate inequality has been found; otherwise, `false`.
     */
    bool getCtrForMixing(int round, int m0, int sz, double* dpA, int* ipCol,
    				int* ipRowPos, int& row, int& col, double& pivot, bool allBinVars);

    /**
     * Given a valid inequality that is a linear combination of problem constraints,
     * the procedure mixes this inequality with that found by `getCtrForMixing()`
     * to eliminate one variable, which current value is far from its bounds, from the initial inequality
     * given as
     *     `sum(i in 0..szA-1) dpA[i] x(ipCol[i]) <= b`.
     * \param[in] row constraint (row) to  be mixed with input inequality;
     * \param[in] pivot when mixing, constraint indexed by `row` is multiplied by `pivot`;
     * \param[in,out] szA size of input and output constraints;
     * \param[in,out] dpA coefficients of input and output constraints, array of size `szA`;
     * \param[in,out] ipCol column (variable) indices of input and output constraints, array of size `szA`;
     * \param[in,out] szP number of constraints that have been mixed;
     * \param[in,out] dpPivot array of size `szP`, when mixing, constraint `ipRow[i]` is multiplied by `dpPivot[i]`;
     * \param[in,out] ipRow indices of mixing constraints, array of size `szP`;
     * \param[in,out] ipRowPos if nonnegative, `ipRowPos[row]` is position of constraint row in array `ipRow`, i.e.,
     *   `ipRow[ipRowPos[row]]=row`; negative, value of `ipRowPos[row]` indicates that constraint indexed by `row`
     *   is not mixing constraint.
     * \param[in,out] if `true` on input (output), then input (output) constraint is local.
     */
    void mixCtrs(int row, double pivot,
        int &szA, double* dpA, int* ipCol, int* ipColPos, double& b,
        int &szP, double* dpPivot, int *ipRow, int* ipRowPos, bool& bLocal);

    /**
     * The procedure does some preparations for subsequent calls of 'mirCut()` and `mixedKnapsackCut()`.
     * In particular, the procedure complements some variables,
     * and then compute an array of factors; the inequality is multiplied, in turn,
     * by each of these factors and the result is "mixes-integer rounded"
     * \param[in] szA size of arrays `dpA` and `ipCol`;
     * \param[in] dpA,ipCol `sum(i in 0..szA-1) dpA[i]*x[ipCol[i]] <= f0` is input inequality;
     * \param[in,out] f0 on output, `f0` is right hand side of the inequality with some variables complemented;
     * \param[in,out] bLocal if `true`, constraint is local.
     * \param[out] dpQ array of factors.
     * \return number of factors stored in `dpQ`.
     */
    int mirPrepareForCuts(int szA, double* dpA, int *ipCol, double &f0, bool &bLocal, double *dpQ);
    
    int buildDeltaMirCut(int szA, double* dpA, int* ipCol, int szP, double delta, double& beta,
          double& violat, unsigned &type, bool flag);

    void substituteVarBounds(int& szA, double* dpA, int *ipCol, double& b,
          int& szP, bool& bLocal, int m0, bool &bMxKnapsack);
          
	int mirGetGUBs(int szA, double* dpA, int *ipCol, int *ipColPos, bool &bLocal);
          
	/**
	 * \param[in] szA,dpA,ipCol valid inequality `sum(i in 0..szA-1) dpA[i]*x(ipCol[i]) <= rhs`;
	 * \param[in,out] rhs right hand side of input and returned inequality;
	 * \param[in] bLocal if `true`, at least one local  inequality or bound for variable were used when deriving input inequality;
	 * \param[out] cutType type of cut inequality;
	 * \param[in] szP number of inequalities (other than variable bounds) mixed to get input inequality.
	 * \param[in,out] rhs right hand side;
	 * \param[out] cutType cut type (bitwise OR of `enVarType` members);
	 * \param[in] bLocal if `true`, cut is local.
	 * \return size of the cut, or '0` if no cut has been found.
	 */
    int mirCut(int szA, double* dpA, int* ipCol, int szP, double &rhs, unsigned &cutType, bool bLocal);

    /**
     * The procedure tries to generate a _MIR_ cut starting from the inequality indexed by `row`.
     * \param[in] row constraint index;
     * \param[in] side side of constraint `row`;
     * \param[in] m0 first `m0` rows of constraint matrix may be used as mixing inequalities;
     * \param[in] needCut if `needCut & 0x1`, _MIR_ cuts to be generated; if `needCut & 0x2`, _mixed knapsack_ cuts to be generated.
     * \param[in,out] sz cut size;
     * \param[out] cut type (bitwise OR of `enVarType` members);
     * \param[in,out] rhs right hand side.
     * \return `1` if MIR cut was generated, `-1` if mixed knapsack cut was generated, and `0` otherwise.
     */
    int mixedIntCut(int row, bool side, int m0, int needCut, int &sz, unsigned &cutType, double &rhs);

    /**
     * The procedure calls MIR and mixed knapsack cut generators.
     * \param[out] mirCutGenerated number of MIR cuts generated;
     * \param[out] mxKnapsackCutGenerated number of mixed-knapsack cuts generated.
     */
    void mixedIntCuts(int &mirCutGenerated, int &mxKnapsackCutGenerated);
        
//////////////////////////////////////////////////////////////////////
////// GOMORY Cuts                      //////////////////////////////

    /**
     * The procedure builds the Chvatal-Gomory cut for input inequality
     *     'sum(i in 0..ctrSize-1) dpVal[i]*x(ipCol[i]) <= b'.
     * \param[in] ctrSize size of input inequality;
     * \param[in] dpVal coefficients of input inequality, array of size 'ctrSize';
     * \param[in] ipCol variable indices of input inequality, array of size 'ctrSize';
     * \param[in] b right hand side of input inequality;
     * \param[in] cpReversed array of size 'm_iN' to be internally used in 'integerRounding()`;
     * \param[in] tol cut is accepted only if it is violated by more than 'tol'
     * at the current LP solution;
     * \param[in] if 'true', input inequality is valid only for the currently processed node
     *   and all its possible ancestors; if 'false', input inequality is valid globally, i.e. for all nodes;
     * \param[out] rhs right hand side of returned inequality;
     * \param[out] type type of returned inequality (bitwise OR of `enVarType` members).
     * \return size of returned inequality (let us denote it by 'sz'), or '0'
     * if no violated cut has been found.
     * \remark The returned inequality (cut) is given as
     *     'sum(i in 0..sz-1) m_dpArray[i]*x(m_ipArray[i]) <= rhs'.
     */
    int integerRounding(int ctrSize, double *dpVal, int *ipCol, double b,
    		char *cpReversed, double tol, bool ctrLocal, double &rhs, unsigned &type);

    /**
     * This procedure just calls `integerRounding()' (with 9 arguments) for
     * a given constraint.
     * \param[in] row constraint index;
     * \param[in] side if 'true' right part of constraint 'row` must be used;
     *   otherwise, left to be used;
     * \param[in] tol to be passed into `integerRounding()' (with 9 arguments);
     * \param[out] rhs,type returned values from `integerRounding()' (with 9 arguments);
     * \param[out] type type of returned inequality (bitwise OR of `enVarType` members).
     * \return if `integerRounding()' (with 9 arguments) is called, its return value;
     * otherwise, '0'.
     */
    int integerRounding(int row, bool side, double tol, double &rhs, unsigned &type);

    /**
     * The procedure generates a _mixed-integer Gomory_ cut
     *  for a weighted sum, `sum(i=0..sz-1) dpVal[i]*x[ipCol[i]]`, of integer variables taking fractional values;
     *  all `dpVal[i]` are integers.
     *  Usually, `sz=1`, `dpVal[0]=1.0`, and a cut is generated for only one variable `x[ipCol[0]]`.
     * \param[in]  bDense if `true` dense cut is to be generated;
     * \param[in] sz number of variables;
     * \param[in] ipCol list of `sz` variable indices;
     * \param[in] dpVal list of `sz` coefficients;
     * \param[out] rhs right hand side of returned inequality;
     * \param[out] type type of returned inequality (bitwise OR of `enVarType` members).
     * \return size of generated cut, or '0' if not cut has been found.
     * \remark If we denote te returned value by 'sz', then the cut is given as
     *      'sum(i in 0..sz-1) m_dpArray[i]*x(m_ipArray[i]) <= rhs'.
     */
    int GomoryCut(bool bDense, int sz, int *ipCol, double *dpVal, double &rhs, unsigned &type);

    /**
     * The procedure generates a family of _mixed-integer Gomory_ cuts.
     * \param[in]  bDense if `true` dense cuts are generated;
     * \param[out] number of dense cuts generated;
     * \param[out] number of sparse cuts generated
     */
    void GomoryCuts(bool bDense, int &denseCutGenerated, int &sparseCutGenerated);

///////////////////////////////////////////////////
    /**
     * The procedure transforms a binary matrix into the upper triangular form.
     * \param[in] number of rows;
     * \param[in] number of columns;
     * \param[in] size of row (in integer words);
     * \param[out] permutation of rows (array of size `m`);
     * \param[in,out] binary matrix written row-wise (`n1` words per row),
     *  on output it is transformed into upper triangular form.
     * \return rank of matrix `A`.
     */
	int mod2Basis(int m, int n, int n1, int* S, int *A);

	/**
	 * The procedure decides which rows and columns be included into the matrix used for generating
	 * Chvatal-Gomory cuts with coefficients `0,1/p,...,(p-1)/p`.
	 * \param[in] p integer from {2,3,5};
	 * \param[in] neqMsk bitwise OR of the members of `CMIP::enCtrType` members,
	 *   rows representing inequalities that match this mask are added to matrix;
	 * \param[in] eqMsk bitwise OR of the members of `CMIP::enCtrType` members,
	 *   rows representing equalities that match this mask are added to matrix;
     * \param[in] maxVarSlack maximum slack value for variable to be included in matrix;
     * \param[in] maxCtrSlack maximum slack value for integer constraints to be included in matrix;
     * \param[out] rowNum number of rows in matrix;
     * \param[out] colNum number of columns in matrix.
	 */
	void buildModPMatrix(int p, int neqMsk, int eqMsk, double maxVarSlack, double maxCtrSlack, int &rowNum, int &colNum);

	/**
     * The procedure generates a family of {0,1/2}-Chvatal-Gomory cuts.
     * \param[in] maxVarSlack maximum slack value for variable to be included in binary matrix;
     * \param[in] maxCtrSlack maximum slack value for integer constraints to be included in binary matrix;
     * \param[out] number of dense cuts generated;
     * \param[out] number of sparse cuts generated.
     */
	void mod2Cuts(double maxVarSlack, double maxCtrSlack, int &denseCutNum, int &sparseCutNum);

////////////////////////////////////////////////////////////////////
//                    Functions for managing cuts                //
////////////////////////////////////////////////////////////////////

	///< The procedure is called to invoke all internal cut generating procedures.
	/**
	 * \return number of cuts generated.
	 */
    int autoCuts();

    /**
     * \param[in] ipFracNum  array of size `AUTO_CUT_CYCLE`,
     *       where `ipFracNum[m_iAutoCutRound-1) % AUTO_CUT_CYCLE]`
     *        is number of integer variables taking fractional values after previous round of generating cuts;
     * \param[in] dpObjProgress array of size `AUTO_CUT_CYCLE`,
     *       where `dpObjProgress[m_iAutoCutRound-1) % AUTO_CUT_CYCLE]`
     *        is objective value after previous round of generating cuts;
     * \param[out] n number of variables (columns);
     * \param[out] index of first new cut generated by __MIPCL__.
     * \return number of new cuts generated by __MIPCL__.
     */
    int generateCuts(int *ipFracNum, double *dpObjProgress, int& n, int &firstAutoCut);
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
//   SERIALIZATION: functions are used when restarting computations
    /**
     * The procedure reads/writes from/to a given stream the matrix of the problem being solved.
     * \param[in,out] ar input or output stream;
     * \param[in] is_storing if `true`, matrix is storing; otherwise, it is restoring.
     */
    void serializeMatrix(std::fstream& ar, bool is_storing);

    /**
     * The procedure reads/writes from/to a given stream the best (record) solution.
     * \param[in,out] ar input or output stream;
     * \param[in] is_storing if `true`, solutionis storing; otherwise, it is restoring.
     */
    void serializeRecSolution(std::fstream& ar, bool is_storing);

    /**
     * The procedure reads/writes from/to a given stream the values of tolerance variables.
     * \param[in,out] ar input or output stream;
     * \param[in] is_storing if `true`, tolerance variables are storing; otherwise, they are restoring.
     */
    void serializeTolVars(std::fstream& ar, bool is_storing);

    /**
     * The procedure reads/writes from/to a given stream different parameters that affect the solution process.
     * \param[in,out] ar input or output stream;
     * \param[in] is_storing if `true`, parameters are storing; otherwise, they are restoring.
     */
    void serializeFlags(std::fstream& ar, bool is_storing);

protected:
    /**
     * The function stores into (or restores from) the stream `ar`
     * CMIP objects (all its member storing permanent data).
     * \param[in] ar reference to a stream;
     * \param[in] is_storing if `true`, the object is written to the stream;
     *  otherwise, the object is restored from the stream.
     * \remark Derived classes may overload this function to store additional information.
     * In such a case, call first `serialize()` of the base class.
     *  \sa CLP::serialize().
     */
    virtual void serialize(std::fstream& ar, bool is_storing);

private:
    void writeMIP(); ///< The procedure writes the current problem state into the file `m_strProblemName.res`


    /**
     * The procedure is used in case of restart
     * to continue solving the problem  from the state saved in the file `m_strProblemName.res`.
     */
    void readMIP();
/////////////////////////////////////////////////////
// Statistics
/////////////////////////////////////////////////////
    char* getProbStatStr(char *str=0);

    /**
     * The functions calls `mipInfo()` to print
     * the current state of the solution process.
     * \param[in] nodeNum total number of nodes;
     * \param[in] leafNum number of leaves (non-processed nodes) in the search tree;
     * \param[in] upperBound upper bound on the optimal objective value;
     * \param[in] header if `true`, the header is displayed.
     */
    void __mipInfo(int nodeNum, int leafNum, double upperBound, bool header);
protected:
    /**
     * The function prints into the standard output stream a message string
     * which describes the current state of the solution process.
     *
     * \param[in] timeElapsed string representation of time elapsed since solution process started;
     * \param[in] nodeNum total number of nodes;
     * \param[in] leafNum number of leaves (non-processed nodes) in the search tree;
     * \param[in] bestObjVal best objective value achieved so far;
     * \param[in] objBound lower or upper (`sense=true`) bound on the optimal objective value;
     * \param[in] gap relative (in percents) difference between `bestObjVal` and `objBound`;
     * \param[in] solsFound number of solutions found so far;
     * \param[in] sense if `true`, objective is maximized; otherwise, minimized;
     * \param[in] header if `true`, the header is displayed.
     * \remark When developing an application with a GUI interface, the user may wish to overload this function.
     */
    virtual void mipInfo(char *timeElapsed, int nodeNum, int leafNum,
    		double bestObjVal, double objBound, double gap, int solsFound, bool sense, bool header);

    /**
     * After each _cut generating round_ (i.e., after calling the cut generating procedure),
     * the solver calls `cutInfo()` to inform about the progress in decreasing the objective value of the relaxation LP.
     *
     * \param[in] time time elapsed since the solution process has started;
     * \param[in] round cut generating round;
     * \param[in] objVal objective value of the relaxation LP;
     * \param[in] fracNum number of integral variables taking fractional values in the current LP solution;
     * \param[in] cutNum number of cuts generated at this particular round.
     * \remark When developing an application with a GUI interface, the user may wish to overload this function.
     */
    virtual void cutInfo(__LONG time, int round, double objVal, int fracNum, int cutNum);

    /**
     * After having been processed the root node,
     * the solver prints into the standard output stream
     * how many cuts of each type were generated, and how many of them were present in the matrix
     * at the beginning of the next cut generating round.
     *
     * \remark When developing an application with a GUI interface, the user may wish to overload this function.
     */
	virtual void cutStatistics();

	/**
	 * The function prints into the stream `out` solution statistics such as:
	 * whether the problem is feasible or not, solution time,
	 * whether optimality was proven or not, and so on.
	 * \param[in] MIPCLver __MIPCL__ version;
	 * \param[in] solTime string representation of solution time;
	 * \param[in] timeLimit if `true`, time limit has been reached;
	 * \param[in] nodeNum number of processed nodes;
	 * \param[in] feasible if `false`, problem is infeasible;
	 * \param[in] hasSolution if `true`, solution (not necessary optimal) has been found;
	 * \param[in] objVal objective value of best solution;
	 * \param[in] opt if `true`, optimality has been proven;
	 * \param[in] gap duality gap;
	 * \param[in] gapLimit if `true`, solution process has been terminated since required duality gap had been reached;
	 * \param[in] bound depending on value of `sense`, lower or upper bound on optimal objective value;
	 * \param[in] difficultNodes number of nodes processed with numerical difficulties;
	 * \param[in] out reference to a stream.
	 * \sa \ref MIPCLmsgs
	 */
	virtual void solStatistics(std::ostream &out, const char* MIPCLver,
			const char* solTime, bool timeLimit, int nodeNum,
			bool feasible, bool hasSolution, double objVal,
			bool opt, double gap, bool gapLimit, double bound,
			int difficultNodes);

private:
	/**
	 * The function calls `solStatistics()` to  prints into the stream `out` solution statistics such as:
	 * whether the problem is feasible or not, solution time,
	 * whether optimality was proven or not, and so on.
	 * \param[in] MIPCLver __MIPCL__ version;
	 * \param
	 * \param[in] out reference to a stream.
	 */
	void _solStatistics(std::ostream &out);

public:
	__LONG getSolTime() const
		{return m_lSolTime;} ///< \return solution time (in seconds).

	/**
	 * \param[in] timeLimit limit on solution time (in seconds).
	 * \sa optimize(), BranchAndCut().
	 */
	void setTimeLimit(__LONG timeLimit)
		{m_luTimeLimit=timeLimit;}

	/**
	 * Given a MIP, the difference between the optimal values of the relaxation LP and MIP itself
	 * is known as _duality_ (or _integrality_) _gap_.
	 * Generating cuts and doing branching, the solver decreases this duality gap.
	 * At a particular moment, the duality gap is the difference between the current upper and lower bounds
	 * on the optimal objective value. If this current duality gap does not exceeds some limit
	 * (given by the user or computed by the solver), the solver terminates,
	 * and the best found solution is printed as a solution to the problem.
	 * \param[in] gap duality gap.
	 * \sa optimize(), BranchAndCut().
	 */
	void setDualGap(double gap)
		{m_dDualGap=gap;}

	int getBranchAndCutNodeNumber()
		{return m_iBranchAndCutNodes;} ///< \return total number of branch-and-cut nodes.

	int getNoOfActiveNodes() const; ///< \return number of leaves in the search tree.

	double getUpperBound() const; ///< \return current upper bound on the optimal objective value.

    /**
     * \return `true` if CMIP cut-messages are to be printed.
     * \sa CMIP::switchMipInfoMsg()
     */
	bool mipCutInfoMsg()
	{return (!(m_iInfoMsgFlag & 0x12))? true: false;}

    /**
     * \return `true` if CMIP run time messages are to be printed.
     * \sa CMIP::switchMipInfoMsg()
     */
    bool mipTreeInfoMsg()
    {return (!(m_iInfoMsgFlag & 0x14))? true: false;}

	/**
	 * The function is called to switch on or off printing CMIP run time messages.
	 * \param[in] cutInfo if `true`, the solver will start printing cut statistics;
	 *  otherwise,  will stop printing such messages;
	 * \param[in] treeInfo if `true`, the solver will start printing branch-and-cut statistics;
	 *  otherwise,  will stop printing such messages.
	 */
	 void switchMipInfoMsg(bool cutInfo, bool treeInfo);

#ifdef __PYTHON_MODULE_
/**
 * When __MIPCL__ is called from __Python__, all handles of variables are their indices,
 * i.e., the handle of variable `x[i]` is `i`. To make communication between __Python__ and __MIPCL__
 * easier, `setSolution()` lists solution components in order of their handles (initial indices).
 */
void setSolution();

/**
 * This function must be called only after calling `sortSolutionByHandles()`.
 * \param[in] ind index (= handle) of variable.
 * \return value of the variable indexed by `ind`.
 */
double getOptVarValue(int ind) const;
#endif

};
#endif // #ifndef __CMIP__H
