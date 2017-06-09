#define d(t) m_ipDemand[t]
#define u(i) m_ipUpCapacity[i]
#define l(i) m_ipLoCapacity[i]
#define r1(i) m_ipRminus[i]
#define r2(i) m_ipRPlus[i]
#define g(i) m_ipStartUpCost[i]
#define f(i) m_ipFixedCost[i]
#define p(i) m_ipUnitCost[i]

int Cunitcom::model()
{
	double q=m_dQ;
	int i,t, n, T;
	n=m_iGenNum; T=m_iT;
	VAR_VECTOR x("x",BIN,n,T), y("y",REAL_GE,n,T), z("z",BIN,n,T); 
	
	minimize(sum(i in [0,n), t in [0,T)) (g(i)*z(i,t) + f(i)* x(i,t) + p(i)* y(i,t)));
	forall(t in [0,T)) {
		sum(i in [0,n)) y(i,t) == d(t);
		sum(i in [0,n)) u(i)*x(i,t) >= q*d(t);
	}
	forall(i in [0,n), t in [0,T)) {
		l(i)*x(i,t) <= y(i,t);
		y(i,t) <= u(i)*x(i,t);
		-r1(i) <= y(i,t) - y(i,(t-1+T) % T) <= r2(i);
		x(i,t) - x(i,(t-1+T) % T) <= z(i,t);
		z(i,t) <= x(i,t);
	}

	optimize();
	printSol(y);
	return 0;
} // end of Cunitcom::model

