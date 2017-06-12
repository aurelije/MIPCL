//#include <iostream>
#include <except.h>
#include <cmip.h>
#include "by_bsu_JVmipcl_MIP.h"
#include "misc.h"

JNIEXPORT jlong JNICALL Java_by_bsu_JVmipcl_MIP_createCMIP(JNIEnv *env, jobject obj, jstring name)
{
	const char *str = env->GetStringUTFChars(name, 0);
	CMIP *prob = 0;
	try {
		prob = new CMIP(str);
	}
	catch (CException* pe) {
		env->ReleaseStringUTFChars(name, str);
		NewJavaException(env, "java/lang/Error", pe->what());
		delete pe;
	}
	env->ReleaseStringUTFChars(name, str);
	return reinterpret_cast<jlong>(prob);	
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_MIP_openMatrix(JNIEnv *env, jobject obj,
        jint m, jint n, jint nz) //, jboolean rowGen, jboolean colGen, jint mMax, jint nMax, jint nzMax)
{
	CMIP *mip = getObjPtr<CMIP>(env, obj);
	jlong hd = reinterpret_cast<jlong>(mip);
	try {
		mip->openMatrix(m,n,nz,true,false,0,0,0);//,rowGen,colGen,mMax,nMax,nzMax);
	}	
	catch (CException* pe) {
		NewJavaException(env, "java/lang/Error", pe->what());
		delete pe;
	}
	catch(...) {
		NewJavaException(env, "java/lang/Error", "Unknown exception type");
	}
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_MIP_closeMatrix(JNIEnv *env, jobject obj)
{
	CMIP *mip = getObjPtr<CMIP>(env, obj);
	try {
		mip->closeMatrix();
	}	
	catch (CException* pe) {
		NewJavaException(env, "java/lang/Error", pe->what()); 
		delete pe;
	}
	catch(...) {
		NewJavaException(env, "java/lang/Error", "Unknown exception type");
	}
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_MIP_setVarPriority(JNIEnv *env, jobject obj, jint j, jint pr)
{
	 getObjPtr<CMIP>(env, obj)->setVarPriority(j,pr);
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_MIP_setAutoCutPattern(JNIEnv *env, jobject obj, jint nodeNum, jint height)
{
	getObjPtr<CMIP>(env, obj)->setAutoCutPattern(nodeNum,height);
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_MIP_optimize__JDLjava_lang_String_2(JNIEnv *env, jobject obj,
              jlong timeLimit, jdouble gap, jstring solFileName)
{
	const char *str = env->GetStringUTFChars(solFileName,0);
	try {
		getObjPtr<CMIP>(env, obj)->optimize(timeLimit,gap,str);
	}
	catch (CException* pe) {
		NewJavaException(env, "java/lang/Error", pe->what()); 
		delete pe;
	}
	catch(...) {
		NewJavaException(env, "java/lang/Error", "Unknown exception type");
	}
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_MIP_optimize__(JNIEnv *env, jobject obj)
{
	try {
		getObjPtr<CMIP>(env, obj)->optimize(1000000l, 0.0, 0);
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

JNIEXPORT jboolean JNICALL Java_by_bsu_JVmipcl_MIP_isSolution(JNIEnv *env, jobject obj)
{
	return getObjPtr<CMIP>(env, obj)->isSolution();
}

JNIEXPORT jboolean JNICALL Java_by_bsu_JVmipcl_MIP_isSolutionOptimal(JNIEnv *env, jobject obj)
{
	return getObjPtr<CMIP>(env, obj)->isSolutionOptimal();
}

JNIEXPORT jboolean JNICALL Java_by_bsu_JVmipcl_MIP_isInfeasible(JNIEnv *env, jobject obj)
{
	return getObjPtr<CMIP>(env, obj)->isInfeasible();
}

JNIEXPORT jboolean JNICALL Java_by_bsu_JVmipcl_MIP_timeLimitStop(JNIEnv *env, jobject obj)
{
	return getObjPtr<CMIP>(env, obj)->timeLimitStop();
}

JNIEXPORT jdouble Java_by_bsu_JVmipcl_MIP_getObjVal(JNIEnv *env, jobject obj)
{
	return getObjPtr<CMIP>(env, obj)->getObjVal();
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_MIP_getSolution(JNIEnv *env, jobject obj)
{
	double *dpX;
	int *ipHd;
	jint n = getObjPtr<CMIP>(env, obj)->getSolution(dpX,ipHd);
	jintArray hd = env->NewIntArray(n);
	jdoubleArray X = env->NewDoubleArray(n);
	env->SetIntArrayRegion(hd, 0, n, reinterpret_cast<jint*>(ipHd));
	env->SetDoubleArrayRegion(X, 0, n, dpX);
	jfieldID fid = env->GetFieldID(env->GetObjectClass(obj), "X", "[D");
	env->SetObjectField(obj, fid, X);
	fid = env->GetFieldID(env->GetObjectClass(obj), "varHd", "[I");
	env->SetObjectField(obj, fid, hd);
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_MIP_printSolution(JNIEnv *env, jobject obj, jstring filename)
{
	const char *str = env->GetStringUTFChars(filename,0);
	try {
		getObjPtr<CMIP>(env, obj)->printSolution(str);
	}
	catch(CFileException *pe) {
		env->ReleaseStringUTFChars(filename, str);
		NewJavaException(env, "java/io/IOException", pe->what());
		delete pe;
	}
	env->ReleaseStringUTFChars(filename, str);
}

JNIEXPORT void JNICALL Java_by_bsu_JVmipcl_MIP_dispose(JNIEnv *env, jobject obj)
{
	CMIP *mip = getObjPtr<CMIP>(env, obj);
	setHandle<CMIP>(env, obj, 0);
	delete mip;
}

