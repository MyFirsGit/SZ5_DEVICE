#include "stdafx.h"
#include "QR_Encode.h"
#include<math.h>
#include "QRErrorCode.h"

CQR_Encode::CQR_Encode()
{
}

CQR_Encode::~CQR_Encode()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		数据编码

@param      (i)int nLevel 纠错级别
@param      (i)int nVersion版本
@param      (i)BOOL bAutoExtent 是否自动扩展图片
@param      (i)int nMaskingNo 掩模
@param      (i)LPCSTR lpsSource 数据内容
@param      (i)int ncSource编码数据长度

@retval     BOOL 编码成功时= TRUE，没有数据，或超出容量时= FALSE

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CQR_Encode::EncodeData(int nLevel, int nVersion, BOOL bAutoExtent, int nMaskingNo, LPCSTR lpsSource, int ncSource)
{
	int i, j;

	m_nLevel = nLevel;
	m_nMaskingNo = nMaskingNo;

	// 如果没有指定数据长度，则由lstrlen获得
	int ncLength = ncSource > 0 ? ncSource : lstrlen(lpsSource);

	if (ncLength == 0)
		return QRCODE_BUFFER_LENGTH_LESS; 

	// 版本检查
	int nEncodeVersion = GetEncodeVersion(nVersion, lpsSource, ncLength);

	if (nEncodeVersion == 0)
		return QRCODE_VERSION_ERROR; // 超出容量

	if (nVersion == 0)
	{
		// 自动版本
		m_nVersion = nEncodeVersion;
	}
	else
	{
		if (nEncodeVersion <= nVersion)
		{
			m_nVersion = nVersion;
		}
		else
		{
			if (bAutoExtent)
				m_nVersion = nEncodeVersion; // バージョン(型番)自動拡張
			else
				return QRCODE_VERSION_ERROR; // 容量オーバー
		}
	}

	// ターミネータコード"0000"付加
	int ncDataCodeWord = QR_VersonInfo[m_nVersion].ncDataCodeWord[nLevel];

	int ncTerminater = min(4, (ncDataCodeWord * 8) - m_ncDataCodeWordBit);

	if (ncTerminater > 0)
		m_ncDataCodeWordBit = SetBitStream(m_ncDataCodeWordBit, 0, ncTerminater);

	// パディングコード"11101100, 00010001"付加
	BYTE byPaddingCode = 0xec;

	for (i = (m_ncDataCodeWordBit + 7) / 8; i < ncDataCodeWord; ++i)
	{
		m_byDataCodeWord[i] = byPaddingCode;

		byPaddingCode = (BYTE)(byPaddingCode == 0xec ? 0x11 : 0xec);
	}

	// 総コードワード算出エリアクリア
	m_ncAllCodeWord = QR_VersonInfo[m_nVersion].ncAllCodeWord;
	ZeroMemory(m_byAllCodeWord, m_ncAllCodeWord);

	int nDataCwIndex = 0; // データコードワード処理位置

	// データブロック分割数
	int ncBlock1 = QR_VersonInfo[m_nVersion].RS_BlockInfo1[nLevel].ncRSBlock;
	int ncBlock2 = QR_VersonInfo[m_nVersion].RS_BlockInfo2[nLevel].ncRSBlock;
	int ncBlockSum = ncBlock1 + ncBlock2;

	int nBlockNo = 0; // 処理中ブロック番号

	// ブロック別データコードワード数
	int ncDataCw1 = QR_VersonInfo[m_nVersion].RS_BlockInfo1[nLevel].ncDataCodeWord;
	int ncDataCw2 = QR_VersonInfo[m_nVersion].RS_BlockInfo2[nLevel].ncDataCodeWord;

	// データコードワードインターリーブ配置
	for (i = 0; i < ncBlock1; ++i)
	{
		for (j = 0; j < ncDataCw1; ++j)
		{
			m_byAllCodeWord[(ncBlockSum * j) + nBlockNo] = m_byDataCodeWord[nDataCwIndex++];
		}

		++nBlockNo;
	}

	for (i = 0; i < ncBlock2; ++i)
	{
		for (j = 0; j < ncDataCw2; ++j)
		{
			if (j < ncDataCw1)
			{
				m_byAllCodeWord[(ncBlockSum * j) + nBlockNo] = m_byDataCodeWord[nDataCwIndex++];
			}
			else
			{
				// ２種目ブロック端数分配置
				m_byAllCodeWord[(ncBlockSum * ncDataCw1) + i]  = m_byDataCodeWord[nDataCwIndex++];
			}	
		}

		++nBlockNo;
	}

	// ブロック別ＲＳコードワード数(※現状では同数)
	int ncRSCw1 = QR_VersonInfo[m_nVersion].RS_BlockInfo1[nLevel].ncAllCodeWord - ncDataCw1;
	int ncRSCw2 = QR_VersonInfo[m_nVersion].RS_BlockInfo2[nLevel].ncAllCodeWord - ncDataCw2;

	/////////////////////////////////////////////////////////////////////////
	// ＲＳコードワード算出

	nDataCwIndex = 0;
	nBlockNo = 0;

	for (i = 0; i < ncBlock1; ++i)
	{
		ZeroMemory(m_byRSWork, sizeof(m_byRSWork));

		memmove(m_byRSWork, m_byDataCodeWord + nDataCwIndex, ncDataCw1);

		GetRSCodeWord(m_byRSWork, ncDataCw1, ncRSCw1);

		// ＲＳコードワード配置
		for (j = 0; j < ncRSCw1; ++j)
		{
			m_byAllCodeWord[ncDataCodeWord + (ncBlockSum * j) + nBlockNo] = m_byRSWork[j];
		}

		nDataCwIndex += ncDataCw1;
		++nBlockNo;
	}

	for (i = 0; i < ncBlock2; ++i)
	{
		ZeroMemory(m_byRSWork, sizeof(m_byRSWork));

		memmove(m_byRSWork, m_byDataCodeWord + nDataCwIndex, ncDataCw2);

		GetRSCodeWord(m_byRSWork, ncDataCw2, ncRSCw2);

		// ＲＳコードワード配置
		for (j = 0; j < ncRSCw2; ++j)
		{
			m_byAllCodeWord[ncDataCodeWord + (ncBlockSum * j) + nBlockNo] = m_byRSWork[j];
		}

		nDataCwIndex += ncDataCw2;
		++nBlockNo;
	}

	m_nSymbleSize = m_nVersion * 4 + 17;

	// モジュール配置
	FormatModule();

	return QRCODE_OK;
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::GetEncodeVersion
// 用  途：エンコード時バージョン(型番)取得
// 引  数：調査開始バージョン、エンコードデータ、エンコードデータ長
// 戻り値：バージョン番号（容量オーバー時=0）

int CQR_Encode::GetEncodeVersion(int nVersion, LPCSTR lpsSource, int ncLength)
{
	int nVerGroup = nVersion >= 27 ? QR_VRESION_L : (nVersion >= 10 ? QR_VRESION_M : QR_VRESION_S);
	int i, j;

	for (i = nVerGroup; i <= QR_VRESION_L; ++i)
	{
		if (EncodeSourceData(lpsSource, ncLength, i))
		{
			if (i == QR_VRESION_S)
			{
				for (j = 1; j <= 9; ++j)
				{
					if ((m_ncDataCodeWordBit + 7) / 8 <= QR_VersonInfo[j].ncDataCodeWord[m_nLevel])
						return j;
				}
			}
			else if (i == QR_VRESION_M)
			{
				for (j = 10; j <= 26; ++j)
				{
					if ((m_ncDataCodeWordBit + 7) / 8 <= QR_VersonInfo[j].ncDataCodeWord[m_nLevel])
						return j;
				}
			}
			else if (i == QR_VRESION_L)
			{
				for (j = 27; j <= 40; ++j)
				{
					if ((m_ncDataCodeWordBit + 7) / 8 <= QR_VersonInfo[j].ncDataCodeWord[m_nLevel])
						return j;
				}
			}
		}
	}

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::EncodeSourceData
// 用  途：入力データエンコード
// 引  数：入力データ、入力データ長、バージョン(型番)グループ
// 戻り値：エンコード成功時=TRUE

BOOL CQR_Encode::EncodeSourceData(LPCSTR lpsSource, int ncLength, int nVerGroup)
{
	ZeroMemory(m_nBlockLength, sizeof(m_nBlockLength));

	int i, j;

	// どのモードが何文字(バイト)継続しているかを調査
	for (m_ncDataBlock = i = 0; i < ncLength; ++i)
	{
		BYTE byMode;

		if (i < ncLength - 1 && IsKanjiData(lpsSource[i], lpsSource[i + 1]))
			byMode = QR_MODE_KANJI;
		else if (IsNumeralData(lpsSource[i]))
			byMode = QR_MODE_NUMERAL;
		else if (IsAlphabetData(lpsSource[i]))
			byMode = QR_MODE_ALPHABET;
		else
			byMode = QR_MODE_8BIT;

		if (i == 0)
			m_byBlockMode[0] = byMode;

		if (m_byBlockMode[m_ncDataBlock] != byMode)
			m_byBlockMode[++m_ncDataBlock] = byMode;

		++m_nBlockLength[m_ncDataBlock];

		if (byMode == QR_MODE_KANJI)
		{
			// 漢字は文字数ではなく	数で記録
			++m_nBlockLength[m_ncDataBlock];
			++i;
		}
	}

	++m_ncDataBlock;

	/////////////////////////////////////////////////////////////////////////
	// 隣接する英数字モードブロックと数字モードブロックの並びをを条件により結合

	int ncSrcBits, ncDstBits; // 元のビット長と単一の英数字モードブロック化した場合のビット長

	int nBlock = 0;

	while (nBlock < m_ncDataBlock - 1)
	{
		int ncJoinFront, ncJoinBehind; // 前後８ビットバイトモードブロックと結合した場合のビット長
		int nJoinPosition = 0; // ８ビットバイトモードブロックとの結合：-1=前と結合、0=結合しない、1=後ろと結合

		// 「数字－英数字」または「英数字－数字」の並び
		if ((m_byBlockMode[nBlock] == QR_MODE_NUMERAL  && m_byBlockMode[nBlock + 1] == QR_MODE_ALPHABET) ||
			(m_byBlockMode[nBlock] == QR_MODE_ALPHABET && m_byBlockMode[nBlock + 1] == QR_MODE_NUMERAL))
		{
			// 元のビット長と単一の英数字モードブロック化した場合のビット長を比較
			ncSrcBits = GetBitLength(m_byBlockMode[nBlock], m_nBlockLength[nBlock], nVerGroup) +
						GetBitLength(m_byBlockMode[nBlock + 1], m_nBlockLength[nBlock + 1], nVerGroup);

			ncDstBits = GetBitLength(QR_MODE_ALPHABET, m_nBlockLength[nBlock] + m_nBlockLength[nBlock + 1], nVerGroup);

			if (ncSrcBits > ncDstBits)
			{
				// 前後に８ビットバイトモードブロックがある場合、それらとの結合が有利かどうかをチェック
				if (nBlock >= 1 && m_byBlockMode[nBlock - 1] == QR_MODE_8BIT)
				{
					// 前に８ビットバイトモードブロックあり
					ncJoinFront = GetBitLength(QR_MODE_8BIT, m_nBlockLength[nBlock - 1] + m_nBlockLength[nBlock], nVerGroup) +
								  GetBitLength(m_byBlockMode[nBlock + 1], m_nBlockLength[nBlock + 1], nVerGroup);

					if (ncJoinFront > ncDstBits + GetBitLength(QR_MODE_8BIT, m_nBlockLength[nBlock - 1], nVerGroup))
						ncJoinFront = 0; // ８ビットバイトモードブロックとは結合しない
				}
				else
					ncJoinFront = 0;

				if (nBlock < m_ncDataBlock - 2 && m_byBlockMode[nBlock + 2] == QR_MODE_8BIT)
				{
					// 後ろに８ビットバイトモードブロックあり
					ncJoinBehind = GetBitLength(m_byBlockMode[nBlock], m_nBlockLength[nBlock], nVerGroup) +
								   GetBitLength(QR_MODE_8BIT, m_nBlockLength[nBlock + 1] + m_nBlockLength[nBlock + 2], nVerGroup);

					if (ncJoinBehind > ncDstBits + GetBitLength(QR_MODE_8BIT, m_nBlockLength[nBlock + 2], nVerGroup))
						ncJoinBehind = 0; // ８ビットバイトモードブロックとは結合しない
				}
				else
					ncJoinBehind = 0;

				if (ncJoinFront != 0 && ncJoinBehind != 0)
				{
					// 前後両方に８ビットバイトモードブロックがある場合はデータ長が短くなる方を優先
					nJoinPosition = (ncJoinFront < ncJoinBehind) ? -1 : 1;
				}
				else
				{
					nJoinPosition = (ncJoinFront != 0) ? -1 : ((ncJoinBehind != 0) ? 1 : 0);
				}

				if (nJoinPosition != 0)
				{
					// ８ビットバイトモードブロックとの結合
					if (nJoinPosition == -1)
					{
						m_nBlockLength[nBlock - 1] += m_nBlockLength[nBlock];

						// 後続をシフト
						for (i = nBlock; i < m_ncDataBlock - 1; ++i)
						{
							m_byBlockMode[i]  = m_byBlockMode[i + 1];
							m_nBlockLength[i] = m_nBlockLength[i + 1];
						}
					}
					else
					{
						m_byBlockMode[nBlock + 1] = QR_MODE_8BIT;
						m_nBlockLength[nBlock + 1] += m_nBlockLength[nBlock + 2];

						// 後続をシフト
						for (i = nBlock + 2; i < m_ncDataBlock - 1; ++i)
						{
							m_byBlockMode[i]  = m_byBlockMode[i + 1];
							m_nBlockLength[i] = m_nBlockLength[i + 1];
						}
					}

					--m_ncDataBlock;
				}
				else
				{
					// 英数字と数字の並びを単一の英数字モードブロックに統合

					if (nBlock < m_ncDataBlock - 2 && m_byBlockMode[nBlock + 2] == QR_MODE_ALPHABET)
					{
						// 結合しようとするブロックの後ろに続く英数字モードブロックを結合
						m_nBlockLength[nBlock + 1] += m_nBlockLength[nBlock + 2];

						// 後続をシフト
						for (i = nBlock + 2; i < m_ncDataBlock - 1; ++i)
						{
							m_byBlockMode[i]  = m_byBlockMode[i + 1];
							m_nBlockLength[i] = m_nBlockLength[i + 1];
						}

						--m_ncDataBlock;
					}

					m_byBlockMode[nBlock] = QR_MODE_ALPHABET;
					m_nBlockLength[nBlock] += m_nBlockLength[nBlock + 1];

					// 後続をシフト
					for (i = nBlock + 1; i < m_ncDataBlock - 1; ++i)
					{
						m_byBlockMode[i]  = m_byBlockMode[i + 1];
						m_nBlockLength[i] = m_nBlockLength[i + 1];
					}

					--m_ncDataBlock;

					if (nBlock >= 1 && m_byBlockMode[nBlock - 1] == QR_MODE_ALPHABET)
					{
						// 結合したブロックの前の英数字モードブロックを結合
						m_nBlockLength[nBlock - 1] += m_nBlockLength[nBlock];

						// 後続をシフト
						for (i = nBlock; i < m_ncDataBlock - 1; ++i)
						{
							m_byBlockMode[i]  = m_byBlockMode[i + 1];
							m_nBlockLength[i] = m_nBlockLength[i + 1];
						}

						--m_ncDataBlock;
					}
				}

				continue; // 現在位置のブロックを再調査
			}
		}

		++nBlock; // 次ブロックを調査
	}

	/////////////////////////////////////////////////////////////////////////
	// 連続する短いモードブロックを８ビットバイトモードブロック化

	nBlock = 0;

	while (nBlock < m_ncDataBlock - 1)
	{
		ncSrcBits = GetBitLength(m_byBlockMode[nBlock], m_nBlockLength[nBlock], nVerGroup)
					+ GetBitLength(m_byBlockMode[nBlock + 1], m_nBlockLength[nBlock + 1], nVerGroup);

		ncDstBits = GetBitLength(QR_MODE_8BIT, m_nBlockLength[nBlock] + m_nBlockLength[nBlock + 1], nVerGroup);

		// 前に８ビットバイトモードブロックがある場合、重複するインジケータ分を減算
		if (nBlock >= 1 && m_byBlockMode[nBlock - 1] == QR_MODE_8BIT)
			ncDstBits -= (4 + nIndicatorLen8Bit[nVerGroup]);

		// 後ろに８ビットバイトモードブロックがある場合、重複するインジケータ分を減算
		if (nBlock < m_ncDataBlock - 2 && m_byBlockMode[nBlock + 2] == QR_MODE_8BIT)
			ncDstBits -= (4 + nIndicatorLen8Bit[nVerGroup]);

		if (ncSrcBits > ncDstBits)
		{
			if (nBlock >= 1 && m_byBlockMode[nBlock - 1] == QR_MODE_8BIT)
			{
				// 結合するブロックの前にある８ビットバイトモードブロックを結合
				m_nBlockLength[nBlock - 1] += m_nBlockLength[nBlock];

				// 後続をシフト
				for (i = nBlock; i < m_ncDataBlock - 1; ++i)
				{
					m_byBlockMode[i]  = m_byBlockMode[i + 1];
					m_nBlockLength[i] = m_nBlockLength[i + 1];
				}

				--m_ncDataBlock;
				--nBlock;
			}

			if (nBlock < m_ncDataBlock - 2 && m_byBlockMode[nBlock + 2] == QR_MODE_8BIT)
			{
				// 結合するブロックの後ろにある８ビットバイトモードブロックを結合
				m_nBlockLength[nBlock + 1] += m_nBlockLength[nBlock + 2];

				// 後続をシフト
				for (i = nBlock + 2; i < m_ncDataBlock - 1; ++i)
				{
					m_byBlockMode[i]  = m_byBlockMode[i + 1];
					m_nBlockLength[i] = m_nBlockLength[i + 1];
				}

				--m_ncDataBlock;
			}

			m_byBlockMode[nBlock] = QR_MODE_8BIT;
			m_nBlockLength[nBlock] += m_nBlockLength[nBlock + 1];

			// 後続をシフト
			for (i = nBlock + 1; i < m_ncDataBlock - 1; ++i)
			{
				m_byBlockMode[i]  = m_byBlockMode[i + 1];
				m_nBlockLength[i] = m_nBlockLength[i + 1];
			}

			--m_ncDataBlock;

			// 結合したブロックの前から再調査
			if (nBlock >= 1)
				--nBlock;

			continue;
		}

		++nBlock; // 次ブロックを調査
	}

	/////////////////////////////////////////////////////////////////////////
	// ビット配列化
	int ncComplete = 0; // 処理済データカウンタ
	WORD wBinCode;

	m_ncDataCodeWordBit = 0; // ビット単位処理カウンタ

	ZeroMemory(m_byDataCodeWord, MAX_DATACODEWORD);

	for (i = 0; i < m_ncDataBlock && m_ncDataCodeWordBit != -1; ++i)
	{
		if (m_byBlockMode[i] == QR_MODE_NUMERAL)
		{
			/////////////////////////////////////////////////////////////////
			// 数字モード

			// インジケータ(0001b)
			m_ncDataCodeWordBit = SetBitStream(m_ncDataCodeWordBit, 1, 4); 

			// 文字数セット
			m_ncDataCodeWordBit = SetBitStream(m_ncDataCodeWordBit, (WORD)m_nBlockLength[i], nIndicatorLenNumeral[nVerGroup]);

			// ビット列保存
			for (j = 0; j < m_nBlockLength[i]; j += 3)
			{
				if (j < m_nBlockLength[i] - 2)
				{
					wBinCode = (WORD)(((lpsSource[ncComplete + j]	  - '0') * 100) +
									  ((lpsSource[ncComplete + j + 1] - '0') * 10) +
									   (lpsSource[ncComplete + j + 2] - '0'));

					m_ncDataCodeWordBit = SetBitStream(m_ncDataCodeWordBit, wBinCode, 10);
				}
				else if (j == m_nBlockLength[i] - 2)
				{
					// 端数２バイト
					wBinCode = (WORD)(((lpsSource[ncComplete + j] - '0') * 10) +
									   (lpsSource[ncComplete + j + 1] - '0'));

					m_ncDataCodeWordBit = SetBitStream(m_ncDataCodeWordBit, wBinCode, 7);
				}
				else if (j == m_nBlockLength[i] - 1)
				{
					// 端数１バイト
					wBinCode = (WORD)(lpsSource[ncComplete + j] - '0');

					m_ncDataCodeWordBit = SetBitStream(m_ncDataCodeWordBit, wBinCode, 4);
				}
			}

			ncComplete += m_nBlockLength[i];
		}

		else if (m_byBlockMode[i] == QR_MODE_ALPHABET)
		{
			/////////////////////////////////////////////////////////////////
			// 英数字モード

			// モードインジケータ(0010b)
			m_ncDataCodeWordBit = SetBitStream(m_ncDataCodeWordBit, 2, 4);

			// 文字数セット
			m_ncDataCodeWordBit = SetBitStream(m_ncDataCodeWordBit, (WORD)m_nBlockLength[i], nIndicatorLenAlphabet[nVerGroup]);

			// ビット列保存
			for (j = 0; j < m_nBlockLength[i]; j += 2)
			{
				if (j < m_nBlockLength[i] - 1)
				{
					wBinCode = (WORD)((AlphabetToBinaly(lpsSource[ncComplete + j]) * 45) +
									   AlphabetToBinaly(lpsSource[ncComplete + j + 1]));

					m_ncDataCodeWordBit = SetBitStream(m_ncDataCodeWordBit, wBinCode, 11);
				}
				else
				{
					// 端数１バイト
					wBinCode = (WORD)AlphabetToBinaly(lpsSource[ncComplete + j]);

					m_ncDataCodeWordBit = SetBitStream(m_ncDataCodeWordBit, wBinCode, 6);
				}
			}

			ncComplete += m_nBlockLength[i];
		}

		else if (m_byBlockMode[i] == QR_MODE_8BIT)
		{
			/////////////////////////////////////////////////////////////////
			// ８ビットバイトモード

			// モードインジケータ(0100b)
			m_ncDataCodeWordBit = SetBitStream(m_ncDataCodeWordBit, 4, 4);

			// 文字数セット
			m_ncDataCodeWordBit = SetBitStream(m_ncDataCodeWordBit, (WORD)m_nBlockLength[i], nIndicatorLen8Bit[nVerGroup]);

			// ビット列保存
			for (j = 0; j < m_nBlockLength[i]; ++j)
			{
				m_ncDataCodeWordBit = SetBitStream(m_ncDataCodeWordBit, (WORD)lpsSource[ncComplete + j], 8);
			}

			ncComplete += m_nBlockLength[i];
		}
		else // m_byBlockMode[i] == QR_MODE_KANJI
		{
			/////////////////////////////////////////////////////////////////
			// 漢字モード

			// モードインジケータ(1000b)
			m_ncDataCodeWordBit = SetBitStream(m_ncDataCodeWordBit, 8, 4);

			// 文字数セット
			m_ncDataCodeWordBit = SetBitStream(m_ncDataCodeWordBit, (WORD)(m_nBlockLength[i] / 2), nIndicatorLenKanji[nVerGroup]);

			// 漢字モードでビット列保存
			for (j = 0; j < m_nBlockLength[i] / 2; ++j)
			{
				WORD wBinCode = KanjiToBinaly((WORD)(((BYTE)lpsSource[ncComplete + (j * 2)] << 8) + (BYTE)lpsSource[ncComplete + (j * 2) + 1]));

				m_ncDataCodeWordBit = SetBitStream(m_ncDataCodeWordBit, wBinCode, 13);
			}

			ncComplete += m_nBlockLength[i];
		}
	}

	return (m_ncDataCodeWordBit != -1);
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::GetBitLength
// 用  途：ビット長取得
// 引  数：データモード種別、データ長、バージョン(型番)グループ
// 戻り値：データビット長
// 備  考：漢字モードでのデータ長引数は文字数ではなくバイト数

int CQR_Encode::GetBitLength(BYTE nMode, int ncData, int nVerGroup)
{
	int ncBits = 0;

	switch (nMode)
	{
	case QR_MODE_NUMERAL:
		ncBits = 4 + nIndicatorLenNumeral[nVerGroup] + (10 * (ncData / 3));
		switch (ncData % 3)
		{
		case 1:
			ncBits += 4;
			break;
		case 2:
			ncBits += 7;
			break;
		default: // case 0:
			break;
		}

		break;

	case QR_MODE_ALPHABET:
		ncBits = 4 + nIndicatorLenAlphabet[nVerGroup] + (11 * (ncData / 2)) + (6 * (ncData % 2));
		break;

	case QR_MODE_8BIT:
		ncBits = 4 + nIndicatorLen8Bit[nVerGroup] + (8 * ncData);
		break;

	default: // case QR_MODE_KANJI:
		ncBits = 4 + nIndicatorLenKanji[nVerGroup] + (13 * (ncData / 2));
		break;
	}

	return ncBits;
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::SetBitStream
// 用  途：ビットセット
// 引  数：挿入位置、ビット配列データ、データビット長(最大16)
// 戻り値：次回挿入位置(バッファオーバー時=-1)
// 備  考：m_byDataCodeWord に結果をセット(要ゼロ初期化)

int CQR_Encode::SetBitStream(int nIndex, WORD wData, int ncData)
{
	int i;

	if (nIndex == -1 || nIndex + ncData > MAX_DATACODEWORD * 8)
		return -1;

	for (i = 0; i < ncData; ++i)
	{
		if (wData & (1 << (ncData - i - 1)))
		{
			m_byDataCodeWord[(nIndex + i) / 8] |= 1 << (7 - ((nIndex + i) % 8));
		}
	}

	return nIndex + ncData;
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::IsNumeralData
// 用  途：数字モード該当チェック
// 引  数：調査文字
// 戻り値：該当時=TRUE

BOOL CQR_Encode::IsNumeralData(unsigned char c)
{
	if (c >= '0' && c <= '9')
		return TRUE;

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::IsAlphabetData
// 用  途：英数字モード該当チェック
// 引  数：調査文字
// 戻り値：該当時=TRUE

BOOL CQR_Encode::IsAlphabetData(unsigned char c)
{
	if (c >= '0' && c <= '9')
		return TRUE;

	if (c >= 'A' && c <= 'Z')
		return TRUE;

	if (c == ' ' || c == '$' || c == '%' || c == '*' || c == '+' || c == '-' || c == '.' || c == '/' || c == ':')
		return TRUE;

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::IsKanjiData
// 用  途：漢字モード該当チェック
// 引  数：調査文字（16ビット文字）
// 戻り値：該当時=TRUE
// 備  考：EBBFh 以降の S-JIS は対象外

BOOL CQR_Encode::IsKanjiData(unsigned char c1, unsigned char c2)
{
	if (((c1 >= 0x81 && c1 <= 0x9f) || (c1 >= 0xe0 && c1 <= 0xeb)) && (c2 >= 0x40))
	{
		if ((c1 == 0x9f && c2 > 0xfc) || (c1 == 0xeb && c2 > 0xbf))
			return FALSE;

		return TRUE;
	}

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::AlphabetToBinaly
// 用  途：英数字モード文字のバイナリ化
// 引  数：対象文字
// 戻り値：バイナリ値

BYTE CQR_Encode::AlphabetToBinaly(unsigned char c)
{
	if (c >= '0' && c <= '9') return (unsigned char)(c - '0');

	if (c >= 'A' && c <= 'Z') return (unsigned char)(c - 'A' + 10);

	if (c == ' ') return 36;

	if (c == '$') return 37;

	if (c == '%') return 38;

	if (c == '*') return 39;

	if (c == '+') return 40;

	if (c == '-') return 41;

	if (c == '.') return 42;

	if (c == '/') return 43;

	return 44; // c == ':'
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::KanjiToBinaly
// 用  途：漢字モード文字のバイナリ化
// 引  数：対象文字
// 戻り値：バイナリ値

WORD CQR_Encode::KanjiToBinaly(WORD wc)
{
	if (wc >= 0x8140 && wc <= 0x9ffc)
		wc -= 0x8140;
	else // (wc >= 0xe040 && wc <= 0xebbf)
		wc -= 0xc140;

	return (WORD)(((wc >> 8) * 0xc0) + (wc & 0x00ff));
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::GetRSCodeWord
// 用  途：ＲＳ誤り訂正コードワード取得
// 引  数：データコードワードアドレス、データコードワード長、ＲＳコードワード長
// 備  考：総コードワード分のエリアを確保してから呼び出し

void CQR_Encode::GetRSCodeWord(LPBYTE lpbyRSWork, int ncDataCodeWord, int ncRSCodeWord)
{
	int i, j;

	for (i = 0; i < ncDataCodeWord ; ++i)
	{
		if (lpbyRSWork[0] != 0)
		{
			BYTE nExpFirst = byIntToExp[lpbyRSWork[0]]; // 初項係数より乗数算出

			for (j = 0; j < ncRSCodeWord; ++j)
			{
				// 各項乗数に初項乗数を加算（% 255 → α^255 = 1）
				BYTE nExpElement = (BYTE)(((int)(byRSExp[ncRSCodeWord][j] + nExpFirst)) % 255);

				// 排他論理和による剰余算出
				lpbyRSWork[j] = (BYTE)(lpbyRSWork[j + 1] ^ byExpToInt[nExpElement]);
			}

			// 残り桁をシフト
			for (j = ncRSCodeWord; j < ncDataCodeWord + ncRSCodeWord - 1; ++j)
				lpbyRSWork[j] = lpbyRSWork[j + 1];
		}
		else
		{
			// 残り桁をシフト
			for (j = 0; j < ncDataCodeWord + ncRSCodeWord - 1; ++j)
				lpbyRSWork[j] = lpbyRSWork[j + 1];
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::FormatModule
// 用  途：モジュールへのデータ配置
// 戻り値：一辺のモジュール数

void CQR_Encode::FormatModule()
{
	int i, j;

	ZeroMemory(m_byModuleData, sizeof(m_byModuleData));

	// 機能モジュール配置
	SetFunctionModule();

	// データパターン配置
	SetCodeWordPattern();

	if (m_nMaskingNo == -1)
	{
		// 最適マスキングパターン選択
		m_nMaskingNo = 0;

		SetMaskingPattern(m_nMaskingNo); // マスキング
		SetFormatInfoPattern(m_nMaskingNo); // フォーマット情報パターン配置

		int nMinPenalty = CountPenalty();

		for (i = 1; i <= 7; ++i)
		{
			SetMaskingPattern(i); // マスキング
			SetFormatInfoPattern(i); // フォーマット情報パターン配置

			int nPenalty = CountPenalty();

			if (nPenalty < nMinPenalty)
			{
				nMinPenalty = nPenalty;
				m_nMaskingNo = i;
			}
		}
	}

	SetMaskingPattern(m_nMaskingNo); // マスキング
	SetFormatInfoPattern(m_nMaskingNo); // フォーマット情報パターン配置

	// モジュールパターンをブール値に変換
	for (i = 0; i < m_nSymbleSize; ++i)
	{
		for (j = 0; j < m_nSymbleSize; ++j)
		{
			m_byModuleData[i][j] = (BYTE)((m_byModuleData[i][j] & 0x11) != 0);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::SetFunctionModule
// 用  途：機能モジュール配置
// 備  考：フォーマット情報は機能モジュール登録のみ(実データは空白)

void CQR_Encode::SetFunctionModule()
{
	int i, j;

	// 位置検出パターン
	SetFinderPattern(0, 0);
	SetFinderPattern(m_nSymbleSize - 7, 0);
	SetFinderPattern(0, m_nSymbleSize - 7);

	// 位置検出パターンセパレータ
	for (i = 0; i < 8; ++i)
	{
		m_byModuleData[i][7] = m_byModuleData[7][i] = '\x20';
		m_byModuleData[m_nSymbleSize - 8][i] = m_byModuleData[m_nSymbleSize - 8 + i][7] = '\x20';
		m_byModuleData[i][m_nSymbleSize - 8] = m_byModuleData[7][m_nSymbleSize - 8 + i] = '\x20';
	}

	// フォーマット情報記述位置を機能モジュール部として登録
	for (i = 0; i < 9; ++i)
	{
		m_byModuleData[i][8] = m_byModuleData[8][i] = '\x20';
	}

	for (i = 0; i < 8; ++i)
	{
		m_byModuleData[m_nSymbleSize - 8 + i][8] = m_byModuleData[8][m_nSymbleSize - 8 + i] = '\x20';
	}

	// バージョン情報パターン
	SetVersionPattern();

	// 位置合わせパターン
	for (i = 0; i < QR_VersonInfo[m_nVersion].ncAlignPoint; ++i)
	{
		SetAlignmentPattern(QR_VersonInfo[m_nVersion].nAlignPoint[i], 6);
		SetAlignmentPattern(6, QR_VersonInfo[m_nVersion].nAlignPoint[i]);

		for (j = 0; j < QR_VersonInfo[m_nVersion].ncAlignPoint; ++j)
		{
			SetAlignmentPattern(QR_VersonInfo[m_nVersion].nAlignPoint[i], QR_VersonInfo[m_nVersion].nAlignPoint[j]);
		}
	}

	// タイミングパターン
	for (i = 8; i <= m_nSymbleSize - 9; ++i)
	{
		m_byModuleData[i][6] = (i % 2) == 0 ? '\x30' : '\x20';
		m_byModuleData[6][i] = (i % 2) == 0 ? '\x30' : '\x20';
	}
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::SetFinderPattern
// 用  途：位置検出パターン配置
// 引  数：配置左上座標

void CQR_Encode::SetFinderPattern(int x, int y)
{
	static BYTE byPattern[] = {0x7f,  // 1111111b
							   0x41,  // 1000001b
							   0x5d,  // 1011101b
							   0x5d,  // 1011101b
							   0x5d,  // 1011101b
							   0x41,  // 1000001b
							   0x7f}; // 1111111b
	int i, j;

	for (i = 0; i < 7; ++i)
	{
		for (j = 0; j < 7; ++j)
		{
			m_byModuleData[x + j][y + i] = (byPattern[i] & (1 << (6 - j))) ? '\x30' : '\x20'; 
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::SetAlignmentPattern
// 用  途：位置合わせパターン配置
// 引  数：配置中央座標

void CQR_Encode::SetAlignmentPattern(int x, int y)
{
	static BYTE byPattern[] = {0x1f,  // 11111b
							   0x11,  // 10001b
							   0x15,  // 10101b
							   0x11,  // 10001b
							   0x1f}; // 11111b
	int i, j;

	if (m_byModuleData[x][y] & 0x20)
		return; // 機能モジュールと重複するため除外

	x -= 2; y -= 2; // 左上隅座標に変換

	for (i = 0; i < 5; ++i)
	{
		for (j = 0; j < 5; ++j)
		{
			m_byModuleData[x + j][y + i] = (byPattern[i] & (1 << (4 - j))) ? '\x30' : '\x20'; 
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::SetVersionPattern
// 用  途：バージョン(型番)情報パターン配置
// 備  考：拡張ＢＣＨ(18,6)符号を誤り訂正として使用

void CQR_Encode::SetVersionPattern()
{
	int i, j;

	if (m_nVersion <= 6)
		return;

	int nVerData = m_nVersion << 12;

	// 剰余ビット算出
	for (i = 0; i < 6; ++i)
	{
		if (nVerData & (1 << (17 - i)))
		{
			nVerData ^= (0x1f25 << (5 - i));
		}
	}

	nVerData += m_nVersion << 12;

	for (i = 0; i < 6; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			m_byModuleData[m_nSymbleSize - 11 + j][i] = m_byModuleData[i][m_nSymbleSize - 11 + j] =
			(nVerData & (1 << (i * 3 + j))) ? '\x30' : '\x20';
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::SetCodeWordPattern
// 用  途：データパターン配置

void CQR_Encode::SetCodeWordPattern()
{
	int x = m_nSymbleSize;
	int y = m_nSymbleSize - 1;

	int nCoef_x = 1; // ｘ軸配置向き
	int nCoef_y = 1; // ｙ軸配置向き

	int i, j;

	for (i = 0; i < m_ncAllCodeWord; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			do
			{
				x += nCoef_x;
				nCoef_x *= -1;

				if (nCoef_x < 0)
				{
					y += nCoef_y;

					if (y < 0 || y == m_nSymbleSize)
					{
						y = (y < 0) ? 0 : m_nSymbleSize - 1;
						nCoef_y *= -1;

						x -= 2;

						if (x == 6) // タイミングパターン
							--x;
					}
				}
			}
			while (m_byModuleData[x][y] & 0x20); // 機能モジュールを除外

			m_byModuleData[x][y] = (m_byAllCodeWord[i] & (1 << (7 - j))) ? '\x02' : '\x00';
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::SetMaskingPattern
// 用  途：マスキングパターン配置
// 引  数：マスキングパターン番号

void CQR_Encode::SetMaskingPattern(int nPatternNo)
{
	int i, j;

	for (i = 0; i < m_nSymbleSize; ++i)
	{
		for (j = 0; j < m_nSymbleSize; ++j)
		{
			if (! (m_byModuleData[j][i] & 0x20)) // 機能モジュールを除外
			{
				BOOL bMask;

				switch (nPatternNo)
				{
				case 0:
					bMask = ((i + j) % 2 == 0);
					break;

				case 1:
					bMask = (i % 2 == 0);
					break;

				case 2:
					bMask = (j % 3 == 0);
					break;

				case 3:
					bMask = ((i + j) % 3 == 0);
					break;

				case 4:
					bMask = (((i / 2) + (j / 3)) % 2 == 0);
					break;

				case 5:
					bMask = (((i * j) % 2) + ((i * j) % 3) == 0);
					break;

				case 6:
					bMask = ((((i * j) % 2) + ((i * j) % 3)) % 2 == 0);
					break;

				default: // case 7:
					bMask = ((((i * j) % 3) + ((i + j) % 2)) % 2 == 0);
					break;
				}

				m_byModuleData[j][i] = (BYTE)((m_byModuleData[j][i] & 0xfe) | (((m_byModuleData[j][i] & 0x02) > 1) ^ bMask));
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::SetFormatInfoPattern
// 用  途：フォーマット情報配置
// 引  数：マスキングパターン番号

void CQR_Encode::SetFormatInfoPattern(int nPatternNo)
{
	int nFormatInfo;
	int i;

	switch (m_nLevel)
	{
	case QR_LEVEL_M:
		nFormatInfo = 0x00; // 00nnnb
		break;

	case QR_LEVEL_L:
		nFormatInfo = 0x08; // 01nnnb
		break;

	case QR_LEVEL_Q:
		nFormatInfo = 0x18; // 11nnnb
		break;

	default: // case QR_LEVEL_H:
		nFormatInfo = 0x10; // 10nnnb
		break;
	}

	nFormatInfo += nPatternNo;

	int nFormatData = nFormatInfo << 10;

	// 剰余ビット算出
	for (i = 0; i < 5; ++i)
	{
		if (nFormatData & (1 << (14 - i)))
		{
			nFormatData ^= (0x0537 << (4 - i)); // 10100110111b
		}
	}

	nFormatData += nFormatInfo << 10;

	// マスキング
	nFormatData ^= 0x5412; // 101010000010010b

	// 左上位置検出パターン周り配置
	for (i = 0; i <= 5; ++i)
		m_byModuleData[8][i] = (nFormatData & (1 << i)) ? '\x30' : '\x20';

	m_byModuleData[8][7] = (nFormatData & (1 << 6)) ? '\x30' : '\x20';
	m_byModuleData[8][8] = (nFormatData & (1 << 7)) ? '\x30' : '\x20';
	m_byModuleData[7][8] = (nFormatData & (1 << 8)) ? '\x30' : '\x20';

	for (i = 9; i <= 14; ++i)
		m_byModuleData[14 - i][8] = (nFormatData & (1 << i)) ? '\x30' : '\x20';

	// 右上位置検出パターン下配置
	for (i = 0; i <= 7; ++i)
		m_byModuleData[m_nSymbleSize - 1 - i][8] = (nFormatData & (1 << i)) ? '\x30' : '\x20';

	// 左下位置検出パターン右配置
	m_byModuleData[8][m_nSymbleSize - 8] = '\x30'; // 固定暗モジュール

	for (i = 8; i <= 14; ++i)
		m_byModuleData[8][m_nSymbleSize - 15 + i] = (nFormatData & (1 << i)) ? '\x30' : '\x20';
}


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode::CountPenalty
// 用  途：マスク後ペナルティスコア算出

int CQR_Encode::CountPenalty()
{
	int nPenalty = 0;
	int i, j, k;

	// 同色の列の隣接モジュール
	for (i = 0; i < m_nSymbleSize; ++i)
	{
		for (j = 0; j < m_nSymbleSize - 4; ++j)
		{
			int nCount = 1;

			for (k = j + 1; k < m_nSymbleSize; k++)
			{
				if (((m_byModuleData[i][j] & 0x11) == 0) == ((m_byModuleData[i][k] & 0x11) == 0))
					++nCount;
				else
					break;
			}

			if (nCount >= 5)
			{
				nPenalty += 3 + (nCount - 5);
			}

			j = k - 1;
		}
	}

	// 同色の行の隣接モジュール
	for (i = 0; i < m_nSymbleSize; ++i)
	{
		for (j = 0; j < m_nSymbleSize - 4; ++j)
		{
			int nCount = 1;

			for (k = j + 1; k < m_nSymbleSize; k++)
			{
				if (((m_byModuleData[j][i] & 0x11) == 0) == ((m_byModuleData[k][i] & 0x11) == 0))
					++nCount;
				else
					break;
			}

			if (nCount >= 5)
			{
				nPenalty += 3 + (nCount - 5);
			}

			j = k - 1;
		}
	}

	// 同色のモジュールブロック（２×２）
	for (i = 0; i < m_nSymbleSize - 1; ++i)
	{
		for (j = 0; j < m_nSymbleSize - 1; ++j)
		{
			if ((((m_byModuleData[i][j] & 0x11) == 0) == ((m_byModuleData[i + 1][j]		& 0x11) == 0)) &&
				(((m_byModuleData[i][j] & 0x11) == 0) == ((m_byModuleData[i]	[j + 1] & 0x11) == 0)) &&
				(((m_byModuleData[i][j] & 0x11) == 0) == ((m_byModuleData[i + 1][j + 1] & 0x11) == 0)))
			{
				nPenalty += 3;
			}
		}
	}

	// 同一列における 1:1:3:1:1 比率（暗:明:暗:明:暗）のパターン
	for (i = 0; i < m_nSymbleSize; ++i)
	{
		for (j = 0; j < m_nSymbleSize - 6; ++j)
		{
			if (((j == 0) ||				 (! (m_byModuleData[i][j - 1] & 0x11))) && // 明 または シンボル外
											 (   m_byModuleData[i][j]     & 0x11)   && // 暗 - 1
											 (! (m_byModuleData[i][j + 1] & 0x11))  && // 明 - 1
											 (   m_byModuleData[i][j + 2] & 0x11)   && // 暗 ┐
											 (   m_byModuleData[i][j + 3] & 0x11)   && // 暗 │3
											 (   m_byModuleData[i][j + 4] & 0x11)   && // 暗 ┘
											 (! (m_byModuleData[i][j + 5] & 0x11))  && // 明 - 1
											 (   m_byModuleData[i][j + 6] & 0x11)   && // 暗 - 1
				((j == m_nSymbleSize - 7) || (! (m_byModuleData[i][j + 7] & 0x11))))   // 明 または シンボル外
			{
				// 前または後に4以上の明パターン
				if (((j < 2 || ! (m_byModuleData[i][j - 2] & 0x11)) && 
					 (j < 3 || ! (m_byModuleData[i][j - 3] & 0x11)) &&
					 (j < 4 || ! (m_byModuleData[i][j - 4] & 0x11))) ||
					((j >= m_nSymbleSize - 8  || ! (m_byModuleData[i][j + 8]  & 0x11)) &&
					 (j >= m_nSymbleSize - 9  || ! (m_byModuleData[i][j + 9]  & 0x11)) &&
					 (j >= m_nSymbleSize - 10 || ! (m_byModuleData[i][j + 10] & 0x11))))
				{
					nPenalty += 40;
				}
			}
		}
	}

	// 同一行における 1:1:3:1:1 比率（暗:明:暗:明:暗）のパターン
	for (i = 0; i < m_nSymbleSize; ++i)
	{
		for (j = 0; j < m_nSymbleSize - 6; ++j)
		{
			if (((j == 0) ||				 (! (m_byModuleData[j - 1][i] & 0x11))) && // 明 または シンボル外
											 (   m_byModuleData[j]    [i] & 0x11)   && // 暗 - 1
											 (! (m_byModuleData[j + 1][i] & 0x11))  && // 明 - 1
											 (   m_byModuleData[j + 2][i] & 0x11)   && // 暗 ┐
											 (   m_byModuleData[j + 3][i] & 0x11)   && // 暗 │3
											 (   m_byModuleData[j + 4][i] & 0x11)   && // 暗 ┘
											 (! (m_byModuleData[j + 5][i] & 0x11))  && // 明 - 1
											 (   m_byModuleData[j + 6][i] & 0x11)   && // 暗 - 1
				((j == m_nSymbleSize - 7) || (! (m_byModuleData[j + 7][i] & 0x11))))   // 明 または シンボル外
			{
				// 前または後に4以上の明パターン
				if (((j < 2 || ! (m_byModuleData[j - 2][i] & 0x11)) && 
					 (j < 3 || ! (m_byModuleData[j - 3][i] & 0x11)) &&
					 (j < 4 || ! (m_byModuleData[j - 4][i] & 0x11))) ||
					((j >= m_nSymbleSize - 8  || ! (m_byModuleData[j + 8][i]  & 0x11)) &&
					 (j >= m_nSymbleSize - 9  || ! (m_byModuleData[j + 9][i]  & 0x11)) &&
					 (j >= m_nSymbleSize - 10 || ! (m_byModuleData[j + 10][i] & 0x11))))
				{
					nPenalty += 40;
				}
			}
		}
	}

	// 全体に対する暗モジュールの占める割合
	int nCount = 0;

	for (i = 0; i < m_nSymbleSize; ++i)
	{
		for (j = 0; j < m_nSymbleSize; ++j)
		{
			if (! (m_byModuleData[i][j] & 0x11))
			{
				++nCount;
			}
		}
	}

	nPenalty += (abs(50 - ((nCount * 100) / (m_nSymbleSize * m_nSymbleSize))) / 5) * 10;

	return nPenalty;
}
