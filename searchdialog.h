#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

namespace Ui { class searchDialog; }

class SearchDialog : public QDialog {
    Q_OBJECT

public:
    explicit SearchDialog(QStandardItemModel *model, QWidget *parent = nullptr);
    ~SearchDialog();

private slots:
    void on_pushButton_clicked(); // UI의 검색 버튼

private:
    Ui::searchDialog *ui;
    QSortFilterProxyModel *proxyModel;
};

#endif // SEARCHDIALOG_H