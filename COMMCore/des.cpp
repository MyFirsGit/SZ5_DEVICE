#include "stdafx.h"
#include "des.h"
#include "string.h"

void Transfer(unsigned char* lpSrc, unsigned char* lpDest, unsigned char* lpTable);
void KeyGenerate(unsigned char* lpKeyIn, unsigned char* lpKeySub, short nCount);
void dtob(unsigned char Data, unsigned char* lpResult);
void Circle(unsigned char* lpBuf, short nLength);	// to complete left circel shift 1 bit per time
void des_algo(unsigned char* lpSrc, unsigned char* lpDest, unsigned char* lpKey, unsigned char bEncrypt);
void S_change(unsigned char* lpBuf);
void str_xor(unsigned char* lpSrc, unsigned char* lpDest, short nLen);
void str_cpy(unsigned char* lpSrc, unsigned char* lpDest, short nLen);

//! 扩展置换
void Transfer(unsigned char* lpSrc, unsigned char* lpDest, unsigned char* lpTable)
{
  short nTableLength, i;
		
  nTableLength = 0;
  while( lpTable[ nTableLength ] != 255 )	
    nTableLength++;
		
  for( i = 0; i < nTableLength; i++ ) 
  {
    lpDest[ i ] = lpSrc[ lpTable[ i ] ];
  }
}

//! 叠代子密钥生成
void KeyGenerate(unsigned char* lpKeyIn, unsigned char* lpKeySub, short nCount)
{
  unsigned char Buffer[ 56 ];
  unsigned char C0[ 28 ];
  unsigned char D0[ 28 ];
  short i = 0;

  unsigned char shift[] = {
    1,  2,  4,  6,  8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 28
  };

  unsigned char PC_1[] = {
    56, 48, 40, 32, 24, 16,  8,  0, 57, 49, 41, 33, 25, 17,
    9,  1, 58, 50, 42, 34, 26, 18, 10,  2, 59, 51, 43, 35,
    62, 54, 46, 38, 30, 22, 14,  6, 61, 53, 45, 37, 29, 21,
    13,  5, 60, 52, 44, 36, 28, 20, 12,  4, 27, 19, 11,  3,
    255
   };

  unsigned char PC_2[] = {
    13, 16, 10, 23,  0,  4,  2, 27, 14,  5, 20,  9, 22, 18, 11,  3,
    25, 7, 15, 6, 26, 19, 12,  1, 40, 51, 30, 36, 46, 54, 29, 39,
    50, 44,  32, 47,  43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31,
    255
  };

  Transfer( lpKeyIn, Buffer, PC_1 );

  memcpy(C0, Buffer + shift[ nCount ], 28 - shift[ nCount ]);
  memcpy(C0 + 28 - shift[ nCount ], Buffer, shift[ nCount ]);  
  memcpy(D0, Buffer + shift[ nCount ] + 28, 28 - shift[ nCount ]);
  memcpy(D0 + 28 - shift[ nCount ], Buffer + 28, shift[ nCount ]);
  
  memcpy(Buffer, C0, 28);
  memcpy(Buffer + 28, D0, 28);
/*#if 0 //dysh 20111019 edit
  for( i = 0; i < 28; i++ )
  {
    C0[ i ] = Buffer[ i ];
    D0[ i ] = Buffer[ i + 28 ];
  }
#else
  memcpy(C0, Buffer, 28);
  memcpy(D0, Buffer + 28, 28);
#endif

  for( i = 0; i < shift[ nCount ]; i++ )
  {
    Circle( C0, 28 );
    Circle( D0, 28 );
  }

#if 0 //dysh 20111019 edit
  for ( i = 0; i < 28; i++ ) 
  {
    Buffer[ i ] = C0[ i ];
    Buffer[ i + 28 ] = D0[ i ];
  }
#else
  memcpy(Buffer, C0, 28);
  memcpy(Buffer + 28, D0, 28);
#endif*/

  Transfer( Buffer, lpKeySub, PC_2 );
}

