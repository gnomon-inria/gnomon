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

#pragma once

#include <dtkComposer>

#include <gnomonComposerExport.h>

class gnomonAbstractCellComplexReader;

class GNOMONCOMPOSER_EXPORT gnomonCellComplexReaderNode : public dtkComposerNodeObject<gnomonAbstractCellComplexReader>
{
public:
     gnomonCellComplexReaderNode(void);
    ~gnomonCellComplexReaderNode(void);

public:
    void run(void);

private:
    class gnomonCellComplexReaderNodePrivate *d;
};


//
// gnomonCellComplexReaderNode.h ends here