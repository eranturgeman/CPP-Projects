/**
 * @file Matrix.h
 * @author  Eran Turgeman <eran.turgeman@mail.huji.ac.il>
 *
 * @brief h file for Matrix class
 *
 * @section LICENSE
 * This program is private and was made for the 2020 67315 course
 */

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#ifndef EX5__MATRIX_H_
#define EX5__MATRIX_H_
#include "MatrixException.h"


class Matrix
{
  int _rows;
  int _cols;
  int _cell_amount;
  float *_matrix;

  /**
   * PRIVATE FUNCTION: getter for _cell_amount
   * @return _cell_amount
   */
  int GetCellAmount() const noexcept{
    return _cell_amount;
  }

  /**
   * calculating cell content in a during matix multiplication according to
   * matrices multiplication rules
   * @param mat1 matrix being multiplied on the right
   * @param mat2 matrix being multiplied on the left
   * @param row row i in the multiplication for calculating cell (i,j)
   * @param col column j in the multiplication for calculating cell (i,j)
   * @param len number of summaries we need in the calculation (number of
   * columns on the right hand matrix)
   * @return calculation result according to matrices multiplication rules
   */
  static float CalculateCell(const Matrix &mat1, const Matrix &mat2, int
  row, int col, int len) noexcept;


  /**
   * multiplying every cell in the matrix by given scalar
   * @param to_update matrix to multiply it's values
   * @param scalar scalar to multiply by
   */
  static void MultiplyByScalar(Matrix& to_update, float scalar) noexcept;


 public:

  /**
   * constructor for Matrix object in size of rows*cols. contains check for non-negative values
   * @param rows number of rows for the Matrix
   * @param cols number of columns for the Matrix
   */
  Matrix(int rows, int cols);

  /**
   * default constructor initiating new Matrix in size of 1*1
   */
  Matrix();

  /**
   * copy constructor creating new Matrix using other given Matrix
   * @param m given Matrix to create from
   */
  Matrix(const Matrix &m);

  /**
   * destructor for Matrix object
   */
  ~Matrix();

  /**
   * getter for matrix._rows
   * @return number of rows in matrix
   */
  int GetRows() const noexcept;

  /**
   * getter for matrix._cols
   * @return number of columns in matrix
   */
  int GetCols() const noexcept;

  /**
   * prints the matrix according to format:
   * space between after element(except last in row)
   * new line after every row(except last row)
   */
  void Print() const noexcept;

  /**
   * transforms a matrix into column vector.
   * IMPORTANT: when activated this action is irreversible due to chane in
   * dimensions
   */
  Matrix& Vectorize() noexcept; //no need for const version

  /**
   *
   * @param other Matrix object to assign it's values into the Matrix we
   * called this operator at
   * @return reference to the Matrix we called the operator at after updating
   */
  Matrix& operator=(const Matrix& other);//no need for const version


  /**
   *
   * @param other Matrix object to multiply with this on the right in
   * dimensions are valid
   * @return new Matrix object whis is the result of the Matrices multiplication
   */
  Matrix operator*(const Matrix& other) const;

  /**
   * multiplies matrix we called the operator at with other matrix accordint
   * to matrices multiplications rules
   * @param other matrix to multiply with
   * @return reference to the updated matrix
   */
  Matrix& operator*=(const Matrix& other);

  /**
   * multiply all matrix cells in scalar, multiplication on the right and
   * create new matrix with the result values
   * @param scalar scalar to multiply
   * @return new Matrix with multiplied values
   */
  Matrix operator*(float scalar) const;

  /**
   * multiply all matrix cells in scalar, multiplication on the left and
   * create new matrix with the result values
   * @param scalar scalar to multiply
   * @param matrix matrix to pultiply
   * @return new Matrix with multiplied values
   */
  friend Matrix operator*(float scalar, const Matrix& matrix);

  /**
   * multiplies Matrix by scalar and updates the given matrix itself
   * @param scalar scalar to multiply
   * @return reference to the matrix that have been multiplied
   */
  Matrix& operator*=(float scalar) noexcept;//no need for const version

  /**
   * creates new copy of the matrix we called the operator at and divides all
   * its values by scalar if scalar !=0
   * @param scalar scalar to divide by
   * @return new matrix after division
   */
  Matrix operator/(float scalar) const;

  /**
   * divides Matrix by scalar and updates the given matrix
   * @param scalar scalar to divide by
   * @return reference to the matrix that have been divided
   */
  Matrix& operator/=(float scalar);//no need for const version

  /**
   * summing to matrices and insert values to new Matrix
   * @param other matrix to preform addition with
   * @return new Matrix with summation values
   */
  Matrix operator+(const Matrix& other) const;

  /**
   * adding given matrix values to the matrix we called the operator on
   * @param other matrix to add it's values
   * @return reference to updated matrix
   */
  Matrix& operator+=(const Matrix& other);//no need for const version

  /**
   * adds the value scalar for every value in the matrix
   * @param scalar number to add
   * @return reference to to updated matrix
   */
  Matrix& operator+=(float scalar) noexcept;//no need for const version

  /**
   * checks if two given matrices are equal
   * @param other matrix to compare to
   * @return true in case of equality, false otherwise
   */
  bool operator==(const Matrix& other) const noexcept;

  /**
   * checks if two given marices are not equal
   * @param othermatrix to compare to
   * @return true in case matrices are not equal, false otherwise
   */
  bool operator!=(const Matrix& other) const noexcept;

  /**
   * returns reference to the value in cell in place (i,j) of given matrix
   * @param i row number
   * @param j column number
   * @return reference to the cell in the requested spot
   */
  float& operator()(int i, int j);

  /**
   * returns the value of the matrix in place (i,j)
   * @param i row number
   * @param j column number
   * @return copy of the value in cell (i,j)
   */
  float operator()(int i, int j) const;

  /**
   * returns reference to a cell in the matrix by single index: index =
   * i*_rows+j
   * @param index index of the wanted cell
   * @return reference of the requested cell
   */
  float& operator[](int index);

  /**
   * return the value of matrix's cell by single index: index = i*_rows+j
   * @param index index of the wanted cell
   * @return copy of the value in the requested cell
   */
  float operator[](int index)const;

  /**
   * input stream operator taking float values and puts them into matrix in
   * the order they where given
   * @param is reference to input stream
   * @param matrix matrix to insert values into
   * @return reference to input stream
   */
  friend std::istream& operator>>(std::istream &is, Matrix& matrix);

  /**
   * output stream operator that send the matrix values by specific format
   * @param os output stream reference
   * @param matrix matrix to send
   * @return reference to output stream
   */
  friend std::ostream& operator<<(std::ostream &os, const Matrix& matrix) noexcept;
};



#endif //EX5__MATRIX_H_
