//#include <iostream>
#include <except.h>
#include <lp.h>
#include "by_bsu_JVmipcl_LP.h"
#include "misc.h"

JNIEXPORT jlong JNICALL Java_by_bsu_JVmipcl_LP_createLP(JNIEnv *env, jobject obj, jstring name)
{
	CLP *prob = 0;
	const char *str = env->GetStringUTFChars(name,0);
	try {
		prob = new CLP(str);
	}
	catch (CException* pe) {
		env->ReleaseStringUTFChars(name, str);
		NewJavaException(env, "java/lang/Error", pe->what()); 
		delete pe;
	}
	env->ReleaseStringUTFChars(name, str);
	return reinterpret_cast<jlong>(prob);
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_LP_openMatrix(JNIEnv *env, jobject obj,
        jint m, jint n, jint nz)//, jboolean rowGen, jboolean colGen, jint mMax, jint nMax, jint nzMax)
{
	try {
		getObjPtr<CLP>(env, obj)->openMatrix(m,n,nz,false,false,0,0,0);//rowGen,colGen,mMax,nMax,nzMax);
	}	
	catch (CException* pe) {
		NewJavaException(env, "java/lang/Error", pe->what());
		delete pe;
	}
	catch(...) {
		NewJavaException(env, "java/lang/Error", "Unknown exception type");
	}
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_LP_closeMatrix(JNIEnv *env, jobject obj)
{
	try {
		getObjPtr<CLP>(env, obj)->closeMatrix();
	}	
	catch (CException* pe) {
		NewJavaException(env, "java/lang/Error", pe->what()); 
		delete pe;
	}
	catch(...) {
		NewJavaException(env, "java/lang/Error", "Unknown exception type");
	}
}

JNIEXPORT jint JNICALL Java_by_bsu_JVmipcl_LP_addVar(JNIEnv *env, jobject obj,
          jint hd, jint type, jdouble cost, jdouble l, jdouble u)
{
	jint ind = -1;
	try {
		ind=getObjPtr<CLP>(env, obj)->addVar(hd,type,cost,l,u);
	}	
	catch (CException* pe) {
		NewJavaException(env, "java/lang/Error", pe->what()); 
		delete pe;
	}
	catch(...) {
		NewJavaException(env, "java/lang/Error", "Unknown exception type");
	}
	return ind;
}

JNIEXPORT jint JNICALL Java_by_bsu_JVmipcl_LP_addColumn(JNIEnv *env, jobject obj,
          jint hd, jint type, jdouble cost, jdouble l, jdouble u,
          jdoubleArray Val, jintArray Row, jboolean bSort)
{
	jdouble *dpVal = env->GetDoubleArrayElements(Val,0);
	jint *ipRow = env->GetIntArrayElements(Row,0);
	jsize sz = env->GetArrayLength(Val);
	jint ind = -1;
	try {
		ind=getObjPtr<CLP>(env, obj)->addColumn(hd,type,cost,l,u,sz,
			reinterpret_cast<double*>(dpVal),reinterpret_cast<int*>(ipRow),
			(bSort==JNI_TRUE)? true: false
			);
	}	
	catch (CException* pe) {
		NewJavaException(env, "java/lang/Error", pe->what());
		delete pe;
	}
	catch(...) {
		NewJavaException(env, "java/lang/Error", "Unknown exception type");
	}
	env->ReleaseDoubleArrayElements(Val, dpVal, JNI_ABORT);
	env->ReleaseIntArrayElements(Row, ipRow, JNI_ABORT);
	return ind;
}

JNIEXPORT jint JNICALL Java_by_bsu_JVmipcl_LP_addCtr(JNIEnv *env, jobject obj,
          jint hd, jint type, jdouble lhs, jdouble rhs)
{
	jint ind= -1;
	try {
		ind=getObjPtr<CLP>(env, obj)->addCtr(hd,type,lhs,rhs);
	}
	catch (CException* pe) {
		NewJavaException(env, "java/lang/Error", pe->what());
		delete pe;
	}
	catch(...) {
		NewJavaException(env, "java/lang/Error", "Unknown exception type");
	}
	return ind;
}

JNIEXPORT jint JNICALL Java_by_bsu_JVmipcl_LP_addRow(JNIEnv *env, jobject obj,
          jint hd, jint type, jdouble lhs, jdouble rhs,
          jdoubleArray Val, jintArray Col, jboolean bSort)
{
	jdouble *dpVal = env->GetDoubleArrayElements(Val,0);
	jint *ipCol = env->GetIntArrayElements(Col,0);
	jsize sz = env->GetArrayLength(Val);
	jint ind = -1;
	try {
		ind=getObjPtr<CLP>(env, obj)->addRow(hd,type,lhs,rhs,sz,
			reinterpret_cast<double*>(dpVal),reinterpret_cast<int*>(ipCol),
			(bSort == JNI_TRUE) ? true : false);
	}	
	catch (CException* pe) {
		NewJavaException(env, "java/lang/Error", pe->what());
		delete pe;
	}
	catch(...) {
		NewJavaException(env, "java/lang/Error", "Unknown exception type");
	}
	env->ReleaseDoubleArrayElements(Val, dpVal, JNI_ABORT);
	env->ReleaseIntArrayElements(Col, ipCol, JNI_ABORT);
	return ind;
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_LP_addEntry(JNIEnv *env, jobject obj, jdouble val, jint i, jint j)
{
	try {
		getObjPtr<CLP>(env, obj)->addEntry(val,i,j);
	}
	catch (CException* pe) {
		NewJavaException(env, "java/lang/Error", pe->what());
		delete pe;
	}
	catch(...) {
		NewJavaException(env, "java/lang/Error", "Unknown exception type");
	}
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_LP_setObjCoeff(JNIEnv *env, jobject obj, jint j, jdouble val)
{
	getObjPtr<CLP>(env, obj)->setObjCoeff(j,val);
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_LP_setObjSense(JNIEnv *env, jobject obj, jboolean sense)
{
	getObjPtr<CLP>(env, obj)->setObjSense((sense == JNI_TRUE) ? true : false);
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_LP_beSilent(JNIEnv *env, jobject obj, jboolean flag)
{
	getObjPtr<CLP>(env, obj)->beSilent((flag == JNI_TRUE) ? true : false);
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_LP_preprocOff(JNIEnv *env, jobject obj)
{
	getObjPtr<CLP>(env, obj)->preprocOff();
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_LP_optimize(JNIEnv *env, jobject obj)
{
	try {
		getObjPtr<CLP>(env, obj)->optimize(1000000l, 0.0, 0);
	}	
	catch (CException* pe) {
		NewJavaException(env, "java/lang/Error", pe->what());
		delete pe;
	}
	catch(...) {
		//translate unknown C++ exception to a Java exception
		NewJavaException(env, "java/lang/Error", "Unknown exception type");
	}
}

JNIEXPORT jboolean JNICALL Java_by_bsu_JVmipcl_LP_isSolution(JNIEnv *env, jobject obj)
{
	return getObjPtr<CLP>(env, obj)->isSolution();
}

JNIEXPORT jboolean JNICALL Java_by_bsu_JVmipcl_LP_isLpInfeasible(JNIEnv *env, jobject obj)
{
	return getObjPtr<CLP>(env, obj)->isLpInfeasible();
}

JNIEXPORT jboolean JNICALL Java_by_bsu_JVmipcl_LP_isLpUnbounded(JNIEnv *env, jobject obj)
{
	return getObjPtr<CLP>(env, obj)->isLpUnbounded();
}

JNIEXPORT jdouble JNICALL Java_by_bsu_JVmipcl_LP_getObjVal(JNIEnv *env, jobject obj)
{
	return getObjPtr<CLP>(env, obj)->getObjVal();
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_LP_getSolution(JNIEnv *env, jobject obj)
{
	double *dpX;
	int *ipHd;
// set solution and variable handles
	jint n = getObjPtr<CLP>(env, obj)->getSolution(dpX,ipHd); // n is number of columns
	jintArray hd = env->NewIntArray(n);
	jdoubleArray X = env->NewDoubleArray(n);
	env->SetIntArrayRegion(hd, 0, n, reinterpret_cast<jint*>(ipHd));
	env->SetDoubleArrayRegion(X, 0, n, dpX);
	jfieldID fid = env->GetFieldID(env->GetObjectClass(obj), "X", "[D");
	env->SetObjectField(obj, fid, X);
	fid = env->GetFieldID(env->GetObjectClass(obj), "varHd", "[I");
	env->SetObjectField(obj, fid, hd);
// set reduced costs
	(void)getObjPtr<CLP>(env, obj)->getReducedCosts(dpX,ipHd);
	X = env->NewDoubleArray(n);
	env->SetDoubleArrayRegion(X, 0, n, dpX);
	fid = env->GetFieldID(env->GetObjectClass(obj), "rdCost", "[D");
	env->SetObjectField(obj, fid, X);
// set shadow prices
	n=getObjPtr<CLP>(env, obj)->getShadowPrices(dpX,ipHd); // now, n is number of rows
	hd = env->NewIntArray(n);
	X = env->NewDoubleArray(n);
	env->SetIntArrayRegion(hd, 0, n, reinterpret_cast<jint*>(ipHd));
	env->SetDoubleArrayRegion(X, 0, n, dpX);
	fid = env->GetFieldID(env->GetObjectClass(obj), "shPrice", "[D");
	env->SetObjectField(obj, fid, X);
	fid = env->GetFieldID(env->GetObjectClass(obj), "ctrHd", "[I");
	env->SetObjectField(obj, fid, hd);
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_LP_getInconsistencyCertificate(JNIEnv *env, jobject obj)
{
	// set inconsistency certificate
	double *dpYd=0, *dpYb=0;
	int *ipRowHd=0, *ipColHd=0;
	int m,n;
	getObjPtr<CLP>(env, obj)->whyLpInfeasible(m,ipRowHd,dpYb, n,ipColHd,dpYd);
// move `ipColHd` to Java `varHd` array
	jintArray hd = env->NewIntArray(n);
	env->SetIntArrayRegion(hd, 0, n, reinterpret_cast<jint*>(ipColHd));
	jfieldID fid = env->GetFieldID(env->GetObjectClass(obj), "varHd", "[I");
	env->SetObjectField(obj, fid, hd);
// move `ipRowHd` to Java `ctrHd` array
	hd = env->NewIntArray(m);
	env->SetIntArrayRegion(hd, 0, m, reinterpret_cast<jint*>(ipRowHd));
	fid = env->GetFieldID(env->GetObjectClass(obj), "ctrHd", "[I");
	env->SetObjectField(obj, fid, hd);
// move `dpYd` to Java `rdCost` array
	jdoubleArray Y = env->NewDoubleArray(n);
	env->SetDoubleArrayRegion(Y, 0, n, dpYd);
	fid = env->GetFieldID(env->GetObjectClass(obj), "rdCost", "[D");
	env->SetObjectField(obj, fid, Y);
// move `dpYb` to Java `shPrice` array
	Y = env->NewDoubleArray(m);
	env->SetDoubleArrayRegion(Y, 0, n, dpYb);
	fid = env->GetFieldID(env->GetObjectClass(obj), "shPrice", "[D");
	env->SetObjectField(obj, fid, Y);
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_LP_printSolution (JNIEnv *env, jobject obj, jstring filename)
{
	const char *str = env->GetStringUTFChars(filename,0);
	try {
		getObjPtr<CLP>(env, obj)->printSolution(str);
	}
	catch(CFileException *pe) {
		env->ReleaseStringUTFChars(filename, str);
		NewJavaException(env, "java/io/IOException", pe->what());
		delete pe;
	}
	env->ReleaseStringUTFChars(filename, str);
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_LP_dispose(JNIEnv *env, jobject obj)
{
	CLP *lp=getObjPtr<CLP>(env, obj);
	setHandle<CLP>(env, obj, 0);
	delete lp;
}

