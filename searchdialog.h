#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QStandardItemModel *model, QWidget *parent = nullptr);
    ~SearchDialog();

private slots:
    void on_pushButton_clicked(); // UI의 검색 버튼

private:
    Ui::SearchDialog *ui;
    QSortFilterProxyModel *proxyModel;
};

#endif // SEARCHDIALOG_H
