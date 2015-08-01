#include "Document.h"
#include "Model.h"
#include "Viewer.h"

Document::Document(QObject *parent) : QObject(parent)
{

}

bool Document::loadModel(QString filename)
{
    auto model = QSharedPointer<Model>(new Model());
    bool isLoaded = model->loadFromFile(filename);
    models << model;
    return isLoaded;
}

void Document::drawModel(QString name, QWidget *widget)
{
    auto glwidget = (Viewer*)widget;
    if (glwidget == nullptr) return;

    auto m = getModel(name);
    if(m != nullptr) m->draw(glwidget);
}

QString Document::firstModelName()
{
    if(models.isEmpty()) return "";
    return models.front()->name();
}

Model* Document::getModel(QString name)
{
    for(auto m : models) if(m->name() == name) return m.data();
    return nullptr;
}
