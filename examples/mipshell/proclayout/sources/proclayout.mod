int Cproclayout::model()
{
	int i,j,s,r, n=m_iN, m=m_iM;
	VAR_VECTOR x("x",BIN,n,m), y("y",BIN,n,n,m,m);

	minimize(sum(i in [0,n), s in [0,m)) p(i,s)*x(i,s) +
		sum(i in [0,n-1), j in [i+1,n), s in [0,m), r in [0,m)) c(i,j,s,r)*y(i,j,s,r));
		 
	forall(i in [0,n))
		sum(s in [0,m)) x(i,s) == 1;
		
	forall(s in [0,m))
		sum(i in [0,n)) x(i,s) <= 1;
		
	forall(i in [0,n-1), j in [i+1,n))
		sum(s in [0,m), r in [0,m)) y(i,j,s,r) == 1; 

	forall(i in [0,n-1), j in [i+1,n), s in [0,m), r in [0,m))
		2*y(i,j,s,r) - x(i,s) - x(j,r) <= 0;

	optimize();
	printSol(x);
	return 0;
} // end of Cproclayout::model

