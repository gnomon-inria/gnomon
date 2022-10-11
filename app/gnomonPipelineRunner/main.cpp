#include <QtCore>

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

#include <dtkCoreApplication.h>
#include <dtkLog>

#include <gnomonPipeline> 
 
int main(int argc, char *argv[])
{
    dtkCoreApplication app = dtkCoreApplication(argc, argv);
    app.setApplicationName("gnomonPipelineRunner");
    app.setApplicationVersion("0.71");
    app.setOrganizationName("inria");

    //Option parser
    QCommandLineParser *parser = app.parser();
    QString desc = "gnomonPipelineRunner allows to run a pipeline from a pipeline file."
    ""
    "Example:"
    "gnomonPipelineRunner ./my_pipeline.json --ios imageReader1,./my_image.inr  --ios imageReader2,./other_image.inr --data-dir /home/toto";
    parser->setApplicationDescription(desc);
    parser->addPositionalArgument("pipeline", "the json file containing the pipeline to run");
    parser->addOption({"ios", "Set a couple <parameter,file> to change the default input/outputs.", "ios"});
    parser->addOption({"data-dir", "Path to the directory containing the data", "data-dir"});

    app.initialize();
    dtkLogger::instance().attachConsole();
    Py_Initialize();

    if(parser->positionalArguments().size() == 1) {
        QString pipeline_file = parser->positionalArguments()[0];

        QString data_dir= "";
        if(parser->isSet("data-dir")) {
            data_dir = parser->value("data-dir");
            dtkInfo() << "Data-dir is" << data_dir;
        }

        PyObject* pModule = PyImport_ImportModule("gnomon.utils.pipelines");
        if(!pModule) {
            dtkError() << "cannot import module gnomon.utils.pipelines";
            return 1;
        } 

        PyObject* pFunc = PyObject_GetAttrString(pModule, "load_pipeline");
        if(pFunc && PyCallable_Check(pFunc))
        {
            PyObject *ios_dict = PyDict_New();
            QStringList io_options = parser->values("ios");
            for(QString io: io_options) {
                QStringList io_list = io.split(QRegularExpression("[, ]"), Qt::SkipEmptyParts);
                dtkInfo() << "Io " << io_list[0] << ":" << io_list[1];
                PyObject *val = PyUnicode_FromString(io_list[1].toStdString().c_str()); 
                PyDict_SetItemString(ios_dict, io_list[0].toStdString().c_str(), val);
                Py_DECREF(val);
            }
            qDebug() << "making args" << ios_dict;
            PyObject* args = Py_BuildValue("(s, s, O)", 
                pipeline_file.toStdString().c_str(),
                data_dir.toStdString().c_str(),
                ios_dict);
            PyObject_CallObject(pFunc, args);
            Py_DECREF(args);
            Py_DECREF(ios_dict);
        } else {
            dtkError() << "Cannot call load_pipeline";
        }
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        dtkError() << "No pipeline set, nothing is done. Run with --help to show usage";
    }

    Py_Finalize();
    return 0;
}
