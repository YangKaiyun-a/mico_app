#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QApplication>


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

};

#endif // APPLICATION_H
