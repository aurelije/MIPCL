#ifndef __HANDLE_H
#define __HANDLE_H

#include <jni.h>
#include <stdexcept>

/** The function returns the pointer to either `CLP` or `CMIP` object stored in a given JAVA object.
 *
 * \param[in] env environment;
 * \param[in] obj JAVA object of class `LP` or `MIP`;
 *   this object stores the pointer to either `CLP` (T=CLP) or `CMIP` (T=CMIP) object to be returned.
 * \return pointer to the `T` object stored in `obj`.
 */
template <typename T>
T *getObjPtr(JNIEnv *env, jobject obj)
{
    jlong handle = env->GetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "nativeHd", "J"));
    return reinterpret_cast<T *>(handle);
}

/** The function stores as a handle (of type `jlong`) in a given JAVA object
 *  the pointer to either `CLP` or `CMIP` object.
 *
 * \param[in] env environment;
 * \param[in] obj JAVA object of class `LP` or `MIP`.
 * \return t pointer to the `T` object (T=CLP or T=CMIP) to be stored in `obj`.
 */
template <typename T>
void setHandle(JNIEnv *env, jobject obj, T *t)
{
    jlong handle = reinterpret_cast<jlong>(t);
    env->SetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "nativeHd", "J"), handle);
}

/** This is how we represent Java exceptions in progress
 */
struct ThrownJavaException : std::runtime_error 
{
    ThrownJavaException() :std::runtime_error("") {}
    ThrownJavaException(const std::string& msg ) :std::runtime_error(msg) {}
};

/*** The structure is used to throw a new Java exception.
 *
 * Use full paths like:
 *     - "java/lang/NoSuchFieldException"
 *     - "java/lang/NullPointerException"
 *     - "java/security/InvalidParameterException"
 * "java/lang/OutOfMemoryError"
 * "java/lang/Error"
 * "java/security/InvalidParameterException"
 * "java/io/IOException"
 */
struct NewJavaException : public ThrownJavaException
{
    NewJavaException(JNIEnv * env, const char* type="", const char* message="")
        :ThrownJavaException(type+std::string(" ")+message)
    {
        jclass newExcCls = env->FindClass(type);
        if (newExcCls != NULL)
            env->ThrowNew(newExcCls, message);
        //if it is null, a NoClassDefFoundError has been thrown
    }
};

#endif
