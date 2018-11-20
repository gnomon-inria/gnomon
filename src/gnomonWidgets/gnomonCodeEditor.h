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

#include <QtWidgets>

class gnomonCodeEditorLineNumberArea;
class gnomonCodeEditorSyntaxHighlighter;
class gnomonCodeEditorPrivate;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonCodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
     gnomonCodeEditor(QWidget *parent = 0);
    ~gnomonCodeEditor(void);

public slots:
    void openScript(void);
    // void saveScript(void);
    // void saveScriptAs(void);

public:
    void lineNumberAreaPaintEvent(QPaintEvent *event);

public:
    int lineNumberAreaWidth(void);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;

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
    gnomonCodeEditorPrivate* d;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonCodeEditorLineNumberArea : public QWidget
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

class gnomonCodeEditorSyntaxHighlighterPrivate;

class gnomonCodeEditorSyntaxHighlighter : public QSyntaxHighlighter
{
public:
     gnomonCodeEditorSyntaxHighlighter(QTextDocument *document);
    ~gnomonCodeEditorSyntaxHighlighter();

public:
    void highlightBlock(const QString& text);

protected:
    gnomonCodeEditorSyntaxHighlighterPrivate *d;
};

//
// gnomonCoreEditor.h ends here
