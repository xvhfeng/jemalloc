#include "test/jemalloc_test.h"
#include "test/bench.h"

static void
malloc_free(void) {
	void *p = malloc(1);
	expect_ptr_not_null(p, "Unexpected malloc failure");
	free(p);
}

static void
new_delete(void) {
	auto p = ::operator new(1);
	expect_ptr_not_null(p, "Unexpected new failure");
	::operator delete(p);
}

static void
malloc_free_array(void) {
	void *p = malloc(sizeof(int)*8);
	expect_ptr_not_null(p, "Unexpected malloc failure");
	free(p);
}

static void
new_delete_array(void) {
	auto p = new int[8];
	expect_ptr_not_null(p, "Unexpected new[] failure");
	delete[] p;
}

#if __cpp_sized_deallocation >= 201309
static void
new_sized_delete(void) {
	auto p = ::operator new(1);
	expect_ptr_not_null(p, "Unexpected new failure");
	::operator delete(p, 1);
}

static void
malloc_sdallocx(void) {
	void *p = malloc(1);
	expect_ptr_not_null(p, "Unexpected malloc failure");
        sdallocx(p, 1, 0);
}
#endif

TEST_BEGIN(test_free_vs_delete) {
	compare_funcs(10*1000*1000, 100*1000*1000, 
	    "malloc_free", (void *)malloc_free, 
	    "new_delete", (void *)new_delete);
}
TEST_END

TEST_BEGIN(test_free_array_vs_delete_array) {
	compare_funcs(10*1000*1000, 100*1000*1000, 
	    "malloc_free_array", (void *)malloc_free_array, 
	    "delete_array", (void *)new_delete_array);
}
TEST_END


TEST_BEGIN(test_sized_delete_vs_sdallocx) {
#if __cpp_sized_deallocation >= 201309
	compare_funcs(10*1000*1000, 100*1000*1000, 
	    "new_size_delete", (void *)new_sized_delete, 
	    "malloc_sdallocx", (void *)malloc_sdallocx);
#else
	malloc_printf("Skipping test_sized_delete_vs_sdallocx since \
	    sized deallocation is not enabled.\n");
#endif
}
TEST_END


int
main() {
	return test_no_reentrancy(
	    test_free_vs_delete,
	    test_free_array_vs_delete_array,
	    test_sized_delete_vs_sdallocx);
}
