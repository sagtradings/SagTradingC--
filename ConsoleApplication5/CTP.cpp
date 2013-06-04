// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "quotationeventhandler.cpp"
#include "TraderEventHandler.cpp"
#include "nativeinterfaces_DefaultNativeInterface.h"
#include "ThostFtdcTraderApi.h"
#include "globals.h"

using namespace std;
static JavaVM * cachedJvm;
HANDLE g_hEvent = CreateEvent(NULL, true, false, NULL);
// participant ID

TThostFtdcBrokerIDType g_chBrokerID;
// user id
TThostFtdcUserIDType g_chUserID;

list<jobject> observers;
CThostFtdcMdApi *pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
quotationeventhandler sh(pUserApi);



int main()
{
	return 0;
}

JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_sendLoginMessage(JNIEnv *env, jobject caller, jstring brokerId, jstring password, jstring investorId){
	 
	 printf("sendLoginMessage()\n");
	 pUserApi -> RegisterSpi(&sh);
	 pUserApi -> RegisterFront("tcp://218.1.96.8:41213");
	 printf("initing\n");
	 pUserApi -> Init();
	 printf("after init and waiting for event\n");
	 WaitForSingleObject(g_hEvent, INFINITE);
	 ResetEvent(g_hEvent);
	 printf("after reset event\n");
	 CThostFtdcReqUserLoginField reqUserLogin;
	 strcpy_s(reqUserLogin.UserID, env->GetStringUTFChars(investorId, false));
	 strcpy_s(reqUserLogin.Password, env->GetStringUTFChars(password, false));
	 strcpy_s(reqUserLogin.BrokerID, env->GetStringUTFChars(brokerId, false));
	 pUserApi->ReqUserLogin(&reqUserLogin, 0);
	 WaitForSingleObject(g_hEvent, INFINITE);
	 ResetEvent(g_hEvent);
	
}

JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_sendOrderInsertMessage(JNIEnv *env, jobject caller, jobjectArray array){
}

JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_sendReturnOrderMessage(JNIEnv *env, jobject caller, jobjectArray array){
};

JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_subscribeListener(JNIEnv *env, jobject caller, jobject subscriber){
	//printf("subscribinganobject\n");
	jobject o = env->NewGlobalRef(subscriber);
	observers.push_back(o);
}

JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_unSubscribeListener(JNIEnv *env, jobject caller, jobject subscriber){
		for(list<jobject>::iterator it = observers.begin(); it != observers.end(); ++it){
			if(env->IsSameObject(*it, subscriber)){
				observers.erase(it);
				return;
			}
		}
}
//
JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_sendQuoteRequest
  (JNIEnv *env, jobject callerObject, jobjectArray instruments){
	printf("sendQuoteRequest\n");
	int arrayLen = env->GetArrayLength(instruments);	
	char **c_instruments = (char**)malloc(sizeof(char*) * arrayLen);
	printf("start of copy loop\n");
	for(int i = 0, n = arrayLen; i < n; i++){
		printf("retrieving jobject\n");
		jobject object = env->GetObjectArrayElement(instruments, i);
		printf("casting jobject to jstring\n");
		jstring j_object = (jstring)object;
		printf("getting StringUTFChars\n");
		const char *c_castedObject = env ->GetStringUTFChars(j_object, 0);
		
		//printf(" malloc for c_instruments[%i]:%s\n", i, c_castedObject);
		//printf("initial value: %sxxx\n", c_instruments[i]);
		c_instruments[i] = (char*)malloc(7 * sizeof(char));
		printf("copying string\n");
		strcpy_s(c_instruments[i], 7, c_castedObject);
		printf("deleting local ref\n");
		env->DeleteLocalRef(j_object);
		printf("releasing chars\n");
		env->ReleaseStringUTFChars(j_object, c_castedObject);

	}
	printf("end of copy loop\n");
	for(int i = 0, n = arrayLen; i < n; i++){
		printf("[%i]:%s\n", i, c_instruments[i]);
	}
	pUserApi->SubscribeMarketData(c_instruments, arrayLen);
	for(int i = 0, n = arrayLen; i < n; i++){
		free(c_instruments[i]);
	}
	free(c_instruments);
	printf("exiting method\n");
}

JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_sendTradeRequest
  (JNIEnv *, jobject, jobjectArray){
		// create a CThostFtdcMdApi instance



}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved){
	cachedJvm = jvm;
	//printf("JVM on load");
	return JNI_VERSION_1_1;
;
}
