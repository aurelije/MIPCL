// macros for improving readability
#define t(e) m_ipTail[e]
#define h(e) m_ipHead[e]
#define c(e) m_dpCost[e]
#define f(e) m_dpFixedCost[e]
#define u(e) m_dpCapacity[e]
#define d(v) m_dpDemand[v]

int Cfcnf::model()
{
	const double zero=0.000001;
	bool suppl;
	double z;
	int v,e, n=m_iVertNum, m=m_iEdgeNum;
	VAR_VECTOR x("x",REAL_GE,m);
	VAR_VECTOR y("y",BIN,m);
	
	z=0.0;
	forall(v in [0,n))
		z+=d(v);
		
	suppl=(z <  -zero)? true: false;

	minimize(sum(e in [0,m)) (f(e)*y(e) + c(e)*x(e)));
	
	if (suppl) {
		forall(v in [0,n): d(v) < -zero)
			sum(e in [0,m): h(e)==v) x(e)  - sum(e in [0,m): t(e)==v) x(e) >= d(v);
		
		forall(v in [0,n): d(v) >= -zero)
			sum(e in [0,m): h(e)==v) x(e)  - sum(e in [0,m): t(e)==v) x(e) == d(v);
	}
	else {
		forall(v in [0,n): d(v) > zero)
			sum(e in [0,m): h(e)==v) x(e)  - sum(e in [0,m): t(e)==v) x(e) <= d(v);
		
		forall(v in [0,n): d(v) <= zero)
			sum(e in [0,m): h(e)==v) x(e)  - sum(e in [0,m): t(e)==v) x(e) == d(v);
	}
		
	forall(e in [0,m))
		x(e) <= u(e)*y(e);
		
//	preprocoff();
		
	optimize();
	printSolution(x);
	
	return 0;
} // end of Cfcnf::model

