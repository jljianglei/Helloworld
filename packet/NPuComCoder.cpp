/*

* (c) PIONEER CORPORATION 2014

* 25-1 Nishi-machi Yamada Kawagoe-shi Saitama-ken 350-8555 Japan

* All Rights Reserved.

*/

 /*===========================================================================*/
/** @file
    �R�[�_�N���X�����t�@�C��

    @attention C++ ��p�ł� C �ł͎g�p�o���܂���
*/
/*===========================================================================*/

/*=============================================================================
   �C���N���[�h�t�@�C���̓ǂݍ���                                            */

#include "NPLOGID_UCOM.h"
#include "NPuComCoder.h"
#include "NBinaryQueue.h"


/*=============================================================================
	�O���[�o���錾                                                           */

// �R�[�_�N���X
NPuComCoder* NPuComCoder::s_pcInstance = NULL;
// �����I�u�W�F�N�g
NSyncObject NPuComCoder::s_cSync;

/*=============================================================================
	�萔��`                                                                 */

const DWORD	NP_UCOM_QUEUE_FRAME_NUM		= 50;	//!< 50��
const DWORD	NP_UCOM_SIZE_DATA_MAX		= NP_UCOM_SIZE_FRAME_MAX + NP_UCOM_SIZE_CS;	//!< �t���[���{�b�r
const DWORD	NP_UCOM_SIZE_DATA_MIN		= NP_UCOM_SIZE_FRAME_MIN + NP_UCOM_SIZE_CS;	//!< �t���[���{�b�r

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

    �R���X�g���N�^

******************************************************************************/
NPuComCoder::NPuComCoder()
:	// ���������X�g
	NObject(),
	m_sPacket(),
	m_sStockData(),
	m_pRcvQueue(NULL),
	m_dwRcvNum(0),
	m_dwRcvStatus(STATUS_WAIT_DLE1)
{
	// �\���̂̏�����
	memset(&m_sPacket, 0, sizeof(m_sPacket));
	memset(&m_sStockData, 0, sizeof(m_sStockData));
	// �L���[�̐���
	m_pRcvQueue = new NBinaryQueue(sizeof(NPuComDataFrame) * NP_UCOM_QUEUE_FRAME_NUM);
}

/******************************************************************************

    �f�X�g���N�^

******************************************************************************/
NPuComCoder::~NPuComCoder()
{
	// �L���[�̔j��
	if(NULL != m_pRcvQueue){
		delete m_pRcvQueue;
		m_pRcvQueue = NULL;
	}
}

/******************************************************************************

    �R�[�_�N���X�C���X�^���X�擾�֐�

    �y����z
        �v���Z�X�ŗB��̃C���X�^���X�̎擾�i�V���O���g���p�^�[���j
        �C���X�^���X���������̏ꍇ�̂ݐ������C���ɃC���X�^���X�����������
        ����ꍇ�͂��̃C���X�^���X��Ԃ��D�V���O���g���p�^�[���ƂȂ��Ă���D

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

    �R�[�_�N���X�C���X�^���X�j���֐�

    �y����z
        �C���X�^���X�̔j�����s�Ȃ�

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

    Update�p�P�b�g���菈��

    �y����z
        �p�����[�^��TP��Update�p�P�b�g���ǂ����𔻒肷��B

******************************************************************************/
NP_BOOL NPuComCoder::IsUpdatePacket( BYTE byTP)
{
	return (  NP_UCOM_TP_UPDATE_WRITE_DATA == byTP);
}

