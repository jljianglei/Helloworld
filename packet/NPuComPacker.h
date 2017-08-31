/*

* (c) PIONEER CORPORATION 2014

* 25-1 Nishi-machi Yamada Kawagoe-shi Saitama-ken 350-8555 Japan

* All Rights Reserved.

*/


/*===========================================================================*/
/** @file
    パッカークラス定義ファイル

    @attention C++ 専用です C では使用出来ません
*/
/*===========================================================================*/

#ifndef CXX_NPUCOMPACKER_H
#define CXX_NPUCOMPACKER_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


/*=============================================================================
   インクルードファイルの読み込み                                            */

#include "NSyncObject.h"
#include "NPSystemBase.h"
#include "NPuComHandler.h"
#include "NPuComMemory.h"


/*=============================================================================
	定数定義                                                                 */

/*===========================================================================*/
/**
    パックＩＤ定義
*/
/*===========================================================================*/
enum NP_UCOM_PACK_ID
{
	NP_UCOM_PACK_CONECHK_SEND			= 0x00,	//!< 初期通信送信 TP=0x00
	NP_UCOM_PACK_CONECHK1_SEND,                                    //!< 初期通信送信 TP=0x02
	NP_UCOM_PACK_CONECHK2_SEND,                                    //!< 初期通信送信 TP=0x05
	NP_UCOM_PACK_APK_ON_NOTIFY,                                      //!< APL ON 送信  TP=0x06
	NP_UCOM_PACK_POWER_ON_OFF_TPE3,			         //!< POWER ON OFF TP=0xE3
	NP_UCOM_PACK_DTV_TPE4_NOTIFY,					     //!< KM506 DTV NOTIFY 通知送信 TP=0xE4
	NP_UCOM_PACK_SRC_CHANGE_RES,                                    //!< SOURCE CHANGE応答送信 TP=0x41, D0=0x01
	NP_UCOM_PACK_SRC_CHANGE_REQ,                                   //!< SOURCE CHANGE要求送信 TP=0x41,D0=0x03
	NP_UCOM_PACK_SRC_STATUS_NOTIFY,                               //!< SOC SOURCE STATUS送信 TP=0x49
	NP_UCOM_PACK_DIRECTCMD_SEND,				        //!< ダイレクトコマンド  (TP=0x50)
	NP_UCOM_PACK_SRC_DIRECTCMD_SEND,			       //!< ソース別ダイレクトコマンド  (TP=0x51)
	NP_UCOM_PACK_DISP_DATA_REQ_SEND,                           //!< 表示データ要求送信  TP=0x68
	NP_UCOM_PACK_UPDATE_START_REQ,                                //!< SYSTEM CHIP VERSION UP START要求送信  TP=0xC0
	NP_UCOM_PACK_UPDATE_WRITE_STA,                                //!< SYSTEM CHIP VERSION UP START応答送信  TP=0xC4
	NP_UCOM_PACK_UPDATE_WRITE_DATA,                             //!< SYSTEM CHIP VERSION UP DATA送信  TP=0xC5
	NP_UCOM_PACK_UPDATE_WRITE_VA_DATA,                          //!< VA VERSION UP DATA送信  TP=0xC5
	NP_UCOM_PACK_UPDATE_CHECK_STA,                               //!< SYSTEM CHIP VERSION UP CHECK-SUM 送信  TP=0xC7
	NP_UCOM_PACK_UPDATE_CHECK_VA_STA,                            //!< VA VERSION UP CHECK SUM 送信  TP=0xC7
	NP_UCOM_PACK_UPDATE_RETRY_RES,
	NP_UCOM_PACK_UPDATE_ERASE_REQ,                               //!< VA ERASE SECTION AND RESET DECRYPTION CYPHER REQUEST TP=0xCB
	NP_UCOM_PACK_UPDATE_RESET_PRO,                               //!< VA Reset Processor TP=0xCD
	NP_UCOM_PACK_UPDATE_RESTART_REQ,                             //!< VA Update Restart TP=0xCA
	NP_UCOM_PACK_BITDATA_NOTIFY,                                                     //!<BITDATA 送信   TP=0xD0
	NP_UCOM_PACK_BEEP1_REQ_BITDATA_NOTIFY,                                 //!<BITDATA 送信   TP=0xD0
	NP_UCOM_PACK_BEEP2_REQ_BITDATA_NOTIFY,                                 //!<BITDATA 送信   TP=0xD0
	NP_UCOM_PACK_SXI_PW_PULL_UP_REQ_BITDATA_NOTIFY,              //!<BITDATA 送信   TP=0xD0
	NP_UCOM_PACK_SXI_PW_PULL_DOWN_REQ_BITDATA_NOTIFY,       //!<BITDATA 送信   TP=0xD0
	NP_UCOM_PACK_SXI_PWREN_PULL_UP_REQ_BITDATA_NOTIFY,        //!<BITDATA 送信   TP=0xD0
	NP_UCOM_PACK_SXI_PWREN_PULL_DOWN_REQ_BITDATA_NOTIFY, //!<BITDATA 送信   TP=0xD0
	NP_UCOM_PACK_BITDATA_NOTIFY_REQ,                            //!<BITDATA要求送信   TP=0xD1
	NP_UCOM_PACK_UCOM_SRC_NOTIFY_REQ,                         //!<SYSTEM SOURCE STATUS要求送信   TP=0xD3
	NP_UCOM_PACK_TESTCMD_SEND,
	NP_UCOM_PACK_RESET_REQ,                                         //!< POWER RESET要求送信   TP=0xE5
	NP_UCOM_PACK_SETUP_DATA,                                     //!< SETUP MENU BACKUP DATA送信   TP=0xE2
	NP_UCOM_PACK_TESTMODE_NOTIFY,                           //!< TESTMODE STATUS送信  TP=0xE5
	NP_UCOM_PACK_SET_DRM,                                           //!< DRM INFO SET送信  TP=0x45
	NP_UCOM_PACK_DRM_INFO_RES,                                 //!< DRM INFORMATION送信  TP=0x45
	NP_UCOM_PACK_TAGGING_RES,                                   //!< TAGGING START 応答送信  TP=0x4D
	NP_UCOM_PACK_TAGGING_DATA1_RES,                     //!< TAGGING DATA1 応答送信  TP=0x4D
	NP_UCOM_PACK_TAGGING_DATA2_RES,                       //!< TAGGING DATA2 応答送信  TP=0x4D
	NP_UCOM_PACK_VAINFO_CURRSRC,                        //!< VA INFO 送信  TP=0x4F,D0=C0
	NP_UCOM_PACK_VAINFO_ICON,                           //!< VA INFO 送信  TP=0x4F,D0=C1
	NP_UCOM_PACK_VAINFO_TUNER_STATUS,                   //!< VA INFO 送信  TP=0x4F,D0=C2
	NP_UCOM_PACK_VAINFO_MEDIA_STATUS,                   //!< VA INFO 送信  TP=0x4F,D0=C3
	NP_UCOM_PACK_VAINFO_PHONE_STATUS,                   //!< VA INFO 送信  TP=0x4F,D0=C4
	NP_UCOM_PACK_VAINFO_TEXT_DATA,                      //!< VA INFO 送信  TP=0x4F,D0=C5
	NP_UCOM_PACK_VAINFO_NAVI_COMMAND,                   //!< VA INFO 送信  TP=0x4F,D0=C6
	NP_UCOM_PACK_VAINFO_STREETNAME,                     //!< VA INFO 送信  TP=0x4F,D0=C7
	NP_UCOM_PACK_VAINFO_DISTANCE,                       //!< VA INFO 送信  TP=0x4F,D0=C8
	NP_UCOM_PACK_VAINFO_ARRIVALTIME,                    //!< VA INFO 送信  TP=0x4F,D0=C9
	NP_UCOM_PACK_VAINFO_REQTIME,                        //!< VA INFO 送信  TP=0x4F,D0=CA
	NP_UCOM_PACK_VAINFO_COMPASSDIR,                     //!< VA INFO 送信  TP=0x4F,D0=CB
	NP_UCOM_PACK_VAINFO_INTERVALTIME,                   //!< VA INFO 送信  TP=0x4F,D0=CD
	NP_UCOM_PACK_SLAVE_TESTMODE_REQ,                                         //!< SLAVE TESTMODE ENTER RESET要求送信   TP=0xE6
	NP_UCOM_PACK_SOURCELIST_NOTIFY,                                         //!< SOURCE LIST送信   TP=0xE7
	NP_UCOM_PACK_VOLUME_MODIFY,                                         //!< VOLUME MODIFY送信   TP=0xE9
	NP_UCOM_PACK_VERIFY_STATUS,                                         //!< VERIFY STATUS 送信   TP=0xEA
	NP_UCOM_PACK_SOC_VERSIONUP_NOTIFY,                                  //!< TP=0xC0 0x01
	NP_UCOM_PACK_VA_VERSIONUP_NOTIFY,                                   //!< TP=0xC0 0x05
	NP_UCOM_PACK_DVD_ONOFF_CMD,                                         //!< DVD ON OFF COMMAND 送信   TP=0xEB
	
