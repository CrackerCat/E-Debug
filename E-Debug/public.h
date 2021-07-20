#pragma once
#include <string>
#include "pluginsdk/bridgemain.h"

//���һЩ���õĺ���


std::string get_shortstring(dsint addr);

//��ȡ��ǰĿ¼
std::string GetCurrentDirA();
std::wstring GetCurrentDirW();


//ʮ����ʮ
unsigned char HexToBin(unsigned char HexCode);
void HexToBin(std::string& HexCode, unsigned char* BinCode);

//ʮ��ʮ��
const char* UCharToStr(unsigned char c);

int ReadInt(unsigned char* pBuf);
unsigned int ReadUInt(unsigned char* pBuf);
unsigned char ReadUChar(unsigned char* pBuf);
unsigned short ReadUShort(unsigned char* pBuf);

std::wstring LocalCpToUtf16(const char* str);

std::string Utf16ToUtf8(const wchar_t* wstr);

std::string LocalCpToUtf8(const char* str);

