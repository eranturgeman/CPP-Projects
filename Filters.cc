/**
 * @file Filters.cc
 * @author  Eran Turgeman <eran.turgeman@mail.huji.ac.il>
 *
 * @brief implementation file for Filters.h file
 *
 * @section LICENSE
 * This program is private and was made for the 2020 67315 course
 */

#include "Filters.h"
#include <cmath>

/**
 * MAX_COLOR maximum color value
 */
#define MAX_COLOR 256

/**
 * MIN_COLOR minimum color value
 */
#define MIN_COLOR 0

/**
 * ALLOC_FAIL_MSG message for MatrixException in case of a allocation failure
 */
#define ALLOC_FAIL_MSG "Allocation failed.\n"

/**
 * BLUR_MATRIX_DATA numeric values for convolution matrix for Blur function
 */
#define BLUR_MATRIX_DATA "0.0625 0.125 0.0625 0.125 0.25 0.125 0.0625 0.125 "\
"0.0625 "

/**
 * CONVOLUTION_MATRIX_SIZE amount of rows and columns in convolution matrix
 */
#define CONVOLUTION_MATRIX_SIZE 3

/**
 * SOBEL_X_MATRIX_DATA numeric values for convolution x matrix for Sobel
 * function
 */
#define SOBEL_X_MATRIX_DATA "0.125 0 -0.125 0.25 0 -0.25 0.125 0 -0.125 "

/**
 * SOBEL_Y_MATRIX_DATA numeric values for convolution y matrix for Sobel
 * function
 */
#define SOBEL_Y_MATRIX_DATA "0.125 0.25 0.125 0 0 0 -0.125 -0.25 -0.125 "



/**
 * creating the averages array with the average values for the Quantization
 * function
 * @param levels number of levels wanted in the division of the colors
 * @param colors_in_level number of colors in each level
 * @return returns new allocated array with the averages
 */
int *GetAverages(int levels, int colors_in_level) noexcept;

/**
 * creating a convolution matrix for different filters given the matrix data
 * @param data string contains matrix data for the requested filter
 * @return matrix with the needed data
 */
Matrix CreateConvolutionMatrix(const std::string& data);

/**
 * preform convolution process on the whole matrix
 * @param to_update matrix to update in the values after convolution
 * @param original_matrix matrix preforming convolution on it's values
 * @param conv_mat convolution matrix
 */
void MatrixConvolution(Matrix &to_update, const Matrix &original_matrix,
                       const Matrix &conv_mat) noexcept;

/**
 * preform convolution process on single cell
 * @param row cell's row
 * @param col cell's column
 * @param matrix matrix with the cell's value
 * @param conv_mat matrix for the convolution operation
 * @return value of the process's result
 */
float CellConvolution(int row, int col, const Matrix &matrix, const Matrix
&conv_mat) noexcept;

/**
 * preforming quantization filter on a given matrix
 * @param image matrix representing image colors by numeric values
 * @param levels number of levels we want to divide the colors by
 * @return new matrix which is the result of the process
 */
Matrix Quantization(const Matrix& image, int levels){
  int colors_in_level = MAX_COLOR / levels;
  Matrix new_mat(image.GetRows(), image.GetCols());
  int *avg_array = GetAverages(levels, colors_in_level);
  if(avg_array == nullptr){
    throw MatrixException(ALLOC_FAIL_MSG);
  }
  int cell_amount = image.GetRows() * image.GetCols();
  for(int i = 0; i < cell_amount; ++i){
    int avg_index = std::floor(image[i] / (float)colors_in_level);
    new_mat[i] = (float)avg_array[avg_index];
  }
  delete[] avg_array;
  return new_mat;
}
/**
 * documentation above
 */
int *GetAverages(const int levels, const int colors_in_level) noexcept{
    auto *avg_array = new(std::nothrow) int[levels];
    if(avg_array == nullptr){
      return nullptr;
    }
    int cell = 0;
    for (int i = 0; i < levels; ++i){
      avg_array[i] =std::floor((cell + (cell + colors_in_level - 1))/2);
      cell += colors_in_level;
    }
    return avg_array;
}

/**
 * preform Blur filter on a matrix representing an image
 * @param image matrix representing image by numeric values
 * @return new matrix which is the result of the process
 */
Matrix Blur(const Matrix& image){
  Matrix blurred(image.GetRows(), image.GetCols());
  Matrix blur_matrix = CreateConvolutionMatrix(BLUR_MATRIX_DATA);
  MatrixConvolution(blurred, image, blur_matrix);
  return blurred;
}

/**
 * documentation above
 */
Matrix CreateConvolutionMatrix(const std::string& data) {
  Matrix conv_matrix(CONVOLUTION_MATRIX_SIZE, CONVOLUTION_MATRIX_SIZE);
  std::string number;
  int counter = 0;
  for(auto c: data){
    if(c == ' '){
      conv_matrix[counter++] = std::stof(number);
      number = "";
    }else{
      number += c;
    }
  }
  return conv_matrix;
}

/**
 * documentation above
 */
void MatrixConvolution(Matrix &to_update,const Matrix &original_matrix, const
Matrix &conv_mat) noexcept{
  for(int i = 0; i < to_update.GetRows(); ++i){
    for(int j = 0; j < to_update.GetCols(); ++j){
      to_update(i,j) = CellConvolution(i, j, original_matrix, conv_mat);
    }
  }
}

/**
 * documentation above
 */
float CellConvolution(int row, int col, const Matrix &matrix, const Matrix
&conv_mat) noexcept{
  float result = 0;
  for(int i = 0; i < 3; ++i){
    for(int j = 0; j < 3; ++j){
      if(row + i - 1 < 0 || row + i - 1 >=matrix.GetRows() || col + j - 1 < 0
      || col + j - 1 >= matrix.GetCols()){
        continue;
      }
      result += conv_mat(i,j) * matrix(row + i - 1, col + j - 1);
    }
  }
  return std::rintf(result);
}

/**
 * preform Sobel filter on a matrix representing an image
 * @param image matrix representing image by numeric values
 * @return new matrix which is the result of the process
 */
Matrix Sobel(const Matrix& image){
  Matrix sobel_x(image.GetRows(), image.GetCols());
  Matrix sobel_y(image.GetRows(), image.GetCols());
  Matrix conv_x = CreateConvolutionMatrix(SOBEL_X_MATRIX_DATA);
  Matrix conv_y = CreateConvolutionMatrix(SOBEL_Y_MATRIX_DATA);
  MatrixConvolution(sobel_x, image, conv_x);
  MatrixConvolution(sobel_y, image, conv_y);
  Matrix result = sobel_x + sobel_y;
  for(int i = 0; i < result.GetRows() * result.GetCols(); ++i){
    if(result[i] < MIN_COLOR){
      result[i] = MIN_COLOR;
    }
    if(result[i] >= MAX_COLOR){
      result[i] = MAX_COLOR - 1;
    }
  }
  return result;
}
