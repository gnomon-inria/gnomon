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

#include <gnomonWidgetsExport>

#include <QtWidgets>

class dtkWidgetsWorkspace;
class QStackedWidget;

class GNOMONWIDGETS_EXPORT gnomonWidgetsWorkspaceBar: public QTabBar
{
    Q_OBJECT

public:
             gnomonWidgetsWorkspaceBar(QWidget *parent = nullptr);
    virtual ~gnomonWidgetsWorkspaceBar();

public:
    void setStack(QStackedWidget *stack);

signals:
    void indexChanged(int);
    void indexDeleted(int);

signals:
    void created(const QString&);

public slots:
    void addWorkspaceInMenu(const QString& group, const QString& group_description, const QString& type, const QString& type_description, const QString& workspace_name = QString(), const QColor& color = QColor());

public:
    void buildFromFactory(void);

public slots:
    void createWorkspace(const QString& type);
    void createWorkspace(const QString &name, const QString& type, bool display_destroy = true);

protected:
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

public:
    void setDynamic(bool);

public Q_SLOTS:
    void setCurrentIndex(int index);

public:
    QSize sizeHint(void) const override;

protected:
    QSize tabSizeHint(int index) const override;

private:
    class gnomonWidgetsWorkspaceBarPrivate *d;
};

//
// gnomonWidgetsWorkspaceBar.h ends here
