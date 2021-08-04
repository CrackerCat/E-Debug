#include "MainWindow.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QString>
#include <QMenu>
#include "pluginsdk/_scriptapi_label.h"
#include "pluginsdk/_scriptapi_gui.h"
#include "pluginsdk/_scriptapi_pattern.h"
#include "pluginsdk/_scriptapi_comment.h"
#include "pluginsdk/_scriptapi_memory.h"
#include "pluginsdk/_scriptapi_function.h"
#include "pluginsdk/_scriptapi_module.h"
#include "TrieTree.h"
#include ".\EAppControl\EAppControlFactory.h"
#include ".\EAppControl\EAppControl.h"
#include "public.h"

MainWindow::MainWindow(unsigned int dwBase, QWidget* parent) : QWidget(parent)
{
	ui.setupUi(this);
	
	ui.tabWidget->clear();
	
	//���ð汾��
	this->setWindowTitle(QStringLiteral("E-Debug 4.0"));
	//��ֹ��󻯰�ť
	//this->setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
	//���ô���ͼ��
	this->setWindowIcon(QIcon(":/ico/ico.png"));
	//���ô����Զ�����
	this->setAttribute(Qt::WA_DeleteOnClose);
	ui.table_Func->setColumnCount(2);
	ui.table_Func->setColumnWidth(0, 100);   //���õ�һ�п��
	
	ui.table_Func->horizontalHeader()->setStretchLastSection(true);
	
	ui.table_Func->setHorizontalHeaderLabels(QStringList() << QStringLiteral("��ַ") << QStringLiteral("��������"));
	ui.table_Func->setEditTriggers(QAbstractItemView::NoEditTriggers);

	ui.table_Func->setItem(0, 1, new QTableWidgetItem("func"));

	connect(ui.list_LibInfo, SIGNAL(currentTextChanged(const QString&)), SLOT(on_LibNameSelected(const QString&)));
	connect(ui.table_Func,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),SLOT(on_FuncDoubleClicked(QTableWidgetItem*)));
	connect(ui.outMsg, SIGNAL(selectionChanged()), SLOT(on_MsgSelected()));


	if (!eAnalyEngine.InitEAnalyEngine(dwBase, ui.outMsg)) {
		QMessageBox::critical(0, QStringLiteral("��Ǹ"), QStringLiteral("��ʼ��ʧ��"));
		return;
	}

	//��̬�������
	if (eAnalyEngine.m_AnalysisMode == 1) {
		InitWindow_EStatic(dwBase);
		return;
	}
}

MainWindow::~MainWindow()
{
	
}


bool isValidAddress(QString& src)
{
	std::string asciiStr = src.toStdString();
	const char* s = asciiStr.c_str();
	while (*s && *s >= '0' && *s <= 'F') s++;
	if (*s)
	{
		return false;
	}
	return true;
}

void MainWindow::on_WindowSelected(int index)
{
	ui.table_Control->setRowCount(0);
	mid_GuiInfo& eGuiInfo = eAnalyEngine.mVec_GuiInfo[index];

	QTextCodec* codec = QTextCodec::codecForName("GB2312");

	ui.table_Control->setSortingEnabled(false);
	for (unsigned int n = 0; n < eGuiInfo.vec_ControlInfo.size(); ++n) {
		mid_ControlInfo& eControlInfo = eGuiInfo.vec_ControlInfo[n];
		int insertRow = ui.table_Control->rowCount();
		ui.table_Control->insertRow(insertRow);
		//����ÿ�и߶�
		ui.table_Control->setRowHeight(insertRow, 30);

		QString strControlId;
		strControlId.sprintf("0x%08X", eControlInfo.controlId);
		QTableWidgetItem* pItemControlId = new QTableWidgetItem(strControlId);
		pItemControlId->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		ui.table_Control->setItem(insertRow, 0, pItemControlId);

		QTableWidgetItem* pItemControlType = new QTableWidgetItem(codec->toUnicode(eControlInfo.controlTypeName.c_str()));
		pItemControlType->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		ui.table_Control->setItem(insertRow, 1, pItemControlType);

		QTableWidgetItem* pItemControlName = new QTableWidgetItem(codec->toUnicode(eControlInfo.controlName.c_str()));
		pItemControlName->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		ui.table_Control->setItem(insertRow, 2, pItemControlName);
	}
	ui.table_Control->setSortingEnabled(true);
}

