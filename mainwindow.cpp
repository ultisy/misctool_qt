#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "searchdialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDate>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("PC Room Management System - VEDA-PC-ROOM");
    setSystemStyle();

    // 1. 공용 데이터 모델 생성 및 헤더 설정
    mainModel = new QStandardItemModel(0, 10, this);
    mainModel->setHorizontalHeaderLabels({"이름", "아이디", "비밀번호", "잔여금액", "전화번호", "생년월일", "성별", "이메일", "주소", "메모"});

    // 2. 트리 뷰 설정
    ui->InfoView_2->setModel(mainModel);
    ui->InfoView_2->setEditTriggers(QAbstractItemView::NoEditTriggers); // 읽기 전용
    ui->InfoView_2->setSelectionBehavior(QAbstractItemView::SelectRows); // 행 단위 선택
    ui->InfoView_2->setColumnHidden(2, true);

    // 1000원씩 올라가고 내려가도록 설정
    ui->spinBox_Charge->setSingleStep(1000);
    ui->spinBox_Charge->setMaximum(10000000);

    // 1. 초기 상태 설정
    ui->lineEdit_PW->setEchoMode(QLineEdit::Password);

    // 2. 액션 생성 (아이콘은 프로젝트에 포함된 눈 모양 아이콘 경로를 적어주세요)
    // 여기서는 예시로 Qt 표준 아이콘을 사용하거나 텍스트를 사용합니다.
    QAction *toggleAction = ui->lineEdit_PW->addAction(
        QIcon(":/icon_eye_closed.png"), // 임시 아이콘
        QLineEdit::TrailingPosition
        );


    // 들여쓰기 없애기
    ui->InfoView_2->setRootIsDecorated(false);
    ui->InfoView_2->setIndentation(5);

    QHeaderView *header = ui->InfoView_2->header();

    // 모드를 Interactive(사용자 조절 가능)로 설정
    header->setSectionResizeMode(0, QHeaderView::Interactive);
    header->setSectionResizeMode(1, QHeaderView::Interactive);
    header->setSectionResizeMode(3, QHeaderView::Interactive);

    // 3. 액션 클릭 시 동작 연결
    connect(toggleAction, &QAction::triggered, this, [=]() {
        if (ui->lineEdit_PW->echoMode() == QLineEdit::Password) {
            ui->lineEdit_PW->setEchoMode(QLineEdit::Normal);
            // 보이기 상태 아이콘으로 변경 가능
            toggleAction->setIcon(QIcon(":/icon_eye_open.png"));
        } else {
            ui->lineEdit_PW->setEchoMode(QLineEdit::Password);
            // 숨기기 상태 아이콘으로 변경 가능
            toggleAction->setIcon(QIcon(":/icon_eye_closed.png"));
        }
    });

    // 3. 리스트 선택 변경 시그널 연결
    connect(ui->InfoView_2->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::on_InfoView_2_selectionChanged);


    connect(ui->QuitBtn_2, &QPushButton::clicked, qApp, &QApplication::exit);
    connect(ui->ConsumerBtn_2, &QPushButton::clicked, this,
            [&]()
            {
                ui->stackedWidget->setCurrentWidget(ui->page_one_2);
            }
            );

    connect(ui->LobbyBtn_2, &QPushButton::clicked, this,
            [&]()
            {
                ui->stackedWidget->setCurrentWidget(ui->page_three);
            }
            );

    connect(ui->InfoAddBtn_2, &QPushButton::clicked, this,
            [&]()
            {
                ui->stackedWidget->setCurrentWidget(ui->page_two_2);
            }
            );


    connect(ui->pushButton_Back, &QPushButton::clicked, this,
            [&]()
            {
                clearInputFields();
                ui->stackedWidget->setCurrentWidget(ui->page_one_2);
            }
            );
    ui->lineEdit_Phone->setInputMask("\\0\\1\\0 - 0000 - 0000; ");
    setLineEditProperty();
}


