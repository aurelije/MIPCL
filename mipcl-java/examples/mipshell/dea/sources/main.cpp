#include <iostream>
#include <iomanip>
#include <cstring>
#include "dea.h"

void readData(const char* probName,
		int &unitNum, int &resNum, int &serviceNum, double* &dpInOut)
{
	char fileName[128];
	strcpy(fileName,probName);
	strcat(fileName,".txt");
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		throw new CFileException("Cdea::readData",fileName);
	}
	int n,m,l;
	fin >> n >> m >> l; 
	unitNum=n; resNum=m; serviceNum=l;
	if (!(dpInOut = new(std::nothrow) double[n*(m+=l)])) {
		throw CMemoryException("readData");
	}
	
	double *dpVal=dpInOut;
	for (int i=0; i < n; ++i) {
		for (int j=0; j < m; ++j)
			fin >> 	*dpVal++;
	}

	fin.close();
} // end of Cdea::readData

void printSol(std::ofstream &fout, int unit, int n, int m, int l,
		double rating, double *dpV, double *dpU, double *dpS, double *dpR)
{
	fout << "=> Unit " << unit+1 << " of rating " << rating << std::endl;
	fout << " v=(";
	--m;
	for (int i=0; i < m; ++i)
		fout << dpV[i] << ",";
	fout << dpV[m] << ")\n u=(";
	--l;
	for (int i=0; i < l; ++i)
		fout << dpU[i] << ",";
	fout << dpU[l] << ")\n";
	fout << "+-------------------------------------+\n";
	fout << "| Unit | Relative |  Weighted sum of  |\n";
	fout << "|      |  rating  | inputs  | outputs |\n";
	fout << "+-------------------------------------+\n";
	for (int j=0; j < n; ++j) {
		fout << "| " << std::setw(4) << std::right << j+1 << "  | "
			<< std::setw(7) << std::setprecision(4) << std::left << dpS[j]/dpR[j] << " | "
			<< std::setw(7) << std::setprecision(4) << std::left << dpR[j] << " | "
			<< std::setw(7) << std::setprecision(4) << std::left << dpS[j] << " |\n";
	}
	fout << "+-------------------------------------+\n";
} // end of printSol 


void solveAll(const char *probName,
		int unitNum, int resNum, int serviceNum, double *dpInOut)
{ 
	double rating, *dpR, *dpS, *dpU, *dpV;
	if (!(dpR = new(std::nothrow) double[2*unitNum+resNum+serviceNum])) {
		throw CMemoryException("solveAll (1)");
	}
	dpU=(dpV=(dpS=dpR+unitNum)+unitNum)+resNum;

	char fileName[128];
	strcpy(fileName,probName);
	strcat(fileName,".sol");
	std::ofstream fout(fileName);
	if (!fout.is_open()) {
		throw new CFileException("solveAll",fileName);
	}

	Cdea *pDea;
	for (int i=0; i < unitNum; ++i) {
		if (!(pDea=new Cdea(probName,unitNum,resNum,serviceNum,dpInOut))) {
			fout.close();
			delete[] dpR;
			throw CMemoryException("solveAll (2)");
		}
		pDea->model(i,rating,dpV,dpU,dpR,dpS);
		printSol(fout,i,unitNum,resNum,serviceNum,rating,dpV,dpU,dpS,dpR);
		delete pDea;
	}
	
	fout.close();
	delete[] dpR;
} // end of solveAll

int main(int argc, const char *argv[])
{
	if (argc < 2) {
		std::cerr << "Enter file name!\n";
		return 1;
	}
		
	int unitNum, resNum, serviceNum;
	double *dpInOut=0;

	try {
		readData(argv[1],unitNum, resNum, serviceNum,dpInOut);
		solveAll(argv[1],unitNum, resNum, serviceNum,dpInOut);
	}
	catch(CException* pe) {
		std::cerr << pe->what() << std::endl;
		delete pe;
		return 1;
	}
	if (dpInOut)
		delete[] dpInOut;
	
	return 0;
}

