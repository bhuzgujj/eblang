#ifndef EBLANG_RESULT_h
#define EBLANG_RESULT_h

#define ResultDec(value_type, err_type, name) \
    struct name##_result_t { \
        union { \
            value_type value; \
            err_type error; \
        } data; \
        bool is_err; \
    }; \
    static struct name##_result_t name##_err(err_type err) { \
        return (struct name##_result_t) { \
            .data.error = err, \
            .is_err = true \
        }; \
    }; \
    static struct name##_result_t name##_ok(value_type val) { \
        return (struct name##_result_t) { \
            .data.value = val, \
            .is_err = false \
        }; \
    };

#define VoidResultDec(err_type, name) \
    struct void_##name##_result_t { \
        union { \
            void* value; \
            err_type error; \
        } data; \
        bool is_err; \
    }; \
    static struct void_##name##_result_t void_##name##_err(err_type err) { \
        return (struct void_##name##_result_t) { \
            .data.error = err, \
            .is_err = true \
        }; \
    }; \
    static struct void_##name##_result_t void_##name##_ok() { \
        return (struct void_##name##_result_t) { \
            .data.value = nullptr, \
            .is_err = false \
        }; \
    };

#endif