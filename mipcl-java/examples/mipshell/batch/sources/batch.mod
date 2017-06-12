int Cbatch::model()
{
	INDEX state, tsk, tsk1, unit;
	int t,tau, M=H*TASKS.getSize();
	RANGE HORIZON(0,H-1), HORIZON1(0,H);
	VAR_ARRAY x("x",BIN,TASKS,UNITS,HORIZON), 
	          y("y",REAL_GE,TASKS,UNITS,HORIZON), 
			  z("z",REAL_GE,STATES,HORIZON1);

	maximize(sum(state in STATES) cp(state)*z(state,H) 
	   - sum(state in STATES, t in [0,H)) cs(state)*z(state,t));

	forall(t in [0,H), unit in UNITS)
		sum(tsk in I(unit)) x(tsk,unit,t) <= 1;
	forall(unit in UNITS, tsk in I(unit), t in [0,H))
		sum(tsk1 in I(unit), tau in [t,t+dur(tsk)): tau < H) x(tsk1,unit,tau) <= 1 + M*(1-x(tsk,unit,t));
	forall(unit in UNITS, tsk in I(unit), t in [0,H)) {
		Vmin(unit)*x(tsk,unit,t) <= y(tsk,unit,t);
		y(tsk,unit,t) <= Vmax(unit)*x(tsk,unit,t);
		if (t+dur(tsk) > H)
			x(tsk,unit,t) == 0;
	}
	forall(state in STATES, t in [0,H)) 
		z(state,t) <= u(state);
	forall(state in STATES)
		z0(state) == z(state,0) + sum(tsk in Tout(state), unit in K(tsk)) rho(state,tsk)*y(tsk,unit,0);
	forall(state in STATES, t in [1,H))
		z(state,t-1) + sum(tsk in Tin(state), unit in K(tsk): t >= p(tsk,state)) rho(tsk,state)*y(tsk,unit,t-p(tsk,state)) ==
		z(state,t) + sum(tsk in Tout(state), unit in K(tsk)) rho(state,tsk)*y(tsk,unit,t);
	forall(state in STATES)
		z(state,H) == z(state,H-1) + sum(tsk in Tin(state), unit in K(tsk): H >= p(tsk,state)) rho(tsk,state)*y(tsk,unit,H-p(tsk,state));

	optimize();
	printSolution(y,z);
	return 0;
} // end of Cbatch::model
