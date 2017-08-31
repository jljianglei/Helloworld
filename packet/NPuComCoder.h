/*

* (c) PIONEER CORPORATION 2014

* 25-1 Nishi-machi Yamada Kawagoe-shi Saitama-ken 350-8555 Japan

* All Rights Reserved.

*/

 
/*===========================================================================*/
/** @file
    �R�[�_�N���X��`�t�@�C��

    @attention C++ ��p�ł� C �ł͎g�p�o���܂���
*/
/*===========================================================================*/

#ifndef CXX_NPUCOMCODER_H
#define CXX_NPUCOMCODER_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*=============================================================================
   �C���N���[�h�t�@�C���̓ǂݍ���                                            */

#include "NSyncObject.h"
#include "NPuComHandler_DEF.h"


/*=============================================================================
   �O���N���X�錾                                                            */

class NBinaryQueue;


/*=============================================================================
     �N���X�錾                                                              */

class NPuComCoder;


/*===========================================================================*/
/**
    �R�[�_�N���X

	@author $Author: jjyathefei $
	@version $Revision: 4985 $
*/
/*===========================================================================*/
class NPuComCoder : public NObject
{
public:
        
    /*************************************************************************/
    /**
        �R�[�_�N���X�̃C���X�^���X�̎擾
        
        @param �Ȃ�

        @return UCOM�n���h���v���L�V�̃C���X�^���X
    */
    /*************************************************************************/
	static NPuComCoder
	*Instance( VOID );

    /*************************************************************************/
    /**
        �R�[�_�N���X�̃C���X�^���X�̔j��
        
        @param �Ȃ�

        @return �Ȃ�
    */
    /*************************************************************************/
	static VOID 
	Destroy( VOID );

    /*************************************************************************/
    /**
        �G���R�[�h����
        
        @param psFrame	:	[IN]  �t���[��
        @param psPacket	:	[OUT] �p�P�b�g

        @return �Ȃ�
    */
    /*************************************************************************/
	VOID 
	Encode( NPuComDataFrameUpdate *psFrame, NPuComPacketUpdate *psPacket );

    /*************************************************************************/
    /**
        �p�P�b�g�ݒ�
        
        @param psPacket	:	�p�P�b�g

        @return DWORD	:	�t���[����
    */
    /*************************************************************************/
	DWORD 
	SetPacket( const NPuComPacketUpdate *psPacket );

    /*************************************************************************/
    /**
        �f�[�^�t���[���擾
        
        @param psFrame	:	�t���[��

        @return �Ȃ�
    */
    /*************************************************************************/
	VOID 
	GetFrame( NPuComDataFrame *psFrame );

private:

	static NSyncObject	s_cSync;		//!< �����I�u�W�F�N�g
	static NPuComCoder	*s_pcInstance;	//!< �v���Z�X�ɗB��̃C���X�^���X

	NPuComPacketUpdate	m_sPacket;
	NPuComPacket		m_sStockData;
	NBinaryQueue		*m_pRcvQueue;	//!< ��M�L���[
	DWORD				m_dwRcvNum;
	DWORD				m_dwRcvStatus;

	/*************************************************************************/
    /**
        �R���X�g���N�^
 
        @param	�Ȃ�

        @retval	�Ȃ�
    */
    /*************************************************************************/
	NPuComCoder();

    /*************************************************************************/
    /**
        �f�X�g���N�^
 
        @param	�Ȃ�

        @retval	�Ȃ�
    */
    /*************************************************************************/
	virtual ~NPuComCoder();

	/**
	 * �R�s�[�R���X�g���N�^
	 *
	 * ���p�֎~
	 *
	 * @param const NPuComCoder& src [IN] �R�s�[��
	 *
	 * @return �Ȃ�
	 */
	NPuComCoder(const NPuComCoder& src);

	/**
	 * �R�s�[���Z�q
	 *
	 * ���p�֎~
	 *
	 * @param const NPuComCoder& src [IN] �R�s�[��
	 *
	 * @return const NPuComCoder& ���M�ւ̎Q��
	 */
	const NPuComCoder& operator=( const NPuComCoder& src);

    /*************************************************************************/
    /**
        �f�R�[�h����
        
        @param �Ȃ�

        @return �Ȃ�
    */
    /*************************************************************************/
	VOID 
	Decode(VOID);

    /*************************************************************************/
    /**
        �`�F�b�N�T���m�F
        
        @param pbyCheckData  : [IN] �m�F�f�[�^
	    @param dwCheckSize   : [IN] �m�F�T�C�Y�{�b�r�f�[�^

        @retval NP_TRUE  : �`�F�b�N�T������
        @retval NP_FALSE : �`�F�b�N�T���ُ�
    */
    /*************************************************************************/
	NP_BOOL
	CheckDataSum(PBYTE pbyCheckData, DWORD dwCheckSize);

	/**
	 * Update�p�P�b�g���菈��
	 *
	 * �p�����[�^��TP��Update�p�P�b�g���ǂ����𔻒肷��B
	 *
	 * @param BYTE byTP [IN] �p�P�b�g��TP
	 *
	 * @return NP_BOOL ���茋��
	 *
	 * @retval NP_TRUE : UPdate�p�P�b�g
	 * @retval NP_FALSE : ��UPdate�p�P�b�g
	 */
	NP_BOOL IsUpdatePacket( BYTE byTP);
};


/*===========================================================================*/
/*
 * �y�X�V�����z
 * $Log: NPuComCoder.h,v $
 * Revision 1.1  2008/08/08 06:20:03  tomoyuki
 * �V�K�o�^
 *
 */
/*===========================================================================*/
#endif /* CXX_NPUCOMCODER_H */
//================================================================= End of File