void MainWindow::on_MsgSelected()
{
	QString selectedTxt = ui.outMsg->textCursor().selectedText();
	if (selectedTxt.isEmpty()) {
		return;
	}

	if (isValidAddress(selectedTxt) && selectedTxt.length() == 0x8) {
		unsigned int goAddr = selectedTxt.toUInt(nullptr, 16);
		if (Script::Memory::IsValidPtr(goAddr)) {
			GuiDisasmAt(goAddr, 0);
		}
	}
}

void MainWindow::on_ApiMenu(const QPoint& point)
{
	QTableWidgetItem* currentItem = ui.table_Api->itemAt(point);
	if (!currentItem) {
		return;
	}

	QMenu* popMenu = new QMenu(ui.table_Api);
	QAction* findAction = popMenu->addAction(QStringLiteral("��������"));

	if (popMenu->exec(QCursor::pos()) == findAction) {
		ui.outMsg->clear();
		ui.outMsg->appendPlainText(QStringLiteral("->ִ������ --==��������==--"));
		duint scanStartAddr = eAnalyEngine.m_UserCodeStartAddr;
		unsigned char apiCode[] = { 0xB8,0x00,0x00,0x00,0x00,0xE8 };
		int apiIndex = ui.table_Api->item(currentItem->row(), 0)->text().toUInt();

		WriteUInt(apiCode + 1, apiIndex);
		std::string strApiCode = ʮ��ʮ��(apiCode, sizeof(apiCode));
		do
		{	
			scanStartAddr = Script::Pattern::FindMem(scanStartAddr + 1, eAnalyEngine.m_UserCodeEndAddr - scanStartAddr, strApiCode.c_str());
			if (!scanStartAddr) {
				break;
			}
			unsigned int callAddr = eAnalyEngine.ReadCallAddr(scanStartAddr + 5);
			if (ReadUShort(eAnalyEngine.LinearAddrToVirtualAddr(callAddr)) != 0x25FF) {
				continue;
			}
			callAddr = ReadUInt(eAnalyEngine.LinearAddrToVirtualAddr(callAddr + 2));
			callAddr = ReadUInt(eAnalyEngine.LinearAddrToVirtualAddr(callAddr));
			if (callAddr == eAnalyEngine.m_KrnlApp.krnl_MCallDllCmd) {
				QString outMsg;
				outMsg.sprintf("%08X    mov eax,%08X     %s", scanStartAddr, apiIndex, StringUtils::LocalCpToUtf8(eAnalyEngine.mVec_ImportsApi[apiIndex].ApiName.c_str()).c_str());
				ui.outMsg->appendPlainText(outMsg);
			}
		} while (true);

	}
}

void MainWindow::on_FuncDoubleClicked(QTableWidgetItem* pItem)
{
	if (!pItem) {
		return;
	}

	QString funcAddr = ui.table_Func->item(pItem->row(), 0)->text();
	duint addr = funcAddr.toUInt(nullptr, 16);
	
	GuiDisasmAt(addr,0);	
}

