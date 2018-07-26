#pragma once

#include <gnomonCoreExport.h>

#include "gnomonAbstractModel.h"

#include <dtkCore>

class GNOMONCORE_EXPORT gnomonExpansionSphereModel : public gnomonAbstractModel
{
public:
    gnomonExpansionSphereModel(void);
    ~gnomonExpansionSphereModel(void);

public:
    void run(std::size_t time, std::size_t dt) override;

public:
    void setExpansionRate(double expansion_rate);
    double expansionRate(void);

private:
    class gnomonExpansionSphereModelPrivate *d;
};

DTK_DECLARE_OBJECT        (gnomonExpansionSphereModel *)
//
// gnomonExpansionSphereModel.h ends here
