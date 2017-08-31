/*

* (c) PIONEER CORPORATION 2014

* 25-1 Nishi-machi Yamada Kawagoe-shi Saitama-ken 350-8555 Japan

* All Rights Reserved.

*/

 /*===========================================================================*/
/** @file
    コーダクラス実装ファイル

    @attention C++ 専用です C では使用出来ません
*/
/*===========================================================================*/

/*=============================================================================
   インクルードファイルの読み込み                                            */

#include "NPLOGID_UCOM.h"
#include "NPuComCoder.h"
#include "NBinaryQueue.h"


/*=============================================================================
	グローバル宣言                                                           */

// コーダクラス
NPuComCoder* NPuComCoder::s_pcInstance = NULL;
// 同期オブジェクト
NSyncObject NPuComCoder::s_cSync;

/*=============================================================================
	定数定義                                                                 */

const DWORD	NP_UCOM_QUEUE_FRAME_NUM		= 50;	//!< 50個分
const DWORD	NP_UCOM_SIZE_DATA_MAX		= NP_UCOM_SIZE_FRAME_MAX + NP_UCOM_SIZE_CS;	//!< フレーム＋ＣＳ
const DWORD	NP_UCOM_SIZE_DATA_MIN		= NP_UCOM_SIZE_FRAME_MIN + NP_UCOM_SIZE_CS;	//!< フレーム＋ＣＳ

const BYTE DATA_DLE = 0x9F;
const BYTE DATA_STX = 0x02;
const BYTE DATA_ETX = 0x03;

const DWORD STATUS_WAIT_DLE1	= 0x00000000;
const DWORD STATUS_WAIT_STX		= 0x00000001;
const DWORD STATUS_WAIT_DATA	= 0x00000002;
const DWORD STATUS_WAIT_ETX		= 0x00000003;

const DWORD DATAPOS_TP	= 0;
const DWORD DATAPOS_BD	= 1;
const DWORD DATAPOS_D	= NP_UCOM_SIZE_BD + 1;

const DWORD CHECKSIZE_D		= NP_UCOM_SIZE_PACKET_LIMIT - (NP_UCOM_SIZE_CS + NP_UCOM_SIZE_DLE_END + NP_UCOM_SIZE_ETX);
const DWORD CHECKSIZE_CS	= NP_UCOM_SIZE_PACKET_LIMIT - (NP_UCOM_SIZE_DLE_END + NP_UCOM_SIZE_ETX);

/******************************************************************************

    コンストラクタ

******************************************************************************/
NPuComCoder::NPuComCoder()
:	// 初期化リスト
	NObject(),
	m_sPacket(),
	m_sStockData(),
	m_pRcvQueue(NULL),
	m_dwRcvNum(0),
	m_dwRcvStatus(STATUS_WAIT_DLE1)
{
	// 構造体の初期化
	memset(&m_sPacket, 0, sizeof(m_sPacket));
	memset(&m_sStockData, 0, sizeof(m_sStockData));
	// キューの生成
	m_pRcvQueue = new NBinaryQueue(sizeof(NPuComDataFrame) * NP_UCOM_QUEUE_FRAME_NUM);
}

/******************************************************************************

    デストラクタ

******************************************************************************/
NPuComCoder::~NPuComCoder()
{
	// キューの破棄
	if(NULL != m_pRcvQueue){
		delete m_pRcvQueue;
		m_pRcvQueue = NULL;
	}
}

/******************************************************************************

    コーダクラスインスタンス取得関数

    【解説】
        プロセスで唯一のインスタンスの取得（シングルトンパターン）
        インスタンスが未生成の場合のみ生成し，既にインスタンスが生成されて
        いる場合はそのインスタンスを返す．シングルトンパターンとなっている．

******************************************************************************/
NPuComCoder
*NPuComCoder::Instance( VOID )
{
	s_cSync.SynchronizeStart();
	
	if ( NULL == s_pcInstance ){
		s_pcInstance = new NPuComCoder();
	}

	s_cSync.SynchronizeEnd();

	return s_pcInstance;
}

/******************************************************************************

    コーダクラスインスタンス破棄関数

    【解説】
        インスタンスの破棄を行なう

******************************************************************************/
VOID
NPuComCoder::Destroy( VOID )
{
	s_cSync.SynchronizeStart();

	if ( NULL != s_pcInstance ){
		delete s_pcInstance;
		s_pcInstance = NULL;
	}

	s_cSync.SynchronizeEnd();
}


