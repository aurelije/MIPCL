int Cufl::model()
{
	int i,j, m=c.getSize(0), n=c.getSize(1); 
	VAR_VECTOR y("y",BIN,m), x("x",BIN,m,n);
	
	minimize(sum(i in [0,m)) f(i)*y(i) + sum(i in [0,m), j in [0,n)) c(i,j)*x(i,j));
	
	sum(i in [0,m)) y(i) <= q;
	
	forall(j in [0,n))
		sum(i in [0,m)) x(i,j) == 1;

	forall(i in [0,m))
		sum(j in [0,n)) x(i,j) <= b(i)*y(i);

	forall(i in [0,m), j in [0,n))
		x(i,j) <= y(i);

	optimize();
	printsol();
	return 0;
} // end of Cufl::model

