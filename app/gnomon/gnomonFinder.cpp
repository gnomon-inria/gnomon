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

#include "gnomonFinder.h"

#include <gnomonFonts>
#include <gnomonStyle>

#ifdef Q_WS_WIN
#include <qt_windows.h>
#endif

#include <QListIterator>

// /////////////////////////////////////////////////////////////////
// gnomonFinderToolBar
// /////////////////////////////////////////////////////////////////

class gnomonFinderToolBarPrivate
{
public:
    QToolButton *prevButton;
    QToolButton *nextButton;
    QToolButton *listViewButton;
    QToolButton *treeViewButton;
    QToolButton *showHiddenFilesButton;

    QLinkedList<QString> pathList;
    QLinkedList<QString>::iterator iterator;

    gnomonFontAwesome *font_awesome;
};

gnomonFinderToolBar::gnomonFinderToolBar(QWidget *parent) : QFrame(parent), d(new gnomonFinderToolBarPrivate)
{
    d->font_awesome = new gnomonFontAwesome(this);
    d->font_awesome->initFontAwesome();
    d->font_awesome->setDefaultOption("color", QColor(GNOMON_STYLE_ACCENTCOLOR));

    d->prevButton = new QToolButton(this);
    d->prevButton->setArrowType(Qt::LeftArrow);
    d->prevButton->setEnabled(0);
    d->prevButton->setIconSize(QSize(16, 16));
    d->prevButton->setToolTip(tr("Back"));

    d->nextButton = new QToolButton(this);
    d->nextButton->setArrowType(Qt::RightArrow);
    d->nextButton->setEnabled(0);
    d->nextButton->setIconSize(QSize(16, 16));
    d->nextButton->setToolTip(tr("Next"));

    d->listViewButton = new QToolButton(this);
    d->listViewButton->setCheckable(true);
    d->listViewButton->setChecked(true);
    d->listViewButton->setIcon(d->font_awesome->icon(fa::th));
    d->listViewButton->setIconSize(QSize(16, 16));
    d->listViewButton->setToolTip(tr("Icon view"));

    d->treeViewButton = new QToolButton(this);
    d->treeViewButton->setCheckable(true);
    d->treeViewButton->setIcon(d->font_awesome->icon(fa::list));
    d->treeViewButton->setIconSize(QSize(16, 16));
    d->treeViewButton->setToolTip(tr("List view"));

    d->showHiddenFilesButton = new QToolButton(this);
    d->showHiddenFilesButton->setCheckable(true);
    d->showHiddenFilesButton->setIcon(d->font_awesome->icon(fa::eye));
    d->showHiddenFilesButton->setIconSize(QSize(16, 16));
    d->showHiddenFilesButton->setToolTip(tr("Show/Hide hidden files"));
#ifdef Q_WS_MAC
    d->showHiddenFilesButton->setChecked(Qt::Checked);
#endif

    QButtonGroup *viewButtonGroup = new QButtonGroup(this);
    viewButtonGroup->setExclusive(true);
    viewButtonGroup->addButton(d->listViewButton);
    viewButtonGroup->addButton(d->treeViewButton);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 0, 10, 0);
    layout->addWidget(d->prevButton);
    layout->addWidget(d->nextButton);
    layout->addWidget(d->treeViewButton);
    layout->addWidget(d->listViewButton);
    layout->addWidget(d->showHiddenFilesButton);

    connect(d->prevButton, SIGNAL(clicked()), this, SLOT(onPrev()));
    connect(d->nextButton, SIGNAL(clicked()), this, SLOT(onNext()));

    connect(d->listViewButton, SIGNAL(clicked()), this, SIGNAL(listView()));
    connect(d->treeViewButton, SIGNAL(clicked()), this, SIGNAL(treeView()));

    connect(d->showHiddenFilesButton, SIGNAL(toggled(bool)), this, SIGNAL(showHiddenFiles(bool)));

    this->setFixedHeight(36);
}

gnomonFinderToolBar::~gnomonFinderToolBar(void)
{
    delete d;

    d = NULL;
}

QSize gnomonFinderToolBar::sizeHint(void) const
{
    return QSize(100, 48);
}

void gnomonFinderToolBar::setPath(const QString &path)
{
    if(d->pathList.count()) {
        if(d->iterator != d->pathList.end())
            d->pathList.erase(d->pathList.begin(), d->iterator);
    }

    d->pathList.prepend(path);

    d->iterator = d->pathList.begin();

    if(d->pathList.count()>1)
        d->prevButton->setEnabled(1);
    else
        d->prevButton->setEnabled(0);

    d->nextButton->setEnabled(0);
}

