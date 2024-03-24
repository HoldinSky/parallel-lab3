#include "common.h"

#include <random>
#include <thread>
#include <iostream>

static std::random_device seed;

void fill_array(
        std::shared_ptr<int32_t[]> &array,
        size_t low_idx,
        size_t high_idx,
        uint32_t elm_limit_abs
) {
    std::default_random_engine generator(seed());
    std::uniform_int_distribution<int32_t> distribution(-static_cast<int32_t>(elm_limit_abs),
                                                        static_cast<int32_t>(elm_limit_abs));

    auto ptr = array.get();
    auto random_value = [&] { return distribution(generator); };

    for (size_t i = low_idx; i <= high_idx; i++) {
        ptr[i] = random_value();
    }
}

void fill_array_multithreaded(
        std::shared_ptr<int32_t[]> &array,
        size_t size,
        uint32_t elm_limit_abs,
        uint32_t threads_used
) {
    std::vector<std::thread> threads{};
    threads.reserve(threads_used);

    size_t one_segment = size / threads_used;
    size_t left_cells = size - one_segment * threads_used;

    size_t low_idx = 0;
    size_t high_idx = one_segment - 1;

    if (left_cells) {
        high_idx++;
        left_cells--;
    }

    for (size_t i = 0; i < threads_used; i++) {
        threads.emplace_back(fill_array, std::ref(array), low_idx, high_idx, elm_limit_abs);

        low_idx = high_idx + 1;
        high_idx = low_idx + one_segment - 1;

        if (left_cells) {
            high_idx++;
            left_cells--;
        }
    }

    for (auto &thread: threads) thread.join();
}

std::shared_ptr<int32_t[]> initialize_array(uint32_t threads_used, uint32_t size, uint32_t elm_limit_abs) {
    if (elm_limit_abs > std::numeric_limits<int32_t>::max()) {
        std::cerr << "Absolute limit value must fit in 32 bit signed integer\n";
        return nullptr;
    }

    std::shared_ptr<int32_t[]> array = std::shared_ptr<int32_t[]>(new int32_t[size]);

    fill_array_multithreaded(std::ref(array), size, elm_limit_abs, threads_used);

    return array;
}

void print_result(const char *const execution_type, const TaskResult &result) {
    std::cout << std::setw(6) << execution_type << " -- Completed in " << std::setw(5) << result.execution_time.count()
              <<
              "ms. Min divisible = "
              << result.min_divisible_by_11
              << ". Sum of divisible = " << result.sum_of_divisible_by_11 << std::endl;
}

int process_args(int argc, char **argv, AppArgs *out_args) {
    if (argc < 4) {
        std::cout << arg_err_message;
        return -1;
    }

    if (strlen(argv[1]) > 10 || strlen(argv[2]) > 10 || strlen(argv[3]) > 10) {
        std::cout << arg_err_message_detailed;
        return -1;
    }

    out_args->array_size = static_cast<int64_t>(std::strtol(argv[1], nullptr, 0));
    out_args->elm_limit_abs = static_cast<int64_t>(std::strtol(argv[2], nullptr, 0));
    out_args->threads_num = static_cast<int64_t>(std::strtol(argv[3], nullptr, 0));

    if (out_args->array_size <= 0 || out_args->array_size > max_uint32 ||
        out_args->elm_limit_abs <= 0 || out_args->elm_limit_abs > 1'000'000 ||
        out_args->threads_num <= 0 || out_args->threads_num > 512) {
        std::cout << arg_err_message_detailed;
        return -1;
    }

    return 0;
}
