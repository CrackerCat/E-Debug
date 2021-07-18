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

struct mid_EDataTypeInfo
{
	std::string m_Name;     //������������
};

struct mid_ELibInfo
{
	std::string m_Name;          //֧�ֿ�����
	std::string m_Guid;          //֧�ֿ��GUID
	int  m_nMajorVersion;  //֧�ֿ�����汾�ţ��������0��
	int  m_nMinorVersion;  //֧�ֿ�Ĵΰ汾�š�

	std::vector<mid_EDataTypeInfo> mVec_DataTypeInfo;      //����������Ϣ
};

struct LibFuncMap
{
	std::string libName;
	std::string libGuid;
	struct FuncInfo
	{
		duint addr;
		//ascii
		std::string name;
	};
	std::vector<FuncInfo> vec_Funcs;
};

class EAnalyEngine:public SectionManager
{
public:
	EAnalyEngine();
	~EAnalyEngine();
public:
	bool InitEAnalyEngine(unsigned int anyAddr);
private:
	bool Parse_EStatic(duint eHeadAddr);

	bool ParseLibInfomation(duint lpLibStartAddr, duint dwLibCount);
public:
	//0��ʧ��,1�Ǿ�̬����,2�Ƕ�̬����,3�Ƕ�������,4�Ǻ��±���
	unsigned int m_AnalysisMode = 0;
	//�Ƿ�����ɹ�
	bool m_bAnalySuccess = false;
	//�û�������ʼ��ַ
	duint m_UserCodeStartAddr = 0;
public:
	//�⺯����
	std::vector<LibFuncMap> mVec_LibFunc;
};