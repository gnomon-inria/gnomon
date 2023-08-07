#pragma once

#include <QtCore>
#include <gnomonView/gnomonVtkView>

class MaterialPreviewPrivate;

class MaterialPreview: public QObject {
    Q_OBJECT

    Q_PROPERTY(gnomonVtkView* view READ view CONSTANT);
    Q_PROPERTY(QColor ambient READ ambient WRITE setAmbient NOTIFY ambientChanged)
    Q_PROPERTY(QColor specular READ specular WRITE setSpecular NOTIFY specularChanged)
    Q_PROPERTY(QColor emission READ emission WRITE setEmission NOTIFY emissionChanged)
    Q_PROPERTY(double diffuse READ diffuse WRITE setDiffuse NOTIFY diffuseChanged)
    Q_PROPERTY(double shininess READ shininess WRITE setShininess NOTIFY shininessChanged)
    Q_PROPERTY(double transparency READ transparency WRITE setTransparency NOTIFY transparencyChanged)

public:
    MaterialPreview(QObject *parent=nullptr);

    ~MaterialPreview() override;

    Q_INVOKABLE void init();
    Q_INVOKABLE void setCamera();

public:
    gnomonVtkView *view() const;

    QColor ambient() const;
    QColor specular() const;
    QColor emission() const;
    double diffuse() const;
    double shininess() const;
    double transparency() const;
    void setAmbient(const QColor& color);
    void setSpecular(const QColor& color);
    void setEmission(const QColor& color);
    void setDiffuse(const double v);
    void setShininess(const double v);
    void setTransparency(const double v);


signals:
    void ambientChanged(QColor ambient);
    void specularChanged(QColor specular);
    void emissionChanged(QColor emission);
    void diffuseChanged(double diffuse);
    void shininessChanged(double shininess);
    void transparencyChanged(double transparency);

private:
    MaterialPreviewPrivate* d = nullptr;


};
