#include "MainWindow.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QString>
#include "pluginsdk/_scriptapi_label.h"
#include "TrieTree.h"
#include "public.h"

MainWindow::MainWindow(unsigned int dwBase, QWidget* parent) : QWidget(parent)
{
	ui.setupUi(this);

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

	if (!eAnalyEngine.InitEAnalyEngine(dwBase)) {
		QMessageBox::critical(0, QStringLiteral("��Ǹ"), QStringLiteral("��������Գ���ʧ��"));
		this->close();
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
	ui.outMsg->appendPlainText(QStringLiteral("->ʶ��������֧�ֿ⺯��..."));

	QTextCodec* codec = QTextCodec::codecForName("GB2312");
	for (unsigned int nLibIndex = 0; nLibIndex < eAnalyEngine.mVec_LibFunc.size(); ++nLibIndex) {

		LibFuncMap& eLibMap = eAnalyEngine.mVec_LibFunc[nLibIndex];
		std::string libPath = GetCurrentDirA() + "\\plugins\\esig\\" + eLibMap.libName + ".esig";

		//����ʶ��
		TrieTree esigTree(&eAnalyEngine);
		if (!esigTree.LoadSig(libPath.c_str())) {
			continue;
		}
		for (unsigned int nFuncIndex = 0; nFuncIndex < eLibMap.vec_Funcs.size(); ++nFuncIndex) {
			char* pFuncName = esigTree.MatchFunc(eAnalyEngine.LinearAddrToVirtualAddr(eLibMap.vec_Funcs[nFuncIndex].addr));
			if (pFuncName) {
				eLibMap.vec_Funcs[nFuncIndex].name = pFuncName;
				std::string u16FuncName = LocalCpToUtf8(pFuncName);
				Script::Label::Set(eLibMap.vec_Funcs[nFuncIndex].addr, u16FuncName.c_str());
			}
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
	return true;
}