#include "gnomonWorkspaceDatabaseBrowser.h"

#include <gnomonCore>
#include <gnomonWidgets>
#include <gnomonVisualization>
#include <gnomonComposer>

#include <dtkThemes>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

#include <QtGui>
#include <QtWidgets>

class gnomonWorkspaceDatabaseBrowserPrivate {

public:
    gnomonDataDriver *dataDriver = nullptr;
    gnomonPipeline *pipeline = nullptr;

public:
    QTableView *view_table = nullptr;
    QStandardItemModel *model_table = nullptr;

public:
    const QString getDocument(const QString &key);
    void itemclicked(const QModelIndex &index);
    void listDbDocuments(void);
    void removeDocument(const QString& key);
};

const QString gnomonWorkspaceDatabaseBrowserPrivate::getDocument(const QString& key)
{
     return this->dataDriver->find(key).join( " ");
}

void gnomonWorkspaceDatabaseBrowserPrivate::listDbDocuments(void)
{
    QStringList docList = this->dataDriver->find("{}"); // TODO !!
    if(docList.isEmpty()) {
        qDebug() << " cannot find any documents :(";
    }

    for(auto&& doc : docList) {
        QList<QStandardItem *> items;
        QJsonObject doc_json = QJsonDocument::fromJson(doc.toUtf8()).object();
        qDebug() << "json: " << doc;

        items.append(new QStandardItem(doc_json["_id"].toString()));
        items.append(new QStandardItem(doc_json["type"].toString()));
        items.append(new QStandardItem(doc_json["date"].toString()));
        items.append(new QStandardItem(doc_json["user"].toString()));
        items.append(new QStandardItem(doc_json["version"].toString()));
        items.append(new QStandardItem(doc_json["name"].toString()));

        items.append(new QStandardItem(dtkFontAwesome::instance()->icon("download"),""));
        items.append(new QStandardItem(dtkFontAwesome::instance()->icon("trash"),""));

        if(doc_json["expiration_date"].toString().isEmpty()) {
            items.append(new QStandardItem(dtkFontAwesome::instance()->icon("unlock"),""));
        } else {
            items.append(new QStandardItem(dtkFontAwesome::instance()->icon("lock"),""));
            items.append(new QStandardItem(doc_json["expiration_date"].toString()));
        }

        for(auto *it: items) {
            it->setEditable(false);
        }

        this->model_table->appendRow(items);
    }
    this->view_table->resizeColumnsToContents();
}

void gnomonWorkspaceDatabaseBrowserPrivate::itemclicked(const QModelIndex &index)
{
    QString query = "{";
    query += "\"_id\": \"" + this->model_table->item(index.row(), 0)->text() + "\", ";
    query += "\"type\": \"" + this->model_table->item(index.row(), 1)->text() + "\", ";
    query += "\"date\": \"" + this->model_table->item(index.row(), 2)->text() + "\", ";
    query += "\"user\": \"" + this->model_table->item(index.row(), 3)->text() + "\" ";
    query += '}';

    QString column_text = this->model_table->horizontalHeaderItem(index.column())->text();
    if (column_text == "get_data") {
        qDebug() << "TODO DOWNLOAD";
    }

    if (column_text == "remove") {
        if(this->dataDriver->delete_one(query)) {
            this->model_table->removeRow(index.row());
        }
    }

    if (column_text == "protected") {
        if(this->dataDriver->protect(query)) {
            this->model_table->setItem(index.row(), index.column(), new QStandardItem(dtkFontAwesome::instance()->icon("lock"), ""));
        }
    }
}

void gnomonWorkspaceDatabaseBrowserPrivate::removeDocument(const QString& key)
{

}

gnomonWorkspaceDatabaseBrowser::gnomonWorkspaceDatabaseBrowser(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    d = new gnomonWorkspaceDatabaseBrowserPrivate;

    d->dataDriver = gnomonDataDriver::instance();
    d->pipeline = gnomonPipeline::instance();

    QStringList columns_labels = {"id", "type", "date", "user", "gnomon_version", "name", "get_data", "remove", "protected", "expiration_date"};
    d->model_table = new QStandardItemModel();
    d->model_table->setHorizontalHeaderLabels(columns_labels);
    d->view_table = new QTableView();
    d->view_table->setModel(d->model_table);
    d->view_table->resizeColumnsToContents();
    d->view_table->setStyleSheet("border:1px solid green");

    connect(d->view_table, &QTableView::clicked, [=](const QModelIndex &index) {
        d->itemclicked(index);
    });

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->view_table);
}

gnomonWorkspaceDatabaseBrowser::~gnomonWorkspaceDatabaseBrowser(void)
{
    delete d;
}

void gnomonWorkspaceDatabaseBrowser::enter(void)
{
    d->listDbDocuments();
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceDatabaseBrowser::leave(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceDatabaseBrowser::apply(void)
{

}

const QColor gnomonWorkspaceDatabaseBrowser::color = QColor("#ff3b30");

bool gnomonWorkspaceDatabaseBrowser::isEmpty(void)
{
    return false;
}
