#define r(j) m_ipRelease[j]
#define d(j) m_ipDead[j]
#define p(i,j) m_ipProcTime[i+j*m]
#define w(j) m_dpWeight[j]

int Ctimeindex::model()
{
	int i,j,t,tau,m=m_iMachineNum,n=m_iJobNum, 
		Rmin=std::numeric_limits<int>::max(), Dmax=0;
	INDEX_SET I;
	for (i=0; i < m; ++i) { 
		for (j=0; j < n; ++j) {
			for (t=r(j); t <= d(j)-p(i,j); ++t)
				I.add(INDEX(i,j,t));
			if (Rmin > r(j)) Rmin=r(j);
			if  (Dmax < d(j)) Dmax=d(j);
		}
	}
//	
	VAR_ARRAY x("x",BIN,I);
	
	maximize(sum(i in [0,m), j in [0,n), t in [r(j),d(j)-p(i,j)]) w(j)*x(i,j,t));
	
	forall(j in [0,n))
		sum(i in [0,m), t in [r(j),d(j)-p(i,j)]) x(i,j,t) <= 1;
	
	forall (i in [0,m))	
		forall(t in [Rmin,Dmax])			
			sum(j in [0,n), tau in [t-p(i,j),t): tau >= r(j) && tau <= d(j)-p(i,j)) x(i,j,tau) <= 1;
		
	optimize();
	printSol(x);
	return 0;
} // end of Ctimeindex::model()

