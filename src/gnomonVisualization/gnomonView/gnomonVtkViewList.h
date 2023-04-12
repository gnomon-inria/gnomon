#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>

class gnomonVtkView;

class GNOMONVISUALIZATION_EXPORT gnomonVtkViewList : public QObject
{
    Q_OBJECT

public:
     gnomonVtkViewList(QObject *parent = nullptr);
    ~gnomonVtkViewList(void);

public:
    Q_PROPERTY(QList<QObject *> views READ viewsAsQObject);

public slots:
    void addView(const QVector<QString> &accepted_forms = {}, QStringList nodePortNames = QStringList());
	void removeAllViews(void);

public:
    gnomonVtkView *operator[](int);
    QList<gnomonVtkView *> views(void);
    QList<QObject *> viewsAsQObject(void);

/* protected:
    void clear(void);
    void update(void); */

signals:
    void formAdded(const QString&);
    void formsChanged(void);
    void viewAdded(gnomonVtkView *);

private:
    class gnomonVtkViewListPrivate *d;
};

//
// gnomonVtkViewList.h ends here
