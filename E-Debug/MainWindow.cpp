#include "MainWindow.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QString>
#include "pluginsdk/_scriptapi_label.h"
#include "pluginsdk/_scriptapi_gui.h"
#include "pluginsdk/_scriptapi_pattern.h"
#include "pluginsdk/_scriptapi_comment.h"
#include "TrieTree.h"
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

	if (!eAnalyEngine.InitEAnalyEngine(dwBase, ui.outMsg)) {
		QMessageBox::critical(0, QStringLiteral("��Ǹ"), QStringLiteral("��ʼ��ʧ��"));
		return;
	}
	
	
	//��̬�������
	if (eAnalyEngine.m_AnalysisMode == 1) {
		InitWindow_EStatic();
		return;
	}
}

MainWindow::~MainWindow()
{
	
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
	
	for (unsigned int nLibIndex = 0; nLibIndex < eAnalyEngine.mVec_LibFunc.size(); ++nLibIndex) {
		LibFuncMap& eLibMap = eAnalyEngine.mVec_LibFunc[nLibIndex];

		std::string uLibName = LocalCpToUtf8(eLibMap.libName.c_str());
		int index = currentText.indexOf(uLibName.c_str());
		if (index == -1) {
			continue;
		}

		//�ر�������
		ui.table_Func->setSortingEnabled(false);
		QTextCodec* codec = QTextCodec::codecForName("GB2312");
		for (unsigned int nFuncIndex = 0; nFuncIndex < eLibMap.vec_Funcs.size(); ++nFuncIndex) {
			int insertRow = ui.table_Func->rowCount();
			ui.table_Func->insertRow(insertRow);
			//����ÿ�и߶�
			ui.table_Func->setRowHeight(insertRow, 20);

			QString strAddr;
			strAddr.sprintf("%08X", eLibMap.vec_Funcs[nFuncIndex].addr);
			QTableWidgetItem* pAddrItem = new QTableWidgetItem(strAddr);
			pAddrItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			ui.table_Func->setItem(insertRow, 0, pAddrItem);

			QString strName = codec->toUnicode(eLibMap.vec_Funcs[nFuncIndex].name.c_str());
			QTableWidgetItem* pNameItem = new QTableWidgetItem(strName);
			pNameItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			ui.table_Func->setItem(insertRow, 1, pNameItem);
		}
		//����������
		ui.table_Func->setSortingEnabled(true);	
	}
}

bool MainWindow::InitWindow_EStatic()
{
	//ui.outMsg->appendPlainText(QStringLiteral("->��ʼʶ��������֧�ֿ⺯��..."));

	ui.tabWidget->addTab(ui.tab_Func, QStringLiteral("����ʶ��"));
	QTextCodec* codec = QTextCodec::codecForName("GB2312");
	for (unsigned int nLibIndex = 0; nLibIndex < eAnalyEngine.mVec_LibFunc.size(); ++nLibIndex) {

		LibFuncMap& eLibMap = eAnalyEngine.mVec_LibFunc[nLibIndex];
		std::string libPath = GetCurrentDirA() + "\\plugins\\esig\\" + eLibMap.libName + ".esig";

		//����ʶ��
		TrieTree esigTree(&eAnalyEngine);
		if (esigTree.LoadSig(libPath.c_str())) {
			for (unsigned int nFuncIndex = 0; nFuncIndex < eLibMap.vec_Funcs.size(); ++nFuncIndex) {
				char* pFuncName = esigTree.MatchFunc(eAnalyEngine.LinearAddrToVirtualAddr(eLibMap.vec_Funcs[nFuncIndex].addr));
				if (pFuncName) {
					eLibMap.vec_Funcs[nFuncIndex].name = pFuncName;
					std::string u16FuncName = LocalCpToUtf8(pFuncName);
					Script::Label::Set(eLibMap.vec_Funcs[nFuncIndex].addr, u16FuncName.c_str());
				}
				else {
					//To do...ģ��ƥ��
				}
			}
		}
		else {
			QString logMsg = QStringLiteral("->���������ļ�ʧ��:") + codec->toUnicode(libPath.c_str());
			ui.outMsg->appendPlainText(logMsg);
		}
		//���½���
		QString LibNameLine = codec->toUnicode(eLibMap.libName.c_str());
		LibNameLine.append(QStringLiteral("(��������:"));
		LibNameLine.append(QString::number(eLibMap.vec_Funcs.size()));
		LibNameLine.append(QStringLiteral(")"));
		ui.list_LibInfo->insertItem(ui.list_LibInfo->count(), new QListWidgetItem(LibNameLine));

		QString LibGuidLine = "   " + QString::fromLocal8Bit(eLibMap.libGuid.c_str());
		QListWidgetItem* pGuidItem = new QListWidgetItem(LibGuidLine);
		pGuidItem->setTextColor(QColor(150, 150, 150));
		ui.list_LibInfo->insertItem(ui.list_LibInfo->count(), pGuidItem);
		ui.list_LibInfo->insertItem(ui.list_LibInfo->count(), new QListWidgetItem(QStringLiteral("������������������������������������������������������������")));
	}

	//��ʼ����DLL�����
	if (eAnalyEngine.mVec_ImportsApi.size()) {

		//��������
		ui.table_Api->setColumnCount(4);
		ui.table_Api->setColumnWidth(0, 60);   //���õ�һ�п��
		ui.table_Api->setColumnWidth(1, 120);   //���õ�һ�п��
		ui.table_Api->setColumnWidth(2, 340);   //���õ�һ�п��
		ui.table_Api->setHorizontalHeaderLabels(QStringList() << QStringLiteral("���") << QStringLiteral("DLL��") << QStringLiteral("��������") << QStringLiteral("���ô���"));

		ui.tabWidget->addTab(ui.tab_Api, QStringLiteral("Api����"));
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
		}

		duint scanStartAddr = eAnalyEngine.m_UserCodeStartAddr;
		do 
		{
			scanStartAddr = Script::Pattern::FindMem(scanStartAddr + 1, eAnalyEngine.m_UserCodeEndAddr - scanStartAddr, "B8????0000E8");
			if (!scanStartAddr) {
				break;
			}
			unsigned int callAddr = eAnalyEngine.ReadCallAddr(scanStartAddr + 5);
			callAddr = ReadUInt(eAnalyEngine.LinearAddrToVirtualAddr(callAddr + 2));
			callAddr = ReadUInt(eAnalyEngine.LinearAddrToVirtualAddr(callAddr));
			if (callAddr == eAnalyEngine.m_KrnlApp.krnl_MCallDllCmd) {
				int index = ReadUInt(eAnalyEngine.LinearAddrToVirtualAddr(scanStartAddr + 1));
				Script::Comment::Set(scanStartAddr, LocalCpToUtf8(eAnalyEngine.mVec_ImportsApi[index].ApiName.c_str()).c_str());
			}
		} while (true);
	}

	
	return true;
}