#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QApplication>


#define hApp (static_cast<Application *>(Application::instance()))


class Application : public QApplication
{
    Q_OBJECT
public:
    ~Application() override;

    static Application* instance();
    static Application* instance(int &argc, char **argv);
    void init();

private:
    explicit Application(int &argc, char **argv);
    static Application* m_singlenApplication;
};

#endif // APPLICATION_H
