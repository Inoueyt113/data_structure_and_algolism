#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// 2-3木のノードを表す構造体
typedef struct Node
{
    int isLeaf;          // ノードが葉であるかどうかを示すフラグ
    int value1, value2;  // ノードが保持する値
    struct Node *left;   // 左の子ノードへのポインタ
    struct Node *middle; // 中央の子ノードへのポインタ
    struct Node *right;  // 右の子ノードへのポインタ
    struct Node *parent; // 親へのポインタ
} Node;

// 新しいノードを作成する関数
Node *createNode(int value)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->isLeaf = 1; // 最初は葉として作成される
    newNode->value1 = value;
    newNode->value2 = -1;
    newNode->left = NULL;
    newNode->middle = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    return newNode;
}

// 2-3木を表示する関数
void printTree(Node *node, int level)
{
    if (node != NULL)
    {

        // インデントを追加してノードの値を表示
        printf("%*s", level * 4, "");

        // 葉ノードの出力　-> []
        if (node->isLeaf == 1)
        {
            // ノードが値を保持していない
            if (node->value1 == -1)
            {
                printf("[]");
                printf("%d ", level);
            }
            // ノードが1つの値を保持
            else if (node->value2 == -1)
            {
                printf("[%d]", node->value1);
                printf("%d ", level);
            }
            // ノードが2つの値を保持
            else
            {
                printf("[%d, %d]", node->value1, node->value2);
                printf("%d ", level);
            }
            if (node->left != NULL)
                printf("l%d ", node->left->value1);

            if (node->middle != NULL)
                printf("m%d ", node->middle->value1);

            if (node->right != NULL)
                printf("r%d ", node->right->value1);

            if (node->parent != NULL)
                printf("p%d ", node->parent->value1);
        }

        // 内部ノードの出力　-> ()
        else if (node->isLeaf == 0)
        {
            // ノードが値を保持していない
            if (node->value1 == -1)
            {
                printf("()");
                printf("%d ", level);
            }
            // ノードが1つの値を保持
            else if (node->value2 == -1)
            {
                printf("(%d)", node->value1);
                printf("%d ", level);
            }
            // ノードが2つの値を保持
            else
            {
                printf("(%d, %d)", node->value1, node->value2);
                printf("%d ", level);
            }
            if (node->left != NULL)
                printf("l%d ", node->left->value1);

            if (node->middle != NULL)
                printf("m%d ", node->middle->value1);

            if (node->right != NULL)
                printf("r%d ", node->right->value1);

            if (node->parent != NULL)
                printf("p%d ", node->parent);
        }

        // 子ノードを再帰的に表示
        printTree(node->left, level + 1);
        printTree(node->middle, level + 1);
        printTree(node->right, level + 1);
    }
}

Node *returnRoot(Node *node)
{
    while (node->parent != NULL)
    {
        node = node->parent;
    }
    return node;
}

int searchValue(Node *node, int value)
{
    // 対象が見つかったら再帰終了
    if (node->value1 == value || node->value2 == value)
    {
        return 1;
    }
    // 対象は見つからないものの、葉ノードであれば子ノードを持たないため終了
    else if (node->isLeaf == 1)
    {
        return 0;
    }

    // 再帰的に探索
    int l = 0, m = 0, r = 0;
    l = searchValue(node->left, value);
    m = searchValue(node->middle, value);
    r = searchValue(node->right, value);

    return (l + m + r);
}

