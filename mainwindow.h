#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // UI에 정의된 버튼들의 자동 연결 슬롯
    void on_pushButton_Clear_clicked();    // 입력 필드 초기화
    void on_pushButton_Add_clicked();      // 추가 및 수정 (저장)
    void on_pushButton_Delete_clicked();   // 제거
    void on_pushButton_Search_clicked();   // 검색 다이얼로그 열기
    void on_pushButton_SaveFile_clicked(); // 선택된 고객 JSON 내보내기
    void on_pushButton_LoadFile_clicked(); // JSON 불러오기
    void on_pushButton_Exit_clicked();     // 종료

    // 트리 뷰에서 고객 선택 시 정보 표시를 위한 슬롯
    void on_treeView_List_selectionChanged();

private:
    Ui::MainWindow *ui;
    void setSystemStyle();
    QStandardItemModel *mainModel; // 전체 고객 데이터를 담는 핵심 모델

    void clearInputFields(); // 입력창 초기화 함수
};

#endif // MAINWINDOW_H