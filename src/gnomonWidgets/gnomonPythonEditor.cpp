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

#include "gnomonPythonEditor.h"
#include "gnomonPythonEditor_p.h"

#include <dtkCore>
#include <dtkFonts>
#include <dtkMacs>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

#include "gnomonHighlighterPython.h"

// ///////////////////////////////////////////////////////////////////
// gnomonPythonEditor
// ///////////////////////////////////////////////////////////////////

gnomonPythonEditor::gnomonPythonEditor(QWidget *parent) : QWidget(parent)
{
    d = new gnomonPythonEditorPrivate;

    d->editor = new dtkMacsWidget(this);
    d->editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    d->highlighter = new gnomonHighlighterPython(this);
    d->highlighter->setDocument(d->editor->document());
    
    QFont font = d->editor->font();
    font.setFamily("Courier");
    font.setPointSize(13);
    d->editor->setCurrentFont(font);

    // -- Organizing the editor column --
    d->layout = new QHBoxLayout;
    d->layout->setContentsMargins(40, 0, 0, 0);
    d->layout->setSpacing(0);
    d->layout->addWidget(d->editor);

    d->script_menubar = new dtkWidgetsMenuBar(this);
    d->script_menubar->setInteractive(false);
    d->script_menubar->setStandalone(true);
    d->script_menubar->setWidth(32);
    d->script_menubar->setMargins(6);

    d->script_menubar->addMenu(fa::folder, "Open Python Script");
    d->script_menubar->addMenu(fa::save, "Save Python Script");
    d->script_menubar->addMenu(fa::play, "Load Python Script");

    connect(d->script_menubar, &dtkWidgetsMenuBar::clicked, [=] (int i_style)
    {
        if (i_style==0) {
            emit openButtonClicked();
        } else if (i_style==1) {
            emit saveButtonClicked();
        } else if (i_style==2) {
            emit loadButtonClicked();
        }

    });
//    d->script_menubar->move(QPoint(0, 0));
    d->script_menubar->touch();

    this->setLayout(d->layout);
}

gnomonPythonEditor::~gnomonPythonEditor(void)
{
    delete d->editor;
    delete d->highlighter;
}


QString gnomonPythonEditor::text(void)
{
    return d->editor->toPlainText();
}

void gnomonPythonEditor::setText(const QString& text)
{
    d->editor->setText(text);
}

void gnomonPythonEditor::resizeEvent(QResizeEvent *event)
{
    if (d->script_menubar) {
        d->script_menubar->setFixedHeight(event->size().height());
        d->script_menubar->touch();
    }

    QWidget::resizeEvent(event);
}

QSize gnomonPythonEditor::sizeHint(void) const
{
    return QSize(1000, 800);
}


//
// gnomonPythonEditor.cpp ends here
