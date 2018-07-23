#include "gnomonFileSystemFormReader.h"
#include "gnomonIntensityImage.h"

#include <dtkImagingCore>

class gnomonFileSystemFormReaderPrivate {
public:
    gnomonTime::Mode time_mode = gnomonTime::Mode::Unknown;

    QMap<gnomonTime, QString> files_paths;
};

#pragma message "Make it possible to have more parameters than just a file path"
// gnomonFileSystemFormReader::gnomonFileSystemFormReader(const QVariantHash& parameters) : d(new gnomonFileSystemFormReaderPrivate)
gnomonFileSystemFormReader::gnomonFileSystemFormReader(const QString& configuration_file_path) : d(new gnomonFileSystemFormReaderPrivate)
{
    // if(!parameters.contains("configuration_file_path")) {
    //     qWarning() << Q_FUNC_INFO << "The configuration file path could not be infered from the parameters";
    //     return;
    // }

    // QString configuration_file_path = parameters["configuration_file_path"].toString();

    if (QFileInfo(configuration_file_path).suffix() != "dyform") {
        qWarning() << Q_FUNC_INFO << "The file doesn't match the required file format : dyform";
        return;
    }

    QFile file(configuration_file_path);

    QFileInfo file_info(file);
    QIODevice *in = &file;

    QIODevice::OpenMode mode = QIODevice::ReadOnly;
    mode |= QIODevice::Text;

    // to avoid troubles with floats separators ('.' and not ',')
    QLocale::setDefault(QLocale::c());
#if defined (Q_OS_UNIX) && !defined(Q_OS_MAC)
    setlocale(LC_NUMERIC, "C");
#endif

    if (!in->open(mode)) {
        qWarning() << Q_FUNC_INFO << "The file could not be found, please verify the path";
        return;
    }

    QString line = in->readLine().trimmed();
    QRegExp re = QRegExp("\\s+");

    if(line.isEmpty()) {
        return;
    }

    QStringList line_split = line.split(re);

    if(line_split.size() != 2 || line_split[0] != "Mode") {
        qWarning() << Q_FUNC_INFO << "The first line of the file must contains : \"Mode \'timeMode\'\" ";
        return;
    }

    QString time_mode = line_split[1];

    if(time_mode == "ArbitraryTime") {
        d->time_mode = gnomonTime::Mode::ArbitraryTime;
    } else if(time_mode == "DateTime") {
        d->time_mode = gnomonTime::Mode::DateTime;
    } else {
        qWarning() << Q_FUNC_INFO << "The time mode :" << line_split[1] << "present in the configuration file does not match any of the available modes";
        return;
    }

    while (!in->atEnd()) {
        line = in->readLine().trimmed();
        line_split = line.split(re);
        if(line_split.size() != 2) {
            qWarning() << Q_FUNC_INFO << "One of the entry of the dyform file doesn't match the POC format : \'time path\'";
            continue;
        }
#pragma message "a QFile could be created at this point and stored in the map"
        d->files_paths[gnomonTime(line_split[0].toULong(), d->time_mode)] = file_info.dir().absolutePath() + QString("/") + line_split[1];
    }

}

gnomonFileSystemFormReader::~gnomonFileSystemFormReader(void)
{
    delete d;
}

gnomonAbstractForm * gnomonFileSystemFormReader::read(const gnomonTime& time)
// gnomonAbstractFormPtr gnomonFileSystemFormReader::read(const gnomonTime& time)
{
    if(time.getMode() != d->time_mode) {
        qWarning() << Q_FUNC_INFO << "The time mode does not match the underlying dyform time mode";
        // return gnomonAbstractFormPtr();
        return NULL;
    }

    if(!d->files_paths.contains(time)) {
        qWarning() << Q_FUNC_INFO << "The requested time is not available";
        // return gnomonAbstractFormPtr();
        return NULL;
    }

#pragma message "The type of the form should be specified in the dyform file"
    dtkImageReader *image_reader = dtkImaging::reader::pluginFactory().create("dtkVtkImageReader");
    if(!image_reader) {
        qWarning() << Q_FUNC_INFO << "The vtkImageReader plugin could lot be loaded, make sure you have compiled the VTK plugins and added them the the dtkImaging plugins path";
        // return gnomonAbstractFormPtr();
        return NULL;
    }

    dtkImage *dtk_image = image_reader->read(d->files_paths[time]);
    if(!dtk_image) {
        qWarning() << Q_FUNC_INFO << "The image could not be properly read.";
        // return gnomonAbstractFormPtr();
        delete image_reader;
        return NULL;
    }

    gnomonIntensityImage *image= new gnomonIntensityImage();
    image->setData(dtk_image);

    // gnomonAbstractFormPtr form = gnomonAbstractFormPtr(image);

    delete image_reader;
    return image;
    // return form;
}

gnomonTime::Mode gnomonFileSystemFormReader::timeMode(void)
{
    return d->time_mode;
}
