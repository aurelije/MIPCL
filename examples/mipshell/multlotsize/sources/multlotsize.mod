#define s0(j) m_ipInitStock[j]
#define sf(j) m_ipFinalStock[j]
#define u(j) m_ipStockCap[j]
#define m(i,t) m_ipMachNum[t*m+i]
#define f(i,t) m_dpFixedCost[t*m+i]
#define c(j,t) m_dpCost[t*n+j]
#define h(j,t) m_dpHoldingCost[t*n+j]
#define d(j,t) m_ipDemand[t*n+j]
#define l(t) m_ipDur[t]
#define tau(i,j) m_dpTau[i*n+j]
#define rho(j,k) m_dpRho[j*n+k]

int Cmultlotsize::model()
{
	int i,j,k,t, m,n,T;
	m=m_iMachTypeNum; n=m_iProdNum; T=m_iT;
	VAR_VECTOR s("s",REAL_GE,n,T), x("x",INT_GE,n,T), y("y",INTEGER,m,T);

	minimize(
	  	sum(t in [0,T), j in [0,n)) c(j,t)*x(j,t)
	  + sum(t in [0,T), j in [0,n)) h(j,t)*s(j,t)
	  + sum(i in [0,m), t in [0,T)) f(i,t)*y(i,t)
	  );
	  
	forall(j in [0,n))
		s0(j) + x(j,0) == 
			d(j,0) + s(j,0) + sum(k in [0,n)) rho(j,k)*x(k,0);
		
	forall(t in [1,T), j in [0,n))
		s(j,t-1) + x(j,t) == 
			d(j,t) + s(j,t) + sum(k in [0,n)) rho(j,k)*x(k,t);
	
	forall(i in [0,m), t in [0,T)) {
		sum(j in [0,n)) tau(i,j)*x(j,t) <= l(t)*y(i,t);
		y(i,t) <= m(i,t);
	}
		
	forall(j in [0,n)) {
		forall(t in [0,T))
			s(j,t) <= u(j);
		s(j,T-1) == sf(j);
	}
		
	optimize();
	printsol();
	return 0;
} // end of Cmultlotsize::model

