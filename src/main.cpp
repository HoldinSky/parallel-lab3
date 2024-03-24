#include "common.h"

int main(int argc, char **argv) {
    AppArgs args{};

    if (process_args(argc, argv, &args)) {
        exit(-1);
    }

    auto array = initialize_array(args.threads_num, args.array_size, args.elm_limit_abs);
    if (!array) {
        return -1;
    }

    auto result = single::run(array, args.array_size);
    print_result("Single", result);

    result = mutex::run(array, args.array_size, args.threads_num);
    print_result("Mutex", result);

    result = atomic::run(array, args.array_size, args.threads_num);
    print_result("Atomic", result);

    return 0;
}
