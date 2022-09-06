#pragma once

#include <gnomonCoreExport>

#include <dtkCore/dtkCorePlugin>
#include <dtkCore/dtkCoreParameters>

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractLStringReader : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractLStringReader(void) = default;

public:
    virtual void setPath(const QString& path) = 0;

public:
    virtual std::shared_ptr<gnomonLStringSeries> lString(void) = 0;

public:
    static inline QString defaultSetter(QString formName) {
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonLString") {
            return {"lString"};
        }
        return {};
    };

public:
    virtual QStringList extensions(void) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractLStringReader *)
DTK_DECLARE_PLUGIN        (gnomonAbstractLStringReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractLStringReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractLStringReader, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractLStringReader, GNOMONCORE_EXPORT, lStringReader);
}

//
// gnomonAbstractLStringReader.h ends here
