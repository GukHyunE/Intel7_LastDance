#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "numpad.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_numpad(new NumPad(this))
    , m_numpadReason(NumpadReason::None)
    , m_currentLanguage("KR") // Default to Korean
    , m_isStopped(false) // Default to running state
{
    ui->setupUi(this);

    updateTexts(); // Set initial texts

    // Numpad is a separate dialog, which will be launched modally
    // connect the passwordEntered signal from the numpad to our slot
    connect(m_numpad, &NumPad::passwordEntered, this, &MainWindow::onPasswordEntered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTexts()
{
    if (m_currentLanguage == "KR") {
        // Set Korean texts
        ui->langKrPB->setText("한국어");
        ui->langEnPB->setText("English");
        ui->label->setText("바프찍는돌쇠");
        ui->carryPB->setText("따라오너라");
        ui->endCB->setText("종료하거라");
        ui->label_2->setText("돌쇠 집가는 사진");
        ui->label_3->setText("돌쇠 퇴근해유~~");

        if (m_isStopped) {
            ui->tempPB->setText("시작하거라");
        } else {
            ui->tempPB->setText("정지하거라");
        }

        // ComboBox items
        ui->naviCB->clear();
        ui->naviCB->addItem("안내하거라");
        ui->naviCB->addItem("스타벅스");
        ui->naviCB->addItem("화장실");
        ui->naviCB->addItem("면세점");

    } else if (m_currentLanguage == "EN") {
        // Set English texts
        ui->langKrPB->setText("한국어");
        ui->langEnPB->setText("English");
        ui->label->setText("Dolsoe taking a body profile photo");
        ui->carryPB->setText("Follow Me");
        ui->endCB->setText("Shutdown");
        ui->label_2->setText("Dolsoe going home");
        ui->label_3->setText("Dolsoe is leaving~~");

        if (m_isStopped) {
            ui->tempPB->setText("Start");
        } else {
            ui->tempPB->setText("Stop");
        }

        // ComboBox items
        ui->naviCB->clear();
        ui->naviCB->addItem("Guide");
        ui->naviCB->addItem("Starbucks");
        ui->naviCB->addItem("Restroom");
        ui->naviCB->addItem("Duty-Free");
    }
}

void MainWindow::on_langKrPB_clicked()
{
    m_currentLanguage = "KR";
    updateTexts();
}

void MainWindow::on_langEnPB_clicked()
{
    m_currentLanguage = "EN";
    updateTexts();
}

void MainWindow::on_carryPB_clicked()
{
    if (m_currentLanguage == "KR") {
        ui->statusLabel->setText("돌쇠가 마님을 따라가는 중입니다");
    } else {
        ui->statusLabel->setText("Dolsoe is following you.");
    }
}

void MainWindow::on_naviCB_activated(int index)
{
    if (m_currentLanguage == "KR") {
        switch(index) {
            case 0: // 길안내
                ui->statusLabel->setText("어디로 뫼실까요?");
                break;
            case 1: // 스타벅스
                ui->statusLabel->setText("돌쇠가 스타벅스로 마님을 모시는 중입니다.");
                break;
            case 2: // 화장실
                ui->statusLabel->setText("돌쇠가 화장실로 마님을 모시는 중입니다.");
                break;
            case 3: // 면세점
                ui->statusLabel->setText("돌쇠가 면세점으로 마님을 모시는 중입니다.");
                break;
        }
    } else { // English
        switch(index) {
            case 0: // Guide
                ui->statusLabel->setText("Where can I take you?");
                break;
            case 1: // Starbucks
                ui->statusLabel->setText("Dolsoe is taking you to Starbucks.");
                break;
            case 2: // Restroom
                ui->statusLabel->setText("Dolsoe is taking you to the restroom.");
                break;
            case 3: // Duty-Free
                ui->statusLabel->setText("Dolsoe is taking you to the duty-free shop.");
                break;
        }
    }
}

void MainWindow::on_tempPB_clicked()
{
    if (m_isStopped) { // If it is stopped, we want to start
        m_numpadReason = NumpadReason::Start;
        m_numpad->clear();
        m_numpad->exec();
    } else { // If it is running, we want to stop
        m_isStopped = true;
        updateTexts();
    }
}

void MainWindow::on_endCB_clicked()
{
    QMessageBox::StandardButton reply;
    QString title = (m_currentLanguage == "KR") ? "종료하거라" : "Shutdown";
    QString text = (m_currentLanguage == "KR") ? "정말 종료하시겠습니까?" : "Are you sure you want to shut down?";

    reply = QMessageBox::question(this, title, text,
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
                m_isStopped = false; // Now it is running
                updateTexts();
                if (m_currentLanguage == "KR") {
                    QMessageBox::information(this, "성공", "인증에 성공했습니다.");
                } else {
                    QMessageBox::information(this, "Success", "Authentication successful.");
                }
                break;
            case NumpadReason::Shutdown:
                ui->stackedWidget->setCurrentWidget(ui->gohomePage);
                break;
            case NumpadReason::None:
                // Should not happen
                break;
        }
    // } else {
    //     if (m_currentLanguage == "KR") {
    //         QMessageBox::warning(this, "실패", "비밀번호가 틀렸습니다.");
    //     } else {
    //         QMessageBox::warning(this, "Failure", "Incorrect password.");
    //     }
    // }

    // Reset the reason
    m_numpadReason = NumpadReason::None;
}