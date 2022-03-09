#pragma once

%module(directors="1", package="gnomon.pipeline", moduleimport="import _gnomonpipeline") gnomonpipeline

%{
#define SWIG_FILE_WITH_INIT
%}

%include "numpy.i"

%init %{
import_array();
%}

%include "std_array.i"
%include "std_vector.i"
%include "carrays.i"

%array_class(double, doubleArray);

%import <dtkBase/dtkBase.i>
%import <dtkCore/dtkCore.i>

%{
    #include <dtkCore>

    #include <gnomonCore>
    #include <gnomonPipeline.h>
    #include <gnomonPipelineManager.h>
    #include <gnomonPipelineEdge.h>
    #include <gnomonPipelineNode.h>
    #include <gnomonPipelinePort.h>
%}

%include <gnomonCore/gnomonCore.i>


// /////////////////////////////////////////////////////////////////
// Macro undefinition
// /////////////////////////////////////////////////////////////////

#undef  GNOMONPIPELINE_EXPORT
#define GNOMONPIPELINE_EXPORT
#undef  Q_INVOKABLE
#define Q_INVOKABLE



%include <gnomonPipeline.h>
%include <gnomonPipelineManager.h>
%include <gnomonPipelineEdge.h>
%include <gnomonPipelineNode.h>
%include <gnomonPipelinePort.h>