// 木を上に伸ばす関数
Node *extendup(Node *node, int value)
{
    // 挿入先の親の値の数で分岐
    // 親の値の個数が0 <=> 根ノード
    if (node->parent == NULL)
    {
        puts("deformation|parent|:0");

        // 根ノードの場合のみ
        // 親として上に出す中央値の変数
        int median = -1;
        int left = -1;
        int middle = -1;

        // value < v1 < v2  (median = v1)
        if (value < node->value1)
        {
            median = node->value1;
            left = value;
            middle = node->value2;
        }

        // v1 < value < v2 (median = value)
        else if (node->value1 < value && value < node->value2)
        {
            median = value;
            left = node->value1;
            middle = node->value2;
        }

        // v1 < v2 < value (median = v2)
        else if (node->value2 < value)
        {
            median = node->value2;
            left = node->value1;
            middle = value;
        }

        //     newParent(median)
        //     /           |
        //  nowNode(left)  newMiddle(middle)

        // 親、右の子の作成
        Node *newParent = createNode(median);
        Node *newMiddle = createNode(middle);

        // 2-3木になるようにポインタを変更
        // newParentの変更
        newParent->left = node;
        newParent->middle = newMiddle;
        newParent->value1 = median;
        newParent->isLeaf = 0;

        // nowNodeの変更
        node->value1 = left;
        node->value2 = -1;

        // newMiddleの変更
        newMiddle->value1 = middle;

        // 親の追加
        newMiddle->parent = newParent;
        node->parent = newParent;

        return returnRoot(node);
    }

    // 親の値の個数が1
    else if (node->parent->value1 != -1 && node->parent->value2 == -1)
    {
        //  [v1, -1]
        //  /      |
        //[a,b]    [a,b]

        puts("deformation|parent|:1");

        // 親として上に出す中央値の変数
        int median = -1;
        int left = -1;
        int right = -1;

        // value < v1 < v2  (median = v1)
        if (value < node->value1)
        {
            median = node->value1;
            left = value;
            right = node->value2;
        }

        // v1 < value < v2 (median = value)
        else if (node->value1 < value && value < node->value2)
        {
            median = value;
            left = node->value1;
            right = node->value2;
        }

        // v1 < v2 < value (median = v2)
        else if (node->value2 < value)
        {
            median = node->value2;
            left = node->value1;
            right = value;
        }

        // 新しい右の子ノードを作成
        Node *newRight = createNode(right);

        // データの整理
        // 親にmedianを挿入する
        if (median > node->parent->value1)
        {
            node->parent->value2 = median;
        }
        else if (median < node->parent->value1)
        {
            node->parent->value2 = node->parent->value1;
            node->parent->value1 = median;
        }

        // 既存のノードnodeの整理
        node->value2 = -1;

        // newRightの整理
        newRight->parent = node->parent;

        // 親(node->parent)のポインタを整理
        if (node->parent->left == node)
        {
            node->parent->right = node->parent->middle;
            node->parent->middle = newRight;
            node->parent->left = node;
        }
        else if (node->parent->middle == node)
        {
            node->parent->middle = node;
            node->parent->right = newRight;
        }
        // 根ノードを返す
        return returnRoot(node);
    }

    // 親の値の数が2 <=> 根ノード
    else if (node->parent->value1 != -1 && node->parent->value2 != -1)
    {
        printf("deformation|parent|:2\n");

        // 親の親の値の個数で分岐する必要あり
        int median = -1;
        int left = -1;
        int right = -1;

        // value < v1 < v2  (median = v1)
        if (value < node->value1)
        {
            median = node->value1;
            left = value;
            right = node->value2;
        }

        // v1 < value < v2 (median = value)
        else if (node->value1 < value && value < node->value2)
        {
            median = value;
            left = node->value1;
            right = node->value2;
        }

        // v1 < v2 < value (median = v2)
        else if (node->value2 < value)
        {
            median = node->value2;
            left = node->value1;
            right = value;
        }

        // 親の親の値がそもそもない
        if (node->parent->parent == NULL)
        {
        }

        // // 親の親の値が1個
        // else if (node->parent->parent->value1 != -1 && node->parent->parent->value2 == -1)
        // {
        //     if (value < node->parent->parent->value1)
        //     {
        //         node->value2 = node->value1;
        //         node->value1 = value;
        //     }
        //     else if (value > node->parent->parent->value1)
        //     {
        //         node->parent
        //     }
        // }

        // // 親の親の値が2個
        // else if (node->parent->parent->value1 != -1 && node->parent->parent->value2 == -1)
        // {
        // }

        return returnRoot(node);
    }
}

