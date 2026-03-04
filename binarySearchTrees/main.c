#include<stdio.h>
#include <stdlib.h>

typedef struct node {
int key;
    struct node *left;
    struct node *right;
} node;

typedef struct tree {
node *root;
} tree;

void insert(tree *a, int k) {
    node *new=malloc(sizeof(node));
    new->key=k;
    new->left=new->right=NULL;
    node *parent=NULL;
    node *temp=a->root;
    if (!a->root) {
        a->root=new;
        return;
    }
    while (temp!=NULL) {
        parent=temp;
        if (k<temp->key) {
            temp=parent->left;
        }
        else {
            temp=parent->right;
        }
    }
    if (k<parent->key)
        parent->left=new;
    else
        parent->right=new;
}

node* search(tree a, int k) {
    node *temp=a.root;
    while (temp!=NULL) {
        if (temp->key==k)
            return temp;
        if (k<temp->key)
            temp=temp->left;
        else
            temp=temp->right;
    }
    return temp;
}

node* minimum(node *x) {
    node *temp=x;
    while (temp->left!=NULL)
        temp=temp->left;
    return temp;
}

node* maximum(node *x) {
    node *temp=x;
    while (temp->right!=NULL)
        temp=temp->right;
    return temp;
}

node* parent(tree *a,node *k) {
    node *p=NULL;
    node *temp=a->root;
    while (temp!=NULL && temp!=k) {
        p=temp;
        if (k->key<temp->key)
            temp=temp->left;
        else
            temp=temp->right;
    }
    return p;
}

void delete(tree *a,node *k) {
    if (k==NULL)
        return;

    node *p=parent(a,k);

    if (k->left==NULL && k->right==NULL) {
        if (p==NULL)
            a->root=NULL;
        else if (p->left==k)
            p->left=NULL;
        else
            p->right=NULL;
        free(k);
        return;
    }

    if (k->left==NULL || k->right==NULL) {
        node *child;
        if (k->left!=NULL)
            child=k->left;
        else
            child=k->right;

        if (p==NULL)
            a->root=child;
        else if (p->left==k)
            p->left=child;
        else
            p->right=child;

        free(k);
        return;
    }

    node *s=minimum(k->right);
    node *ps=parent(a,s);

    if (ps->left==s)
        ps->left=s->right;
    else
        ps->right=s->right;

    k->key=s->key;
    free(s);
}