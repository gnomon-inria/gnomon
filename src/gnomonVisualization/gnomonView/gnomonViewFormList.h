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
    Q_PROPERTY(QList<gnomonViewForm *> views READ views);

public slots:
    void addView(void);
	void removeAllViews(void);

public:
    QList<gnomonViewForm *> views(void);

/* protected:
    void clear(void);
    void update(void); */

signals:
    void formAdded(const QString&);
    void viewAdded(gnomonViewForm *);

private:
    class gnomonViewFormListPrivate *d;
};

//
// gnomonViewFormList.h ends here
