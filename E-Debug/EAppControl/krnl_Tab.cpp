#include "krnl_Tab.h"

std::string krnl_Tab::ȡ�¼�����(int eventIndex)
{
	std::string ret;
	switch (eventIndex)
	{
	case 0:
		ret = "������";
		break;
	case 1:
		ret = "���ı��Ӽ�";
		break;
	case 2:
		ret = "�Ӽб��ı�";
		break;
	case -1:
		ret = "������������";
		break;
	case -2:
		ret = "���������ſ�";
		break;
	case -3:
		ret = "��˫��";
		break;
	case -4:
		ret = "����Ҽ�������";
		break;
	case -5:
		ret = "����Ҽ����ſ�";
		break;
	case -6:
		ret = "���λ�ñ��ƶ�";
		break;
	case -12:
		ret = "���ֱ�����";
		break;
	default:
		ret = "δ֪�¼�";
		break;
	}
	return ret;
}