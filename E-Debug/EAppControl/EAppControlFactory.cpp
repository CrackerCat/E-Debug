#include "EAppControlFactory.h"
#include "krnl_window.h"
#include "krnl_EditBox.h"
#include "krnl_Label.h"
#include "krnl_Button.h"
#include "krnl_Timer.h"
#include "krnl_PicBox.h"
#include "krnl_ShapeBox.h"
#include "krnl_DrawPanel.h"
#include "krnl_GroupBox.h"
#include "krnl_CheckBox.h"
#include "krnl_RadioBox.h"
#include "krnl_ComboBox.h"
#include "krnl_ListBox.h"
#include "krnl_ChkListBox.h"
#include "krnl_HScrollBar.h"
#include "krnl_VScrollBar.h"
#include "krnl_ProcessBar.h"
#include "krnl_SliderBar.h"
#include "krnl_Tab.h"
#include "krnl_AnimateBox.h"

class krnl_window gkrnl_window;
class krnl_EditBox gkrnl_EditBox;
class krnl_PicBox gkrnl_PicBox;
class krnl_Label gkrnl_Label;
class krnl_Button gkrnl_Button;
class krnl_Timer gkrnl_Timer;
class krnl_ShapeBox gkrnl_ShapeBox;
class krnl_DrawPanel gkrnl_DrawPanel;
class krnl_GroupBox gkrnl_GroupBox;
class krnl_CheckBox gkrnl_CheckBox;
class krnl_RadioBox gkrnl_RadioBox;
class krnl_ComboBox gkrnl_ComboBox;
class krnl_ListBox gkrnl_ListBox;
class krnl_ChkListBox gkrnl_ChkListBox;
class krnl_HScrollBar gkrnl_HScrollBar;
class krnl_VScrollBar gkrnl_VScrollBar;
class krnl_ProcessBar gkrnl_ProcessBar;
class krnl_SliderBar gkrnl_SliderBar;
class krnl_Tab gkrnl_Tab;
class krnl_AnimateBox gkrnl_AnimateBox;

std::map<controlType_t, EAppControl*> g_EControlClassMap =
{
	{krnl_window, &gkrnl_window},
	{krnl_EditBox,&gkrnl_EditBox},
	{krnl_PicBox,&gkrnl_PicBox},
	{krnl_Label,&gkrnl_Label},
	{krnl_Button,&gkrnl_Button},
	{krnl_Timer,&gkrnl_Timer},
	{krnl_ShapeBox,&gkrnl_ShapeBox},
	{krnl_DrawPanel,&gkrnl_DrawPanel},
	{krnl_GroupBox,&gkrnl_GroupBox},
	{krnl_CheckBox,&gkrnl_CheckBox},
	{krnl_RadioBox,&gkrnl_RadioBox},
	{krnl_ComboBox,&gkrnl_ComboBox},
	{krnl_ListBox,&gkrnl_ListBox},
	{krnl_ChkListBox,&gkrnl_ChkListBox},
	{krnl_HScrollBar,&gkrnl_HScrollBar},
	{krnl_VScrollBar,&gkrnl_VScrollBar},
	{krnl_ProcessBar,&gkrnl_ProcessBar},
	{krnl_SliderBar,&gkrnl_SliderBar},
	{krnl_Tab,&gkrnl_Tab},
	{krnl_AnimateBox,&gkrnl_AnimateBox}
};

std::map<std::string, controlType_t> g_ControlTypeMap
{
	{"����",krnl_window},
	{"�˵�",krnl_menu},
	{"�༭��",krnl_EditBox},
	{"ͼƬ��",  krnl_PicBox},
	{"���ο�", krnl_ShapeBox},
	{"����"  ,krnl_DrawPanel},
	{"�����", krnl_GroupBox},
	{"��ǩ", krnl_Label},
	{"��ť", krnl_Button},
	{"ѡ���", krnl_CheckBox},
	{"��ѡ��", krnl_RadioBox},
	{"��Ͽ�", krnl_ComboBox},
	{"�б��", krnl_ListBox},
	{"ѡ���б��", krnl_ChkListBox},
	{"���������", krnl_HScrollBar},
	{"���������", krnl_VScrollBar},
	{"������", krnl_ProcessBar},
	{"������", krnl_SliderBar},
	{"ѡ���", krnl_Tab},
	{"Ӱ���", krnl_AnimateBox},
	{"���ڿ�", krnl_DatePicker},
	{"����", krnl_MonthCalendar},
	{"��������",krnl_DriverBox},
	{"Ŀ¼��", krnl_DirBox},
	{"�ļ���", krnl_FileBox},
	{"��ɫѡ����", krnl_ColorPicker},
	{"����������", krnl_HyperLinker},
	{"������",krnl_Spin},
	{"ͨ�öԻ���", krnl_CommonDlg},
	{"ʱ��", krnl_Timer},
	{"��ӡ��", krnl_printer},
	{"���ݱ�", krnl_UDP},
	{"�ͻ�", krnl_Client},
	{"������", krnl_Server},
	{"�˿�",krnl_SerialPort},
	{"���", krnl_Grid},
	{"����Դ", krnl_DataSrc},
	{"ͨ���ṩ��", krnl_NProvider},
	{"���ݿ��ṩ��", krnl_DBProvider},
	{"ͼ�ΰ�ť", krnl_PicBtn},
	{"�ⲿ���ݿ�", krnl_ODBCDB},
	{"�ⲿ�����ṩ��", krnl_ODBCProvider},
	{"�ϷŶ���", krnl_DropTarget}
};

EAppControl* EAppControlFactory::GetEAppControl(controlType_t type)
{
	return g_EControlClassMap[type];
}

controlType_t EAppControlFactory::GetControlType(std::string controlTypeName)
{
	std::map<std::string, controlType_t>::iterator it = g_ControlTypeMap.find(controlTypeName);
	if (it == g_ControlTypeMap.end()) {
		return UnknownControl;
	}
	return it->second;
}