void gnomonFinderToolBar::onNext(void)
{
    if(d->iterator != d->pathList.begin()) {
        emit(changed(*(--d->iterator)));
        d->prevButton->setEnabled(1);
        if (d->iterator == d->pathList.begin())
            d->nextButton->setEnabled(0);
    } else {
        d->nextButton->setEnabled(0);
    }
}

void gnomonFinderToolBar::onPrev(void)
{
    if(d->iterator!=--d->pathList.end()) {
        emit(changed(*(++d->iterator)));
        d->nextButton->setEnabled(1);
        if (d->iterator == (--d->pathList.end()))
            d->prevButton->setEnabled(0);
    } else {
        d->prevButton->setEnabled(0);
    }
}

void gnomonFinderToolBar::onTreeView(void)
{
    d->treeViewButton->setChecked(true);
    d->listViewButton->setChecked(false);
}

void gnomonFinderToolBar::onListView(void)
{
    d->treeViewButton->setChecked(false);
    d->listViewButton->setChecked(true);
}

void gnomonFinderToolBar::onShowHiddenFiles(bool show)
{
    d->showHiddenFilesButton->setChecked(show);
}

// /////////////////////////////////////////////////////////////////
// gnomonFinderSideView
// /////////////////////////////////////////////////////////////////

class gnomonFinderSideViewPrivate
{
public:
    QList<QTreeWidgetItem *> items;
    int headerFontSize;
};

gnomonFinderSideView::gnomonFinderSideView(QWidget *parent) : QTreeWidget(parent), d(new gnomonFinderSideViewPrivate)
{
    d->headerFontSize = 12;

    this->header()->hide();
    this->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    this->setDragDropMode(QAbstractItemView::DropOnly);
    this->setIndentation(10);
    this->setFrameStyle(QFrame::NoFrame);
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
    this->setFocusPolicy(Qt::NoFocus);
    this->populate();

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenu(const QPoint&)));
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem *,int)), this, SLOT(onItemCicked(QTreeWidgetItem *, int)));
}

gnomonFinderSideView::~gnomonFinderSideView(void)
{
    delete d;

    d = NULL;
}

void gnomonFinderSideView::populate(void)
{
    this->clear(); d->items.clear();

    QFont groupFont;
    groupFont.setPointSize(d->headerFontSize);
    groupFont.setBold(true);
    groupFont.setCapitalization(QFont::AllUppercase);

    QBrush groupBrush;
    groupBrush.setColor(QColor("#758192"));

    // QFont itemFont;
    // itemFont.setPointSize(10);

    QFileIconProvider provider;

    QTreeWidgetItem *item1  = new QTreeWidgetItem(this, QStringList() << "Drives");
    item1->setFlags(Qt::ItemIsEnabled);
    item1->setData(0, Qt::FontRole, groupFont);
    item1->setData(0, Qt::ForegroundRole, groupBrush);

    QFileInfoList driveList;
#ifdef Q_WS_MAC
    QDir macDir("/Volumes");
    driveList = macDir.entryInfoList(QDir::AllEntries|QDir::NoDotAndDotDot);
#else
    driveList = QDir::drives();
#endif

    foreach(QFileInfo info, driveList) {

        QString dlabel = this->driveLabel( info.absoluteFilePath() );
        QTreeWidgetItem *item = new QTreeWidgetItem(item1, QStringList() <<(dlabel.isEmpty() ? "HD" : dlabel));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        // item->setData(0, Qt::FontRole, itemFont);
        item->setData(0, Qt::UserRole, info.absoluteFilePath());
        item->setIcon(0, provider.icon(QFileIconProvider::Drive));

        d->items << item;
    }

    QTreeWidgetItem *item2  = new QTreeWidgetItem(this, QStringList() << "Places");
    item2->setFlags(Qt::ItemIsEnabled);
    item2->setData(0, Qt::FontRole, groupFont);
    item2->setData(0, Qt::ForegroundRole, groupBrush);

    QTreeWidgetItem *item21 = new QTreeWidgetItem(item2, QStringList() << "Desktop");
    item21->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    // item21->setData(0, Qt::FontRole, itemFont);
    item21->setData(0, Qt::UserRole, QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first());
    item21->setIcon(0, provider.icon(QFileInfo(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first())));

    QTreeWidgetItem *item22 = new QTreeWidgetItem(item2, QStringList() << "Home");
    item22->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    // item22->setData(0, Qt::FontRole, itemFont);
    item22->setData(0, Qt::UserRole, QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first());
    item22->setIcon(0, provider.icon(QFileInfo(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first())));

    QTreeWidgetItem *item23 = new QTreeWidgetItem(item2, QStringList() << "Documents");
    item23->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    // item23->setData(0, Qt::FontRole, itemFont);
    item23->setData(0, Qt::UserRole, QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first());
    item23->setIcon(0, provider.icon(QFileInfo(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first())));

    d->items << item21 << item22 << item23;

    QTreeWidgetItem *item3  = new QTreeWidgetItem(this, QStringList() << "Bookmarks");
    item3->setFlags(Qt::ItemIsEnabled);
    item3->setData(0, Qt::FontRole, groupFont);
    item3->setData(0, Qt::ForegroundRole, groupBrush);

    QSettings settings; QStringList bookmarks = settings.value("gnomonFinderBookmarks").toStringList();

    foreach(QString path, bookmarks) {

        QFileInfo info(path);

        if(info.exists()) {
            QTreeWidgetItem *item = new QTreeWidgetItem(item3, QStringList() << info.baseName());
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            // item->setData(0, Qt::FontRole, itemFont);
            item->setData(0, Qt::UserRole, info.absoluteFilePath());
            item->setIcon(0, provider.icon(info));

            d->items << item;
        }
    }

    this->expandItem(item1);
    this->expandItem(item2);
    this->expandItem(item3);
}

