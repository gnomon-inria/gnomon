#include "gnomonBinaryImageReaderCommand.h"


gnomonBinaryImageReaderCommandPrivate 
{
public:
    gnomonBinaryImageReaderCommandPrivate *gnomonBinaryImage = nullptr;
};

gnomonBinaryImageReaderCommand::gnomonBinaryImageReaderCommand(void):
    d(new gnomonBinaryImageReaderCommandPrivate)
{
    this->factory_name = "binaryImageReader";
    load_plugin(this->factory_name());
    for(auto key: gnomonCore::binaryImageReader::pluginFactory().keys()){
        auto algo = gnomonCore::binaryImageReader::pluginFactory().create(key);
        if(!this->action){
            this->action = algo;
            this-key = key;
        }
        m_description.insert(key, algo->documentation());
        m_extensions.insert(key, algo->extensions());
        m_actions.insert(key, algo);
    }
}
gnomonBinaryImageReaderCommand::~gnomonBinaryImageReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonBinaryImageReaderCommand::redo(void)
{
    Q_ASSERT(this->action);


    ((gnomonAbstractBinaryImageReader *)this->action)->setPath(this->m_path);

}