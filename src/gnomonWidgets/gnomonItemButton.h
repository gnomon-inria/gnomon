#include <gnomonWidgetsExport.h>

#include <QtCore>
#include <QLabel>

class gnomonFontAwesome;

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

private:
    gnomonFontAwesome *font;
};
