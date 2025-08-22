#include "threadpoolmanager.h"

ThreadPoolManager::ThreadPoolManager(QObject *parent)
    : QObject{parent}
{
    m_pool = new QThreadPool(this);
    m_pool->setMaxThreadCount(QThread::idealThreadCount());
}

ThreadPoolManager *ThreadPoolManager::instance()
{
    static ThreadPoolManager instance;
    return &instance;
}

QThreadPool *ThreadPoolManager::pool() const
{
    return m_pool;
}

void ThreadPoolManager::setMaxThreadCount(int n)
{
    m_pool->setMaxThreadCount(n);
}

int ThreadPoolManager::maxThreadCount() const
{
    return m_pool->maxThreadCount();
}

QString ThreadPoolManager::startFunction(std::function<void (TaskRunnable *)> work, int priority, const QString &id)
{
    QString finalId = id.isEmpty() ? QUuid::createUuid().toString(QUuid::WithoutBraces) : id;

    auto* r = new TaskRunnable(finalId, std::move(work));
    trackAndForwardSignals(r);
    startRunnable(r, priority);
    return finalId;
}

void ThreadPoolManager::startRunnable(TaskRunnable *r, int priority)
{
    if (!r) return;
    {
        QMutexLocker _(&m_mutex);
        m_tasks.insert(r->id(), QPointer<TaskRunnable>(r));
    }

    connect(r, &TaskRunnable::finished, this, &ThreadPoolManager::onTaskDone, Qt::QueuedConnection);
    connect(r, &TaskRunnable::failed, this, &ThreadPoolManager::onTaskDone, Qt::QueuedConnection);
    m_pool->start(r, priority);
}

void ThreadPoolManager::cancel(const QString &id)
{
    QPointer<TaskRunnable> ptr;
    {
        QMutexLocker _(&m_mutex);
        ptr = m_tasks.value(id);
    }
    if (ptr) ptr->cancel();
}

void ThreadPoolManager::waitForDone()
{
    m_pool->waitForDone();
}

void ThreadPoolManager::onTaskDone(const QString &id)
{
    QMutexLocker _(&m_mutex);
    m_tasks.remove(id);
}

void ThreadPoolManager::trackAndForwardSignals(TaskRunnable *r)
{
    connect(r, &TaskRunnable::started, this, &ThreadPoolManager::taskStarted, Qt::QueuedConnection);
    connect(r, &TaskRunnable::progress, this, &ThreadPoolManager::taskProgress, Qt::QueuedConnection);
    connect(r, &TaskRunnable::finished, this, &ThreadPoolManager::taskFinished, Qt::QueuedConnection);
    connect(r, &TaskRunnable::failed, this, &ThreadPoolManager::taskFailed, Qt::QueuedConnection);
}
