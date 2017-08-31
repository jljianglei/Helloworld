/*

* (c) PIONEER CORPORATION 2014

* 25-1 Nishi-machi Yamada Kawagoe-shi Saitama-ken 350-8555 Japan

* All Rights Reserved.

*/


/*===========================================================================*/
/** @file
    �p�b�J�[�N���X��`�t�@�C��

    @attention C++ ��p�ł� C �ł͎g�p�o���܂���
*/
/*===========================================================================*/

#ifndef CXX_NPUCOMPACKER_H
#define CXX_NPUCOMPACKER_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


/*=============================================================================
   �C���N���[�h�t�@�C���̓ǂݍ���                                            */

#include "NSyncObject.h"
#include "NPSystemBase.h"
#include "NPuComHandler.h"
#include "NPuComMemory.h"


/*=============================================================================
	�萔��`                                                                 */

/*===========================================================================*/
/**
    �p�b�N�h�c��`
*/
/*===========================================================================*/
enum NP_UCOM_PACK_ID
{
	NP_UCOM_PACK_CONECHK_SEND			= 0x00,	//!< �����ʐM���M TP=0x00
	NP_UCOM_PACK_CONECHK1_SEND,                                    //!< �����ʐM���M TP=0x02
	NP_UCOM_PACK_CONECHK2_SEND,                                    //!< �����ʐM���M TP=0x05
	NP_UCOM_PACK_APK_ON_NOTIFY,                                      //!< APL ON ���M  TP=0x06
	NP_UCOM_PACK_POWER_ON_OFF_TPE3,			         //!< POWER ON OFF TP=0xE3
	NP_UCOM_PACK_DTV_TPE4_NOTIFY,					     //!< KM506 DTV NOTIFY �ʒm���M TP=0xE4
	NP_UCOM_PACK_SRC_CHANGE_RES,                                    //!< SOURCE CHANGE�������M TP=0x41, D0=0x01
	NP_UCOM_PACK_SRC_CHANGE_REQ,                                   //!< SOURCE CHANGE�v�����M TP=0x41,D0=0x03
	NP_UCOM_PACK_SRC_STATUS_NOTIFY,                               //!< SOC SOURCE STATUS���M TP=0x49
	NP_UCOM_PACK_DIRECTCMD_SEND,				        //!< �_�C���N�g�R�}���h  (TP=0x50)
	NP_UCOM_PACK_SRC_DIRECTCMD_SEND,			       //!< �\�[�X�ʃ_�C���N�g�R�}���h  (TP=0x51)
	NP_UCOM_PACK_DISP_DATA_REQ_SEND,                           //!< �\���f�[�^�v�����M  TP=0x68
	NP_UCOM_PACK_UPDATE_START_REQ,                                //!< SYSTEM CHIP VERSION UP START�v�����M  TP=0xC0
	NP_UCOM_PACK_UPDATE_WRITE_STA,                                //!< SYSTEM CHIP VERSION UP START�������M  TP=0xC4
	NP_UCOM_PACK_UPDATE_WRITE_DATA,                             //!< SYSTEM CHIP VERSION UP DATA���M  TP=0xC5
	NP_UCOM_PACK_UPDATE_WRITE_VA_DATA,                          //!< VA VERSION UP DATA���M  TP=0xC5
	NP_UCOM_PACK_UPDATE_CHECK_STA,                               //!< SYSTEM CHIP VERSION UP CHECK-SUM ���M  TP=0xC7
	NP_UCOM_PACK_UPDATE_CHECK_VA_STA,                            //!< VA VERSION UP CHECK SUM ���M  TP=0xC7
	NP_UCOM_PACK_UPDATE_RETRY_RES,
	NP_UCOM_PACK_UPDATE_ERASE_REQ,                               //!< VA ERASE SECTION AND RESET DECRYPTION CYPHER REQUEST TP=0xCB
	NP_UCOM_PACK_UPDATE_RESET_PRO,                               //!< VA Reset Processor TP=0xCD
	NP_UCOM_PACK_UPDATE_RESTART_REQ,                             //!< VA Update Restart TP=0xCA
	NP_UCOM_PACK_BITDATA_NOTIFY,                                                     //!<BITDATA ���M   TP=0xD0
	NP_UCOM_PACK_BEEP1_REQ_BITDATA_NOTIFY,                                 //!<BITDATA ���M   TP=0xD0
	NP_UCOM_PACK_BEEP2_REQ_BITDATA_NOTIFY,                                 //!<BITDATA ���M   TP=0xD0
	NP_UCOM_PACK_SXI_PW_PULL_UP_REQ_BITDATA_NOTIFY,              //!<BITDATA ���M   TP=0xD0
	NP_UCOM_PACK_SXI_PW_PULL_DOWN_REQ_BITDATA_NOTIFY,       //!<BITDATA ���M   TP=0xD0
	NP_UCOM_PACK_SXI_PWREN_PULL_UP_REQ_BITDATA_NOTIFY,        //!<BITDATA ���M   TP=0xD0
	NP_UCOM_PACK_SXI_PWREN_PULL_DOWN_REQ_BITDATA_NOTIFY, //!<BITDATA ���M   TP=0xD0
	NP_UCOM_PACK_BITDATA_NOTIFY_REQ,                            //!<BITDATA�v�����M   TP=0xD1
	NP_UCOM_PACK_UCOM_SRC_NOTIFY_REQ,                         //!<SYSTEM SOURCE STATUS�v�����M   TP=0xD3
	NP_UCOM_PACK_TESTCMD_SEND,
	NP_UCOM_PACK_RESET_REQ,                                         //!< POWER RESET�v�����M   TP=0xE5
	NP_UCOM_PACK_SETUP_DATA,                                     //!< SETUP MENU BACKUP DATA���M   TP=0xE2
	NP_UCOM_PACK_TESTMODE_NOTIFY,                           //!< TESTMODE STATUS���M  TP=0xE5
	NP_UCOM_PACK_SET_DRM,                                           //!< DRM INFO SET���M  TP=0x45
	NP_UCOM_PACK_DRM_INFO_RES,                                 //!< DRM INFORMATION���M  TP=0x45
	NP_UCOM_PACK_TAGGING_RES,                                   //!< TAGGING START �������M  TP=0x4D
	NP_UCOM_PACK_TAGGING_DATA1_RES,                     //!< TAGGING DATA1 �������M  TP=0x4D
	NP_UCOM_PACK_TAGGING_DATA2_RES,                       //!< TAGGING DATA2 �������M  TP=0x4D
	NP_UCOM_PACK_VAINFO_CURRSRC,                        //!< VA INFO ���M  TP=0x4F,D0=C0
	NP_UCOM_PACK_VAINFO_ICON,                           //!< VA INFO ���M  TP=0x4F,D0=C1
	NP_UCOM_PACK_VAINFO_TUNER_STATUS,                   //!< VA INFO ���M  TP=0x4F,D0=C2
	NP_UCOM_PACK_VAINFO_MEDIA_STATUS,                   //!< VA INFO ���M  TP=0x4F,D0=C3
	NP_UCOM_PACK_VAINFO_PHONE_STATUS,                   //!< VA INFO ���M  TP=0x4F,D0=C4
	NP_UCOM_PACK_VAINFO_TEXT_DATA,                      //!< VA INFO ���M  TP=0x4F,D0=C5
	NP_UCOM_PACK_VAINFO_NAVI_COMMAND,                   //!< VA INFO ���M  TP=0x4F,D0=C6
	NP_UCOM_PACK_VAINFO_STREETNAME,                     //!< VA INFO ���M  TP=0x4F,D0=C7
	NP_UCOM_PACK_VAINFO_DISTANCE,                       //!< VA INFO ���M  TP=0x4F,D0=C8
	NP_UCOM_PACK_VAINFO_ARRIVALTIME,                    //!< VA INFO ���M  TP=0x4F,D0=C9
	NP_UCOM_PACK_VAINFO_REQTIME,                        //!< VA INFO ���M  TP=0x4F,D0=CA
	NP_UCOM_PACK_VAINFO_COMPASSDIR,                     //!< VA INFO ���M  TP=0x4F,D0=CB
	NP_UCOM_PACK_VAINFO_INTERVALTIME,                   //!< VA INFO ���M  TP=0x4F,D0=CD
	NP_UCOM_PACK_SLAVE_TESTMODE_REQ,                                         //!< SLAVE TESTMODE ENTER RESET�v�����M   TP=0xE6
	NP_UCOM_PACK_SOURCELIST_NOTIFY,                                         //!< SOURCE LIST���M   TP=0xE7
	NP_UCOM_PACK_VOLUME_MODIFY,                                         //!< VOLUME MODIFY���M   TP=0xE9
	NP_UCOM_PACK_VERIFY_STATUS,                                         //!< VERIFY STATUS ���M   TP=0xEA
	NP_UCOM_PACK_SOC_VERSIONUP_NOTIFY,                                  //!< TP=0xC0 0x01
	NP_UCOM_PACK_VA_VERSIONUP_NOTIFY,                                   //!< TP=0xC0 0x05
	NP_UCOM_PACK_DVD_ONOFF_CMD,                                         //!< DVD ON OFF COMMAND ���M   TP=0xEB
	
