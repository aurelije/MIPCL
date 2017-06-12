#define beta(t)	m_ipCardCost[t]
#define c(t) m_ipCardCap[t]
#define d (*m_paDemand)

int Cmultiplexer::model()
{
	INDEX e, end[2];
	int i,r,t,
		n=m_iNodeNum, R=m_iRingNum, RC=m_iRingCap,
		alpha=m_iMultCost, S=m_iSlotNum, T=m_iCardNum;
	RANGE NODE(1,n), RING(1,R), TCARD(0,T-1);
	VAR_ARRAY x("x",BIN+2,NODE,RING), y("y",BIN,d,RING), z("z",INT_GE,NODE,RING,TCARD);

	minimize(sum( r in RING, i in NODE) alpha*x(i,r) + 
	         sum(r in RING, i in NODE, t in TCARD) beta(t)* z(i,r,t));

	forall(e in d)
		sum(r in RING) y(e,r) == 1;
	
	forall(r in RING) {
		sum(e in d) d(e)*y(e,r) <= RC;
		forall(i in NODE) {
			sum(e in d: i % e) d(e)*y(e,r) <= sum(t in TCARD) c(t)*z(i,r,t);
			sum(t in TCARD) z(i,r,t) <= S*x(i,r);
		}
		forall(e in d) {
			e.split(2,end);
			y(e,r) <= x(end[0],r);
			y(e,r) <= x(end[1],r);
		}
	}

	optimize();
	printsol();
	return 0;
} // end of Cmultiplexer::model

