#define prec(e) m_ipPrec[e]
#define succ(e) m_ipSucc[e]
#define t(j) m_ipTime[j]

int Cassembly::model()
{
	int i,j, n=m_iOpNum, m=m_iStNum, q=m_iRelNum, C=m_iCycleTime;
	VAR_VECTOR x("x",BIN,m,n), y("y",BIN,m), z("z",REAL_GE,n);

	minimize(sum(i in [0,m)) y(i));
	forall(j in [0,n)) {
		sum(i in [0,m)) x(i,j) == 1;
		sum(i in [0,m)) (i+1)*x(i,j) == z(j);
	}
	forall(i in [0,m)) {
		sum(j in [0,n)) t(j)* x(i,j) <= C*y(i);
		forall(j in [0,n))
			x(i,j) <= y(i);
	}
	forall(i in [0,q))
		z(prec(i)) <= z(succ(i));
		
	forall(i in [1,m))
		y(i-1) >= y(i);

	optimize();
	printSol(x);
	return 0;
} // end of Cassembly::model()

