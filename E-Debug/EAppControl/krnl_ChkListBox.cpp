#include "krnl_ChkListBox.h"

std::string krnl_ChkListBox::ȡ�¼�����(int eventIndex)
{
	std::string ret;
	switch (eventIndex)
	{
	case 0:
		ret = "�б��ѡ��";
		break;
	case 1:
		ret = "ѡ��״̬���ı�";
		break;
	case 2:
		ret = "˫��ѡ��";
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
	case -7:
		ret = "��ý���";
		break;
	case -8:
		ret = "ʧȥ����";
		break;
	case -9:
		ret = "����ĳ��";
		break;
	case -10:
		ret = "�ſ�ĳ��";
		break;
	case -11:
		ret = "�ַ�����";
		break;
	case -12:
		ret = "���ֱ�����";
	default:
		ret = "δ֪�¼�";
		break;
	}

	return ret;
}