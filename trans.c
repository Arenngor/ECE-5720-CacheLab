/*Rowan Antonuccio 
 *A02184507
 *ECE 5720 Cachelab
 */

#include <stdio.h>
#include "cachelab.h"

void LARGE_trans(int M, int N, int A[N][M], int B[M][N]);
int is_transpose(int M, int N, int A[N][M], int B[M][N]);
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	int i, j, k, l, b = 16;
	int temp, d; // 7 variables of type int

	if (N == 32 && M == 32)
	{
		b = 8;//small
	}

	if (N == 64 && M == 64)
	{
		LARGE_trans(M, N, A, B);//big
	}
	else
	{

		for (i = 0; i < N; i += b)
		{
			for (j = 0; j < M; j += b)
			{
				for (k = 0; k < b && i + k < N; k++)
				{
					for (l = 0; l < b && j + l < M; l++)
					{
						if (j + l != i + k)
						{

							B[j + l][i + k] = A[i + k][j + l];
						}
						else
						{

							temp = A[i + k][j + l];
							d = i + k;
						}
					}
					if (i == j)
					{
						B[d][d] = temp;
					}
				}
			}
		}
	}
}

void LARGE_trans(int M, int N, int A[N][M], int B[M][N])
{

	int i, j, k, l;
	int var1, var2, var3, var4;
	int var5, var6, var7, var8; //12 variables of type int

	for (i = 0; i < N; i += 8)
	{
		for (j = 0; j < M; j += 8)
		{
			for (k = i; k < i + 4; k++)
			{

				var1 = A[k][j + 0];
				var2 = A[k][j + 1];
				var3 = A[k][j + 2];
				var4 = A[k][j + 3];
				var5 = A[k][j + 4];
				var6 = A[k][j + 5];
				var7 = A[k][j + 6];
				var8 = A[k][j + 7];

				B[j + 0][k] = var1;
				B[j + 1][k] = var2;
				B[j + 2][k] = var3;
				B[j + 3][k] = var4;

				B[j + 0][k + 4] = var5;
				B[j + 1][k + 4] = var6;
				B[j + 2][k + 4] = var7;
				B[j + 3][k + 4] = var8;
			}
			for (k = j; k < j + 4; k++)
			{

				var5 = A[i + 4][k];
				var6 = A[i + 5][k];
				var7 = A[i + 6][k];
				var8 = A[i + 7][k];

				var1 = B[k][i + 4];
				var2 = B[k][i + 5];
				var3 = B[k][i + 6];
				var4 = B[k][i + 7];

				B[k][i + 4] = var5;
				B[k][i + 5] = var6;
				B[k][i + 6] = var7;
				B[k][i + 7] = var8;

				B[k + 4][i + 0] = var1;
				B[k + 4][i + 1] = var2;
				B[k + 4][i + 2] = var3;
				B[k + 4][i + 3] = var4;

				for (l = 0; l < 4; l++)
				{
					B[k + 4][i + l + 4] = A[i + l + 4][k + 4];
				}
			}
		}
	}
}

/* 
 *You can define additional transpose functions below. We've defined
 *a simple one below to help you get started. 
 */

/* 
 *trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
	int i, j, tmp;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			tmp = A[i][j];
			B[j][i] = tmp;
		}
	}
}

/*
 *registerFunctions - This function registers your transpose
 *    functions with the driver.  At runtime, the driver will
 *    evaluate each of the registered functions and summarize their
 *    performance. This is a handy way to experiment with different
 *    transpose strategies.
 */
void registerFunctions()
{
	/*Register your solution function */
	registerTransFunction(transpose_submit, transpose_submit_desc);

	/*Register any additional transpose functions */
	registerTransFunction(trans, trans_desc);

}

/* 
 *is_transpose - This helper function checks if B is the transpose of
 *    A. You can check the correctness of your transpose by calling
 *    it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
	int i, j;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; ++j)
		{
			if (A[i][j] != B[j][i])
			{
				return 0;
			}
		}
	}
	return 1;
}