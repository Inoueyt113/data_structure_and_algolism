#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ALPHABET_SIZE 26

struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    bool isEndOfWord;
};

struct TrieNode *createNode()
{
    struct TrieNode *newNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (newNode)
    {
        int i;
        newNode->isEndOfWord = false;
        for (i = 0; i < ALPHABET_SIZE; i++)
            newNode->children[i] = NULL;
    }
    return newNode;
}

void insert(struct TrieNode *root, const char *key)
{
    int level;
    int length = strlen(key);
    int index;

    struct TrieNode *currentNode = root;

    for (level = 0; level < length; level++)
    {
        index = key[level] - 'a';
        if (!currentNode->children[index])
            currentNode->children[index] = createNode();

        currentNode = currentNode->children[index];
    }

    currentNode->isEndOfWord = true;
}

bool search(struct TrieNode *root, const char *key)
{
    int level;
    int length = strlen(key);
    int index;

    struct TrieNode *currentNode = root;

    for (level = 0; level < length; level++)
    {
        index = key[level] - 'a';
        if (!currentNode->children[index])
            return false;

        currentNode = currentNode->children[index];
    }

    return (currentNode != NULL && currentNode->isEndOfWord);
}

bool isEmpty(struct TrieNode *node)
{
    int i;
    for (i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node->children[i])
            return false;
    }
    return true;
}

struct TrieNode *removeHelper(struct TrieNode *root, const char *key, int depth)
{
    if (!root)
        return NULL;

    if (depth == strlen(key))
    {
        if (root->isEndOfWord)
            root->isEndOfWord = false;

        if (isEmpty(root))
        {
            free(root);
            root = NULL;
        }

        return root;
    }

    int index = key[depth] - 'a';
    root->children[index] = removeHelper(root->children[index], key, depth + 1);

    if (isEmpty(root) && !root->isEndOfWord)
    {
        free(root);
        root = NULL;
    }

    return root;
}

void removeKey(struct TrieNode *root, const char *key)
{
    removeHelper(root, key, 0);
}

void delete(struct TrieNode *root, const char *key)
{
    if (!search(root, key))
    {
        printf("'%s' は木に存在しません。\n", key);
        return;
    }

    removeKey(root, key);
    printf("'%s' は木から削除されました。\n", key);
}

void printHelper(struct TrieNode *root, char *buffer, int level)
{
    if (root->isEndOfWord)
    {
        buffer[level] = '\0';
        printf("%s\n", buffer);
    }

    int i;
    for (i = 0; i < ALPHABET_SIZE; i++)
    {
        if (root->children[i])
        {
            buffer[level] = i + 'a';
            printHelper(root->children[i], buffer, level + 1);
        }
    }
}

void printTree(struct TrieNode *root)
{
    printf("トライ木の単語: \n");
    char buffer[100];
    printHelper(root, buffer, 0);
    printf("\n");
}

void freeTrie(struct TrieNode *root)
{
    if (!root)
        return;

    int i;
    for (i = 0; i < ALPHABET_SIZE; i++)
        freeTrie(root->children[i]);

    free(root);
}

int main()
{
    // Trie
    struct TrieNode *root = createNode();

    // // 1. car, card, try, tried, catを挿入する
    // insert(root, "car");
    // insert(root, "card");
    // insert(root, "try");
    // insert(root, "tried");
    // insert(root, "cat");

    // printTree(root);

    // // 2. card, carp, trieを検索する
    // search(root, "card");
    // search(root, "carp");
    // search(root, "trie");

    // // 3. tried, cat, try, car, cardを削除する
    // delete (root, "tried");
    // delete (root, "cat");
    // delete (root, "try");
    // delete (root, "card");

    // printTree(root);

    // freeTrie(root);

    // TrieNode *root = createNode();

    while (1)
    {
        int select;
        printf("exit:0 insert:1 search:2 delete:3\nselect>>");
        scanf("%d", &select);

        if (!select || select > 3)
            break;

        switch (select)
        {
            char input[BUFSIZ];
        case 1:
            printf("insert>>");
            scanf("%s", &input);
            insert(root, input);
            printTree(root);
            printf("\n");
            break;
        case 2:
            printf("search>>");
            scanf("%s", &input);
            bool s = search(root, input);
            if (s == true)
                printf("%s は見つかりました。\n", input);
            else
                printf("%s は見つかりませんでした。\n", input);
            printf("\n");
            break;
        case 3:
            printf("insert>>");
            scanf("%s", &input);
            delete (root, input);
            printTree(root);
            printf("\n");
            break;
        default:
            break;
        }
    }

    return 0;
}