void MainWindow::clearInputFields() {
    ui->lineEdit_Name->clear();
    ui->lineEdit_ID->clear();
    ui->lineEdit_PW->clear();
    ui->spinBox_Charge->setValue(0);
    ui->lineEdit_Phone->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->radioButton_Male->setChecked(true);
    ui->lineEdit_Email->clear();
    ui->textEdit_Address->clear();
    ui->textEdit_Memo->clear();
}

void MainWindow::on_pushButton_Add_clicked() {
    QString id = ui->lineEdit_ID->text();
    if (id.isEmpty() || ui->lineEdit_Name->text().isEmpty()) {
        QMessageBox::warning(this, "경고", "이름과 아이디는 필수 입력 사항입니다.");
        return;
    }

    // 모델에서 현재 입력된 ID가 이미 존재하는지 검색
    int targetRow = -1;
    for (int i = 0; i < mainModel->rowCount(); ++i) {
        if (mainModel->item(i, 1)->text() == id) {
            targetRow = i; // 기존 고객이면 해당 행을 타겟으로 설정
            break;
        }
    }


    // 존재하지 않으면 새 행 추가
    if (targetRow == -1) {
        targetRow = mainModel->rowCount();
        mainModel->insertRow(targetRow);
    }


    mainModel->setItem(targetRow, 0, new QStandardItem(ui->lineEdit_Name->text()));
    mainModel->setItem(targetRow, 1, new QStandardItem(id));
    mainModel->setItem(targetRow, 2, new QStandardItem(ui->lineEdit_PW->text()));
    mainModel->setItem(targetRow, 3, new QStandardItem(QString::number(ui->spinBox_Charge->value())));
    mainModel->setItem(targetRow, 4, new QStandardItem(ui->lineEdit_Phone->text()));
    mainModel->setItem(targetRow, 5, new QStandardItem(ui->dateEdit->date().toString("yyyy-MM-dd")));
    mainModel->setItem(targetRow, 6, new QStandardItem(ui->radioButton_Male->isChecked() ? "남" : "여"));
    mainModel->setItem(targetRow, 7, new QStandardItem(ui->lineEdit_Email->text()));
    mainModel->setItem(targetRow, 8, new QStandardItem(ui->textEdit_Address->toPlainText()));
    mainModel->setItem(targetRow, 9, new QStandardItem(ui->textEdit_Memo->toPlainText()));


    QMessageBox::information(this, "성공", "저장되었습니다.");
    clearInputFields();
    ui->stackedWidget->setCurrentWidget(ui->page_one_2);
}

void MainWindow::on_InfoDelBtn_2_clicked() {
    QModelIndex currentIndex = ui->InfoView_2->currentIndex();
    if (currentIndex.isValid()) {
        mainModel->removeRow(currentIndex.row());
        clearInputFields();
        QMessageBox::information(this, "성공", "제거되었습니다.");
    } else {
        QMessageBox::warning(this, "경고", "제거할 고객을 리스트에서 선택해주세요.");
    }
}

// [검색 다이얼로그 호출]
void MainWindow::on_SearchBtn_2_clicked() {
    SearchDialog searchDlg(mainModel, this);
    searchDlg.exec();
}

void MainWindow::on_InfoView_2_selectionChanged() {
    QModelIndex currentIndex = ui->InfoView_2->currentIndex();
    if (!currentIndex.isValid()) return;

    int row = currentIndex.row();
    ui->lineEdit_Name->setText(mainModel->item(row, 0)->text());
    ui->lineEdit_ID->setText(mainModel->item(row, 1)->text());
    ui->lineEdit_PW->setText(mainModel->item(row, 2)->text());
    ui->spinBox_Charge->setValue(mainModel->item(row, 3)->text().toInt());
    ui->lineEdit_Phone->setText(mainModel->item(row, 4)->text());
    ui->dateEdit->setDate(QDate::fromString(mainModel->item(row, 5)->text(), "yyyy-MM-dd"));
    QString gender = mainModel->item(row, 6)->text();
    if(gender == "남") ui->radioButton_Male->setChecked(true);
    else ui->radioButton_Female->setChecked(true);
    ui->lineEdit_Email->setText(mainModel->item(row, 7)->text());
    ui->textEdit_Address->setPlainText(mainModel->item(row, 8)->text());
    ui->textEdit_Memo->setPlainText(mainModel->item(row, 9)->text());

    ui->stackedWidget->setCurrentWidget(ui->page_two_2);
}

