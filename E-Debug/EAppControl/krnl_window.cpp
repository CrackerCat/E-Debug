#include "krnl_window.h"

std::string krnl_window::ȡ�¼�����(int eventIndex)
{
	std::string ret;
	switch (eventIndex)
	{
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
		break;
	case 0:
		ret = "�������";
		break;
	case 1:
		ret = "�ɷ񱻹ر�";
		break;
	case 2:
		ret = "��������";
		break;
	case 3:
		ret = "λ�ñ��ı�";
		break;
	case 4:
		ret = "�ߴ类�ı�";
		break;
	case 5:
		ret = "������";
		break;
	case 6:
		ret = "��ȡ������";
		break;
	case 7:
		ret = "����";
		break;
	case 8:
		ret = "�״μ���";
		break;
	case 9:
		ret = "�����¼�";
		break;
	case 10:
		ret = "����ʾ";
		break;
	case 11:
		ret = "������";
		break;
	case 12:
		ret = "���ڿɷ񱻹ر�";
		break;
	default:
		ret = "δ֪�¼�";
		break;
	}
	return ret;
}