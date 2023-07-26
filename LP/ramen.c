#include <stdio.h>
#include <float.h>

// 一般化されていない非規定変数が3変数の場合のみ, シンプレックス法を用いたプログラム
//
//  最大化: a*x1 + b*x2 + c*x3
//  制約条件:
// a11*x1 + b12*x2 + c13*x3 <= 60
// a21*x1 + b22*x2 + c23*x3 <= 60
// a31*x1 + b32*x2 + c33*x3 <= 30

// (*table)[8] 配列へのポインタ
void showTable(double (*table)[8])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            printf("%7.1f", table[i][j]);
        }
        printf("\n");
    }
}

void simplex(double (*table)[8])
{
    while (1)
    {
        // 目的関数の係数がすべて非負(>=0)ならば終了
        if (table[0][0] >= 0 && table[0][1] >= 0 && table[0][2] >= 0)
            return;

        // 列選択
        // 目的関数の係数の最小値を求める-> qを求める
        double min = 0;
        int q = 0;
        if (table[0][0] < min)
        {
            min = table[0][0];
            q = 0;
        }
        if (table[0][1] < min)
        {
            min = table[0][1];
            q = 1;
        }
        if (table[0][2] < min)
        {
            min = table[0][2];
            q = 2;
        }

        // c/Xqの計算
        for (int i = 1; i <= 3; i++)
            table[i][7] = table[i][6] / table[i][q];

        // 行選択
        // c/Xqの中から最小値を求める->pを求める
        min = DBL_MAX;
        int p = 0;
        if (table[1][7] < min)
        {
            min = table[1][7];
            p = 1;
        }
        if (table[2][7] < min)
        {
            min = table[2][7];
            p = 2;
        }
        if (table[3][7] < min)
        {
            min = table[3][7];
            p = 3;
        }

        // 行基本変形によって掃き出しを行う (table_pq)
        // table_pq を1にする定数k を求める
        double k = 1 / table[p][q];

        // p行*k -> pを1にする
        for (int i = 0; i < 8; i++)
            table[p][i] = table[p][i] * k;

        // 掃き出し
        for (int i = 0; i <= 3; i++)
        {
            // i=p の場合はスキップ
            if (i == p)
                continue;

            // cはiが等しい場合, 共通して使用
            double c = (table[i][q] * -1);
            for (int j = 0; j < 8; j++)
            {
                table[i][j] += table[p][j] * c;
            }
        }

        // // c/Xqの初期化
        for (int i = 0; i <= 3; i++)
            table[i][7] = 0;
    }
}

// 結果を表示する関数
void showResult(double (*table)[8])
{
    printf("(");
    for (int i = 1; i <= 3; i++)
        printf("%7.1f ", table[i][6]);

    printf(") => (");
    printf("%7.1f)", table[0][6]);
}

int main()
{
    // ----------------------------------------INPUT----------------------------------------
    // 最大化する目的関数の係数を格納
    double obj[3] = {8, 9, 10};

    // 制約条件を格納
    double con[3][4] = {
        {2, 1, 1, 60},
        {1, 2, 1, 60},
        {0, 0, 1, 30}};

    // ----------------------------------------INPUT----------------------------------------

    // 初期辞書の作成
    double table[4][8] = {
        {-1 * obj[0], -1 * obj[1], -1 * obj[2], 0, 0, 0, 0, 0},
        {con[0][0], con[0][1], con[0][2], 1, 0, 0, con[0][3], 0},
        {con[1][0], con[1][1], con[1][2], 0, 1, 0, con[1][3], 0},
        {con[2][0], con[2][1], con[2][2], 0, 0, 1, con[2][3], 0}};

    // シンプレックス法の実行
    simplex(table);

    // 表示
    showTable(table);
    showResult(table);

    return 0;
}