void gnomonFinderSideView::setHeaderFontSize(int value)
{
    d->headerFontSize = value;
    this->populate();
}

int gnomonFinderSideView::headerFontSize(void) const
{
    return d->headerFontSize;
}

QSize gnomonFinderSideView::sizeHint(void) const
{
    return QSize(-1, -1);
}

void gnomonFinderSideView::setPath(const QString& path)
{
    foreach(QTreeWidgetItem *item, d->items)
        if(item->data(0, Qt::UserRole).toString() == path)
            item->setSelected(true);
        else
            item->setSelected(false);
}

void gnomonFinderSideView::addBookmark(const QString& path)
{
    QFileInfo info(path);

    if(!info.isDir())
        return;

    QSettings settings;
    QStringList bookmarks = settings.value("gnomonFinderBookmarks").toStringList();
    bookmarks.removeAll(path);
    bookmarks.append(path);
    settings.setValue("gnomonFinderBookmarks", bookmarks);

    this->populate();
}

void gnomonFinderSideView::removeBookmark(const QString& path)
{
    QFileInfo info(path);

    if(!info.isDir())
        return;

    QSettings settings;
    QStringList bookmarks = settings.value("gnomonFinderBookmarks").toStringList();
    bookmarks.removeAll(path);
    settings.setValue("gnomonFinderBookmarks", bookmarks);

    this->populate();
}

void gnomonFinderSideView::clearBookmarks(void)
{
    QSettings settings;
    settings.remove("gnomonFinderBookmarks");

    this->populate();
}

void gnomonFinderSideView::onItemCicked(QTreeWidgetItem *item, int)
{
    emit changed(item->data(0, Qt::UserRole).toString());
}

void gnomonFinderSideView::onContextMenu(const QPoint& position)
{
    QTreeWidgetItem *item = this->itemAt(position);

    if(!item)
        return;

    QSettings   settings;
    QStringList bookmarks = settings.value("gnomonFinderBookmarks").toStringList();
    QString     bookmark  = item->data(0, Qt::UserRole).toString();

    if(!bookmarks.contains(bookmark))
        return;

    QMenu menu; menu.addAction("Remove bookmark");

    if(menu.exec(this->mapToGlobal(position)))
        this->removeBookmark(bookmark);
}

void gnomonFinderSideView::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()) {

        QFileInfo info(event->mimeData()->urls().first().toLocalFile());

        if(!info.isDir()) {
            event->ignore();
            return;
        } else {
            event->accept();
            return;
        }
    }

    event->ignore();
}

void gnomonFinderSideView::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasUrls()) {

        QFileInfo info(event->mimeData()->urls().first().toLocalFile());

        if(!info.isDir()) {
            event->ignore();
            return;
        } else {
            event->accept();
            return;
        }
    }

    event->ignore();
}

void gnomonFinderSideView::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasUrls()) {
        QFileInfo info(event->mimeData()->urls().first().toLocalFile());
        this->addBookmark(info.absoluteFilePath());
        event->accept();
        return;
    }

    event->ignore();
}

QString gnomonFinderSideView::driveLabel(QString drive)
{
#ifdef Q_WS_WIN
    drive.replace("/", "\\");
    TCHAR  szVolumeName[256];
    TCHAR  szFileSystemName[256];
    DWORD dwSerialNumber = 0;
    DWORD dwMaxFileNameLength=256;
    DWORD dwFileSystemFlags=0;
    bool ret = GetVolumeInformation( drive.toAscii().constData(),
                                     szVolumeName, 256,
                                     &dwSerialNumber, &dwMaxFileNameLength,
                                     &dwFileSystemFlags, szFileSystemName, 256);
    if(!ret) {
        drive.remove("\\");
        QString decoratedDrive = "("+drive+")";
        return decoratedDrive;
    }

    QString vName = QString::fromAscii(szVolumeName) ;
    vName.trimmed();
    drive.remove("\\");
    vName += "("+drive+")";
    return vName;
#endif

#ifdef Q_WS_MAC
    return QFileInfo(drive).baseName();
#endif

    return drive;
}

