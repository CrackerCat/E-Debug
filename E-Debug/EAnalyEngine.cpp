#include "EAnalyEngine.h"
#include <QPlainTextEdit>
#include "pluginsdk/_scriptapi_memory.h"
#include "pluginsdk/_scriptapi_pattern.h"
#include "SymbolTable.h"
#include "public.h"

struct EStaticHead
{
	duint dwMagic;  //δ֪,ֵ�̶�Ϊ3
	duint szNone2;  //δ֪,ֵ�̶�Ϊ0
	duint szNone3;  //δ֪,�����Ǹ������,�޸Ĳ�Ӱ�����
	duint lpStartCode;   //��ʼ�û������ַ,�����޸�
	duint lpEString;     //�ַ�����Դ,���û���ַ�����Դ,��Ϊ0
	duint dwEStringSize; //�ַ�����Դ��С,���û���ַ�����Դ,��Ϊ0
	duint lpEWindow;     //���������Ϣ
	duint dwEWindowSize; //���������Ϣ��С
	duint dwLibNum;      //֧�ֿ�����
	duint lpLibEntry;    //֧�ֿ���Ϣ���
	duint dwApiCount;    //Api����
	duint lpModuleName;  //ָ��ģ������
	duint lpApiName;     //ָ��Api����
};

EAnalyEngine::EAnalyEngine()
{

}

EAnalyEngine::~EAnalyEngine()
{

}

bool EAnalyEngine::InitEAnalyEngine(unsigned int anyAddr, QPlainTextEdit* outMsg)
{
	m_outMsg = outMsg;
	bool bRet = false;
	if (!InitSectionManager(anyAddr,outMsg)) {
		return false;
	}
	
	duint codeBase = Script::Memory::GetBase(anyAddr);
	duint codeSize = Script::Memory::GetSize(anyAddr);

	duint eMagicHead = Script::Pattern::FindMem(codeBase, codeSize, "50 64 89 25 00 00 00 00 81 EC AC 01 00 00 53 56 57");
	if (eMagicHead != 0) {
		m_AnalysisMode = 1;
		outMsg->appendPlainText(QStringLiteral("->��⵽�����Ծ�̬�������"));
		duint dwHeadAddr = Script::Memory::ReadDword(eMagicHead + 0x26);
		m_bAnalySuccess = Parse_EStatic(dwHeadAddr);
		bRet = true;
	}

	//To do...
	//֧�ָ���ģʽ

	SymbolTable::InitSymbolTable();
	return bRet;
}

bool EAnalyEngine::ParseLibInfomation(duint lpLibStartAddr, duint dwLibCount)
{
	for (unsigned int nLibIndex = 0; nLibIndex < dwLibCount; ++nLibIndex) {

		LIB_INFO tmpLibInfo;
		Script::Memory::Read(Script::Memory::ReadDword(lpLibStartAddr), &tmpLibInfo, sizeof(LIB_INFO), 0);
		lpLibStartAddr = lpLibStartAddr + 4;

		//�ж��Ƿ����֧�ֿ��ʽ
		if (tmpLibInfo.m_dwLibFormatVer != 0x1312D65) {
			continue;
		}

		mid_ELibInfo eLibInfo;
		eLibInfo.m_Name = get_shortstring(tmpLibInfo.m_lpName);
		eLibInfo.m_Guid = get_shortstring(tmpLibInfo.m_lpGuid);
		eLibInfo.m_nMajorVersion = tmpLibInfo.m_nMajorVersion;
		eLibInfo.m_nMinorVersion = tmpLibInfo.m_nMinorVersion;

		//������ȫ���Ŀ⺯������
		LibFuncMap eLibFuncMap;
		eLibFuncMap.libName = get_shortstring(tmpLibInfo.m_lpName);
		eLibFuncMap.libGuid = get_shortstring(tmpLibInfo.m_lpGuid);
		if (tmpLibInfo.m_nCmdCount && tmpLibInfo.m_lpCmdsFunc) {
			duint* pFuncBuf = (duint*)BridgeAlloc(tmpLibInfo.m_nCmdCount * 4);
			Script::Memory::Read(tmpLibInfo.m_lpCmdsFunc, pFuncBuf, tmpLibInfo.m_nCmdCount * 4, 0);
			for (unsigned int nFuncIndex = 0; nFuncIndex < tmpLibInfo.m_nCmdCount; ++nFuncIndex) {
				LibFuncMap::FuncInfo tmpFunc;
				tmpFunc.addr = pFuncBuf[nFuncIndex];
				eLibFuncMap.vec_Funcs.push_back(tmpFunc);
			}
			BridgeFree(pFuncBuf);
		}

		mVec_LibFunc.push_back(eLibFuncMap);
	}

	return true;
}

bool EAnalyEngine::Parse_EStatic(duint eHeadAddr)
{
	EStaticHead eHead;

	Script::Memory::Read(eHeadAddr, &eHead, sizeof(EStaticHead), 0);
	if (eHead.dwMagic != 0x3) {
		return false;
	}

	if (!ParseLibInfomation(eHead.lpLibEntry, eHead.dwLibNum)) {
		return false;
	}

	duint dwKrnlEntry = eHead.lpEString;
	if (dwKrnlEntry == 0) {
		dwKrnlEntry = eHead.lpEWindow;
	}
	m_UserCodeStartAddr= eHead.lpStartCode;


	return true;
}