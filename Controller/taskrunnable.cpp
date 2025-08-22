#include "taskrunnable.h"

TaskRunnable::TaskRunnable(QString id, WorkFn work,QObject *parent)
    : QObject{parent}, m_id(std::move(id)), m_work(std::move(work))
{
    // 交给线程池自动回收
    setAutoDelete(true);
}

QString TaskRunnable::id() const
{
    return m_id;
}

void TaskRunnable::cancel()
{
    m_cancelled.store(true, std::memory_order_relaxed);
}

bool TaskRunnable::isCancelled() const
{
    return m_cancelled.load(std::memory_order_relaxed);
}

// 任务内部汇报进度时调用（线程内调用，跨线程自动排队到接收者）
void TaskRunnable::reportProgress(int percent)
{
    emit progress(m_id, percent);
}

void TaskRunnable::run()
{
    emit started(m_id);
    if (isCancelled())
    {
        emit finished(m_id);
        return;
    }

    try
    {
        if (m_work)
        {
            m_work(this);
        }
        emit finished(m_id);
    }
    catch (const std::exception& e)
    {
        emit failed(m_id, QString::fromUtf8(e.what()));
    }
    catch (...)
    {
        emit failed(m_id, QStringLiteral("unknown exception"));
    }
}
