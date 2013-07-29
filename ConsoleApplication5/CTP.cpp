// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "quotationeventhandler.cpp"
#include "TraderEventHandler.cpp"
#include "nativeinterfaces_MarketDataNativeInterface.h"
#include "ThostFtdcTraderApi.h"
#include "globals.h"

using namespace std;
 JavaVM * cachedJvm;
HANDLE end_hEvent = CreateEvent(NULL, true, false, NULL);
HANDLE g_hEvent = CreateEvent(NULL, true, false, NULL);
// participant ID

TThostFtdcBrokerIDType g_chBrokerID;
// user id
TThostFtdcUserIDType g_chUserID;

list<jobject> observers;
CThostFtdcMdApi *pUserApi = NULL;
quotationeventhandler sh;



int main()
{
	return 0;
}

JNIEXPORT void JNICALL Java_nativeinterfaces_MarketDataNativeInterface_sendLoginMessage(JNIEnv *env, jobject caller, jstring brokerId, jstring password, jstring investorId, jstring connectionUrl){	 
	if(pUserApi == NULL){
		printf("pUserApi is null!");
		pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
		pUserApi -> RegisterSpi(&sh);
		//int urlLength = env->GetStringLength(connectionUrl);
		int urlLength = env->GetStringLength(connectionUrl);
		char * writable = new char[urlLength + 1];
		printf("the length is %i", urlLength);
		writable[urlLength + 1] = '\0';
		strcpy_s(writable, urlLength + 1, env->GetStringUTFChars(connectionUrl, false));
		pUserApi ->RegisterFront(writable);
		printf("initing\n");
		pUserApi -> Init();
		WaitForSingleObject(g_hEvent, INFINITE);
		ResetEvent(g_hEvent);
		delete[] writable;
		
	}
			CThostFtdcReqUserLoginField reqUserLogin;
		strcpy_s(reqUserLogin.UserID, env->GetStringUTFChars(investorId, false));
		strcpy_s(reqUserLogin.Password, env->GetStringUTFChars(password, false));
		strcpy_s(reqUserLogin.BrokerID, env->GetStringUTFChars(brokerId, false));
		pUserApi->ReqUserLogin(&reqUserLogin, 0);
		WaitForSingleObject(g_hEvent, INFINITE);




	 printf("done logging in\n");
	
}





JNIEXPORT void JNICALL Java_nativeinterfaces_MarketDataNativeInterface_subscribeListener(JNIEnv *env, jobject caller, jobject subscriber){
	//printf("subscribinganobject\n");
	jobject o = env->NewGlobalRef(subscriber);
	observers.push_back(o);
}

JNIEXPORT void JNICALL Java_nativeinterfaces_MarketDataNativeInterface_unSubscribeListener(JNIEnv *env, jobject caller, jobject subscriber){
	if(pUserApi != NULL) {
		//pUserApi->Release();

		//pUserApi = NULL;
		observers.clear();
	}
	cachedJvm->DetachCurrentThread();
	
	
}
//
JNIEXPORT void JNICALL Java_nativeinterfaces_MarketDataNativeInterface_sendQuoteRequest
  (JNIEnv *env, jobject callerObject, jobjectArray instruments){
	int arrayLen = env->GetArrayLength(instruments);	
	char **c_instruments = (char**)malloc(sizeof(char*) * arrayLen);
	for(int i = 0, n = arrayLen; i < n; i++){

		jobject object = env->GetObjectArrayElement(instruments, i);

		jstring j_object = (jstring)object;

		const char *c_castedObject = env ->GetStringUTFChars(j_object, 0);
		c_instruments[i] = (char*)malloc(7 * sizeof(char));
		strcpy_s(c_instruments[i], 7, c_castedObject);
		env->DeleteLocalRef(j_object);
		env->ReleaseStringUTFChars(j_object, c_castedObject);

	}
	pUserApi->SubscribeMarketData(c_instruments, arrayLen);
	
	for(int i = 0, n = arrayLen; i < n; i++){
		free(c_instruments[i]);
	}
	free(c_instruments);
}

JNIEXPORT void JNICALL Java_nativeinterfaces_MarketDataNativeInterface_sendUnsubscribeQuoteRequest
  (JNIEnv *env, jobject callerObject, jobjectArray instruments){
	int arrayLen = env->GetArrayLength(instruments);	
	char **c_instruments = (char**)malloc(sizeof(char*) * arrayLen);
	for(int i = 0, n = arrayLen; i < n; i++){

		jobject object = env->GetObjectArrayElement(instruments, i);

		jstring j_object = (jstring)object;

		const char *c_castedObject = env ->GetStringUTFChars(j_object, 0);
		c_instruments[i] = (char*)malloc(7 * sizeof(char));
		strcpy_s(c_instruments[i], 7, c_castedObject);
		env->DeleteLocalRef(j_object);
		env->ReleaseStringUTFChars(j_object, c_castedObject);

	}
	pUserApi->UnSubscribeMarketData(c_instruments, arrayLen);
	
	for(int i = 0, n = arrayLen; i < n; i++){
		free(c_instruments[i]);
	}
	free(c_instruments);



}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved){
	cachedJvm = jvm;
	//printf("JVM on load");
	return JNI_VERSION_1_6;

}