// /////////////////////////////////////////////////////////////////
// gnomonFinderPathBarItem
// /////////////////////////////////////////////////////////////////

class gnomonFinderPathBarItem
{
public:
    QString text;
    QIcon icon;
    QRect rect;
    QDir dir;
};

// /////////////////////////////////////////////////////////////////
// gnomonFinderPathBar
// /////////////////////////////////////////////////////////////////

class gnomonFinderPathBarPrivate
{
public:
    QString path;

    QList<gnomonFinderPathBarItem *> items;
};

gnomonFinderPathBar::gnomonFinderPathBar(QWidget *parent) : QFrame(parent), d(new gnomonFinderPathBarPrivate)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // this->setFixedHeight(23);
    // this->setStyleSheet("font: 11px; border-top: 1px solid #d4d4d4;");
}

gnomonFinderPathBar::~gnomonFinderPathBar(void)
{
    foreach(gnomonFinderPathBarItem *item, d->items)
        delete item;

    delete d;

    d = NULL;
}

QSize gnomonFinderPathBar::sizeHint(void) const
{
    return QSize(-1, 23);
}

void gnomonFinderPathBar::setPath(const QString &path)
{
    d->path = path;
    foreach(gnomonFinderPathBarItem *item, d->items)
        delete item;
    d->items.clear();

    QFileIconProvider provider;

    QDir dir(d->path); do {

        gnomonFinderPathBarItem *item = new gnomonFinderPathBarItem;
        item->text = dir.dirName().isEmpty() ? dir.absolutePath() : dir.dirName();
        if(!dir.entryInfoList(QStringList() << ".").isEmpty())
            item->icon = provider.icon(dir.entryInfoList(QStringList() << ".").first());
        item->dir = dir;
        d->items.prepend(item);

    } while(dir.cdUp());

    this->update();
}

void gnomonFinderPathBar::mousePressEvent(QMouseEvent *event)
{
    foreach(gnomonFinderPathBarItem *item, d->items) {
        if(item->rect.contains(event->x(), event->y())) {
            // setPath will delete the items. Store the string so that it is still valid afterwards.
            QString path(item->dir.absolutePath());
            this->setPath(path);
            emit changed(path);
            return;
        }
    }
}

void gnomonFinderPathBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QRect rect = this->rect();

    int x = 10;
    int y =(rect.height()-16)/2;

    QPainter painter(this);

    foreach(gnomonFinderPathBarItem *item, d->items) {
        painter.drawPixmap(x, y, item->icon.pixmap(16, 16));
        painter.drawText(x + 16 + 4, y + 14, item->text);

        int delta = 16 + 4 + painter.fontMetrics().width(item->text) + 10;

        item->rect = QRect(x, 0, delta, rect.height());

        x += delta;
    }

    if(d->items.last()->rect.right() >= this->width()) {
        QRect prect(this->width()-30, 0, 30, rect.height());
        painter.fillRect(prect, this->palette().background());
        painter.drawText(prect.left(), y+14, "...");
    }
}

// /////////////////////////////////////////////////////////////////
// gnomonFinderListView
// /////////////////////////////////////////////////////////////////

class gnomonFinderListViewPrivate
{
public:
    QMenu *menu;
    QList<QAction *> defaultActions;
    QList<QAction *> customActions;
    QAction *bookmarkAction;
    bool allowFileBookmarking;
};

gnomonFinderListView::gnomonFinderListView(QWidget *parent) : QListView(parent), d(new gnomonFinderListViewPrivate)
{
    this->setViewMode(QListView::IconMode);
    this->setWordWrap(true);
    this->setWrapping(true);
    this->setIconSize(QSize(64, 64));
    this->setResizeMode(QListView::Adjust);
    this->setGridSize(QSize(128, 96));
    this->setFrameStyle(QFrame::NoFrame);
    this->setAttribute(Qt::WA_MacShowFocusRect, false);

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    d->menu = new QMenu(this);
    d->allowFileBookmarking = true;
    d->bookmarkAction = new QAction(tr("Bookmark"), this);
    d->bookmarkAction->setIconVisibleInMenu(true);
    // d->bookmarkAction->setIcon(QIcon(":dtkGui/pixmaps/star.svg"));
    connect(d->bookmarkAction, SIGNAL(triggered()), this, SLOT(onBookmarkSelectedItemsRequested()));
    d->menu->addAction(d->bookmarkAction);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(updateContextMenu(const QPoint&)));
}

