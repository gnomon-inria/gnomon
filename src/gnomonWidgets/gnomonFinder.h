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

#include <gnomonWidgetsExport>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

// /////////////////////////////////////////////////////////////////
// gnomonFinderToolBar
// /////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonFinderToolBar : public QFrame
{
    Q_OBJECT

public:
     gnomonFinderToolBar(QWidget *parent = 0);
    ~gnomonFinderToolBar(void);

public:
     QSize sizeHint(void) const;

signals:
    void changed(const QString& path);
    void listView(void);
    void treeView(void);
    void showHiddenFiles(bool);

public slots:
    void setPath(const QString& path);
    void onPrev(void);
    void onNext(void);
    void onListView(void);
    void onTreeView(void);
    void onShowHiddenFiles(bool);

private:
    class gnomonFinderToolBarPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// gnomonFinderSideView
// /////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonFinderSideView : public QTreeWidget
{
    Q_OBJECT
    Q_PROPERTY(int headerFontSize READ headerFontSize WRITE setHeaderFontSize)

public:
     gnomonFinderSideView(QWidget *parent = 0);
    ~gnomonFinderSideView(void);

    void populate(void);

    int headerFontSize(void) const;

    QSize sizeHint (void) const;

signals:
    void changed(const QString& path);

public slots:
    void setPath(const QString& path);
    void setHeaderFontSize(int value);

public slots:
    void    addBookmark(const QString& path);
    void removeBookmark(const QString& path);
    void  clearBookmarks(void);

private slots:
    void onItemCicked(QTreeWidgetItem *, int);
    void onContextMenu(const QPoint&);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

    QString driveLabel(QString drive);

private:
    class gnomonFinderSideViewPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// gnomonFinderPathBar
// /////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonFinderPathBar : public QFrame
{
    Q_OBJECT

public:
     gnomonFinderPathBar(QWidget *parent = 0);
    ~gnomonFinderPathBar(void);

     QSize sizeHint(void) const;

signals:
    void changed(const QString& path);

public slots:
    void setPath(const QString &path);

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    class gnomonFinderPathBarPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// gnomonFinderListView
// /////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonFinderListView : public QListView
{
    Q_OBJECT

public:
     gnomonFinderListView(QWidget *parent = 0);
    ~gnomonFinderListView(void);

    void addContextMenuAction(QAction *action);

    void addDefaultContextMenuAction(QAction *action);

    QString selectedPath(void) const;

    QStringList selectedPaths(void) const;

    void allowFileBookmarking(bool isAllowed);

signals:
    void changed(const QString& path);
    void bookmarked(const QString& path);

public slots:
    void onBookmarkSelectedItemsRequested(void);

protected slots:
    void updateContextMenu(const QPoint&);

protected:
     void keyPressEvent(QKeyEvent *event);
     void mouseDoubleClickEvent(QMouseEvent *event);
     void startDrag(Qt::DropActions supportedActions);

private:
    class  gnomonFinderListViewPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// gnomonFinderTreeView
// /////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonFinderTreeView : public QTreeView
{
    Q_OBJECT

public:
     gnomonFinderTreeView(QWidget *parent = 0);
    ~gnomonFinderTreeView(void);

    int sizeHintForColumn(int column) const;

    void addContextMenuAction(QAction *action);

    void addDefaultContextMenuAction(QAction *action);

    QString selectedPath(void) const;

    QStringList selectedPaths(void) const;

    void allowFileBookmarking(bool isAllowed);

signals:
    void changed(const QString& path);
    void bookmarked(const QString& path);

public slots:
    void onBookmarkSelectedItemsRequested(void);

protected slots:
    void updateContextMenu(const QPoint&);

protected:
     void keyPressEvent(QKeyEvent *event);
     void mouseDoubleClickEvent(QMouseEvent *event);
     void startDrag(Qt::DropActions supportedActions);
     void resizeEvent(QResizeEvent *event);

private:
     class gnomonFinderTreeViewPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// gnomonFinder
// /////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonFinder : public QFrame
{
    Q_OBJECT

public:
     gnomonFinder(QWidget *parent = 0);
    ~gnomonFinder(void);

    void addContextMenuAction(QAction *action);

    QString selectedPath(void) const;
    QStringList selectedPaths(void) const;

    void allowFileBookmarking(bool isAllowed);
    void allowMultipleSelection(bool isAllowed);

signals:
    void changed(const QString& path);
    void bookmarked(const QString& path);
    void fileDoubleClicked(const QString &filename);
    void fileClicked(const QFileInfo &info);
    void selectionChanged(const QStringList& paths);
    void nothingSelected(void);
    void listView(void);
    void treeView(void);
    void showHiddenFiles(bool);

public slots:
    void setPath(const QString& path);
    void switchToListView(void);
    void switchToTreeView(void);
    void onShowHiddenFiles(bool);
    void switchShowHiddenFiles(void);

public slots:
    void onBookmarkSelectedItemsRequested(void);

protected slots:
    void onIndexDoubleClicked(QModelIndex);
    void onIndexClicked(QModelIndex);
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void emitSelectedItems();

private:
    class gnomonFinderPrivate *d;
};

//
// gnomonFinder.h ends here
