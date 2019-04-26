#ifndef TYPE_H
#define TYPE_H

#include <stdio.h>
#include "TreeNode.h"

typedef struct Type_ Type;
typedef struct FieldList_ FieldList;
struct Type_
{
    enum
    {
        BASIC,
        ARRAY,
        STRUCTURE
    } kind;
    union {
        //BASIC: int = 0, float = 1
        int basic;
        //ARRAY: 数组类型信息包括元素类型与数组大小构成
        struct
        {
            Type *elem;
            int size;
        } array;
        //STRUCTURE: 结构体类型信息是一个链表,第一个域是struct本身的名字(且其type==NULL)
        FieldList *structure;
    } u;

    //for linked-list
    Type *next;
};
struct FieldList_
{
    char *name;      // 域的名字
    Type *type;      // 域的类型
    FieldList *next; // 下一个域
    bool init;
    int first_line;
};

extern Type *struct_list;

extern Type *TYPE_INT;
extern Type *TYPE_FLOAT;

extern FieldList *struct_field_list;

Type *get_type_from_specifier(TreeNode *node);

Type *look_up_struct_list(char *name);

void add_to_struct_list(Type *type);

FieldList *get_def_list(TreeNode *def_list);

FieldList *get_dec(TreeNode *dec, Type *def_type);

FieldList *get_var_dec(TreeNode *var_dec, Type *def_type);

FieldList *get_var_list(TreeNode *var_list);

void add_to_struct_field_list(FieldList *p);

FieldList *look_up_struct_field_list(char *name);

#endif