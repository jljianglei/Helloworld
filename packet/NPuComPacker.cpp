/*

* (c) PIONEER CORPORATION 2014

* 25-1 Nishi-machi Yamada Kawagoe-shi Saitama-ken 350-8555 Japan

* All Rights Reserved.

*/


/*===========================================================================*/
/** @file
    パッカークラス実装ファイル

    @attention C++ 専用です C では使用出来ません
*/
/*===========================================================================*/

/*=============================================================================
   インクルードファイルの読み込み                                            */

#include "NPLOGID_UCOM.h"
#include "cutils/properties.h"

#include "NPuComPacker.h"
#include "NPuComHandler.h"
#include "NPuComTimer.h"
#include "NPSysInfoSet.h"
#include "NPSysInfoGet.h"
#include "NPFile.h"
#include "PFileServiceProxy.h"
#include "SetValueStore.h"
#include "NPKernel.h"
#include "NPWarp.h"

#define NP_UCOM_READ_SIZE_MAX (100*1024)

//#define NP_PROPERTY_BOFF_STATUS "rw.BOffResetFlg"
#define NP_PROPERTY_BOFF_STATUS "sys.BOffResetFlg"
#define NP_PROPERTY_VERIFY_STATUS "sys.VerifyFlg"

/*=============================================================================
	グローバル宣言                                                           */

// インスタンス
NPuComPacker*		NPuComPacker::s_pcInstance		= NULL;
NPuComDataUpdate*	NPuComDataUpdate::s_pcInstance	= NULL;
// 同期オブジェクト
NSyncObject	NPuComPacker::s_cSync;
NSyncObject	NPuComDataUpdate::s_cSync;

/*=============================================================================
	定数定義                                                                 */
// 受信用ビットデータ初期値
const BYTE NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_SIZE_BD] = 
{
	NP_UCOM_NODATA,
	NP_UCOM_NODATA,
	NP_UCOM_NODATA,
	NP_UCOM_NODATA,
	NP_UCOM_NODATA,
	NP_UCOM_NODATA,
	NP_UCOM_NODATA,
	NP_UCOM_NODATA,
	NP_UCOM_NODATA,
	NP_UCOM_NODATA
};

// 送信用ビットデータ初期値
const BYTE NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_SIZE_BD] = 
{
	NP_UCOM_NODATA,
	NP_UCOM_NODATA,
	NP_UCOM_NODATA,
	NP_UCOM_NODATA,
	NP_UCOM_NODATA,
	NP_UCOM_NODATA,
	NP_UCOM_NODATA,
	NP_UCOM_NODATA,
	NP_UCOM_NODATA,
	NP_UCOM_NODATA
};

// パック用テーブル
const NPuComPackTable NP_UCOM_PACK_TABLE[NP_UCOM_PACK_ID_MAX] =
{
	// NP_UCOM_PACK_CONECHK_SEND
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_CONECHK_SEND					// TP
	},
	// NP_UCOM_PACK_CONECHK1_SEND
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_CONECHK1_SEND					// TP
	},
	// NP_UCOM_PACK_CONECHK2_SEND
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_CONECHK2_SEND					// TP
	},
	//NP_UCOM_PACK_APK_ON_NOTIFY
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_APK_ON_NOTIFY					// TP
	},
	//NP_UCOM_PACK_POWER_ON_OFF_TPE3
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 2,	// サイズ
		NP_UCOM_TP_CMD_POWER_ON_OFF			// TP
	},
	//NP_UCOM_PACK_DTV_TPE4_NOTIFY
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 2,	// サイズ
		NP_UCOM_TP_DTV_TPE4_NOTIFY			// TP
	},
	//NP_UCOM_PACK_SRC_CHANGE_RES
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 5,	// サイズ
		NP_UCOM_TP_SRC_CHG_RES			// TP
	},
	//NP_UCOM_PACK_SRC_CHANGE_REQ
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 3,	// サイズ
		NP_UCOM_TP_SRC_CHG_RES			// TP
	},
	//NP_UCOM_PACK_SRC_STATUS_NOTIFY
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 18,	// サイズ
		NP_UCOM_TP_SRC_STATUS_SEND			// TP
	},
	// NP_UCOM_PACK_DIRECTCMD_SEND
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 2,	// サイズ
		NP_UCOM_TP_DIRECTCMD_SEND				// TP
	},
	// NP_UCOM_PACK_SRC_DIRECTCMD_SEND
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_SRC_DIRECTCMD_SEND			// TP
	},
	// NP_UCOM_PACK_DISP_DATA_REQ_SEND
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 3,	// サイズ
		NP_UCOM_TP_DISP_DATA_REQ_SEND			// TP
	},
	// NP_UCOM_PACK_UPDATE_START_REQ
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_UPDATE_START_REQ				// TP
	},
	// NP_UCOM_PACK_UPDATE_WRITE_STA
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_UPDATE_WRITE_STA					// TP
	},
	//NP_UCOM_PACK_UPDATE_WRITE_DATA
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 258,	// サイズ
		NP_UCOM_TP_UPDATE_WRITE_DATA    // TP
	},
	//NP_UCOM_PACK_UPDATE_WRITE_VA_DATA
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 71,	// サイズ
		NP_UCOM_TP_UPDATE_WRITE_DATA    // TP
	},
	// NP_UCOM_PACK_UPDATE_CHECK_STA
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 2,	// サイズ
		NP_UCOM_TP_UPDATE_CHECK_STA				// TP
	},
	// NP_UCOM_PACK_UPDATE_CHECK_VA_STA
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_UPDATE_CHECK_STA				// TP
	},
	// NP_UCOM_PACK_UPDATE_RETRY_RES
	{
		0,    //NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_UPDATE_WRITE_DATA				// TP
	},
	// NP_UCOM_PACK_UPDATE_ERASE_REQ
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_UPDATE_ERASE_REQ				// TP
	},
	// NP_UCOM_PACK_UPDATE_RESET_PRO
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_UPDATE_RESET_PRO				// TP
	},
	// NP_UCOM_PACK_UPDATE_RESTART_REQ
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_UPDATE_RESTART_REQ			// TP
	},
	// NP_UCOM_PACK_BITDATA_NOTIFY
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_BITDATA_NOTIFY				// TP
	},
	//NP_UCOM_PACK_BEEP1_REQ_BITDATA_NOTIFY
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_BITDATA_NOTIFY				// TP
	},
	//NP_UCOM_PACK_BEEP2_REQ_BITDATA_NOTIFY
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_BITDATA_NOTIFY				// TP
	},
	//NP_UCOM_PACK_SXI_PW_PULL_UP_REQ_BITDATA_NOTIFY
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_BITDATA_NOTIFY				// TP
	},
	//NP_UCOM_PACK_SXI_PW_PULL_DOWN_REQ_BITDATA_NOTIFY
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_BITDATA_NOTIFY				// TP
	},
	//NP_UCOM_PACK_SXI_PWREN_PULL_UP_REQ_BITDATA_NOTIFY
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_BITDATA_NOTIFY				// TP
	},
	//NP_UCOM_PACK_SXI_PWREN_PULL_DOWN_REQ_BITDATA_NOTIFY
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_BITDATA_NOTIFY				// TP
	},
	// NP_UCOM_PACK_BITDATA_NOTIFY_REQ
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_BITDATA_NOTIFY_REQ			// TP
	},
	// NP_UCOM_PACK_UCOM_SRC_NOTIFY_REQ
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_UCOM_SRC_NOTIFY_REQ			// TP
	},
	// NP_UCOM_PACK_TESTCMD_SEND
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD,		// サイズ
		0										// TP
	},
	// NP_UCOM_PACK_RESET_REQ
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 3,	// サイズ
		NP_UCOM_TP_RESET_REQ					// TP
	},
	//NP_UCOM_PACK_SETUP_DATA
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + NP_UCOM_SIZE_SETUPDATA_MAX,	// サイズ
		NP_UCOM_TP_SETUP_DATA					// TP
	},
	//NP_UCOM_PACK_TESTMODE_NOTIFY
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + NP_UCOM_SIZE_TESTMODE_STATUS_MAX,	// サイズ
		NP_UCOM_TP_RESET_REQ					// TP
	},
	//NP_UCOM_PACK_SET_DRM
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + NP_UCOM_SIZE_DRM_INFO_MAX,	// サイズ
		NP_UCOM_TP_SET_DRM					// TP
	},
	//NP_UCOM_PACK_DRM_INFO_RES
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + NP_UCOM_SIZE_DRM_INFO_MAX+ 2,	// サイズ
		NP_UCOM_TP_SET_DRM					// TP
	},
	//NP_UCOM_PACK_TAGGING_RES
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 2,	// サイズ
		NP_UCOM_TP_TAGGING_DATA_RES                  // TP
	},
	//NP_UCOM_PACK_TAGGING_DATA1_RES
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 4,	// サイズ
		NP_UCOM_TP_TAGGING_DATA_RES                  // TP
	},
	//NP_UCOM_PACK_TAGGING_DATA2_RES
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 2,	// サイズ
		NP_UCOM_TP_TAGGING_DATA_RES                  // TP
	},
	//NP_UCOM_PACK_VAINFO_CURRSRC
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + NP_UCOM_SIZE_SOURCENAME_MAX + 3,	// サイズ
		NP_UCOM_TP_VAINFO_SEND				// TP
	},
	//NP_UCOM_PACK_VAINFO_ICON
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 2,	// サイズ
		NP_UCOM_TP_VAINFO_SEND				// TP
	},
	//NP_UCOM_PACK_VAINFO_TUNER_STATUS
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 5,	// サイズ
		NP_UCOM_TP_VAINFO_SEND				// TP
	},
	//NP_UCOM_PACK_VAINFO_MEDIA_STATUS
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 7,	// サイズ
		NP_UCOM_TP_VAINFO_SEND				// TP
	},
	//NP_UCOM_PACK_VAINFO_PHONE_STATUS
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + NP_UCOM_SIZE_VAINFO_PHONESTATUS_MAX + 1,	// サイズ
		NP_UCOM_TP_VAINFO_SEND				// TP
	},
	//NP_UCOM_PACK_VAINFO_TEXT_DATA
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + NP_UCOM_SIZE_VAINFOTEXT_MAX + 5,	// サイズ
		NP_UCOM_TP_VAINFO_SEND				// TP
	},
	//NP_UCOM_PACK_VAINFO_NAVI_COMMAND
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + NP_UCOM_SIZE_DISTANCESTR_MAX + 5,	// サイズ
		NP_UCOM_TP_VAINFO_SEND				// TP
	},
	//NP_UCOM_PACK_VAINFO_STREETNAME
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + NP_UCOM_SIZE_STREETNAME_MAX + 4,	// サイズ
		NP_UCOM_TP_VAINFO_SEND				// TP
	},
	//NP_UCOM_PACK_VAINFO_DISTANCE
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + NP_UCOM_SIZE_DISTANCE_MAX + 2,	// サイズ
		NP_UCOM_TP_VAINFO_SEND				// TP
	},
	//NP_UCOM_PACK_VAINFO_ARRIVALTIME
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + NP_UCOM_SIZE_TIME_MAX + 2,	// サイズ
		NP_UCOM_TP_VAINFO_SEND				// TP
	},
	//NP_UCOM_PACK_VAINFO_REQTIME
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + NP_UCOM_SIZE_TIME_MAX + 2,	// サイズ
		NP_UCOM_TP_VAINFO_SEND				// TP
	},
	//NP_UCOM_PACK_VAINFO_COMPASSDIR
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 3,	// サイズ
		NP_UCOM_TP_VAINFO_SEND				// TP
	},
	//NP_UCOM_PACK_VAINFO_INTERVALTIME
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 3,	// サイズ
		NP_UCOM_TP_VAINFO_SEND				// TP
	},
	//NP_UCOM_PACK_SLAVE_TESTMODE_REQ
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_SLAVE_TESTMODE_REQ                  // TP
	},
	//NP_UCOM_PACK_SOURCELIST_NOTIFY
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + NP_UCOM_SIZE_SOURCELIST_MAX,	// サイズ
		NP_UCOM_TP_SOURCELIST_NOTIFY                  // TP
	},
	//NP_UCOM_PACK_VOLUME_MODIFY
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_VOLUME_MODIFY                  // TP
	},
	//NP_UCOM_PACK_VERIFY_STATUS
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_VERIFY_STATUS                  // TP
	},
	//NP_UCOM_PACK_SOC_VERSIONUP_NOTIFY
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_UPDATE_START_REQ                  // TP
	},
	//NP_UCOM_PACK_VA_VERSIONUP_NOTIFY
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_UPDATE_START_REQ                  // TP
	},
	//NP_UCOM_PACK_DVD_ONOFF_CMD
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1,	// サイズ
		NP_UCOM_TP_DVD_ON_OFF_CMD                  // TP
	},
    //NP_UCOM_PACK_TPEC
	{
		NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + NP_UCOM_SIZE_KEYTABLE_MAX,  // サイズ
		NP_UCOM_TP_EC                           // TP
	},
};

// アンパック用テーブル
const NPuComUnPackTable NP_UCOM_UNPACK_TABLE[NP_UCOM_UNPACK_ID_MAX] =
{
	// NP_UCOM_UNPACK_CONECHK1_RCV
	{
		NP_UCOM_UNPACK_CONECHK1_RCV,		// ID
		NP_UCOM_TP_CONECHK1_RCV	// TP
	},
	// NP_UCOM_UNPACK_CONECHK2_RCV
	{
		NP_UCOM_UNPACK_CONECHK2_RCV,		// ID
		NP_UCOM_TP_CONECHK2_RCV	// TP
	},
	//NP_UCOM_UNPACK_SRC_CHG
	{
		NP_UCOM_UNPACK_SRC_CHG,				// ID
		NP_UCOM_TP_SRC_CHG			// TP
	},
	//NP_UCOM_UNPACK_SRC_STATUS_REQ
	{
		NP_UCOM_UNPACK_SRC_STATUS_REQ,	// ID
		NP_UCOM_TP_SRC_STAUS_REQ    // TP
	},
	// NP_UCOM_UNPACK_DISP_DATA_NAVI_FRONT
	{
		NP_UCOM_UNPACK_DISP_DATA_NAVI_FRONT,	// ID
		NP_UCOM_TP_DISP_DATA_NAVI_FRONT			// TP
	},
	// NP_UCOM_UNPACK_UPDATE_START_RES
	{
		NP_UCOM_UNPACK_UPDATE_START_RES,	// ID
		NP_UCOM_TP_UPDATE_START_RES			// TP
	},
	// NP_UCOM_UNPACK_UPDATE_ERASE_END
	{
		NP_UCOM_UNPACK_UPDATE_ERASE_END,	// ID
		NP_UCOM_TP_UPDATE_ERASE_END			// TP
	},
	// NP_UCOM_UNPACK_UPDATE_WRITE_END
	{
		NP_UCOM_UNPACK_UPDATE_WRITE_END,	// ID
		NP_UCOM_TP_UPDATE_WRITE_END			// TP
	},
	// NP_UCOM_UNPACK_UPDATE_CHECK_END
	{
		NP_UCOM_UNPACK_UPDATE_CHECK_END,	// ID
		NP_UCOM_TP_UPDATE_CHECK_END			// TP
	},
	// NP_UCOM_UNPACK_UPDATE_ERASE_RES
	{
		NP_UCOM_UNPACK_UPDATE_ERASE_RES,	// ID
		NP_UCOM_TP_UPDATE_ERASE_RES			// TP
	},
	// NP_UCOM_UNPACK_UPDATE_VAUPDATE_RES
	{
		NP_UCOM_UNPACK_UPDATE_VAUPDATE_RES,	// ID
		NP_UCOM_TP_UPDATE_RESULT_RES		// TP
	},
	// NP_UCOM_UNPACK_BITDATA_NOTIFY_REQ
	{
		NP_UCOM_UNPACK_BITDATA_NOTIFY_REQ,		// ID
		NP_UCOM_TP_BITDATA_NOTIFY_REQ			// TP
	},
	// NP_UCOM_UNPACK_UCOM_SRC_NOTIFY
	{
		NP_UCOM_UNPACK_UCOM_SRC_NOTIFY,		// ID
		NP_UCOM_TP_UCOM_SRC_NOTIFY			// TP
	},
	// NP_UCOM_UNPACK_VERSION_RES
	{
		NP_UCOM_UNPACK_VERSION_RES,	// ID
		NP_UCOM_TP_VERSION_RES		// TP
	},
	// NP_UCOM_UNPACK_ACCOFF_NOTIFY
	{
		NP_UCOM_UNPACK_ACCOFF_NOTIFY,	// ID
		NP_UCOM_TP_ACCOFF_NOTIFY	// TP
	},
	//NP_UCOM_UNPACK_UCOM_IMAGE_NOTIFY
	{
		NP_UCOM_UNPACK_UCOM_IMAGE_NOTIFY,	// ID
		NP_UCOM_TP_UCOM_IMAGE_SETTING_NOTIFY// TP
	},
	//NP_UCOM_UNPACK_BACK_UP_DATA_NOTIFY
//	{
//		NP_UCOM_UNPACK_BACK_UP_DATA_NOTIFY,	// ID		//lijian, 15mid -> TPE3 = NP_UCOM_UNPACK_DTV_CMD_RESULT
//		NP_UCOM_TP_BACK_UP_DATA      // TP
//	},
	//NP_UCOM_UNPACK_DRM_CMD
	{
		NP_UCOM_UNPACK_DRM_CMD,	// ID
		NP_UCOM_TP_DRM_CMD      // TP
	},
	//NP_UCOM_UNPACK_TAGGING_DATA
	{
		NP_UCOM_UNPACK_TAGGING_DATA,	// ID
		NP_UCOM_TP_TAGGING_DATA      // TP
	},
	
	
	//NP_UCOM_UNPACK_DTV_LIST_INFOR
	{
		NP_UCOM_UNPACK_DTV_LIST_INFOR,	// ID
		NP_UCOM_TP_DTV_LIST_INFOR		// TP.0x4A
	},
	//NP_UCOM_UNPACK_DTV_CMD_RESULT
	{
		NP_UCOM_UNPACK_DTV_CMD_RESULT,	// ID
		NP_UCOM_TP_DTV_CMD_RESULT		// TP.0xE3
	},
	//NP_UCOM_UNPACK_POWER_ON_OFF
	{
		NP_UCOM_UNPACK_POWER_ON_OFF,	// ID
		NP_UCOM_TP_POWER_ON_OFF		// TP.0xE4
	},
	//NP_UCOM_UNPACK_TPED
	{
		NP_UCOM_UNPACK_TPED,			// ID
		NP_UCOM_TP_ED					// TP.0xED
	},
	// NP_UCOM_UNPACK_TPD3
	{
		NP_UCOM_UNPACK_TPD3,				// ID
		NP_UCOM_TP_UCOM_SRC_NOTIFY_CHANGED	// TP.D3
	},
	// NP_UCOM_UNPACK_TPD5
	{
		NP_UCOM_UNPACK_TPD5,				// ID
		NP_UCOM_TP_UCOM_DISP_BACKCAM		// TP.D5
	},
	// NP_UCOM_UNPACK_INTERRUPT_SCREEN
	{
		NP_UCOM_UNPACK_INTERRUPT_SCREEN,	// ID
		NP_UCOM_TP_UCOM_INTERR_SCREEN		// TP.D6
	},
	// NP_UCOM_UNPACK_PARK_PILOT_SCREEN
	{
		NP_UCOM_UNPACK_PARK_PILOT_SCREEN,	// ID
		NP_UCOM_TP_UCOM_PARK_PILOT_SCREEN	// TP.D7
	},
	// NP_UCOM_UNPACK_STOP_NAVIGUIDE
	{
		NP_UCOM_UNPACK_STOP_NAVIGUIDE,	// ID
		NP_UCOM_TP_UCOM_STOP_NAVIGUIDE	// TP.D8
	},
	// NP_UCOM_UNPACK_VA50
	{
		NP_UCOM_UNPACK_VA50,	//ID
		NP_UCOM_TP_UCOM_VACOMMAND50
	},
};


