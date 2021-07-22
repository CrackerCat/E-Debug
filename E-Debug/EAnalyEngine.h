#pragma once
#include "pluginsdk/bridgemain.h"
#include "SectionManager.h"
#include <map>

struct LIB_INFO
{
	duint m_dwLibFormatVer;    //֧�ֿ��ʽ�汾��,Ӧ��Ϊ0x1312D65
	duint m_lpGuid;            //��Ӧ֧�ֿ��GUIDָ�롣
	dsint  m_nMajorVersion;     //֧�ֿ�����汾�ţ��������0��
	dsint  m_nMinorVersion;     //֧�ֿ�Ĵΰ汾�š�
	dsint  m_nBuildNumber;      //�����汾��
	dsint  m_nRqSysMajorVer;    //����Ҫ������ϵͳ�����汾��
	dsint  m_nRqSysMinorVer;    //����Ҫ������ϵͳ�Ĵΰ汾��
	dsint  m_nRqSysKrnlLibMajorVer;   //����Ҫ��ϵͳ����֧�ֿ�����汾��
	dsint  m_nRqSysKrnlLibMinorVer;   //����Ҫ��ϵͳ����֧�ֿ�Ĵΰ汾��
	duint  m_lpName;            //֧�ֿ�����ָ��
	dsint  m_nLanguage;         //֧�ֿ���֧�ֵ�����,Ӧ����1
	duint m_lpExplain;         //֧�ֿ��������ָ��,��Ϊ��
	duint m_dwState;           //֧�ֿ�����״̬˵��
	duint m_lpszAuthor;        //���������Ϣ
	duint m_lpszZipCode;       //���������Ϣ
	duint m_lpszAddress;       //���������Ϣ
	duint m_lpszPhone;         //���������Ϣ
	duint m_lpszFax;           //���������Ϣ
	duint m_lpszEmail;         //���������Ϣ
	duint m_lpszHomePage;      //���������Ϣ
	duint m_lpszOther;         //���������Ϣ

//////////////////
	dsint m_nDataTypeCount;     //֧�ֿ�ȫ�����������͸���
	duint m_lpDataType;        //ָ������õ�������������Ϣ��ָ��,LIB_DATA_TYPE_INFO

	dsint m_nCategoryCount;     //ȫ�����������Ŀ
	duint m_lpszzCategory;     //ȫ���������˵����ÿ��Ϊһ�ַ�����ǰ��λ���ֱ�ʾͼ�������ţ���1��ʼ��0��ʾ�ޣ���
								// ��һ���ֵΪָ��֧�ֿ�����Ϊ"LIB_BITMAP"��BITMAP��Դ��ĳһ����16X13λͼ������

	dsint m_nCmdCount;          //�������ṩ����������(ȫ��������󷽷�)����Ŀ(������Ϊ0)��
	duint m_lpBeginCmdInfo;    //ָ��������������Ķ�����Ϣ����(��m_nCmdCountΪ0,��ΪNULL),CMD_INFO
	duint m_lpCmdsFunc;        //ָ��ÿ�������ʵ�ִ����׵�ַ��(��m_nCmdCountΪ0, ��ΪNULL)��

	duint m_lpfnRunAddInFn;    //��ΪNULL������Ϊ������IDE�ṩ���ӹ���
	duint m_szzAddInFnInfo;    //�й�AddIn���ܵ�˵���������ַ���˵��һ������

	duint m_lpfnNotify;        //����ΪNULL���ṩ��������������IDE�����л���֪ͨ��Ϣ�ĺ�����

	// ����ģ����ʱ�������á�
	duint m_lpfnSuperTemplate;       //Ϊ��
	duint m_lpszzSuperTemplateInfo;  //Ϊ��

	// ���ⶨ������г�����
	dsint m_nLibConstCount;   //��������
	duint m_lpLibConst;      //ָ�������������ָ��

	duint m_lpszzDependFiles; //����������������Ҫ�����������ļ�����������װ���ʱ�����Զ�������Щ�ļ�,��Ϊ��
};