	NP_UCOM_PACK_TPEC,                                  //!< TP=0xEC, key table receive

	// �p�b�N�h�c����ǉ����邱��
	NP_UCOM_PACK_ID_MAX				//!< �p�b�N�h�c��
};

/*===========================================================================*/
/**
    �A���p�b�N�h�c��`   For NPuComRxServer,NPuComPacker
*/
/*===========================================================================*/
enum NP_UCOM_UNPACK_ID
{
	NP_UCOM_UNPACK_CONECHK1_RCV          = 0x00, // �����ʐM TP=0x01
	NP_UCOM_UNPACK_CONECHK2_RCV,                 // �����ʐM TP=0x04
	NP_UCOM_UNPACK_SRC_CHG,                      // SOURCE CHANGE COMMAND TP=0x40
	NP_UCOM_UNPACK_SRC_STATUS_REQ,               // SOC SOURCE STATUS�v�� TP=0x48
	NP_UCOM_UNPACK_DISP_DATA_NAVI_FRONT,         // �\���f�[�^  TP=0x61
	NP_UCOM_UNPACK_UPDATE_START_RES,             // SYSTEM CHIP VERSION UP START����        TP=0xC1
	NP_UCOM_UNPACK_UPDATE_ERASE_END,             // SYSTEM CHIP VERSION UP FLASH-ERASED���� TP=0xC3
	NP_UCOM_UNPACK_UPDATE_WRITE_END,             // SYSTEM CHIP VERSION UP DATA WRITE����   TP=0xC6
	NP_UCOM_UNPACK_UPDATE_CHECK_END,             // SYSTEM CHIP VERSION UP CHECK-SUM ����   TP=0xC8
	NP_UCOM_UNPACK_UPDATE_ERASE_RES,             // VA VERSION UP Erase Request ACK        TP=0xCC
	NP_UCOM_UNPACK_UPDATE_VAUPDATE_RES,          // VA VERSION UP RESULT TP=0xCE
	NP_UCOM_UNPACK_BITDATA_NOTIFY_REQ,           // BITDATA�v��  TP=0xD1   
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

