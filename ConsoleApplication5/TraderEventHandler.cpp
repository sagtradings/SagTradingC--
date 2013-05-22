#include "globals.h"

class TraderEventHandler : public CThostFtdcTraderSpi{
		public:
			TraderEventHandler(CThostFtdcTraderApi *pTraderApi) : m_TraderApi(pTraderApi){}
			~TraderEventHandler(){}

		virtual void OnFrontConnected(){

			printf("the front is connected");
			CThostFtdcReqUserLoginField reqUserLogin;
			// get BrokerID
			
			//const char  *userID = "00000008";
			//const char  *password = "123321";
			//const char  *brokerID = "1013";
			strcpy_s(reqUserLogin.UserID, "00000008");
			strcpy_s(reqUserLogin.Password, "123321");
			strcpy_s(reqUserLogin.BrokerID, "1013");
			

			//delete userID;
			//delete password;
			//delete brokerID;


			// send the login request
			m_TraderApi->ReqUserLogin(&reqUserLogin, 0);
		}

		
		

		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField*pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){

				printf("OnRspUserLogin:\n");
				printf("ErrorCode=[%d], ErrorMsg=[%s]\n",
				pRspInfo->ErrorID, pRspInfo->ErrorMsg);
					printf("RequestID=[%d], Chain=[%d]\n", nRequestID,bIsLast);
				if (pRspInfo->ErrorID != 0) {
				// in case any login failure, the client should handlethis error.
					printf("Failed to login, errorcode=%d errormsg=%s requestid=%d chain=%d", pRspInfo->ErrorID, pRspInfo->ErrorMsg,
					nRequestID, bIsLast);
					exit(-1);
				}
				// login success, then send order insertion request.
				CThostFtdcInputOrderField ord;
				memset(&ord, 0, sizeof(ord));
				
				//broker id
				strcpy_s(ord.BrokerID, "1013");
				//investor ID
				strcpy_s(ord.InvestorID, "1013");
				// instrument ID
				//strcpy_s(ord.InstrumentID, "cn0601");
				//strcpy_s(ord.InstrumentID, "IF0809");
				strcpy_s(ord.InstrumentID, "IF1305");
				///order reference
				strcpy_s(ord.OrderRef, "000000000001");
				// user id
				strcpy_s(ord.UserID, "00000008");
				// order price type
				ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
				// direction
				ord.Direction = THOST_FTDC_D_Buy;
				// combination order’s offset flag
				strcpy_s(ord.CombOffsetFlag, "0");
				// combination or hedge flag
				strcpy_s(ord.CombHedgeFlag, "1");
				// price
				ord.LimitPrice = 50000;
				// volume
				ord.VolumeTotalOriginal = 10;
				// valid date
				ord.TimeCondition = THOST_FTDC_TC_GFD;
				// GTD DATE
				strcpy_s(ord.GTDDate, "");
				// volume condition
				ord.VolumeCondition = THOST_FTDC_VC_AV;
				// min volume
				ord.MinVolume = 0;
				// trigger condition
				
				ord.ContingentCondition = THOST_FTDC_CC_Immediately;
				// stop price
				ord.StopPrice = 0;
				// force close reason
				ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
				// auto suspend flag
				ord.IsAutoSuspend = 0;
				//m_TraderApi->ReqOrderInsert(&ord, 1);
				
				CThostFtdcQryTradeField qryTrade;

			const char  *userID = "00000008";
			const char  *password = "123321";
			const char  *brokerID = "1013";


				strcpy_s(qryTrade.BrokerID, brokerID);
				strcpy_s(qryTrade.InstrumentID, "if1305");
				strcpy_s(qryTrade.InvestorID, userID);
				
				m_TraderApi -> ReqQryTrade(&qryTrade, 0);

				//m_TraderApi->ReqQryDeptMarketData();

		}



		virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
			printf("OnRspError:\n");
			printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,pRspInfo->ErrorMsg);
			printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
		}

		virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){

			//output the order insert result
			printf("OnRspQryTrade");
			//printf("askPrice1=[%f], Instrument=[%s]\n", pDepthMarketData->AskPrice1,pDepthMarketData->InstrumentID);




			JNIEnv * g_env;
			jint success = cachedJvm -> GetEnv((void**)&g_env, JNI_VERSION_1_6);
			if(success != JNI_OK){
				printf("attempting to resync jvm to thread\n");
				jint attachSuccess = cachedJvm ->AttachCurrentThread((void**)&g_env, NULL);
				if(attachSuccess == 0){
					printf("resync successful!\n");
				}
				else{
					printf("resync unsuccessful\n");
					return;
				}
			}
			list<jobject>::iterator it = observers.begin();			
			while (it != observers.end()) {
				jobject &obj = *it;
				jclass cls = g_env->GetObjectClass(obj);
				jmethodID mid = g_env->GetMethodID(cls, "onRtnTradingData", "(Lbo/TradeDataResponse;)V");

				jclass parameter = g_env->FindClass("bo/TradeDataResponse");
			    jmethodID midConstructor = (g_env)->GetMethodID(parameter, "<init>", "()V");
				jobject paramObject = (g_env)->NewObject(parameter, midConstructor);

				if (mid == 0) {
					printf("mid was 0!!!\n");
					return;
				}

				g_env->CallVoidMethod(obj, mid, paramObject);
				it++;
			}
		}

		virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
			// output the order insertion result
			printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
			// inform the main thread order insertion is over
			SetEvent(g_hEvent);
		}

		private:
			CThostFtdcTraderApi *m_TraderApi;
};