#define p(t) m_ipPrice[t]
#define h(t) m_ipHoldCost[t]
#define f(t) m_ipFixedCost[t]
#define d(t) m_ipDemand[t]
#define c(t) m_ipCapacity[t]

int Clotsize::model()
{
	int t, T=m_iT, s0=m_iInitStock;
	VAR_VECTOR x("x",REAL_GE,T), s("s",REAL_GE,T), y("y",BIN,T);

	minimize(sum(t in [0,T)) (p(t)*x(t) + h(t)*s(t) + f(t)*y(t)));

	s0 + x(0) == d(0) + s(0);
	x(0) <= c(0)*y(0);
	forall(t in [1,T)) {
		s(t-1) + x(t) == d(t) + s(t);
		x(t) <= c(t)*y(t);
	}

	optimize();
	printSol(x,s);
	return 0;
} // end of Clotsize::model