void MainWindow::setLineEditProperty()
{
    // 최대 입력 길이 설정
    ui->lineEdit_Name->setMaxLength(20);
    ui->lineEdit_ID->setMaxLength(20);
    ui->lineEdit_PW->setMaxLength(20);
    ui->lineEdit_Email->setMaxLength(50);

    // 플레이스홀더 텍스트
    ui->lineEdit_Name->setPlaceholderText("이름을 입력하세요...");
    ui->lineEdit_ID->setPlaceholderText("아이디를 입력하세요...");
    ui->lineEdit_PW->setPlaceholderText("비밀번호를 입력하세요...");
    ui->lineEdit_Email->setPlaceholderText("example@email.com");
    ui->textEdit_Address->setPlaceholderText("주소를 입력하세요...");
}

void MainWindow::on_InfoView_2_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;
    // goToDetailPage(index.row());
}

bool MainWindow::saveCurrentData() {
    QString id = ui->lineEdit_ID->text();
    QString name = ui->lineEdit_Name->text();

    if (id.isEmpty() || name.isEmpty()) {
        QMessageBox::warning(this, "경고", "이름과 아이디는 필수 입력 사항입니다.");
        return false;
    }

    // 모델에서 현재 입력된 ID가 이미 존재하는지 검색
    int targetRow = -1;
    for (int i = 0; i < mainModel->rowCount(); ++i) {
        if (mainModel->item(i, 1)->text() == id) {
            if (mainModel->item(i, 0)->text() != name) {
                // 이름이 다르면 → 중복 ID 차단
                QMessageBox::warning(this, "경고", "이미 사용 중인 아이디입니다.");
                return false;
            }

            targetRow = i; // 기존 고객이면 해당 행을 타겟으로 설정
            break;
        }
    }

    // 존재하지 않으면 새 행 추가
    if (targetRow == -1) {
        targetRow = mainModel->rowCount();
        mainModel->insertRow(targetRow);
    }

    // 데이터 저장 (추가/수정 공통)
    mainModel->setItem(targetRow, 0, new QStandardItem(ui->lineEdit_Name->text()));
    mainModel->setItem(targetRow, 1, new QStandardItem(id));
    mainModel->setItem(targetRow, 2, new QStandardItem(ui->lineEdit_PW->text()));
    mainModel->setItem(targetRow, 3, new QStandardItem(QString::number(ui->spinBox_Charge->value())));
    mainModel->setItem(targetRow, 4, new QStandardItem(ui->lineEdit_Phone->text()));
    mainModel->setItem(targetRow, 5, new QStandardItem(ui->dateEdit->date().toString("yyyy-MM-dd")));
    mainModel->setItem(targetRow, 6, new QStandardItem(ui->radioButton_Male->isChecked() ? "남" : "여"));
    mainModel->setItem(targetRow, 7, new QStandardItem(ui->lineEdit_Email->text()));
    mainModel->setItem(targetRow, 8, new QStandardItem(ui->textEdit_Address->toPlainText()));
    mainModel->setItem(targetRow, 9, new QStandardItem(ui->textEdit_Memo->toPlainText()));

    return true;
}


