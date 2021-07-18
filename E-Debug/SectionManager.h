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

class SectionManager
{
public:
	bool InitSectionManager(unsigned int anyAddr);

	//���Ե�ַת��Ϊ�����ַ
	unsigned char* LinearAddrToVirtualAddr(unsigned int LinerAddr);
	//�����ַת��Ϊ���Ե�ַ
	unsigned int VirtualAddrToLinearAddr(unsigned char* pVirtualAddr);
private:
	std::vector<SegmentInfomation> mVec_segInfo;
};
