#include "gnomonAbstractQmlVisualization.h"
#include "gnomonAbstractVisualization_p.h"

#include "gnomonView/gnomonQmlView.h"


// /////////////////////////////////////////////////////////////////
// gnomonAbstractQmlVisualization
// /////////////////////////////////////////////////////////////////

gnomonAbstractQmlVisualization::gnomonAbstractQmlVisualization(void) : gnomonAbstractVisualization()
{
}

gnomonAbstractQmlVisualization::~gnomonAbstractQmlVisualization(void)
{
}

gnomonQmlView *gnomonAbstractQmlVisualization::qmlView(void)
{
    if (auto qml_view = dynamic_cast<gnomonQmlView *>(d->view)) {
        return qml_view;
    } else {
        return nullptr;
    }
}

QImage gnomonAbstractQmlVisualization::imageRendering(void)
{
    QImage image(1500, 1500, QImage::Format_RGB32);
    QRgb value = qRgb(168, 168, 168);
    for(int w=0; w<image.width(); w++) {
        for(int h=0; h<image.height(); h++) {
            image.setPixel(w, h, value);
        }
    }

    QString text = this->qmlView()->displayText();
    int n_lines = text.count("\n")+1;
    int font_size = image.height() / (2.5*(n_lines+1));

    QPainter p;
    p.begin(&image);
    p.setPen(QPen(Qt::white));
    p.setFont(QFont("Times", font_size, QFont::Bold));
    p.drawText(image.rect(), Qt::AlignLeft|Qt::AlignVCenter, text);
    p.end();

    return image;
}

//
// gnomonAbstractQmlVisualization.cpp ends here
