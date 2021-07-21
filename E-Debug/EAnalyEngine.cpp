#include "EAnalyEngine.h"
#include <QPlainTextEdit>
#include "pluginsdk/_scriptapi_memory.h"
#include "pluginsdk/_scriptapi_pattern.h"
#include "pluginsdk/_scriptapi_comment.h"
#include "pluginsdk/_scriptapi_label.h"
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

void EAnalyEngine::ParseKrnlInterface(duint lpKrnlEntry)
{
	lpKrnlEntry -= sizeof(mid_KrnlApp);
	Script::Memory::Read(lpKrnlEntry, &m_KrnlApp, sizeof(mid_KrnlApp), 0);

	Script::Label::Set(m_KrnlApp.krnl_MReportError, LocalCpToUtf8("����ص�").c_str());
	Script::Label::Set(m_KrnlApp.krnl_MCallDllCmd, LocalCpToUtf8("DLL����").c_str());
	Script::Label::Set(m_KrnlApp.krnl_MCallLibCmd, LocalCpToUtf8("����֧�ֿ�����").c_str());
	Script::Label::Set(m_KrnlApp.krnl_MCallKrnlLibCmd, LocalCpToUtf8("����֧�ֿ�����").c_str());
	Script::Label::Set(m_KrnlApp.krnl_MReadProperty, LocalCpToUtf8("��ȡ�������").c_str());
	Script::Label::Set(m_KrnlApp.krnl_MWriteProperty, LocalCpToUtf8("�����������").c_str());
	Script::Label::Set(m_KrnlApp.krnl_MMalloc, LocalCpToUtf8("�����ڴ�").c_str());
	Script::Label::Set(m_KrnlApp.krnl_MRealloc, LocalCpToUtf8("���·����ڴ�").c_str());
	Script::Label::Set(m_KrnlApp.krnl_MFree, LocalCpToUtf8("�ͷ��ڴ�").c_str());
	Script::Label::Set(m_KrnlApp.krnl_MExitProcess, LocalCpToUtf8("����").c_str());
	Script::Label::Set(m_KrnlApp.krnl_MMessageLoop, LocalCpToUtf8("������Ϣѭ��").c_str());
	Script::Label::Set(m_KrnlApp.krnl_MLoadBeginWin, LocalCpToUtf8("������������").c_str());
	Script::Label::Set(m_KrnlApp.krnl_MOtherHelp, LocalCpToUtf8("��������").c_str());
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

bool EAnalyEngine::ParseUserImports(duint dwApiCount, duint lpModuleName, duint lpApiName)
{
	unsigned char* pszLibnameAddr = SectionManager::LinearAddrToVirtualAddr(lpModuleName);
	unsigned char* pszApinameAddr = SectionManager::LinearAddrToVirtualAddr(lpApiName);

	for (unsigned int n = 0; n < dwApiCount; ++n) {
		char* pszLibname = (char*)SectionManager::LinearAddrToVirtualAddr(ReadUInt(pszLibnameAddr));
		char* pszApiname = (char*)SectionManager::LinearAddrToVirtualAddr(ReadUInt(pszApinameAddr));

		ImportsApi eImportsApi = {};
		eImportsApi.LibName = pszLibname;
		eImportsApi.ApiName = pszApiname;

		mVec_ImportsApi.push_back(eImportsApi);
		pszLibnameAddr += 4;
		pszApinameAddr += 4;
	}
	return true;
}

duint EAnalyEngine::GetUserCodeEndAddr()
{
	//���ȳ���ͨ������������λ�û����������ַ
	duint codeEndAddr = SeachUserCodeEndAddr();
	if (codeEndAddr) {
		codeEndAddr = codeEndAddr + 1;
		codeEndAddr = codeEndAddr + ReadInt(LinearAddrToVirtualAddr(codeEndAddr + 1)) + 5;

		QString outMsg;
		outMsg.sprintf("->%s: %08X",LocalCpToUtf8("�����Գ������").c_str(), codeEndAddr);
		m_outMsg->appendPlainText(outMsg);
		Script::Comment::Set(codeEndAddr,LocalCpToUtf8("�����Գ������").c_str());
		return codeEndAddr;
	}

	//ϵͳ֧�ֿ⺯���е���С��ַ������Ϊ������ַ
	if (mVec_LibFunc[0].vec_Funcs.size()) {
		unsigned int nMinAddress = 0xFFFFFFFF;
		for (unsigned int n = 0; n < mVec_LibFunc[0].vec_Funcs.size(); ++n) {
			if (mVec_LibFunc[0].vec_Funcs[n].addr < nMinAddress) {
				nMinAddress = mVec_LibFunc[0].vec_Funcs[n].addr;
			}
		}
		return nMinAddress;
	}

	return mVec_segInfo[0].m_segStart + mVec_segInfo[0].m_segSize - 1;
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
	ParseKrnlInterface(dwKrnlEntry);
	m_UserCodeStartAddr = eHead.lpStartCode;
	m_UserCodeEndAddr = GetUserCodeEndAddr();

	if (eHead.dwApiCount) {
		ParseUserImports(eHead.dwApiCount, eHead.lpModuleName, eHead.lpApiName);
	}



	return true;
}