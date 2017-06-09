int Crefining::model()
{
	VAR HeavyCrude("Heavy Crude",REAL_GE),
		LightCrude("Light Crude",REAL_GE),
		LightNaphthas("Light Naphthas",REAL_GE),
		HeavyNaphthas("Heavy Naphthas",REAL_GE),
		Oils("Oils",REAL_GE),
		HeavyOils("Heavy Oils",REAL_GE),
		Residium("Residium",REAL_GE),
		CrackedGasolines("Cracked Gasolines",REAL_GE),
		CrackedOils("Cracked Oils",REAL_GE),
		AviationGasoline("Aviation Gasoline",REAL_GE),
		MotorFuel("Motor Fuel",REAL_GE),
		JetFuel("Jet Fuel",REAL_GE),
		FuelOils("Fuel Oils",REAL_GE),
		LubeOils("Lube Oils",REAL_GE);

	maximize(6.5*AviationGasoline + 4.6*MotorFuel +
		3.5*JetFuel + 2.5*FuelOils + 0.8*LubeOils
		- 1.5*HeavyCrude - 1.7*LightCrude
		- 0.4*LightNaphthas - 0.4*HeavyNaphthas - 0.9*Oils
		- 0.3*HeavyOils - 0.3*Residium
		- 0.4*CrackedGasolines - 0.3*CrackedOils);

	LightCrude <=  10; // Light crude avail.
	(HeavyCrude + LightCrude <= 25).setName("Dist. capacity");
	Oils <=  8; // Cracking capacity
	(LightNaphthas == 0.12*HeavyCrude + 0.17*LightCrude).setName("Dist->LightNaphthas");
	(HeavyNaphthas == 0.23*HeavyCrude + 0.28*LightCrude).setName("Dist->HevyNaphthas");
	(Oils + HeavyOils == 0.41*HeavyCrude + 0.34*LightCrude).setName("Dist->Oils");
	(Residium + LubeOils == 0.24*HeavyCrude + 0.21*LightCrude).setName("Dist->Residium");
	(CrackedGasolines == 0.65*Oils).setName("Cracking->Gasoline");
	(CrackedOils == 0.35*Oils).setName("Cracking->Oils");
	(AviationGasoline + MotorFuel ==
	LightNaphthas + HeavyNaphthas + CrackedGasolines).setName("Gasoline blending");
	(JetFuel + FuelOils <= HeavyOils + Residium + CrackedOils).setName("Fuel-oil blending");
	(AviationGasoline <= 1.5*LightNaphthas).setName("Quality");
	(AviationGasoline <= 1.2*LightNaphthas + 0.3*HeavyNaphthas).setName("Octane number");
	(AviationGasoline <= 0.5*MotorFuel).setName("Sales limit (av.gas)");
	JetFuel <= 4; // Sales limit

	preprocoff();
	optimize();
	printsol("refining.sol");
} // end of Crefining::model

