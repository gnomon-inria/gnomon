
#pragma once

#include <QtWidgets>


class tissueGrowthSimulatorCodeEditorLineNumberArea;
class tissueGrowthSimulatorCodeEditorSyntaxHighlighter;

class tissueGrowthSimulatorCodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    tissueGrowthSimulatorCodeEditor(QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    QWidget *m_lineNumberArea;
    tissueGrowthSimulatorCodeEditorSyntaxHighlighter* m_highlighter;
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

class tissueGrowthSimulatorCodeEditorSyntaxHighlighter : QSyntaxHighlighter
{
public:

    tissueGrowthSimulatorCodeEditorSyntaxHighlighter(QTextDocument* document);

    void highlightBlock(const QString& text);

protected:

    QRegularExpression m_commentExpr;
    QRegularExpression m_wordExpr;

    QStringList m_keywords;
    QStringList m_builtins;

    QTextCharFormat m_keywordFormat;
    QTextCharFormat m_builtinFormat;
    QTextCharFormat m_commentFormat;
};
