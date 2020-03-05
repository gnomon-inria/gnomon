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

#include <dtkComposer/dtkComposerWriter.h>

#include <QtWidgets>

#include <gnomonComposerExport.h>

class dtkComposerSceneNode;
class dtkComposerWidget;

class gnomonComposerWidgetPrivate;

class GNOMONCOMPOSER_EXPORT gnomonComposerWidget : public QFrame
{
    Q_OBJECT

public:
    static gnomonComposerWidget *instance(void);

signals:
    void shrink(void);
    void expand(void);

public:
    QSize sizeHint(void) const;

public:
    dtkComposerWidget* composerWidget(void);

public slots:
    void addWorkspace(const QString&);
    void addNode(dtkComposerSceneNodeComposite *node);

public slots:
    bool compositionSave(void);
    bool compositionSaveAs(void);
    bool compositionSaveAs(const QString& file, dtkComposerWriter::Type type = dtkComposerWriter::Ascii);
    bool compositionInsert(void);
    bool compositionInsert(const QString& file);

protected:
     gnomonComposerWidget(QWidget *parent = 0);
    ~gnomonComposerWidget(void);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

protected:
    void mousePressEvent(QMouseEvent *);

protected:
    void paintEvent(QPaintEvent *);

protected:
    void closeEvent(QCloseEvent *event);

private:
    gnomonComposerWidgetPrivate *d;

private:
    static gnomonComposerWidget *s_instance;
};

//
// gnomonComposerWidget.h ends here
