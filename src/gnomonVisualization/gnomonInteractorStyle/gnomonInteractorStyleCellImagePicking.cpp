#include "gnomonInteractorStyleCellImagePicking.h"

#include <vtkCellPicker.h>
#include <vtkCommand.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

vtkStandardNewMacro(gnomonInteractorStyleCellImagePicking);

gnomonInteractorStyleCellImagePicking::gnomonInteractorStyleCellImagePicking(void)
{
}

gnomonInteractorStyleCellImagePicking::~gnomonInteractorStyleCellImagePicking(void)
{
}

void gnomonInteractorStyleCellImagePicking::OnLeftButtonDown(void)
{
    // Get the location of the click (in window coordinates)
    int* pos = this->GetInteractor()->GetEventPosition();

    vtkNew<vtkCellPicker> picker;
    picker->SetTolerance(0.0005);

    // Pick from this location.
    picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

    double* worldPosition = picker->GetPickPosition();
    qDebug() << "Cell id is: " << picker->GetCellId();

    emit pickedCell(picker->GetCellId());
    // Forward events
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}