	NP_UCOM_PACK_TPEC,                                  //!< TP=0xEC, key table receive

	// パックＩＤ名を追加すること
	NP_UCOM_PACK_ID_MAX				//!< パックＩＤ数
};

/*===========================================================================*/
/**
    アンパックＩＤ定義   For NPuComRxServer,NPuComPacker
*/
/*===========================================================================*/
enum NP_UCOM_UNPACK_ID
{
	NP_UCOM_UNPACK_CONECHK1_RCV          = 0x00, // 初期通信 TP=0x01
	NP_UCOM_UNPACK_CONECHK2_RCV,                 // 初期通信 TP=0x04
	NP_UCOM_UNPACK_SRC_CHG,                      // SOURCE CHANGE COMMAND TP=0x40
	NP_UCOM_UNPACK_SRC_STATUS_REQ,               // SOC SOURCE STATUS要求 TP=0x48
	NP_UCOM_UNPACK_DISP_DATA_NAVI_FRONT,         // 表示データ  TP=0x61
	NP_UCOM_UNPACK_UPDATE_START_RES,             // SYSTEM CHIP VERSION UP START応答        TP=0xC1
	NP_UCOM_UNPACK_UPDATE_ERASE_END,             // SYSTEM CHIP VERSION UP FLASH-ERASED応答 TP=0xC3
	NP_UCOM_UNPACK_UPDATE_WRITE_END,             // SYSTEM CHIP VERSION UP DATA WRITE応答   TP=0xC6
	NP_UCOM_UNPACK_UPDATE_CHECK_END,             // SYSTEM CHIP VERSION UP CHECK-SUM 応答   TP=0xC8
	NP_UCOM_UNPACK_UPDATE_ERASE_RES,             // VA VERSION UP Erase Request ACK        TP=0xCC
	NP_UCOM_UNPACK_UPDATE_VAUPDATE_RES,          // VA VERSION UP RESULT TP=0xCE
	NP_UCOM_UNPACK_BITDATA_NOTIFY_REQ,           // BITDATA要求  TP=0xD1   
	NP_UCOM_UNPACK_UCOM_SRC_NOTIFY,              // SYSTEM SOURCE STATUS TP=0xD2
	NP_UCOM_UNPACK_VERSION_RES,                  // SYSTEM VERSION INFORMATION
	NP_UCOM_UNPACK_ACCOFF_NOTIFY,                // ACC OFF   TP=0xF0
	NP_UCOM_UNPACK_UCOM_IMAGE_NOTIFY,            // IMAGE SETTING STATUS    TP=0xD4
//	NP_UCOM_UNPACK_BACK_UP_DATA_NOTIFY,          // SETUP MENU BACKUP DATA  TP=0xE3	//lijian, 15mid -> TPE3 = NP_UCOM_UNPACK_DTV_CMD_RESULT
	NP_UCOM_UNPACK_DRM_CMD,                      // DRM COMMAND             TP=0x44
	NP_UCOM_UNPACK_TAGGING_DATA,                 // TAGGING DATA            TP=0x4C

