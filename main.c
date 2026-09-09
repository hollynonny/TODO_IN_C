#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct {
    int id;
    char title[25];
    char content[150];
    bool completed;
} Task;

static void add(Task **task_save, int *size, int *capacity);
static void push(Task **task_save, int *size, int *capacity, Task new_task);

static void delete(Task **task_save, int *size, int *capacity);
static void pop(Task **task_save, int *size, int *capacity, int index);

static void look(Task **task_save, const int *size);

static void edit(Task **task_save, const int *size);

static void save(Task **task_save, const int *size);
static void load(Task **task_save, int *size, int *capacity);

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    int capacity = 2;
    int size = 0;
    Task *task_save = malloc(capacity * sizeof(Task));

    printf("Приветствую тебя в блокнотике задач, НАХУЙ!\n");

    load(&task_save, &size, &capacity);

    bool run = true;
    while (run) {
        printf("y/Y. Соснуть\ne/E. Отредачить срань\nl/L. Пасареть чё там насрано\nd/D. Удалить нахуй хуйню что высрал\nНапиши свой выбор: ");
        const char input = (char)getchar();

        switch (input) {
            case 'y':
            case 'Y':
                add(&task_save, &size, &capacity);
                save(&task_save, &size);
                break;
            case 'e':
            case 'E':
                edit(&task_save, &size);
                save(&task_save, &size);
                break;
            case 'l':
            case 'L':
                look(&task_save, &size);
                break;
            case 'd':
            case 'D':
                delete(&task_save, &size, &capacity);
                save(&task_save, &size);
                break;
            default:
                printf("Пашол нахуй, сын шалавы ёбаной!\n");
                while (getchar() != '\n') {}
                break;
        }
    }

    free(task_save);

    return 0;
}

static void add(Task **task_save, int *size, int *capacity) {
    while (getchar() != '\n') {}

    char title_buffer[27];
    char content_buffer[152];

    printf("Название блиять: ");
    if (fgets(title_buffer, sizeof(title_buffer), stdin) != NULL) {
        title_buffer[strcspn(title_buffer, "\n")] = '\0';
    }

    printf("Шо там хочш видеть, Говна Кусок: ");
    if (fgets(content_buffer, sizeof(content_buffer), stdin) != NULL) {
        content_buffer[strcspn(content_buffer, "\n")] = '\0';
    }

    Task task;
    task.id = *size + 1;
    task.completed = false;

    strcpy(task.title, title_buffer);
    strcpy(task.content, content_buffer);

    push(task_save, size, capacity, task);
}

static void edit(Task **task_save, const int *size) {
    printf("Кого пробить: ");
    int index;
    if (scanf("%d", &index) == 1) {
        printf("[TODO]\tInd -> %d\n", index);
    }

    if (index < 0 || index >= *size) {
        printf("Я тебе жопу пробью, если будешь такую хуйню писать!\n");
        while (getchar() != '\n') {}
        return;
    }

    char title_buffer[27];
    char content_buffer[152];

    printf("Напиши новое имя: ");
    if (fgets(title_buffer, sizeof(title_buffer), stdin) != NULL) {
        title_buffer[strcspn(title_buffer, "\n")] = '\0';
    }

    while (getchar() != '\n') {}

    printf("\nНапиши новое описание: ");
    if (fgets(content_buffer, sizeof(content_buffer), stdin) != NULL) {
        content_buffer[strcspn(content_buffer, "\n")] = '\0';
    }

    Task task;
    task.id = index + 1;
    task.completed = false;

    strcpy(task.title, title_buffer);
    strcpy(task.content, content_buffer);

    (*task_save)[index] = task;
}

static void look(Task **task_save, const int *size) {
    printf("Обосранный лист: \n");
    for (int i = 0; i < *size; i++) {
        printf("%d.(i: %d) -> Название: %s\tВыполнено: %s\nОписание: %s\n\n",
            (*task_save)[i].id, i, (*task_save)[i].title, (*task_save)[i].completed ? "выполнено" : "невыполнено", (*task_save)[i].content);
    }
}

static void delete(Task **task_save, int *size, int *capacity) {
    while (getchar() != '\n') {}

    printf("Напиши каво пробить: ");
    int index;
    if (scanf("%d", &index) == 1) {
        printf("[TODO]\tInd -> %d", index);
    }

    while (getchar() != '\n') {}

    pop(task_save, size, capacity, index);
}

static void save(Task **task_save, const int *size) {
    FILE *file = fopen("tasks.json", "w");
    if (file == NULL) {
        printf("[TODO]\tCouldn't load the file: tasks.json\n");
        return;
    }

    fprintf(file, "[\n");

    for (int i = 0;i < *size; i++) {
        fprintf(file, "\t{\n");
        fprintf(file, "\t\t\"id\": %d,\n", (*task_save)[i].id);
        fprintf(file, "\t\t\"title\": \"%s\",\n", (*task_save)[i].title);
        fprintf(file, "\t\t\"content\": \"%s\",\n", (*task_save)[i].content);
        fprintf(file, "\t\t\"completed\": \"%s\"\n", (*task_save)[i].completed ? "true" : "false");

        if (i < *size - 1) {
            fprintf(file, "\t},\n");
        }
        else {
            fprintf(file, "\t}\n");
        }
    }

    fprintf(file, "]\n");
    fclose(file);

    printf("[TODO]\tData saved!\n");
}

static void load(Task **task_save, int *size, int *capacity) {
    FILE *file = fopen("tasks.json", "r");
    if (file == NULL) {
        printf("[TODO]\tCouldn't load the file: tasks.json\n");
        return;
    }

    Task temp_task;
    char bool_str[10];

    fscanf(file, " [");

    while (1) {
        memset(&temp_task, 0, sizeof(Task));

        if (fscanf(file, " { \"id\": %d,", &temp_task.id) != 1) {
            break;
        }

        fscanf(file, " \"title\": \"%26[^\"]\",", temp_task.title);
        fscanf(file, " \"content\": \"%150[^\"]\",", temp_task.content);

        fscanf(file, " \"completed\": \"%9[^\"]\"", bool_str);
        temp_task.completed = (strcmp(bool_str, "true") == 0);

        fscanf(file, " },");

        push(task_save, size, capacity, temp_task);
    }

    fclose(file);

    if (*size == 0) {
        printf("[TODO]\tThere's no tasks in tasks.json, nothing to load!\n");
    }
    else {
        printf("[TODO]\tData loaded!\n");
    }
}

static void push(Task **task_save, int *size, int *capacity, const Task new_task) {
    if (*size >= *capacity - 1) {
        *capacity += 5;
        Task *temp_task_save = realloc(*task_save, (*capacity) * sizeof(Task));
        if (temp_task_save != NULL) {
            *task_save = temp_task_save;
        }
    }
    (*task_save)[*size] = new_task;

    (*size)++;
}

static void pop(Task **task_save, int *size, int *capacity, const int index) {
    if (index < 0 || *size <= index) {
        printf("Иди НАХУЙ, пидор!\n");
        return;
    }

    for (int i = index; i < *size - 1; i++) {
        (*task_save)[i] = *task_save[i + 1];
    }
    (*size)--;

    if (*size < *capacity - 5 && *capacity - 5 > 0) {
        *capacity -= 5;
        Task *temp_task_save = realloc(*task_save, (*capacity) * sizeof(Task));
        if (temp_task_save != NULL) {
            *task_save = temp_task_save;
        }
        printf("Приузил твоё очко, НАХУЙ!\n");
    }

    printf("Разобрался с этой шавкой, НАХУЙ!\n");
}