// [JSON 내보내기] - 선택된 고객 데이터를 JSON 파일로 저장
void MainWindow::on_pushButton_SaveFile_clicked() {
    QModelIndex currentIndex = ui->InfoView_2->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "경고", "내보낼 고객을 리스트에서 선택해주세요.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "JSON 내보내기", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    int row = currentIndex.row();
    QJsonObject obj;
    obj["name"] = mainModel->item(row, 0)->text();
    obj["id"] = mainModel->item(row, 1)->text();
    obj["pw"] = mainModel->item(row, 2)->text();
    obj["charge"] = mainModel->item(row, 3)->text().toInt();
    obj["phone"] = mainModel->item(row, 4)->text();
    obj["birthday"] = mainModel->item(row, 5)->text();
    obj["gender"] = mainModel->item(row, 6)->text();
    obj["email"] = mainModel->item(row, 7)->text();
    obj["address"] = mainModel->item(row, 8)->text();
    obj["memo"] = mainModel->item(row, 9)->text();

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(obj).toJson());
        file.close();
        QMessageBox::information(this, "성공", "고객 정보가 내보내기 되었습니다.");
    }
}

// [JSON 불러오기] - JSON 파일을 읽어 UI 입력 폼에 띄움
void MainWindow::on_pushButton_LoadFile_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "JSON 불러오기", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject obj = doc.object();
        file.close();

        // 입력 폼에 데이터 세팅 (사용자가 확인 후 '저장'을 누르면 리스트에 등록됨)
        ui->lineEdit_Name->setText(obj["name"].toString());
        ui->lineEdit_ID->setText(obj["id"].toString());
        ui->lineEdit_PW->setText(obj["pw"].toString());
        ui->spinBox_Charge->setValue(obj["charge"].toInt());
        ui->lineEdit_Phone->setText(obj["phone"].toString());
        ui->dateEdit->setDate(QDate::fromString(obj["birthday"].toString(), "yyyy-MM-dd"));
        QString gender = obj["gender"].toString();
        if(gender == "남") ui->radioButton_Male->setChecked(true);
        else ui->radioButton_Female->setChecked(true);
        ui->lineEdit_Email->setText(obj["email"].toString());
        ui->textEdit_Address->setText(obj["address"].toString());
        ui->textEdit_Memo->setText(obj["memo"].toString());

        if (!saveCurrentData()) return;

        clearInputFields();
        QMessageBox::information(this, "성공", "데이터를 불러와 리스트에 등록하였습니다.");
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSystemStyle()
{

    QString fullStyle = R"(
        QWidget {
            font-family: 'MonaS10x12';
            background-color: #f7f7f7;
            color: black;
            font-size: 12px;
        }

        QLineEdit, QTextEdit {
            background-color: #ffffff;
            padding: 3px 5px;
        }


        QRadioButton::indicator {
            height: 14px;
            width: 14px;
            border-style:solid;
            border-radius:7px;
            border-width: 1px;
        }
        QRadioButton::indicator:checked {
            border-color: #48a5fd;
            background-color: qradialgradient(cx:0.5, cy:0.5, radius:0.4,fx:0.5, fy:0.5, stop:0 #ffffff, stop:0.5 #ffffff, stop:0.6 #48a5fd, stop:1 #48a5fd);
        }
        QRadioButton::indicator:!checked {
            border-color: #a9b7c6;
            background-color: #fbfdfa;
        }

        QDateEdit {
            background-color: #ffffff;
        }

        QPushButton {
            background-color: #3381ff;
            color: white;
            font-size: 12px;
            padding: 3px 6px;
            border-radius: 5px;

            border-bottom-color: #c9c9c9;
            border-bottom-width: 2px;
            border-style: groove;
        }

        QPushButton:hover {
            background-color: #0565ff;
            border: none;
        }

        QPushButton:pressed {
            background-color: #4c87e6;
        }

        QTreeView {
            background-color: #fcfcfc;
            border: 1px solid #4f81d1;
        }

        QTreeView::item {
            height: 35px;
        }

        QHeaderView::section {
            background-color: #4f81d1;
            color: white;
            padding: 4px;
            border: none;
        }

    )";

    this->setStyleSheet(fullStyle);

    // int id = QFontDatabase::addApplicationFont(":resources/Pre.ttf");
}
