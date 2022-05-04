#include "gnomonWallForm.h"

class gnomonWallFormPrivate {
public:
    gnomon::vec3_t position = {0., 0., 0.};
    gnomon::vec3_t normal = {1., 0., 0.};
};

gnomonWallForm::gnomonWallForm(void) : d(new gnomonWallFormPrivate)
{
//    this->record();
}

gnomonWallForm::~gnomonWallForm(void)
{
    delete d;
}

gnomonAbstractForm *gnomonWallForm::clone()
{
    gnomonWallForm *res = new gnomonWallForm();
    res->setNormal(this->normal());
    res->setPosition(this->position());
    return res;
}

void gnomonWallForm::setNormal(gnomon::vec3_t normal)
{
    d->normal = normal;
}

const gnomon::vec3_t gnomonWallForm::normal(void) const
{
    return d->normal;
}

QString gnomonWallForm::name(void) const
{
    return "gnomonWallForm";
}

void gnomonWallForm::setPosition(gnomon::vec3_t position)
{
    d->position = position;
}

const gnomon::vec3_t gnomonWallForm::position(void) const
{
    return d->position;
}

const QString gnomonWallForm::pluginName(void) {
    return QString();
}

QJsonObject gnomonWallForm::serialize(void) {
    QJsonObject out;
    QJsonArray position;
    for (int i = 0; i < d->position.size(); ++i) {
        position[i] = d->position[i];
    }
    out["position"] = position;
    QJsonArray normal;
    for (int i = 0; i < d->normal.size(); ++i) {
        normal[i] = d->normal[i];
    }
    out["normal"] = normal;
    return out;
}

void gnomonWallForm::deserialize(QJsonObject &serialization) {
    auto position = serialization["position"].toArray();
    for (int i = 0; i < d->position.size(); ++i) {
        d->position[i] = position[i].toDouble();
    }
    auto normal = serialization["normal"].toArray();
    for (int i = 0; i < d->normal.size(); ++i) {
        d->normal[i] = normal[i].toDouble();;
    }
}

gnomonWallForm::gnomonWallForm(QJsonObject &serialization): d(new gnomonWallFormPrivate) {
    deserialize(serialization);
}
