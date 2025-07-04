#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QApplication>

#include "ros2thread.h"


#define hApp (static_cast<Application *>(Application::instance()))


class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);
    ~Application() override;

    static Application* instance(int &argc, char **argv);
    void init();

protected:


private:
    static Application* m_singlenApplication;
    Ros2Thread *m_rosThread = nullptr;
};

#endif // APPLICATION_H
