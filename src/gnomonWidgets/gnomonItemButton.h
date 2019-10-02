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

class GNOMONWIDGETS_EXPORT gnomonItemButton : public QLabel
{
    Q_OBJECT

public:
     gnomonItemButton(const QColor& color, int icon, QWidget *parent = nullptr);
    ~gnomonItemButton(void);

signals:
    void clicked(void);

protected:
    void mousePressEvent(QMouseEvent *);
};

// 
// gnomonItemButton.h ends here