	// �p�b�N�h�c����ǉ����邱��
	NP_UCOM_UNPACK_ID_MAX		                 		//!< �A���p�b�N�h�c��
};


/*===========================================================================*/
/**
    �p�b�N�p�e�[�u���\����
*/
/*===========================================================================*/
struct NPuComPackTable
{
	DWORD	dwSize;	//!< �T�C�Y
	BYTE	byTP;	//!< TP
};

/*===========================================================================*/
/**
    �A���p�b�N�p�e�[�u���\����
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

#define D_KEY_RELEASE			( 0x00 )					/* �^�b�`��ԁF����					*/

#define D_KEYTYPE_HARDKEY_VALUE			( 0x00000000 )	
#define D_KEYTYPE_WIREDKEY_VALUE			( 0x00000100 )	
#define D_KEYTYPE_REMOTEKEY_VALUE		( 0x10000000 )	

enum D_KEYTYPE
{
	D_KEYTYPE_RELEASE						/** �L�[������				*/
,	D_KEYTYPE_HARDKEY						/** �{�̃L�[				*/
,	D_KEYTYPE_WIREDKEY						/** ���C���[�h�L�[			*/
,	D_KEYTYPE_REMOKEY						/** �����R���L�[			*/
};

typedef struct 
{
	unsigned char  rotary_code;
	unsigned char  hardkey_code;												/* �{�̃L�[�R�[�h					*/
	unsigned char  wired_code;											/* ���C���[�h�L�[�R�[�h				*/
	unsigned short remocon_code;											/* �����R���L�[�R�[�h				*/
}ST_KEY_STATUS;

