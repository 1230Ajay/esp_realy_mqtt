
#include <Arduino.h>
#include <string.h>

#ifndef PERIODIC_TASK_MANAGER_H
#define PERIODIC_TASK_MANAGER_H

#define MAX_TASKS 5
#define MAX_ID_LENGTH 32

class PeriodicTaskManager
{
public:
    PeriodicTaskManager();

    void registerTask(const char *id, void (*callback)(), unsigned long interval);
    void run();
    void unregisterTask(const char *id);
    bool isTaskRegistered(const char *id);

private:
    struct Task
    {
        char id[MAX_ID_LENGTH];
        void (*callback)();
        unsigned long interval;
        unsigned long lastRun;
        bool active;
    };

    Task tasks[MAX_TASKS];
    int taskCount;
};
#endif