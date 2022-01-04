#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QProcess;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    virtual void closeEvent(QCloseEvent* event);

private Q_SLOTS:
     void readyReadOutput();
     void readyReadError();

     void on_open_triggered();
     void on_save_triggered();
     void on_clear_triggered();

private:
     void readSettings();
     void writeSettings();

private:
    Ui::MainWindow *ui;
    QProcess *m_pProcess;
};

#endif // MAINWINDOW_H
