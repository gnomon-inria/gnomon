#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkLight.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>

#include "MaterialPreview.hpp"

// -----------------------------------------------------------------------
// MaterialPreviewPrivate
// -----------------------------------------------------------------------

class MaterialPreviewPrivate {
public:
    MaterialPreviewPrivate(QObject *parent): view(parent), actor(vtkNew<vtkActor>()) {

    }

public:
    gnomonVtkView view;

    vtkSmartPointer<vtkActor> actor;
};

// -----------------------------------------------------------------------
// MaterialPreview
// -----------------------------------------------------------------------


MaterialPreview::MaterialPreview(QObject *parent) : QObject(parent), d(new MaterialPreviewPrivate(parent)) {
    // The following lines create a sphere represented by polygons.
    //
    vtkNew<vtkSphereSource> sphereSource;
    sphereSource->SetCenter(0.0, 0.0, 0.0);
    sphereSource->SetRadius(5.0);
    // Make the surface smooth.
    sphereSource->SetPhiResolution(100);
    sphereSource->SetThetaResolution(100);

    // The mapper is responsible for pushing the geometry into the graphics
    // library. It may also do color mapping, if scalars or other attributes
    // are defined.
    //
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(sphereSource->GetOutputPort());

    d->actor->SetMapper(mapper);

}

MaterialPreview::~MaterialPreview() {
    delete d;
}


void MaterialPreview::init() {
    auto renderer = d->view.renderer3D();
    vtkNew<vtkNamedColors> colors;

    vtkNew<vtkLight> light;
    light->SetFocalPoint(1.875, 0.6125, 0);
    light->SetPosition(0.875, 1.6125, 1);
    light->SetAmbientColor(colors->GetColor3d("Gray").GetData());
    renderer->AddLight(light);

    d->actor->GetProperty()->SetInterpolationToPBR();

    renderer->AddActor(d->actor);

    auto cam = renderer->GetActiveCamera();
    cam->SetFocalPoint(0., 0., 0.);
    cam->SetPosition(0., 0., 1.);
    cam->SetViewUp(0., 1., 0.);
    renderer->ResetCamera();

    d->view.render();
}

gnomonVtkView *MaterialPreview::view() const {
    return &d->view;
}

QColor MaterialPreview::ambient() const {
    double color[3];
    d->actor->GetProperty()->GetAmbientColor(color);
    return QColor::fromRgb(color[0], color[1], color[2]);
}

QColor MaterialPreview::specular() const {
    double color[3];
    d->actor->GetProperty()->GetSpecularColor(color);
    return QColor::fromRgb(color[0], color[1], color[2]);
}

QColor MaterialPreview::emission() const {
    double color[3];
    d->actor->GetProperty()->GetEmissiveFactor(color);
    return QColor::fromRgbF(color[0], color[1], color[2]);
}

double MaterialPreview::diffuse() const {
    return d->actor->GetProperty()->GetDiffuse()*500;
}

double MaterialPreview::shininess() const {
    return 1.0 - d->actor->GetProperty()->GetRoughness();
}

double MaterialPreview::transparency() const {
    return 1.0 - d->actor->GetProperty()->GetOpacity();
}

void MaterialPreview::setAmbient(const QColor &color) {
    d->actor->GetProperty()->SetAmbientColor(color.red(), color.green(), color.blue());
    d->actor->GetProperty()->SetDiffuseColor(color.red(), color.green(), color.blue());
    d->view.render();
    emit ambientChanged(color);
}

void MaterialPreview::setSpecular(const QColor &color) {
    d->actor->GetProperty()->SetSpecularColor(color.red(), color.green(), color.blue());
    //d->actor->GetProperty()->SetSpecular(color.valueF());
    d->view.render();
}

void MaterialPreview::setEmission(const QColor &color) {
    d->actor->GetProperty()->SetEmissiveFactor(color.red(), color.green(), color.blue());
    d->view.render();
}

void MaterialPreview::setDiffuse(const double v) {
    d->actor->GetProperty()->SetDiffuse(v/500);
    d->view.render();
}

void MaterialPreview::setShininess(const double v) {
    d->actor->GetProperty()->SetRoughness(1 - v);
    d->actor->GetProperty()->SetSpecularPower(50*v);
    d->view.render();
}

void MaterialPreview::setTransparency(const double v) {
    d->actor->GetProperty()->SetOpacity(1 - v);
    d->view.render();
}

void MaterialPreview::setCamera() {
    auto renderer = d->view.renderer3D();
    auto cam = renderer->GetActiveCamera();
    renderer->ResetCamera();
    cam->SetFocalPoint(-1.6, 0, 0);
    cam->SetPosition(-1.6, 0, 20);
    cam->SetViewUp(0., 1., 0.);
    cam->Zoom(10);
    d->view.render();
}
