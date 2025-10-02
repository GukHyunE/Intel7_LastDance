#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "numpad.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_numpad(new NumPad(this))
    , m_numpadReason(NumpadReason::None)
{
    ui->setupUi(this);

    // ComboBox setup
    ui->naviCB->addItem("길안내");
    ui->naviCB->addItem("스타벅스");
    ui->naviCB->addItem("화장실");
    ui->naviCB->addItem("면세점");

    // Numpad is a separate dialog, which will be launched modally
    // connect the passwordEntered signal from the numpad to our slot
    connect(m_numpad, &NumPad::passwordEntered, this, &MainWindow::onPasswordEntered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_carryPB_clicked()
{
    ui->statusLabel->setText("돌쇠가 마님을 따라가는 중입니다");
}

void MainWindow::on_naviCB_activated(int index)
{
    switch(index) {
        case 0: // 길안내
            ui->statusLabel->setText("어디로 안내할까요?");
            break;
        case 1: // 스타벅스
            ui->statusLabel->setText("돌쇠가 스타벅스로 마님을 안내하는 중입니다.");
            break;
        case 2: // 화장실
            ui->statusLabel->setText("돌쇠가 화장실로 마님을 안내하는 중입니다.");
            break;
        case 3: // 면세점
            ui->statusLabel->setText("돌쇠가 면세점으로 마님을 안내하는 중입니다.");
            break;
    }
}

void MainWindow::on_tempPB_clicked()
{
    if (ui->tempPB->text() == "일시 정지") {
        ui->tempPB->setText("시작");
    } else { // "시작"
                    m_numpadReason = NumpadReason::Start;
                    m_numpad->clear();
                    m_numpad->exec();    }
}

void MainWindow::on_endCB_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "종료", "정말 종료하시겠습니까?",
                                    QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        m_numpadReason = NumpadReason::Shutdown;
        m_numpad->clear();
        m_numpad->exec();
    }
}

void MainWindow::onPasswordEntered(const QString &password)
{
    // The original password was hardcoded as "1234".
    // As requested, the check is now bypassed and will always succeed.
    // const QString correctPassword = "1234"; 

    // if (password == correctPassword) {
        switch (m_numpadReason) {
            case NumpadReason::Start:
                ui->tempPB->setText("일시 정지");
                QMessageBox::information(this, "성공", "인증에 성공했습니다.");
                break;
            case NumpadReason::Shutdown:
                ui->stackedWidget->setCurrentWidget(ui->gohomePage);
                break;
            case NumpadReason::None:
                // Should not happen
                break;
        }
    // } else {
    //     QMessageBox::warning(this, "실패", "비밀번호가 틀렸습니다.");
    // }

    // Reset the reason
    m_numpadReason = NumpadReason::None;
}