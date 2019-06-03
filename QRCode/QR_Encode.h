
#if !defined(AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_)
#define AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QRcodeDef.h"

// 纠错级别
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3

// 数据模式
#define QR_MODE_NUMERAL		0
#define QR_MODE_ALPHABET	1
#define QR_MODE_8BIT		2
#define QR_MODE_KANJI		3

// 版本级别
#define QR_VRESION_S	0 // 1 ～ 9
#define QR_VRESION_M	1 // 10 ～ 26
#define QR_VRESION_L	2 // 27 ～ 40

#define MAX_ALLCODEWORD	 3706 // 总代码字数最大值
#define MAX_DATACODEWORD 2956 // 数据编码字最大值（40）- L版本
#define MAX_CODEBLOCK	  153 // 块数据编码字数最大值（RS代码字在内）
#define MAX_MODULESIZE	  177 // 一边模块数最大值

// 位图绘画是空白处宽度
#define QR_MARGIN	0/*4*/


/**
@brief  QR编码类
*/
class CQR_Encode
{
public:
	CQR_Encode();
	~CQR_Encode();

public:
	int m_nLevel;		// 纠错级别
	int m_nVersion;		// 版本
	BOOL m_bAutoExtent;	// 自动大小或指定大小
	int m_nMaskingNo;	// 掩蔽模式

public:
	int m_nSymbleSize;
	BYTE m_byModuleData[MAX_MODULESIZE][MAX_MODULESIZE]; // [x][y]
	// bit5:機能モジュール（マスキング対象外）フラグ
	// bit4:機能モジュール描画データ
	// bit1:エンコードデータ
	// bit0:マスク後エンコード描画データ
	// 20hとの論理和により機能モジュール判定、11hとの論理和により描画（最終的にはBOOL値化）

private:
	int m_ncDataCodeWordBit; // 数据编码字位长
	BYTE m_byDataCodeWord[MAX_DATACODEWORD]; // 输入数据编码区

	int m_ncDataBlock;
	BYTE m_byBlockMode[MAX_DATACODEWORD];
	int m_nBlockLength[MAX_DATACODEWORD];

	int m_ncAllCodeWord; // 总代码字数（RS错误修正数据在内）
	BYTE m_byAllCodeWord[MAX_ALLCODEWORD]; // 总代码字计算区域
	BYTE m_byRSWork[MAX_CODEBLOCK]; // ＲＳコードワード算出ワーク

// 数据编码相关函数
public:
	int EncodeData(int nLevel, int nVersion, BOOL bAutoExtent, int nMaskingNo, LPCSTR lpsSource, int ncSource = 0);

private:
	int GetEncodeVersion(int nVersion, LPCSTR lpsSource, int ncLength);
	BOOL EncodeSourceData(LPCSTR lpsSource, int ncLength, int nVerGroup);

	int GetBitLength(BYTE nMode, int ncData, int nVerGroup);

	int SetBitStream(int nIndex, WORD wData, int ncData);

	BOOL IsNumeralData(unsigned char c);
	BOOL IsAlphabetData(unsigned char c);
	BOOL IsKanjiData(unsigned char c1, unsigned char c2);

	BYTE AlphabetToBinaly(unsigned char c);
	WORD KanjiToBinaly(WORD wc);

	void GetRSCodeWord(LPBYTE lpbyRSWork, int ncDataCodeWord, int ncRSCodeWord);

// 模块布局相关函数
private:
	void FormatModule();

	void SetFunctionModule();
	void SetFinderPattern(int x, int y);
	void SetAlignmentPattern(int x, int y);
	void SetVersionPattern();
	void SetCodeWordPattern();
	void SetMaskingPattern(int nPatternNo);
	void SetFormatInfoPattern(int nPatternNo);
	int CountPenalty();
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_QR_ENCODE_H__AC886DF7_C0AE_4C9F_AC7A_FCDA8CB1DD37__INCLUDED_)