	NP_UCOM_UNPACK_DTV_LIST_INFOR,               // SYS->R2R, DTV ListInfor,TP=0x4A
	NP_UCOM_UNPACK_DTV_CMD_RESULT,               // SYS->R2R, DTV ListInfor,TP=0xE3
	NP_UCOM_UNPACK_POWER_ON_OFF,                 // SYS->R2R, POWER ON OFF,TP=0xE4

    NP_UCOM_UNPACK_TPED,                         // SYS->R2R, TP=0xED, DTV Status, d0,d1
	NP_UCOM_UNPACK_TPD3,                         // SYS->R2R, TP=0xD3, Source Changed Finished Notify
	NP_UCOM_UNPACK_TPD5,                         // SYS->R2R, TP=0xD5, display back camera notify
	NP_UCOM_UNPACK_INTERRUPT_SCREEN,             // SYS->R2R, TP=0xD6, interrupt screens
	NP_UCOM_UNPACK_PARK_PILOT_SCREEN,            // SYS->R2R, TP=0xD7, Park Pilot screen
	NP_UCOM_UNPACK_STOP_NAVIGUIDE,               // SYS->R2R, TP=0xD8, Stop Navigation Guide
	NP_UCOM_UNPACK_VA50,                         // SYS->R2R, TP=0xD9, VA Command 0x50

