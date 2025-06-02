
#include <TaskManager.h>

PeriodicTaskManager::PeriodicTaskManager()
{
    taskCount = 0;
}

void PeriodicTaskManager::registerTask(const char *id, void (*callback)(), unsigned long interval)
{
    for (int i = 0; i < taskCount; i++)
    {
        if (strcmp(tasks[i].id, id) == 0)
        {
            Serial.printf("Task %s already registered\n", id);
            return;
        }
    }

    if (taskCount < MAX_TASKS)
    {
        strncpy(tasks[taskCount].id, id, MAX_ID_LENGTH - 1);
        tasks[taskCount].id[MAX_ID_LENGTH - 1] = '\0';
        tasks[taskCount].callback = callback;
        tasks[taskCount].interval = interval;
        tasks[taskCount].lastRun = millis();
        tasks[taskCount].active = true;
        taskCount++;
        Serial.printf("Task %s registered. Total tasks: %d\n", id, taskCount);
    }
    else
    {
        Serial.println("Task limit reached!");
    }
}

void PeriodicTaskManager::run()
{
    unsigned long currentMillis = millis();

    for (int i = 0; i < taskCount; i++)
    {
        if (tasks[i].active && currentMillis - tasks[i].lastRun >= tasks[i].interval)
        {
            tasks[i].lastRun = currentMillis;
            tasks[i].callback();
        }
    }
}

void PeriodicTaskManager::unregisterTask(const char *id)
{
    for (int i = 0; i < taskCount; i++)
    {
        if (strcmp(tasks[i].id, id) == 0)
        {
            for (int j = i; j < taskCount - 1; j++)
            {
                tasks[j] = tasks[j + 1];
            }
            taskCount--; 
            break;     
        }
    }
}


bool PeriodicTaskManager::isTaskRegistered(const char *id)
{
    for (int i = 0; i < taskCount; i++)
    {
        if (strcmp(tasks[i].id, id) == 0 && tasks[i].active)
        {
            return true;
        }
    }
    return false;
}


