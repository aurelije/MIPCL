#define s(i,k) m_dpInOut[i*(m+l)+m+k]
#define r(i,j) m_dpInOut[i*(m+l)+j]

int Cdea::model(int i0, double &rating,
	double *dpV, double *dpU, double *dpR, double *dpS)
{
	int i,j,k, n=m_iUnitNum, m=m_iResNum, l=m_iServiceNum;
	VAR_VECTOR u("u",REAL_GE,l), v("v",REAL_GE,m);
	
	maximize(sum(k in [0,l)) s(i0,k)*u(k));
	
	sum(j in [0,m)) r(i0,j)*v(j) == 1;
	
	forall(i in [0,n): i != i0)
		sum(k in [0,l)) s(i,k)*u(k) <= sum(j in [0,m)) r(i,j)*v(j);

	optimize();

// preparing the output
	rating=getobj();
	forall (k in [0,l))
		dpU[k]=getval(u(k));
		
	forall(j in [0,m))
		dpV[j]=getval(v(j));
		
	forall(i in [0,n)) {
		dpS[i]=dpR[i]=0.0;
		forall(k in [0,l))
			dpS[i]+=s(i,k)*dpU[k];
			
		forall(j in [0,m))
			dpR[i]+=r(i,j)*dpV[j];
	}

	return 0;
} // end of Cdea::model

