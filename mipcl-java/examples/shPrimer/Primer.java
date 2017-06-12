import by.bsu.JVmipshell.*;
import java.io.IOException;

public class Primer
{
	static {
            System.loadLibrary("JVmipcl");
        }

   	public static void main(String[] args) {
		final double[] c = {100, 64};
		final double[] b = {250, 4};
		final double[][] A = {{50, 31}, {-3,2}};
		try {
			DenceMIP prob = new DenceMIP("primer", c, b, A);
			prob.mipclModel();
			prob.optimize();
			prob.printSolution();
		}
		catch(Throwable e) {
			System.out.println(e);
		}
	}
}

class DenceMIP extends MIPshell
{
	public DenceMIP(String name, double[] c, double[] b, double[][] A)
	{
		super(name);
		int m = b.length; // m - number of rows
		int n = c.length; // n - number of columns
		VarVector x = new VarVector(this,"x",VAR_INT,n); // each x[j] >= 0 is integer

		LinSum lsum = new LinSum();

		// build the objective
		for (int j=0; j < n; ++j) {
			lsum.add(c[j],x.get(j));
		}
		maximize(lsum);

		// pose the constraints
		for (int i=0; i < m; ++i) {
			lsum = new LinSum();
			for (int j=0; j < n; ++j) {
				lsum.add(A[i][j],x.get(j));
			}
			addCtr(lsum,-INF,b[i]);	
		}
	}
} 
