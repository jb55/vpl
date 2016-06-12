#ifndef _ast_h
#define _ast_h

//==============================================================================
//
// Definitions
//
//==============================================================================

// Defines the types of expressions available.
typedef enum vpl_ast_node_type_e {
    VPL_AST_TYPE_NUMBER,
    VPL_AST_TYPE_VARIABLE,
    VPL_AST_TYPE_BINARY_EXPR,
    VPL_AST_TYPE_CALL,
    VPL_AST_TYPE_PROTOTYPE,
    VPL_AST_TYPE_FUNCTION,
    VPL_AST_TYPE_IF_EXPR,
} vpl_ast_node_type_e;

// Defines the types of binary expressions.
typedef enum vpl_ast_binop_e {
    VPL_BINOP_PLUS,
    VPL_BINOP_MINUS,
    VPL_BINOP_MUL,
    VPL_BINOP_DIV,
} vpl_ast_binop_e;


struct vpl_ast_node;

// Represents a number in the AST.
typedef struct vpl_ast_number {
    double value;
} vpl_ast_number;

// Represents a variable in the AST.
typedef struct vpl_ast_variable {
    char *name;
} vpl_ast_variable;

// Represents a binary expression in the AST.
typedef struct vpl_ast_binary_expr {
    vpl_ast_binop_e operator;
    struct vpl_ast_node *lhs;
    struct vpl_ast_node *rhs;
} vpl_ast_binary_expr;

// Represents a function call in the AST.
typedef struct vpl_ast_call {
    char *name;
    struct vpl_ast_node **args;
    unsigned int arg_count;
} vpl_ast_call;

// Represents a function prototype in the AST.
typedef struct vpl_ast_prototype {
    char *name;
    char **args;
    unsigned int arg_count;
} vpl_ast_prototype;

// Represents a function in the AST.
typedef struct vpl_ast_function {
    struct vpl_ast_node *prototype;
    struct vpl_ast_node *body;
} vpl_ast_function;

// Represents an if statement in the AST.
typedef struct vpl_ast_if_expr {
    struct vpl_ast_node *condition;
    struct vpl_ast_node *true_expr;
    struct vpl_ast_node *false_expr;
} vpl_ast_if_expr;

// Represents an expression in the AST.
typedef struct vpl_ast_node {
    vpl_ast_node_type_e type;
    union {
        vpl_ast_number number;
        vpl_ast_variable variable;
        vpl_ast_binary_expr binary_expr;
        vpl_ast_call call;
        vpl_ast_prototype prototype;
        vpl_ast_function function;
        vpl_ast_if_expr if_expr;
    };
} vpl_ast_node;



//==============================================================================
//
// Functions
//
//==============================================================================

vpl_ast_node *vpl_ast_number_create(double value);

vpl_ast_node *vpl_ast_variable_create(char *name);

vpl_ast_node *vpl_ast_binary_expr_create(vpl_ast_binop_e operator,
    vpl_ast_node *lhs, vpl_ast_node *rhs);

vpl_ast_node *vpl_ast_call_create(char *name, vpl_ast_node **args,
    int arg_count);

vpl_ast_node *vpl_ast_prototype_create(char *name, char **args,
    int arg_count);

vpl_ast_node *vpl_ast_function_create(vpl_ast_node *prototype,
    vpl_ast_node *body);

vpl_ast_node *vpl_ast_if_expr_create(vpl_ast_node *condition,
    vpl_ast_node *true_expr, vpl_ast_node *false_expr);

void vpl_ast_node_free(vpl_ast_node *node);

#endif
