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
     gnomonComposerWidget(QWidget *parent = 0);
    ~gnomonComposerWidget(void);

public:
    dtkComposerWidget* composerWidget(void);

public slots:
    bool compositionOpen(void);
    bool compositionOpen(const QString& file);
    bool compositionSave(void);
    bool compositionSaveAs(void);
    bool compositionSaveAs(const QString& file, dtkComposerWriter::Type type = dtkComposerWriter::Ascii);
    bool compositionInsert(void);
    bool compositionInsert(const QString& file);

protected slots:
    void showControls(void);
    void onComposerNodeFlagged(dtkComposerSceneNode *);

protected:
    void closeEvent(QCloseEvent *event);

private:
    gnomonComposerWidgetPrivate *d;
};

//
// gnomonComposerWidget.h ends here
