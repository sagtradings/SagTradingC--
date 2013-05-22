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





int main()
{
	return 0;
}

JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_sendLoginMessage(JNIEnv *env, jobject caller, jobjectArray array){
	 CThostFtdcTraderApi *pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	 printf("successfully made it to the cpp\n");
	 TraderEventHandler sh(pUserApi);
	 //sh.setJNIEnvironment(env);
	 pUserApi -> RegisterSpi(&sh);
	 pUserApi -> SubscribePrivateTopic(THOST_TERT_RESUME);
	 pUserApi -> SubscribePublicTopic(THOST_TERT_RESUME);
	 pUserApi -> RegisterFront("tcp://218.1.96.8:41213");
	 //CThostFtdcReqUserLoginField field();
	 
	 //pUserApi -> ReqUserLogin(

	 pUserApi -> Init();

	 WaitForSingleObject(g_hEvent, WAIT_TIMEOUT);

	 
	 pUserApi ->Release();
	 
	 printf("exiting program");
	 observers.clear();
	
}

JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_sendOrderInsertMessage(JNIEnv *env, jobject caller, jobjectArray array){
}

JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_sendReturnOrderMessage(JNIEnv *env, jobject caller, jobjectArray array){
};

JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_subscribeListener(JNIEnv *env, jobject caller, jobject subscriber){
	printf("subscribinganobject\n");
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
  (JNIEnv *, jobject, jobjectArray){
	// create a CThostFtdcMdApi instance
	CThostFtdcMdApi *pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	CThostFtdcTraderApi *pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	// create an event handler instance
	quotationeventhandler sh(pUserApi);
	TraderEventHandler th(pTraderApi);
	// register an event handler instance
	
	pTraderApi->RegisterSpi(&th);
	// register the CTP front address and port
	
	//pTraderApi ->RegisterFront("tcp://218.1.96.8:41205");
	pTraderApi->SubscribePrivateTopic(THOST_TERT_RESUME);
	pTraderApi->SubscribePublicTopic(THOST_TERT_RESUME);
	
	// start the connection between client and CTP server
	
	//pTraderApi->Init();
	// waiting for the quotation data
	//WaitForSingleObject(g_hEvent, INFINITE);
	// release API instance

	//pTraderApi -> Release();

	pUserApi->RegisterSpi(&sh);
	pUserApi->RegisterFront("tcp://218.1.96.8:41213");
	
	pUserApi->Init();
	WaitForSingleObject(g_hEvent, INFINITE);
	pUserApi->Release();
}

JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_sendTradeRequest
  (JNIEnv *, jobject, jobjectArray){
		// create a CThostFtdcMdApi instance
	CThostFtdcMdApi *pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	CThostFtdcTraderApi *pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	// create an event handler instance
	quotationeventhandler sh(pUserApi);
	TraderEventHandler th(pTraderApi);
	// register an event handler instance
	
	pTraderApi->RegisterSpi(&th);
	// register the CTP front address and port
	
	pTraderApi ->RegisterFront("tcp://218.1.96.8:41205");
	pTraderApi->SubscribePrivateTopic(THOST_TERT_RESUME);
	pTraderApi->SubscribePublicTopic(THOST_TERT_RESUME);
	
	// start the connection between client and CTP server
	
	pTraderApi->Init();
	// waiting for the quotation data
	WaitForSingleObject(g_hEvent, INFINITE);
	// release API instance

	pTraderApi -> Release();

	//pUserApi->RegisterSpi(&sh);
	//pUserApi->RegisterFront("tcp://218.1.96.8:41213");
	
	//pUserApi->Init();
	//WaitForSingleObject(g_hEvent, INFINITE);
	//pUserApi->Release();


}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved){
	cachedJvm = jvm;
	printf("JVM on load");
	return JNI_VERSION_1_6;
}