gnomonFinderListView::~gnomonFinderListView(void)
{
    delete d;

    d = NULL;
}

void gnomonFinderListView::addContextMenuAction(QAction *action)
{
    d->customActions.append(action);
}

void gnomonFinderListView::addDefaultContextMenuAction(QAction *action)
{
    d->defaultActions.append(action);
}

/** Returns the currently selected path, or the first one if more than one item is selected. */

QString gnomonFinderListView::selectedPath() const
{
    if(!selectedIndexes().count())
        return QString();

    return this->selectedPaths()[0];
}

QStringList gnomonFinderListView::selectedPaths() const
{
    if(!selectedIndexes().count())
        return QStringList();

    if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model()))
    {
        QStringList selectedPaths = *(new QStringList());

        foreach(QModelIndex index, selectedIndexes())
            selectedPaths << model->filePath(index);

        return selectedPaths;
    }
    else
        return QStringList();
}

void gnomonFinderListView::allowFileBookmarking(bool isAllowed)
{
    d->allowFileBookmarking = isAllowed;
}

void gnomonFinderListView::updateContextMenu(const QPoint& point)
{
    QModelIndex index = this->indexAt(point);

    d->menu->clear();

    if(index.isValid()) {
        if(!d->allowFileBookmarking) {
            bool removed = false;
            QString selectedPath = this->selectedPath();
            if(!selectedPath.isEmpty())
            {
                QFileInfo fileInfo = QFileInfo(selectedPath);
                if(fileInfo.isFile())
                {
                    d->menu->removeAction(d->bookmarkAction);
                    removed = true;
                }
            }

            if(!removed)
            {
                if(d->menu->actions().size() > 0)
                    d->menu->insertAction(d->menu->actions()[0], d->bookmarkAction);
                else
                    d->menu->addAction(d->bookmarkAction);
            }
        }

        for(int i = 0; i < d->customActions.size(); i++) {
            d->menu->addAction(d->customActions.value(i));
        }

        d->menu->addSeparator();

    }

    for(int i = 0; i < d->defaultActions.size(); i++) {
        d->menu->addAction(d->defaultActions.value(i));
    }

    d->menu->exec(mapToGlobal(point));


}

void gnomonFinderListView::onBookmarkSelectedItemsRequested(void)
{
    if(!selectedIndexes().count())
        return;

    foreach(QString path, this->selectedPaths())
        emit bookmarked(path);
}

void gnomonFinderListView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Up &&(event->modifiers() & Qt::ControlModifier)) {
        if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {
            QDir dir = QDir(model->filePath(this->rootIndex()));
            dir.cdUp();
            this->setRootIndex(model->index(dir.absolutePath()));
            emit changed(dir.absolutePath());
            event->accept();
            return;
        }
    }

    if(event->key() == Qt::Key_Down &&(event->modifiers() & Qt::ControlModifier)) {
        if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {

            if(!this->selectionModel()->selectedIndexes().count())
                return;

            QFileInfo selection = model->fileInfo(this->selectionModel()->selectedIndexes().first());

            if(selection.isDir()) {
                this->setRootIndex(model->index(selection.filePath()));
                emit changed(selection.absoluteFilePath());
                event->accept();
            }

            return;
        }
    }

    QListView::keyPressEvent(event);
}

void gnomonFinderListView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {

        Q_UNUSED(model);

        QModelIndex index = indexAt(event->pos());

        if(!index.isValid())
            return;

        QListView::mouseDoubleClickEvent(event);
    }
}

void gnomonFinderListView::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes = selectedIndexes();

    if(indexes.count() > 0) {

        QMimeData *data = model()->mimeData(indexes);
        if(!data)
            return;

        QFileIconProvider provider;

        QPixmap pixmap = provider.icon(QFileInfo(data->urls().first().toLocalFile())).pixmap(64, 64);
        QDrag *drag = new QDrag(this);
        drag->setPixmap(pixmap);
        drag->setMimeData(data);
        drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
        drag->exec(supportedActions, Qt::IgnoreAction);
    }
}

// /////////////////////////////////////////////////////////////////
// gnomonFinderTreeView
// /////////////////////////////////////////////////////////////////

class gnomonFinderTreeViewPrivate
{
public:
    QMenu *menu;
    QList<QAction *> defaultActions;
    QList<QAction *> customActions;
    QAction *bookmarkAction;
    bool allowFileBookmarking;
};

