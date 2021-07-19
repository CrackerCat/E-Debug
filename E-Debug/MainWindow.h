#pragma once
#include <QtWidgets/QDialog>
#include "ui_MainWindow.h"
#include "EAnalyEngine.h"

class MainWindow :public QWidget
{
	Q_OBJECT
public:
	MainWindow(unsigned int dwBase, QWidget* parent = Q_NULLPTR);
	~MainWindow();
public:
	//׼�������Ծ�̬������򴰿�
	bool InitWindow_EStatic();
private slots:
	void on_LibNameSelected(const QString& currentText);
	void on_FuncDoubleClicked(QTableWidgetItem* pItem);
private:
	Ui::MainWindow ui;
	EAnalyEngine eAnalyEngine;
};