// サイズ用定義
const DWORD SIZE_MODEL_CMD_DEFAULT = NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1;
const DWORD SIZE_DISP_REQ_DEFAULT  = NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 3;
const DWORD SIZE_DISP_DATA_DEFAULT = NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1;

const size_t CHAR_SIZE_UCOMVER	= 5;
const size_t CHAR_SIZE_SYS_CWW_VER	= 9;

const BYTE	ACCOFF_RESTARTMODE		= 0x00;			//!< B+ OFF reset flag mask
const BYTE	ERROR_RESTARTMODE		= 0x01;			//!< B+ OFF reset flag mask
const BYTE	BOFF_RESTARTMODE		= 0x02;			//!< B+ OFF reset flag mask
const BYTE	BAT_ON_RESTARTMODE		= 0x03;			//!< B+ OFF reset flag mask
const BYTE	BOFF_RESET_MASK		= 0x1F;			//!< B+ OFF reset flag mask
const BYTE	VERIFY_STATUS_MASK		= 0x20;			//!< B+ OFF reset flag mask

/******************************************************************************

    コンストラクタ

******************************************************************************/
NPuComPacker::NPuComPacker()
:	// 初期化リスト
	NObject(),
	m_cRcvBD(),
	m_cSndBD(),
	m_sConeChk(),
	m_bVerRes(NP_FALSE),
	m_blBitNotifyStarted( NP_FALSE),
	m_bDetectWiredKeyPress(FALSE),
	m_bWiredKeyPress(FALSE)	,
	m_isTestModeNotify(0x00000000)
{
	// ビットデータクラスの初期化
	m_cRcvBD.Initialize();
	m_cSndBD.Initialize();

	// デフォルト設定(受信BD)
	SetRxBitData(NP_UCOM_BD1, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD1]);
	SetRxBitData(NP_UCOM_BD2, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD2]);
	SetRxBitData(NP_UCOM_BD3, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD3]);
	SetRxBitData(NP_UCOM_BD4, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD4]);
	SetRxBitData(NP_UCOM_BD5, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD5]);
	SetRxBitData(NP_UCOM_BD6, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD6]);
	SetRxBitData(NP_UCOM_BD7, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD7]);
	SetRxBitData(NP_UCOM_BD8, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD8]);
	SetRxBitData(NP_UCOM_BD9, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD9]);
	SetRxBitData(NP_UCOM_BD10, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD10]);
	// デフォルト設定(送信BD)
	SetTxBitData(NP_UCOM_BD1, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD1]);
	SetTxBitData(NP_UCOM_BD2, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD2]);
	SetTxBitData(NP_UCOM_BD3, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD3]);
	SetTxBitData(NP_UCOM_BD4, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD4]);
	SetTxBitData(NP_UCOM_BD5, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD5]);
	SetTxBitData(NP_UCOM_BD6, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD6]);
	SetTxBitData(NP_UCOM_BD7, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD7]);
	SetTxBitData(NP_UCOM_BD8, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD8]);
	SetTxBitData(NP_UCOM_BD9, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD9]);
	SetTxBitData(NP_UCOM_BD10, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD10]);

	// 構造体の初期化
	memset(&m_sConeChk, 0, sizeof(m_sConeChk));

	// 初期設定
	m_sConeChk.byModel1= NP_SYSINFO_MODEL1_UNKNOWN;
	m_sConeChk.byModel2= NP_SYSINFO_MODEL2_UNKNOWN;
	m_sConeChk.bResult  = NP_FALSE;
}

/******************************************************************************

    デストラクタ

******************************************************************************/
NPuComPacker::~NPuComPacker()
{
}

/******************************************************************************

    パッカークラスインスタンス取得関数

    【解説】
        プロセスで唯一のインスタンスの取得（シングルトンパターン）
        インスタンスが未生成の場合のみ生成し，既にインスタンスが生成されて
        いる場合はそのインスタンスを返す．シングルトンパターンとなっている．

******************************************************************************/
NPuComPacker
*NPuComPacker::Instance( VOID )
{
	s_cSync.SynchronizeStart();
	
	if ( NULL == s_pcInstance ){
		s_pcInstance = new NPuComPacker();
	}

	s_cSync.SynchronizeEnd();

	return s_pcInstance;
}

/******************************************************************************

    パッカークラスインスタンス破棄関数

    【解説】
        インスタンスの破棄を行なう

******************************************************************************/
VOID
NPuComPacker::Destroy( VOID )
{
	s_cSync.SynchronizeStart();

	if ( NULL != s_pcInstance ){
		delete s_pcInstance;
		s_pcInstance = NULL;
	}

	s_cSync.SynchronizeEnd();
}