//! 字节到位的转换
void dtob(unsigned char Data, unsigned char* lpResult)
{
  lpResult[0] = Data >> 7;
  lpResult[1] = (Data << 1) >> 7;
  lpResult[2] = (Data << 2) >> 7;
  lpResult[3] = (Data << 3) >> 7;
  lpResult[4] = (Data << 4) >> 7;
  lpResult[5] = (Data << 5) >> 7;
  lpResult[6] = (Data << 6) >> 7;
  lpResult[7] = (Data << 7) >> 7;
  /*short i;

  for( i = 0; i < 8; i++ )
  {
    lpResult[ i ] = 0;
    if( Data & 0x80 ) 
      lpResult[ i ] = 1;
    Data = Data << 1;
  }*/
}

//! 循环移位
void Circle(unsigned char* lpBuf, short nLength)	// to complete left circel shift 1 bit per time
{
  unsigned char tmp = lpBuf[ 0 ];
  /*short i;

  tmp = lpBuf[ 0 ];
  for( i = 0; i < nLength - 1; i++ )
    lpBuf[ i ] = lpBuf[ i + 1 ];*/
  memcpy(lpBuf, lpBuf + 1, nLength);
  lpBuf[ nLength - 1 ] = tmp;
}

//! DES计算
void des_algo(unsigned char* lpSrc, unsigned char* lpDest, unsigned char* lpKey, unsigned char bEncrypt)
{
  unsigned char SubKey[ 48 ];
  unsigned char Tmp[ 32 ];
  unsigned char Buffer[ 48 ];
  unsigned char Left[ 32 ];
  unsigned char Right[ 32 ];
  short i;
  unsigned char IP[] = {
    57, 49, 41, 33, 25, 17,  9,  1, 59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5, 63, 55, 47, 39, 31, 23, 15,  7,
    56, 48, 40, 32, 24, 16,  8,  0, 58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4, 62, 54, 46, 38, 30, 22, 14,  6,
    255
  };

  unsigned char IP_1[] = {
    39,  7, 47, 15, 55, 23, 63, 31, 38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29, 36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27, 34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25, 32,  0, 40,  8, 48, 16, 56, 24,
    255
  };

  unsigned char E[] = {
    31,  0,  1,  2,  3,  4,  3,  4,  5,  6,  7,  8,  7,  8,  9, 10,
    11, 12,	11, 12, 13, 14, 15, 16, 15, 16, 17, 18, 19, 20, 19, 20,
    21, 22, 23, 24,	23, 24, 25, 26, 27, 28, 27, 28, 29, 30, 31,  0,
    255
  };

  unsigned char P[] = {
    15,  6, 19, 20, 28, 11, 27, 16,  0, 14, 22, 25,  4, 17, 30,  9,
    1,  7, 23, 13, 31, 26,  2,  8, 18, 12, 29,  5, 21, 10,  3, 24,
    255
  };

  Transfer( lpSrc, lpDest, IP );
  
#if 0 //dysh 20111019 edit
  for( i = 0; i < 32; i++ ) 
  {
    Left[ i ] = lpDest[ i ];
    Right[ i ] = lpDest[ i + 32 ];
  }
#else
  memcpy(Left, lpDest, 32);
  memcpy(Right, lpDest + 32, 32);
#endif

  //! 主循环
  for( i = 0; i < 16; i++ )
  {			
    if( bEncrypt )
      KeyGenerate( lpKey, SubKey, i );
    else
      KeyGenerate( lpKey, SubKey, 15 - i );
    //str_cpy( Right, Tmp, 32 );
    memcpy(Tmp, Right, 32);

    Transfer( Right, Buffer, E );
    str_xor( SubKey, Buffer, 48 );
    S_change( Buffer );
    Transfer( Buffer, Right, P );
    
    str_xor( Left, Right, 32 );
    //str_cpy( Tmp, Left, 32 );
    memcpy(Left, Tmp, 32);
  }

#if 0 //dysh 20111019 edit
  for( i = 0; i < 32; i++ ) 
  {
    lpSrc[ i ] = Right[ i ];
    lpSrc[ 32 + i ] = Left[ i ];
  }
#else
  memcpy(lpSrc, Right, 32);
  memcpy(lpSrc + 32, Left, 32);
#endif

  Transfer( lpSrc, lpDest, IP_1 );
}

