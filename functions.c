#include <stdio.h>
#include <stdlib.h>
#define pi 3.14

typedef struct {
    int rows;
    int columns;
    float** elements;
} matrix;

matrix mat_init(int rows, int columns);
void mat_print(matrix* mat);
void mat_free(matrix* mat);
void mat_scalar_mul(matrix* mat, float a);
matrix mat_mul(matrix* mat1, matrix* mat2);
matrix mat_add(matrix* mat1, matrix* mat2);
matrix mat_sub (matrix* mat1, matrix* mat2);
matrix mat_cofactors(matrix* mat);
matrix mat_transpose(matrix* mat);
matrix mat_inverse(matrix* mat);
float det(matrix* mat);

int main(void)
{
    int i, j;

    matrix mat1 = mat_init(3, 3);
    mat1.elements[0][0] = 1;
    mat1.elements[0][1] = 2;
    mat1.elements[0][2] = 3;
    mat1.elements[1][0] = 4;
    mat1.elements[1][1] = 5;
    mat1.elements[1][2] = 6;
    mat1.elements[2][0] = 7;
    mat1.elements[2][1] = 8;
    mat1.elements[2][2] = 9;

    matrix cof = mat_cofactors(&mat1);

    matrix mat2 = mat_init(3, 3);
    mat2.elements[0][0] = 3;
    mat2.elements[0][1] = 0;
    mat2.elements[0][2] = 0;
    mat2.elements[1][0] = 0;
    mat2.elements[1][1] = 3;
    mat2.elements[1][2] = 0;
    mat2.elements[2][0] = 0;
    mat2.elements[2][1] = 0;
    mat2.elements[2][2] = 3;

    mat2 = mat_inverse(&mat2);
    mat_print(&mat2);

    matrix mat = mat_mul(&mat1, &mat2);

    mat = mat_add(&mat1, &mat2);

    mat_free(&mat1);
    mat_free(&mat2);
    mat_free(&mat);

    return 0;
}

matrix mat_init(int rows, int columns)
{
    matrix mat;
    mat.rows = rows;
    mat.columns = columns;
    mat.elements = calloc(rows, sizeof(float*));
    int i, j;
    for (i = 0; i < rows; i++)
    {
	mat.elements[i] = calloc(columns, sizeof(float));
    }
    return mat;
}

void mat_print(matrix* mat)
{
    int i, j;
    for (i = 0; i < mat->rows; i++)                 {                                                  for (j= 0; j < mat->columns; j++)
	{
	    printf("%.2f ", mat->elements[i][j]);
	}
	printf("\n");
    }
    printf("\n");
}

void mat_free(matrix* mat)
{
    int i;
    for (i = 0; i < mat->rows; i++)
    {
        free(mat->elements[i]);
    }
    free(mat->elements);
}

matrix mat_mul(matrix* mat1, matrix* mat2)
{
    matrix mat = {0};
    if (mat1->columns != mat2->rows)
    {
	printf("Multiplication dimension error\n");
	return mat;
    }
    mat = mat_init(mat1->rows, mat2->columns);
    int i, j, m;
    float mul = 0;
    for (i = 0; i < mat.rows; i++)
    {
	for (j = 0; j < mat.columns; j++)
	{
	    for (m = 0; m < mat1->columns; m++)
	    {
		mul += mat1->elements[i][m] * mat2->elements[m][j];
	    }
	    mat.elements[i][j] = mul;
	    mul = 0;
	}
    }
    return mat;
}

matrix mat_add(matrix* mat1, matrix* mat2)
{
    matrix mat = {0};
    int i, j;
    if ((mat1->rows != mat2->rows) || (mat1->columns != mat2->columns))
    {
	printf("Addition dimension error\n");
	return mat;
    }
    mat = mat_init(mat1->rows, mat1->columns);
    for (i = 0; i < mat1->rows; i++)
    {
	for (j = 0; j < mat1->columns; j++)
	{
	    mat.elements[i][j] = mat1->elements[i][j] + mat2->elements[i][j];
	}
    }
    return mat;
}

