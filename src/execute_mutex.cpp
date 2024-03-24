#include "common.h"

#include <mutex>
#include <shared_mutex>

#include <vector>
#include <thread>

using my_lock = std::shared_mutex;
using lock_guard = std::unique_lock<my_lock>;

class MutexRunner {
private:
    int64_t min_divisible = max_int64;
    int64_t sum_of_divisible = 0;

    const int32_t *array;
    uint32_t array_size;
    uint32_t threads_num;

    my_lock lock{};

public:

    MutexRunner(const int32_t *array, uint32_t array_size, uint32_t threads_num) {
        this->array = array;
        this->array_size = array_size;
        this->threads_num = threads_num;
    }

    ~MutexRunner() = default;

    void start_task() {
        this->reset_stats();

        std::vector<std::thread> threads{};
        threads.reserve(this->threads_num);

        size_t one_segment = this->array_size / this->threads_num;
        size_t left_cells = this->array_size - one_segment * this->threads_num;

        size_t low_idx = 0;
        size_t high_idx = one_segment - 1;

        if (left_cells) {
            high_idx++;
            left_cells--;
        }

        for (size_t i = 0; i < this->threads_num; i++) {
            threads.emplace_back(&MutexRunner::task_for_thread, this, low_idx, high_idx);

            low_idx = high_idx + 1;
            high_idx = low_idx + one_segment - 1;

            if (left_cells) {
                high_idx++;
                left_cells--;
            }
        }

        for (auto &thread: threads) thread.join();
    }

    [[nodiscard]] int64_t get_min() const {
        return this->min_divisible;
    }

    [[nodiscard]] int64_t get_sum() const {
        return this->sum_of_divisible;
    }

private:

    void task_for_thread(uint32_t low_idx, uint32_t high_idx) {
        for (uint32_t i = low_idx; i <= high_idx; i++) {
            auto &elm = this->array[i];
            if (elm % 11 == 0) {
                {
                    lock_guard _(this->lock);
                    if (elm < this->min_divisible) {
                        this->min_divisible = elm;
                    }

                    this->sum_of_divisible += elm;
                }
            }
        }
    }

    void reset_stats() {
        this->min_divisible = max_int64;
        this->sum_of_divisible = 0;
    }

};

TaskResult mutex::run(const std::shared_ptr<int32_t[]> &array, uint32_t array_size, uint32_t threads_num) {
    MutexRunner runner(array.get(), array_size, threads_num);

    auto execution_time = measure_execution_time([&]() { runner.start_task(); });

    return {
            runner.get_min(),
            runner.get_sum(),
            execution_time
    };
}
