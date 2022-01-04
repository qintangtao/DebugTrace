#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDebug>
#include <QStringList>
#include <QFileDialog>
#include <QSettings>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pProcess = new QProcess(this);
    connect(m_pProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadOutput()));
    connect(m_pProcess, SIGNAL(readyReadStandardError()), this, SLOT(readyReadError()));

    connect(ui->actionopen, SIGNAL(triggered()), this, SLOT(on_open_triggered()));
    connect(ui->actionsave, SIGNAL(triggered()), this, SLOT(on_save_triggered()));
    connect(ui->actionclear, SIGNAL(triggered()), this, SLOT(on_clear_triggered()));

    readSettings();
}

MainWindow::~MainWindow()
{
    m_pProcess->kill();
    m_pProcess->deleteLater();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (!isMinimized() && !isMaximized() && !isFullScreen())
        writeSettings();

    QMainWindow::closeEvent(event);
}


void MainWindow::on_open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("文件对话框！"),
                                                        "",
                                                        tr("文件(*exe);;"));
   qDebug()<<"filename : "<<fileName;

   ui->statusBar->showMessage(fileName);
   ui->textBrowser->clear();

   m_pProcess->kill();
   m_pProcess->waitForFinished();

   m_pProcess->start(fileName);
   m_pProcess->waitForStarted();
}

void MainWindow::on_save_triggered()
{
    QString selectDir = QFileDialog::getExistingDirectory();
    qDebug()<<"filename : "<<selectDir;

   QString fileName;

   QDir dir(selectDir);
   for (int i = 1; i < INT_MAX; i++) {
       fileName = dir.absoluteFilePath(QString("DebugTrace%1.log").arg(i > 1 ? QString::number(i) : ""));
       if (!QFile::exists(fileName))
           break;
   }

   QFile f(fileName);
   if (f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
   {
       f.write(ui->textBrowser->toPlainText().toUtf8());
       f.close();
   }
}


void MainWindow::on_clear_triggered()
{
    ui->textBrowser->clear();
}

void MainWindow::readyReadOutput()
{
    QString message = QString::fromUtf8(m_pProcess->readAllStandardOutput());
    message.remove(QLatin1String("\r\n"));
    ui->textBrowser->append(message);
    qDebug() << message;
}

void MainWindow::readyReadError()
{
    QString message = QString::fromUtf8(m_pProcess->readAllStandardError());
    message.remove(QLatin1String("\r\n"));
    ui->textBrowser->append(message);
    qDebug() << message;
}

void MainWindow::readSettings()
{
    QSettings settings(this);
    QRect geom = QApplication::desktop()->availableGeometry();
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(2 * geom.width() / 3, 2 * geom.height() / 3)).toSize();
    move(pos); resize(size);
}

void MainWindow::writeSettings()
{
    QSettings settings(this);
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}
