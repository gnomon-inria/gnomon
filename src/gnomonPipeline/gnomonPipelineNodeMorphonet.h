#pragma once

#include <gnomonPipelineExport.h>
#include "gnomonPipelineNode.h"


class GNOMONPIPELINE_EXPORT gnomonPipelineNodeMorphonet : public gnomonPipelineNode 
{
public:
    gnomonPipelineNodeMorphonet(const QString& form_name, QJsonObject morphonet_data);
    ~gnomonPipelineNodeMorphonet(void);

public:
    virtual QString toToml(void) override;
    virtual QString toLuigiClass(void) override;
    virtual const QJsonObject toJson(void) override;
    virtual QJsonObject morphonet_data(void) override; 


protected:
    class gnomonPipelineNodeMorphonetPrivate *dd;


};
