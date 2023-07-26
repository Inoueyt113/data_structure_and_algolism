#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// トライのノードを表す構造体
typedef struct TrieNode
{
    // データを持たないのはchildren[i]がNULLであるかどうか, でアルファベットを区別している (0<=i<=25)
    struct TrieNode *children[26]; // 英字アルファベットの小文字を格納するために、子ノードを指すポインタの配列を持つ
    int isEndOfWord;               // このノードが単語の終端であるかどうかを示すフラグ
} TrieNode;

// 新しいトライノードを作成し、初期化する関数
TrieNode *createNode()
{
    TrieNode *newNode = (TrieNode *)malloc(sizeof(TrieNode));
    newNode->isEndOfWord = 0;

    for (int i = 0; i < 26; i++)
    {
        newNode->children[i] = NULL;
    }

    return newNode;
}

// トライに単語を挿入する関数
void insert(TrieNode *root, const char *word)
{
    TrieNode *currentNode = root;

    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a'; // 文字を0から25の範囲に変換する

        if (currentNode->children[index] == NULL)
        {
            currentNode->children[index] = createNode();
        }

        if (word[i + 1] != '\0')
            currentNode = currentNode->children[index];
    }

    currentNode->isEndOfWord = 1;

    return;
}

// トライから単語を検索する関数
int search(TrieNode *root, const char *word)
{
    TrieNode *currentNode = root;

    // 　文字列を先頭から順に木をたどる
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';

        // 検索文字が見つからず遷移できなかったら、探索失敗
        if (currentNode->children[index] == NULL)
            return 0;

        // 見つかったら遷移する
        currentNode = currentNode->children[index];
    }

    // 遷移が終了したら、現在いるノードのisEndOfWordで検索文字列が存在するか確認
    return (currentNode != NULL && currentNode->isEndOfWord);
}

int delete(TrieNode *root, const char *word)
{
    TrieNode *currentNode = root;

    // 　文字列を先頭から順に木をたどる
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';

        // 検索文字が見つからず遷移できなかったら、探索失敗
        if (currentNode->children[index] == NULL)
            return 0;

        // 見つかったら遷移する
        currentNode = currentNode->children[index];
    }

    // 遷移が終了したら、現在いるノードのisEndOfWordで検索文字列が存在するか確認
    if (currentNode != NULL && currentNode->isEndOfWord == 1)
    {
        currentNode->isEndOfWord = 0;
        return 1;
    }
}

// 表示関数
void printTree(TrieNode *root, int level)
{
    TrieNode *currentNode = root;

    // 　文字列を先頭から順に木をたどる
    for (int i = 0; i < 26; i++)
    {
        // 検索文字が見つからず遷移できなかったら、探索失敗
        if (currentNode->children[i] != NULL)
        {
            printf("%c ", 'a' + i);
            if (currentNode->isEndOfWord == 1)
                printf("\n");
            printTree(currentNode->children[i], level + 1);
        }
    }
}

// トライの動作をテストするためのサンプルコード
int main()
{
    TrieNode *root = createNode();

    insert(root, "car");
    insert(root, "card");
    insert(root, "try");
    insert(root, "tried");
    insert(root, "cat");

    delete (root, "car");

    printTree(root, 0);
    return 0;
}
