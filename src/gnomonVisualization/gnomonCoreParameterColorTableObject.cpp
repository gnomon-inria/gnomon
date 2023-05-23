#include "gnomonCoreParameterColorTableObject.h"

// ///////////////////////////////////////////////////////////////////
// gnomonCoreParameterColorObject implementation
// ///////////////////////////////////////////////////////////////////

gnomonCoreParameterColorTableObject::gnomonCoreParameterColorTableObject(gnomonCoreParameterColorTable *p) : dtkCoreParameterObject(p), m_param(p)
{

}

gnomonCoreParameterColorTableObject::~gnomonCoreParameterColorTableObject(void)
{
    m_param = nullptr;
}

void gnomonCoreParameterColorTableObject::setColorTable(const gnomonColorTable& map)
{
    m_param->setValue(map);
}

gnomonColorTable gnomonCoreParameterColorTableObject::colorTable(void) const
{
    return m_param->value();
}

void gnomonCoreParameterColorTableObject::setValue(const QVariantMap& color_map)
{
    gnomonColorTable colorTable;
    for (auto it = color_map.begin(); it != color_map.end(); ++it) {
        long i = it.key().toLong();
        if (it.value().canConvert<QColor>()) {
            colorTable.setColor(i, it.value().value<QColor>());
        } else if(it.value().canConvert<QString>()){
            colorTable.setColor(i, it.value().value<QString>());
        } else {
            auto material = it.value().value<QVariantMap>();
            colorTable.setAmbient(i, material["ambient"].value<QColor>());
            colorTable.setSpecular(i, material["specular"].value<QColor>());
            colorTable.setEmission(i, material["emission"].value<QColor>());
            colorTable.setDiffuse(i, material["diffuse"].value<double>());
            colorTable.setShininess(i, material["shininess"].value<double>());
            colorTable.setTransparency(i, material["transparency"].value<double>());
        }
    }
    m_param->setValue(colorTable);
}

QVariantMap gnomonCoreParameterColorTableObject::value(void) const
{
    gnomonColorTable table = m_param->value();
    QVariantMap color_map;
    for (auto i : table.indices())
    {
        if (table.isColor(i)) {
            color_map[QString::number(i)] = QVariant::fromValue(table.color(i));
        } else if (table.isTexture(i)) {
            color_map[QString::number(i)] = QVariant::fromValue(table.textureFile(i));
        } else if (table.isMaterial(i)) {
            QVariantMap material;
            material["ambient"] = QVariant::fromValue(table.ambient(i));
            material["specular"] = QVariant::fromValue(table.specular(i));
            material["emission"] = QVariant::fromValue(table.emission(i));
            material["diffuse"] = QVariant::fromValue(table.diffuse(i));
            material["shininess"] = QVariant::fromValue(table.shininess(i));
            material["transparency"] = QVariant::fromValue(table.transparency(i));
            color_map[QString::number(i)] = QVariant::fromValue(material);
        }
    }
    return color_map;
}

gnomonCoreParameterColorTable *gnomonCoreParameterColorTableObject::parameter(void)
{
    return m_param;
}

bool gnomonCoreParameterColorTableObject::isColor(long i) const
{
    return m_param->value().isColor(i);
}

QColor gnomonCoreParameterColorTableObject::color(long i) const
{
    return m_param->color(i);
}

void gnomonCoreParameterColorTableObject::setColor(long i, const QColor& color)
{
    bool new_color = !m_param->value().indices().contains(i);
    m_param->setColor(i, color);
    if (new_color) {
        emit colorIndexChanged();
    }
    notifyColorTable(m_param->value());
}

bool gnomonCoreParameterColorTableObject::isTexture(long i) const
{
    return m_param->value().isTexture(i);
}

QString gnomonCoreParameterColorTableObject::texture(long i) const
{
    return m_param->texture(i);
}

void gnomonCoreParameterColorTableObject::setTexture(long i, const QString& texture)
{
    bool new_texture = !m_param->value().indices().contains(i);
    m_param->setTexture(i, texture);
    if (new_texture) {
        emit colorIndexChanged();
    }
    notifyColorTable(m_param->value());
}

void gnomonCoreParameterColorTableObject::clearColors(void)
{
    m_param->clearColors();
    emit colorIndexChanged();
    notifyColorTable(m_param->value());
}

int gnomonCoreParameterColorTableObject::colorIndexCount(void) const
{
    return m_param->colorIndexCount();
}

QList<long> gnomonCoreParameterColorTableObject::colorIndices(void) const
{
    return m_param->colorIndices();
}

Q_INVOKABLE long gnomonCoreParameterColorTableObject::colorIndexAt(int index) const
{
    return m_param->colorIndexAt(index);
}

void gnomonCoreParameterColorTableObject::notifyColorTable(const gnomonColorTable& map)
{
    emit colorTableChanged(map);
    QVariantMap color_map;
    for (auto i : map.indices())
    {
        if (map.isColor(i)) {
            color_map[QString::number(i)] = QVariant::fromValue(map.color(i));
        } else if (map.isTexture(i)) {
            color_map[QString::number(i)] = QVariant::fromValue(map.textureFile(i));
        }
    }
    emit valueChanged(color_map);
}

bool gnomonCoreParameterColorTableObject::isMaterial(long i) const {
    return m_param->value().isMaterial(i);
}

QColor gnomonCoreParameterColorTableObject::ambient(long i) const {
    return m_param->ambient(i);
}

void gnomonCoreParameterColorTableObject::setAmbient(long i, const QColor &color) {
    m_param->setAmbient(i, color);
    notifyColorTable(m_param->value());
}

QColor gnomonCoreParameterColorTableObject::specular(long i) const {
    return m_param->specular(i);
}

void gnomonCoreParameterColorTableObject::setSpecular(long i, const QColor &color) {
    m_param->setSpecular(i, color);
    notifyColorTable(m_param->value());
}

QColor gnomonCoreParameterColorTableObject::emission(long i) const {
    return m_param->emission(i);
}

void gnomonCoreParameterColorTableObject::setEmission(long i, const QColor &color) {
    m_param->setEmission(i, color);
    notifyColorTable(m_param->value());
}

double gnomonCoreParameterColorTableObject::diffuse(long i) const {
    return m_param->diffuse(i);
}

void gnomonCoreParameterColorTableObject::setDiffuse(long i, const double v) {
    m_param->setDiffuse(i, v);
    notifyColorTable(m_param->value());
}

double gnomonCoreParameterColorTableObject::shininess(long i) const {
    return m_param->shininess(i);
}

void gnomonCoreParameterColorTableObject::setShininess(long i, const double v) {
    m_param->setShininess(i, v);
    notifyColorTable(m_param->value());
}

double gnomonCoreParameterColorTableObject::transparency(long i) const {
    return m_param->transparency(i);
}

void gnomonCoreParameterColorTableObject::setTransparency(long i, const double v) {
    m_param->setTransparency(i, v);
    notifyColorTable(m_param->value());
}

//
// gnomonCoreParameterColorObject.cpp ends here
