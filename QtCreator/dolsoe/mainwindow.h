#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class NumPad; // Forward declaration

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum class NumpadReason {
        None,
        Start,
        Shutdown
    };

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_carryPB_clicked();
    void on_naviCB_activated(int index);
    void on_tempPB_clicked();
    void on_endCB_clicked();
    void onPasswordEntered(const QString &password);

private:
    Ui::MainWindow *ui;
    NumPad *m_numpad;
    NumpadReason m_numpadReason;
};
#endif // MAINWINDOW_H
