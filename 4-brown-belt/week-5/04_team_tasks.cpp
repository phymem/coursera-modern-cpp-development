#include <unordered_map>
#include <iostream>
#include <tuple>
#include <map>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
	NEW,          // новая
	IN_PROGRESS,  // в разработке
	TESTING,      // на тестировании
	DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:

	// Получить статистику по статусам задач конкретного разработчика
	const TasksInfo& GetPersonTasksInfo(const string& person) const {
		return tasks.at(person);
	}

	// Добавить новую задачу (в статусе NEW) для конкретного разработчитка
	void AddNewTask(const string& person) {
		tasks[person][TaskStatus::NEW]++;
	}

	// Обновить статусы по данному количеству задач конкретного разработчика,
	// подробности см. ниже
	tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
		TasksInfo& entry = tasks[person];

		TasksInfo untouched;
		TasksInfo updated;
		int to_update = 0;

		for (TaskStatus status = TaskStatus::NEW; status != TaskStatus::DONE; status = Next(status)) {

			int count = 0;
			if (auto it = entry.find(status); it != entry.end()) {
				count = min(task_count, it->second);
				task_count -= count;
				it->second -= count;

				if (it->second)
					untouched[status] = it->second;
				else
					entry.erase(it);
			}

			if (to_update) {
				updated[status] = to_update;
				entry[status] += to_update;
			}
			to_update = count;
		}

		if (to_update) {
			updated[TaskStatus::DONE] = to_update;
			entry[TaskStatus::DONE] += to_update;
		}

		return make_tuple(updated, untouched);
	}

private:

	TaskStatus Next(TaskStatus status) const {
		return static_cast<TaskStatus>(static_cast<int>(status) + 1);
	}

private:

	unordered_map<string, TasksInfo> tasks;
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
	cout << tasks_info[TaskStatus::NEW] << " new tasks" << ", "
		<< tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" << ", "
		<< tasks_info[TaskStatus::TESTING] << " tasks are being tested" << ", "
		<< tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

int main() {
	TeamTasks tasks;
	tasks.AddNewTask("Ilia");
	for (int i = 0; i < 3; ++i) {
		tasks.AddNewTask("Ivan");
	}
	cout << "Ilia's tasks: ";
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
	cout << "Ivan's tasks: ";
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

	TasksInfo updated_tasks, untouched_tasks;

	tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
	cout << "Updated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);

	tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
	cout << "Updated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);

	return 0;
}
