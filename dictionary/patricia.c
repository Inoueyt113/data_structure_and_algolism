#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_KEY_LENGTH 256

// パトリシア木のノード構造
typedef struct Node
{
    char *data;             // キー
    bool isEndOfWord;       // 単語の最後の文字を表すかどうか
    struct Node **children; // 子ノードへのポインタの配列
    int numChildren;        // 子ノードの数
} Node;

// 新しいノードを作成する関数
Node *createNode(const char *data, bool isEndOfWord)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = strdup(data);
    newNode->isEndOfWord = isEndOfWord;
    newNode->children = NULL;
    newNode->numChildren = 0;
    return newNode;
}

// 子ノードを追加する関数
void addChild(Node *parent, Node *child)
{
    parent->children = (Node **)realloc(parent->children, (parent->numChildren + 1) * sizeof(Node *));
    parent->children[parent->numChildren] = child;
    parent->numChildren++;
}

// パトリシア木に値を挿入する関数
void insert(Node **root, const char *data)
{
    if (*root == NULL)
    {
        *root = createNode(data, true); // 最後の文字なので true
    }
    else
    {
        Node *currentNode = *root;
        Node *parentNode = NULL;
        int i = 0;

        // 接頭辞の最初の一致まで移動する
        while (currentNode != NULL && i < strlen(currentNode->data) && data[i] == currentNode->data[i])
        {
            parentNode = currentNode;
            currentNode = NULL;
            for (int j = 0; j < parentNode->numChildren; j++)
            {
                if (data[i] == parentNode->children[j]->data[0])
                {
                    currentNode = parentNode->children[j];
                    break;
                }
            }
            i++;
        }

        // 新しいノードを作成して親ノードに接続する
        if (currentNode == NULL)
        {
            Node *newNode = createNode(data, true); // 最後の文字なので true
            addChild(parentNode, newNode);
        }
        else
        {
            // 共通の接頭辞を持つ場合、新しいノードの一部として既存のノードを再利用
            if (i < strlen(currentNode->data))
            {
                Node *newNode = createNode(data, false); // 最後の文字でないので false
                addChild(parentNode, newNode);
                for (int j = 0; j < currentNode->numChildren; j++)
                {
                    addChild(newNode, currentNode->children[j]);
                }
                newNode->data = strdup(currentNode->data + i);
                currentNode->data[i] = '\0';
            }
            else
            {
                // キーが既に存在する場合は単語の最後の文字を true に設定
                currentNode->isEndOfWord = true;
            }
        }
    }
}

// パトリシア木から値を検索する関数
bool search(Node *root, const char *data)
{
    Node *currentNode = root;
    int i = 0;

    while (currentNode != NULL && i < strlen(currentNode->data) && data[i] == currentNode->data[i])
    {
        currentNode = NULL;
        for (int j = 0; j < currentNode->numChildren; j++)
        {
            if (data[i] == currentNode->children[j]->data[0])
            {
                currentNode = currentNode->children[j];
                break;
            }
        }
        i++;
    }

    // キーの最後の文字に到達し、ノードが単語の終わりを示している場合に true を返す
    return (currentNode != NULL && i == strlen(currentNode->data) && currentNode->isEndOfWord);
}

// パトリシア木を削除する関数（メモリを解放する）
void destroy(Node *root)
{
    if (root == NULL)
        return;

    for (int i = 0; i < root->numChildren; i++)
    {
        destroy(root->children[i]);
    }
    free(root->children);
    free(root->data);
    free(root);
}

// 辞書順に表示する関数
void display(Node *root, char *prefix, int depth)
{
    if (root == NULL)
        return;

    // 末端のノードであればデータ（単語）を表示
    if (root->isEndOfWord)
    {
        prefix[depth] = '\0';
        printf("%s\n", prefix);
    }

    // 子ノードを再帰的に表示
    for (int i = 0; i < root->numChildren; i++)
    {
        prefix[depth] = root->data[0];
        display(root->children[i], prefix, depth + 1);
    }
}

int main()
{
    Node *root = NULL;

    insert(&root, "car");
    insert(&root, "card");
    // insert(&root, "try");
    // insert(&root, "cat");
    // insert(&root, "tried");

    printf("Words in the tree:\n");
    char prefix[MAX_KEY_LENGTH];
    display(root, prefix, 0);

    destroy(root);

    return 0;
}
