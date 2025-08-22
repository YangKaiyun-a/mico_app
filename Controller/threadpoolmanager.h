#ifndef THREADPOOLMANAGER_H
#define THREADPOOLMANAGER_H

#include "taskrunnable.h"

#include <QObject>
#include <QThreadPool>

class ThreadPoolManager : public QObject
{
    Q_OBJECT
public:
    static ThreadPoolManager *instance();
    ThreadPoolManager(const ThreadPoolManager&) = delete;
    ThreadPoolManager& operator=(const ThreadPoolManager&) = delete;

    QThreadPool *pool() const;
    void setMaxThreadCount(int n);
    int maxThreadCount() const;
    QString startFunction(std::function<void(TaskRunnable*)> work, int priority = 0, const QString& id = QString());
    void startRunnable(TaskRunnable* r, int priority = 0);
    // 取消任务（协作式）：若任务已开始，将在任务自行检查 isCancelled() 后尽快退出
    void cancel(const QString& id);
    // 等待全部任务完成
    void waitForDone();

signals:
    void taskStarted(const QString& id);
    void taskProgress(const QString& id, int percent);
    void taskFinished(const QString& id);
    void taskFailed(const QString& id, const QString& error);


private slots:
    void onTaskDone(const QString& id);

private:
    explicit ThreadPoolManager(QObject *parent = nullptr);
    ~ThreadPoolManager() override = default;

    void trackAndForwardSignals(TaskRunnable* r);

    QThreadPool* m_pool;
    QHash<QString, QPointer<TaskRunnable>> m_tasks; // 追踪活跃任务
    QMutex m_mutex;
};

#endif // THREADPOOLMANAGER_H
