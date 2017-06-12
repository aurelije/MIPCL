int Cgenassign::model()
{
	int i, j, m=c.getSize(0), n=c.getSize(1);

	VAR_VECTOR x("x",BIN,m,n);

	minimize(sum(i in [0,m), j in [0,n)) c(i,j)*x(i,j));

	forall(i in [0,m))
		sum(j in [0,n)) x(i,j) == 1;

	forall(j in [0,n))
		sum(i in [0,m)) p(i,j)*x(i,j) <= l(j);
	
	optimize();
	printsol();
	
	return 0;
} // end of Cgenassign::model