	// パックＩＤ名を追加すること
	NP_UCOM_UNPACK_ID_MAX		                 		//!< アンパックＩＤ数
};


/*===========================================================================*/
/**
    パック用テーブル構造体
*/
/*===========================================================================*/
struct NPuComPackTable
{
	DWORD	dwSize;	//!< サイズ
	BYTE	byTP;	//!< TP
};

/*===========================================================================*/
/**
    アンパック用テーブル構造体
*/
/*===========================================================================*/
struct NPuComUnPackTable
{
	DWORD	dwId;	//!< ID
	BYTE	byTP;	//!< TP
};

//#define NP_UCOM_OPEN_FILE_BYSELF

/*==========================================================================*/
/*                       USE  BY  TP=0xE5                                 */
/*==========================================================================*/
#define D_UCOM_RESET_FOR_TESTMODE_STOP   (0x00)
#define D_UCOM_RESET_FOR_TESTMODE_START   (0x01)

/*==========================================================================*/
/*                       USE  BY  TP=0xC0                                 */
/*==========================================================================*/
#define D_VERSION_UP_ID_DISP          (0x01)
#define D_VERSION_UP_ID_DISP_RECOVERY (0x02)
#define D_VERSION_UP_ID_SYSTEM        (0x03)
#define D_VERSION_UP_ID_WWR           (0x04)
#define D_VERSION_UP_ID_VA            (0x05)

#define D_VERSION_UP_OK   (0x00)
#define D_VERSION_UP_NG   (0xFF)

#define D_KEY_RELEASE			( 0x00 )					/* タッチ状態：離し					*/

#define D_KEYTYPE_HARDKEY_VALUE			( 0x00000000 )	
#define D_KEYTYPE_WIREDKEY_VALUE			( 0x00000100 )	
#define D_KEYTYPE_REMOTEKEY_VALUE		( 0x10000000 )	

enum D_KEYTYPE
{
	D_KEYTYPE_RELEASE						/** キー離され				*/
,	D_KEYTYPE_HARDKEY						/** 本体キー				*/
,	D_KEYTYPE_WIREDKEY						/** ワイヤードキー			*/
,	D_KEYTYPE_REMOKEY						/** リモコンキー			*/
};

typedef struct 
{
	unsigned char  rotary_code;
	unsigned char  hardkey_code;												/* 本体キーコード					*/
	unsigned char  wired_code;											/* ワイヤードキーコード				*/
	unsigned short remocon_code;											/* リモコンキーコード				*/
}ST_KEY_STATUS;

/*--------------------------------------------------------------------------------------------------*/
/* キーデータ格納用構造体																			*/
/*--------------------------------------------------------------------------------------------------*/
typedef struct
{
	ST_KEY_STATUS			key_sts;					/* ハードキー状態					*/
	unsigned short					key_type;					/* キー種別							*/
	unsigned short					key_code;					/* キーコード						*/
}ST_KEY_DATA;

union UN_RCV_BIT_DATA
{
	struct ST_RCV_BIT_DATA
	{
		unsigned char BD1_1;
		unsigned char BD1_2;
		unsigned char BD1_3;
		unsigned char BD1_4;
		unsigned char BD1_5;
		
		unsigned char rotary;    /* BD1_6 : bit0-7	: Rotary                 */
		unsigned char hardkey;   /* BD1_7 : bit0-7	: 本体キーコード          */
		unsigned char wiredkey;  /* BD1_8 : bit0-7	: ワイヤードキーコード    */
		unsigned char remo_1st;  /* BD1_9 : bit0-7	: リモコンコード1st       */
		unsigned char remo_2nd;  /* BD1_10: bit0-7	: リモコンコード2nd       */
	}bit;

	unsigned char	bit_array[NP_UCOM_SIZE_BD];
};


/*=============================================================================
     クラス宣言                                                              */

class NPuComBitData;
class NPuComPacker;
class NPuComDataUpdate;


/*===========================================================================*/
/**
    マイコンビットデータクラス
*/
/*===========================================================================*/
class NPuComBitData : public NObject
{
public:

