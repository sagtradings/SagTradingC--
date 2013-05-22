
#include "globals.h"
#include <ctime>


class quotationeventhandler : public CThostFtdcMdSpi{
	public:
		quotationeventhandler(CThostFtdcMdApi *pUserApi) :  m_pUserApi(pUserApi){}
			~quotationeventhandler(){}

		virtual void OnFrontConnected(){
			printf("the front is connected");
			CThostFtdcReqUserLoginField reqUserLogin;
			// get BrokerID
			
			const char  *userID = "00000008";
			const char  *password = "123321";
			const char  *brokerID = "1013";
			strcpy_s(reqUserLogin.UserID, userID);
		
			strcpy_s(reqUserLogin.Password, password);
			strcpy_s(reqUserLogin.BrokerID, brokerID);

			//delete userID;
			//delete password;
			//delete brokerID;


			// send the login request
			m_pUserApi->ReqUserLogin(&reqUserLogin, 0);
			

		}

		virtual void OnFrontDisconnected(int nReason){
			printf("front disconnected");
		}

	

		
		

		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
			printf("OnRspUserLogin:\n");
			printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
			printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
			if (pRspInfo->ErrorID != 0) {
				// login failure, the client should handle this error.
				printf("Failed to login, errorcode=%d errormsg=%s requestid=%d=%d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
				return;
			}
			// login success, then subscribe the quotation information
			//char * Instrumnet[]={"IF0809","IF0812"};
			//char * Instrumnet[]={"CADC","CAIHF"};
			char * Instrumnet[]={"IF1306","IF1306"};

			m_pUserApi->SubscribeMarketData (Instrumnet,2);
			
			printf("marketDataSubscribed");

			
			

			CThostFtdcQryDepthMarketDataField field;
			strcpy_s(field.InstrumentID, "CADC");
			//m_pTraderApi ->ReqQryDepthMarketData(&field, 0);

			//or unsubscribe the quotation
			//m_pUserApi->UnSubscribeMarketData (Instrumnet,2);
			printf("sent quoatation request");
			
		}

		virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData){
			//output the order insert result




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
				jmethodID mid = g_env->GetMethodID(cls, "onRtnDepthMarketData", "(Lbo/MarketDataResponse;)V");

				jclass parameter = g_env->FindClass("bo/MarketDataResponse");
			    jmethodID midConstructor = (g_env)->GetMethodID(parameter, "<init>", "()V");
				jobject paramObject = (g_env)->NewObject(parameter, midConstructor);


				jmethodID askPriceId = g_env->GetMethodID(parameter, "setAskPrice1", "(D)V");
				jmethodID actionDayId = g_env->GetMethodID(parameter, "setActionDay", "(Ljava/lang/String;)V");
				jmethodID askPrice2Id = g_env->GetMethodID(parameter, "setAskPrice2", "(D)V");
				jmethodID askPrice3Id = g_env->GetMethodID(parameter, "setAskPrice3", "(D)V");
				jmethodID askPrice4Id = g_env->GetMethodID(parameter, "setAskPrice4", "(D)V");

				jmethodID askPrice5Id = g_env->GetMethodID(parameter, "setAskPrice5", "(D)V");
				jmethodID askVolume1Id = g_env->GetMethodID(parameter, "setAskVolume1", "(I)V");
				jmethodID askVolume2Id = g_env->GetMethodID(parameter, "setAskVolume2", "(I)V");
				jmethodID askVolume3Id = g_env->GetMethodID(parameter, "setAskVolume3", "(I)V");
				jmethodID askVolume4Id = g_env->GetMethodID(parameter, "setAskVolume4", "(I)V");

				jmethodID askVolume5Id = g_env->GetMethodID(parameter, "setAskVolume5", "(I)V");
				jmethodID averagePriceId = g_env->GetMethodID(parameter, "setAveragePrice", "(D)V");
				jmethodID bidPrice1Id = g_env->GetMethodID(parameter, "setBidPrice1", "(D)V");
				jmethodID bidPrice2Id = g_env->GetMethodID(parameter, "setBidPrice2", "(D)V");
				jmethodID bidPrice3Id = g_env->GetMethodID(parameter, "setBidPrice3", "(D)V");

				jmethodID bidPrice4Id = g_env->GetMethodID(parameter, "setBidPrice4", "(D)V");
				jmethodID bidPrice5Id = g_env->GetMethodID(parameter, "setBidPrice5", "(D)V");
				jmethodID bidVolume1Id = g_env->GetMethodID(parameter, "setBidVolume1", "(I)V");
				jmethodID bidVolume2Id = g_env->GetMethodID(parameter, "setBidVolume2", "(I)V");
				jmethodID bidVolume3Id = g_env->GetMethodID(parameter, "setBidVolume3", "(I)V");

				jmethodID bidVolume4Id = g_env->GetMethodID(parameter, "setBidVolume4", "(I)V");
				jmethodID bidVolume5Id = g_env->GetMethodID(parameter, "setBidVolume5", "(I)V");
				jmethodID closePriceId = g_env->GetMethodID(parameter, "setClosePrice", "(D)V");
				jmethodID currDeltaId = g_env->GetMethodID(parameter, "setCurrDelta", "(D)V");
				jmethodID exchangeIdId = g_env->GetMethodID(parameter, "setExchangeID", "(Ljava/lang/String;)V");



				jmethodID highestPriceId = g_env->GetMethodID(parameter, "setHighestPrice", "(D)V");
				jmethodID InstrumentIdId = g_env->GetMethodID(parameter, "setInstrumentId", "(Ljava/lang/String;)V");
				jmethodID lastPriceId = g_env->GetMethodID(parameter, "setLastPrice", "(D)V");
				jmethodID lowerLimitPriceId = g_env->GetMethodID(parameter, "setLowerLimitPrice", "(D)V");
				jmethodID lowestPriceId = g_env->GetMethodID(parameter, "setLowestPrice", "(D)V");

				jmethodID openInterestId = g_env->GetMethodID(parameter, "setOpenInterest", "(D)V");
				jmethodID openPriceId = g_env->GetMethodID(parameter, "setOpenPrice", "(D)V");
				jmethodID preClosePriceId = g_env->GetMethodID(parameter, "setPreClosePrice", "(D)V");
				jmethodID preDeltaId = g_env->GetMethodID(parameter, "setPreDelta", "(D)V");
				jmethodID preOpenInterestId = g_env->GetMethodID(parameter, "setPreOpenInterest", "(D)V");

				jmethodID preSettlementPriceId = g_env->GetMethodID(parameter, "setPreSettlementPrice", "(D)V");
				jmethodID settlementPriceId = g_env->GetMethodID(parameter, "setSettlementPrice", "(D)V");
				jmethodID tradingDayId = g_env->GetMethodID(parameter, "setTradingDay", "(Ljava/lang/String;)V");
				jmethodID turnoverId = g_env->GetMethodID(parameter, "setTurnOver", "(D)V");
				jmethodID updateMillisecId = g_env->GetMethodID(parameter, "setUpdateMillisec", "(I)V");

				jmethodID updateTimeId = g_env->GetMethodID(parameter, "setUpdateTime", "(Ljava/lang/String;)V");
				jmethodID upperLimitPriceId = g_env->GetMethodID(parameter, "setUpperLimitPrice", "(D)V");
				jmethodID volumeId = g_env->GetMethodID(parameter, "setVolume", "(I)V");

				
				g_env->CallVoidMethod(paramObject, askPriceId, pDepthMarketData->AskPrice1);
				g_env->CallVoidMethod(paramObject, actionDayId, g_env->NewStringUTF(pDepthMarketData->ActionDay));
				g_env->CallVoidMethod(paramObject, askPrice2Id, pDepthMarketData->AskPrice2);
				g_env->CallVoidMethod(paramObject, askPrice3Id, pDepthMarketData->AskPrice3);
				g_env->CallVoidMethod(paramObject, askPrice4Id, pDepthMarketData->AskPrice4);

				g_env->CallVoidMethod(paramObject, askPrice5Id, pDepthMarketData->AskPrice5);
				g_env->CallVoidMethod(paramObject, askVolume1Id, pDepthMarketData->AskVolume1);
				g_env->CallVoidMethod(paramObject, askVolume2Id, pDepthMarketData->AskVolume2);
				g_env->CallVoidMethod(paramObject, askVolume3Id, pDepthMarketData->AskVolume3);
				g_env->CallVoidMethod(paramObject, askVolume4Id, pDepthMarketData->AskVolume4);

				g_env->CallVoidMethod(paramObject, askVolume5Id, pDepthMarketData->AskVolume5);
				g_env->CallVoidMethod(paramObject, averagePriceId, pDepthMarketData->AveragePrice);
				g_env->CallVoidMethod(paramObject, bidPrice1Id, pDepthMarketData->BidPrice1);
				g_env->CallVoidMethod(paramObject, bidPrice2Id, pDepthMarketData->BidPrice2);
				g_env->CallVoidMethod(paramObject, bidPrice3Id, pDepthMarketData->BidPrice3);

				g_env->CallVoidMethod(paramObject, bidPrice4Id, pDepthMarketData->BidPrice4);
				g_env->CallVoidMethod(paramObject, bidPrice5Id, pDepthMarketData->BidPrice5);
				g_env->CallVoidMethod(paramObject, bidVolume1Id, pDepthMarketData->BidVolume1);
				g_env->CallVoidMethod(paramObject, bidVolume2Id, pDepthMarketData->BidVolume2);
				g_env->CallVoidMethod(paramObject, bidVolume3Id, pDepthMarketData->BidVolume3);

				g_env->CallVoidMethod(paramObject, bidVolume4Id, pDepthMarketData->BidVolume4);
				g_env->CallVoidMethod(paramObject, bidVolume5Id, pDepthMarketData->BidVolume5);
				g_env->CallVoidMethod(paramObject, closePriceId, pDepthMarketData->ClosePrice);
				g_env->CallVoidMethod(paramObject, currDeltaId, pDepthMarketData->CurrDelta);
				g_env->CallVoidMethod(paramObject, exchangeIdId, g_env->NewStringUTF(pDepthMarketData->ExchangeID));

				/////

				g_env->CallVoidMethod(paramObject, highestPriceId, pDepthMarketData->HighestPrice);
				g_env->CallVoidMethod(paramObject, InstrumentIdId, g_env->NewStringUTF(pDepthMarketData->InstrumentID));
				g_env->CallVoidMethod(paramObject, lastPriceId, pDepthMarketData->LastPrice);
				g_env->CallVoidMethod(paramObject, lowerLimitPriceId, pDepthMarketData->LowerLimitPrice);
				g_env->CallVoidMethod(paramObject, lowestPriceId, pDepthMarketData->LowestPrice);

				g_env->CallVoidMethod(paramObject, openInterestId, pDepthMarketData->OpenInterest);
				g_env->CallVoidMethod(paramObject, openPriceId, pDepthMarketData->OpenPrice);
				g_env->CallVoidMethod(paramObject, preClosePriceId, pDepthMarketData->PreClosePrice);
				g_env->CallVoidMethod(paramObject, preDeltaId, pDepthMarketData->PreDelta);
				g_env->CallVoidMethod(paramObject, preOpenInterestId, pDepthMarketData->PreOpenInterest);

				g_env->CallVoidMethod(paramObject, preSettlementPriceId, pDepthMarketData->PreSettlementPrice);
				g_env->CallVoidMethod(paramObject, settlementPriceId, pDepthMarketData->SettlementPrice);
				g_env->CallVoidMethod(paramObject, tradingDayId, g_env->NewStringUTF(pDepthMarketData->TradingDay));
				g_env->CallVoidMethod(paramObject, turnoverId, pDepthMarketData->Turnover);
				g_env->CallVoidMethod(paramObject, updateMillisecId, pDepthMarketData->UpdateMillisec);


				g_env->CallVoidMethod(paramObject, updateTimeId, g_env->NewStringUTF(pDepthMarketData->UpdateTime));
				g_env->CallVoidMethod(paramObject, upperLimitPriceId, pDepthMarketData->UpperLimitPrice);
				g_env->CallVoidMethod(paramObject, volumeId, pDepthMarketData->Volume);



				

				if (mid == 0) {
					printf("mid was 0!!!\n");
					return;
				}

				g_env->CallVoidMethod(obj, mid, paramObject);
				it++;

			}

			//SetEvent(g_hEvent);
			// set the flag when the quotation data received.
			//SetEvent(CreateEvent(NULL, true, false, NULL));
		}


		virtual void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
			printf("You did it this time jerk");
		}

		virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
			printf("OnRspError:\n");
			printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,pRspInfo->ErrorMsg);
			printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
		}

		virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
			printf("SUBSCRIBBBED MARKET DATA!!!!!");
		}

		virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
			printf("OnRspQryInstrument\n\n\n");
		}





	private:
		CThostFtdcMdApi *m_pUserApi;
		
};