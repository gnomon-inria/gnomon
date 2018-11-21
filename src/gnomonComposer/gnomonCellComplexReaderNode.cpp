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

#include "gnomonCellComplexReaderNode.h"

#include "gnomonAbstractCellComplexReader.h"
#include "gnomonCellComplex.h"
#include <QtCore>

#include <dtkLog>

class gnomonCellComplexReaderNodePrivate
{
public:
    dtkComposerTransmitterReceiver<QString> path_in;

	dtkComposerTransmitterEmitter<gnomonCellComplex *> cellcomplex_out;
};


gnomonCellComplexReaderNode::gnomonCellComplexReaderNode(void) : dtkComposerNodeObject<gnomonAbstractCellComplexReader>(), d(new gnomonCellComplexReaderNodePrivate())
{
    this->setFactory(gnomonCore::cellComplexReader::pluginFactory());

    this->appendReceiver(&d->path_in);

    this->appendEmitter (&d->cellcomplex_out);
}

gnomonCellComplexReaderNode::~gnomonCellComplexReaderNode(void)
{
    delete d;
}

void gnomonCellComplexReaderNode::run(void)
{
    if (d->path_in.isEmpty()) {
        dtkError() << Q_FUNC_INFO << "The input is not set. Aborting.";
        return;

    } else {

        gnomonAbstractCellComplexReader *reader = this->object();
        if (!reader && this->implementations().empty()) {
            dtkError() << Q_FUNC_INFO << "No Cellcomplex reader found. Aborting.";
        }

        QString path = d->path_in.constData();

        gnomonCellComplex *cellcomplex = nullptr;
        if (reader) {
            cellcomplex = reader->read(path);

        }

        if (!cellcomplex) {
            dtkError() << Q_FUNC_INFO << "No Cellcomplex read. Aborting.";
            return;
        }
        d->cellcomplex_out.setData(cellcomplex);
    }
}

//
// gnomonCellComplexReaderNode.cpp ends here