void MainWindow::on_LibNameSelected(const QString& currentText)
{
	ui.table_Func->setRowCount(0);
	
	for (unsigned int nLibIndex = 0; nLibIndex < eAnalyEngine.mVec_LibInfo.size(); ++nLibIndex) {
		ElibInfo& eLibInfo = eAnalyEngine.mVec_LibInfo[nLibIndex];

		std::string uLibName = StringUtils::LocalCpToUtf8(eLibInfo.libName.c_str());
		int index = currentText.indexOf(uLibName.c_str());
		if (index == -1) {
			continue;
		}

		//�ر�������
		ui.table_Func->setSortingEnabled(false);
		QTextCodec* codec = QTextCodec::codecForName("GB2312");
		for (unsigned int nFuncIndex = 0; nFuncIndex < eLibInfo.vec_Funcs.size(); ++nFuncIndex) {
			int insertRow = ui.table_Func->rowCount();
			ui.table_Func->insertRow(insertRow);
			//����ÿ�и߶�
			ui.table_Func->setRowHeight(insertRow, 20);

			QString strAddr;
			strAddr.sprintf("%08X", eLibInfo.vec_Funcs[nFuncIndex].addr);
			QTableWidgetItem* pAddrItem = new QTableWidgetItem(strAddr);
			pAddrItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			ui.table_Func->setItem(insertRow, 0, pAddrItem);

			QString strName = codec->toUnicode(eLibInfo.vec_Funcs[nFuncIndex].name.c_str());
			QTableWidgetItem* pNameItem = new QTableWidgetItem(strName);
			pNameItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			ui.table_Func->setItem(insertRow, 1, pNameItem);
		}
		//����������
		ui.table_Func->setSortingEnabled(true);	
	}
}

