#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scene3d.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, m_ui(new Ui::MainWindow)
{
	m_ui->setupUi(this);

	setCentralWidget(new Scene3D());
}

MainWindow::~MainWindow()
{
	delete m_ui;
}