gnomonFinderTreeView::gnomonFinderTreeView(QWidget *parent) : QTreeView(parent), d(new gnomonFinderTreeViewPrivate)
{
    this->setDragEnabled(true);
    this->setFrameStyle(QFrame::NoFrame);
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setSortingEnabled(true);
    this->sortByColumn(0, Qt::AscendingOrder);

    d->menu = new QMenu(this);
    d->allowFileBookmarking = true;
    d->bookmarkAction = new QAction(tr("Bookmark"), this);
    d->bookmarkAction->setIconVisibleInMenu(true);
    // d->bookmarkAction->setIcon(QIcon(":dtkGui/pixmaps/star.svg"));
    connect(d->bookmarkAction, SIGNAL(triggered()), this, SLOT(onBookmarkSelectedItemsRequested()));
    d->menu->addAction(d->bookmarkAction);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(updateContextMenu(const QPoint&)));
}

gnomonFinderTreeView::~gnomonFinderTreeView(void)
{
    delete d;

    d = NULL;
}

int gnomonFinderTreeView::sizeHintForColumn(int column) const
{
    if(column == 1)
        return 400;

    return 100;
}

void gnomonFinderTreeView::addContextMenuAction(QAction *action)
{
    d->customActions.append(action);
}

void gnomonFinderTreeView::addDefaultContextMenuAction(QAction *action)
{
    d->defaultActions.append(action);
}

QString gnomonFinderTreeView::selectedPath() const
{
    if(!selectedIndexes().count())
        return QString();

    return this->selectedPaths()[0];
}

QStringList gnomonFinderTreeView::selectedPaths() const
{
    if(!selectedIndexes().count())
        return QStringList();

    QList<int> alreadyReadRows;

    if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model()))
    {
        QStringList selectedPaths = *(new QStringList());

        foreach(QModelIndex index, selectedIndexes())
        {
            if(!alreadyReadRows.contains(index.row()))
            {
                selectedPaths << model->filePath(index);
                alreadyReadRows << index.row();
            }
        }

        return selectedPaths;
    }
    else
        return QStringList();
}

void gnomonFinderTreeView::allowFileBookmarking(bool isAllowed)
{
    d->allowFileBookmarking = isAllowed;
}

void gnomonFinderTreeView::updateContextMenu(const QPoint& point)
{
    QModelIndex index = this->indexAt(point);

    d->menu->clear();

    if(index.isValid()) {
        if(!d->allowFileBookmarking) {
            bool removed = false;
            QString selectedPath = this->selectedPath();
            if(!selectedPath.isEmpty())
            {
                QFileInfo fileInfo = QFileInfo(selectedPath);
                if(fileInfo.isFile())
                {
                    d->menu->removeAction(d->bookmarkAction);
                    removed = true;
                }
            }
            if(!removed)
            {
                if(d->menu->actions().size() > 0)
                    d->menu->insertAction(d->menu->actions()[0], d->bookmarkAction);
                else
                    d->menu->addAction(d->bookmarkAction);
            }
        }

        for(int i = 0; i < d->customActions.size(); i++) {
            d->menu->addAction(d->customActions.value(i));
        }

        d->menu->addSeparator();

    }

    for(int i = 0; i < d->defaultActions.size(); i++) {
        d->menu->addAction(d->defaultActions.value(i));
    }

    d->menu->exec(mapToGlobal(point));

}

void gnomonFinderTreeView::onBookmarkSelectedItemsRequested(void)
{
    if(!selectedIndexes().count())
        return;

    foreach(QString path, this->selectedPaths())
        emit bookmarked(path);
}

void gnomonFinderTreeView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Up &&(event->modifiers() & Qt::ControlModifier)) {
        if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {
            QDir dir = QDir(model->filePath(this->rootIndex()));
            dir.cdUp();
            this->setRootIndex(model->index(dir.absolutePath()));
            emit changed(dir.absolutePath());
            event->accept();
            return;
        }
    }

    if(event->key() == Qt::Key_Down &&(event->modifiers() & Qt::ControlModifier)) {
        if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {

            if(!this->selectionModel()->selectedIndexes().count())
                return;

            QFileInfo selection = model->fileInfo(this->selectionModel()->selectedIndexes().first());

            if(selection.isDir()) {
                this->setRootIndex(model->index(selection.filePath()));
                emit changed(selection.absoluteFilePath());
                event->accept();
            }

            return;
        }
    }

    QTreeView::keyPressEvent(event);
}

void gnomonFinderTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(QFileSystemModel *model = qobject_cast<QFileSystemModel *>(this->model())) {

        Q_UNUSED(model);

        QModelIndex index = indexAt(event->pos());

        if(!index.isValid())
            return;

        QTreeView::mouseDoubleClickEvent(event);
    }
}

void gnomonFinderTreeView::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes = selectedIndexes();

    if(indexes.count() > 0) {

        QMimeData *data = model()->mimeData(indexes);
        if(!data)
            return;

        QFileIconProvider provider;

        QPixmap pixmap = provider.icon(QFileInfo(data->urls().first().toLocalFile())).pixmap(64, 64);
        QDrag *drag = new QDrag(this);
        drag->setPixmap(pixmap);
        drag->setMimeData(data);
        drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
        drag->exec(supportedActions, Qt::IgnoreAction);
    }
}

