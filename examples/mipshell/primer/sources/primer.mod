int Cprimer::model()
{
	VAR_VECTOR x("x",INT_GE,2);
	maximize(x(0) + 2*x(1));
	-2*x(0) + 3*x(1) <=  4;
	 2*x(0) + 2*x(1) <= 11;
	 1 <= x(0) <= 4;
	 1 <= x(1) <= 5;
	optimize();
	printsol("primer.sol");
	return 0;
} // end of Cprimer::model

