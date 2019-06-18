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

#include <QtWidgets>

class gnomonViewForm;

class GNOMONVISUALIZATION_EXPORT gnomonGridLayout : public QGridLayout
{
    Q_OBJECT

public:
     gnomonGridLayout(QWidget *parent = nullptr);
    ~gnomonGridLayout(void);

public slots:
    void addView(void);
	void removeAllViews(void);

public:
    QList<gnomonViewForm *> views(void);

protected:
    void clear(void);
    void update(void);

signals:
    void formAdded(const QString&);

private:
    class gnomonGridLayoutPrivate *d;
};

//
// gnomonGridLayout.h ends here