//! S盒转换
void S_change(unsigned char* lpBuf)
{
  unsigned char Src[ 8 ][ 6 ];
  unsigned char Dest[ 8 ][ 4 ];
  short	 i, j;
  short nAdr;

  unsigned char S[ 8 ][ 64 ] = {
    {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
     0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
     4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
     15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
    },

    {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
     3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
     0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
     13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
    },

    {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
     13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
     13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
     1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
    },

    { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
     13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
     10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
     3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
    },

    { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
     14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
     4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
     11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
    },

    {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
     10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
     9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
     4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
    },

    { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
     13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
     1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
     6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
    },

    {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
     1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
     7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
     2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
    }
  };
		
  for( i = 0; i < 8; i++ ) 
    for( j = 0; j < 6; j++ ) 
      Src[ i ][ j ] = lpBuf[ i * 6 + j ];
  
  for( i = 0; i < 8; i++ ) 
  {
    j = Src[ i ][ 1 ] * 8 + Src[ i ][ 2 ] * 4 + Src[ i ][ 3 ] * 2 + Src[ i ][ 4 ];
    nAdr = ( Src[ i ][ 0 ] * 2 + Src[ i ][ 5 ] ) * 16 + j;
    j = S[ i ][ nAdr ];
    Dest[ i ][ 0 ] = j >> 3;//j / 8;
    j -= ((j >> 3) << 3);//j %= 8;
    Dest[ i ][ 1 ] = j >> 2;//j / 4;
    j -= ((j >> 2) << 2);//j %= 4;
    Dest[ i ][ 2 ] = j >> 1;//j / 2;
    Dest[ i ][ 3 ] = (j & 0x01);//j % 2;
  }

  for( i = 0; i < 8; i++ ) 
    for( j = 0; j < 4; j++ )
      lpBuf[ i * 4 + j ] = Dest[ i ][ j ];
}

