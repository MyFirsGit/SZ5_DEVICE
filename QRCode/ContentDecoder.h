#if !defined(AFX_DECODER_H__AC3D15D3_0D11_467B_B981_E73FD0B97EB7__INCLUDED_)
#define AFX_DECODER_H__AC3D15D3_0D11_467B_B981_E73FD0B97EB7__INCLUDED_

#include "RsDecode.h"

// �汾��
#define QR_VRESION_S	0 // 1-9
#define QR_VRESION_M	1 // 10-26
#define QR_VRESION_L	2 // 27-40

// ������
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3

// ��������
/*
#define QR_MODE_NUMERAL		0
#define QR_MODE_ALPHABET	1
#define QR_MODE_8BIT		2
#define QR_MODE_HANZI		3
*/

#define MAX_ALLCODEWORD	 3706 // �����������ֵ
#define MAX_DATACODEWORD 2956 // �����������ֵ���汾40-L��
#define MAX_CODEBLOCK	  153 // ģ���������������ֵ������RS���֣�
#define MAX_MODULESIZE	  177 // ģ�������ֵ177*177

// ���������ֱ��
#define NOT_DATA 111

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
using namespace std;
class ContentDecoder  
{
public:
	ContentDecoder();
	virtual ~ContentDecoder();
	
public:
	int m_nCodeSize;//��С
	int m_nLevel;//������
	int m_nVersion;//�汾
	int m_nMaskingNo;//��Ĥ��ʽ
	int m_nVersionGroup;//�汾���1~9 10~26 27~40
	
	string m_strData;//����

	BYTE m_CodeData[MAX_MODULESIZE][MAX_MODULESIZE]; // �洢������λ����

	int m_ncAllCodeWord; //��������
	int m_ncDataCodeWord; //����������
	int m_ncRSCodeWord; //RS����������
	BYTE m_byAllCodeWord[MAX_ALLCODEWORD]; //��������
	BYTE m_byDataCodeWord[MAX_DATACODEWORD]; //������������
	BYTE m_byRSWork[MAX_CODEBLOCK];//RS��������
	int m_nIndex;//λ���е�ǰλ��

public:
	int DecodeData(int nCodeSize,int nVersion,BYTE CodeData[MAX_MODULESIZE][MAX_MODULESIZE]);
	
	//���÷����ݱ��
	void SetFinderPattern();
	void SetAlignmentPattern();
	void SetFormatPattern();
	void SetVersionPattern();

	int GetVersionInfo();//�汾��Ϣ
	int GetFormatInfo();//��ʽ��Ϣ

	void GetCodeWord();//��ȡ����
	int ParseDataCodeWord();//�������֣�����Ϊ����
	WORD GetBitStream(int ncData);//��ȡλ��
	char BinaryToAlphabet(BYTE by);
	void CorrectDataBlocks();

};

#endif // !defined(AFX_DECODER_H__AC3D15D3_0D11_467B_B981_E73FD0B97EB7__INCLUDED_)