bool MainWindow::InitWindow_EStatic(duint codeAddr)
{
	//����������
	DbgCmdExecDirect("cfanalyze");

	//ui.outMsg->appendPlainText(QStringLiteral("->��ʼʶ��������֧�ֿ⺯��..."));

	ui.tabWidget->addTab(ui.tab_Func, QStringLiteral("����ʶ��"));
	QTextCodec* codec = QTextCodec::codecForName("GB2312");
	for (unsigned int nLibIndex = 0; nLibIndex < eAnalyEngine.mVec_LibInfo.size(); ++nLibIndex) {

		ElibInfo& eLibInfo = eAnalyEngine.mVec_LibInfo[nLibIndex];
		std::string libPath = GetApplicationDirA() + "\\plugins\\esig\\" + eLibInfo.libName + ".esig";

		//����ʶ��
		TrieTree esigTree(&eAnalyEngine);
		if (esigTree.LoadSig(libPath.c_str())) {
			for (unsigned int nFuncIndex = 0; nFuncIndex < eLibInfo.vec_Funcs.size(); ++nFuncIndex) {
				double score = 0;
				const char* pFuncName = esigTree.MatchFunc(eAnalyEngine.LinearAddrToVirtualAddr(eLibInfo.vec_Funcs[nFuncIndex].addr));
				if (pFuncName) {
					eLibInfo.vec_Funcs[nFuncIndex].name = pFuncName;
					std::string u16FuncName = StringUtils::LocalCpToUtf8(pFuncName);
					SetX64DbgLabel(eLibInfo.vec_Funcs[nFuncIndex].addr, u16FuncName.c_str());
				}
				else if ((pFuncName = esigTree.MatchFunc_Fuzzy(eAnalyEngine.LinearAddrToVirtualAddr(eLibInfo.vec_Funcs[nFuncIndex].addr), score))) {
					eLibInfo.vec_Funcs[nFuncIndex].name = std::string(pFuncName) + "_ģ��";
					std::string u16FuncName = StringUtils::LocalCpToUtf8(eLibInfo.vec_Funcs[nFuncIndex].name.c_str());
					SetX64DbgLabel(eLibInfo.vec_Funcs[nFuncIndex].addr, u16FuncName.c_str());
#ifdef _DEBUG
					QString logMsg; logMsg.sprintf("%s	%08X:%lf", StringUtils::LocalCpToUtf8("ģ��ƥ��ɹ�").c_str(), eLibInfo.vec_Funcs[nFuncIndex].addr,score);
					ui.outMsg->appendPlainText(logMsg);
#endif	
				}
				else {
#ifdef _DEBUG
					QString logMsg; logMsg.sprintf("%s	%08X", StringUtils::LocalCpToUtf8("ʶ����ʧ��").c_str(), eLibInfo.vec_Funcs[nFuncIndex].addr);
					ui.outMsg->appendPlainText(logMsg);
#endif
				}
			}
		}
		else {
			QString logMsg = QStringLiteral("->���������ļ�ʧ��:") + codec->toUnicode(libPath.c_str());
			ui.outMsg->appendPlainText(logMsg);
		}

		
		//���½���
		QString LibNameLine = codec->toUnicode(eLibInfo.libName.c_str());
		LibNameLine.append(QString::number(eLibInfo.nMajorVersion) + QStringLiteral(".") + QString::number(eLibInfo.nMinorVersion));
		LibNameLine.append(QStringLiteral("(��������:"));
		LibNameLine.append(QString::number(eLibInfo.vec_Funcs.size()));
		LibNameLine.append(QStringLiteral(")"));
		ui.list_LibInfo->insertItem(ui.list_LibInfo->count(), new QListWidgetItem(LibNameLine));

		QString LibGuidLine = "   " + QString::fromLocal8Bit(eLibInfo.libGuid.c_str());
		QListWidgetItem* pGuidItem = new QListWidgetItem(LibGuidLine);
		pGuidItem->setTextColor(QColor(150, 150, 150));
		ui.list_LibInfo->insertItem(ui.list_LibInfo->count(), pGuidItem);
		ui.list_LibInfo->insertItem(ui.list_LibInfo->count(), new QListWidgetItem(QStringLiteral("������������������������������������������������������������")));
	}

	//��������ʶ��
	std::string basicLibPath = GetApplicationDirA() + "\\plugins\\esig\\�����Ի�������.esig";
	TrieTree basicEsigTree(&eAnalyEngine);
	if (basicEsigTree.LoadSig(basicLibPath.c_str())) {
		BridgeList<Script::Function::FunctionInfo> vec_Funcs;
		Script::Function::GetList(&vec_Funcs);
		duint codeBaseAddr = Script::Module::BaseFromAddr(codeAddr);
		for (unsigned int n = 0; n < vec_Funcs.Count(); ++n) {
			duint funcAddr = codeBaseAddr + vec_Funcs[n].rvaStart;
			char* pFuncName = basicEsigTree.MatchFunc(eAnalyEngine.LinearAddrToVirtualAddr(funcAddr));
			if (pFuncName) {
				SetX64DbgLabel(funcAddr, StringUtils::LocalCpToUtf8(pFuncName).c_str());
			}
		}
	}
	else {
		QString logMsg = QStringLiteral("->���������Ի������������ļ�ʧ��");
		ui.outMsg->appendPlainText(logMsg);
	}


	//��ʼ����DLL�����
	if (eAnalyEngine.mVec_ImportsApi.size()) {

		ui.tabWidget->addTab(ui.tab_Api, QStringLiteral("Api����"));
		//��������
		ui.table_Api->setColumnCount(4);
		ui.table_Api->setColumnWidth(0, 60);   //���õ�1�п��
		ui.table_Api->setColumnWidth(1, 120);   //���õ�2�п��
		ui.table_Api->setColumnWidth(2, 340);   //���õ�3�п��
		ui.table_Api->setHorizontalHeaderLabels(QStringList() << QStringLiteral("���") << QStringLiteral("DLL��") << QStringLiteral("��������") << QStringLiteral("���ô���"));

		connect(ui.table_Api, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(on_ApiMenu(const QPoint&)));
		duint scanStartAddr = eAnalyEngine.m_UserCodeStartAddr;
		do
		{
			scanStartAddr = Script::Pattern::FindMem(scanStartAddr + 1, eAnalyEngine.m_UserCodeEndAddr - scanStartAddr, "B8????0000E8");
			if (!scanStartAddr) {
				break;
			}
			unsigned int callAddr = eAnalyEngine.ReadCallAddr(scanStartAddr + 5);
			if (ReadUShort(eAnalyEngine.LinearAddrToVirtualAddr(callAddr)) != 0x25FF) {
				continue;
			}
			callAddr = ReadUInt(eAnalyEngine.LinearAddrToVirtualAddr(callAddr + 2));
			callAddr = ReadUInt(eAnalyEngine.LinearAddrToVirtualAddr(callAddr));
			if (callAddr == eAnalyEngine.m_KrnlApp.krnl_MCallDllCmd) {
				int index = ReadUInt(eAnalyEngine.LinearAddrToVirtualAddr(scanStartAddr + 1));
				Script::Comment::Set(scanStartAddr, StringUtils::LocalCpToUtf8(eAnalyEngine.mVec_ImportsApi[index].ApiName.c_str()).c_str());
				eAnalyEngine.mVec_ImportsApi[index].refCount++;
			}
		} while (true);

		for (unsigned int n = 0; n < eAnalyEngine.mVec_ImportsApi.size(); ++n) {
			int insertRow = ui.table_Api->rowCount();
			ui.table_Api->insertRow(insertRow);
			//����ÿ�и߶�
			ui.table_Api->setRowHeight(insertRow, 20);

			QTableWidgetItem* pItemIndex = new QTableWidgetItem(QString::number(n));
			pItemIndex->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			ui.table_Api->setItem(insertRow, 0, pItemIndex);

			QTableWidgetItem* pItemLib = nullptr;
			if (eAnalyEngine.mVec_ImportsApi[n].LibName.empty()) {
				pItemLib = new QTableWidgetItem(QStringLiteral("NULL"));
			}
			else {
				pItemLib = new QTableWidgetItem(codec->toUnicode(eAnalyEngine.mVec_ImportsApi[n].LibName.c_str()));
			}
			pItemLib->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			ui.table_Api->setItem(insertRow, 1, pItemLib);

			QTableWidgetItem* pItemFunc = new QTableWidgetItem(codec->toUnicode(eAnalyEngine.mVec_ImportsApi[n].ApiName.c_str()));
			pItemFunc->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			ui.table_Api->setItem(insertRow, 2, pItemFunc);

			QTableWidgetItem* pItemCount = new QTableWidgetItem(QString::number(eAnalyEngine.mVec_ImportsApi[n].refCount));
			pItemCount->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			ui.table_Api->setItem(insertRow, 3, pItemCount);
		}
	}

	//��ʼ���ɴ��ڿؼ���Ϣ
	if (eAnalyEngine.mVec_GuiInfo.size()) {
		ui.tabWidget->addTab(ui.tab_Control, QStringLiteral("���ڿؼ�"));
		ui.table_Control->setColumnCount(3);
		ui.table_Control->setColumnWidth(0, 110);   //���õ�1�п��
		ui.table_Control->setColumnWidth(1, 120);   //���õ�2�п��
		ui.table_Control->horizontalHeader()->setStretchLastSection(true);
		ui.table_Control->setHorizontalHeaderLabels(QStringList() << QStringLiteral("�ؼ�ID") << QStringLiteral("�ؼ�����") << QStringLiteral("�ؼ�����"));
		connect(ui.combo_Window, SIGNAL(currentIndexChanged(int)), SLOT(on_WindowSelected(int)));

		for (unsigned int nIndexWindow = 0; nIndexWindow < eAnalyEngine.mVec_GuiInfo.size(); ++nIndexWindow) {
			mid_GuiInfo& eGuiInfo = eAnalyEngine.mVec_GuiInfo[nIndexWindow];
			QString windowName;
			windowName.sprintf("%s_0x%08X(%d)", StringUtils::LocalCpToUtf8("����").c_str(), eGuiInfo.windowId, eGuiInfo.vec_ControlInfo.size());
			ui.combo_Window->addItem(windowName);
			for (unsigned int nIndexControl = 0; nIndexControl < eGuiInfo.vec_ControlInfo.size(); ++nIndexControl) {
				mid_ControlInfo& eControlInfo = eGuiInfo.vec_ControlInfo[nIndexControl];
				EAppControl* pControlClass = EAppControlFactory::GetEAppControl(eControlInfo.controlType);
				if (pControlClass) {
					for (unsigned int nIndexEvent = 0; nIndexEvent < eControlInfo.vec_eventInfo.size(); ++nIndexEvent) {
						std::string strEventName;
						strEventName = "_" + eControlInfo.controlName + "_" + pControlClass->ȡ�¼�����(eControlInfo.vec_eventInfo[nIndexEvent].nEventIndex);
						SetX64DbgLabel(eControlInfo.vec_eventInfo[nIndexEvent].eventAddr, StringUtils::LocalCpToUtf8(strEventName.c_str()).c_str());
					}
				}
			}
		}
		on_WindowSelected(0);
	}
	
	
	return true;
}