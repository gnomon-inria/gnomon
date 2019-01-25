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

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonCodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
     gnomonCodeEditor(QWidget *parent = 0);
    ~gnomonCodeEditor(void);

public slots:
    void openScript(void);
    void saveScript(void);

signals:
    void scriptLoaded(void);
    void scriptOpened(void);
    void scriptSaved(void);

public:
    void lineNumberAreaPaintEvent(QPaintEvent *event);

public:
    int lineNumberAreaWidth(void);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;

 protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

protected:
    QString textUnderCursor(void);

public slots:
    void enableAutocompletion(bool enabled);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine(void);
    void updateLineNumberArea(const QRect &, int);
    void updateVocabulary(void);
    void insertCompletion(QString);

private:
    class gnomonCodeEditorPrivate *d;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonCodeEditorLineNumberArea : public QWidget
{
public:
    gnomonCodeEditorLineNumberArea(gnomonCodeEditor *editor) : QWidget(editor) {
        m_codeEditor = editor;
    }

    QSize sizeHint(void) const override {
        return QSize(m_codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        m_codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    gnomonCodeEditor *m_codeEditor;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonCodeEditorSyntaxHighlighter : public QSyntaxHighlighter
{
public:
     gnomonCodeEditorSyntaxHighlighter(QTextDocument *document);
    ~gnomonCodeEditorSyntaxHighlighter(void);

public:
    void highlightBlock(const QString& text);

protected:
    class gnomonCodeEditorSyntaxHighlighterPrivate *d;
};

//
// gnomonCoreEditor.h ends here