/******************************************************************************

    �p�P�b�g�ݒ�

    �y����z
        ��M�����p�P�b�g��ۑ����ăf�R�[�h���s��

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
	
	// �p�P�b�g�T�C�Y�̐���
	if( NP_FALSE != IsUpdatePacket( psFrame->m_byTP)) {
		// Update�n�p�P�b�g�Ȃ̂ŁA�T�C�Y�̊g��(260byte)��OK
		if( NP_UCOM_SIZE_D_UPDATE < dwDataSize) {
			psPacket->m_dwPacketNum = 0;
			return;
		}
	}
	else {
		// �ʏ�n�p�P�b�g�Ȃ̂ŁA�T�C�Y��44byte
		if( NP_UCOM_SIZE_D_NORMAL < dwDataSize) {
			psPacket->m_dwPacketNum = 0;
			return;
		}
	}

	// DLE�̐ݒ�
	psPacket->m_byPacket[dwSetSize] = DATA_DLE;
	++dwSetSize;

	// STX�̐ݒ�
	psPacket->m_byPacket[dwSetSize] = DATA_STX;
	++dwSetSize;

	// TP�̐ݒ�
	psPacket->m_byPacket[dwSetSize] = psFrame->m_byTP;
	byCS = byCS ^ psFrame->m_byTP;
	++dwSetSize;
	if(DATA_DLE == psFrame->m_byTP){
		psPacket->m_byPacket[dwSetSize] = DATA_DLE;
		++dwSetSize;
	}

	// BD�̐ݒ�
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

	// D�̐ݒ�
	{
		for(DWORD dwLoopD = 0; dwLoopD < dwDataSize; ++dwLoopD){
			psPacket->m_byPacket[dwSetSize] = psFrame->m_byD[dwLoopD];
			byCS = static_cast<BYTE>( byCS ^ psFrame->m_byD[dwLoopD]);
			++dwSetSize;
			if(DATA_DLE == psFrame->m_byD[dwLoopD]){
				psPacket->m_byPacket[dwSetSize] = DATA_DLE;
				++dwSetSize;
			}
			// �T�C�Y�`�F�b�N
			if(CHECKSIZE_D < dwSetSize){
				// ����������
				return;
			}
		}
	}

	// CS�̐ݒ�
	psPacket->m_byPacket[dwSetSize] = byCS;
	++dwSetSize;
	if(DATA_DLE == byCS){
		psPacket->m_byPacket[dwSetSize] = DATA_DLE;
		++dwSetSize;
	}
	// �T�C�Y�`�F�b�N
	if(CHECKSIZE_CS < dwSetSize){
		// ����������
		return;
	}

	// DLE�̐ݒ�
	psPacket->m_byPacket[dwSetSize] = DATA_DLE;
	++dwSetSize;

	// ETX�̐ݒ�
	psPacket->m_byPacket[dwSetSize] = DATA_ETX;
	++dwSetSize;

	// �Ō�ɃT�C�Y�̐ݒ�
	psPacket->m_dwPacketNum = dwSetSize;
}

/******************************************************************************

    �p�P�b�g�ݒ�

    �y����z
        ��M�����p�P�b�g��ۑ����ăf�R�[�h���s��

******************************************************************************/
DWORD
NPuComCoder::SetPacket( const NPuComPacketUpdate *psPacket )
{
	// �f�[�^�̃R�s�[
	memcpy(&m_sPacket, psPacket, sizeof(m_sPacket));

	// �f�R�[�h
	Decode();

	return m_dwRcvNum;
}

/******************************************************************************

    �p�P�b�g�ݒ�

    �y����z
        ��M�����p�P�b�g��ۑ����ăf�R�[�h���s��

******************************************************************************/
VOID
NPuComCoder::GetFrame( NPuComDataFrame *psFrame )
{
	if(0 != m_dwRcvNum){
		// �f�[�^�̃R�s�[
		m_pRcvQueue->Pop(psFrame, sizeof(NPuComDataFrame));
		// �f�[�^���f�N�������g
		--m_dwRcvNum;
	}
}