/*--------------------------------------------------------------------------------------------------*/
/* �L�[�f�[�^�i�[�p�\����																			*/
/*--------------------------------------------------------------------------------------------------*/
typedef struct
{
	ST_KEY_STATUS			key_sts;					/* �n�[�h�L�[���					*/
	unsigned short					key_type;					/* �L�[���							*/
	unsigned short					key_code;					/* �L�[�R�[�h						*/
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
		unsigned char hardkey;   /* BD1_7 : bit0-7	: �{�̃L�[�R�[�h          */
		unsigned char wiredkey;  /* BD1_8 : bit0-7	: ���C���[�h�L�[�R�[�h    */
		unsigned char remo_1st;  /* BD1_9 : bit0-7	: �����R���R�[�h1st       */
		unsigned char remo_2nd;  /* BD1_10: bit0-7	: �����R���R�[�h2nd       */
	}bit;

	unsigned char	bit_array[NP_UCOM_SIZE_BD];
};


/*=============================================================================
     �N���X�錾                                                              */

class NPuComBitData;
class NPuComPacker;
class NPuComDataUpdate;


/*===========================================================================*/
/**
    �}�C�R���r�b�g�f�[�^�N���X
*/
/*===========================================================================*/
class NPuComBitData : public NObject
{
public:

	/*************************************************************************/
	/**
		�}�C�R���r�b�g�f�[�^�N���X�R���X�g���N�^
	*/
	/*************************************************************************/
	NPuComBitData();

	/*************************************************************************/
	/**
		�}�C�R���r�b�g�f�[�^�N���X�f�X�g���N�^
	*/
	/*************************************************************************/
	virtual ~NPuComBitData();

	/*************************************************************************/
	/**
		����������

		@param  �Ȃ�

		@retval �Ȃ�
	*/
	/*************************************************************************/
	VOID
	Initialize(VOID);

	/*************************************************************************/
	/**
		�r�b�g�f�[�^�ݒ菈��
	    
		@param dwNum	:	BD�ԍ�
		@param byData	:	�w��ԍ��̓��e

		@return �Ȃ�
	*/
	/*************************************************************************/
	VOID 
	SetBitData( DWORD dwNum, BYTE byData );

	/*************************************************************************/
	/**
		�r�b�g�f�[�^�擾����
	    
		@param dwNum	:	BD�ԍ�

		@return BYTE	:	�w��ԍ���BD
	*/
	/*************************************************************************/
	BYTE 
	GetBitData( DWORD dwNum );

private:

	BYTE	*m_pbyBD;

	/**
	 * �R�s�[�R���X�g���N�^
	 *
	 * ���p�֎~
	 *
	 * @param const NPuComBitData& src [IN] �R�s�[��
	 *
	 * @return �Ȃ�
	 */
	NPuComBitData( const NPuComBitData& src);

	/**
	 * �R�s�[���Z�q
	 *
	 * ���p�֎~
	 *
	 * @param const NPuComBitData& src [IN] �R�s�[��
	 *
	 * @return const NPuComBitData& �������g�ւ̎Q��
	 */
	const NPuComBitData& operator=( const NPuComBitData& src);
};


/*===========================================================================*/
/**
    �p�b�J�[�N���X

	@author $Author: hustyy $
	@version $Revision: 5833 $
*/
/*===========================================================================*/
class NPuComPacker : public NObject
{
public:
	/*************************************************************************/
	/**
		�p�b�J�[�N���X�̃C���X�^���X�̎擾
	    
		@param �Ȃ�

		@return UCOM�n���h���v���L�V�̃C���X�^���X
	*/
	/*************************************************************************/
	static NPuComPacker
	*Instance( VOID );

	/*************************************************************************/
	/**
		�p�b�J�[�N���X�̃C���X�^���X�̔j��
	    
		@param �Ȃ�

		@return �Ȃ�
	*/
	/*************************************************************************/
	static VOID 
	Destroy( VOID );