	/*************************************************************************/
	/**
		マイコンビットデータクラスコンストラクタ
	*/
	/*************************************************************************/
	NPuComBitData();

	/*************************************************************************/
	/**
		マイコンビットデータクラスデストラクタ
	*/
	/*************************************************************************/
	virtual ~NPuComBitData();

	/*************************************************************************/
	/**
		初期化処理

		@param  なし

		@retval なし
	*/
	/*************************************************************************/
	VOID
	Initialize(VOID);

	/*************************************************************************/
	/**
		ビットデータ設定処理
	    
		@param dwNum	:	BD番号
		@param byData	:	指定番号の内容

		@return なし
	*/
	/*************************************************************************/
	VOID 
	SetBitData( DWORD dwNum, BYTE byData );

	/*************************************************************************/
	/**
		ビットデータ取得処理
	    
		@param dwNum	:	BD番号

		@return BYTE	:	指定番号のBD
	*/
	/*************************************************************************/
	BYTE 
	GetBitData( DWORD dwNum );

private:

	BYTE	*m_pbyBD;

	/**
	 * コピーコンストラクタ
	 *
	 * 利用禁止
	 *
	 * @param const NPuComBitData& src [IN] コピー元
	 *
	 * @return なし
	 */
	NPuComBitData( const NPuComBitData& src);

	/**
	 * コピー演算子
	 *
	 * 利用禁止
	 *
	 * @param const NPuComBitData& src [IN] コピー元
	 *
	 * @return const NPuComBitData& 自分自身への参照
	 */
	const NPuComBitData& operator=( const NPuComBitData& src);
};


/*===========================================================================*/
/**
    パッカークラス

	@author $Author: hustyy $
	@version $Revision: 5833 $
*/
/*===========================================================================*/
class NPuComPacker : public NObject
{
public:
	/*************************************************************************/
	/**
		パッカークラスのインスタンスの取得
	    
		@param なし

		@return UCOMハンドラプロキシのインスタンス
	*/
	/*************************************************************************/
	static NPuComPacker
	*Instance( VOID );

	/*************************************************************************/
	/**
		パッカークラスのインスタンスの破棄
	    
		@param なし

		@return なし
	*/
	/*************************************************************************/
	static VOID 
	Destroy( VOID );

	/*************************************************************************/
	/**
		パック処理
	    
		@param dwId		:	[IN]  パックＩＤ
		@param pBuff	:	[IN]  バッファ
		@param pdwSize	:	[IN]  バッファサイズ
		@param psFrame	:	[OUT] パケット

		@return なし
	*/
	/*************************************************************************/
	VOID 
	Pack( const DWORD dwId, LPCVOID pBuff, const DWORD dwSize, NPuComDataFrameUpdate *psFrame );

	/*************************************************************************/
	/**
		アンパック処理
	    
		@param psFrame		:	[IN]  パケット

		@return なし
	*/
	/*************************************************************************/
	VOID 
	UnPack( const NPuComDataFrame *psFrame );

	/*************************************************************************/
	/**
		受信用BD設定処理
	    
		@param dwNum	:	BD番号
		@param byData	:	指定番号の内容

		@return なし
	*/
	/*************************************************************************/
	VOID 
	SetRxBitData( DWORD dwNum, BYTE byData );

	/*************************************************************************/
	/**
		送信用BD設定処理
	    
		@param dwNum	:	BD番号
		@param byData	:	指定番号の内容

		@return なし
	*/
	/*************************************************************************/
	VOID 
	SetTxBitData( DWORD dwNum, BYTE byData );

	/*************************************************************************/
	/**
		受信用BD取得処理
	    
		@param dwNum	:	BD番号

		@return BYTE	:	指定番号のBD
	*/
	/*************************************************************************/
	BYTE 
	GetRxBitData( DWORD dwNum );

	/*************************************************************************/
	/**
		送信用BD取得処理
	    
		@param dwNum	:	BD番号

		@return BYTE	:	指定番号のBD
	*/
	/*************************************************************************/
	BYTE 
	GetTxBitData( DWORD dwNum );

	/*************************************************************************/
	/**
		送信ビットデータ設定処理
	    
		@param dwId		:	ビットＩＤ
		@param bStatus	:	ビット状態

		@retval NP_TRUE		: ビット変化あり
		@retval	NP_FALSE	: ビット変化なし
	*/
	/*************************************************************************/
	NP_BOOL 
	ChangeTxBitData( const DWORD dwId, const NP_BOOL bStatus );