/******************************************************************************

    Updateパケット判定処理

    【解説】
        パラメータのTPがUpdateパケットかどうかを判定する。

******************************************************************************/
NP_BOOL NPuComCoder::IsUpdatePacket( BYTE byTP)
{
	return (  NP_UCOM_TP_UPDATE_WRITE_DATA == byTP);
}

/******************************************************************************

    パケット設定

    【解説】
        受信したパケットを保存してデコードを行う

******************************************************************************/
VOID
NPuComCoder::Encode( NPuComDataFrameUpdate *psFrame, NPuComPacketUpdate *psPacket )
{
	DWORD dwDataSize = 0;
	DWORD dwSetSize  = 0;
	BYTE  byCS	     = 0;

	if (psFrame->m_dwFrameSize >= (NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD))
	{
		dwDataSize = psFrame->m_dwFrameSize - (NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD);
	}
	else
	{
		psPacket->m_dwPacketNum = 0;
		return;
	}
	
	// パケットサイズの制限
	if( NP_FALSE != IsUpdatePacket( psFrame->m_byTP)) {
		// Update系パケットなので、サイズの拡張(260byte)はOK
		if( NP_UCOM_SIZE_D_UPDATE < dwDataSize) {
			psPacket->m_dwPacketNum = 0;
			return;
		}
	}
	else {
		// 通常系パケットなので、サイズは44byte
		if( NP_UCOM_SIZE_D_NORMAL < dwDataSize) {
			psPacket->m_dwPacketNum = 0;
			return;
		}
	}

	// DLEの設定
	psPacket->m_byPacket[dwSetSize] = DATA_DLE;
	++dwSetSize;

	// STXの設定
	psPacket->m_byPacket[dwSetSize] = DATA_STX;
	++dwSetSize;

	// TPの設定
	psPacket->m_byPacket[dwSetSize] = psFrame->m_byTP;
	byCS = byCS ^ psFrame->m_byTP;
	++dwSetSize;
	if(DATA_DLE == psFrame->m_byTP){
		psPacket->m_byPacket[dwSetSize] = DATA_DLE;
		++dwSetSize;
	}

	// BDの設定
	{
		for(DWORD dwLoopBD = 0; dwLoopBD < NP_UCOM_SIZE_BD; ++dwLoopBD){
			psPacket->m_byPacket[dwSetSize] = psFrame->m_byBD[dwLoopBD];
			byCS = static_cast<BYTE>( byCS ^ psFrame->m_byBD[dwLoopBD]);
			++dwSetSize;
			if(DATA_DLE == psFrame->m_byBD[dwLoopBD]){
				psPacket->m_byPacket[dwSetSize] = DATA_DLE;
				++dwSetSize;
			}
		}
	}

	// Dの設定
	{
		for(DWORD dwLoopD = 0; dwLoopD < dwDataSize; ++dwLoopD){
			psPacket->m_byPacket[dwSetSize] = psFrame->m_byD[dwLoopD];
			byCS = static_cast<BYTE>( byCS ^ psFrame->m_byD[dwLoopD]);
			++dwSetSize;
			if(DATA_DLE == psFrame->m_byD[dwLoopD]){
				psPacket->m_byPacket[dwSetSize] = DATA_DLE;
				++dwSetSize;
			}
			// サイズチェック
			if(CHECKSIZE_D < dwSetSize){
				// 即時抜ける
				return;
			}
		}
	}

	// CSの設定
	psPacket->m_byPacket[dwSetSize] = byCS;
	++dwSetSize;
	if(DATA_DLE == byCS){
		psPacket->m_byPacket[dwSetSize] = DATA_DLE;
		++dwSetSize;
	}
	// サイズチェック
	if(CHECKSIZE_CS < dwSetSize){
		// 即時抜ける
		return;
	}

	// DLEの設定
	psPacket->m_byPacket[dwSetSize] = DATA_DLE;
	++dwSetSize;

	// ETXの設定
	psPacket->m_byPacket[dwSetSize] = DATA_ETX;
	++dwSetSize;

	// 最後にサイズの設定
	psPacket->m_dwPacketNum = dwSetSize;
}

/******************************************************************************

    パケット設定

    【解説】
        受信したパケットを保存してデコードを行う

******************************************************************************/
DWORD
NPuComCoder::SetPacket( const NPuComPacketUpdate *psPacket )
{
	// データのコピー
	memcpy(&m_sPacket, psPacket, sizeof(m_sPacket));

	// デコード
	Decode();

	return m_dwRcvNum;
}

