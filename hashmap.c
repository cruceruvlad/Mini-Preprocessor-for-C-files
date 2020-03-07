#include<stdlib.h>
#include<string.h>

struct node {
	char *key;
	char *val;
	struct node *next;
};

struct list {
	struct node *head;
};

struct table {
	int size;
	struct list *list;
};

struct table *createTable(int size)
{
	int i;
	struct table *t = (struct table *)malloc(sizeof(struct table));

	t->size = size;
	t->list = (struct list *)malloc(sizeof(struct list)*size);
	for (i = 0; i < size; i++)
		t->list[i].head = NULL;
		return t;
}

int hashCode(struct table *t, char *key)
{
	int i = 0;
	int hash = 0;

	for (i = 0; i < strlen(key); i++)
		hash += (int)key[i];
	return hash%(t->size);
}

void insert(struct table *t, char *key, char *val)
{
	int pos = hashCode(t, key);
	struct node *temp = t->list[pos].head;
	struct node *newNode = (struct node *)malloc(sizeof(struct node));

	while (temp) {
		if (!strcmp(temp->key, key)) {
			strcpy(temp->val, val);
			return;
		}
		temp = temp->next;
	}
	newNode->key = (char *)malloc((strlen(key)+1)*sizeof(char));
	strcpy(newNode->key, key);
	newNode->val = (char *)malloc((strlen(val)+1)*sizeof(char));
	strcpy(newNode->val, val);
	newNode->next = t->list[pos].head;
	t->list[pos].head = newNode;
}

char *lookup(struct table *t, char *key)
{
	int pos = hashCode(t, key);
	struct node *temp = t->list[pos].head;

	while (temp) {
		if (!strcmp(temp->key, key))
			return temp->val;
		temp = temp->next;
	}
	return NULL;
}

int deleteTable(struct table *t)
{
	int size = t->size;
	int i;
	struct node *next;
	struct node *node;

	for (i = 0; i < size; i++) {
		if (t->list[i].head == NULL)
			continue;
		node = t->list[i].head;
		next = node->next;
		free(node->key);
		free(node->val);
		free(node);
		while (next) {
			node = next;
			next = node->next;
			free(node->key);
			free(node->val);
			free(node);
		}
	}
	free(t->list);
	free(t);
	return 0;
}

void delete(struct table *t, char *key)
{
	int pos = hashCode(t, key);
	struct node *temp = t->list[pos].head;
	struct node *prev;

	if (temp == NULL)
		return;
	if (!strcmp(temp->key, key)) {
		t->list[pos].head = temp->next;
		free(temp->key);
		free(temp->val);
		free(temp);
		return;
	}
	prev = temp;
	temp = temp->next;
	while (temp) {
		if (!strcmp(temp->key, key)) {
			prev->next = temp->next;
			free(temp->key);
			free(temp->val);
			free(temp);
		}
		prev = temp;
		temp = temp->next;
	}
}
