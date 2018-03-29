void test_mutex() {
	mutex_t mut = mutex_create();

	mutex_take(mut, TIMEOUT_MAX);

	mutex_give(mut);
}