// 挿入
// 返り値は根ノード
Node *insertValue(Node *node, int value)
{

    // // 探索してすでに値があったら終了
    // if (searchValue(node, value) != 0)
    // {
    //     printf("2-3tree already has %d\n", value);
    //     return returnRoot(node);
    // }

    //  内部ノードであれば
    if (node->isLeaf == 0)
    {
        printf("%d :Inner->", value);
        // 探索
        // 現在のノードが2ノード [v, -1]
        if (node->value1 != -1 && node->value2 == -1)
        {
            printf("2 node->");
            // 再帰
            //  value < v => 左に移動
            if (value < node->value1)
            {
                printf("move to left\n");
                insertValue(node->left, value);
            }

            // value < v => 右に移動
            else if (value > node->value1)
            {
                printf("move to middle\n");
                insertValue(node->middle, value);
            }

            // 内部ノード -含む-> 根ノード
            // 根ノードが返される <=> 表示がうまくいく
            return returnRoot(node);
        }

        // 現在のノードが3ノード  [v1, v2]
        else if (node->value1 != -1 && node->value2 != -1)
        {
            printf("v1:%d v2:%d\n", node->value1, node->value2);
            printf("3node->");
            // value < v1 < v2 => move to left
            if (value < node->value1)
            {
                printf("move to left\n");
                insertValue(node->left, value);
            }

            // v1 < value < v2 => move to middle
            else if (node->value1 < value && value < node->value2)
            {
                printf("move to middle\n");
                insertValue(node->middle, value);
            }

            // v1 < v2 < value => move to right
            else if (node->value2 < value)
            {
                printf("move to right\n");
                insertValue(node->right, value);
            }

            // 根ノードを返す
            return returnRoot(node);
        }
    }

    // 　ノードが葉である場合
    else if (node->isLeaf == 1)
    {
        printf("%d :Leaf->\n", value);
        // [-1,-1] = 両方空の場合
        if (node->value1 == -1 && node->value2 == -1)
        {
            // 一番最初の挿入の場合
            node->value1 = value;
            return node;
        }
        // [v,-1] = 一つ入る 問題点 3ノードになったらどうなる？
        else if (node->value1 != -1 && node->value2 == -1)
        {
            if (value < node->value1)
            {
                //[value, value1]になる場合
                node->value2 = node->value1;
                node->value1 = value;
                return node;
            }
            else //[value1, value]になる場合
            {
                node->value2 = value;
                return node;
            }
        }

        // [v1,v2] = 入らない場合
        else if (node->value1 != -1 && node->value1 != -1)
        {

            // 上に値を伸ばす関数
            //     |
            // node[v1, v2]

            extendup(node, value);
        }
    }

    // ノードが葉でない場合
}

// 必要があれば変形
int main()
{
    // 2-3木のルートノードを作成
    Node *root = createNode(-1);

    // // 根が葉である場合のテスト
    // while (1)
    // {
    //     printf("input>>");
    //     int input = -1;
    //     scanf("%d", &input);
    //     if (input == -1)
    //         break;

    //     root = insertValue(root, input);
    //     printTree(root, 0);
    // }

    // 時刻をシード値として入力
    srand(time(NULL));

    // 8つの乱数を生成
    int i;
    for (i = 0; i < 16; i++)
    {
        // 他の処理との間に微小な遅延を挿入
        Sleep(1000); // 1000ms
        srand(time(NULL));

        // 0-20 までの乱数 + 2 = 2-22の乱数
        int randomNumber = rand() % 21 + 2;
        root = insertValue(root, randomNumber);
        printTree(root, 0);
    }

    // while (1)
    // {
    //     printf("search>>");
    //     int input = -1;
    //     scanf("%d", &input);
    //     if (input == -1)
    //         break;

    //     if (searchValue(root, input))
    //         printf("%d is found.\n", input);
    //     else
    //         printf("%d is not found.\n", input);
    //     printTree(root, 0);
    // }

    return 0;
}

// 5 1 7 4 でばぐる？