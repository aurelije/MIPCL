int CIndexFund::model()
{
	int i,j, q=m_iQ, n=rho.getSize(0);
	VAR_VECTOR y("y",BIN,n), x("x",BIN,n,n);

	maximize(sum(i in [0,n), j in [0,n)) rho(i,j)*x(i,j));

	sum(j in [0,n)) y(j) == q;
	forall(i in [0,n)) {
		sum(j in [0,n)) x(i,j) == 1;
		forall(j in [0,n))
			x(i,j) <= y(j);
	}

	optimize();
	printSol(x,y);
	return 0;
} // end of CIndexFund::model

