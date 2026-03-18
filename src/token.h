#ifndef EBLANG_TOKENIZER_token_h
#define EBLANG_TOKENIZER_token_h

/**
 * Single character Token
 */
enum CharacterToken {
    SemiColon = ';',
    Colon = ':',
    Comma = ',',
    Period = '.',
    Exclamation = '!',
    Question = '?',

    Equal = '=',
    Plus = '+',
    Minus = '-',
    Star = '*',
    Slash = '/',

    Percent = '%',
    At = '@',
    Dollar = '$',

    OpenParen = '(',
    CloseParen = ')',
    OpenBracket = '[',
    CloseBracket = ']',
    OpenBrace = '{',
    CloseBrace = '}',
};

/**
 * Token Identifier
 */
enum TokenType {
    Identifier,
    ConstantToken,
    StrToken
};

/**
 * Container for the data of the Token
 */
union TokenData {
    const char* identifier;
    enum CharacterToken character;
    const char* str;
};

/**
 * Token from a string
 */
struct token_t {
    enum TokenType type;
    union TokenData data;
};

static struct token_t create_ident(const char* ident) {
    return (struct token_t){
        .type = Identifier,
        .data.identifier = ident
    };
}

static struct token_t create_common(const enum CharacterToken common) {
    return (struct token_t){
        .type = ConstantToken,
        .data.character = common
    };
}

static struct token_t create_str(const char* str) {
    return (struct token_t){
        .type = StrToken,
        .data.str = str
    };
}

static void print_token(const struct token_t token) {
    switch (token.type) {
        case ConstantToken:
            printf("Const '%c'", token.data.character);
            break;
        case Identifier:
            printf("Ident '%s'", token.data.identifier);
            break;
        case StrToken:
            printf("Str '%s'", token.data.str);
            break;
        default:
            printf("<Unknown>");
            break;
    }
}

#endif
