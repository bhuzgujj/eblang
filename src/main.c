#include <string.h>

#include "tokenizer.h"

const char* str = "LOL,ddd ed ; sess \" \\\" jujj \"  a";

int main(int argument_count, char *arguments[]) {
    struct tokenizer_result_t ctx = create_token_ctx(1);
    if (ctx.is_err) {
        printf("Error: %s", ctx.data.error);
        exit(1);
    }
    struct void_tokenizer_result_t tokenizer_result = tokenize(ctx.data.value, str, strlen(str));
    if (tokenizer_result.is_err) {
        printf("Error: %s", tokenizer_result.data.error);
    } else{
        token_list_print(ctx.data.value);
    }
    destroy_token_ctx(ctx.data.value);
    return 0;
}