void gnomonFinderTreeView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    this->setColumnWidth(0, this->width() / 2);
}

// /////////////////////////////////////////////////////////////////
// gnomonFinder
// /////////////////////////////////////////////////////////////////

class gnomonFinderPrivate
{
public:
    void setup(void);
    void setdw(void);

public:
    QFileSystemModel *model;

public:
    bool isAllowedMultipleSelection;
    bool hiddenFilesShown;

public:
    QAction *iconviewAction;
    QAction *listviewAction;
    QAction *showHideAction;

public:
    gnomonFinderListView *list;
    gnomonFinderTreeView *tree;

public:
    QStackedWidget *stack;

public:
    QString path;

public:
    gnomonFinder *q = nullptr;
};

void gnomonFinderPrivate::setup(void)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    q->setPath(settings.value("path").toString());
}

void gnomonFinderPrivate::setdw(void)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    settings.setValue("path", this->path);
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonFinder::gnomonFinder(QWidget *parent) : QFrame(parent), d(new gnomonFinderPrivate)
{
    d->q = this;

    d->model = new QFileSystemModel(this);
    d->model->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    d->hiddenFilesShown = false;
#ifdef Q_WS_MAC
    d->hiddenFilesShown = true;
    d->model->setFilter(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);
#endif
    d->model->setRootPath(QDir::rootPath());

    d->list = new gnomonFinderListView(this);
    d->list->setModel(d->model);
    d->list->setRootIndex(d->model->index(QDir::currentPath()));

    d->tree = new gnomonFinderTreeView(this);
    d->tree->setModel(d->model);
    d->tree->setRootIndex(d->model->index(QDir::currentPath()));
    d->tree->setSelectionBehavior(QAbstractItemView::SelectRows);
    d->tree->setAllColumnsShowFocus(true);

    d->iconviewAction = new QAction(tr("Icon view"), this);
    d->iconviewAction->setIconVisibleInMenu(true);
    // d->iconviewAction->setIcon(QIcon(":dtkGui/pixmaps/dtk-view-list.png"));
    d->listviewAction = new QAction(tr("List view"), this);
    d->listviewAction->setIconVisibleInMenu(true);
    // d->listviewAction->setIcon(QIcon(":dtkGui/pixmaps/dtk-view-tree.png"));
    d->showHideAction = new QAction(tr("Show hidden files"), this);

#ifdef Q_WS_MAC
    d->showHideAction->setText(tr("Hide hidden files"));
#endif
    d->showHideAction->setIconVisibleInMenu(true);
    // d->showHideAction->setIcon(QIcon(":dtkGui/pixmaps/hidden-folder.png"));

    d->list->addDefaultContextMenuAction(d->listviewAction);
    d->list->addDefaultContextMenuAction(d->showHideAction);

    d->tree->addDefaultContextMenuAction(d->iconviewAction);
    d->tree->addDefaultContextMenuAction(d->showHideAction);

    d->stack = new QStackedWidget(this);
    d->stack->addWidget(d->list);
    d->stack->addWidget(d->tree);
    d->stack->setCurrentIndex(0);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->stack);

    connect(d->iconviewAction, SIGNAL(triggered()), this, SLOT(switchToListView()));
    connect(d->listviewAction, SIGNAL(triggered()), this, SLOT(switchToTreeView()));
    connect(d->showHideAction, SIGNAL(triggered()), this, SLOT(switchShowHiddenFiles()));

    connect(d->list, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onIndexDoubleClicked(QModelIndex)));
    connect(d->tree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onIndexDoubleClicked(QModelIndex)));

    connect(d->list, SIGNAL(clicked(QModelIndex)), this, SLOT(onIndexClicked(QModelIndex)));
    connect(d->tree, SIGNAL(clicked(QModelIndex)), this, SLOT(onIndexClicked(QModelIndex)));

    connect(d->list, SIGNAL(changed(QString)), this, SLOT(setPath(QString)));
    connect(d->tree, SIGNAL(changed(QString)), this, SLOT(setPath(QString)));

    connect(d->list, SIGNAL(changed(QString)), this, SIGNAL(changed(QString)));
    connect(d->tree, SIGNAL(changed(QString)), this, SIGNAL(changed(QString)));

    connect(d->list, SIGNAL(bookmarked(QString)), this, SIGNAL(bookmarked(QString)));
    connect(d->tree, SIGNAL(bookmarked(QString)), this, SIGNAL(bookmarked(QString)));

    connect(d->list->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(onSelectionChanged(const QItemSelection&, const QItemSelection&)));
    connect(d->tree->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(onSelectionChanged(const QItemSelection&, const QItemSelection&)));

    QAction *switchToListViewAction = new QAction(this);
    QAction *switchToTreeViewAction = new QAction(this);

    switchToListViewAction->setShortcut(Qt::ControlModifier + Qt::Key_1);
    switchToTreeViewAction->setShortcut(Qt::ControlModifier + Qt::Key_2);

    this->addAction(switchToListViewAction);
    this->addAction(switchToTreeViewAction);

    connect(switchToListViewAction, SIGNAL(triggered()), this, SLOT(switchToListView()));
    connect(switchToTreeViewAction, SIGNAL(triggered()), this, SLOT(switchToTreeView()));

    d->setup();
}

