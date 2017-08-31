/*

* (c) PIONEER CORPORATION 2014

* 25-1 Nishi-machi Yamada Kawagoe-shi Saitama-ken 350-8555 Japan

* All Rights Reserved.

*/

 
/*===========================================================================*/
/** @file
    コーダクラス定義ファイル

    @attention C++ 専用です C では使用出来ません
*/
/*===========================================================================*/

#ifndef CXX_NPUCOMCODER_H
#define CXX_NPUCOMCODER_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*=============================================================================
   インクルードファイルの読み込み                                            */

#include "NSyncObject.h"
#include "NPuComHandler_DEF.h"


/*=============================================================================
   外部クラス宣言                                                            */

class NBinaryQueue;


/*=============================================================================
     クラス宣言                                                              */

class NPuComCoder;


/*===========================================================================*/
/**
    コーダクラス

	@author $Author: jjyathefei $
	@version $Revision: 4985 $
*/
/*===========================================================================*/
class NPuComCoder : public NObject
{
public:
        
    /*************************************************************************/
    /**
        コーダクラスのインスタンスの取得
        
        @param なし

        @return UCOMハンドラプロキシのインスタンス
    */
    /*************************************************************************/
	static NPuComCoder
	*Instance( VOID );

    /*************************************************************************/
    /**
        コーダクラスのインスタンスの破棄
        
        @param なし

        @return なし
    */
    /*************************************************************************/
	static VOID 
	Destroy( VOID );

    /*************************************************************************/
    /**
        エンコード処理
        
        @param psFrame	:	[IN]  フレーム
        @param psPacket	:	[OUT] パケット

        @return なし
    */
    /*************************************************************************/
	VOID 
	Encode( NPuComDataFrameUpdate *psFrame, NPuComPacketUpdate *psPacket );

    /*************************************************************************/
    /**
        パケット設定
        
        @param psPacket	:	パケット

        @return DWORD	:	フレーム数
    */
    /*************************************************************************/
	DWORD 
	SetPacket( const NPuComPacketUpdate *psPacket );

    /*************************************************************************/
    /**
        データフレーム取得
        
        @param psFrame	:	フレーム

        @return なし
    */
    /*************************************************************************/
	VOID 
	GetFrame( NPuComDataFrame *psFrame );

private:

	static NSyncObject	s_cSync;		//!< 同期オブジェクト
	static NPuComCoder	*s_pcInstance;	//!< プロセスに唯一のインスタンス

	NPuComPacketUpdate	m_sPacket;
	NPuComPacket		m_sStockData;
	NBinaryQueue		*m_pRcvQueue;	//!< 受信キュー
	DWORD				m_dwRcvNum;
	DWORD				m_dwRcvStatus;

	/*************************************************************************/
    /**
        コンストラクタ
 
        @param	なし

        @retval	なし
    */
    /*************************************************************************/
	NPuComCoder();

    /*************************************************************************/
    /**
        デストラクタ
 
        @param	なし

        @retval	なし
    */
    /*************************************************************************/
	virtual ~NPuComCoder();

	/**
	 * コピーコンストラクタ
	 *
	 * 利用禁止
	 *
	 * @param const NPuComCoder& src [IN] コピー元
	 *
	 * @return なし
	 */
	NPuComCoder(const NPuComCoder& src);

	/**
	 * コピー演算子
	 *
	 * 利用禁止
	 *
	 * @param const NPuComCoder& src [IN] コピー元
	 *
	 * @return const NPuComCoder& 自信への参照
	 */
	const NPuComCoder& operator=( const NPuComCoder& src);

    /*************************************************************************/
    /**
        デコード処理
        
        @param なし

        @return なし
    */
    /*************************************************************************/
	VOID 
	Decode(VOID);

    /*************************************************************************/
    /**
        チェックサム確認
        
        @param pbyCheckData  : [IN] 確認データ
	    @param dwCheckSize   : [IN] 確認サイズ＋ＣＳデータ

        @retval NP_TRUE  : チェックサム正常
        @retval NP_FALSE : チェックサム異常
    */
    /*************************************************************************/
	NP_BOOL
	CheckDataSum(PBYTE pbyCheckData, DWORD dwCheckSize);

	/**
	 * Updateパケット判定処理
	 *
	 * パラメータのTPがUpdateパケットかどうかを判定する。
	 *
	 * @param BYTE byTP [IN] パケットのTP
	 *
	 * @return NP_BOOL 判定結果
	 *
	 * @retval NP_TRUE : UPdateパケット
	 * @retval NP_FALSE : 非UPdateパケット
	 */
	NP_BOOL IsUpdatePacket( BYTE byTP);
};


/*===========================================================================*/
/*
 * 【更新履歴】
 * $Log: NPuComCoder.h,v $
 * Revision 1.1  2008/08/08 06:20:03  tomoyuki
 * 新規登録
 *
 */
/*===========================================================================*/
#endif /* CXX_NPUCOMCODER_H */
//================================================================= End of File
