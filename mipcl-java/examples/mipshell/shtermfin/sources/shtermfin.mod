#define cs(i,t) m_dpSaleCost[i*(T+1)+t]
#define cb(i,t) m_dpPurchaseCost[i*T+t]
#define f(l,t) m_dpLoanCost[l*T+t]
#define k(i) m_ipInitPortfolio[i]
#define d(t) m_dpDemand[t]
#define q(t) m_dpMinCash[t]
#define u(l) m_dpLineCapacity[l]
#define q0 m_dInitCash

int Cshtermfin::model()
{
	int i,l,t, 
		T=m_iT, n=m_iSecurityNum, m=m_iCreditLineNum;
	VAR_VECTOR x("x",INT_GE,n,T), xb("xb",INT_GE,n,T), xs("xs",INT_GE,n,T);
	VAR_VECTOR y("y",REAL_GE,T);
	VAR_VECTOR z("z",REAL_GE,m,T);

	maximize(y(T-1) + sum(i in [0,n)) cs(i,T)*x(i,T-1) - sum(l in [0,m), t in [0,T)) (1+f(l,t))*z(l,t));
 
    d(0) + q0 + sum(i in [0,n)) cs(i,0)*xs(i,0) + sum(l in [0,m)) z(l,0) ==
	y(0) + sum(i in [0,n)) cb(i,0)*xb(i,0);
 
	forall(t in [1,T))
		d(t) + y(t-1) + sum(i in [0,n)) cs(i,t)*xs(i,t) + sum(l in [0,m)) z(l,t) ==
		y(t) + sum(i in [0,n)) cb(i,t)*xb(i,t);
  
	forall(i in [0,n))
		k(i) + xb(i,0) == x(i,0) + xs(i,0);

	forall(t in [1,T), i in [0,n))
		x(i,t-1) + xb(i,t) == x(i,t) + xs(i,t);
	
	forall(l in [0,m))
		sum(t in [0,T)) z(l,t) <= u(l);
 
	forall(t in [0,T))
		y(t) >= q(t);

	optimize();
	printSol(x,xs,xb,y,z);
	return 0;
} // end of Cshtermfin::model