	/*************************************************************************/
	/**
		送信ビットデータ設定処理
	    
		@param dwId		:	ビットＩＤ
		@param bStatus	:	ビット状態

		@retval NP_TRUE		: ビット変化あり
		@retval	NP_FALSE	: ビット変化なし
	*/
	/*************************************************************************/
	NP_BOOL 
	ChangeTxBitData( const DWORD dwId, const BYTE bStatus );

	/*************************************************************************/
	/**
		送信ビットデータ設定処理
	    
		@param pbyChgBD	:	ビット状態

		@retval NP_TRUE		: ビット変化あり
		@retval	NP_FALSE	: ビット変化なし
	*/
	/*************************************************************************/
	NP_BOOL 
	ChangeTxBitData(PBYTE pbyChgBD);
	/*************************************************************************/
	/**
		受信ビットデータ設定処理
	    
	   @param pbyChgBD		:[OUT]  変化フラグ

		@retval NP_TRUE		: ビット変化あり
		@retval	NP_FALSE	: ビット変化なし
	*/
	/*************************************************************************/
	NP_BOOL 
	ChangeRxBitData( PBYTE pbyChgBD );

	/*************************************************************************/
	/**
		key event type judge処理
	    
		@param keystatus		:[IN]  KEY-BORAD STATUS
		@param key_code		:[OUT]  KEYEVENT--KEYCODE
		@param key_type		:[OUT]  KEYEVENT--KEYTYPE

		@return なし
	*/
	/*************************************************************************/
	void
	CheckKeyLevel(ST_KEY_DATA *keystatus,unsigned short *key_code,unsigned short *key_type);

	/*************************************************************************/
	/**
		ビットデータ確認処理
	    
		@param psFrame		:	[IN]  パケット

		@retval NP_TRUE		: ビット変化あり
		@retval	NP_FALSE	: ビット変化なし
	*/
	/*************************************************************************/
	NP_BOOL 
	CheckReceiveBD( const NPuComDataFrame *psFrame );

	NP_BOOL
	GetReceiveBD( NPuComRcvBD *psBD );

	/*************************************************************************/
	/**
		初期通信結果取得
	    
		@param なし

		@retval なし
	*/
	/*************************************************************************/
	VOID 
	GetConeChkStatus( NPConeChkStatus *pcStatus );


	/*************************************************************************/
	/**
		Reset Tx/Rx bitdata when resume
	    
		@param なし

		@retval なし
	*/
	/*************************************************************************/
	VOID 
	ResetRxTxBitdata( VOID );

	VOID 
	EnableDetectLearningKey(BOOL bEnable);

	BOOL 
	IsLearningKeyPush();
	
private:

	static NSyncObject	s_cSync;		//!< 同期オブジェクト
	static NPuComPacker	*s_pcInstance;	//!< プロセスに唯一のインスタンス

	NPuComBitData	m_cRcvBD;	//!< 受信用BD
	NPuComBitData	m_cSndBD;	//!< 送信用BD

	NPConeChkStatus	m_sConeChk;	//!< 初期通信情報

	NP_BOOL	m_bVerRes;
	NP_BOOL	m_blBitNotifyStarted;	//!< 最初のBitData受信処理判定フラグ

	NSyncObject s_cSyncBD;
	NSyncObject s_cSyncRxBD;

	BOOL m_bDetectWiredKeyPress;
	BOOL m_bWiredKeyPress;
	NSyncObject s_cSyncKey;
	
	unsigned int m_isTestModeNotify;	// LH=isNotified, LL=TP61.70.04.[D9];
	
	/*************************************************************************/
	/**
		コンストラクタ

		@param	なし

		@retval	なし
	*/
	/*************************************************************************/
	NPuComPacker();

	/*************************************************************************/
	/**
		デストラクタ

		@param	なし

		@retval	なし
	*/
	/*************************************************************************/
	virtual ~NPuComPacker();

	/*************************************************************************/
	/**
		初期通信送信時処理
	    
		@param なし

		@retval なし
	*/
	/*************************************************************************/
	VOID 
	OnSendConeChkReq( VOID );

	/*************************************************************************/
	/**
		初期通信受信時処理
	    
		@param なし

		@retval なし
	*/
	/*************************************************************************/
	VOID 
	OnReceiveConeChkRes( VOID );