//! 字符串异或运算
void str_xor(unsigned char* lpSrc, unsigned char* lpDest, short nLen)
{
#if 0 //dysh 20111019 edit
  int i;
  for( i = 0; i < nLen; i++ )
    lpDest[ i ] = ( lpSrc[ i ] + lpDest[ i ] ) & 0x01;//% 2;
#else
    lpDest[ 0 ] = ( lpSrc[ 0 ] + lpDest[ 0 ] ) & 0x01;//% 2;
    lpDest[ 1 ] = ( lpSrc[ 1 ] + lpDest[ 1 ] ) & 0x01;//% 2;
    lpDest[ 2 ] = ( lpSrc[ 2 ] + lpDest[ 2 ] ) & 0x01;//% 2;
    lpDest[ 3 ] = ( lpSrc[ 3 ] + lpDest[ 3 ] ) & 0x01;//% 2;
    lpDest[ 4 ] = ( lpSrc[ 4 ] + lpDest[ 4 ] ) & 0x01;//% 2;
    lpDest[ 5 ] = ( lpSrc[ 5 ] + lpDest[ 5 ] ) & 0x01;//% 2;
    lpDest[ 6 ] = ( lpSrc[ 6 ] + lpDest[ 6 ] ) & 0x01;//% 2;
    lpDest[ 7 ] = ( lpSrc[ 7 ] + lpDest[ 7 ] ) & 0x01;//% 2;    
    
    lpDest[ 8 ] = ( lpSrc[ 8 ] + lpDest[ 8 ] ) & 0x01;//% 2;
    lpDest[ 9 ] = ( lpSrc[ 9 ] + lpDest[ 9 ] ) & 0x01;//% 2;
    lpDest[ 10 ] = ( lpSrc[ 10 ] + lpDest[ 10 ] ) & 0x01;//% 2;
    lpDest[ 11 ] = ( lpSrc[ 11 ] + lpDest[ 11 ] ) & 0x01;//% 2;
    lpDest[ 12 ] = ( lpSrc[ 12 ] + lpDest[ 12 ] ) & 0x01;//% 2;
    lpDest[ 13 ] = ( lpSrc[ 13 ] + lpDest[ 13 ] ) & 0x01;//% 2;
    lpDest[ 14 ] = ( lpSrc[ 14 ] + lpDest[ 14 ] ) & 0x01;//% 2;
    lpDest[ 15 ] = ( lpSrc[ 15 ] + lpDest[ 15 ] ) & 0x01;//% 2;    
    
    lpDest[ 16 ] = ( lpSrc[ 16 ] + lpDest[ 16 ] ) & 0x01;//% 2;
    lpDest[ 17 ] = ( lpSrc[ 17 ] + lpDest[ 17 ] ) & 0x01;//% 2;
    lpDest[ 18 ] = ( lpSrc[ 18 ] + lpDest[ 18 ] ) & 0x01;//% 2;
    lpDest[ 19 ] = ( lpSrc[ 19 ] + lpDest[ 19 ] ) & 0x01;//% 2;
    lpDest[ 20 ] = ( lpSrc[ 20 ] + lpDest[ 20 ] ) & 0x01;//% 2;
    lpDest[ 21 ] = ( lpSrc[ 21 ] + lpDest[ 21 ] ) & 0x01;//% 2;
    lpDest[ 22 ] = ( lpSrc[ 22 ] + lpDest[ 22 ] ) & 0x01;//% 2;
    lpDest[ 23 ] = ( lpSrc[ 23 ] + lpDest[ 23 ] ) & 0x01;//% 2;    
    
    lpDest[ 24 ] = ( lpSrc[ 24 ] + lpDest[ 24 ] ) & 0x01;//% 2;
    lpDest[ 25 ] = ( lpSrc[ 25 ] + lpDest[ 25 ] ) & 0x01;//% 2;
    lpDest[ 26 ] = ( lpSrc[ 26 ] + lpDest[ 26 ] ) & 0x01;//% 2;
    lpDest[ 27 ] = ( lpSrc[ 27 ] + lpDest[ 27 ] ) & 0x01;//% 2;
    lpDest[ 28 ] = ( lpSrc[ 28 ] + lpDest[ 28 ] ) & 0x01;//% 2;
    lpDest[ 29 ] = ( lpSrc[ 29 ] + lpDest[ 29 ] ) & 0x01;//% 2;
    lpDest[ 30 ] = ( lpSrc[ 30 ] + lpDest[ 30 ] ) & 0x01;//% 2;
    lpDest[ 31 ] = ( lpSrc[ 31 ] + lpDest[ 31 ] ) & 0x01;//% 2;
    
    if(48 == nLen)
    {    
         lpDest[ 32 ] = ( lpSrc[ 32 ] + lpDest[ 32 ] ) & 0x01;//% 2; 
         lpDest[ 33 ] = ( lpSrc[ 33 ] + lpDest[ 33 ] ) & 0x01;//% 2; 
         lpDest[ 34 ] = ( lpSrc[ 34 ] + lpDest[ 34 ] ) & 0x01;//% 2; 
         lpDest[ 35 ] = ( lpSrc[ 35 ] + lpDest[ 35 ] ) & 0x01;//% 2; 
         lpDest[ 36 ] = ( lpSrc[ 36 ] + lpDest[ 36 ] ) & 0x01;//% 2; 
         lpDest[ 37 ] = ( lpSrc[ 37 ] + lpDest[ 37 ] ) & 0x01;//% 2; 
         lpDest[ 38 ] = ( lpSrc[ 38 ] + lpDest[ 38 ] ) & 0x01;//% 2; 
         lpDest[ 39 ] = ( lpSrc[ 39 ] + lpDest[ 39 ] ) & 0x01;//% 2; 
                                                                     
         lpDest[ 40 ] = ( lpSrc[ 40 ] + lpDest[ 40 ] ) & 0x01;//% 2; 
         lpDest[ 41 ] = ( lpSrc[ 41 ] + lpDest[ 41 ] ) & 0x01;//% 2; 
         lpDest[ 42 ] = ( lpSrc[ 42 ] + lpDest[ 42 ] ) & 0x01;//% 2; 
         lpDest[ 43 ] = ( lpSrc[ 43 ] + lpDest[ 43 ] ) & 0x01;//% 2; 
         lpDest[ 44 ] = ( lpSrc[ 44 ] + lpDest[ 44 ] ) & 0x01;//% 2; 
         lpDest[ 45 ] = ( lpSrc[ 45 ] + lpDest[ 45 ] ) & 0x01;//% 2; 
         lpDest[ 46 ] = ( lpSrc[ 46 ] + lpDest[ 46 ] ) & 0x01;//% 2; 
         lpDest[ 47 ] = ( lpSrc[ 47 ] + lpDest[ 47 ] ) & 0x01;//% 2; 
    }
#endif
}