matrix mat_sub(matrix* mat1, matrix* mat2)
{
    matrix mat = {0};                              int i, j;
    if ((mat1->rows != mat2->rows) || (mat1->columns != mat2->columns))                           {                                                  printf("Addition dimension error\n");          return mat;                                }                                              mat = mat_init(mat1->rows, mat1->columns);     for (i = 0; i < mat1->rows; i++)               {                                                  for (j = 0; j < mat1->columns; j++)            {                                                  mat.elements[i][j] = mat1->elements[i][j] - mat2->elements[i][j];
        }                                          }                                              return mat;                                }

float det(matrix* mat)
{
    float determinant = 0, element;

    if (mat->rows != mat->columns)
    {
	printf("Determinant dimension error\n");
	return determinant;
    }

    if (mat->rows == 2)
    {
	determinant = mat->elements[0][0] * mat->elements[1][1] - mat->elements[0][1] * mat->elements[1][0];
	return determinant;
    }

    int i, j, k;
    for (i = 0; i < mat->columns; i++)
    {
	element = mat->elements[0][i];
	if (i % 2 != 0)
	    element *= -1;

	matrix new = mat_init(mat->rows - 1, mat->columns - 1);
	for (j = 0; j < new.rows; j++)
	{
	    for(k = 0; k < new.columns + 1; k++)
	    {
		if (k < i)
		    new.elements[j][k] = mat->elements[j + 1][k];
		if (k == i)
		    continue;
		if (k > i)
		    new.elements[j][k - 1] = mat->elements[j + 1][k];
	    }
	}

	determinant += element * det(&new);
    }
    return determinant;
}

matrix mat_cofactors(matrix* mat)
{
    int i, j, m, n;
    matrix cof = mat_init(mat->rows, mat->columns);
    matrix new = mat_init(mat->rows - 1, mat->columns - 1);;
    for (i = 0; i < mat->rows;i++)
    {
	for(j = 0; j < mat->columns; j++)
	{
	    for (m = 0; m < mat->rows; m++)
	    {
		for (n = 0; n < mat->columns; n++)
		{
		    if (m == i || n == j)
			continue;
		    if (m < i && n < j)
			new.elements[m][n] = mat->elements[m][n];
		    if (m < i && n > j)
			new.elements[m][n - 1] = mat->elements[m][n];
		    if (m > i && n < j)
			    new.elements[m - 1][n] = mat->elements[m][n];
		    if (m > i && n > j)
			    new.elements[m - 1][n - 1] = mat->elements[m][n];
		}
	    }
	    cof.elements[i][j] = det(&new);
	    if ((i * mat->rows + j) % 2 != 0)
	        cof.elements[i][j] *= -1;
	}
    }
    return cof;
}

matrix mat_transpose(matrix* mat)
{
    matrix trn = mat_init(mat->columns, mat->rows);

    int i,  j;
    for(i = 0; i < mat->columns; i++)
    {
	for(j = 0; j < mat->rows; j++)
	{
	    trn.elements[i][j] = mat->elements[j][i];
	}
    }

    return trn;
}

void mat_scalar_mul(matrix* mat, float a)
{
    int i, j;
    for (i = 0; i < mat->rows; i++)
    {
	for (j = 0; j < mat->columns; j++)
	{
	    mat->elements[i][j] *= a;
	}
    }
}

matrix mat_inverse(matrix* mat)
{
    matrix inverse = {0};
    if (mat->rows != mat->columns)
    {
	printf("Inverse dimension error\n");
	return inverse;
    }

    float determinant = det(mat);
    if (determinant == 0)
    {
	printf("Inverse error, singular matrix\n");
	return inverse;
    }

    inverse = mat_cofactors(mat);
    inverse = mat_transpose(&inverse);
    mat_scalar_mul(&inverse, 1 / determinant);
    return inverse;

}
