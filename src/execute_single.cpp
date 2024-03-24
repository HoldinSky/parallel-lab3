#include "common.h"

int64_t
single_thread_execution(const std::shared_ptr<int32_t[]> &array, size_t size, int64_t *sum_of_divisible_by_11) {
    int64_t least_divisible_by_11 = max_int64 - max_int64 % 11;

    auto ptr = array.get();

    for (size_t i = 0; i < size; i++) {
        const auto &elm = ptr[i];

        if (elm % 11 == 0) {
            if (elm < least_divisible_by_11) {
                least_divisible_by_11 = elm;
            }

            *sum_of_divisible_by_11 += elm;
        }
    }

    return least_divisible_by_11;
}

TaskResult single::run(const std::shared_ptr<int32_t[]> &array, uint32_t array_size) {
    int64_t minimal;
    int64_t sum_of_divisible{};

    auto execution_time = measure_execution_time([&]() {
        minimal = single_thread_execution(array, array_size, &sum_of_divisible);
    });

    return {
            minimal,
            sum_of_divisible,
            execution_time
    };
}