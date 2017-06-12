#define k m_iK
#define n m_iN
#define m m_iM
#define s1(e) m_ipStation1[e] 
#define s2(e) m_ipStation2[e] 
#define p(e)  m_ipInterference[e] 

int Cfreqassign::model()
{
	VAR x("max_frequency",REAL_GE);
	VAR_VECTOR f("frequency",INT_GE,n), delta("dj",BIN,m);

	minimize(x);
	
	forall(int e in [0,m)) {
		f(s1(e)) - f(s2(e)) >= p(e) - (k+p(e))*delta(e);
		f(s2(e)) - f(s1(e)) >= p(e) - (k+p(e))*(1-delta(e));
	}
	
	forall(int i in [0,n)) {
		f(i) >= 1;
		f(i) <= x;
	}
	1 <= x <= k;
	
	optimize();
	printSol(f);
	return 0;
} // end of Cfreqassign::model

