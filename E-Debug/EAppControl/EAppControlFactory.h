#pragma once

enum controlType_t;
class EAppControl;
class EAppControlFactory
{
public:
	//����type���Ͷ�λ����ָ��
	static EAppControl* getEAppControl(controlType_t type);
};