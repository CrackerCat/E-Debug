#include "krnl_DropTarget.h"

std::string krnl_DropTarget::ȡ�¼�����(int eventIndex)
{
	std::string ret;
	switch (eventIndex)
	{
	case 0:
		ret = "�õ��ı�";
		break;
	case 1:
		ret = "�õ����ı�";
		break;
	case 2:
		ret = "�õ�URL";
		break;
	case 3:
		ret = "�õ��ļ�";
		break;
	default:
		ret = "δ֪�¼�";
		break;
	}

	return ret;
}