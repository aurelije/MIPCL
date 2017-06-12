#define c(j,i,o) (m_pNode[j].prob*m_dpCost[(m_pNode[j].period*3+o)*m+i])
#define d(j,i,o) m_pNode[j].demand[o*m+i]
#define parent(j) m_pNode[j].parent
#define s(k) m_pPlane[k].num
#define q1(k) m_pPlane[k].q1
#define q2(k) m_pPlane[k].q2
#define q3(k) m_pPlane[k].q3
#define r1(k) m_pPlane[k].r1
#define r2(k) m_pPlane[k].r2
#define r3(k) m_pPlane[k].r3
#define f(k)  m_pPlane[k].cost

int Cyield::model()
{
	int i,j,k,o,
		T=m_iT, n=m_iNodeNum,
		n0=m_iNodeNum-m_iLeafNum,
		m=m_iOptNum, I=3,
		K=m_iPlaneTypeNum;
	
	VAR_VECTOR v("v",INT_GE,K);
	VAR_VECTOR x("x",INT_GE,n,I,m), // x[0] is not used 
 	           y("y",BIN,n0,m), z("z",INT_GE,n,I);

	maximize(
	 sum(j in [1,n), o in [0,m), i in [0,I)) c(j,i,o)*x(j,i,o)
	 - sum(k in [0,K)) f(k)*v(k)
	);
	
	forall(j in [0,n0))
		sum(o in [0,m)) y(j,o) == 1;

	forall(j in [1,n), i in [0,I), o in [0,m))
		x(j,i,o) <= d(j,i,o)*y(parent(j),o);
		
	z(0,0) == 0;
	z(0,1) == 0;
	z(0,2) == 0;
	
	forall(j in [1,n), i in [0,I))
		z(j,i) == z(parent(j),i) + sum(o in [0,m)) x(j,i,o);
		
	forall(j in [n0,n)) {
		z(j,0) <= sum(k in [0,K)) (q1(k) + (r2(k)*q2(k))/100)*v(k);
		z(j,1) <= sum(k in [0,K)) (q2(k) + (r1(k)*q1(k)+r3(k)*q3(k))/100)*v(k);
		z(j,2) <= sum(k in [0,K)) (q3(k) + (r2(k)*q2(k))/100)*v(k);
		z(j,0) + z(j,2) <= sum(k in [0,K)) (q1(k)+q3(k)+(r2(k)*q2(k))/100)*v(k);
		z(j,0) + z(j,1) + z(j,2) <= sum(k in [0,K)) (q1(k)+q2(k)+q3(k))*v(k);
	}
	
	forall(k in [0,K))
		v(k) <= s(k);
			
	optimize();
	printsol();
	return 0;
} // end of Cyield::model