struct LIB_DATA_TYPE_INFO   //�ⶨ���������ͽṹ
{
	duint m_lpszName;     //����
	duint m_lpszEGName;   //Ӣ������,��Ϊ��
	duint m_szExplain;    //��ϸ����,��Ϊ��
	dsint  m_nCmdCount;    //���������ͳ�Ա��������Ŀ(��Ϊ0)
	duint m_lpnCmdsIndex; //ָ�����г�Ա����������֧�ֿ�������е�����ֵָ��,��������ݱ�Ĩ��
	duint m_dwState;      //�������͵���������

	 ////////////////////////////////////////////
	// ���³�Աֻ����Ϊ���ڵ�Ԫ���˵�ʱ����Ч��

	duint m_dwUnitBmpID;     //ָ����֧�ֿ��еĵ�Ԫͼ����ԴID
	dsint  m_nEventCount;     //����Ԫ���¼���Ŀ
	duint m_lpEventBegin;    //ָ��Ԫ�������¼���ָ��,EVENT_INFO,��������ݱ�Ĩ��
	dsint m_nPropertyCount;   //����Ԫ��������Ŀ
	duint m_lpPropertyBegin; //ָ��Ԫ���������Ե�ָ��,UNIT_PROPERTY

	duint m_lpfnGetInterface; //�����ṩ�����ڵ�Ԫ�����нӿڡ�

	////////////////////////////////////////////
	// ���³�Աֻ���ڲ�Ϊ���ڵ�Ԫ���˵�ʱ����Ч��

	dsint m_nElementCount;    //�������������ӳ�Ա����Ŀ(��Ϊ0)
	duint m_lpElementBegin;   //ָ���ӳ�Ա������׵�ַ,LIB_DATA_TYPE_ELEMENT
};


struct EDataTypeInfo
{
	std::string dataTypeName;   //������������
};

struct ElibInfo
{
	std::string libName;
	std::string libGuid;
	int nMajorVersion;  //֧�ֿ�����汾�ţ��������0��
	int nMinorVersion;  //֧�ֿ�Ĵΰ汾�š�
	struct FuncInfo
	{
		duint addr;
		//ascii
		std::string name;
	};
	std::vector<FuncInfo> vec_Funcs;
	std::vector<EDataTypeInfo> vec_DataTypeInfo;      //����������Ϣ
};

struct ImportsApi
{
	std::string LibName;
	std::string ApiName;
	int refCount;
};


enum controlType_t
{
	UnknownControl = 0,
	krnl_window,     //����
	krnl_menu,       //�˵�
	krnl_EditBox,    //�༭��
	krnl_PicBox,     //ͼƬ��
	krnl_ShapeBox,   //���ο�
	krnl_DrawPanel,  //����
	krnl_GroupBox,   //�����
	krnl_Label,      //��ǩ
	krnl_Button,     //��ť
	krnl_CheckBox,   //ѡ���
	krnl_RadioBox,   //��ѡ��
	krnl_ComboBox,   //��Ͽ�
	krnl_ListBox,    //�б��
	krnl_ChkListBox, //ѡ���б��
	krnl_HScrollBar, //���������
	krnl_VScrollBar, //���������
	krnl_ProcessBar, //������
	krnl_SliderBar,  //������
	krnl_Tab,        //ѡ���
	krnl_AnimateBox, //Ӱ���
	krnl_DatePicker, //���ڿ�
	krnl_MonthCalendar,  //����
	krnl_DriverBox,  //��������
	krnl_DirBox,     //Ŀ¼��
	krnl_FileBox,    //�ļ���
	krnl_ColorPicker, //��ɫѡ����
	krnl_HyperLinker, //����������
	krnl_Spin,        //������
	krnl_CommonDlg,   //ͨ�öԻ���
	krnl_Timer,       //ʱ��
	krnl_printer,     //��ӡ��
	krnl_UDP,         //���ݱ�
	krnl_Client,      //�ͻ�
	krnl_Server,      //������
	krnl_SerialPort,  //�˿�
	krnl_Grid,        //���
	krnl_DataSrc,     //����Դ
	krnl_NProvider,   //ͨ���ṩ��
	krnl_DBProvider,  //���ݿ��ṩ��
	krnl_PicBtn,      //ͼ�ΰ�ť
	krnl_ODBCDB,      //�ⲿ���ݿ�
	krnl_ODBCProvider,//�ⲿ�����ṩ��
	krnl_DropTarget,  //�ϷŶ���
};

