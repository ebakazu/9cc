#include <stdbool.h>

// トークンの種類
typedef enum {
    TK_RESERVED, // 記号
    TK_IDENT,    // 識別子
    TK_NUM, // 整数トークン
    TK_RETURN, // return文
    TK_EOF, // 入力の終わりを示すトークン
} TokenKind;

typedef struct Token Token;
struct Token {
    TokenKind kind; // トークンの型
    Token *next;    // 次の入力トークン
    int val;        // kind が TK_NUM の場合の数値
    char *str;      // トークン文字列
    int len;        // トークンの長さ
};

// 抽象構文木 (AST) のノードの種類
typedef enum {
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /
    ND_EQ, // ==
    ND_NE, // !=
    ND_GT, // >
    ND_LT, // <
    ND_GE, // >=
    ND_LE, // <=
    ND_ASSIGN, // =
    ND_LVAR,   // ローカル変数
    ND_RETURN, // return文
    ND_NUM, // 整数
} NodeKind;

typedef struct Node Node;

// 抽象構文木のノードの型
struct Node {
    NodeKind kind; // ノードの型
    Node *lhs;     // 左辺のノード
    Node *rhs;     // 右辺のノード
    int val;       // ノードの型がND_NUMの場合に使用
    int offset;    // ノードの型がND_LVARの場合に使用 ベースポインタからのオフセット
};

typedef struct LVar LVar;

// ローカル変数
struct LVar {
    LVar *next;  // 次の変数もしくはNULL
    char *name;  // 変数の名前
    int len;     // 変数の名前の長さ
    int offset;  // RBPからのオフセット
};

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
void program();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *primary();
Node *unary();
LVar *find_lvar(Token *tok);
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
bool consume_return();
Token *consume_ident();
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);
void gen_lval(Node *node);
void gen(Node *node);

extern char *user_input;
extern Token *token;
extern Node *code[];
extern char *identifer;
extern LVar *locals;