	/*************************************************************************/
	/**
		�p�b�N����
	    
		@param dwId		:	[IN]  �p�b�N�h�c
		@param pBuff	:	[IN]  �o�b�t�@
		@param pdwSize	:	[IN]  �o�b�t�@�T�C�Y
		@param psFrame	:	[OUT] �p�P�b�g

		@return �Ȃ�
	*/
	/*************************************************************************/
	VOID 
	Pack( const DWORD dwId, LPCVOID pBuff, const DWORD dwSize, NPuComDataFrameUpdate *psFrame );

	/*************************************************************************/
	/**
		�A���p�b�N����
	    
		@param psFrame		:	[IN]  �p�P�b�g

		@return �Ȃ�
	*/
	/*************************************************************************/
	VOID 
	UnPack( const NPuComDataFrame *psFrame );

	/*************************************************************************/
	/**
		��M�pBD�ݒ菈��
	    
		@param dwNum	:	BD�ԍ�
		@param byData	:	�w��ԍ��̓��e

		@return �Ȃ�
	*/
	/*************************************************************************/
	VOID 
	SetRxBitData( DWORD dwNum, BYTE byData );

	/*************************************************************************/
	/**
		���M�pBD�ݒ菈��
	    
		@param dwNum	:	BD�ԍ�
		@param byData	:	�w��ԍ��̓��e

		@return �Ȃ�
	*/
	/*************************************************************************/
	VOID 
	SetTxBitData( DWORD dwNum, BYTE byData );

	/*************************************************************************/
	/**
		��M�pBD�擾����
	    
		@param dwNum	:	BD�ԍ�

		@return BYTE	:	�w��ԍ���BD
	*/
	/*************************************************************************/
	BYTE 
	GetRxBitData( DWORD dwNum );

	/*************************************************************************/
	/**
		���M�pBD�擾����
	    
		@param dwNum	:	BD�ԍ�

		@return BYTE	:	�w��ԍ���BD
	*/
	/*************************************************************************/
	BYTE 
	GetTxBitData( DWORD dwNum );

	/*************************************************************************/
	/**
		���M�r�b�g�f�[�^�ݒ菈��
	    
		@param dwId		:	�r�b�g�h�c
		@param bStatus	:	�r�b�g���

		@retval NP_TRUE		: �r�b�g�ω�����
		@retval	NP_FALSE	: �r�b�g�ω��Ȃ�
	*/
	/*************************************************************************/
	NP_BOOL 
	ChangeTxBitData( const DWORD dwId, const NP_BOOL bStatus );

	/*************************************************************************/
	/**
		���M�r�b�g�f�[�^�ݒ菈��
	    
		@param dwId		:	�r�b�g�h�c
		@param bStatus	:	�r�b�g���

		@retval NP_TRUE		: �r�b�g�ω�����
		@retval	NP_FALSE	: �r�b�g�ω��Ȃ�
	*/
	/*************************************************************************/
	NP_BOOL 
	ChangeTxBitData( const DWORD dwId, const BYTE bStatus );

	/*************************************************************************/
	/**
		���M�r�b�g�f�[�^�ݒ菈��
	    
		@param pbyChgBD	:	�r�b�g���

		@retval NP_TRUE		: �r�b�g�ω�����
		@retval	NP_FALSE	: �r�b�g�ω��Ȃ�
	*/
	/*************************************************************************/
	NP_BOOL 
	ChangeTxBitData(PBYTE pbyChgBD);
	/*************************************************************************/
	/**
		��M�r�b�g�f�[�^�ݒ菈��
	    
	   @param pbyChgBD		:[OUT]  �ω��t���O

		@retval NP_TRUE		: �r�b�g�ω�����
		@retval	NP_FALSE	: �r�b�g�ω��Ȃ�
	*/
	/*************************************************************************/
	NP_BOOL 
	ChangeRxBitData( PBYTE pbyChgBD );

	/*************************************************************************/
	/**
		key event type judge����
	    
		@param keystatus		:[IN]  KEY-BORAD STATUS
		@param key_code		:[OUT]  KEYEVENT--KEYCODE
		@param key_type		:[OUT]  KEYEVENT--KEYTYPE

		@return �Ȃ�
	*/
	/*************************************************************************/
	void
	CheckKeyLevel(ST_KEY_DATA *keystatus,unsigned short *key_code,unsigned short *key_type);

