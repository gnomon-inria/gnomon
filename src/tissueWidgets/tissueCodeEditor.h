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

class tissueCodeEditorLineNumberArea;
class tissueCodeEditorSyntaxHighlighter;
class tissueCodeEditorPrivate;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class tissueCodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
     tissueCodeEditor(QWidget *parent = 0);
    ~tissueCodeEditor(void);

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
    tissueCodeEditorPrivate* d;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class tissueCodeEditorLineNumberArea : public QWidget
{
public:
    tissueCodeEditorLineNumberArea(tissueCodeEditor *editor) : QWidget(editor) {
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
    tissueCodeEditor *m_codeEditor;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class tissueCodeEditorSyntaxHighlighterPrivate;

class tissueCodeEditorSyntaxHighlighter : public QSyntaxHighlighter
{
public:
     tissueCodeEditorSyntaxHighlighter(QTextDocument *document);
    ~tissueCodeEditorSyntaxHighlighter();

public:
    void highlightBlock(const QString& text);

protected:
    tissueCodeEditorSyntaxHighlighterPrivate *d;
};

//
// tissueCoreEditor.h ends here
