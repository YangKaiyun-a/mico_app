#ifndef DEFINE_H
#define DEFINE_H

#include <QObject>


/***************************宏**************************/
#define SigManager SignalManager::instance()





/***************************枚举类型******************************/
class ENUM_CLASS : public QObject {
    Q_OBJECT

public:
    explicit ENUM_CLASS(QObject *parent = nullptr);

    // 主页面的功能模块
    enum FUNCTION_INDEX
    {
        HOME,
        TASK,
        MAP,
        RECORD,
        ACCOUNT,
        DEBUG
    };


    // 任务状态
    enum TASK_STATUS
    {
        WAITING,
        EXECUTING,
        FINISH,
        FAILED
    };

};



/***************************结构体********************************/



#endif // DEFINE_H