	/*************************************************************************/
	/**
		�r�b�g�f�[�^�m�F����
	    
		@param psFrame		:	[IN]  �p�P�b�g

		@retval NP_TRUE		: �r�b�g�ω�����
		@retval	NP_FALSE	: �r�b�g�ω��Ȃ�
	*/
	/*************************************************************************/
	NP_BOOL 
	CheckReceiveBD( const NPuComDataFrame *psFrame );

	NP_BOOL
	GetReceiveBD( NPuComRcvBD *psBD );

	/*************************************************************************/
	/**
		�����ʐM���ʎ擾
	    
		@param �Ȃ�

		@retval �Ȃ�
	*/
	/*************************************************************************/
	VOID 
	GetConeChkStatus( NPConeChkStatus *pcStatus );


	/*************************************************************************/
	/**
		Reset Tx/Rx bitdata when resume
	    
		@param �Ȃ�

		@retval �Ȃ�
	*/
	/*************************************************************************/
	VOID 
	ResetRxTxBitdata( VOID );

	VOID 
	EnableDetectLearningKey(BOOL bEnable);

	BOOL 
	IsLearningKeyPush();
	
private:

	static NSyncObject	s_cSync;		//!< �����I�u�W�F�N�g
	static NPuComPacker	*s_pcInstance;	//!< �v���Z�X�ɗB��̃C���X�^���X

	NPuComBitData	m_cRcvBD;	//!< ��M�pBD
	NPuComBitData	m_cSndBD;	//!< ���M�pBD

	NPConeChkStatus	m_sConeChk;	//!< �����ʐM���

	NP_BOOL	m_bVerRes;
	NP_BOOL	m_blBitNotifyStarted;	//!< �ŏ���BitData��M��������t���O

	NSyncObject s_cSyncBD;
	NSyncObject s_cSyncRxBD;

	BOOL m_bDetectWiredKeyPress;
	BOOL m_bWiredKeyPress;
	NSyncObject s_cSyncKey;
	
	unsigned int m_isTestModeNotify;	// LH=isNotified, LL=TP61.70.04.[D9];
	
	/*************************************************************************/
	/**
		�R���X�g���N�^

		@param	�Ȃ�

		@retval	�Ȃ�
	*/
	/*************************************************************************/
	NPuComPacker();

	/*************************************************************************/
	/**
		�f�X�g���N�^

		@param	�Ȃ�

		@retval	�Ȃ�
	*/
	/*************************************************************************/
	virtual ~NPuComPacker();

	/*************************************************************************/
	/**
		�����ʐM���M������
	    
		@param �Ȃ�

		@retval �Ȃ�
	*/
	/*************************************************************************/
	VOID 
	OnSendConeChkReq( VOID );

	/*************************************************************************/
	/**
		�����ʐM��M������
	    
		@param �Ȃ�

		@retval �Ȃ�
	*/
	/*************************************************************************/
	VOID 
	OnReceiveConeChkRes( VOID );

	/*************************************************************************/
	/**
		�d���m�F����
	    
		@param �Ȃ�

		@retval �Ȃ�
	*/
	/*************************************************************************/
	VOID 
	CheckModel( VOID );

	/**
	 * �R�s�[�R���X�g���N�^
	 *
	 * ���p�֎~
	 *
	 * @param const NPuComPacker& src [IN] �R�s�[��
	 *
	 * @return �Ȃ�
	 */
	NPuComPacker( const NPuComPacker& src);

	/**
	 * �R�s�[���Z�q
	 *
	 * ���p�֎~
	 *
	 * @param const NPuComPacker& src [IN] �R�s�[��
	 *
	 * @return const NPuComPacker& �������g�ւ̎Q��
	 */
	const NPuComPacker& operator=( const NPuComPacker& src);
};


/*===========================================================================*/
/**
    �}�C�R�����������N���X

	@author $Author: hustyy $
	@version $Revision: 5833 $
*/
/*===========================================================================*/
class NPuComDataUpdate : public NObject
{
public:
	/*************************************************************************/
	/**
		�}�C�R�����������N���X�̃C���X�^���X�̎擾
	    
		@param �Ȃ�

		@return UCOM�n���h���v���L�V�̃C���X�^���X
	*/
	/*************************************************************************/
	static NPuComDataUpdate
	*Instance( VOID );

