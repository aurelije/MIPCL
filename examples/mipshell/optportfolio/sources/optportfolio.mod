#define l(j) m_dpL[j] 
#define mu(j) m_dpMu[j] 
#define a(j,t) m_dpRet[j*T+t]
#define p(t) m_dpP[t]
#define yv(t) m_dpArray[t]

int Coptportfolio::model()
{
	int j,t, n=m_iN, T=m_iT, q=m_iQ;
	VAR_VECTOR x("x",REAL_GE,n), v("v",BIN,n), y("y",REAL,T);
	m_pY = &y;

	maximize(sum(j in [0,n)) mu(j)*x(j));

	sum(j in [0,n)) x(j) == 1;
	sum(j in [0,n)) v(j) <= q;

	forall(t in [0,T))
		y(t) == sum(j in [0,n)) a(j,t)*x(j);
		
	forall (j in [0,n))
		x(j) >= l(j)*v(j);
	
	preprocoff(); // to prevent elimination of y(t) variables
	optimize();
	printsol();
	return 0;
} // end of Coptportfolio::model

bool Coptportfolio::separate()
{
	double b,d=0.0, V=m_dV, zero=m_dZero;
	VAR_VECTOR &y=*m_pY;
	int T=m_iT;
	bool flag=false;
	
	forall(int t in [0,T)) {
		yv(t)=getval(y(t));
		d+=p(t)*yv(t)*yv(t);
	}
	b=sqrt(d*V);
	if (d > b+m_dTol) {
		flag=true;
		sum(int t in [0,T): fabs(yv(t)) > zero) (p(t)*yv(t))*y(t) <= b;
	}
	return flag;
} // end of Coptportfolio::separate()
