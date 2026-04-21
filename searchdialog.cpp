#include "searchdialog.h"
#include "./ui_searchdialog.h"
#include <QHeaderView>

SearchDialog::SearchDialog(QStandardItemModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::searchDialog)
{
    ui->setupUi(this);

    // 프록시 모델을 생성하고 MainWindow에서 받아온 원본 모델을 연결
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive); // 대소문자 구분 없음

    // 테이블 뷰 설정
    ui->tableView->setModel(proxyModel);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 비밀번호 열 등 불필요한 정보 숨기기 (선택 사항)
    ui->tableView->setColumnHidden(2, true);
}

SearchDialog::~SearchDialog() {
    delete ui;
}

// [검색 버튼 클릭 시]
void SearchDialog::on_pushButton_clicked() {
    // lineEdit에 입력된 값을 기반으로 '이름(0번 열)' 컬럼을 정규식 필터링
    QString keyword = ui->lineEdit->text();
    proxyModel->setFilterKeyColumn(0);
    proxyModel->setFilterRegularExpression(keyword);
}