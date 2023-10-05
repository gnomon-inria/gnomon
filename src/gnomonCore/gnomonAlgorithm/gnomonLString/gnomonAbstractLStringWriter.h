#pragma once

#include <gnomonCoreExport>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractFormWriter.h"

#include "gnomonForm/gnomonLString/gnomonLString.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractLStringWriter : public gnomonAbstractFormWriter
{
public:
    virtual ~gnomonAbstractLStringWriter(void) = default;

public:
    virtual void setLString(std::shared_ptr<gnomonLStringSeries> lString) = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonLString") {
            return {"setLString"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonLString") {
            return {"lString"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        return {};
    };
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractLStringWriter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractLStringWriter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractLStringWriter, GNOMONCORE_EXPORT, lStringWriter, gnomonCore)


//
// gnomonAbstractImageWriter.h ends here
