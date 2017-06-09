int Cprodmix::model()
{
	VAR_VECTOR x("x",INT_GE,5);
	maximize(550*x(0)+600*x(1)+350*x(2)+400*x(3)+200*x(4));
	12*x(0)+20*x(1)+        25*x(3)+15*x(4) <= 288;
	10*x(0)+ 8*x(1)+16*x(2)                 <= 192;
	20*x(0)+20*x(1)+20*x(2)+20*x(3)+20*x(4) <= 384;
	optimize();
	printsol("prodmix.sol");
	return 0;
} // end of Cprodmix::model

