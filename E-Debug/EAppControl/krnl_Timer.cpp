#include "krnl_Timer.h"

std::string krnl_Timer::ȡ�¼�����(int eventIndex)
{
	std::string ret;
	switch (eventIndex)
	{
	case 0:
		ret = "�����¼�";
		break;
	default:
		ret = "δ֪�¼�";
		break;
	}
	return ret;
}