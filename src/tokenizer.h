#ifndef EBLANG_TOKENIZER_tokenizer_h
#define EBLANG_TOKENIZER_tokenizer_h
/**
 * @file tokenizer.h
 */

#include <stdio.h>
#include <stdlib.h>

#include "mem_util.h"
#include "token.h"
#include "result.h"

/**
 * Wrapper structure to parse token into
 */
struct token_context_t {
    size_t size; /* token counts */
    size_t capacity;
    struct token_t* tokens;
};

ResultDec(struct token_context_t*, char*, tokenizer);
ResultDec(size_t, char*, token);
VoidResultDec(char*, tokenizer);

static struct void_tokenizer_result_t expend(struct token_context_t *ctx) {
    ctx->capacity *= 3;
    struct token_t* reallocated = realloc(ctx->tokens, sizeof(struct token_t) * ctx->capacity);
    if (reallocated == nullptr) {
        free(ctx->tokens);
        return void_tokenizer_err("Out of memory");
    }
    ctx->tokens = reallocated;
    return void_tokenizer_ok();
}

static struct token_result_t parse_ident(struct token_context_t *ctx, const char *ctnt, size_t start, size_t cursor) {
    if (ctx == nullptr) {
        return token_err("Context cannot be null");
    }
    if (ctx->tokens == nullptr) {
        return token_err("Token pointer in context cannot be null");
    }
    if (ctx->size >= ctx->capacity) {
        struct void_tokenizer_result_t result = expend(ctx);
        if (result.is_err) {
            return token_err(result.data.error);
        }
    }
    char* str = slice(ctnt, start, cursor);
    ctx->tokens[ctx->size] = create_ident(str);
    ctx->size++;
    return token_ok(cursor + 1);
}

static struct token_result_t parse_token(struct token_context_t *ctx, const char *ctnt, size_t start, size_t cursor) {
    if (ctx->tokens == nullptr) {
        return token_err("Token pointer in context cannot be null");
    }
    if (start < cursor) {
        struct token_result_t result = parse_ident(ctx, ctnt, start, cursor);
        if (result.is_err) {
            return token_err(result.data.error);
        }
    }
    if (ctx->size >= ctx->capacity) {
        struct void_tokenizer_result_t result = expend(ctx);
        if (result.is_err) {
            return token_err(result.data.error);
        }
    }
    ctx->tokens[ctx->size] = create_common(ctnt[cursor]);
    ctx->size++;
    return token_ok(cursor + 1);
}

/**
 * Parse tokens for a string
 *
 * @param ctx Context to the tokenizer
 * @param ctnt String to parse tokens
 * @param ctnt_size Size of the string
 * @throw Assertion when @ref TokenContext ctx is nullptr
 */
static struct void_tokenizer_result_t tokenize(struct token_context_t *ctx, const char *ctnt, size_t ctnt_size) {
    if (ctnt == nullptr) {
        return void_tokenizer_err("tokenize: Null is not parsable");
    }
    if (ctx == nullptr) {
        return void_tokenizer_err("tokenize: Tokenizer must not null");
    }

    size_t start = 0;
    size_t cursor = 0;
    while (cursor < ctnt_size) {
        struct token_result_t token_result;
        switch (ctnt[cursor]) {
            case SemiColon:
            case Colon:
            case Comma:
            case Period:
            case Exclamation:
            case Question:
            case Equal:
            case Plus:
            case Minus:
            case Star:
            case Slash:
            case Percent:
            case At:
            case Dollar:
            case OpenParen:
            case CloseParen:
            case OpenBracket:
            case CloseBracket:
            case OpenBrace:
            case CloseBrace:
                token_result = parse_token(ctx, ctnt, start, cursor);
                if (token_result.is_err) {
                    return void_tokenizer_err(token_result.data.error);
                }
                start = token_result.data.value;
                break;
            case '"':
                cursor++;
                start = cursor;
                bool skip = false;
                while (cursor < ctnt_size) {
                    if (ctnt[cursor] == '\\' || skip) {
                        skip = !skip;
                    } else if (ctnt[cursor] == '"') {
                        break;
                    }
                    cursor++;
                }
                ctx->tokens[ctx->size] = create_str(slice(ctnt, start, cursor));
                ctx->size++;
                start = cursor + 1;
                break;

            // Whitespace
            case '\r':
            case '\t':
            case '\n':
            case ' ':
                if (start < cursor) {
                    token_result = parse_ident(ctx, ctnt, start, cursor);
                    if (token_result.is_err) {
                        return void_tokenizer_err(token_result.data.error);
                    }
                }
                start = cursor + 1;
                break;

            default:
                break;
        }
        cursor++;
    }
    if (start < cursor) {
        struct token_result_t ident_result = parse_ident(ctx, ctnt, start, cursor);
        if (ident_result.is_err) {
            return void_tokenizer_err(ident_result.data.error);
        }
        start = ident_result.data.value;
    }
    if (start <= cursor) {
        printf("%lli / %lli\n", start, cursor);
        return void_tokenizer_err("tokenize: start must be bigger than cursor");
    }
    return void_tokenizer_ok();
}

/**
 * Create a heap allocated TokenContext
 *
 * Note: Ptr ownership is to the caller
 *
 * @param tokens_capacity Initial capacity, it will grow and slice at the end
 * @return The ptr to the created
 */
static struct tokenizer_result_t create_token_ctx(const size_t tokens_capacity) {
    if (tokens_capacity < 1) {
        return tokenizer_err("Initial token capacity must be higher than 1");
    }
    struct token_context_t* result = malloc(sizeof(struct token_context_t));
    if (result == nullptr) {
        return tokenizer_err("Out of memory");
    }
    result->tokens = malloc(sizeof(struct token_t) * tokens_capacity);
    result->size = 0;
    result->capacity = tokens_capacity;
    return tokenizer_ok(result);
}

static void destroy_token_ctx(struct token_context_t* tokens_result) {
    for (int i = 0; i < tokens_result->size; i++) {
        if (tokens_result->tokens[i].type == Identifier) {
            free((void*)tokens_result->tokens[i].data.identifier);
        }
    }
    free(tokens_result->tokens);
    tokens_result->tokens = nullptr;
    free(tokens_result);
}

static void token_list_print(const struct token_context_t *ctx) {
    printf("TokenList {\n");
    printf("\tsize: %llu,\n", ctx->size);
    printf("\ttoken: [\n");
    for (int i = 0; i < ctx->size; i++) {
        printf("\t\t%d: ", i);
        print_token(ctx->tokens[i]);
        printf(",\n");
    }
    printf("\t]\n");
    printf("}\n");
}

#endif
