// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>

class gnomonViewForm;

class GNOMONVISUALIZATION_EXPORT gnomonViewFormList : public QObject
{
    Q_OBJECT

public:
     gnomonViewFormList(QObject *parent = nullptr);
    ~gnomonViewFormList(void);

public:
    Q_PROPERTY(QList<QObject *> views READ viewsAsQObject);

public slots:
    void addView(const QVector<QString> &accepted_forms = {});
	void removeAllViews(void);

public:
    gnomonViewForm *operator[](int);
    QList<gnomonViewForm *> views(void);
    QList<QObject *> viewsAsQObject(void);

/* protected:
    void clear(void);
    void update(void); */

signals:
    void formAdded(const QString&);
    void formsChanged(void);
    void viewAdded(gnomonViewForm *);

private:
    class gnomonViewFormListPrivate *d;
};

//
// gnomonViewFormList.h ends here
