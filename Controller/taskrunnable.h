#ifndef TASKRUNNABLE_H
#define TASKRUNNABLE_H

#include <QObject>
#include <QRunnable>
#include <QThreadPool>
#include <QPointer>
#include <QHash>
#include <QMutex>
#include <QMutexLocker>
#include <QAtomicInteger>
#include <QDebug>
#include <QUuid>
#include <functional>
#include <atomic>

class TaskRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    using WorkFn = std::function<void(TaskRunnable*)>;
    explicit TaskRunnable(QString id, WorkFn work, QObject *parent = nullptr);
    QString id() const;
    void cancel();
    bool isCancelled() const;
    void reportProgress(int percent);
    void run() override;

signals:
    void started(const QString& id);
    void progress(const QString& id, int percent);
    void finished(const QString& id);
    void failed(const QString& id, const QString& error);

private:
    QString m_id;
    WorkFn m_work;
    std::atomic<bool> m_cancelled{false};
};

#endif // TASKRUNNABLE_H
