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
    gnomonWorkspaceDatabaseBrowser *q = nullptr;

public:
    QTableView *view_table = nullptr;
    QStandardItemModel *model_table = nullptr;

public:
    const QString getDocument(const QString &key);
    void itemclicked(const QModelIndex &index);
    void listDbDocuments(void);
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
        items.append(new QStandardItem(doc_json["gnomonVersion"].toString()));
        items.append(new QStandardItem(doc_json["name"].toString()));

        items.append(new QStandardItem(dtkFontAwesome::instance()->icon("download"),""));
        items.append(new QStandardItem(dtkFontAwesome::instance()->icon("trash"),""));

        if(doc_json["expiration_date"].toString().isEmpty()) {
            items.append(new QStandardItem(dtkFontAwesome::instance()->icon("unlock"),""));
        } else {
            auto *protected_item = new QStandardItem(dtkFontAwesome::instance()->icon("lock"),"");
            protected_item->setToolTip("Protected document");
            items.append(protected_item);

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
    int row = index.row();
    int column = index.column();

    QString query = "{";
    query += "\"_id\": \"" + this->model_table->item(row, 0)->text() + "\", ";
    query += "\"type\": \"" + this->model_table->item(row, 1)->text() + "\", ";
    query += "\"date\": \"" + this->model_table->item(row, 2)->text() + "\", ";
    query += "\"user\": \"" + this->model_table->item(row, 3)->text() + "\" ";
    query += '}';

    QString column_text = this->model_table->horizontalHeaderItem(column)->text();
    if (column_text == "get_data") {
        QJsonDocument doc = QJsonDocument::fromJson(this->getDocument(query).toUtf8());

        gnomonCoreSettings settings;
        QString path = settings.value("dbbrowser/last_open_dir", QDir::homePath()).toString();
        QString url = QFileDialog::getSaveFileName(q,
                                              "Save document",
                                               path,
                                               "JSON (*.json)");
        if(!url.endsWith(".json")) {
            url += ".json";
        }

        QFile file(url);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            dtkWarn() << "cannot save file " << url;
            return;
        }

        file.write(doc.toJson());
        file.close();
    }

    if (column_text == "remove") {
        if(this->dataDriver->delete_one(query)) {
            this->model_table->removeRow(row);
        }
    }

    if (column_text == "protected") {
        QString tooltip = this->model_table->item(row, column)->toolTip();

        if( (tooltip != "Protected document") && this->dataDriver->protect(query)) {
            auto *new_item = new QStandardItem(dtkFontAwesome::instance()->icon("lock"), "");
            new_item->setEditable(false);
            new_item->setToolTip("Protected document");
            this->model_table->setItem(row, column, new_item);

            QString exp_date = QJsonDocument::fromJson(this->getDocument(query).toUtf8()).object()["expiration_date"].toString();
            auto *exp_date_item = new QStandardItem(exp_date);
            exp_date_item->setEditable(false);
            this->model_table->setItem(row, column + 1, exp_date_item);
        }
    }
}

gnomonWorkspaceDatabaseBrowser::gnomonWorkspaceDatabaseBrowser(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    d = new gnomonWorkspaceDatabaseBrowserPrivate;

    d->dataDriver = gnomonDataDriver::instance();
    d->pipeline = gnomonPipeline::instance();
    d->q = this;

    QStringList columns_labels = {"id", "type", "date", "user", "gnomon_version", "name", "get_data", "remove", "protected", "expiration_date"};
    d->model_table = new QStandardItemModel();
    d->model_table->setHorizontalHeaderLabels(columns_labels);
    d->view_table = new QTableView();
    d->view_table->setModel(d->model_table);
    d->view_table->resizeColumnsToContents();

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
