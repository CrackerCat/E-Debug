#include "krnl_Tab.h"

std::string krnl_Tab::取事件名称(int eventIndex)
{
	std::string ret;
	switch (eventIndex)
	{
	case 0:
		ret = "被单击";
		break;
	case 1:
		ret = "将改变子夹";
		break;
	case 2:
		ret = "子夹被改变";
		break;
	case -1:
		ret = "鼠标左键被按下";
		break;
	case -2:
		ret = "鼠标左键被放开";
		break;
	case -3:
		ret = "被双击";
		break;
	case -4:
		ret = "鼠标右键被按下";
		break;
	case -5:
		ret = "鼠标右键被放开";
		break;
	case -6:
		ret = "鼠标位置被移动";
		break;
	case -12:
		ret = "滚轮被滚动";
		break;
	default:
		ret = "未知事件";
		break;
	}
	return ret;
}