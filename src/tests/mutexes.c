static static_sem_s_t mutex_test_buffer;

void test_mutex() {
	mutex_t mut_static = mutex_create_static(&mutex_test_buffer);

    mutex_take(mut_static, TIMEOUT_MAX);

    mutex_give(mut_static);

	mutex_t mut = mutex_create();

	mutex_take(mut, TIMEOUT_MAX);

	mutex_give(mut);
}
