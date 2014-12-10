#ifndef torntPage_H
#define torntPage_H

#include <QDialog>
#include <QtSql>
#include <QThread>

namespace Ui {
    class torntPage;
}
class torntTableModel;
class ClientModel;

QT_BEGIN_NAMESPACE
class QTableView;
class QItemSelection;
class QSortFilterProxyModel;
class QMenu;
class QModelIndex;
QT_END_NAMESPACE

/** Widget that shows a list of sending or receiving addresses.
  */
class torntPage : public QDialog
{
    Q_OBJECT

public:

    explicit torntPage(QWidget *parent = 0);
    ~torntPage();

    void setModel(torntTableModel *model);
    void setClientModel(ClientModel *model);

    const QString &getReturnValue() const { return returnValue; }
    void startExecutor();

public slots:
    void refreshtorntTable();
private:
    Ui::torntPage *ui;
    torntTableModel *model;
    ClientModel *clientModel;
    QString returnValue;
    QSortFilterProxyModel *proxyModel;
    QMenu *contextMenu;
    QAction *deleteAction;
    QString newAddressToSelect;
    


signals:
    void doubleClicked(const QModelIndex&);
    void updateRequest();
    void stopExecutor();

private slots:
    void searchButtonClicked();
    void showDetails();
private:
    

};

#endif // torntPage_H
