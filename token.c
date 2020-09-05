#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "9cc.h"


// 入力プログラム
char *user_input;

Token *token;

// 新しいトークンを作成して cur に繋げる
Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

// 次のトークンが期待している記号のときには､ トークンを1つ読み進めて
// 真を返す｡ それ以外の場合には偽を返す｡
bool consume(char *op) {
    if (token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
        return false;
    token = token->next;
    return true;
}

// 次のトークンが期待している記号のときには､ トークンを1つ読み進める｡
// それ以外の場合にはエラーを報告する｡
void expect(char *op) {
    if (token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len)) 
        error_at(token->str, "'%c'ではありません", op);
    token = token->next;
}


// 次のトークンが数値の場合､ トークンを1つ読み進めてその数値を返す｡
// それ以外の場合にはエラーを報告する｡
int expect_number() {
    if (token->kind != TK_NUM)
        error_at(token->str, "数ではありません");
    int val = token->val;
    token = token->next;
    return val;
}

// エラーを表示する関数
// printf と引数が同じ
void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// エラー箇所を表示する関数
void error_at(char *loc, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, "");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}


// 入力文字列 p をトークナイズしてそれを返す
Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (!strncmp(p, "<=", 2) || !strncmp(p, "==", 2) || !strncmp(p, ">=", 2) || !strncmp(p, "!=", 2)) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;

            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')' || *p == '<' || *p == '>') {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            // p++; 後置インクリメントは new_token 実行後にインクリメントの処理が入る
            
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 1);
            // strtolは base で変換できない文字までポインタを進める
            cur->val = strtol(p, &p, 10);
            continue;
        }

        error("トークナイズできません");
    }

    new_token(TK_EOF, cur, p, 1);
    return head.next;
}