/******************************************************************************

    パック処理

    【解説】
        設定されたデータをフレームにまとめる

******************************************************************************/
VOID
NPuComPacker::Pack( const DWORD dwId, LPCVOID pBuff, const DWORD dwSize, NPuComDataFrameUpdate *psFrame )
{
	// 共通
	{
		NPAutoSync cSync(s_cSyncBD);
		
		psFrame->m_byBD[NP_UCOM_BD1] = GetTxBitData(NP_UCOM_BD1);
		psFrame->m_byBD[NP_UCOM_BD2] = GetTxBitData(NP_UCOM_BD2);
		psFrame->m_byBD[NP_UCOM_BD3] = GetTxBitData(NP_UCOM_BD3);
		psFrame->m_byBD[NP_UCOM_BD4] = GetTxBitData(NP_UCOM_BD4);
		psFrame->m_byBD[NP_UCOM_BD5] = GetTxBitData(NP_UCOM_BD5);
		psFrame->m_byBD[NP_UCOM_BD6] = GetTxBitData(NP_UCOM_BD6);
		psFrame->m_byBD[NP_UCOM_BD7] = GetTxBitData(NP_UCOM_BD7);
		psFrame->m_byBD[NP_UCOM_BD8] = GetTxBitData(NP_UCOM_BD8);
		psFrame->m_byBD[NP_UCOM_BD9] = GetTxBitData(NP_UCOM_BD9);
		psFrame->m_byBD[NP_UCOM_BD10] = GetTxBitData(NP_UCOM_BD10);
	}

	// 種類別（クラス分けしたいなぁ）
	switch(dwId){
		case NP_UCOM_PACK_CONECHK_SEND:
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = 0x00;	// ０固定
				// 送信時の処理
				OnSendConeChkReq();
			}
			break;
		case NP_UCOM_PACK_CONECHK1_SEND:
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = m_sConeChk.byBootStatus;//0x02;	// ０固定
			}
			break;
		case NP_UCOM_PACK_CONECHK2_SEND:
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = 0x00;	// ０固定
			}
			break;
		case NP_UCOM_PACK_APK_ON_NOTIFY:
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = 0x00;	// ０固定
				
				{
					NPuComTimer *pcTimer = 	NPuComTimer::Instance();

					if( NULL != pcTimer) {
						pcTimer->MergeStatus(NP_UCOM_STATUS_APL_ON);
					}
				}
				
				// SysCtrlハンドラに通知
				{
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					if (pcMain!=NULL)
					{
						pcMain->ReqCallback(NP_UCOMHANDLER_CALLBACK_APK_ON, NULL, 0);
					}
				}
			}
			break;
		case NP_UCOM_PACK_POWER_ON_OFF_TPE3:
			if ((NULL == pBuff) || (1 != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				memcpy(&psFrame->m_byD[NP_UCOM_D0], pBuff, dwSize);
			}
			break;
		case NP_UCOM_PACK_DTV_TPE4_NOTIFY:
			if ((NULL == pBuff) || (2 != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				memcpy(&psFrame->m_byD[NP_UCOM_D0], pBuff, dwSize);
			}
			break;
		case NP_UCOM_PACK_SRC_CHANGE_RES:
			if ((NULL == pBuff) || (sizeof(NPuComSrcChgRes) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0]=NP_UCOM_D0_SRC_CHG;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComSrcChgRes));
			}
			break;
		case NP_UCOM_PACK_SRC_CHANGE_REQ:
			if ((NULL == pBuff) || (sizeof(NPuComSrcChgReq) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0]=NP_UCOM_D0_SRC_CHG_REQ;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComSrcChgReq));
			}
			break;
		case NP_UCOM_PACK_SRC_STATUS_NOTIFY:
			if ((NULL == pBuff) || (sizeof(NPuComSrcStatus) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0]=NP_UCOM_D0_SRC_CHG;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComSrcStatus));
			}
			break;
		case NP_UCOM_PACK_DIRECTCMD_SEND:
			if((NULL == pBuff) || (sizeof(NPuComDirectCmd) != dwSize)){
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				NPuComDirectCmd sDirectCmd;
				// 構造体の初期化
				memset(&sDirectCmd, 0, sizeof(sDirectCmd));
				// バッファのコピー
				memcpy(&sDirectCmd, pBuff, dwSize);
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = sDirectCmd.byCmdHi;
				psFrame->m_byD[NP_UCOM_D1] = sDirectCmd.byCmdLo;
			}
			break;
		case NP_UCOM_PACK_SRC_DIRECTCMD_SEND:
			if((NULL == pBuff) || (sizeof(NPuComModelCmd) != dwSize)){
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				NPuComModelCmd sModelCmd;
				// 構造体の初期化
				memset(&sModelCmd, 0, sizeof(sModelCmd));
				// バッファのコピー
				memcpy(&sModelCmd, pBuff, dwSize);
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize + sModelCmd.dwDataNum;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = sModelCmd.byKind;
				size_t Size = static_cast<size_t>(sModelCmd.dwDataNum);
				if(NP_UCOM_SIZE_D_NORMAL > Size){
					memcpy(&psFrame->m_byD[NP_UCOM_D1], sModelCmd.byData, Size);
				}
			}
			break;
		case NP_UCOM_PACK_DISP_DATA_REQ_SEND:
			if((NULL == pBuff) || (sizeof(NPuComDisplayReq) != dwSize)){
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				NPuComDisplayReq sDispReq;
				// 構造体の初期化
				memset(&sDispReq, 0, sizeof(sDispReq));
				// バッファのコピー
				memcpy(&sDispReq, pBuff, dwSize);
				// データ設定
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = NP_UCOM_TP_DISP_DATA_NAVI_FRONT;
				psFrame->m_byD[NP_UCOM_D1] = sDispReq.bySrc;
				psFrame->m_byD[NP_UCOM_D2] = sDispReq.byType;
				// サイズチェック
				if(NP_UCOM_SIZE_DISP_REQ_DATA < sDispReq.dwDataNum){
					// サイズオーバーの場合は丸め込む
					sDispReq.dwDataNum = NP_UCOM_SIZE_DISP_REQ_DATA;
				}
				psFrame->m_dwFrameSize = NP_UCOM_PACK_TABLE[dwId].dwSize + sDispReq.dwDataNum;
				size_t Size = static_cast<size_t>(sDispReq.dwDataNum);
				if((0 < Size) && (NP_UCOM_SIZE_DISP_REQ_DATA >= Size)){
					memcpy(&psFrame->m_byD[NP_UCOM_D3], sDispReq.byData, Size);
				}
			}
			break;
		case NP_UCOM_PACK_UPDATE_START_REQ:
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = D_VERSION_UP_ID_SYSTEM;	
				// 指定別データ設定
				NPuComDataUpdate *pcUpdate = NPuComDataUpdate::Instance();
				if(NULL != pcUpdate){
					psFrame->m_byD[NP_UCOM_D0] = pcUpdate->GetUpdateTypeID();
					pcUpdate->SendUpdateStartReq(psFrame);
				}
			}
			break;
		case NP_UCOM_PACK_UPDATE_WRITE_STA:
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = D_VERSION_UP_ID_SYSTEM;	

				NPuComDataUpdate *pcUpdate = NPuComDataUpdate::Instance();
				if(NULL != pcUpdate){
					psFrame->m_byD[NP_UCOM_D0] = pcUpdate->GetUpdateTypeID();
					// NP_UCOM_UNPACK_UPDATE_ERASE_END(TPC3) tiggers sendig for NP_UCOM_PACK_UPDATE_WRITE_STA(TPC4)
					// and the first data package NP_UCOM_PACK_UPDATE_WRITE_DATA(TPC5)
				}
			}
			break;
		case NP_UCOM_PACK_UPDATE_WRITE_DATA:
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				// 指定別データ設定
				NPuComDataUpdate *pcUpdate = NPuComDataUpdate::Instance();
				if(NULL != pcUpdate){
					pcUpdate->SendUpdateWriteSta(psFrame);
				}
			}
			break;
		case NP_UCOM_PACK_UPDATE_WRITE_VA_DATA:
			if((NULL == pBuff) || (sizeof(NPuComVAVerUpData) != dwSize)){
				// すぐ抜ける
				break;
			}
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;

				NPuComVAVerUpData sUpData;
				// 構造体の初期化
				memset(&sUpData, 0xFF, sizeof(sUpData));
				// バッファのコピー
				memcpy(&sUpData, pBuff, dwSize);
				// データ設定
				size_t Size = static_cast<size_t>(sUpData.dwDataNum);
				if(NP_UCOM_SIZE_VADATA_MAX >= Size){
					memcpy(&psFrame->m_byD[NP_UCOM_D0], &sUpData.byData[0], Size);
				}
			}
			break;
		case NP_UCOM_PACK_UPDATE_CHECK_STA:
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				// 指定別データ設定
				NPuComDataUpdate *pcUpdate = NPuComDataUpdate::Instance();
				if(NULL != pcUpdate){
					pcUpdate->SendUpdateCheckSta(psFrame);
				}
			}
			break;
		case NP_UCOM_PACK_UPDATE_CHECK_VA_STA:
			if((NULL == pBuff) || (sizeof(BYTE) != dwSize)){
				// すぐ抜ける
				break;
			}
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				BYTE byD0 = 0;
				memcpy(&byD0, pBuff, dwSize);
				psFrame->m_byD[NP_UCOM_D0] = byD0;
			}
			break;
		case NP_UCOM_PACK_UPDATE_RETRY_RES:
			if((NULL == pBuff) || (sizeof(BYTE) != dwSize)){
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定（デフォルト）
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = 0x00;	// ０固定
				// 指定別データ設定
				BYTE byTP = 0;
				memcpy(&byTP, pBuff, dwSize);
				NPuComDataUpdate *pcUpdate = NPuComDataUpdate::Instance();
				if(NULL != pcUpdate){
					pcUpdate->SendUpdateRetryRes(psFrame, byTP);
				}
			}
			break;
		case NP_UCOM_PACK_UPDATE_ERASE_REQ:
			if((NULL == pBuff) || (sizeof(BYTE) != dwSize)){
				// すぐ抜ける
				break;
			}
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;

				memcpy(&psFrame->m_byD[NP_UCOM_D0], pBuff, dwSize);
			}
			break;
		case NP_UCOM_PACK_UPDATE_RESET_PRO:
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = 0x00;	// ０固定
			}
			break;
		case NP_UCOM_PACK_UPDATE_RESTART_REQ:
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = 0x01;	// 01h固定
			}
			break;
		case NP_UCOM_PACK_BITDATA_NOTIFY:
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = 0x00;	// ０固定
			}
			break;
		case NP_UCOM_PACK_BEEP1_REQ_BITDATA_NOTIFY:
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = 0x00;	// ０固定
				psFrame->m_byBD[NP_UCOM_BD2] |= NP_UCOM_BD2_TX_BEEP1_REQ;
			}
			break;
		case NP_UCOM_PACK_BEEP2_REQ_BITDATA_NOTIFY:
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = 0x00;	// ０固定
				psFrame->m_byBD[NP_UCOM_BD2] |= NP_UCOM_BD2_TX_BEEP2_REQ;
			}
			break;
		case NP_UCOM_PACK_SXI_PW_PULL_UP_REQ_BITDATA_NOTIFY:
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = 0x00;	// ０固定
				psFrame->m_byBD[NP_UCOM_BD3] |= NP_UCOM_BD3_TX_SXI_PW_PULL_UP_REQ;
			}
			break;
		case NP_UCOM_PACK_SXI_PW_PULL_DOWN_REQ_BITDATA_NOTIFY:
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = 0x00;	// ０固定
				psFrame->m_byBD[NP_UCOM_BD3] |= NP_UCOM_BD3_TX_SXI_PW_PULL_DOWN_REQ;
			}
			break;
		case NP_UCOM_PACK_SXI_PWREN_PULL_UP_REQ_BITDATA_NOTIFY:
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = 0x00;	// ０固定
				psFrame->m_byBD[NP_UCOM_BD3] |= NP_UCOM_BD3_TX_SXI_PWREN_PULL_UP_REQ;
			}
			break;
		case NP_UCOM_PACK_SXI_PWREN_PULL_DOWN_REQ_BITDATA_NOTIFY:
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = 0x00;	// ０固定
				psFrame->m_byBD[NP_UCOM_BD3] |= NP_UCOM_BD3_TX_SXI_PWREN_PULL_DOWN_REQ;
			}
			break;
		case NP_UCOM_PACK_BITDATA_NOTIFY_REQ:
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = 0x00;	// ０固定
			}
			break;
		case NP_UCOM_PACK_UCOM_SRC_NOTIFY_REQ:
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = 0x00;	// ０固定
			}
			break;
		case NP_UCOM_PACK_TESTCMD_SEND:
			if((NULL == pBuff) || (sizeof(NPuComTestCmd) != dwSize)){
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				NPuComTestCmd sTestCmd;
				// 構造体の初期化
				memset(&sTestCmd, 0, sizeof(sTestCmd));
				// バッファのコピー
				memcpy(&sTestCmd, pBuff, dwSize);
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize + sTestCmd.dwDataNum;
				psFrame->m_byTP			   = sTestCmd.byTP;
				size_t Size = static_cast<size_t>(sTestCmd.dwDataNum);
				if(NP_UCOM_SIZE_D_NORMAL >= Size){
					memcpy(&psFrame->m_byD[NP_UCOM_D0], sTestCmd.byD, Size);
				}
			}
			break;
		case NP_UCOM_PACK_RESET_REQ:
			if (NULL == pBuff)
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = D_UCOM_RESET_FOR_TESTMODE_START;
				psFrame->m_byD[NP_UCOM_D1] = (*(BYTE *)pBuff);
				psFrame->m_byD[NP_UCOM_D2] = (0x00);

				if (psFrame->m_byD[NP_UCOM_D1]==0xFF)
				{
					psFrame->m_byD[NP_UCOM_D0] = D_UCOM_RESET_FOR_TESTMODE_STOP;
					psFrame->m_byD[NP_UCOM_D1] = 0x00;
				}
			}
			break;
		case NP_UCOM_PACK_SETUP_DATA:
			if( ( NULL == pBuff) || ( sizeof( NPuComSetupData) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				memcpy(&psFrame->m_byD[NP_UCOM_D0], pBuff, sizeof(NPuComSetupData));
			}
			break;
		case NP_UCOM_PACK_TESTMODE_NOTIFY:
			if( ( NULL == pBuff) || ( sizeof( NPuComTestModeStatus) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				memcpy(&psFrame->m_byD[NP_UCOM_D0], pBuff, sizeof(NPuComTestModeStatus));
			}
			break;
		case NP_UCOM_PACK_SET_DRM:
			if( ( NULL == pBuff) || ( sizeof( NPuComDRMInfoSet) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = NP_UCOM_D0_DRM_RES;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComDRMInfoSet));
				
			}
			break;
		case NP_UCOM_PACK_DRM_INFO_RES:
			if( ( NULL == pBuff) || ( sizeof( NPuComDRMInfoRes) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = NP_UCOM_D0_DRM_INFO;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComDRMInfoRes));
			}
			break;
		case NP_UCOM_PACK_TAGGING_RES:
			if( ( NULL == pBuff) || ( sizeof( NPuComTaggingRes) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = NP_UCOM_D0_TAGGING_START;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComTaggingRes));
			}
			break;
		case NP_UCOM_PACK_TAGGING_DATA1_RES:
			if( ( NULL == pBuff) || ( sizeof( NPuComTaggingData1Res) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = NP_UCOM_D0_TAGGING_DATA1;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComTaggingData1Res));
			}
			break;
		case NP_UCOM_PACK_TAGGING_DATA2_RES:
			if( ( NULL == pBuff) || ( sizeof( NPuComTaggingData2Res) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = NP_UCOM_D0_TAGGING_DATA2;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComTaggingData2Res));
			}
			break;
		case NP_UCOM_PACK_VAINFO_CURRSRC:
			if( ( NULL == pBuff) || ( sizeof( NPuComVAInfoCurrSrc) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize      = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP             = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0]  = NP_UCOM_D0_VAINFO_CURRSRC;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComVAInfoCurrSrc));
			}
			break;
		case NP_UCOM_PACK_VAINFO_ICON:
			if( ( NULL == pBuff) || ( sizeof( NPuComVAInfoIcon) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize      = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP             = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0]  = NP_UCOM_D0_VAINFO_ICON;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComVAInfoIcon));
			}
			break;
		case NP_UCOM_PACK_VAINFO_TUNER_STATUS:
			if( ( NULL == pBuff) || ( sizeof( NPuComVAInfoTunerStatus) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize      = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP             = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0]  = NP_UCOM_D0_VAINFO_TUNERSTA;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComVAInfoTunerStatus));
			}
			break;
		case NP_UCOM_PACK_VAINFO_MEDIA_STATUS:
			if( ( NULL == pBuff) || ( sizeof( NPuComVAInfoMediaStatus) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize      = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP             = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0]  = NP_UCOM_D0_VAINFO_MEDIASTA;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComVAInfoMediaStatus));
			}
			break;
		case NP_UCOM_PACK_VAINFO_PHONE_STATUS:
			if( ( NULL == pBuff) || ( sizeof( NPuComVAInfoPhoneStatus) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize      = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP             = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0]  = NP_UCOM_D0_VAINFO_PHONESTA;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComVAInfoPhoneStatus));
			}
			break;
		case NP_UCOM_PACK_VAINFO_TEXT_DATA:
			if( ( NULL == pBuff) || ( sizeof( NPuComVAInfoText) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize      = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP             = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0]  = NP_UCOM_D0_VAINFO_TEXT;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComVAInfoText));
			}
			break;
		case NP_UCOM_PACK_VAINFO_NAVI_COMMAND:
			if( ( NULL == pBuff) || ( sizeof( NPuComVAInfoNaviCMD) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize      = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP             = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0]  = NP_UCOM_D0_VAINFO_NAVICMD;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComVAInfoNaviCMD));
			}
			break;
		case NP_UCOM_PACK_VAINFO_STREETNAME:
			if( ( NULL == pBuff) || ( sizeof( NPuComVAInfoStreetName) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize      = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP             = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0]  = NP_UCOM_D0_VAINFO_STREETNAME;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComVAInfoStreetName));
			}
			break;
		case NP_UCOM_PACK_VAINFO_DISTANCE:
			if( ( NULL == pBuff) || ( sizeof( NPuComVAInfoDistance) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize      = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP             = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0]  = NP_UCOM_D0_VAINFO_DISTANCE;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComVAInfoDistance));
			}
			break;
		case NP_UCOM_PACK_VAINFO_ARRIVALTIME:
			if( ( NULL == pBuff) || ( sizeof( NPuComVAInfoArrivalTime) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize      = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP             = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0]  = NP_UCOM_D0_VAINFO_ARRIVALTIME;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComVAInfoArrivalTime));
			}
			break;
		case NP_UCOM_PACK_VAINFO_REQTIME:
			if( ( NULL == pBuff) || ( sizeof( NPuComVAInfoReqTime) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize      = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP             = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0]  = NP_UCOM_D0_VAINFO_REQTIME;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComVAInfoReqTime));
			}
			break;
		case NP_UCOM_PACK_VAINFO_COMPASSDIR:
			if( ( NULL == pBuff) || ( sizeof( NPuComVAInfoCompassDir) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize      = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP             = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0]  = NP_UCOM_D0_VAINFO_COMPASSDIR;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComVAInfoCompassDir));
			}
			break;
		case NP_UCOM_PACK_VAINFO_INTERVALTIME:
			if( ( NULL == pBuff) || ( sizeof( NPuComVAInfoIntervaltime) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize      = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP             = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0]  = NP_UCOM_D0_VAINFO_INTERVALTIME;
				memcpy(&psFrame->m_byD[NP_UCOM_D1], pBuff, sizeof(NPuComVAInfoIntervaltime));
			}
			break;
		case NP_UCOM_PACK_SLAVE_TESTMODE_REQ:
			if( ( NULL == pBuff) || ( sizeof( BYTE) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				memcpy(&psFrame->m_byD[NP_UCOM_D0], pBuff, sizeof(BYTE));
			}
			break;
		case NP_UCOM_PACK_SOURCELIST_NOTIFY:
			if( ( NULL == pBuff) || ( sizeof( NPuComSourceList) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				memcpy(&psFrame->m_byD[NP_UCOM_D0], pBuff, sizeof(NPuComSourceList));
			}
			break;
		case NP_UCOM_PACK_VOLUME_MODIFY:
			if( ( NULL == pBuff) || ( sizeof( BYTE) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				memcpy(&psFrame->m_byD[NP_UCOM_D0], pBuff, sizeof(BYTE));
			}
			break;
		case NP_UCOM_PACK_VERIFY_STATUS:
			if( ( NULL == pBuff) || ( sizeof( BYTE) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				memcpy(&psFrame->m_byD[NP_UCOM_D0], pBuff, sizeof(BYTE));
			}
			break;
		case NP_UCOM_PACK_SOC_VERSIONUP_NOTIFY:
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = D_VERSION_UP_ID_DISP;
			}
			break;
		case NP_UCOM_PACK_VA_VERSIONUP_NOTIFY:
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				psFrame->m_byD[NP_UCOM_D0] = D_VERSION_UP_ID_VA;
			}
			break;
		case NP_UCOM_PACK_DVD_ONOFF_CMD:
			if( ( NULL == pBuff) || ( sizeof( BYTE) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				memcpy(&psFrame->m_byD[NP_UCOM_D0], pBuff, sizeof(BYTE));
			}
			break;
		case NP_UCOM_PACK_TPEC:     // TPEC key table receive, D0~D60
			if( ( NULL == pBuff) || ( sizeof( NPuComKeyTable) != dwSize))
			{
				// すぐ抜ける
				break;
			}
			// データの設定
			{
				// データ設定
				psFrame->m_dwFrameSize	   = NP_UCOM_PACK_TABLE[dwId].dwSize;
				psFrame->m_byTP			   = NP_UCOM_PACK_TABLE[dwId].byTP;
				memcpy(&psFrame->m_byD[NP_UCOM_D0], pBuff, sizeof(NPuComKeyTable));
			}
			break;
		default:
			// 何もしない
			break;
	}
}

/******************************************************************************

    アンパック処理

    【解説】
        設定されたフレームをデータに分解する

******************************************************************************/
VOID
NPuComPacker::UnPack( const NPuComDataFrame *psFrame )
{
	// BD解析
	NP_BOOL bBDcheck=NP_TRUE;
	
	if (m_blBitNotifyStarted==NP_FALSE)
	{
		if ((psFrame->m_byTP!=NP_UCOM_TP_DISP_DATA_NAVI_FRONT)
			||(psFrame->m_byD[NP_UCOM_D0]!=0x80))
		{
			bBDcheck=NP_FALSE;
		}
	}

	if (bBDcheck==NP_TRUE)
	{
		CheckReceiveBD(psFrame);
	}
	
	// テストモード用に全パケット通知
	{
		NPuComTimer *pcTimer = 	NPuComTimer::Instance();
		if( ( NULL != pcTimer)
			&& ( NP_TRUE == pcTimer->IsTestModeOn())){
			// テストモード時のみ処理を行う
			NPuComTestCmd sCmd;
			memset(&sCmd, 0, sizeof(sCmd));

			sCmd.dwDataNum = psFrame->m_dwFrameSize - (NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD);
			sCmd.byTP	   = psFrame->m_byTP;
			// データ部のコピー
			size_t Size = static_cast<size_t>(sCmd.dwDataNum);
			if(NP_UCOM_SIZE_D_NORMAL >= Size){
				memcpy(sCmd.byD, &psFrame->m_byD[NP_UCOM_D0], Size);
			}
			// イベント送信
			NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
			pcMain->ReqEvent(new NPuComEvent(&sCmd));
		}
	}

	NP_BOOL bMatch = NP_FALSE;
	DWORD	dwId = NP_UCOM_UNPACK_ID_MAX;

	// TP検索
	for(DWORD dwLoop = 0; dwLoop < NP_UCOM_UNPACK_ID_MAX; ++dwLoop){
		if(psFrame->m_byTP == NP_UCOM_UNPACK_TABLE[dwLoop].byTP){
			// TP一致
			bMatch = NP_TRUE;
			dwId   = NP_UCOM_UNPACK_TABLE[dwLoop].dwId;
			break;
		}
	}
	if(NP_FALSE == bMatch){
		// 不一致はすぐ抜ける
		return;
	}

	// 種類別（クラス分けしたいなぁ）
	switch(dwId){
		case NP_UCOM_UNPACK_CONECHK1_RCV:
			{
				NPuComTimer *pcTimer = 	NPuComTimer::Instance();
				if(NULL != pcTimer)
				{
					if(NP_UCOM_STATUS_CONECHK_WAIT_RES == (NP_UCOM_STATUS_CONECHK_WAIT_RES & pcTimer->GetStatus()))
					{
						// 初期通信情報
						m_sConeChk.byBootStatus = (psFrame->m_byD[NP_UCOM_D0] & BOFF_RESET_MASK);
						if (NPWarp::STATUS_NORMAL_BOOT_CATCH_SNAPSHOT != NPWarp::Instance()->GetBootMode()) 
						{
							if ((psFrame->m_byD[NP_UCOM_D0] & VERIFY_STATUS_MASK)==VERIFY_STATUS_MASK)
							{
								property_set(NP_PROPERTY_VERIFY_STATUS, "1");
							}
							else
							{
								property_set(NP_PROPERTY_VERIFY_STATUS, "0");
							}
						}
					}
				}
				{
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					pcMain->SendConeChk1Req();
				}
			}
			break;
		case NP_UCOM_UNPACK_CONECHK2_RCV:
			{
				NPuComTimer *pcTimer = 	NPuComTimer::Instance();
				NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
				
				pcMain->SendConeChk2Req();

				if(NULL != pcTimer)
				{
					if(NP_UCOM_STATUS_CONECHK_WAIT_RES == (NP_UCOM_STATUS_CONECHK_WAIT_RES & pcTimer->GetStatus()))
					{
						if (NPWarp::STATUS_NORMAL_BOOT_CATCH_SNAPSHOT != NPWarp::Instance()->GetBootMode()) {
							// 初期通信情報
							m_sConeChk.byModel1		= psFrame->m_byD[NP_UCOM_D0];
							m_sConeChk.byModel2		= psFrame->m_byD[NP_UCOM_D1];
						} else {
							// if is normal boot,set model1 and model2 to unknow
							m_sConeChk.byModel1		= 0xFF;
							m_sConeChk.byModel2		= 0xFF;
							m_sConeChk.WarpCatchModel  = psFrame->m_byD[NP_UCOM_D0];
							m_sConeChk.WarpCatchRegion = psFrame->m_byD[NP_UCOM_D1];
						}
						
						{
							char LocalBuf[15] ={'\0'};
							sprintf(LocalBuf,"%02X.%02x", psFrame->m_byD[NP_UCOM_D0], psFrame->m_byD[NP_UCOM_D1]);
							property_set( "revision.sys.model1.2", LocalBuf );
						}
						   
						//	LIJIAN,2014.07.13 FOR TEST ONLY
						/*
						{
							char LocalBuf[15] ={'\0'};
							m_sConeChk.byModel1			= NP_SYSINFO_MODEL1_KM612;
							m_sConeChk.byModel2			= NP_SYSINFO_MODEL2_EU5;
							sprintf(LocalBuf,"%02X.%02x", m_sConeChk.byModel1, m_sConeChk.byModel2 );
							property_set( "revision.sys.model1.2.f", LocalBuf );
						} 
						*/
						
						m_sConeChk.video_output_type  = psFrame->m_byD[NP_UCOM_D2];
						m_sConeChk.divx_license_id[0] = psFrame->m_byD[NP_UCOM_D3];
						m_sConeChk.divx_license_id[1] = psFrame->m_byD[NP_UCOM_D4];
						m_sConeChk.detach_type        = ((psFrame->m_byD[NP_UCOM_D5]>>1)&0x01);
						m_sConeChk.isDvdConnected     = ((psFrame->m_byD[NP_UCOM_D5]>>0)&0x01);
						m_sConeChk.security_status    = 0;
						memcpy(&m_sConeChk.firmware_version[0], &psFrame->m_byD[NP_UCOM_D6], 3);
						memcpy(&m_sConeChk.hardware_version[0], &psFrame->m_byD[NP_UCOM_D9], 3);
						memcpy(&m_sConeChk.model_number[0], &psFrame->m_byD[NP_UCOM_D12], 12);
						
						// 受信時の処理
						OnReceiveConeChkRes();
					}
					
					{
						char LocalBuf[15] ={'\0'};
						sprintf(LocalBuf,"%02X.%02x",
								psFrame->m_byD[NP_UCOM_D0],
								psFrame->m_byD[NP_UCOM_D1]
							   );
						property_set( "revision.sys.model1.2", LocalBuf );
					}
				}
			}
			break;
		case NP_UCOM_UNPACK_SRC_CHG:
			{
				if (NP_UCOM_D0_SRC_CHG == psFrame->m_byD[NP_UCOM_D0])
				{
					NPuComSourceSwitch sSwitch;
					
					memset(&sSwitch, 0, sizeof(sSwitch));
					
					sSwitch.byStatus	= psFrame->m_byD[NP_UCOM_D1];
					sSwitch.byKind	= psFrame->m_byD[NP_UCOM_D2];
					// 指定ソース
					sSwitch.bySrc 		= psFrame->m_byD[NP_UCOM_D3];

					// アプリに通知
					{
						NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
						pcMain->ReqEvent(new NPuComRcvSourceSwitchEvent(&sSwitch));
					}
				}

			}
			break;
		case NP_UCOM_UNPACK_SRC_STATUS_REQ:
			{
				if (NP_UCOM_D0_SRC_CHG == psFrame->m_byD[NP_UCOM_D0])
				{
					BYTE sReq;
										
					sReq	= psFrame->m_byD[NP_UCOM_D1];
					
					// アプリに通知
					{
						NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
						pcMain->ReqEvent(new NPuComRcvSrcStatusReqEvent(&sReq));
					}
				}

			}
			break;
		case NP_UCOM_UNPACK_DISP_DATA_NAVI_FRONT:
			{
				NPuComDisplayData sDispData;
				memset(&sDispData, 0, sizeof(sDispData));

				// データのコピー
				sDispData.dwDataNum	= psFrame->m_dwFrameSize - (NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 1);
				sDispData.byKind = NP_UCOM_SRC_KIND_FRONT;
				// ソース番号の設定
				sDispData.bySrc = psFrame->m_byD[NP_UCOM_D0];
				// データ部のコピー
				size_t Size = static_cast<size_t>(sDispData.dwDataNum);
				if(NP_UCOM_SIZE_D_NORMAL <= Size){
					break;
				}
				memcpy(sDispData.byData, &psFrame->m_byD[NP_UCOM_D1], Size);

				// アプリに通知
				{
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					pcMain->ReqEvent(new NPuComRcvDisplayDataEvent(&sDispData));
					
					if (
						(psFrame->m_byD[NP_UCOM_D0] == 0x70)	&&	// TP61 D0.70 TESTMODE表示データ
						(psFrame->m_byD[NP_UCOM_D1] == 0x0A)	&&	//      D1.0A test mode normal data
						(psFrame->m_byD[NP_UCOM_D9] != 0x00)		//      D9.XX test mode type
					   )
					{
						if ( ( m_isTestModeNotify == 0x00000000) || ( (m_isTestModeNotify&0x000000FF)!= psFrame->m_byD[NP_UCOM_D9] ) )
						{
							m_isTestModeNotify = 0x00000100 + psFrame->m_byD[NP_UCOM_D9];
							pcMain->ReqCallback(NP_UCOMHANDLER_CALLBACK_TESTMODE_NOTIFY, (void *)&psFrame->m_byD[NP_UCOM_D9], sizeof(BYTE));
							OutputDebugLog( "NPuComHandler-- CALLBACK_TESTMODE_NOTIFY D9.%02X ",psFrame->m_byD[NP_UCOM_D9]);
						}
					}
					
					OutputDebugLog(
									"NPuComHandler-- TP61 D0.%02X D1.%02X D9.%02X ",
									psFrame->m_byD[NP_UCOM_D0],
									psFrame->m_byD[NP_UCOM_D1],
									psFrame->m_byD[NP_UCOM_D9]
								  );
				}

			}
			break;
		case NP_UCOM_UNPACK_UPDATE_START_RES:
			{
				// マイコンに返答
				NPuComHandler *pcMain = NPuComHandler::GetuComHandler();

				if (D_VERSION_UP_ID_VA == psFrame->m_byD[NP_UCOM_D0])
				{
					BYTE sRes;
					sRes	= psFrame->m_byD[NP_UCOM_D1];

					// アプリに通知
					{
						pcMain->ReqEvent(new NPuComRcvVAUpdateResEvent(&sRes));
					}
				}
				else
				{
					BYTE Res = D_VERSION_UP_OK;

					if(
					   ( (psFrame->m_byD[NP_UCOM_D0]!=D_VERSION_UP_ID_SYSTEM) && (psFrame->m_byD[NP_UCOM_D0]!=D_VERSION_UP_ID_WWR) )
					   ||
					   ( psFrame->m_byD[NP_UCOM_D1]!=D_VERSION_UP_OK)
					  )
					{
						Res =D_VERSION_UP_NG;
					}

					pcMain->OnReceiveUpdateStartRes(Res);
				}
			}
			break;
		case NP_UCOM_UNPACK_UPDATE_ERASE_END:
			{
				// マイコンに返答
				{
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					BYTE Res = D_VERSION_UP_OK;

					if(
					   ( (psFrame->m_byD[NP_UCOM_D0]!=D_VERSION_UP_ID_SYSTEM) && (psFrame->m_byD[NP_UCOM_D0]!=D_VERSION_UP_ID_WWR) )
					   ||
					   ( psFrame->m_byD[NP_UCOM_D1]!=D_VERSION_UP_OK)
					  )
					{
						Res = D_VERSION_UP_NG;
					}
					
					pcMain->OnReceiveUpdateEraseRes(Res);
				}
			}
			break;
		case NP_UCOM_UNPACK_UPDATE_WRITE_END:
			{
				// マイコンに返答
				{
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					pcMain->OnReceiveUpdateWriteRes(psFrame->m_byD[NP_UCOM_D0]);

					BYTE sRes;
					sRes	= psFrame->m_byD[NP_UCOM_D0];
					// アプリに通知
					pcMain->ReqEvent(new NPuComRcvVAUpdateWriteResEvent(&sRes));
				}
			}
			break;
		case NP_UCOM_UNPACK_UPDATE_CHECK_END:
			{
				// マイコンに返答
				{
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					pcMain->OnReceiveUpdateCheckRes(psFrame->m_byD[NP_UCOM_D0]);

					BYTE sRes;
					sRes	= psFrame->m_byD[NP_UCOM_D0];
					// アプリに通知
					pcMain->ReqEvent(new NPuComRcvVAUpdateCheckSumResEvent(&sRes));
				}
			}
			break;
		case NP_UCOM_UNPACK_UPDATE_ERASE_RES:
			{
				BYTE sRes;
				sRes	= psFrame->m_byD[NP_UCOM_D0];

				// アプリに通知
				{
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					pcMain->ReqEvent(new NPuComRcvVAEraseResEvent(&sRes));
				}
			}
			break;
		case NP_UCOM_UNPACK_UPDATE_VAUPDATE_RES:
			{
				BYTE sRes;
				sRes	= psFrame->m_byD[NP_UCOM_D0];

				// アプリに通知
				{
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					pcMain->ReqEvent(new NPuComRcvVAUpdateResultEvent(&sRes));
				}
			}
			break;
		case NP_UCOM_UNPACK_BITDATA_NOTIFY_REQ:
			{
				// マイコンに返答
				{
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					pcMain->SendBitData();
				}
			}
			break;
		case NP_UCOM_UNPACK_UCOM_SRC_NOTIFY:
			{
				NPuComRcvSrcStatus sStatus;
				memset(&sStatus, 0, sizeof(sStatus));

				// データのコピー
				sStatus.byFrontSrc = psFrame->m_byD[NP_UCOM_D0];
				sStatus.byRearSrc  = psFrame->m_byD[NP_UCOM_D1];
				// データ部のコピー
				size_t Size = NP_UCOM_SIZE_UCOMSRC_MAX;
				memcpy(sStatus.byStatus, &psFrame->m_byD[NP_UCOM_D2], Size);

				// アプリに通知
				{
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					pcMain->ReqEvent(new NPuComRcvSrcStatusEvent(&sStatus));
				}
			}
			break;
		case NP_UCOM_UNPACK_VERSION_RES:
			if (NPWarp::STATUS_NORMAL_BOOT_CATCH_SNAPSHOT != NPWarp::Instance()->GetBootMode())
			{
				// バージョン受信中フラグON
				m_bVerRes = NP_TRUE;

				NPSysInfoSet cSet;

				switch(psFrame->m_byD[NP_UCOM_D0])
				{
					case NP_UCOM_VERSION_SYSCOM:		// TPE1 D001
						{
							NPSysInfouComVer sVer;		// TPE1 D001
							memset(&sVer, 0, sizeof(sVer));

							sVer.Flg = NP_TRUE;
//							memcpy(&sVer.Ver, &psFrame->m_byD[NP_UCOM_D1], (NP_SYSINFO_UCOMVER_SIZE - 1));
							// ROMコレ部分は書き込まない
							memcpy(sVer.Ver, &psFrame->m_byD[NP_UCOM_D1], CHAR_SIZE_UCOMVER);

							cSet.Set(NP_SYSINFO_UCOM_SYSTEM, &sVer);
							
							NP_UCOM_PRINTF(("NPuComHandler--recieve sys uCom version: %x %x %x %x %x \r\n",
								sVer.Ver[0],sVer.Ver[1],sVer.Ver[2],sVer.Ver[3],sVer.Ver[4]));
							{
								char LocalBuf[15] ={'\0'};
								sprintf(LocalBuf,"%c%c%c%c%c",
										sVer.Ver[0],sVer.Ver[1],sVer.Ver[2],sVer.Ver[3],sVer.Ver[4]
										);
								property_set( "revision.sys.ucom", LocalBuf );
							}
						}
						break;
					case NP_UCOM_VERSION_DVD_MECHA:		// TPE1 D002
						{
							NPSysInfouComVer sVer;		// TPE1 D002
							memset(&sVer, 0, sizeof(sVer));

							sVer.Flg = NP_TRUE;
							memcpy(sVer.Ver, &psFrame->m_byD[NP_UCOM_D1], (NP_SYSINFO_UCOMVER_SIZE - 1));

							cSet.Set(NP_SYSINFO_MECHA, &sVer);

							NP_UCOM_PRINTF(("NPuComHandler--recieve DVD Mecha version: %x %x %x %x %x %x %x %x %x %x %x \r\n",
								sVer.Ver[0],sVer.Ver[1],sVer.Ver[2],sVer.Ver[3],sVer.Ver[4],sVer.Ver[5],
								sVer.Ver[6],sVer.Ver[7],sVer.Ver[8],sVer.Ver[9],sVer.Ver[10]));
							{
								char LocalBuf[15] ={'\0'};
								sprintf(LocalBuf,"%c%c%c%c%c%c%c%c%c%c%c",
										sVer.Ver[0],sVer.Ver[1],sVer.Ver[2],sVer.Ver[3],sVer.Ver[4],
										sVer.Ver[5],sVer.Ver[6],sVer.Ver[7],sVer.Ver[8],sVer.Ver[9],
										sVer.Ver[10]
										);
								property_set( "revision.dvd.mecha", LocalBuf );
							}
						}
						break;
					case NP_UCOM_VERSION_DVD_REGION:	// TPE1 D003
						{
							NPSysInfoMechaInfo sInfo;
							memset(&sInfo, 0, sizeof(sInfo));

							sInfo.Flg = NP_TRUE;
							memcpy(sInfo.Region, &psFrame->m_byD[NP_UCOM_D1], (NP_SYSINFO_MECHAREGION_SIZE - 1));

							cSet.Set(&sInfo);

							NP_UCOM_PRINTF(("NPuComHandler--recieve DVD Mecha  Region version: %x %x %x %x %x %x %x %x %x %x %x \r\n",
								sInfo.Region[0],sInfo.Region[1],sInfo.Region[2],sInfo.Region[3],sInfo.Region[4],sInfo.Region[5],
								sInfo.Region[6],sInfo.Region[7],sInfo.Region[8],sInfo.Region[9],sInfo.Region[10]));
							{
								char LocalBuf[15] ={'\0'};
								sprintf(LocalBuf,"%c%c%c%c%c%c%c%c%c%c%c",
										sInfo.Region[0],sInfo.Region[1],sInfo.Region[2],sInfo.Region[3],sInfo.Region[4],
										sInfo.Region[5],sInfo.Region[6],sInfo.Region[7],sInfo.Region[8],sInfo.Region[9],
										sInfo.Region[10]
										);
								property_set( "revision.dvd.region", LocalBuf );
							}
						}
						break;
					case NP_UCOM_VERSION_DTV_SDK:		// TPE1 D004
						{
							NPSysInfouComVer sVer;		// TPE1 D004
							memset(&sVer, 0, sizeof(sVer));

							sVer.Flg = NP_TRUE;
							memcpy(sVer.Ver, &psFrame->m_byD[NP_UCOM_D1], 11 );

							cSet.Set(NP_SYSINFO_DTV_SDK, &sVer);

							NP_UCOM_PRINTF(("NPuComHandler--recieve DTV SDK version: %x %x %x %x %x %x %x %x %x %x %x \r\n",
								sVer.Ver[0],sVer.Ver[1],sVer.Ver[2],sVer.Ver[3],sVer.Ver[4],sVer.Ver[5],
								sVer.Ver[6],sVer.Ver[7],sVer.Ver[8],sVer.Ver[9],sVer.Ver[10] ));
							{
								char LocalBuf[15] ={'\0'};
								sprintf(LocalBuf,"%c%c%c%c%c%c%c%c%c%c%c",
										sVer.Ver[0],sVer.Ver[1],sVer.Ver[2],sVer.Ver[3],sVer.Ver[4],
										sVer.Ver[5],sVer.Ver[6],sVer.Ver[7],sVer.Ver[8],sVer.Ver[9],
										sVer.Ver[10]
										);
								property_set( "revision.dtv.sdk", LocalBuf );
							}
						}
						break;
					case NP_UCOM_VERSION_DTV_IF:		// TPE1 D005
						{
							NPSysInfouComVer sVer;		// TPE1 D005
							memset(&sVer, 0, sizeof(sVer));

							sVer.Flg = NP_TRUE;
							memcpy(sVer.Ver, &psFrame->m_byD[NP_UCOM_D1], 11 );

							cSet.Set(NP_SYSINFO_DTV_IF, &sVer);

							NP_UCOM_PRINTF(("NPuComHandler--recieve DTV IF version: %x %x %x %x %x %x %x %x %x %x %x \r\n",
								sVer.Ver[0],sVer.Ver[1],sVer.Ver[2],sVer.Ver[3],sVer.Ver[4],sVer.Ver[5],
								sVer.Ver[6],sVer.Ver[7],sVer.Ver[8],sVer.Ver[9],sVer.Ver[10] ));
							{
								char LocalBuf[15] ={'\0'};
								sprintf(LocalBuf,"%c%c%c%c%c%c%c%c%c%c%c",
										sVer.Ver[0],sVer.Ver[1],sVer.Ver[2],sVer.Ver[3],sVer.Ver[4],
										sVer.Ver[5],sVer.Ver[6],sVer.Ver[7],sVer.Ver[8],sVer.Ver[9],
										sVer.Ver[10]
										);
								property_set( "revision.dtv.if", LocalBuf );
							}
						}
						break;
					case NP_UCOM_VERSION_SYS_CWW:		// TPE1 D006
						{
							NPSysInfouComVer sVer;		// TPE1 D006
							memset(&sVer, 0, sizeof(sVer));

							sVer.Flg = NP_TRUE;
							memcpy(sVer.Ver, &psFrame->m_byD[NP_UCOM_D1], CHAR_SIZE_SYS_CWW_VER);

							cSet.Set(NP_SYSINFO_SYS_CWW, &sVer);

							NP_UCOM_PRINTF(("NPuComHandler--recieve SYS CWW version: %x %x %x %x %x %x %x %x %x %x %x \r\n",
								sVer.Ver[0],sVer.Ver[1],sVer.Ver[2],sVer.Ver[3],sVer.Ver[4],sVer.Ver[5],
								sVer.Ver[6],sVer.Ver[7],sVer.Ver[8],sVer.Ver[9],sVer.Ver[10]));
							{
								char LocalBuf[15] ={'\0'};
								sprintf(LocalBuf,"%c%c%c%c%c%c%c%c%c",
										sVer.Ver[0],sVer.Ver[1],sVer.Ver[2],sVer.Ver[3],sVer.Ver[4],
										sVer.Ver[5],sVer.Ver[6],sVer.Ver[7],sVer.Ver[8]
										);
								property_set( "revision.sys.cvj", LocalBuf );
							}
						}
						break;
					case NP_UCOM_VERSION_WWR:			// TPE1 D008
						{
							NPSysInfouComVer sVer;		// TPE1 D008
							memset(&sVer, 0, sizeof(sVer));

							sVer.Flg = NP_TRUE;
							memcpy(sVer.Ver, &psFrame->m_byD[NP_UCOM_D1], (NP_SYSINFO_MECHAREGION_SIZE - 1));

							cSet.Set(NP_SYSINFO_UCOM_WWR, &sVer);

							NP_UCOM_PRINTF(("NPuComHandler--recieve SYS WWR version: %x %x %x %x %x %x %x %x %x %x %x \r\n",
								sVer.Ver[0],sVer.Ver[1],sVer.Ver[2],sVer.Ver[3],sVer.Ver[4],sVer.Ver[5],
								sVer.Ver[6],sVer.Ver[7],sVer.Ver[8],sVer.Ver[9],sVer.Ver[10]));
							{
								char LocalBuf[15] ={'\0'};
								sprintf(LocalBuf,"%c%c%c%c",
										sVer.Ver[0],sVer.Ver[1],sVer.Ver[2],sVer.Ver[3]
										);
								property_set( "revision.sys.wwr", LocalBuf );
							}
						}
						break;
					default:
						break;
				}

				// バージョン受信中フラグOFF
				m_bVerRes = NP_FALSE;
			}
			break;

		case NP_UCOM_UNPACK_ACCOFF_NOTIFY:
			{
				NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
				pcMain->NotifyAccOff();
			}
			break;
		case NP_UCOM_UNPACK_UCOM_IMAGE_NOTIFY:
			{
				NPuComRcvImageStatus sStatus;
				memset(&sStatus, 0, sizeof(sStatus));

				// データ部のコピー
				size_t Size = NP_UCOM_SIZE_UCOMIMAGE_MAX;
				memcpy(sStatus.byStatus, &psFrame->m_byD[NP_UCOM_D0], Size);

				// アプリに通知
				{
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					pcMain->ReqEvent(new NPuComRcvImageStatusEvent(&sStatus));
				}
			}
			break;
/*		case NP_UCOM_UNPACK_BACK_UP_DATA_NOTIFY:	//lijian, 15mid -> TPE3 = NP_UCOM_UNPACK_DTV_CMD_RESULT
			{
				NPuComSetupData sStatus;
				memset(&sStatus, 0, sizeof(sStatus));

				// データ部のコピー
				size_t Size = NP_UCOM_SIZE_SETUPDATA_MAX;
				memcpy(sStatus.m_byPacket, &psFrame->m_byD[NP_UCOM_D0], Size);

				// アプリに通知
				{
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					pcMain->ReqEvent(new NPuComRcvBackupDataEvent(&sStatus));
				}
			}
			break;*/
		case NP_UCOM_UNPACK_DRM_CMD:
			{
				if (NP_UCOM_D0_DRM_RES==psFrame->m_byD[NP_UCOM_D0])
				{
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					pcMain->ReqEvent(new NPuComRcvDRMResEvent(psFrame->m_byD[NP_UCOM_D1]));
				}
				else if (NP_UCOM_D0_DRM_INFO==psFrame->m_byD[NP_UCOM_D0])
				{
					NPuComDRMInfo sStatus;
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					
					memset(&sStatus, 0, sizeof(NPuComDRMInfo));
					memcpy(sStatus.m_byPacket, &psFrame->m_byD[NP_UCOM_D1], NP_UCOM_SIZE_DRM_INFO_MAX);
					pcMain->ReqEvent(new NPuComRcvDRMInfoEvent(&sStatus));
				}
			}
			break;
		case NP_UCOM_UNPACK_TAGGING_DATA:
			{
				if (NP_UCOM_D0_TAGGING_START==psFrame->m_byD[NP_UCOM_D0])
				{
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					NPuComTaggingStartInfo sStatus;

					memset(&sStatus, 0, sizeof(NPuComTaggingStartInfo));
					sStatus.m_TagNum = psFrame->m_byD[NP_UCOM_D1];
					sStatus.m_TagType = psFrame->m_byD[NP_UCOM_D2];
					
					pcMain->ReqEvent(new NPuComRcvTaggingStartEvent(&sStatus));
				}
				else if (NP_UCOM_D0_TAGGING_DATA1==psFrame->m_byD[NP_UCOM_D0])
				{
					NPuComTaggingData1 sStatus;
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					
					memset(&sStatus, 0, sizeof(NPuComTaggingData1));
					memcpy(sStatus.m_byPacket, &psFrame->m_byD[NP_UCOM_D1], NP_UCOM_SIZE_TAGGING_DATA1_MAX);
					pcMain->ReqEvent(new NPuComRcvTaggingData1Event(&sStatus));
				}
				else if (NP_UCOM_D0_TAGGING_DATA2==psFrame->m_byD[NP_UCOM_D0])
				{
					NPuComTaggingData2 sStatus;
					NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
					
					memset(&sStatus, 0, sizeof(NPuComTaggingData2));
					memcpy(sStatus.m_byPacket, &psFrame->m_byD[NP_UCOM_D1], NP_UCOM_SIZE_TAGGING_DATA2_MAX);
					pcMain->ReqEvent(new NPuComRcvTaggingData2Event(&sStatus));
				}
			}
			break;
			
			case NP_UCOM_UNPACK_DTV_LIST_INFOR:	// TP=0x4A, D001, d1-d67
			{
				NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
				
				NPuComDTV_Data_ListInfor sStatus;
				memset(&sStatus, 0, sizeof( NPuComDTV_Data_ListInfor ));
				memcpy(sStatus.m_byPacket, &psFrame->m_byD[NP_UCOM_D1], NP_UCOM_SIZE_DTVLIST_INFO_MAX );
				
				pcMain->ReqEvent(new NPuCom_DTV_ListInfor(&sStatus));
			}break;
			
			case NP_UCOM_UNPACK_DTV_CMD_RESULT:	// TP=0xE3, d0-d1
			{
				NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
				
				NPuComDTV_Data_CmdResult sStatus;
				memset(&sStatus, 0, sizeof( NPuComDTV_Data_CmdResult ));
				memcpy(sStatus.m_byPacket, &psFrame->m_byD[NP_UCOM_D0], NP_UCOM_SIZE_DTVCMD_RESULT_MAX );
				
				pcMain->ReqEvent(new NPuCom_DTV_CmdResult(&sStatus));
			}break;

			case NP_UCOM_UNPACK_POWER_ON_OFF:	// TP=0xE4, d0-d1
			{
				NPuComHandler *pcMain = NPuComHandler::GetuComHandler();

				NPuComPowerOnOff sStatus;
				memset(&sStatus, 0, sizeof( NPuComPowerOnOff ));
				memcpy(sStatus.m_byPacket, &psFrame->m_byD[NP_UCOM_D0], NP_UCOM_SIZE_POWER_ON_OFF_MAX );

				pcMain->ReqEvent(new NPuComRcvPowerOnOff(&sStatus));
			}break;
			
			case NP_UCOM_UNPACK_TPED:			// TP=0xED, d0-d1
			{
				NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
				
				NPuComPacket_TPED iRxData;
				DWORD             iDataLen = sizeof( NPuComPacket_TPED );
				memset(&iRxData, 0, iDataLen );
				memcpy( iRxData.m_byPacket, &psFrame->m_byD[NP_UCOM_D0], iDataLen );
				
				pcMain->ReqEvent(new NPuComEvent_RX_TPED(&iRxData));
			}break;
			
			case NP_UCOM_UNPACK_TPD3:			// TP=0xD3, d0
			{
				NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
				
				NPuComPacket_TPD3 iRxData;
				DWORD             iDataLen = sizeof( NPuComPacket_TPD3 );
				memset(&iRxData, 0, iDataLen );
				memcpy( iRxData.m_byPacket, &psFrame->m_byD[NP_UCOM_D0], iDataLen );
				
				BYTE iByte = GetRxBitData( NP_UCOM_BD1 );
				if ( iByte & NP_UCOM_BIT1)
				{
					iRxData.m_byPacket[0] = 1;
				}
				else
				{
					iRxData.m_byPacket[0] = 0;
				}
				OutputDebugLog( "NPuComHandler-- TPD3.Notify=0x%02X (D0=0x%02X,BD1=0x%02X) ",
								iRxData.m_byPacket[0], psFrame->m_byD[NP_UCOM_D0], iByte );
				// NP_UCOM_PRINTF  --> by NP_UCOM_DEBUG, for PC debug
				pcMain->ReqEvent(new NPuComEvent_RX_TPD3(&iRxData));
			}break;
			case NP_UCOM_UNPACK_TPD5:			// TP=0xD5, d0
			{
				NPuComHandler *pcMain = NPuComHandler::GetuComHandler();

				NPuComPacket_TPD5 iRxData;
				DWORD             iDataLen = sizeof( NPuComPacket_TPD5 );
				memset(&iRxData, 0, iDataLen );
				memcpy( iRxData.m_byPacket, &psFrame->m_byD[NP_UCOM_D0], iDataLen );

				pcMain->ReqEvent(new NPuComEvent_RX_TPD5(&iRxData));
			}break;
			case NP_UCOM_UNPACK_INTERRUPT_SCREEN:			// TP=0xD6
			{
				NPuComHandler *pcMain = NPuComHandler::GetuComHandler();

				NPuComInterrputScreen iRxData;
				DWORD             iDataLen = sizeof( NPuComInterrputScreen );
				memset(&iRxData, 0, iDataLen );
				memcpy( iRxData.m_byPacket, &psFrame->m_byD[NP_UCOM_D0], iDataLen );

				pcMain->ReqEvent(new NPuComRcvInterrputScreen(&iRxData));
			}break;
			case NP_UCOM_UNPACK_PARK_PILOT_SCREEN:			// TP=0xD7
			{
				NPuComHandler *pcMain = NPuComHandler::GetuComHandler();

				NPuComParkPilotScreen iRxData;
				DWORD             iDataLen = sizeof( NPuComParkPilotScreen );
				memset(&iRxData, 0, iDataLen );
				memcpy( iRxData.m_byPacket, &psFrame->m_byD[NP_UCOM_D0], iDataLen );

				pcMain->ReqEvent(new NPuComRcvParkPilotScreen(&iRxData));
			}break;
			case NP_UCOM_UNPACK_STOP_NAVIGUIDE:			// TP=0xD8
			{
				NPuComHandler *pcMain = NPuComHandler::GetuComHandler();

				NPuComStopNaviGuide iRxData;
				DWORD             iDataLen = sizeof( NPuComStopNaviGuide );
				memset(&iRxData, 0, iDataLen );
				memcpy( iRxData.m_byPacket, &psFrame->m_byD[NP_UCOM_D0], iDataLen );

				pcMain->ReqEvent(new NPuComRcvStopNaviGuide(&iRxData));
			}break;
			case NP_UCOM_UNPACK_VA50:					// TP=0xD9
			{
				NPuComHandler *pcMain = NPuComHandler::GetuComHandler();

				NPuComVACommand50 iRxData;
				DWORD             iDataLen = sizeof( NPuComVACommand50 );
				memset(&iRxData, 0, iDataLen );
				iRxData.m_byScreenActivity = psFrame->m_byD[NP_UCOM_D0];
				iRxData.m_byCancellationNotification = psFrame->m_byD[NP_UCOM_D1];
				iRxData.m_byScreenRequest = psFrame->m_byD[NP_UCOM_D2];

				pcMain->ReqEvent(new NPuComRcvVACommand50(&iRxData));
			}break;
		default:
			break;
	}
}

/******************************************************************************

    ビットデータ設定処理

    【解説】
        ビットデータの設定を行う

*******************************************************************************/
VOID
NPuComPacker::SetRxBitData( DWORD dwNum, BYTE byData )
{
	m_cRcvBD.SetBitData(dwNum, byData);
}
/******************************************************************************

    ビットデータ設定処理

    【解説】
        ビットデータの設定を行う

*******************************************************************************/
VOID
NPuComPacker::SetTxBitData( DWORD dwNum, BYTE byData )
{
	m_cSndBD.SetBitData(dwNum, byData);
}

/******************************************************************************

    受信ビットデータ取得処理

    【解説】
        受信ビットデータの取得を行う

*******************************************************************************/
BYTE
NPuComPacker::GetRxBitData( DWORD dwNum )
{
	return m_cRcvBD.GetBitData(dwNum);
}

/******************************************************************************

    送信ビットデータ取得処理

    【解説】
        送信ビットデータの取得を行う

*******************************************************************************/
BYTE
NPuComPacker::GetTxBitData( DWORD dwNum )
{
	return m_cSndBD.GetBitData(dwNum);
}

/******************************************************************************

    受信ＢＤ取得関数

    【解説】
        最新の受信ＢＤの状態を取得する

*******************************************************************************/
NP_BOOL
NPuComPacker::GetReceiveBD( NPuComRcvBD *psBD )
{
	if (psBD==NULL)
	{
		return NP_FALSE;
	}
	
	NPAutoSync cSync(s_cSyncRxBD);
	
	// ＢＤ数の設定
	psBD->dwBDNum = NP_UCOM_SIZE_BD;
	// 最新のデータを取得
	psBD->byBD[NP_UCOM_BD1] = GetRxBitData(NP_UCOM_BD1);
	psBD->byBD[NP_UCOM_BD2] = GetRxBitData(NP_UCOM_BD2);
	psBD->byBD[NP_UCOM_BD3] = GetRxBitData(NP_UCOM_BD3);
	psBD->byBD[NP_UCOM_BD4] = GetRxBitData(NP_UCOM_BD4);
	psBD->byBD[NP_UCOM_BD5] = GetRxBitData(NP_UCOM_BD5);
	psBD->byBD[NP_UCOM_BD6] = GetRxBitData(NP_UCOM_BD6);
	psBD->byBD[NP_UCOM_BD7] = GetRxBitData(NP_UCOM_BD7);
	psBD->byBD[NP_UCOM_BD8] = GetRxBitData(NP_UCOM_BD8);
	psBD->byBD[NP_UCOM_BD9] = GetRxBitData(NP_UCOM_BD9);
	psBD->byBD[NP_UCOM_BD10] = GetRxBitData(NP_UCOM_BD10);

	return NP_TRUE;
}

/******************************************************************************

    ビットデータ設定関数

    【解説】
        マイコンへの通知が必要なビットデータを設定する

*******************************************************************************/
NP_BOOL
NPuComPacker::ChangeTxBitData( const DWORD dwId, const NP_BOOL bStatus )
{
	NP_BOOL bRet = NP_FALSE;
	NPAutoSync cSync(s_cSyncBD);

	// 番号の確認
	switch (dwId)
	{
	case NP_UCOM_CHANGEBD_ID_IPODVIDEO_REAR_MUTE:
		{
			BYTE byTmpBD = GetTxBitData(NP_UCOM_BD1);

			if (NP_UCOM_BD1_TX_IPODVIDEO_REAR_MUTE ==
			   (NP_UCOM_BD1_TX_IPODVIDEO_REAR_MUTE & byTmpBD))
			{
				if(NP_FALSE == bStatus)
				{
					// TRUE→FALSEに変化
					byTmpBD ^= NP_UCOM_BD1_TX_IPODVIDEO_REAR_MUTE;
					SetTxBitData(NP_UCOM_BD1, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
			else
			{
				if(NP_TRUE == bStatus)
				{
					// FALSE→TRUEに変化
					byTmpBD |= NP_UCOM_BD1_TX_IPODVIDEO_REAR_MUTE;
					SetTxBitData(NP_UCOM_BD1, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
		}		
		break;
	case NP_UCOM_CHANGEBD_ID_GOOGLE_VR:
		{
			BYTE byTmpBD = GetTxBitData(NP_UCOM_BD1);

			if (NP_UCOM_BD1_TX_GOOGLE_VR ==
			   (NP_UCOM_BD1_TX_GOOGLE_VR & byTmpBD))
			{
				if(NP_FALSE == bStatus)
				{
					// TRUE→FALSEに変化
					byTmpBD ^= NP_UCOM_BD1_TX_GOOGLE_VR;
					SetTxBitData(NP_UCOM_BD1, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
			else
			{
				if(NP_TRUE == bStatus)
				{
					// FALSE→TRUEに変化
					byTmpBD |= NP_UCOM_BD1_TX_GOOGLE_VR;
					SetTxBitData(NP_UCOM_BD1, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
		}		
		break;		
	case NP_UCOM_CHANGEBD_ID_DVD_DIVX_PERMIT:
		{
			BYTE byTmpBD = GetTxBitData(NP_UCOM_BD2);

			if (NP_UCOM_BD2_TX_DVD_DIVX_PERMIT ==
			   (NP_UCOM_BD2_TX_DVD_DIVX_PERMIT & byTmpBD))
			{
				if(NP_FALSE == bStatus)
				{
					// TRUE→FALSEに変化
					byTmpBD ^= NP_UCOM_BD2_TX_DVD_DIVX_PERMIT;
					SetTxBitData(NP_UCOM_BD2, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
			else
			{
				if(NP_TRUE == bStatus)
				{
					// FALSE→TRUEに変化
					byTmpBD |= NP_UCOM_BD2_TX_DVD_DIVX_PERMIT;
					SetTxBitData(NP_UCOM_BD2, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
		}		
		break;
	case NP_UCOM_CHANGEBD_ID_JPEG_CAPTURE:
		{
			BYTE byTmpBD = GetTxBitData(NP_UCOM_BD2);

			if (NP_UCOM_BD2_TX_JPEG_CAPTURE_ING ==
			   (NP_UCOM_BD2_TX_JPEG_CAPTURE_ING & byTmpBD))
			{
				if(NP_FALSE == bStatus)
				{
					// TRUE→FALSEに変化
					byTmpBD ^= NP_UCOM_BD2_TX_JPEG_CAPTURE_ING;
					SetTxBitData(NP_UCOM_BD2, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
			else
			{
				if(NP_TRUE == bStatus)
				{
					// FALSE→TRUEに変化
					byTmpBD |= NP_UCOM_BD2_TX_JPEG_CAPTURE_ING;
					SetTxBitData(NP_UCOM_BD2, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
		}		
		break;
/*	case NP_UCOM_CHANGEBD_ID_WIREDKEY_LEARNING_STATUS:
		{
			BYTE byTmpBD = GetTxBitData(NP_UCOM_BD4);

			if (NP_UCOM_BD4_TX_WIREDKEY_LEARNING_STATUS ==
			   (NP_UCOM_BD4_TX_WIREDKEY_LEARNING_STATUS & byTmpBD))
			{
				if(NP_FALSE == bStatus)
				{
					// TRUE→FALSEに変化
					byTmpBD ^= NP_UCOM_BD4_TX_WIREDKEY_LEARNING_STATUS;
					SetTxBitData(NP_UCOM_BD4, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
			else
			{
				if(NP_TRUE == bStatus)
				{
					// FALSE→TRUEに変化
					byTmpBD |= NP_UCOM_BD4_TX_WIREDKEY_LEARNING_STATUS;
					SetTxBitData(NP_UCOM_BD4, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
		}		
		break;*/
/*	case NP_UCOM_CHANGEBD_ID_BACKCAMERA_DISP_STATUS:
		{
			BYTE byTmpBD = GetTxBitData(NP_UCOM_BD4);

			if (NP_UCOM_BD4_TX_BACKCAMERA_DISP_STATUS ==
			   (NP_UCOM_BD4_TX_BACKCAMERA_DISP_STATUS & byTmpBD))
			{
				if(NP_FALSE == bStatus)
				{
					// TRUE→FALSEに変化
					byTmpBD ^= NP_UCOM_BD4_TX_BACKCAMERA_DISP_STATUS;
					SetTxBitData(NP_UCOM_BD4, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
			else
			{
				if(NP_TRUE == bStatus)
				{
					// FALSE→TRUEに変化
					byTmpBD |= NP_UCOM_BD4_TX_BACKCAMERA_DISP_STATUS;
					SetTxBitData(NP_UCOM_BD4, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
		}		
		break;*/
	case NP_UCOM_CHANGEBD_ID_SECURITY_LOCK:
		{
			BYTE byTmpBD = GetTxBitData(NP_UCOM_BD4);

			if (NP_UCOM_BD4_TX_SECURITY_LOCK ==
			   (NP_UCOM_BD4_TX_SECURITY_LOCK & byTmpBD))
			{
				if(NP_FALSE == bStatus)
				{
					// TRUE→FALSEに変化
					byTmpBD ^= NP_UCOM_BD4_TX_SECURITY_LOCK;
					SetTxBitData(NP_UCOM_BD4, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
			else
			{
				if(NP_TRUE == bStatus)
				{
					// FALSE→TRUEに変化
					byTmpBD |= NP_UCOM_BD4_TX_SECURITY_LOCK;
					SetTxBitData(NP_UCOM_BD4, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
		}		
		break;
	case NP_UCOM_CHANGEBD_ID_DEMO_DISP_STATUS:
		{
			BYTE byTmpBD = GetTxBitData(NP_UCOM_BD4);

			if (NP_UCOM_BD4_TX_DEMO_DISP_STATUS ==
			   (NP_UCOM_BD4_TX_DEMO_DISP_STATUS & byTmpBD))
			{
				if(NP_FALSE == bStatus)
				{
					// TRUE→FALSEに変化
					byTmpBD ^= NP_UCOM_BD4_TX_DEMO_DISP_STATUS;
					SetTxBitData(NP_UCOM_BD4, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
			else
			{
				if(NP_TRUE == bStatus)
				{
					// FALSE→TRUEに変化
					byTmpBD |= NP_UCOM_BD4_TX_DEMO_DISP_STATUS;
					SetTxBitData(NP_UCOM_BD4, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
		}		
		break;
	case NP_UCOM_CHANGEBD_ID_DEMO_SETTING_STATUS:
		{
			BYTE byTmpBD = GetTxBitData(NP_UCOM_BD4);

			if (NP_UCOM_BD4_TX_DEMO_SETTING_STATUS ==
			   (NP_UCOM_BD4_TX_DEMO_SETTING_STATUS & byTmpBD))
			{
				if(NP_FALSE == bStatus)
				{
					// TRUE→FALSEに変化
					byTmpBD ^= NP_UCOM_BD4_TX_DEMO_SETTING_STATUS;
					SetTxBitData(NP_UCOM_BD4, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
			else
			{
				if(NP_TRUE == bStatus)
				{
					// FALSE→TRUEに変化
					byTmpBD |= NP_UCOM_BD4_TX_DEMO_SETTING_STATUS;
					SetTxBitData(NP_UCOM_BD4, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
		}		
		break;
	case NP_UCOM_CHANGEBD_ID_MENU_DISP_STATUS:
		{
			BYTE byTmpBD = GetTxBitData(NP_UCOM_BD4);

			if (NP_UCOM_BD4_TX_MENU_DISP_STATUS ==
			   (NP_UCOM_BD4_TX_MENU_DISP_STATUS & byTmpBD))
			{
				if(NP_FALSE == bStatus)
				{
					// TRUE→FALSEに変化
					byTmpBD ^= NP_UCOM_BD4_TX_MENU_DISP_STATUS;
					SetTxBitData(NP_UCOM_BD4, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
			else
			{
				if(NP_TRUE == bStatus)
				{
					// FALSE→TRUEに変化
					byTmpBD |= NP_UCOM_BD4_TX_MENU_DISP_STATUS;
					SetTxBitData(NP_UCOM_BD4, byTmpBD);
					bRet = NP_TRUE;
					break;
				}
			}
		}		
		break;		
	default:
		break;
	}
	return bRet;
}
/******************************************************************************

    ビットデータ設定関数

    【解説】
        マイコンへの通知が必要なビットデータを設定する

*******************************************************************************/
NP_BOOL
NPuComPacker::ChangeTxBitData( const DWORD dwId, const BYTE bStatus )
{
	NP_BOOL bRet = NP_FALSE;
	NPAutoSync cSync(s_cSyncBD);

	// 番号の確認
	switch (dwId)
	{
	case NP_UCOM_CHANGEBD_ID_APPMODE_INFO:
		{
			BYTE byTmpBD = GetTxBitData(NP_UCOM_BD1);
			BYTE TempValue;

			TempValue = ((bStatus<<1)&NP_UCOM_BD1_TX_APPMODE_INFO);

			if (TempValue !=(NP_UCOM_BD1_TX_APPMODE_INFO & byTmpBD))
			{
				byTmpBD |= NP_UCOM_BD1_TX_APPMODE_INFO;
				byTmpBD ^= NP_UCOM_BD1_TX_APPMODE_INFO;
				byTmpBD |= TempValue;
				SetTxBitData(NP_UCOM_BD1, byTmpBD);
				bRet = NP_TRUE;
			}
		}		
		break;		
	case NP_UCOM_CHANGEBD_ID_TV_SIGNAL_OUTPUT:
		{
			BYTE byTmpBD = GetTxBitData(NP_UCOM_BD3);
			BYTE TempValue;

			TempValue = ((bStatus<<6)&NP_UCOM_BD3_TX_TV_SIGNAL_OUTPUT);

			if (TempValue !=(NP_UCOM_BD3_TX_TV_SIGNAL_OUTPUT & byTmpBD))
			{
				byTmpBD |= NP_UCOM_BD3_TX_TV_SIGNAL_OUTPUT;
				byTmpBD ^= NP_UCOM_BD3_TX_TV_SIGNAL_OUTPUT;
				byTmpBD |= TempValue;
				SetTxBitData(NP_UCOM_BD3, byTmpBD);
				bRet = NP_TRUE;
			}
		}		
		break;
	case NP_UCOM_CHANGEBD_ID_WIREDKEY_LEARNING_STATUS:
		{
			BYTE byTmpBD = GetTxBitData(NP_UCOM_BD4);
			BYTE TempValue;

			TempValue = ((bStatus<<4)&NP_UCOM_BD4_TX_WIREDKEY_LEARNING_STATUS);

			if (TempValue !=(NP_UCOM_BD4_TX_WIREDKEY_LEARNING_STATUS & byTmpBD))
			{
				byTmpBD |= NP_UCOM_BD4_TX_WIREDKEY_LEARNING_STATUS;
				byTmpBD ^= NP_UCOM_BD4_TX_WIREDKEY_LEARNING_STATUS;
				byTmpBD |= TempValue;
				SetTxBitData(NP_UCOM_BD4, byTmpBD);
				bRet = NP_TRUE;
			}
		}		
		break;
	case NP_UCOM_CHANGEBD_ID_LEARNING_KEY_CARTYPE_DB4_76:
		{
			BYTE byTmpBD = GetTxBitData(NP_UCOM_BD4);
			BYTE TempValue;

			TempValue = ((bStatus<<6)&NP_UCOM_BD4_TX_LEARNING_KEY_CARTYPE);

			if (TempValue !=(NP_UCOM_BD4_TX_LEARNING_KEY_CARTYPE & byTmpBD))
			{
				byTmpBD |= NP_UCOM_BD4_TX_LEARNING_KEY_CARTYPE;
				byTmpBD ^= NP_UCOM_BD4_TX_LEARNING_KEY_CARTYPE;
				byTmpBD |= TempValue;
				SetTxBitData(NP_UCOM_BD4, byTmpBD);
				bRet = NP_TRUE;
			}
		}		
		break;		
		
	default:
		break;
	}
	return bRet;
}

/******************************************************************************

    ビットデータ設定関数

    【解説】
        マイコンへの通知が必要なビットデータを設定する

*******************************************************************************/
NP_BOOL
NPuComPacker::ChangeTxBitData(PBYTE pbyChgBD)
{
	if (NULL==pbyChgBD)
	{
		return NP_FALSE;
	}

	NPAutoSync cSync(s_cSyncBD);
	// デフォルト設定(送信BD)
	SetTxBitData(NP_UCOM_BD1, pbyChgBD[NP_UCOM_BD1]);
	SetTxBitData(NP_UCOM_BD2, pbyChgBD[NP_UCOM_BD2]);
	SetTxBitData(NP_UCOM_BD3, pbyChgBD[NP_UCOM_BD3]);
	SetTxBitData(NP_UCOM_BD4, pbyChgBD[NP_UCOM_BD4]);
	SetTxBitData(NP_UCOM_BD5, pbyChgBD[NP_UCOM_BD5]);
	SetTxBitData(NP_UCOM_BD6, pbyChgBD[NP_UCOM_BD6]);
	SetTxBitData(NP_UCOM_BD7, pbyChgBD[NP_UCOM_BD7]);
	SetTxBitData(NP_UCOM_BD8, pbyChgBD[NP_UCOM_BD8]);
	SetTxBitData(NP_UCOM_BD9, pbyChgBD[NP_UCOM_BD9]);
	SetTxBitData(NP_UCOM_BD10, pbyChgBD[NP_UCOM_BD10]);

	return NP_TRUE;
}


/******************************************************************************

    key event type judge処理関数

    【解説】
       Decide which key event occured , according to BitData from system.

*******************************************************************************/
void NPuComPacker::CheckKeyLevel(ST_KEY_DATA *keystatus,unsigned short *key_code,unsigned short *key_type)
{
	if(keystatus->key_sts.hardkey_code != D_KEY_RELEASE )
	{
		*key_type = D_KEYTYPE_HARDKEY;						/* キー種別は本体キー			*/
		*key_code = keystatus->key_sts.hardkey_code;		/* 受信したキーコードを設定		*/
	}
	else if(keystatus->key_sts.rotary_code != D_KEY_RELEASE )
	{
		*key_type = D_KEYTYPE_HARDKEY;
		*key_code = keystatus->key_sts.rotary_code;
	}
	/*----------------------------------------------------------------------------------------------*/
	/*	ワイヤードリモコンキーの状態を確認															*/
	/*----------------------------------------------------------------------------------------------*/
	else if(keystatus->key_sts.wired_code != D_KEY_RELEASE )
	{
		*key_type = D_KEYTYPE_WIREDKEY;						/* キー種別はワイヤードキー		*/
		*key_code = keystatus->key_sts.wired_code;		/* 受信したキーコードを設定		*/
	}
	/*----------------------------------------------------------------------------------------------*/
	/*	リモコンキーの状態を確認																	*/
	/*----------------------------------------------------------------------------------------------*/
	else if(keystatus->key_sts.remocon_code != 0x0000 )
	{
		*key_type = D_KEYTYPE_REMOKEY;						/* キー種別はワイヤードキー		*/
		*key_code =keystatus->key_sts.remocon_code;		/* 受信したキーコードを設定		*/
	}
	/*----------------------------------------------------------------------------------------------*/
	/*	上記以外の場合は押され無しと判断															*/
	/*----------------------------------------------------------------------------------------------*/
	else
	{
		*key_type = D_KEYTYPE_RELEASE;						/* キー種別は無し				*/
		*key_code = keystatus->key_code;
	}
}

/******************************************************************************

    ビットデータ設定関数

    【解説】
        マイコンへの通知が必要なビットデータを設定する

*******************************************************************************/
NP_BOOL
NPuComPacker::CheckReceiveBD( const NPuComDataFrame *psFrame )
{
	NP_BOOL bChg  = NP_FALSE;			// BD変化フラグ
	BYTE pbyChgBD[NP_UCOM_SIZE_BD];

	// 確保成否と初期化
	memset(pbyChgBD, 0, NP_UCOM_SIZE_BD);

	if( NP_FALSE != m_blBitNotifyStarted) {
		// 変化の確認
		for(DWORD dwBDCnt = 0; dwBDCnt < NP_UCOM_SIZE_BD; ++dwBDCnt){
			BYTE TmpBD1 = GetRxBitData(dwBDCnt);
			BYTE TmpBD2 = psFrame->m_byBD[NP_UCOM_BD1 + dwBDCnt];
			pbyChgBD[dwBDCnt] = TmpBD1 ^ TmpBD2; 
			if(NP_UCOM_NODATA != pbyChgBD[dwBDCnt]){
				bChg = NP_TRUE;
			}
		}
	}
	else {
		// 初期起動時はすべての通知を行うため、全ビット変化ありと判定します。
		for(DWORD dwBDCnt = 0; dwBDCnt < NP_UCOM_SIZE_BD; ++dwBDCnt){
			pbyChgBD[dwBDCnt] = 0xFF;
		}
		bChg = NP_TRUE;
		// 次回から通常判定になります。
		m_blBitNotifyStarted = NP_TRUE;
	}

	{
		NPAutoSync cSync(s_cSyncKey);
		if (m_bDetectWiredKeyPress!=FALSE)
		{
			if (psFrame->m_byBD[NP_UCOM_BD8] != D_KEY_RELEASE )
			{
				m_bWiredKeyPress = TRUE;
			}
		}
	}

	// key event get
	{
		UN_RCV_BIT_DATA *bitdata;	
		UN_RCV_BIT_DATA *last_bitdata;
		BYTE tmpbitdata[NP_UCOM_SIZE_BD];
		
		bitdata=(UN_RCV_BIT_DATA *)(&psFrame->m_byBD[NP_UCOM_BD1]);
		
		// 確保初期化
		memset(tmpbitdata, 0, NP_UCOM_SIZE_BD);
		for(DWORD dwBDCnt = 0; dwBDCnt < NP_UCOM_SIZE_BD; dwBDCnt++)
		{
			tmpbitdata[dwBDCnt] = GetRxBitData(dwBDCnt);
		}
		last_bitdata=(UN_RCV_BIT_DATA *)tmpbitdata;

		if(
		   (bitdata->bit.rotary   !=last_bitdata->bit.rotary  ) ||
		   (bitdata->bit.hardkey  !=last_bitdata->bit.hardkey ) ||
		   (bitdata->bit.wiredkey !=last_bitdata->bit.wiredkey) ||
		   (bitdata->bit.remo_1st !=last_bitdata->bit.remo_1st) ||
		   (bitdata->bit.remo_2nd !=last_bitdata->bit.remo_2nd)
		  )
		{
			DWORD dwKeyCode;
			ST_KEY_DATA	key_data;					/* キーデータ						*/
			key_data.key_sts.rotary_code    = bitdata->bit.rotary;
			key_data.key_sts.hardkey_code   = bitdata->bit.hardkey;         /* 本体キーコード        */
			key_data.key_sts.wired_code     = bitdata->bit.wiredkey;        /* ワイヤードキーコード  */
			key_data.key_sts.remocon_code   = ( bitdata->bit.remo_1st << 8 ) +
											  ( bitdata->bit.remo_2nd );    /* リモコンキーコード    */
			
			CheckKeyLevel(&key_data, &key_data.key_code, &key_data.key_type);
			switch (key_data.key_type)
			{
				case D_KEYTYPE_HARDKEY:
					dwKeyCode=key_data.key_code+D_KEYTYPE_HARDKEY_VALUE;
					break;
				case D_KEYTYPE_WIREDKEY:
					dwKeyCode=key_data.key_code+D_KEYTYPE_WIREDKEY_VALUE;
					break;
				case D_KEYTYPE_REMOKEY:
					dwKeyCode=key_data.key_code+D_KEYTYPE_REMOTEKEY_VALUE;
					break;
				case D_KEYTYPE_RELEASE:
				default:
					dwKeyCode=0;
					break;
			}
			
			// Keyハンドラに通知
			{
				NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
				if(NP_FALSE != pcMain->GetKeyTouchValidStatus()) 
				{
					pcMain->ReqCallback(NP_UCOMHANDLER_CALLBACK_UCOMKEY, &dwKeyCode, sizeof(dwKeyCode));
				}
				else
				{
					OutputDebugLog("NPuComPacker  receive key , but not handle -- Suspending key forbidden\n");
				}
			}
		}		
	}

	// 変化時の処理を行なう
	if (FALSE != bChg) 
	{
		{
			// 今回のビットデータを保存
			NPAutoSync cSync(s_cSyncRxBD);
			
			SetRxBitData(NP_UCOM_BD1, psFrame->m_byBD[NP_UCOM_BD1]);
			SetRxBitData(NP_UCOM_BD2, psFrame->m_byBD[NP_UCOM_BD2]);
			SetRxBitData(NP_UCOM_BD3, psFrame->m_byBD[NP_UCOM_BD3]);
			SetRxBitData(NP_UCOM_BD4, psFrame->m_byBD[NP_UCOM_BD4]);
			SetRxBitData(NP_UCOM_BD5, psFrame->m_byBD[NP_UCOM_BD5]);
			SetRxBitData(NP_UCOM_BD6, psFrame->m_byBD[NP_UCOM_BD6]);
			SetRxBitData(NP_UCOM_BD7, psFrame->m_byBD[NP_UCOM_BD7]);
			SetRxBitData(NP_UCOM_BD8, psFrame->m_byBD[NP_UCOM_BD8]);
			SetRxBitData(NP_UCOM_BD9, psFrame->m_byBD[NP_UCOM_BD9]);
			SetRxBitData(NP_UCOM_BD10, psFrame->m_byBD[NP_UCOM_BD10]);
		}
		{	
			// BitData変化イベント通知開始
			NPuComRcvChangeBD sBDChg;
			// 構造体の初期化
			memset(&sBDChg, 0, sizeof(sBDChg));
			// ビットデータ数の設定
			sBDChg.dwBDNum = NP_UCOM_SIZE_BD;
			// ビットデータ変更フラグの設定
			memcpy(&sBDChg.byBDChg, pbyChgBD, NP_UCOM_SIZE_BD);
			// 現在のビットデータの設定
			memcpy(&sBDChg.byBD, &psFrame->m_byBD[NP_UCOM_BD1], NP_UCOM_SIZE_BD);
			// アプリに通知
			{
				NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
				pcMain->ReqEvent(new NPuComRcvBitDataChgEvent(&sBDChg));
			}
		}
	}

	return NP_TRUE;
}

/******************************************************************************

    初期通信結果取得

    【解説】
        初期通信結果の取得を行う

*******************************************************************************/
VOID
NPuComPacker::GetConeChkStatus( NPConeChkStatus *pcStatus )
{
	if(NULL != pcStatus){
		memcpy(pcStatus, &m_sConeChk, sizeof(NPConeChkStatus));
	}
}

/******************************************************************************

    初期通信送信時処理

    【解説】
        初期通信（接続確認コマンド）送信時の処理を実装する

******************************************************************************/
VOID
NPuComPacker::OnSendConeChkReq( VOID )
{
	NPuComTimer *pcTimer = 	NPuComTimer::Instance();

	// 初期通信応答待ちフラグをセットする
	if( NULL != pcTimer) {
		pcTimer->MergeStatus(NP_UCOM_STATUS_CONECHK_WAIT_RES);
	}
}

/******************************************************************************

    初期通信受信時処理

    【解説】
        初期通信（接続応答コマンド）受信時の処理を実装する

******************************************************************************/
VOID
NPuComPacker::OnReceiveConeChkRes( VOID )
{
	NPuComTimer *pcTimer = 	NPuComTimer::Instance();
	if( NULL == pcTimer) {
		return;
	}

	if(NP_UCOM_STATUS_CONECHK_WAIT_RES == (NP_UCOM_STATUS_CONECHK_WAIT_RES & pcTimer->GetStatus())){
		// タイマ停止
		pcTimer->StopTimer(NP_UCOM_TIMER_ID_CONECHK);

		// 初期通信応答受信フラグをセットする
		pcTimer->MergeStatus(NP_UCOM_STATUS_CONECHK_RCV_RES);

		pcTimer->ClearStatus(NP_UCOM_STATUS_CONECHK_WAIT_RES);

		// 仕向とかの判別＆設定
		CheckModel();

		// 完了通知
		{
			// set BOFF reset flag
			BYTE byBOFFResetStatus = (m_sConeChk.byBootStatus&BOFF_RESET_MASK);
			if( BOFF_RESTARTMODE == byBOFFResetStatus ) {
				property_set(NP_PROPERTY_BOFF_STATUS, "true");
			}
			else if(ACCOFF_RESTARTMODE == byBOFFResetStatus){
				property_set(NP_PROPERTY_BOFF_STATUS, "false");
			}
			else if(ERROR_RESTARTMODE == byBOFFResetStatus){
				property_set(NP_PROPERTY_BOFF_STATUS, "errorcause");
			}
			else if(BAT_ON_RESTARTMODE == byBOFFResetStatus){
				property_set(NP_PROPERTY_BOFF_STATUS, "bat-on");
			}
			else{
			}

			NPuComHandler *pcMain = NPuComHandler::GetuComHandler();
			m_sConeChk.bResult = NP_TRUE;
			pcMain->EndConeChk(NP_TRUE);
		}
	}
}

/******************************************************************************

    仕向確認処理

    【解説】仕向の確認と設定を実装する

******************************************************************************/
VOID
NPuComPacker::CheckModel( VOID )
{
	// 仕向の判別
	switch (m_sConeChk.byModel1)
	{
		case NP_SYSINFO_MODEL1_KM613:
		case NP_SYSINFO_MODEL1_NX356:
		case NP_SYSINFO_MODEL1_NX406:
		case NP_SYSINFO_MODEL1_NX407:
		case NP_SYSINFO_MODEL1_NX357:
		case NP_SYSINFO_MODEL1_NX355:
        case NP_SYSINFO_MODEL1_NX454:
        case NP_SYSINFO_MODEL1_NX490:
			break;
		default:
			// 不明
			m_sConeChk.byModel1 = NP_SYSINFO_MODEL1_UNKNOWN;
			break;
	}

	// 仕向の判別
	switch (m_sConeChk.WarpCatchModel)
	{
		case NP_SYSINFO_MODEL1_KM613:
		case NP_SYSINFO_MODEL1_NX356:
		case NP_SYSINFO_MODEL1_NX406:
		case NP_SYSINFO_MODEL1_NX407:
		case NP_SYSINFO_MODEL1_NX357:
		case NP_SYSINFO_MODEL1_NX355:
        case NP_SYSINFO_MODEL1_NX454:
        case NP_SYSINFO_MODEL1_NX490:
			break;
		default:
			// 不明
			m_sConeChk.WarpCatchModel = NP_SYSINFO_MODEL1_UNKNOWN;
			break;
	}
	
	switch (m_sConeChk.byModel2)
	{
		case NP_SYSINFO_MODEL2_EW5:
		case NP_SYSINFO_MODEL2_UW5:
		case NP_SYSINFO_MODEL2_UC:
		case NP_SYSINFO_MODEL2_UW:
		case NP_SYSINFO_MODEL2_RC:
		case NP_SYSINFO_MODEL2_RD:
		case NP_SYSINFO_MODEL2_RI:
		case NP_SYSINFO_MODEL2_CN:
		case NP_SYSINFO_MODEL2_ES:
		case NP_SYSINFO_MODEL2_BR:
		case NP_SYSINFO_MODEL2_J:
		case NP_SYSINFO_MODEL2_ID:
		case NP_SYSINFO_MODEL2_AU:
		case NP_SYSINFO_MODEL2_EU:
		case NP_SYSINFO_MODEL2_EU5:
		case NP_SYSINFO_MODEL2_UR:
			break;
		default:
			// 不明
			m_sConeChk.byModel2 = NP_SYSINFO_MODEL2_UNKNOWN;
			break;	
	}
	
	{
       if (NPWarp::STATUS_NORMAL_BOOT_CATCH_SNAPSHOT != NPWarp::Instance()->GetBootMode()) {
        	EN_DATA_TYPE setvalue_type;

            if (m_sConeChk.byModel2==NP_SYSINFO_MODEL2_UC)
            {
                if (
                    ( m_sConeChk.byModel1 == NP_SYSINFO_MODEL1_KM613 ) ||
                    ( m_sConeChk.byModel1 == NP_SYSINFO_MODEL1_NX356 ) ||
                    ( m_sConeChk.byModel1 == NP_SYSINFO_MODEL1_NX355 ) ||
                    ( m_sConeChk.byModel1 == NP_SYSINFO_MODEL1_NX454 )
                   )
                {
                    setvalue_type = EN_DATA_TYPE_1;
                }
                else if (
                         ( m_sConeChk.byModel1 == NP_SYSINFO_MODEL1_NX357 ) ||
                         ( m_sConeChk.byModel1 == NP_SYSINFO_MODEL1_NX407 ) ||
                         ( m_sConeChk.byModel1 == NP_SYSINFO_MODEL1_NX490 )
                        )
                {
                    setvalue_type = EN_DATA_TYPE_3;
                }
                else
                {
                    setvalue_type = EN_DATA_TYPE_1;
                }
            }
            else
            {
                if (
                    ( m_sConeChk.byModel1 == NP_SYSINFO_MODEL1_KM612 ) ||
                    ( m_sConeChk.byModel1 == NP_SYSINFO_MODEL1_NX356 ) ||
                    ( m_sConeChk.byModel1 == NP_SYSINFO_MODEL1_NX355 ) ||
                    ( m_sConeChk.byModel1 == NP_SYSINFO_MODEL1_NX454 )
                   )
                {
                    setvalue_type = EN_DATA_TYPE_2;
                }
                else if (
                         ( m_sConeChk.byModel1 == NP_SYSINFO_MODEL1_NX357 ) ||
                         ( m_sConeChk.byModel1 == NP_SYSINFO_MODEL1_NX407 ) ||
                         ( m_sConeChk.byModel1 == NP_SYSINFO_MODEL1_NX490 )
                        )
                {
                    setvalue_type = EN_DATA_TYPE_4;
                }
                else
                {
                    setvalue_type = EN_DATA_TYPE_2;
                }
            }
        		
        	SetValueStore *pSetvalue=SetValueStore::Instance();
        	if (pSetvalue!=NULL)
        	{
        		pSetvalue->CorrectSetValueErr(setvalue_type);
        	}
       }
	}
	
	{
		PRINTF_USED_STACK("NPuComPacker.cpp", __LINE__);
		// 共有メモリに設定
		NPSysInfoSet cSet;
		NPSysInfoKind info;
		PRINTF_USED_STACK("NPuComPacker.cpp", __LINE__);
	
		info.Model1=m_sConeChk.byModel1;
		info.Model2=m_sConeChk.byModel2;
		info.WarpCatchModel      = m_sConeChk.WarpCatchModel;
		info.video_output_type   = m_sConeChk.video_output_type;
		info.divx_license_id[0]  = m_sConeChk.divx_license_id[0];
		info.divx_license_id[1]  = m_sConeChk.divx_license_id[1];
		info.firmware_version[0] = m_sConeChk.firmware_version[0];
		info.firmware_version[1] = m_sConeChk.firmware_version[1];
		info.firmware_version[2] = m_sConeChk.firmware_version[2];
		info.hardware_version[0] = m_sConeChk.hardware_version[0];
		info.hardware_version[1] = m_sConeChk.hardware_version[1];
		info.hardware_version[2] = m_sConeChk.hardware_version[2];
		memcpy(info.model_number, m_sConeChk.model_number, 12);
		cSet.SetModelID(info);
	}
}

/******************************************************************************

	Reset Rx/Tx bitdata
    【解説】
		Reset Rx/Tx bitdata when resume

******************************************************************************/
VOID
NPuComPacker::ResetRxTxBitdata()
{
	OutputDebugLog("NPuComPacker::ResetRxTxBitdata()");
	// ビットデータクラスの初期化
	m_cRcvBD.Initialize();
	m_cSndBD.Initialize();

       m_blBitNotifyStarted = FALSE;
	// デフォルト設定(受信BD)
	SetRxBitData(NP_UCOM_BD1, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD1]);
	SetRxBitData(NP_UCOM_BD2, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD2]);
	SetRxBitData(NP_UCOM_BD3, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD3]);
	SetRxBitData(NP_UCOM_BD4, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD4]);
	SetRxBitData(NP_UCOM_BD5, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD5]);
	SetRxBitData(NP_UCOM_BD6, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD6]);
	SetRxBitData(NP_UCOM_BD7, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD7]);
	SetRxBitData(NP_UCOM_BD8, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD8]);
	SetRxBitData(NP_UCOM_BD9, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD9]);
	SetRxBitData(NP_UCOM_BD10, NP_UCOM_INI_BD_RX_TABLE[NP_UCOM_BD10]);
	// デフォルト設定(送信BD)
	SetTxBitData(NP_UCOM_BD1, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD1]);
	SetTxBitData(NP_UCOM_BD2, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD2]);
	SetTxBitData(NP_UCOM_BD3, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD3]);
	SetTxBitData(NP_UCOM_BD4, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD4]);
	SetTxBitData(NP_UCOM_BD5, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD5]);
	SetTxBitData(NP_UCOM_BD6, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD6]);
	SetTxBitData(NP_UCOM_BD7, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD7]);
	SetTxBitData(NP_UCOM_BD8, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD8]);
	SetTxBitData(NP_UCOM_BD9, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD9]);
	SetTxBitData(NP_UCOM_BD10, NP_UCOM_INI_BD_TX_TABLE[NP_UCOM_BD10]);

	// 構造体の初期化
	memset(&m_sConeChk, 0, sizeof(m_sConeChk));

	// 初期設定
	m_sConeChk.byModel1 = NP_SYSINFO_MODEL1_UNKNOWN;
	m_sConeChk.byModel2 = NP_SYSINFO_MODEL2_UNKNOWN;
	m_sConeChk.bResult  = NP_FALSE;
}

VOID 
NPuComPacker::EnableDetectLearningKey(BOOL bEnable)
{
	if (bEnable==FALSE)
	{
		NPAutoSync cSync(s_cSyncKey);
		m_bWiredKeyPress = FALSE;
		m_bDetectWiredKeyPress = FALSE;
	}
	else
	{
		BYTE wiredkeycode = D_KEY_RELEASE;
		{
			NPAutoSync cSync(s_cSyncRxBD);
			wiredkeycode = GetRxBitData(NP_UCOM_BD8);
		}
		{
			NPAutoSync cSync(s_cSyncKey);
			if (wiredkeycode != D_KEY_RELEASE )
			{
				m_bWiredKeyPress = TRUE;
			}
			else
			{
				m_bWiredKeyPress = FALSE;
			}
			m_bDetectWiredKeyPress = TRUE;
		}
	}
}

BOOL 
NPuComPacker::IsLearningKeyPush()
{
	NPAutoSync cSync(s_cSyncKey);
	return m_bWiredKeyPress;
}


/******************************************************************************

    マイコンタイマリトライカウントクラスコンストラクタ

    【解説】
        マイコンタイマリトライカウントクラスの初期化

*******************************************************************************/
NPuComBitData::NPuComBitData()
:	// 初期化リスト
	NObject(),
	m_pbyBD(NULL)
{
}

/******************************************************************************

    マイコンタイマリトライカウントクラスデストラクタ

    【解説】
        マイコンタイマリトライカウントクラスの終了

*******************************************************************************/
NPuComBitData::~NPuComBitData()
{
	if( NULL != m_pbyBD) {
		delete[] m_pbyBD;
		m_pbyBD = NULL;
	}
}

/******************************************************************************

    初期化処理

    【解説】
        メモリの生成を行う

*******************************************************************************/
VOID
NPuComBitData::Initialize(VOID)
{
	// メモリの生成
	m_pbyBD = new BYTE[NP_UCOM_SIZE_BD];
	// 配列の初期化
	for(DWORD dwLoop = 0; dwLoop < NP_UCOM_SIZE_BD; ++dwLoop){
		m_pbyBD[dwLoop] = 0;
	}
}

/******************************************************************************

    ビットデータ設定処理

    【解説】
        ビットデータの設定を行う

*******************************************************************************/
VOID
NPuComBitData::SetBitData( DWORD dwNum, BYTE byData )
{
	if(NP_UCOM_SIZE_BD > dwNum){
		// ビットデータ設定
		m_pbyBD[dwNum] = byData;
	}
}

/******************************************************************************

    ビットデータ取得処理

    【解説】
        ビットデータの取得を行う

*******************************************************************************/
BYTE
NPuComBitData::GetBitData( DWORD dwNum )
{
	BYTE byResult = 0;

	if(NP_UCOM_SIZE_BD > dwNum){
		// ビットデータ設定
		byResult = m_pbyBD[dwNum];
	}

	return byResult;
}


/******************************************************************************

    コンストラクタ

******************************************************************************/
NPuComDataUpdate::NPuComDataUpdate()
:	// 初期化リスト
	NObject(),
	m_pcMain(NPuComHandler::GetuComHandler()),
	m_pcMemUpdate(NULL),
	m_sSendData(),
	m_sPrgHead(),
	m_dwUpdateAddress(0),
	m_dwUpdateFileSize(0),
	m_dwUpdateOverSize(0),
	m_pbyUpdateBinary(NULL),
	m_byUpdateCheckSum(0),
	m_szPrgFilePath(),
	m_UpdateTypeID( D_VERSION_UP_ID_SYSTEM )
{
	// 構造体の初期化
	memset( &m_sSendData, 0, sizeof( m_sSendData));
	memset(&m_sPrgHead,			0,	sizeof(m_sPrgHead));

	// 配列の初期化
	memset(m_szPrgFilePath,		0,	sizeof(m_szPrgFilePath));

	// マイコン書換え用共有メモリクラスの生成
	m_pcMemUpdate	= new NPuComMemoryUpdate();
	// マイコン書換え用共有メモリの確保
	m_pcMemUpdate->Create();
}

/******************************************************************************

    デストラクタ

******************************************************************************/
NPuComDataUpdate::~NPuComDataUpdate()
{
	// マイコン書換え用共有メモリクラスの破棄
	if( NULL != m_pcMemUpdate ){
		m_pcMemUpdate->Destroy();
		delete m_pcMemUpdate;
		m_pcMemUpdate = NULL;
	}

	// メインスレッドポインタのクリア
	if(NULL != m_pcMain){
		m_pcMain = NULL;
	}
}

/******************************************************************************

    マイコン書き換えクラスインスタンス取得関数

    【解説】
        プロセスで唯一のインスタンスの取得（シングルトンパターン）
        インスタンスが未生成の場合のみ生成し，既にインスタンスが生成されて
        いる場合はそのインスタンスを返す．シングルトンパターンとなっている．

******************************************************************************/
NPuComDataUpdate
*NPuComDataUpdate::Instance( VOID )
{
	s_cSync.SynchronizeStart();
	
	if ( NULL == s_pcInstance ){
		s_pcInstance = new NPuComDataUpdate();
	}

	s_cSync.SynchronizeEnd();

	return s_pcInstance;
}

/******************************************************************************

    マイコン書き換えクラスインスタンス破棄関数

    【解説】
        インスタンスの破棄を行なう

******************************************************************************/
VOID
NPuComDataUpdate::Destroy( VOID )
{
	s_cSync.SynchronizeStart();

	if ( NULL != s_pcInstance ){
		delete s_pcInstance;
		s_pcInstance = NULL;
	}

	s_cSync.SynchronizeEnd();
}

/******************************************************************************

    マイコン書換え用パス設定

    【解説】
        マイコン書換え用パスを設定する

*******************************************************************************/
VOID
NPuComDataUpdate::SetUpdatePath(LPCVOID pData, DWORD UpdateTypeID )
{
	memset(m_szPrgFilePath,		0,	sizeof(m_szPrgFilePath));
	memcpy(m_szPrgFilePath, pData, sizeof(m_szPrgFilePath)-1); 
	
	m_UpdateTypeID = UpdateTypeID;
}

DWORD
NPuComDataUpdate::GetUpdateTypeID( VOID )
{
	return m_UpdateTypeID;
}

/******************************************************************************

    マイコン書き換え開始処理

    【解説】
        マイコン書き換え開始時の処理を実装する

******************************************************************************/
NP_BOOL
NPuComDataUpdate::StartUpdateReq( VOID )
{
	NPuComMemoryInfoUpdate	sUpdateInfo;

	// バッファの初期化
	memset(&sUpdateInfo, 0, sizeof(sUpdateInfo));
	m_pcMemUpdate->GetData(&sUpdateInfo, sizeof(sUpdateInfo));

	// 状態設定
	sUpdateInfo.dwStatus = NP_UCOM_UPDATE_MODE_READY;
	m_pcMemUpdate->SetDataNotify(&sUpdateInfo, sizeof(sUpdateInfo));

	NP_BOOL bResult = NP_FALSE;
	if ( m_UpdateTypeID == D_VERSION_UP_ID_WWR )
	{
		bResult = ReadyUpdate_ForWWR();
	}
	else
	{
		bResult	= ReadyUpdate();
	}

	if(NP_FALSE == bResult){		// 準備失敗

		NPuComTimer *pcTimer = NPuComTimer::Instance();
		if( ( NULL != pcTimer)
			&& ( NP_FALSE == pcTimer->IsAccOff())){
			OutputErrLog("NPuComPacker.cpp", __LINE__, AIL_ERROR_LOG_CODE_COMDEVICE_ERROR, "DroidAVN", "NPuComDataUpdate::StartUpdateReq error");
		}
		// 状態設定
		sUpdateInfo.dwStatus = NP_UCOM_UPDATE_MODE_READY_ERR;
		m_pcMemUpdate->SetDataNotify(&sUpdateInfo, sizeof(sUpdateInfo));
	}
	else{
		// 状態設定
		sUpdateInfo.dwStatus = NP_UCOM_UPDATE_MODE_START;
		m_pcMemUpdate->SetDataNotify(&sUpdateInfo, sizeof(sUpdateInfo));
	}

	return bResult;
}

/******************************************************************************

	マイコンアップデート準備関数

    【解説】
        バージョンファイルやバイナリファイルを確認してマイコン書換えの準備を行なう

*******************************************************************************/
NP_BOOL
NPuComDataUpdate::ReadyUpdate(VOID)
{
	NP_BOOL	bRet			= NP_TRUE;
	DWORD	dwRetReadSize	= 0;
	/* open file by PFileOperator */
	PFileServiceProxy cPrgFile;

	// プログラムファイルのオープン
	if (FALSE == cPrgFile.OpenFile(m_szPrgFilePath, "r")){
		bRet = NP_FALSE;
	}

	// ヘッダ部の読み込み
	if (FALSE == cPrgFile.Read(&m_sPrgHead, sizeof(m_sPrgHead), &dwRetReadSize))
	{
		NP_Printf("NPuComDataUpdate ReadyUpdate  file read failed");
		bRet = NP_FALSE;
	}
	else
	{
		NP_Printf("NPuComDataUpdate ReadyUpdate  file read success");
		// 読込サイズ確認
		if (sizeof(m_sPrgHead) != dwRetReadSize)
		{
			bRet = NP_FALSE;
		}
		else
		{
			int i;
			const char *file_str=NULL;
			unsigned char i_forprint=0;

			for (i=strlen(m_szPrgFilePath)-1;i>0;i--)
			{
				if ((m_szPrgFilePath[i]=='/')
					||(m_szPrgFilePath[i]=='\\'))
				{
					break;
				}
			}

			if (i>0)
			{
				if ( (0==strncmp(&m_szPrgFilePath[i+1], "PEQ518",6)) )
				{
					file_str=VERUP_SYS_ID_PEQ518;
					i_forprint=1;
				}
				else if ( (0==strncmp(&m_szPrgFilePath[i+1], "PEQ543",6)) )
				{
					file_str=VERUP_SYS_ID_PEQ543;
					i_forprint=2;
				}
				else if ( (0==strncmp(&m_szPrgFilePath[i+1], "PEQ544",6)) )
				{
					file_str=VERUP_SYS_ID_PEQ544;
					i_forprint=3;
				}
				else if ( (0==strncmp(&m_szPrgFilePath[i+1], "PEQ545",6)) )
				{
					file_str=VERUP_SYS_ID_PEQ545;
					i_forprint=4;
				}
				else if ( (0==strncmp(&m_szPrgFilePath[i+1], "PEQ546",6)) )
				{
					file_str=VERUP_SYS_ID_PEQ546;
					i_forprint=5;
				}
				else if ( (0==strncmp(&m_szPrgFilePath[i+1], "PEQ547",6)) )
				{
					file_str=VERUP_SYS_ID_PEQ547;
					i_forprint=6;
				}
				else if ( (0==strncmp(&m_szPrgFilePath[i+1], "PEQ548",6)) )
				{
					file_str=VERUP_SYS_ID_PEQ548;
					i_forprint=7;
				}
				else if ( (0==strncmp(&m_szPrgFilePath[i+1], "PEQ549",6)) )
				{
					file_str=VERUP_SYS_ID_PEQ549;
					i_forprint=8;
				}
				else if ( (0==strncmp(&m_szPrgFilePath[i+1], "PEQ550",6)) )
				{
					file_str=VERUP_SYS_ID_PEQ550;
					i_forprint=9;
				}
			}
			NP_Printf("NPuComDataUpdate ReadyUpdate 1 : %d(%s)", i_forprint, &m_szPrgFilePath[i+1]);

			if (file_str!=NULL)
			{
				if (memcmp(&m_sPrgHead.ver_id[0], file_str, strlen(file_str)) != 0)
				{
					bRet = NP_FALSE;
					NP_Printf("NPuComDataUpdate ReadyUpdate ERR13.actually: %c%c%c%c%c%c", m_sPrgHead.ver_id[0], m_sPrgHead.ver_id[1], m_sPrgHead.ver_id[2], m_sPrgHead.ver_id[3], m_sPrgHead.ver_id[4], m_sPrgHead.ver_id[5]);
					NP_Printf("NPuComDataUpdate ReadyUpdate ERR13.expected: %c%c%c%c%c%c", file_str[0], file_str[1], file_str[2], file_str[3], file_str[4], file_str[5]);
					i_forprint = (i_forprint & 0x0F) + 0x10;
				}
				else
				{
					// アップデート用変数の設定
					m_dwUpdateFileSize = (m_sPrgHead.dat_size[3]<<24)
										|(m_sPrgHead.dat_size[2]<<16)
										|(m_sPrgHead.dat_size[1]<<8)
										|(m_sPrgHead.dat_size[0]);

					if((m_dwUpdateFileSize>(cPrgFile.GetFileSize() - dwRetReadSize))
						||(m_dwUpdateFileSize>VERUP_DATA_SYS_MAXSIZE)
						||(m_dwUpdateFileSize == 0))
					{
						bRet = NP_FALSE;
						i_forprint = (i_forprint & 0x0F) + 0x20;
					}
					else
					{
						m_dwUpdateAddress	= 0;
						m_dwUpdateOverSize	= 0;
						i_forprint = (i_forprint & 0x0F) + 0x30;
					}
				}
			}
			else
			{
				bRet = NP_FALSE;
				i_forprint = (i_forprint & 0x0F) + 0x40;
			}

			NP_Printf("NPuComDataUpdate ReadyUpdate 2 : %d", i_forprint);
		}
	}
	// アップデート用メモリ領域の確保
	if(NP_FALSE != bRet)
	{
		m_pbyUpdateBinary = new BYTE[m_dwUpdateFileSize];
		if(NULL == m_pbyUpdateBinary){
			bRet = NP_FALSE;
		}
	}
	// データ部の読み込み
	if(NP_FALSE != bRet)
	{
		INT count, index;
		DWORD rest_size;

		count=(m_dwUpdateFileSize/NP_UCOM_READ_SIZE_MAX);
		rest_size = m_dwUpdateFileSize-(count*NP_UCOM_READ_SIZE_MAX);

		for (index=0; index<count; index++)
		{
			if(FALSE == cPrgFile.Read(m_pbyUpdateBinary+(NP_UCOM_READ_SIZE_MAX*index), NP_UCOM_READ_SIZE_MAX, &dwRetReadSize))
			{
				bRet = NP_FALSE;
				break;
			}
			else
			{
				if (NP_UCOM_READ_SIZE_MAX != dwRetReadSize)
				{
					bRet = NP_FALSE;
					break;
				}
			}
		}
		
		if ((NP_FALSE != bRet)
			&&(rest_size>0))
		{
			if(FALSE == cPrgFile.Read(m_pbyUpdateBinary+(NP_UCOM_READ_SIZE_MAX*count), rest_size, &dwRetReadSize))
			{
				bRet = NP_FALSE;
			}
			else
			{
				if (rest_size != dwRetReadSize)
				{
					bRet = NP_FALSE;
				}
			}
		}
	}

	// ここまででエラーがある場合
	if (NP_FALSE == bRet)
	{
		cPrgFile.CloseFile();
		if (NULL != m_pbyUpdateBinary)
		{
			delete[] m_pbyUpdateBinary;
			m_pbyUpdateBinary = NULL;
		}
		NP_Printf("NPuComDataUpdate ReadyUpdate  NG");
		return bRet;
	}

	// 終了処理
	cPrgFile.CloseFile();
	NP_Printf("NPuComDataUpdate ReadyUpdate  OK");

	return bRet;
}

NP_BOOL
NPuComDataUpdate::ReadyUpdate_ForWWR(VOID)
{
	NP_BOOL	bRet			= NP_TRUE;
	DWORD	dwRetReadSize	= 0;
	/* open file by PFileOperator */
	PFileServiceProxy cPrgFile;

	// プログラムファイルのオープン
	if (FALSE == cPrgFile.OpenFile(m_szPrgFilePath, "r")){
		bRet = NP_FALSE;
	}

	// ヘッダ部の読み込み
	if (FALSE == cPrgFile.Seek(16, SEEK_END, FALSE))
	{
		bRet = NP_FALSE;
	}
	
	if (FALSE == cPrgFile.Read(&m_sWwrTail, sizeof(m_sWwrTail), &dwRetReadSize))
	{
		NP_Printf("NPuComDataUpdate ReadyUpdate  file read failed %s", m_szPrgFilePath );
		bRet = NP_FALSE;
	}
	else
	{
		NP_Printf("NPuComDataUpdate ReadyUpdate  file read success");
		NP_Printf("NPuComDataUpdate ReadyUpdate  file %s", m_szPrgFilePath );
		NP_Printf("NPuComDataUpdate ReadyUpdate  file pad.%02X%02X%02X%02X ver.%02X%02X%02X%02X len.%02X%02X%02X%02X crc.%02X%02X%02X%02X",
				  m_sWwrTail.pad[0],m_sWwrTail.pad[1],m_sWwrTail.pad[2],m_sWwrTail.pad[3],
				  m_sWwrTail.ver[0],m_sWwrTail.ver[1],m_sWwrTail.ver[2],m_sWwrTail.ver[3],
				  m_sWwrTail.len[0],m_sWwrTail.len[1],m_sWwrTail.len[2],m_sWwrTail.len[3],
				  m_sWwrTail.crc[0],m_sWwrTail.crc[1],m_sWwrTail.crc[2],m_sWwrTail.crc[3]
				  );
		
		cPrgFile.Seek(0, SEEK_SET, TRUE);
		
		// 読込サイズ確認
		if (sizeof( m_sWwrTail ) != dwRetReadSize)
		{
			bRet = NP_FALSE;
		}
		else
		{
			int i;
			
			// アップデート用変数の設定
			m_dwUpdateFileSize  = m_sWwrTail.len[3]; m_dwUpdateFileSize = ( m_dwUpdateFileSize<<8 ) & 0xFFFFFF00;
			m_dwUpdateFileSize += m_sWwrTail.len[2]; m_dwUpdateFileSize = ( m_dwUpdateFileSize<<8 ) & 0xFFFFFF00;
			m_dwUpdateFileSize += m_sWwrTail.len[1]; m_dwUpdateFileSize = ( m_dwUpdateFileSize<<8 ) & 0xFFFFFF00;
			m_dwUpdateFileSize += m_sWwrTail.len[0];
			
			NP_Printf("NPuComDataUpdate ReadyUpdate  Size %08X", m_dwUpdateFileSize );
			
			if(
			   ( m_dwUpdateFileSize != (cPrgFile.GetFileSize() - dwRetReadSize) )
			   ||
			   ( m_dwUpdateFileSize  > VERUP_DATA_WWR_MAXSIZE )
			   ||
			   ( m_dwUpdateFileSize == 0)
			  )
			{
				bRet = NP_FALSE;
				NP_Printf("NPuComDataUpdate ReadyUpdate  Size NG");
			}
			else
			{
				m_dwUpdateAddress	= 0;
				m_dwUpdateOverSize	= 0;

				NP_Printf("NPuComDataUpdate ReadyUpdate  Size OK");
			}
		}
	}
	// アップデート用メモリ領域の確保
	if(NP_FALSE != bRet)
	{
		m_pbyUpdateBinary = new BYTE[m_dwUpdateFileSize];
		if(NULL == m_pbyUpdateBinary){
			bRet = NP_FALSE;
		}
	}
	// データ部の読み込み
	if(NP_FALSE != bRet)
	{
		INT count, index;
		DWORD rest_size;

		count=(m_dwUpdateFileSize/NP_UCOM_READ_SIZE_MAX);
		rest_size = m_dwUpdateFileSize-(count*NP_UCOM_READ_SIZE_MAX);

		for (index=0; index<count; index++)
		{
			if(FALSE == cPrgFile.Read(m_pbyUpdateBinary+(NP_UCOM_READ_SIZE_MAX*index), NP_UCOM_READ_SIZE_MAX, &dwRetReadSize))
			{
				bRet = NP_FALSE;
				break;
			}
			else
			{
				if (NP_UCOM_READ_SIZE_MAX != dwRetReadSize)
				{
					bRet = NP_FALSE;
					break;
				}
			}
		}
		
		if ((NP_FALSE != bRet)
			&&(rest_size>0))
		{
			if(FALSE == cPrgFile.Read(m_pbyUpdateBinary+(NP_UCOM_READ_SIZE_MAX*count), rest_size, &dwRetReadSize))
			{
				bRet = NP_FALSE;
			}
			else
			{
				if (rest_size != dwRetReadSize)
				{
					bRet = NP_FALSE;
				}
			}
		}
	}

	// ここまででエラーがある場合
	if (NP_FALSE == bRet)
	{
		cPrgFile.CloseFile();
		if (NULL != m_pbyUpdateBinary)
		{
			delete[] m_pbyUpdateBinary;
			m_pbyUpdateBinary = NULL;
		}
		NP_Printf("NPuComDataUpdate ReadyUpdate  NG");
		return bRet;
	}

	// 終了処理
	cPrgFile.CloseFile();
	NP_Printf("NPuComDataUpdate ReadyUpdate  OK");

	return bRet;
}

/******************************************************************************

    マイコンアップデート終了確認関数

    【解説】
        マイコン書換え終了時に確認を行なう

*******************************************************************************/
// Add by qiaoyong Temporary to fix update Micon problem
extern int g_isUpdate;

VOID
NPuComDataUpdate::EndUpdate(NP_BOOL bResult)
{
	// Add by qiaoyong Temporary to fix update Micon problem
	g_isUpdate  = 0;
	
	// メモリの開放
	if (m_pbyUpdateBinary!=NULL)
	{
		delete[] m_pbyUpdateBinary;
		m_pbyUpdateBinary = NULL;
	}

	NPuComTimer *pcTimer = 	NPuComTimer::Instance();

	// 初期通信中フラグをクリアする
	if( NULL != pcTimer) {
		pcTimer->ClearStatus(NP_UCOM_STATUS_UPDATE_WHILE);
	}

	// マイコン書換え用共有メモリ
	NPuComMemoryInfoUpdate	sUpdateInfo;

	// バッファの初期化
	memset(&sUpdateInfo, 0, sizeof(sUpdateInfo));
	m_pcMemUpdate->GetData(&sUpdateInfo, sizeof(sUpdateInfo));

	if(NP_TRUE == bResult){
		// 初期通信完了フラグをセットする
		if( NULL != pcTimer) {
			pcTimer->MergeStatus(NP_UCOM_STATUS_UPDATE_FINISH);
		}
		// 状態設定
		sUpdateInfo.dwStatus = NP_UCOM_UPDATE_MODE_FINISH;
	}
	else{
		// 初期通信失敗フラグをセットする
		if( NULL != pcTimer) {
			pcTimer->MergeStatus(NP_UCOM_STATUS_UPDATE_FAILURE);
		}
		// 状態設定
		sUpdateInfo.dwStatus = NP_UCOM_UPDATE_MODE_FAILED;
		// エラーログ
		if( ( NULL != pcTimer)
			&& ( NP_FALSE == pcTimer->IsAccOff())){
			OutputErrLog("NPuComPacker.cpp", __LINE__, AIL_ERROR_LOG_CODE_COMDEVICE_ERROR, "DroidAVN", "NPuComDataUpdate::EndUpdate   error");
		}
	}
	// データ設定
	m_pcMemUpdate->SetDataNotify(&sUpdateInfo, sizeof(sUpdateInfo));

	return;	
}

/******************************************************************************

    マイコン書換え開始要求送信処理

    【解説】
        作成したパケットを送信する

*******************************************************************************/
NP_BOOL
NPuComDataUpdate::SendUpdateStartReq(NPuComDataFrameUpdate *psFrame)
{
	NPuComTimer *pcTimer = 	NPuComTimer::Instance();
	// ステータス遷移
	if( NULL != pcTimer) {
		pcTimer->MergeStatus(NP_UCOM_STATUS_UPDATE_WAIT_START_RES);	// ステータス設定
	}

	// 送信するパケットを記録する
	SetSendData( psFrame);

	return NP_TRUE;
}

/******************************************************************************

    マイコン書換え消去要求送信処理

    【解説】
        作成したパケットを送信する

*******************************************************************************/
NP_BOOL
NPuComDataUpdate::SendUpdateWriteSta(NPuComDataFrameUpdate *psFrame)
{
	int count =0;

	size_t Size;
	if ( m_UpdateTypeID == D_VERSION_UP_ID_WWR )
	{
		Size = static_cast<size_t>( NP_UCOM_SIZE_UPDATE_DATA_MAX_WWR );
		psFrame->m_dwFrameSize = NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 2 + NP_UCOM_SIZE_UPDATE_DATA_MAX_WWR;
	}
	else
	{
		Size = static_cast<size_t>( NP_UCOM_SIZE_UPDATE_DATA_MAX );
	}
	
	memset(&psFrame->m_byD[NP_UCOM_D2], 0xFF, Size);
	if(m_dwUpdateFileSize > m_dwUpdateOverSize)
	{
		if ((m_dwUpdateFileSize-m_dwUpdateOverSize)<Size)
		{
			Size = (m_dwUpdateFileSize-m_dwUpdateOverSize);
		}
	}
	else
	{
		Size = 0;
	}
	// アドレスの設定(D0～D1)
	psFrame->m_byD[NP_UCOM_D0]=((m_dwUpdateAddress>>8)&0xFF);
	psFrame->m_byD[NP_UCOM_D1]=(m_dwUpdateAddress&0xFF);
	// 転送データの設定(D2～)
	memcpy(&psFrame->m_byD[NP_UCOM_D2], (m_pbyUpdateBinary + m_dwUpdateOverSize), Size);
	for (count=0; count<Size; count++)
	{
		m_byUpdateCheckSum += psFrame->m_byD[NP_UCOM_D2+count];
	}
	
	/*
	NP_Printf("NPuComDataUpdate SendUpdateWriteSta size=%d,fsize=%d, %02X %02X %02X %02X %02X %02X %02X %02X",
			  Size, psFrame->m_dwFrameSize,
			  psFrame->m_byD[2+ 0], psFrame->m_byD[2+ 1],psFrame->m_byD[2+ 2],psFrame->m_byD[2+ 3],
			  psFrame->m_byD[2+60], psFrame->m_byD[2+61],psFrame->m_byD[2+62],psFrame->m_byD[2+63]
	    	  );
	*/

	// 更新用変数の設定
	m_dwUpdateOverSize		+= Size;
	m_dwUpdateAddress		++;

	NPuComTimer *pcTimer = 	NPuComTimer::Instance();
	// ステータス遷移
	if( NULL != pcTimer) {
		pcTimer->MergeStatus(NP_UCOM_STATUS_UPDATE_WAIT_WRITE_RES);	// ステータス設定
	}

	// 送信するパケットを記録する
	SetSendData( psFrame);

	return NP_TRUE;
}

/******************************************************************************

    マイコン書換え確認要求送信処理

    【解説】
        作成したパケットを送信する

*******************************************************************************/
NP_BOOL
NPuComDataUpdate::SendUpdateCheckSta(NPuComDataFrameUpdate *psFrame)
{
	// D0=00h固定
	if ( m_UpdateTypeID == D_VERSION_UP_ID_WWR )
	{
		memcpy(&psFrame->m_byD[NP_UCOM_D0], &m_sWwrTail, 16 );
		psFrame->m_dwFrameSize = NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD + 16;
	}
	else
	{
		// default D_VERSION_UP_ID_SYSTEM
		// psFrame->m_dwFrameSize = 1 + 10 + 2;
		psFrame->m_byD[NP_UCOM_D0] = ((m_byUpdateCheckSum>>8)&0x00FF);
		psFrame->m_byD[NP_UCOM_D1] = (m_byUpdateCheckSum&0x00FF);
	}

	NPuComTimer *pcTimer = 	NPuComTimer::Instance();
	// ステータス遷移
	if( NULL != pcTimer) {
		pcTimer->MergeStatus(NP_UCOM_STATUS_UPDATE_WAIT_CHECK_RES);	// ステータス設定
	}

	// 送信するパケットを記録する
	SetSendData( psFrame);

	return NP_TRUE;
}

/******************************************************************************

    マイコン書き換え再送応答送信関数

    【解説】
        作成したパケットを送信する

*******************************************************************************/
NP_BOOL
NPuComDataUpdate::SendUpdateRetryRes(NPuComDataFrameUpdate *psFrame, BYTE byTP)
{
	NP_BOOL bRet = NP_TRUE;

	if(GetSendDataTP() != byTP){
		// TP不一致エラー
		bRet = NP_FALSE;
	}
	else{
		NPuComTimer *pcTimer = 	NPuComTimer::Instance();

		switch(byTP){
		case NP_UCOM_TP_UPDATE_WRITE_DATA:
			// ステータス遷移
			if( NULL != pcTimer) {
				pcTimer->MergeStatus(NP_UCOM_STATUS_UPDATE_WAIT_WRITE_RES);	// ステータス設定
			}
			break;

		case NP_UCOM_TP_UPDATE_CHECK_STA:
			// ステータス遷移
			if( NULL != pcTimer) {
				pcTimer->MergeStatus(NP_UCOM_STATUS_UPDATE_WAIT_CHECK_RES);	// ステータス設定
			}
			break;

		default:
			bRet = NP_FALSE;
			break;
		}
	}

	if(NP_TRUE == bRet){
		GetSendData(psFrame);
	}
	return bRet;
}

/******************************************************************************

    マイコン書換えデータ保存処理関数

    【解説】
        最新の送信データを保持する

*******************************************************************************/
VOID
NPuComDataUpdate::SetSendData( const NPuComDataFrameUpdate *psFrame)
{
	memcpy(&m_sSendData, psFrame, sizeof(m_sSendData));
}

/******************************************************************************

    マイコン書換えデータ読出処理関数

    【解説】
        最新の送信データを取得する

*******************************************************************************/
VOID
NPuComDataUpdate::GetSendData(NPuComDataFrameUpdate *psFrame)
{
	memcpy(psFrame, &m_sSendData, sizeof(m_sSendData));
}

/******************************************************************************

    マイコン書換えデータ読出処理関数

    【解説】
        最新の送信データを取得する

*******************************************************************************/
BYTE
NPuComDataUpdate::GetSendDataTP(VOID)
{
	return m_sSendData.m_byTP;
}

/*===========================================================================*/
/*
 * 【更新履歴】
 * $Log: NPuComPacker.cpp,v $
 * Revision 1.1  2008/08/08 06:20:03  tomoyuki
 * 新規登録
 *
 */
/*===========================================================================*/
//================================================================= End of File
