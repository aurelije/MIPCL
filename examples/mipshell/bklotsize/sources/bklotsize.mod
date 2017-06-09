#define d(t) m_ipDemand[t]
#define u(t) m_ipCapacity[t]
#define f(t) m_ipFxCost[t]
#define c(tau,t) m_ipCost[tau*T+t]

int Cbklotsize::model()
{
	int t, tau, T=m_iT, Tp=m_iTp;
	VAR_VECTOR x("x",REAL_GE,Tp,T), y("y",BIN,Tp);
		
	minimize(
		sum(tau in [0,Tp), t in [0,T): c(tau,t) >= 0) c(tau,t)*x(tau,t) +
		sum(tau in [0,Tp)) f(tau)*y(tau)
	);
	
	forall(t in [0,T))
		sum(tau in [0,Tp)) x(tau,t) == d(t);
		
	forall(tau in [0,Tp))
		sum(t in [0,T)) x(tau,t) <= u(tau)*y(tau);

	forall(tau in [0,Tp), t in [0,T): c(tau,t) < 0)
		x(tau,t) == 0;
	
	optimize();
	printSol(x,y);
	return 0;
} // end of Cbklotsize::model

