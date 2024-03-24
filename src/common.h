#ifndef LAB3_COMMON_H
#define LAB3_COMMON_H

#include <chrono>
#include <memory>

#include <iostream>
#include <iomanip>
#include <cstring>

// Section 'Constants'

constexpr int64_t max_int64 = std::numeric_limits<int64_t>::max();
constexpr uint32_t max_uint32 = std::numeric_limits<uint32_t>::max();

const char *const arg_err_message = "Illegal arguments list:\n"
                                    "$1 - Array size.\n"
                                    "$2 - Maximum absolute value for array element.\n"
                                    "$3 - Number of threads to run asynchronous code.\n";

const char *const arg_err_message_detailed = "Wrong arguments specified.\n"
                                             "$1 - Array size (1 - 4'294'967'295).\n"
                                             "$2 - Array element abs (1 - 1'000'000).\n"
                                             "$3 - Number of threads (1 - 512).\n";

// End Section 'Constants'
// Section 'Types'

struct AppArgs {
    int64_t array_size, elm_limit_abs, threads_num;
};

struct TaskResult {
    int64_t min_divisible_by_11;
    int64_t sum_of_divisible_by_11;
    std::chrono::duration<int64_t, std::milli> execution_time;

    static constexpr TaskResult default_value() {
        return {
                -1,
                -1,
                std::chrono::milliseconds(-1)
        };
    };
};

// End Section 'Types'
// Section 'Business Logic'

namespace single {
    TaskResult run(const std::shared_ptr<int32_t[]> &array, uint32_t array_size);
}

namespace mutex {
    TaskResult run(const std::shared_ptr<int32_t[]> &array, uint32_t array_size, uint32_t threads_num);
}

namespace atomic {
    TaskResult run(const std::shared_ptr<int32_t[]> &array, uint32_t array_size, uint32_t threads_num);
}

// End Section 'Business Logic'
// Section 'Functions'

template<typename FT>
std::chrono::duration<int64_t, std::milli>
measure_execution_time(FT func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

std::shared_ptr<int32_t[]> initialize_array(uint32_t threads_used, uint32_t size, uint32_t elm_limit_abs);

void print_result(const char *execution_type, const TaskResult &result);

int process_args(int argc, char **argv, AppArgs *out_args);

// End Section 'Functions'

#endif //LAB3_COMMON_H
