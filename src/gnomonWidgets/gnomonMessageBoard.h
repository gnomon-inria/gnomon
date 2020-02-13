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

#include <gnomonWidgetsExport.h>

#include <QtCore>
#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// gnomonItemButton
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonMessageBoard : public QWidget
{
    Q_OBJECT

public:
     gnomonMessageBoard(QWidget *parent = 0);
    ~gnomonMessageBoard(void);

public:
    void setMessage(const QString& text);

signals:
    void fileDropped(const QString&);

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

protected:
    QLabel *message_label = nullptr;
};

//
// gnomonMessageBoard.h ends here