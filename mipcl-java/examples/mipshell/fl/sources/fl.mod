#define f(i) m_ipFixedCost[i]
#define a(i) m_ipCapacity[i]
#define b(j) m_ipPopulation[j]
#define c(i,j) m_ipCost[i*n+j]
#define min(a,b) ((a < b)? a: b)

int Cfl::model()
{
	int i,j, m=m_iSiteNum, n=m_iLocationNum, q=m_iQ; 
	VAR_VECTOR y("y",BIN,m), x("x",INT_GE,m,n);
	
	minimize(sum(i in [0,m)) f(i)*y(i) + sum(i in [0,m), j in [0,n)) c(i,j)*x(i,j));
	
	sum(i in [0,m)) y(i) <= q;
	
	forall(j in [0,n))
		sum(i in [0,m)) x(i,j) == b(j);

	forall(i in [0,m))
		sum(j in [0,n)) x(i,j) <= a(i)*y(i);

	forall(i in [0,m), j in [0,n))
		x(i,j) <= min(a(i),b(j))*y(i);

	optimize();
	printSol(x,y);
	return 0;
} // end of Cfl::model

