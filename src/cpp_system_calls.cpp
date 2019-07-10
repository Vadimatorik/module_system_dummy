#include "cpp_system_calls.h"

#ifdef MODULE_CPP_SYSTEM_CALLS_DUMMY_ENABLED

#undef errno

extern int errno;

extern uint32_t __bss_end;    // Конечный адрес BSS области.
// (По этому адресу записывать уже не нужно (проверяется по <)).

//*********************************************************************
// Функции, необходимые для поддержания работы бибилиотеки libc.a.
//*********************************************************************
extern "C" {

void HAL_NVIC_SystemReset (void);

//*********************************************************************
// Заглушка для сборки. После сборки заменяется соответствующей
// функцией из startup файла
//*********************************************************************
void __attribute__ ((weak)) _init (void) {
}

//**********************************************************************
// Метод вызывается при завершении программы
// ( вызывается перезагрузка чипа ).
// Вызвать ее могут exit, abort.
//**********************************************************************
void __attribute__ ((weak)) _exit (int) {
    HAL_NVIC_SystemReset();
    while (1);
}

void __attribute__ ((weak)) _abort (void) {
    HAL_NVIC_SystemReset();
}

//**********************************************************************
// Убиваем процесс ( заглушка ).
//**********************************************************************
int __attribute__ ((weak)) _kill (int pid, int sig) {
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

//**********************************************************************
// Закрываем файл ( заглушка ).
//**********************************************************************
int __attribute__ ((weak)) _close (void) {
    return -1;
}

//**********************************************************************
// Если имеется системный терминал, выводить в него данные.
//**********************************************************************
int __attribute__ ((weak)) _write (char *buffer, uint32_t count) {
    (void)buffer;
    (void)count;
    // Дописать поддержку терминала!
    return count;
}

//*********************************************************************
// Если имеется системный терминал, то считать из него данные.
//*********************************************************************
int __attribute__ ((weak)) _read (char *ptr, int len) {
    (void)ptr;
    (void)len;
    // Реализовать считывание!
    return 0;
}

//**********************************************************************
// Возвращаем тип "файла"
// ( нашего терминала, если испольуется ).
//**********************************************************************
int __attribute__ ((weak)) _fstat (struct stat *st) {
    st->st_mode = S_IFCHR;                      // Символно-ореинтированный "файл".
    return 0;
}

//**********************************************************************
// Установка позиции в файле.
// У нас используется ( если разрешен )
// терминал.
// Не используется ( заглушка ).
//**********************************************************************
int __attribute__ ((weak)) _lseek (void) {
    return 0;
}

//**********************************************************************
// Используется для уточнения, является ли
// файл терминалом.
//**********************************************************************
int __attribute__ ((weak)) _isatty (int file) {
    (void)file;
    return 1;
}

//**********************************************************************
// Возвращает ID процесса.
// Не используется ( заглушка ).
//**********************************************************************
int __attribute__ ((weak)) _getpid (void) {
    return 1;
}

//**********************************************************************
// Передача управления новому процессу.
// Процессов нет -> возвращаем ошибку.
//**********************************************************************
int __attribute__ ((weak)) _execve (char *name, char **argv, char **env) {
    (void)name;
    (void)argv;
    (void)env;
    errno = ENOMEM;
    return -1;
}

//**********************************************************************
// fork - создание нового процесса.
// Мы их не поддерживаем.
//**********************************************************************
int __attribute__ ((weak)) _fork (void) {
    errno = EAGAIN;
    return -1;
}

//**********************************************************************
// times - временная информация о процессе
// ( сколько тиков: системных, процессорных и т.д. )
// ( Заглушка ).
//**********************************************************************

clock_t __attribute__ ((weak)) _times (struct tms *buf) {
    (void)buf;
    return -1;
}

//*********************************************************************
// Удаление имени файла. Не используется.
//*********************************************************************

int __attribute__ ((weak)) _unlink (char *name) {
    (void)name;
    errno = ENOENT;
    return -1;
}

//**********************************************************************
// Ожидание дочерних процессов.
// ( Не используется ).
//**********************************************************************
int __attribute__ ((weak)) _wait (int *status) {
    (void)status;
    errno = ECHILD;
    return -1;
}

//**********************************************************************
// Проверяем, что наши данные ( которые malloc и прочие могут
// попробовать запросить ) не наложатся на стек.
//**********************************************************************
void *__attribute__ ((weak)) _sbrk (int incr) {
    extern char __heap_start;
    extern char __heap_end;

    static char *heap_end;
    char *prev_heap_end;

    if (0 == heap_end) {
        heap_end = &__heap_start;            /* Initialize first time round */
    }

    prev_heap_end = heap_end;
    heap_end += incr;
    //check
    if (heap_end < (&__heap_end)) {

    } else {
        errno = ENOMEM;
        return (char *)-1;
    }
    return (void *)prev_heap_end;
}

}

#endif
