static static_sem_s_t sem_test_buffer;

void test_semaphore() {
    sem_t sem_static = sem_create_static(3, 2, &sem_test_buffer);

    sem_wait(sem_static, TIMEOUT_MAX);

    sem_post(sem_static);

    sem_t sem = sem_create(3, 2);

    sem_wait(sem, TIMEOUT_MAX);

    sem_post(sem);
}
