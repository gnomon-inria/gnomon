#pragma once

#include "gnomonDynamicFormMetadata.h"

class gnomonAbstractDynamicFormPrivate {
public:
    gnomonAbstractDynamicFormPrivate(): metadata(new gnomonDynamicFormMetadata) {}
    virtual ~gnomonAbstractDynamicFormPrivate() {
        delete metadata;
    }

public:
    gnomonDynamicFormMetadata* metadata = nullptr;
};
