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

protected:
    QLabel *message_label = nullptr;
};

//
// gnomonMessageBoard.h ends here