#pragma once
#include <vector>
#include <string>


struct SegmentInfomation
{
	unsigned int m_segStart;                   //������ʼ��ַ
	unsigned int m_segSize;                  //���δ�С
	std::string m_segName;                 //��������
	std::vector<unsigned char> m_segData;  //��������
};

class QPlainTextEdit;
class SectionManager
{
public:
	bool InitSectionManager(unsigned int anyAddr, QPlainTextEdit* outMsg);

	//���Ե�ַת��Ϊ�����ַ
	unsigned char* LinearAddrToVirtualAddr(unsigned int LinerAddr);
	//�����ַת��Ϊ���Ե�ַ
	unsigned int VirtualAddrToLinearAddr(unsigned char* pVirtualAddr);

	//ģ������
	unsigned int SeachUserCodeEndAddr();
	//����Ϊcallָ�����ڵ�ʵ�ʵ�ַ,������ת�õ���ʵ�ʵ�ַ
	unsigned int ReadCallAddr(unsigned int addr);
public:
	std::vector<SegmentInfomation> mVec_segInfo;
private:
	QPlainTextEdit* m_outMsg = nullptr;
};
