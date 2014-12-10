#include "tornttablemodel.h"
#include "guiutil.h"
#include "bitcoinrpc.h"
#include "base58.h"
#include "walletmodel.h"
#include "util.h"


#include <QFont>
#include <QColor>




#include "ui_interface.h"
#include "json/json_spirit_reader_template.h"
#include "json/json_spirit_writer_template.h"
#include "json/json_spirit_utils.h"

#include <boost/algorithm/string.hpp>


#include <stdlib.h>


using namespace boost;
using namespace json_spirit;


struct torntTableEntry
{

    QString title;
    QString link;
    QString address;
    QString amount;


    torntTableEntry() {}
    torntTableEntry(const QString &title,const QString &link,const QString &address,const QString &amount):
        title(title),link(link),address(address),amount(amount) {}
};



class torntTablePriv
{
public:

    QList<torntTableEntry> cachedtorntTable;
    torntTableModel *parent;

    torntTablePriv(torntTableModel *parent):
         parent(parent) {}


    void getLasttornt()
    {
        try{
            QSqlQuery query;
            query.exec(QString("select txid from tornt  order by blockindex desc limit 1000"));
            while (query.next())
            {
                std::vector<std::string> args;
                args.push_back("gettornt");
                args.push_back(query.value(0).toString().toStdString());
                Value value = tableRPC.execute(args[0],RPCConvertValues(args[0], std::vector<std::string>(args.begin() + 1, args.end())));
                if (value.type() == obj_type)
                {
                    Object reply = value.get_obj();

                    std::string  title  = find_value(reply, "title").get_str();
                    std::string  link  = find_value(reply, "link").get_str();
                    std::string  address  = find_value(reply, "address").get_str();
                    std::string  amount  = find_value(reply, "amount").get_str();

                    cachedtorntTable.append(torntTableEntry(
                                      QString::fromStdString(title),
                                     QString::fromStdString(link),
                                     QString::fromStdString(address),
                                     QString::fromStdString(amount)));
                }

            }
        }catch (json_spirit::Object& objError)
        {
            
        }
        catch(std::runtime_error &) 
        {  
              
        }
        catch (std::exception& e)
        {
            
        }
    }

    





    void refreshTable()
    {
        cachedtorntTable.clear();
        getLasttornt();
    }



    int size()
    {
        return cachedtorntTable.size();
    }

    torntTableEntry *index(int idx)
    {
        if(idx >= 0 && idx < cachedtorntTable.size())
        {
            return &cachedtorntTable[idx];
        }
        else
        {
            return 0;
        }
    }


    QString describe(torntTableEntry *rec)
    {

        QString strHTML;

        strHTML.reserve(4000);
        strHTML += "<html><font face='verdana, arial, helvetica, sans-serif'>";
        strHTML += "<b>Title:</b> " + rec->title + "<br>";
        strHTML += "<b>Send Address:</b> " + rec->address + "<br>";
        strHTML += "<b>Amount:</b> " + rec->amount + "<br>";
        strHTML += "<b>Link:</b> " + rec->link + "<br>";
        return strHTML;


    }
};

torntTableModel::torntTableModel(WalletModel *parent) :
    QAbstractTableModel(parent),priv(0)
{
    columns << tr("Title") ;
    columns << tr("Address") ;
    columns << tr("Amount") ;
    priv = new torntTablePriv(this);
}

torntTableModel::~torntTableModel()
{
    delete priv;
}

int torntTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return priv->size();
}

int torntTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return columns.length();
}

void torntTableModel::refreshtorntTable()
{
    priv->refreshTable();
    emit layoutChanged();
}

QVariant torntTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    torntTableEntry *rec = static_cast<torntTableEntry*>(index.internalPointer());

    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case Title:
            return rec->title;
        case Address:
            return rec->address;
        case Amount:
            return rec->amount;
        }
    }
    else if(role == torntRole)
    {
        return priv->describe(rec);
    }
        
    return QVariant();
}


QVariant torntTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        if(role == Qt::DisplayRole)
        {
            return columns[section];
        }
    }
    return QVariant();
}



QModelIndex torntTableModel::index(int row, int column, const QModelIndex & parent) const
{
   Q_UNUSED(parent);
    torntTableEntry *data = priv->index(row);
    if(data)
    {
        return createIndex(row, column, priv->index(row));
    }
    else
    {
        return QModelIndex();
    }
}