	/*************************************************************************/
	/**
		仕向確認処理
	    
		@param なし

		@retval なし
	*/
	/*************************************************************************/
	VOID 
	CheckModel( VOID );

	/**
	 * コピーコンストラクタ
	 *
	 * 利用禁止
	 *
	 * @param const NPuComPacker& src [IN] コピー元
	 *
	 * @return なし
	 */
	NPuComPacker( const NPuComPacker& src);

	/**
	 * コピー演算子
	 *
	 * 利用禁止
	 *
	 * @param const NPuComPacker& src [IN] コピー元
	 *
	 * @return const NPuComPacker& 自分自身への参照
	 */
	const NPuComPacker& operator=( const NPuComPacker& src);
};


/*===========================================================================*/
/**
    マイコン書き換えクラス

	@author $Author: hustyy $
	@version $Revision: 5833 $
*/
/*===========================================================================*/
class NPuComDataUpdate : public NObject
{
public:
	/*************************************************************************/
	/**
		マイコン書き換えクラスのインスタンスの取得
	    
		@param なし

		@return UCOMハンドラプロキシのインスタンス
	*/
	/*************************************************************************/
	static NPuComDataUpdate
	*Instance( VOID );

	/*************************************************************************/
	/**
		マイコン書き換えクラスのインスタンスの破棄
	    
		@param なし

		@return なし
	*/
	/*************************************************************************/
	static VOID 
	Destroy( VOID );

	/*************************************************************************/
	/**
		マイコンアップデート領域サイズ取得関数

		@param	なし

		@retval	DWORD	: 領域サイズ
	*/
	/*************************************************************************/
	DWORD
	GetUpdateAllSize(VOID)
	{
		return m_dwUpdateFileSize;
	};

	/*************************************************************************/
	/**
		マイコンアップデート書込み済みサイズ取得関数

		@param	なし

		@retval	DWORD	: 書込み済みサイズ
	*/
	/*************************************************************************/
	DWORD
	GetUpdateOverSize(VOID)
	{
		return m_dwUpdateOverSize;
	};

	/*************************************************************************/
	/**
		マイコンアップデート残りサイズ取得関数

		@param	なし

		@retval	DWORD	: 残りサイズ
	*/
	/*************************************************************************/
	DWORD
	GetUpdateRestSize(VOID)
	{
		if (m_dwUpdateFileSize>m_dwUpdateOverSize)
		{
			return m_dwUpdateFileSize - m_dwUpdateOverSize;
		}
		else
		{
			return 0;
		}
	};

	/*************************************************************************/
	/**
		マイコンアップデート準備関数

		@param	なし

		@retval	TRUE	: 成功
		@retval	FALSE	: 失敗
	*/
	/*************************************************************************/
	VOID
	SetUpdatePath(LPCVOID pData, DWORD uUpdateTypeID );
	
	DWORD
	GetUpdateTypeID( VOID );

	/*************************************************************************/
	/**
		マイコン書き換え開始処理
	    
		@param なし

		@retval NP_TRUE		: 成功
		@retval	NP_FALSE	: 失敗
	*/
	/*************************************************************************/
	NP_BOOL 
	StartUpdateReq( VOID );

	/*************************************************************************/
	/**
		マイコンアップデート準備関数

		@param	なし

		@retval	TRUE	: 成功
		@retval	FALSE	: 失敗
	*/
	/*************************************************************************/
	NP_BOOL
	ReadyUpdate( VOID );
	NP_BOOL
	ReadyUpdate_ForWWR( VOID );

	/*************************************************************************/
	/**
		マイコンアップデート終了関数

		@param	なし

		@retval	なし
	*/
	/*************************************************************************/
	VOID
	EndUpdate(NP_BOOL bResult);

	/*************************************************************************/
	/**
		マイコン書換え開始要求送信処理関数

		@param psFrame		:	[OUT] パケット

		@retval	NP_TRUE		: 成功
		@retval	NP_FALSE	: 失敗
	*/
	/*************************************************************************/
	NP_BOOL
	SendUpdateStartReq(NPuComDataFrameUpdate *psFrame);

	/*************************************************************************/
	/**
		マイコン書換え書込開始要求送信処理関数

		@param psFrame		:	[OUT] パケット

		@retval	NP_TRUE	: 成功
		@retval	NP_FALSE	: 失敗
	*/
	/*************************************************************************/
	NP_BOOL
	SendUpdateWriteSta(NPuComDataFrameUpdate *psFrame);

