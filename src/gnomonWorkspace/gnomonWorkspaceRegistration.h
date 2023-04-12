#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

#include "gnomonVisualization/gnomonView/gnomonQmlView.h"

class gnomonVtkView;
class gnomonVtkViewList;

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceRegistration : public gnomonAlgorithmWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace allows to perform the geometrical registration of an Image \
form (called \"floating\") onto another Image form (called \"reference\").\n\
\n\
The output is a Image form that corresponds to the transformed floating Image. \
The resulting geometrical trasformation is also stored as an output, displayed \
in the right menu.\n\
\n")

public:
     gnomonWorkspaceRegistration(QObject *parent = nullptr);
    ~gnomonWorkspaceRegistration(void);

public:
    Q_PROPERTY(int stackSize READ stackSize NOTIFY stackSizeChanged);
    Q_PROPERTY(int stackLevel READ stackLevel WRITE setStackLevel NOTIFY stackLevelChanged);
    Q_PROPERTY(gnomonQmlView* targetDict READ targetDict CONSTANT);

public:
    int stackSize(void) const;

    int stackLevel(void) const;
    void setStackLevel(int level);

signals:
    void stackSizeChanged(void);
    void stackLevelChanged(void);

// public:
//     Q_INVOKABLE QString transformStringAt(int level) const;

public slots:
    virtual void setInputs(void) override;
    virtual void viewOutputs(void) override;
    void iterate(void);

public: 
    gnomonQmlView *targetDict(void) const {return this->m_target_dict;};

protected:
    class gnomonWorkspaceRegistrationPrivate *dd = nullptr;
    gnomonQmlView *m_target_dict = nullptr;
};

//
// gnomonWorkspaceRegistration.h ends here
