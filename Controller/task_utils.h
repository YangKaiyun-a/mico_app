// 任务功能函数库（集中放常用任务逻辑）

#ifndef TASK_UTILS_H
#define TASK_UTILS_H


#include <QString>

class TaskRunnable;

namespace TaskUtils {

void exec(TaskRunnable* self);

}



#endif // TASK_UTILS_H
