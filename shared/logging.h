// https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
#define NDEBUG 0

#define DEBUG(fmt, ...) \
        do { if (!NDEBUG) fprintf(stdout, "[DEBUG] %s:%d:%s(): " fmt "\n", __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

#define INFO(fmt, ...) \
        do { fprintf(stdout, "[INFO ] %s:%d:%s(): " fmt "\n", __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)
