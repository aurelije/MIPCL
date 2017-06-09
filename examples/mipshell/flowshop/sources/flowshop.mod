int Cflowshop::model()
{
	int i,j,k, m,n, T=0;
	n=p.getSize(0); m=p.getSize(1); T=p.Sum();

	VAR_VECTOR start("start",INT_GE,n,m);
	VAR_VECTOR delta("delta",BIN,n,n,m);
	VAR tau("tau",INT_GE);
	
	minimize(tau);
	forall (k in [1,m), i in [0,n))
		start(i,k-1) + p(i,k-1) <= start(i,k);
	forall(k in [0,m), i in [0,n), j in [0,n): j != i) {
		start(i,k) + p(i,k) <= start(j,k) + T*delta(j,i,k);
		setpriority(delta(i,j,k),m-k);
	}
	forall(k in [0,m), i in [0,n), j in [i+1,n))
		delta(i,j,k) + delta(j,i,k) == 1;
	k=m-1;
	forall(i in [0,n))
		start(i,k) + p(i,k) <= tau;
	forall(i in [0,n), j in [0,n): i != j)
		delta(i,j,k-1) == delta(i,j,k);

	optimize();
	printsol();
	return 0;
} // end of Cflowshop::model