/******************************************************************************

    �p�P�b�g�ݒ�

    �y����z
        ��M�����p�P�b�g��ۑ����ăf�R�[�h���s��

******************************************************************************/
VOID
NPuComCoder::Decode( VOID )
{
	// Store last decoded data
	DWORD	&dwSize   = m_sStockData.m_dwPacketNum;;
	BYTE	*pbyBuff = m_sStockData.m_byPacket;

	// Decode  every byte from UART(m_sPacket.m_dwPacketNum)
	for(DWORD dwReadNum = 0; dwReadNum < m_sPacket.m_dwPacketNum; ++dwReadNum){
		// �f�R�[�h����
		switch(m_dwRcvStatus){
			// wait DLE(0x9f),change wait state to wait STX,do not store data,
		case STATUS_WAIT_DLE1:
			if(DATA_DLE == m_sPacket.m_byPacket[dwReadNum]){
				// STX�҂��ɐi��
				m_dwRcvStatus = STATUS_WAIT_STX;
			}
			break;
				// wait STX(0x02)
		case STATUS_WAIT_STX:
			// if wait STX(0x02) OK,change wait state to wait data, not store data
			if(DATA_STX == m_sPacket.m_byPacket[dwReadNum]){
				// DATA�҂��ɐi��
				m_dwRcvStatus = STATUS_WAIT_DATA;
			}
			// if no STX(0x02), Return to DLE wait state,reset size
			else{
				// DLE1�҂��ɖ߂�
				m_dwRcvStatus = STATUS_WAIT_DLE1;
				// �\���̂̏�����
				memset(pbyBuff, 0, NP_UCOM_SIZE_DATA_MAX);
				dwSize = 0;
			}
			break;
			// wait data(data(0xxx)  / DLE(0x9f)STX(02)  /  DLE(0x9f)DLE(0x9f)  / DLE(0x9f)ETX(03)....)
		case STATUS_WAIT_DATA:
			// if DLE(0x9f),change state to wait ETX,don't store data
			if(DATA_DLE == m_sPacket.m_byPacket[dwReadNum]){
				// ETX�҂��ɐi��
				m_dwRcvStatus = STATUS_WAIT_ETX;
			}
			// other case 
			else{
				// if oversize error �T�C�Y�`�F�b�N,reset wait state,reset dwSize
				if(NP_UCOM_SIZE_DATA_MAX <= dwSize){
					// �T�C�Y�I�[�o�[�iDLE1�҂��ɖ߂�j
					m_dwRcvStatus = STATUS_WAIT_DLE1;
					// �\���̂̏�����
					memset(pbyBuff, 0, NP_UCOM_SIZE_DATA_MAX);
					dwSize = 0;
				}
				// if size < NP_UCOM_SIZE_DATA_MAX,there is free space to hold data,store data
				else{
					// �f�[�^�̕ۑ�
					pbyBuff[dwSize] = m_sPacket.m_byPacket[dwReadNum];
					++dwSize;
				}
			}
			break;
			// the state is changed from wait data, (DLE(0x9f)STX(02)  /  DLE(0x9f)DLE(0x9f) / DLE(0x9f)ETX(03) ......)
		case STATUS_WAIT_ETX:
			// if DLE(0x9f)DLE(0x9f) ,it is treated as data byte
			if(DATA_DLE == m_sPacket.m_byPacket[dwReadNum]){
				// �T�C�Y�`�F�b�N,Oversize error,reset wait state,reset  dwSize
				if(NP_UCOM_SIZE_DATA_MAX <= dwSize){
					// �T�C�Y�I�[�o�[�iDLE1�҂��ɖ߂�j
					m_dwRcvStatus = STATUS_WAIT_DLE1;
					// �\���̂̏�����
					memset(pbyBuff, 0, NP_UCOM_SIZE_DATA_MAX);
					dwSize = 0;
				}
				// double DLE,save one DLE(0x9f) in decoded data
				else{
					// �Q�dDLE(�ۑ�����DATA�҂��ɖ߂�)
					m_dwRcvStatus = STATUS_WAIT_DATA;
					pbyBuff[dwSize] = m_sPacket.m_byPacket[dwReadNum];
					++dwSize;
				}
			}
			// if DLE(0x9f)STX(02) ,wait end error,wait STX,change wait state to wait data,reset dwSize
			else if(DATA_STX == m_sPacket.m_byPacket[dwReadNum]){
				// DATA�҂��ɖ߂�
				m_dwRcvStatus = STATUS_WAIT_DATA;
				// �f�[�^�͔j��
				memset(pbyBuff, 0, NP_UCOM_SIZE_DATA_MAX);
				dwSize = 0;
			}
			// DLE(0x9f)ETX(03) , wait end OK
			else if(DATA_ETX == m_sPacket.m_byPacket[dwReadNum]){
				// �P�t���[����M����,size check
				if((NP_UCOM_SIZE_DATA_MIN <= dwSize) && (NP_UCOM_SIZE_DATA_MAX >= dwSize)){
					// �f�[�^�T�C�Y����,check sum
					if(NP_FALSE != CheckDataSum(pbyBuff, dwSize)){
						// �`�F�b�N�T������
						{
							NPuComDataFrame sTmpFrame;
							// �\���̂̏�����
							memset(&sTmpFrame, 0, sizeof(sTmpFrame));

							// �f�[�^�̃R�s�[
							sTmpFrame.m_dwFrameSize = (dwSize - 1);	// CS�������O
							sTmpFrame.m_byTP = pbyBuff[DATAPOS_TP];
							memcpy(&sTmpFrame.m_byBD[NP_UCOM_BD1], &pbyBuff[DATAPOS_BD], NP_UCOM_SIZE_BD);
							memcpy(&sTmpFrame.m_byD[NP_UCOM_D0],   &pbyBuff[DATAPOS_D],  (sTmpFrame.m_dwFrameSize - (NP_UCOM_SIZE_TP + NP_UCOM_SIZE_BD)));

							// �L���[��push
							if(FALSE == m_pRcvQueue->IsFull()){
								m_pRcvQueue->Push(&sTmpFrame, sizeof(sTmpFrame));
								// �L���[���̕ύX
								++m_dwRcvNum;
							}
							else{
								// �L���[���ӂ�
								OutputErrLog("NPuComCoder.cpp", __LINE__, AIL_ERROR_LOG_CODE_COMDEVICE_ERROR, "DroidAVN", "NPuComCoder::Decode QUEUE OVER FLOW");
							}
						}
					}
				}
				// config one decoded package,reset wait state,reset dwSize
				// DLE1�҂�����ēx����
				m_dwRcvStatus = STATUS_WAIT_DLE1;
				// �\���̂̏�����
				memset(pbyBuff, 0, NP_UCOM_SIZE_DATA_MAX);
				dwSize = 0;
			}
			// other case,wait end error,Reset wait state to wait DLE,reset dwSize
			else{
				// DLE1�҂��ɖ߂�
				m_dwRcvStatus = STATUS_WAIT_DLE1;
				// �\���̂̏�����
				memset(pbyBuff, 0, NP_UCOM_SIZE_DATA_MAX);
				dwSize = 0;
			}
			break;
			//other case, state error
		default:
			// ���肦�Ȃ����ǂƂ肠����
			m_dwRcvStatus = STATUS_WAIT_DLE1;
			// �\���̂̏�����
			memset(pbyBuff, 0, NP_UCOM_SIZE_DATA_MAX);
			dwSize = 0;
			break;
		}
	}

}

/******************************************************************************

    �`�F�b�N�T���m�F

    �y����z
        �`�F�b�N�T���̊m�F���s��

******************************************************************************/
NP_BOOL
NPuComCoder::CheckDataSum(PBYTE pbyCheckData, DWORD dwCheckSize)
{
	DWORD dwChkNum	 = dwCheckSize - 1;
	BYTE  byPacketCS = pbyCheckData[dwChkNum];
	BYTE  byCheckCS  = 0; 

	// 1BYTE�Â�XOR���Ă���
	for(DWORD dwLoop = 0; dwLoop < dwChkNum; ++dwLoop){
		byCheckCS = static_cast<BYTE>(byCheckCS ^ pbyCheckData[dwLoop]);
	}

	// �`�F�b�N
	if(byPacketCS != byCheckCS){
		return NP_FALSE;
	}

	return NP_TRUE;
}

/*===========================================================================*/
/*
 * �y�X�V�����z
 * $Log: NPuComCoder.cpp,v $
 * Revision 1.1  2008/08/08 06:20:03  tomoyuki
 * �V�K�o�^
 *
 */
/*===========================================================================*/
//================================================================= End of File