/******************************************************************************

    パケット設定

    【解説】
        受信したパケットを保存してデコードを行う

******************************************************************************/
VOID
NPuComCoder::GetFrame( NPuComDataFrame *psFrame )
{
	if(0 != m_dwRcvNum){
		// データのコピー
		m_pRcvQueue->Pop(psFrame, sizeof(NPuComDataFrame));
		// データ数デクリメント
		--m_dwRcvNum;
	}
}

/******************************************************************************

    パケット設定

    【解説】
        受信したパケットを保存してデコードを行う

******************************************************************************/
VOID
NPuComCoder::Decode( VOID )
{
	// Store last decoded data
	DWORD	&dwSize   = m_sStockData.m_dwPacketNum;;
	BYTE	*pbyBuff = m_sStockData.m_byPacket;

	// Decode  every byte from UART(m_sPacket.m_dwPacketNum)
	for(DWORD dwReadNum = 0; dwReadNum < m_sPacket.m_dwPacketNum; ++dwReadNum){
		// デコード処理
		switch(m_dwRcvStatus){
			// wait DLE(0x9f),change wait state to wait STX,do not store data,
		case STATUS_WAIT_DLE1:
			if(DATA_DLE == m_sPacket.m_byPacket[dwReadNum]){
				// STX待ちに進む
				m_dwRcvStatus = STATUS_WAIT_STX;
			}
			break;
				// wait STX(0x02)
		case STATUS_WAIT_STX:
			// if wait STX(0x02) OK,change wait state to wait data, not store data
			if(DATA_STX == m_sPacket.m_byPacket[dwReadNum]){
				// DATA待ちに進む
				m_dwRcvStatus = STATUS_WAIT_DATA;
			}
			// if no STX(0x02), Return to DLE wait state,reset size
			else{
				// DLE1待ちに戻る
				m_dwRcvStatus = STATUS_WAIT_DLE1;
				// 構造体の初期化
				memset(pbyBuff, 0, NP_UCOM_SIZE_DATA_MAX);
				dwSize = 0;
			}
			break;
			// wait data(data(0xxx)  / DLE(0x9f)STX(02)  /  DLE(0x9f)DLE(0x9f)  / DLE(0x9f)ETX(03)....)
		case STATUS_WAIT_DATA:
			// if DLE(0x9f),change state to wait ETX,don't store data
			if(DATA_DLE == m_sPacket.m_byPacket[dwReadNum]){
				// ETX待ちに進む
				m_dwRcvStatus = STATUS_WAIT_ETX;
			}
			// other case 
			else{
				// if oversize error サイズチェック,reset wait state,reset dwSize
				if(NP_UCOM_SIZE_DATA_MAX <= dwSize){
					// サイズオーバー（DLE1待ちに戻る）
					m_dwRcvStatus = STATUS_WAIT_DLE1;
					// 構造体の初期化
					memset(pbyBuff, 0, NP_UCOM_SIZE_DATA_MAX);
					dwSize = 0;
				}
				// if size < NP_UCOM_SIZE_DATA_MAX,there is free space to hold data,store data
				else{
					// データの保存
					pbyBuff[dwSize] = m_sPacket.m_byPacket[dwReadNum];
					++dwSize;
				}
			}
			break;
			// the state is changed from wait data, (DLE(0x9f)STX(02)  /  DLE(0x9f)DLE(0x9f) / DLE(0x9f)ETX(03) ......)
		case STATUS_WAIT_ETX:
			// if DLE(0x9f)DLE(0x9f) ,it is treated as data byte
			if(DATA_DLE == m_sPacket.m_byPacket[dwReadNum]){
				// サイズチェック,Oversize error,reset wait state,reset  dwSize
				if(NP_UCOM_SIZE_DATA_MAX <= dwSize){
					// サイズオーバー（DLE1待ちに戻る）
					m_dwRcvStatus = STATUS_WAIT_DLE1;
					// 構造体の初期化
					memset(pbyBuff, 0, NP_UCOM_SIZE_DATA_MAX);
					dwSize = 0;
				}
				// double DLE,save one DLE(0x9f) in decoded data
				else{
					// ２重DLE(保存してDATA待ちに戻る)
					m_dwRcvStatus = STATUS_WAIT_DATA;
					pbyBuff[dwSize] = m_sPacket.m_byPacket[dwReadNum];
					++dwSize;
				}
			}
			// if DLE(0x9f)STX(02) ,wait end error,wait STX,change wait state to wait data,reset dwSize
			else if(DATA_STX == m_sPacket.m_byPacket[dwReadNum]){
				// DATA待ちに戻る
				m_dwRcvStatus = STATUS_WAIT_DATA;
				// データは破棄
				memset(pbyBuff, 0, NP_UCOM_SIZE_DATA_MAX);
				dwSize = 0;
			}
			// DLE(0x9f)ETX(03) , wait end OK
			else if(DATA_ETX == m_sPacket.m_byPacket[dwReadNum]){
				// １フレーム受信完了,size check
				if((NP_UCOM_SIZE_DATA_MIN <= dwSize) && (NP_UCOM_SIZE_DATA_MAX >= dwSize)){
					// データサイズ正常,check sum
					if(NP_FALSE != CheckDataSum(pbyBuff, dwSize)){
						// チェックサム正常
						{
							NPuComDataFrame sTmpFrame;
							// 構造体の初期化
							memset(&sTmpFrame, 0, sizeof(sTmpFrame));

							// データのコピー
							sTmpFrame.m_dwFrameSize = (dwSize - 1);	// CS分を除外
							sTmpFrame.m_byTP = pbyBuff[DATAPOS_TP];
							memcpy(&sTmpFrame.m_byBD[NP_UCOM_BD1], &pbyBuff[DATAPOS_BD], NP_UCOM_SIZE_BD);
							memcpy(&sTmpFrame.m_byD[NP_UCOM_D0],   &pbyBuff[DATAPOS_D],  (sTmpFrame.m_dwFrameSize - (NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD)));

							// キューにpush
							if(FALSE == m_pRcvQueue->IsFull()){
								m_pRcvQueue->Push(&sTmpFrame, sizeof(sTmpFrame));
								// キュー数の変更
								++m_dwRcvNum;
							}
							else{
								// キューあふれ
								OutputErrLog("NPuComCoder.cpp", __LINE__, AIL_ERROR_LOG_CODE_COMDEVICE_ERROR, "DroidAVN", "NPuComCoder::Decode QUEUE OVER FLOW");
							}
						}
					}
				}
				// config one decoded package,reset wait state,reset dwSize
				// DLE1待ちから再度処理
				m_dwRcvStatus = STATUS_WAIT_DLE1;
				// 構造体の初期化
				memset(pbyBuff, 0, NP_UCOM_SIZE_DATA_MAX);
				dwSize = 0;
			}
			// other case,wait end error,Reset wait state to wait DLE,reset dwSize
			else{
				// DLE1待ちに戻る
				m_dwRcvStatus = STATUS_WAIT_DLE1;
				// 構造体の初期化
				memset(pbyBuff, 0, NP_UCOM_SIZE_DATA_MAX);
				dwSize = 0;
			}
			break;
			//other case, state error
		default:
			// ありえないけどとりあえず
			m_dwRcvStatus = STATUS_WAIT_DLE1;
			// 構造体の初期化
			memset(pbyBuff, 0, NP_UCOM_SIZE_DATA_MAX);
			dwSize = 0;
			break;
		}
	}

}

/******************************************************************************

    チェックサム確認

    【解説】
        チェックサムの確認を行う

******************************************************************************/
NP_BOOL
NPuComCoder::CheckDataSum(PBYTE pbyCheckData, DWORD dwCheckSize)
{
	DWORD dwChkNum	 = dwCheckSize - 1;
	BYTE  byPacketCS = pbyCheckData[dwChkNum];
	BYTE  byCheckCS  = 0; 

	// 1BYTEづつXORしていく
	for(DWORD dwLoop = 0; dwLoop < dwChkNum; ++dwLoop){
		byCheckCS = static_cast<BYTE>(byCheckCS ^ pbyCheckData[dwLoop]);
	}

	// チェック
	if(byPacketCS != byCheckCS){
		return NP_FALSE;
	}

	return NP_TRUE;
}

/*===========================================================================*/
/*
 * 【更新履歴】
 * $Log: NPuComCoder.cpp,v $
 * Revision 1.1  2008/08/08 06:20:03  tomoyuki
 * 新規登録
 *
 */
/*===========================================================================*/
//================================================================= End of File
