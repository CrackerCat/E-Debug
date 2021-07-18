#include "SectionManager.h"
#include "pluginsdk/bridgemain.h"
#include "pluginsdk/_scriptapi_memory.h"
#include "pluginsdk/_scriptapi_gui.h"
#include "pluginsdk/_scriptapi_module.h"

unsigned int AlignByMemory(unsigned int originValue)
{
	unsigned int Alignment = 0x1000;
	DWORD reminder = originValue / Alignment;
	DWORD mod = originValue % Alignment;

	if (mod != 0) {
		reminder += 1;
	}
	return reminder * Alignment;
}

bool SectionManager::InitSectionManager(unsigned int anyAddr)
{
	BridgeList<Script::Module::ModuleSectionInfo> moduleList;
	if (!Script::Module::SectionListFromAddr(anyAddr, &moduleList)) {
		return false;;
	}

	for (unsigned int n = 0; n < moduleList.Count(); ++n) {

		SegmentInfomation tmpInfo;
		tmpInfo.m_segStart = moduleList[n].addr;
		tmpInfo.m_segSize = AlignByMemory(moduleList[n].size);
		tmpInfo.m_segName = moduleList[n].name;

		tmpInfo.m_segData.resize(tmpInfo.m_segSize);
		Script::Memory::Read(tmpInfo.m_segStart, &tmpInfo.m_segData[0], tmpInfo.m_segSize, 0);

		mVec_segInfo.push_back(tmpInfo);
	}
	return true;
}

unsigned char* SectionManager::LinearAddrToVirtualAddr(unsigned int LinerAddr)
{
	//存储上一次命中的索引,用于加速访问
	static unsigned int saveIndex = 0;

	unsigned int index = saveIndex;

	for (unsigned int n = 0; n < mVec_segInfo.size(); ++n) {
		unsigned int endAddr = mVec_segInfo[index].m_segStart + mVec_segInfo[index].m_segSize;
		if (LinerAddr >= mVec_segInfo[index].m_segStart && LinerAddr < endAddr) {
			unsigned int offset = LinerAddr - mVec_segInfo[index].m_segStart;
			saveIndex = index;
			return &mVec_segInfo[index].m_segData[offset];
		}
		++index;
		if (index == mVec_segInfo.size()) {
			index = 0;
		}
	}
	return 0;
}


unsigned int SectionManager::VirtualAddrToLinearAddr(unsigned char* pVirtualAddr)
{
	for (unsigned int n = 0; n < mVec_segInfo.size(); ++n) {
		unsigned char* pEndAddr = &mVec_segInfo[n].m_segData[0] + mVec_segInfo[n].m_segSize;
		if (pVirtualAddr >= &mVec_segInfo[n].m_segData[0] && pVirtualAddr < pEndAddr) {
			unsigned int offset = pVirtualAddr - &mVec_segInfo[n].m_segData[0];
			return mVec_segInfo[n].m_segStart + offset;
		}
	}
	return -1;
}