#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "SearchDialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDate>
#include <QAction>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setSystemStyle();
    setWindowTitle("PC Room Management System");

    // 1. 공용 데이터 모델 생성 및 헤더 설정
    mainModel = new QStandardItemModel(0, 9, this);
    mainModel->setHorizontalHeaderLabels({"이름", "아이디", "비밀번호", "잔여금액", "전화번호", "생년월일", "이메일", "주소", "즐겨찾기"});

    // 2. 트리 뷰 설정
    ui->treeView_List->setModel(mainModel);
    ui->treeView_List->setEditTriggers(QAbstractItemView::NoEditTriggers); // 읽기 전용
    ui->treeView_List->setSelectionBehavior(QAbstractItemView::SelectRows); // 행 단위 선택

    // 리스트에는 이름(0)과 아이디(1)만 보이도록 나머지 열 숨기기
    for(int i = 2; i < 9; ++i) {
        ui->treeView_List->setColumnHidden(i, true);
    }

    // 1000원씩 올라가고 내려가도록 설정
    ui->spinBox_Charge->setSingleStep(1000);
    ui->spinBox_Charge->setMaximum(10000000);

    // 1. 초기 상태 설정
    ui->lineEdit_PW->setEchoMode(QLineEdit::Password);

    // 2. 액션 생성 (아이콘은 프로젝트에 포함된 눈 모양 아이콘 경로를 적어주세요)
    // 여기서는 예시로 Qt 표준 아이콘을 사용하거나 텍스트를 사용합니다.
    QAction *toggleAction = ui->lineEdit_PW->addAction(
        style()->standardIcon(QStyle::SP_FileDialogContentsView), // 임시 아이콘
        QLineEdit::TrailingPosition
        );

    // 3. 액션 클릭 시 동작 연결
    connect(toggleAction, &QAction::triggered, this, [=]() {
        if (ui->lineEdit_PW->echoMode() == QLineEdit::Password) {
            ui->lineEdit_PW->setEchoMode(QLineEdit::Normal);
            // 보이기 상태 아이콘으로 변경 가능
            // toggleAction->setIcon(QIcon(":/icons/eye_open.png"));
        } else {
            ui->lineEdit_PW->setEchoMode(QLineEdit::Password);
            // 숨기기 상태 아이콘으로 변경 가능
            // toggleAction->setIcon(QIcon(":/icons/eye_closed.png"));
        }
    });

    // 3. 리스트 선택 변경 시그널 연결
    connect(ui->treeView_List->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::on_treeView_List_selectionChanged);
}

MainWindow::~MainWindow() {
    delete ui;
}

// [리스트 항목 선택 시]: 입력 폼에 해당 고객 데이터 채우기
void MainWindow::on_treeView_List_selectionChanged() {
    QModelIndex currentIndex = ui->treeView_List->currentIndex();
    if (!currentIndex.isValid()) return;

    int row = currentIndex.row();
    ui->lineEdit_Name->setText(mainModel->item(row, 0)->text());
    ui->lineEdit_ID->setText(mainModel->item(row, 1)->text());
    ui->lineEdit_PW->setText(mainModel->item(row, 2)->text());
    ui->spinBox_Charge->setValue(mainModel->item(row, 3)->text().toInt());
    ui->lineEdit_Phone->setText(mainModel->item(row, 4)->text());
    ui->dateEdit->setDate(QDate::fromString(mainModel->item(row, 5)->text(), "yyyy-MM-dd"));
    ui->lineEdit_Email->setText(mainModel->item(row, 6)->text());
    ui->lineEdit_Address->setText(mainModel->item(row, 7)->text());
    ui->checkBox_Favorite->setChecked(mainModel->item(row, 8)->text() == "O");
}

// 입력 필드 초기화
void MainWindow::clearInputFields() {
    ui->lineEdit_Name->clear();
    ui->lineEdit_ID->clear();
    ui->lineEdit_PW->clear();
    ui->spinBox_Charge->setValue(0);
    ui->lineEdit_Phone->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->lineEdit_Email->clear();
    ui->lineEdit_Address->clear();
    ui->checkBox_Favorite->setChecked(false);
}

void MainWindow::on_pushButton_Clear_clicked() {
    ui->treeView_List->clearSelection(); // 리스트 선택 해제
    clearInputFields();                  // 필드 초기화
}

// [추가/수정 - 저장 버튼]
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

    // 데이터 저장 (추가/수정 공통)
    mainModel->setItem(targetRow, 0, new QStandardItem(ui->lineEdit_Name->text()));
    mainModel->setItem(targetRow, 1, new QStandardItem(id));
    mainModel->setItem(targetRow, 2, new QStandardItem(ui->lineEdit_PW->text()));
    mainModel->setItem(targetRow, 3, new QStandardItem(QString::number(ui->spinBox_Charge->value())));
    mainModel->setItem(targetRow, 4, new QStandardItem(ui->lineEdit_Phone->text()));
    mainModel->setItem(targetRow, 5, new QStandardItem(ui->dateEdit->date().toString("yyyy-MM-dd")));
    mainModel->setItem(targetRow, 6, new QStandardItem(ui->lineEdit_Email->text()));
    mainModel->setItem(targetRow, 7, new QStandardItem(ui->lineEdit_Address->text()));
    mainModel->setItem(targetRow, 8, new QStandardItem(ui->checkBox_Favorite->isChecked() ? "O" : "X"));

    QMessageBox::information(this, "성공", "저장되었습니다.");
    clearInputFields();
}

// [제거 버튼]
void MainWindow::on_pushButton_Delete_clicked() {
    QModelIndex currentIndex = ui->treeView_List->currentIndex();
    if (currentIndex.isValid()) {
        mainModel->removeRow(currentIndex.row());
        clearInputFields();
        QMessageBox::information(this, "성공", "제거되었습니다.");
    } else {
        QMessageBox::warning(this, "경고", "제거할 고객을 리스트에서 선택해주세요.");
    }
}

// [검색 다이얼로그 호출]
void MainWindow::on_pushButton_Search_clicked() {
    SearchDialog searchDlg(mainModel, this);
    searchDlg.exec();
}

// [JSON 내보내기] - 선택된 고객 데이터를 JSON 파일로 저장
void MainWindow::on_pushButton_SaveFile_clicked() {
    QModelIndex currentIndex = ui->treeView_List->currentIndex();
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
    obj["email"] = mainModel->item(row, 6)->text();
    obj["address"] = mainModel->item(row, 7)->text();
    obj["favorite"] = (mainModel->item(row, 8)->text() == "O");

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
        ui->lineEdit_Email->setText(obj["email"].toString());
        ui->lineEdit_Address->setText(obj["address"].toString());
        ui->checkBox_Favorite->setChecked(obj["favorite"].toBool());

        QMessageBox::information(this, "안내", "데이터를 불러왔습니다. 확인 후 [저장] 버튼을 누르면 등록됩니다.");
    }
}

// [종료 버튼]
void MainWindow::on_pushButton_Exit_clicked() {
    this->close();
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

        QLineEdit, QDateEdit, QTextEdit {
            background-color: #ffffff;
            border: 1px solid #cccccc;
            border-radius: 5px;
            font-size: 12px;
            padding: 2px;
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

        QHeaderView::section {
            background-color: #4f81d1;
            color: black;
            padding: 4px;
            border: none;
        }

    )";

    this->setStyleSheet(fullStyle);

    // int id = QFontDatabase::addApplicationFont(":resources/Pre.ttf");
}