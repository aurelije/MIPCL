#include <mipshell.h>

struct sPlane {
	int num; ///< number of planes of this type
	double cost; ///< plane cost
    int q1, q2, q3, ///< numbers of seats of first business and economy classes
    	r1, r2, r3;
};

struct sNode {
	int ind, parent,
		period; ///< period(ind) = period(parent)+1
	double prob;
	int *demand; ///< array of options (of size 3*optionNum)
};

class Cyield: public CProblem
{
	int m_iT; ///< number of periods
	int	m_iPlaneTypeNum; ///< number of types of planes
	sPlane *m_pPlane; ///< array of size m_iPlaneTypeNum, where  m_pPlane[k] describes plane of type k
	int m_iOptNum; ///< number of options at any period
	double *m_dpCost; ///< array of size 3*m_iT*m_iOptNum, where m_dpCost[(t*3+o)*m_iOptNum+i] is cost of class i ticket if option o is used in period t
	int m_iNodeNum, m_iLeafNum; ///< number of nodes and leaves in scenario tree
	sNode *m_pNode; ///< array of node (of size m_iNodeNum) of scenario tree
public:
	Cyield(const char* name);
#ifdef __THREADS_
	Cyield(const Cyield &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cyield();

	int model();
private:
	void readParameters();
	void readOptions();
	void readTree();
	void readData(const char *name);
};

