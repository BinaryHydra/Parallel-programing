#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <cstring>
#include "display_utils.h"

void write_matrix_to_file(double **matrix, int matrix_dimention, double max_value, char *filename, bool use_abs_value)
{
    // burrowed from
    // https://stackoverflow.com/questions/22580812/writing-a-png-in-c/22580958#22580958
    FILE *imageFile;
    int x, y, height = matrix_dimention, width = matrix_dimention, MAX_PGM_SUPPORTED_VALUE = 255;
    // WTF c++, why string iterpolation is only in c++20?
    imageFile = fopen(filename, "wb");
    if (imageFile == NULL)
    {
        perror("ERROR: Cannot open output file");
        exit(EXIT_FAILURE);
    }
    if (!use_abs_value)
    {
        max_value = 0.00001; // does not support lower value
        int max_x = 0, max_y = 0;
        for (int i = 0; i < matrix_dimention; i++)
        {
            for (int j = 0; j < matrix_dimention; j++)
            {
                if (matrix[i][j] > max_value)
                {
                    max_value = matrix[i][j];
                    max_x = i;
                    max_y = j;
                }
            }
        }
    }
    // pgm limitation, cannot be 0
    max_value = std::max(max_value, 0.00001);

    fprintf(imageFile, "P5\n");                          // P5 filetype
    fprintf(imageFile, "%d %d\n", width, height);        // dimensions
    fprintf(imageFile, "%d\n", MAX_PGM_SUPPORTED_VALUE); // Max pixel

    for (x = 0; x < height; x++)
    {
        for (y = 0; y < width; y++)
        {
            double pixel = matrix[x][y];
            int pixel_value = (int)(pixel / max_value * MAX_PGM_SUPPORTED_VALUE);
            fputc(pixel_value, imageFile);
        }
    }

    fclose(imageFile);
}

void print_matrix(double **matrix, int matrix_dimention)
{
    for (int i = 0; i < matrix_dimention; i++)
    {
        for (int j = 0; j < matrix_dimention; j++)
        {
            printf(" %.2f", matrix[i][j]);
        }
        printf("\n");
    }
}

char *generate_filename(char const *filename_postfix)
{
    char *filename = (char *)malloc(100 * sizeof(char));
    strcpy(filename, "images/image_");
    strcat(filename, filename_postfix);
    strcat(filename, ".pgm");
    return filename;
}

void save_to_file(double **matrix, int matrix_dimention, double max_value, int index, bool use_abs_value)
{
    char index_str[5];
    snprintf(index_str, 6, "%05d", index);
    char *filename = generate_filename(index_str);
    printf("%s\n", filename);
    write_matrix_to_file(matrix, matrix_dimention, matrix_dimention, filename, use_abs_value);
    free(filename);
}