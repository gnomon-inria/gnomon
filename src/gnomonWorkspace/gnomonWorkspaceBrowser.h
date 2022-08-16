#pragma once

#include <gnomonWorkspaceExport>
#include "gnomonAbstractWorkspace.h"

class gnomonViewForm;

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceBrowser : public gnomonAbstractWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace allows to load data from a local file system into the \
application. It reads a local file, provided there exists a reader plugin \
that is able to open it.\n\
\n\
When a file is dropped into the main view in the center, a menu pops up with \
the choice of available readers for this file format. If the reader is unique, \
the file is read directly. The form contained in the file is then displayed in \
the main view, replacing an existing form of the same type is there was one.\n\
\n")


public:
     gnomonWorkspaceBrowser(QObject *parent = nullptr);
    ~gnomonWorkspaceBrowser(void);

public:
    Q_PROPERTY(gnomonViewForm* view READ view CONSTANT);
    Q_PROPERTY(QStringList extensions READ readerExtensions);
    Q_PROPERTY(QString readerPath READ readerPath WRITE setReaderPath NOTIFY readerPathChanged);

signals:
    void available(const QVariantMap& readers);
    void readerPathChanged(void);

public:
    const QString& readerPath(void) const;

public slots:
    void setReaderPath(const QString&);
    void requestReaders(QString default_reader);
    bool readWith(const QString&);
    inline void saveState() {};  // nothing to be saved
    void restoreState();

    void export_outputs(void) override;

public:
    Q_INVOKABLE QUrl defaultReadPath();
    Q_INVOKABLE gnomonViewForm *view(void);
    QStringList readerExtensions(void);

private:
    class gnomonWorkspaceBrowserPrivate *d;
};

//
// gnomonWorkspaceBrowser.h ends here