struct ControlProperty
{
	std::string controlName;
	
};

struct mid_EventInfo
{
	int  nEventIndex;       //�¼�����
	duint eventAddr;        //�¼���ַ
};

struct mid_ControlInfo
{
	controlType_t controlType;            //�ؼ�����
	duint controlId;                      //�ؼ�����ID
	duint controlTypeId;                  //�ؼ�����ID
	std::string controlTypeName;          //�ؼ���������
	std::string controlName;              //�ؼ�����
	duint propertyAddr;                   //���Ե�ַ
	dsint propertySize;                   //���Դ�С
	std::vector<mid_EventInfo> vec_eventInfo;   //�¼�����
};

struct mid_GuiInfo
{
	unsigned int windowId;                        //�ؼ���������ID
	std::vector<mid_ControlInfo> vec_ControlInfo;
};

struct mid_KrnlApp
{
	duint krnl_MReportError;               //����ص�
	duint krnl_MCallDllCmd;                //DLL����
	duint krnl_MCallLibCmd;                //����֧�ֿ�����
	duint krnl_MCallKrnlLibCmd;            //����֧�ֿ�����
	duint krnl_MReadProperty;              //��ȡ�������
	duint krnl_MWriteProperty;             //�����������
	duint krnl_MMalloc;                    //�����ڴ�
	duint krnl_MRealloc;                   //���·����ڴ�
	duint krnl_MFree;                      //�ͷ��ڴ�
	duint krnl_MExitProcess;               //����
	duint krnl_MMessageLoop;               //������Ϣѭ��
	duint krnl_MLoadBeginWin;              //������������
	duint krnl_MOtherHelp;                 //��������
};

class QPlainTextEdit;
class EAnalyEngine:public SectionManager
{
public:
	EAnalyEngine();
	~EAnalyEngine();
public:
	bool InitEAnalyEngine(unsigned int anyAddr, QPlainTextEdit* outMsg);
private:
	bool Parse_EStatic(duint eHeadAddr);
	void ParseKrnlInterface(duint lpKrnlEntry);
	bool ParseLibInfomation(duint lpLibStartAddr, duint dwLibCount);
	bool ParseUserImports(duint dwApiCount, duint lpModuleName, duint lpApiName);
	bool ParseGUIResource(duint lpGUIStart, duint infoSize);
	//��ȡ�û����������ַ
	duint GetUserCodeEndAddr();

	void ParseControlBasciProperty(unsigned char* lpControlInfo, mid_ControlInfo& out_Property);
	std::string GetControlTypeName(duint typeId);
public:
	//0��ʧ��,1�Ǿ�̬����,2�Ƕ�̬����,3�Ƕ�������,4�Ǻ��±���
	unsigned int m_AnalysisMode = 0;
	//�Ƿ�����ɹ�
	bool m_bAnalySuccess = false;
	//�û�������ʼ��ַ
	duint m_UserCodeStartAddr = 0;
	//�û����������ַ
	duint m_UserCodeEndAddr = 0;

	mid_KrnlApp m_KrnlApp;
public:
	//�⺯����
	std::vector<ElibInfo> mVec_LibInfo;
	//���뺯����
	std::vector<ImportsApi> mVec_ImportsApi;
	//���ڿؼ���Ϣ
	std::vector<mid_GuiInfo> mVec_GuiInfo;
	//��־��ӡ���
	QPlainTextEdit* m_outMsg = nullptr;
};