	/*************************************************************************/
	/**
		マイコン書換え確認開始要求送信処理関数

		@param psFrame		:	[OUT] パケット

		@retval	NP_TRUE	: 成功
		@retval	NP_FALSE	: 失敗
	*/
	/*************************************************************************/
	NP_BOOL
	SendUpdateCheckSta(NPuComDataFrameUpdate *psFrame);

	/*************************************************************************/
	/**
		マイコン書き換え再送応答送信関数

		@param psFrame		:	[OUT] パケット

		@retval	NP_TRUE	: 成功
		@retval	NP_FALSE	: 失敗
	*/
	/*************************************************************************/
	NP_BOOL
	SendUpdateRetryRes(NPuComDataFrameUpdate *psFrame, BYTE byTP);

private:

	static NSyncObject		s_cSync;		//!< 同期オブジェクト
	static NPuComDataUpdate	*s_pcInstance;	//!< プロセスに唯一のインスタンス

	NPuComHandler			*m_pcMain;			//!< ハンドラメインスレッドポインタ
	NPuComMemoryUpdate		*m_pcMemUpdate;		//!< マイコン書換え用共有メモリ
	NPuComDataFrameUpdate	m_sSendData;		//!< 最新の送信データ
	


	// マイコン書換え用
	NPuComPrgFileHeader		m_sPrgHead;									//!< マイコン書き換え用プログラムファイル情報
	NPuComPrgFileTail_WWR	m_sWwrTail;
	
	DWORD					m_dwUpdateAddress;							//!< マイコン書き換え用書込先アドレス
	DWORD					m_dwUpdateFileSize;							//!< マイコン書き換え用ファイルサイズ
	DWORD					m_dwUpdateOverSize;							//!< マイコン書き換え用書込済サイズ
	PBYTE					m_pbyUpdateBinary;							//!< マイコン書き換え用データポインタ
	WORD					m_byUpdateCheckSum;			//!< マイコン書き換え用チェックサム格納

	TCHAR					m_szPrgFilePath[NP_UCOM_SIZE_PATH_MAX];		//!< マイコン書き換え用バージョンファイルパス
	DWORD					m_UpdateTypeID;		//!< 書換uCom IC'ID : D_VERSION_UP_ID_DISP
												//!                   D_VERSION_UP_ID_DISP_RECOVERY
												//!                   D_VERSION_UP_ID_SYSTEM
												//!                   D_VERSION_UP_ID_WWR

	/*************************************************************************/
	/**
		コンストラクタ

		@param	なし

		@retval	なし
	*/
	/*************************************************************************/
	NPuComDataUpdate();

	/*************************************************************************/
	/**
		デストラクタ

		@param	なし

		@retval	なし
	*/
	/*************************************************************************/
	virtual ~NPuComDataUpdate();

	/*************************************************************************/
	/**
		マイコン書換えデータ保存処理関数

		@param psFrame		:	[IN] パケット

		@retval	なし
	*/
	/*************************************************************************/
	VOID
	SetSendData( const NPuComDataFrameUpdate *psFrame);

	/*************************************************************************/
	/**
		マイコン書換えデータ読出処理関数

		@param psFrame		:	[OUT] パケット

		@retval	なし
	*/
	/*************************************************************************/
	VOID
	GetSendData(NPuComDataFrameUpdate *psFrame);

	/*************************************************************************/
	/**
		マイコン書換えTP読出処理関数

		@param なし

		@retval	BYTE	: TP
	*/
	/*************************************************************************/
	BYTE
	GetSendDataTP(VOID);

	/**
	 * コピーコンストラクタ
	 *
	 * 利用禁止
	 *
	 * @param const NPuComDataUpdate& src [IN] コピー元
	 *
	 * @return なし
	 */
	NPuComDataUpdate( const NPuComDataUpdate& src);

	/**
	 * コピー演算子
	 *
	 * 利用禁止
	 *
	 * @param const NPuComDataUpdate&src [IN] コピー元
	 *
	 * @return const NPuComDataUpdate& 自分自身への参照
	 */
	const NPuComDataUpdate& operator=( const NPuComDataUpdate&src);
};


/*===========================================================================*/
/*
 * 【更新履歴】
 * $Log: NPuComPacker.h,v $
 * Revision 1.1  2008/08/08 06:20:03  tomoyuki
 * 新規登録
 *
 */
/*===========================================================================*/
#endif /* CXX_NPUCOMPACKER_H */
//================================================================= End of File