	/*************************************************************************/
	/**
		�}�C�R�����������N���X�̃C���X�^���X�̔j��
	    
		@param �Ȃ�

		@return �Ȃ�
	*/
	/*************************************************************************/
	static VOID 
	Destroy( VOID );

	/*************************************************************************/
	/**
		�}�C�R���A�b�v�f�[�g�̈�T�C�Y�擾�֐�

		@param	�Ȃ�

		@retval	DWORD	: �̈�T�C�Y
	*/
	/*************************************************************************/
	DWORD
	GetUpdateAllSize(VOID)
	{
		return m_dwUpdateFileSize;
	};

	/*************************************************************************/
	/**
		�}�C�R���A�b�v�f�[�g�����ݍς݃T�C�Y�擾�֐�

		@param	�Ȃ�

		@retval	DWORD	: �����ݍς݃T�C�Y
	*/
	/*************************************************************************/
	DWORD
	GetUpdateOverSize(VOID)
	{
		return m_dwUpdateOverSize;
	};

	/*************************************************************************/
	/**
		�}�C�R���A�b�v�f�[�g�c��T�C�Y�擾�֐�

		@param	�Ȃ�

		@retval	DWORD	: �c��T�C�Y
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
		�}�C�R���A�b�v�f�[�g�����֐�

		@param	�Ȃ�

		@retval	TRUE	: ����
		@retval	FALSE	: ���s
	*/
	/*************************************************************************/
	VOID
	SetUpdatePath(LPCVOID pData, DWORD uUpdateTypeID );
	
	DWORD
	GetUpdateTypeID( VOID );

	/*************************************************************************/
	/**
		�}�C�R�����������J�n����
	    
		@param �Ȃ�

		@retval NP_TRUE		: ����
		@retval	NP_FALSE	: ���s
	*/
	/*************************************************************************/
	NP_BOOL 
	StartUpdateReq( VOID );

	/*************************************************************************/
	/**
		�}�C�R���A�b�v�f�[�g�����֐�

		@param	�Ȃ�

		@retval	TRUE	: ����
		@retval	FALSE	: ���s
	*/
	/*************************************************************************/
	NP_BOOL
	ReadyUpdate( VOID );
	NP_BOOL
	ReadyUpdate_ForWWR( VOID );

	/*************************************************************************/
	/**
		�}�C�R���A�b�v�f�[�g�I���֐�

		@param	�Ȃ�

		@retval	�Ȃ�
	*/
	/*************************************************************************/
	VOID
	EndUpdate(NP_BOOL bResult);

	/*************************************************************************/
	/**
		�}�C�R���������J�n�v�����M�����֐�

		@param psFrame		:	[OUT] �p�P�b�g

		@retval	NP_TRUE		: ����
		@retval	NP_FALSE	: ���s
	*/
	/*************************************************************************/
	NP_BOOL
	SendUpdateStartReq(NPuComDataFrameUpdate *psFrame);

	/*************************************************************************/
	/**
		�}�C�R�������������J�n�v�����M�����֐�

		@param psFrame		:	[OUT] �p�P�b�g

		@retval	NP_TRUE	: ����
		@retval	NP_FALSE	: ���s
	*/
	/*************************************************************************/
	NP_BOOL
	SendUpdateWriteSta(NPuComDataFrameUpdate *psFrame);

	/*************************************************************************/
	/**
		�}�C�R���������m�F�J�n�v�����M�����֐�

		@param psFrame		:	[OUT] �p�P�b�g

		@retval	NP_TRUE	: ����
		@retval	NP_FALSE	: ���s
	*/
	/*************************************************************************/
	NP_BOOL
	SendUpdateCheckSta(NPuComDataFrameUpdate *psFrame);

	/*************************************************************************/
	/**
		�}�C�R�����������đ��������M�֐�

		@param psFrame		:	[OUT] �p�P�b�g

		@retval	NP_TRUE	: ����
		@retval	NP_FALSE	: ���s
	*/
	/*************************************************************************/
	NP_BOOL
	SendUpdateRetryRes(NPuComDataFrameUpdate *psFrame, BYTE byTP);

private:

	static NSyncObject		s_cSync;		//!< �����I�u�W�F�N�g
	static NPuComDataUpdate	*s_pcInstance;	//!< �v���Z�X�ɗB��̃C���X�^���X

	NPuComHandler			*m_pcMain;			//!< �n���h�����C���X���b�h�|�C���^
	NPuComMemoryUpdate		*m_pcMemUpdate;		//!< �}�C�R���������p���L������
	NPuComDataFrameUpdate	m_sSendData;		//!< �ŐV�̑��M�f�[�^
	


	// �}�C�R���������p
	NPuComPrgFileHeader		m_sPrgHead;									//!< �}�C�R�����������p�v���O�����t�@�C�����
	NPuComPrgFileTail_WWR	m_sWwrTail;
	
	DWORD					m_dwUpdateAddress;							//!< �}�C�R�����������p������A�h���X
	DWORD					m_dwUpdateFileSize;							//!< �}�C�R�����������p�t�@�C���T�C�Y
	DWORD					m_dwUpdateOverSize;							//!< �}�C�R�����������p�����σT�C�Y
	PBYTE					m_pbyUpdateBinary;							//!< �}�C�R�����������p�f�[�^�|�C���^
	WORD					m_byUpdateCheckSum;			//!< �}�C�R�����������p�`�F�b�N�T���i�[

	TCHAR					m_szPrgFilePath[NP_UCOM_SIZE_PATH_MAX];		//!< �}�C�R�����������p�o�[�W�����t�@�C���p�X
	DWORD					m_UpdateTypeID;		//!< ����uCom IC'ID : D_VERSION_UP_ID_DISP
												//!                   D_VERSION_UP_ID_DISP_RECOVERY
												//!                   D_VERSION_UP_ID_SYSTEM
												//!                   D_VERSION_UP_ID_WWR

	/*************************************************************************/
	/**
		�R���X�g���N�^

		@param	�Ȃ�

		@retval	�Ȃ�
	*/
	/*************************************************************************/
	NPuComDataUpdate();

	/*************************************************************************/
	/**
		�f�X�g���N�^

		@param	�Ȃ�

		@retval	�Ȃ�
	*/
	/*************************************************************************/
	virtual ~NPuComDataUpdate();

	/*************************************************************************/
	/**
		�}�C�R���������f�[�^�ۑ������֐�

		@param psFrame		:	[IN] �p�P�b�g

		@retval	�Ȃ�
	*/
	/*************************************************************************/
	VOID
	SetSendData( const NPuComDataFrameUpdate *psFrame);

	/*************************************************************************/
	/**
		�}�C�R���������f�[�^�Ǐo�����֐�

		@param psFrame		:	[OUT] �p�P�b�g

		@retval	�Ȃ�
	*/
	/*************************************************************************/
	VOID
	GetSendData(NPuComDataFrameUpdate *psFrame);

	/*************************************************************************/
	/**
		�}�C�R��������TP�Ǐo�����֐�

		@param �Ȃ�

		@retval	BYTE	: TP
	*/
	/*************************************************************************/
	BYTE
	GetSendDataTP(VOID);

	/**
	 * �R�s�[�R���X�g���N�^
	 *
	 * ���p�֎~
	 *
	 * @param const NPuComDataUpdate& src [IN] �R�s�[��
	 *
	 * @return �Ȃ�
	 */
	NPuComDataUpdate( const NPuComDataUpdate& src);

	/**
	 * �R�s�[���Z�q
	 *
	 * ���p�֎~
	 *
	 * @param const NPuComDataUpdate&src [IN] �R�s�[��
	 *
	 * @return const NPuComDataUpdate& �������g�ւ̎Q��
	 */
	const NPuComDataUpdate& operator=( const NPuComDataUpdate&src);
};


/*===========================================================================*/
/*
 * �y�X�V�����z
 * $Log: NPuComPacker.h,v $
 * Revision 1.1  2008/08/08 06:20:03  tomoyuki
 * �V�K�o�^
 *
 */
/*===========================================================================*/
#endif /* CXX_NPUCOMPACKER_H */
//================================================================= End of File
