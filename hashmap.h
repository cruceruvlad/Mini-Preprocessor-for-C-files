#ifndef HASHMAP_H
#define HASHMAP_H

struct node;

struct table;

struct list;

struct table *createTable(int size);

int hashCode(struct table *t, char *key);

void insert(struct table *t, char *key, char *val);

char *lookup(struct table *t, char *key);

void delete(struct table *t, char *key);

int deleteTable(struct table *t);

#endif
