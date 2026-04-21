#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_pushButton_Add_clicked();
    void clearInputFields();
    void on_InfoDelBtn_2_clicked();
    void on_SearchBtn_2_clicked();
    void on_pushButton_SaveFile_clicked();
    void on_pushButton_LoadFile_clicked();
    void on_InfoView_2_selectionChanged();
    void setLineEditProperty();
    void on_InfoView_2_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    void setSystemStyle();
    bool saveCurrentData();
    QStandardItemModel *mainModel; // 전체 고객 데이터를 담는 핵심 모델
};
#endif // MAINWINDOW_H
