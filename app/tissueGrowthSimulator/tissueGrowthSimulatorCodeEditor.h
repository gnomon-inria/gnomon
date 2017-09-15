
#pragma once

#include <QtWidgets>


class tissueGrowthSimulatorCodeEditorLineNumberArea;
class tissueGrowthSimulatorCodeEditorSyntaxHighlighter;
class tissueGrowthSimulatorCodeEditorPrivate;

class tissueGrowthSimulatorCodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    tissueGrowthSimulatorCodeEditor(QWidget *parent = 0);
    ~tissueGrowthSimulatorCodeEditor();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    //void updateVocabulary();

private:
    tissueGrowthSimulatorCodeEditorPrivate* d;
};


class tissueGrowthSimulatorCodeEditorLineNumberArea : public QWidget
{
public:
    tissueGrowthSimulatorCodeEditorLineNumberArea(tissueGrowthSimulatorCodeEditor *editor) : QWidget(editor) {
        m_codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(m_codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        m_codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    tissueGrowthSimulatorCodeEditor *m_codeEditor;
};

class tissueGrowthSimulatorCodeEditorSyntaxHighlighterPrivate;

class tissueGrowthSimulatorCodeEditorSyntaxHighlighter : QSyntaxHighlighter
{
public:

    tissueGrowthSimulatorCodeEditorSyntaxHighlighter(QTextDocument* document);

    ~tissueGrowthSimulatorCodeEditorSyntaxHighlighter();

    void highlightBlock(const QString& text);

protected:

    tissueGrowthSimulatorCodeEditorSyntaxHighlighterPrivate *d;
};

