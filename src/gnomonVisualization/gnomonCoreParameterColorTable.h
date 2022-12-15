#pragma once

#include <gnomonVisualizationExport>

#include <QtCore>
#include <QtGui>

#include <dtkCore/dtkCoreParameter>

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterColorTable declaration
// ///////////////////////////////////////////////////////////////////


class GNOMONVISUALIZATION_EXPORT gnomonColorTable: public QObject
{
    Q_OBJECT

public:
    gnomonColorTable(void) = default;
    gnomonColorTable(const gnomonColorTable&);

    ~gnomonColorTable(void) = default;

    gnomonColorTable& operator = (const gnomonColorTable&);
    bool operator != (const gnomonColorTable&);

public:
    int size(void) const;

    QList<long> indices(void) const;
    long indexAt(int pos) const;

public:
    void clear(void);

public:
    bool isTexture(long index) const;
    QString textureFile(long index) const;

    bool isColor(long index) const;
    QColor color(long index) const;

public:
    void setColor(long index, const QColor& color);
    void setTexture(long index, const QString& texture);

private:
    QMap<long, QString> textures;
    QMap<long, QColor> colors;
};


class GNOMONVISUALIZATION_EXPORT gnomonCoreParameterColorTable : public dtkCoreParameterBase<gnomonCoreParameterColorTable>
{
public:

    using self_type = gnomonCoreParameterColorTable;
    using base_type = dtkCoreParameterBase<self_type>;

     gnomonCoreParameterColorTable(void);
     gnomonCoreParameterColorTable(const dtkCoreParameter *);
     gnomonCoreParameterColorTable(const QString& label, const gnomonColorTable& color_table, const QString& doc = QString());
     gnomonCoreParameterColorTable(const QVariant&);
     gnomonCoreParameterColorTable(const gnomonCoreParameterColorTable&);
    ~gnomonCoreParameterColorTable(void);

    gnomonCoreParameterColorTable& operator = (const dtkCoreParameter *);
    gnomonCoreParameterColorTable& operator = (const QVariant&);
    gnomonCoreParameterColorTable& operator = (const gnomonCoreParameterColorTable&);

    void setValue(const gnomonColorTable&);
    void setValue(const QVariant&) override;

    gnomonColorTable value(void) const;

public:
    QColor color(long i) const;
    void setColor(long i, const QColor& color);

    QString texture(long i) const;
    void setTexture(long i, const QString& texture);

    void clearColors(void);

    int colorIndexCount(void) const;
    QList<long> colorIndices(void) const;
    long colorIndexAt(int index) const;

public:
    QVariantHash toVariantHash(void) const override;

    dtkCoreParameterObject *object(void) override;

    using dtkCoreParameter::documentation;
    using dtkCoreParameter::setDocumentation;
    using dtkCoreParameter::label;
    using dtkCoreParameter::setLabel;

private:
    using dtkCoreParameter::m_label;
    using dtkCoreParameter::m_doc;

    gnomonColorTable m_c;

private:
    class gnomonCoreParameterColorTableObject *m_object = nullptr;
};

GNOMONVISUALIZATION_EXPORT QDataStream& operator << (QDataStream&, const gnomonCoreParameterColorTable&);
GNOMONVISUALIZATION_EXPORT QDataStream& operator >> (QDataStream&, gnomonCoreParameterColorTable&);
GNOMONVISUALIZATION_EXPORT QDebug operator << (QDebug, gnomonCoreParameterColorTable);

DTK_DECLARE_PARAMETER(gnomonCoreParameterColorTable);

//
// gnomonCoreParameterColor.h ends here