gnomonFinder::~gnomonFinder(void)
{
    d->setdw();

    delete d;

    d = NULL;
}

void gnomonFinder::addContextMenuAction(QAction *action)
{
    d->list->addContextMenuAction(action);
    d->tree->addContextMenuAction(action);
}

QString gnomonFinder::selectedPath(void) const
{
    if(d->stack->currentIndex() == 0)
        return d->list->selectedPath();

    if(d->stack->currentIndex() == 1)
        return d->tree->selectedPath();

    return QString();
}

QStringList gnomonFinder::selectedPaths() const
{
    if(d->stack->currentIndex() == 0)
        return d->list->selectedPaths();

    if(d->stack->currentIndex() == 1)
        return d->tree->selectedPaths();

    return QStringList();
}

void gnomonFinder::allowFileBookmarking(bool isAllowed)
{
    d->list->allowFileBookmarking(isAllowed);
    d->tree->allowFileBookmarking(isAllowed);
}

void gnomonFinder::allowMultipleSelection(bool isAllowed)
{
    d->isAllowedMultipleSelection = isAllowed;

    if(isAllowed)
    {
        d->list->setSelectionMode(QAbstractItemView::ExtendedSelection);
        d->tree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
    else
    {
        d->list->setSelectionMode(QAbstractItemView::SingleSelection);
        d->tree->setSelectionMode(QAbstractItemView::SingleSelection);
    }
}

void gnomonFinder::setPath(const QString& path)
{
    d->path = path;

    if(sender() != d->list)
        d->list->setRootIndex(d->model->index(path));

    if(sender() != d->tree)
        d->tree->setRootIndex(d->model->index(path));
}

void gnomonFinder::switchToListView(void)
{
    emit listView();

    d->stack->setCurrentIndex(0);

    emitSelectedItems();
}

void gnomonFinder::switchToTreeView(void)
{
    emit treeView();

    d->stack->setCurrentIndex(1);

    emitSelectedItems();
}

void gnomonFinder::onShowHiddenFiles(bool show)
{
    if(show) {
        d->hiddenFilesShown = true;
        d->showHideAction->setText(tr("Hide hidden files"));
        d->model->setFilter(QDir::Hidden | QDir::AllEntries | QDir::NoDotAndDotDot);
    }
    else {
        d->hiddenFilesShown = false;
        d->showHideAction->setText(tr("Show hidden files"));
        d->model->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    }
}

void gnomonFinder::switchShowHiddenFiles(void)
{
//    this->onShowHiddenFiles(!d->hiddenFilesShown);
    emit showHiddenFiles(!d->hiddenFilesShown);

}

void gnomonFinder::onIndexClicked(QModelIndex index)
{
    emit fileClicked(d->model->fileInfo(index));
}

void gnomonFinder::onIndexDoubleClicked(QModelIndex index)
{
    QFileInfo selection = d->model->fileInfo(index);

    if(selection.isDir()) {
        QModelIndex idx = d->model->index(d->model->filePath(index));

        d->list->setRootIndex(idx);
        d->tree->setRootIndex(idx);

        d->path = d->model->filePath(index);

        emit changed(selection.absoluteFilePath());
    }
    else
        emit fileDoubleClicked(selection.absoluteFilePath());

    emit nothingSelected();
}

void gnomonFinder::onBookmarkSelectedItemsRequested(void)
{
    if(d->stack->currentIndex() == 0)
        d->list->onBookmarkSelectedItemsRequested();

    if(d->stack->currentIndex() == 1)
        d->tree->onBookmarkSelectedItemsRequested();
}

void gnomonFinder::emitSelectedItems()
{
    QStringList selectedPaths = *(new QStringList());

    if(d->stack->currentIndex() == 0)
        selectedPaths = d->list->selectedPaths();

    if(d->stack->currentIndex() == 1)
        selectedPaths = d->tree->selectedPaths();

    emit selectionChanged(selectedPaths);

    if(!selectedPaths.size())
        emit nothingSelected();
}

void gnomonFinder::onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    emitSelectedItems();
}

//
// gnomonFinder.cpp ends here