//! 字符串拷贝
void str_cpy(unsigned char* lpSrc, unsigned char* lpDest, short nLen)
{
  /*short i;

  for( i = 0; i < nLen; i++ )
    lpDest[ i ] = lpSrc[ i ];*/
  memcpy(lpDest, lpSrc, nLen);
}

/*
*	8字节数据的DES运算
*
*	bEncrypt 运算模式，true：加密计算，false：解密计算
*	lpSrc 原始数据，要求为8字节
*	lpKey 密钥，8字节长
*	lpResult 计算结果，空间已经分配好为8个字节
*/
void des0(unsigned char bEncrypt, unsigned char *lpSrc, unsigned char *lpKey, unsigned char *lpResult)
{
  unsigned char Src[ 64 ];
  unsigned char Dest[ 64 ];
  unsigned char KeyMain[ 64 ];
  short  i, j;
		
  for( i = 0; i < 8; i++ )
  {
    dtob( lpSrc[ i ], Src + i * 8 );
    dtob( lpKey[ i ], KeyMain + i * 8 );
  }

  des_algo( Src, Dest, KeyMain, bEncrypt );
  for( i = 0; i < 8; i++ )
  {
    lpResult[ i ] = 0;
    for( j = 0; j < 8; j++ ) 
      lpResult[ i ] |= ( 1 << ( 7 - j ) ) * Dest[ 8 * i + j ];
  }
}

/*
*	8字节数据的3DES运算
*
*	bEncrypt 运算模式，true：加密计算，false：解密计算
*	lpSrc 原始数据，要求为8字节
*	lpKey 密钥，16字节长
*	lpResult 计算结果，空间已经分配好为8个字节
*/
void trides0(unsigned char bEncrypt, unsigned char* lpSrc, unsigned char* lpKey, unsigned char* lpResult)
{
  unsigned char Src0[ 8 ];
  unsigned char Key0[ 8 ];
  short i;

  for( i = 0; i < 8; i++ )
  {
    Src0[ i ] = lpSrc[ i ];
    Key0[ i ] = lpKey[ i ];
  }
		
  des0( bEncrypt, Src0, Key0, lpResult );

  bEncrypt = !bEncrypt;
  for( i = 0; i < 8; i++ )
  {
    Src0[ i ] = lpResult[ i ];
    Key0[ i ] = lpKey[ i + 8 ];
  }
  des0( bEncrypt, Src0, Key0, lpResult );

  bEncrypt = !bEncrypt;
  for( i = 0; i < 8; i++ )
  {
    Src0[ i ] = lpResult[ i ];
    Key0[ i ] = lpKey[ i ];
  }
  des0( bEncrypt, Src0, Key0, lpResult );
}

