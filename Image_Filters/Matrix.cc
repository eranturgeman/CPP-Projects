/**
 * @file Matrix.cc
 * @author  Eran Turgeman <eran.turgeman@mail.huji.ac.il>
 *
 * @brief implementation file for Matrix class
 *
 * @section LICENSE
 * This program is private and was made for the 2020 67315 course
 */

#include "Matrix.h"

/**
 * MATRIX_DIMENSION_ERROR_MSG message for MatrixException in case of invalid
 * dimensions
 */
#define MATRIX_DIMENSION_ERROR_MSG "Invalid matrix dimensions.\n"

/**
 * ZERO_DIVISION_ERROR_MSG message for MatrixException in case of division
 * by 0
 */
#define ZERO_DIVISION_ERROR_MSG "Division by zero.\n"

/**
 * INDEX_RANGE_ERROR_MSG message for MatrixException in case of accessing
 * out of matrix range
 */
#define INDEX_RANGE_ERROR_MSG "Index out of range.\n"

/**
 * INPUT_STREAM_ERROR_MSG message for MatrixException in case of a problem
 * with input stream
 */
#define INPUT_STREAM_ERROR_MSG "Error loading from input stream.\n"

/**
 * ALLOC_FAIL_MSG message for MatrixException in case of a allocation failure
 */
#define ALLOC_FAIL_MSG "Allocation failed.\n"

/**
 * MATRIX_INITIAL_VALUE initial value for matrix
 */
#define MATRIX_INITIAL_VALUE 0


/**
 * constructor for Matrix object in size of rows*cols. contains check for non-negative values
 * @param rows number of rows for the Matrix
 * @param cols number of columns for the Matrix
 */
Matrix::Matrix(int rows, int cols) {
  if(rows <=0 || cols <=0){
    throw MatrixException(MATRIX_DIMENSION_ERROR_MSG);
  }
  try{
    _matrix = new float[rows * cols];
  }catch(const std::bad_alloc& err){
    throw MatrixException(ALLOC_FAIL_MSG);
  }

  for(int i = 0; i < rows * cols; ++i){
    _matrix[i] = MATRIX_INITIAL_VALUE;
  }
  _rows = rows;
  _cols = cols;
  _cell_amount = _rows * _cols;
}

/**
 * default constructor initiating new Matrix in size of 1*1
 */
Matrix::Matrix() {
  try{
    _matrix = new float[1];
  }catch(const std::bad_alloc& err){
    throw MatrixException(ALLOC_FAIL_MSG);
  }
  _matrix[0] = MATRIX_INITIAL_VALUE;
  _rows = 1;
  _cols = 1;
  _cell_amount = 1;
}

/**
 * copy constructor creating new Matrix using other given Matrix
 * @param m given Matrix to create from
 */
Matrix::Matrix(const Matrix &m) {
  //no need to check rows,cols > 0
  if(this != &m){
    try{
      _matrix = new float[m._rows * m._cols];
    }catch(const std::bad_alloc &err){
      throw MatrixException(ALLOC_FAIL_MSG);
    }
    _rows = m._rows;
    _cols = m._cols;
    _cell_amount = m._rows * m._cols;
    for(int idx = 0; idx < _rows * _cols; idx++){
      this->_matrix[idx] = m._matrix[idx];
    }
  }
}

/**
 * destructor for Matrix object
 */
Matrix:: ~Matrix() {
  delete[] _matrix;
}

/**
 * getter for matrix._rows
 * @return number of rows in matrix
 */
int Matrix::GetRows() const noexcept{
  return this->_rows;
}

/**
 * getter for matrix._cols
 * @return number of columns in matrix
 */
int Matrix::GetCols() const noexcept {
  return this->_cols;
}


/**
 * transforms a matrix into column vector.
 * IMPORTANT: when activated this action is irreversible due to chane in
 * dimensions
 */
Matrix& Matrix::Vectorize() noexcept{
  _rows = _rows * _cols;
  _cols = 1;
  return *this;
}

/**
 *
 * @param other Matrix object to assign it's values into the Matrix we
 * called this operator at
 * @return reference to the Matrix we called the operator at after updating
 */
Matrix& Matrix::operator=(const Matrix& other){
  if(this != &other){
    float* tmp_mat = nullptr;
    try{
      tmp_mat = new float[other._rows * other._cols];
    }catch(const std::bad_alloc &err){
      throw MatrixException(ALLOC_FAIL_MSG);
    }
    _cols = other._cols;
    _rows = other._rows;
    _cell_amount = other._cell_amount;
    delete[] _matrix;
    _matrix = tmp_mat;
    for(int i = 0; i < _rows * _cols; ++i){
      _matrix[i] = other._matrix[i];
    }
  }
  return *this;
}

/**
 *
 * @param other Matrix object to multiply with this on the right in
 * dimensions are valid
 * @return new Matrix object whis is the result of the Matrices multiplication
 */
Matrix Matrix::operator*(const Matrix& other) const{
  if(_cols != other._rows){
    throw MatrixException(MATRIX_DIMENSION_ERROR_MSG);
  }
  Matrix new_mat(_rows, other._cols);
  for(int i = 0; i < new_mat._rows; ++i){
    for(int j = 0; j < new_mat._cols; ++j){
      new_mat._matrix[i * new_mat._cols + j] = CalculateCell(*this, other,
                                                             i, j, _cols);
    }
  }
  return new_mat;
}

/**
 * multiplies matrix we called the operator at with other matrix accordint
 * to matrices multiplications rules
 * @param other matrix to multiply with
 * @return reference to the updated matrix
 */
Matrix& Matrix::operator*=(const Matrix& other){
  *this = (*this) * other;
  return *this;
}

/**
 * multiply all matrix cells in scalar, multiplication on the right and
 * create new matrix with the result values
 * @param scalar scalar to multiply
 * @return new Matrix with multiplied values
 */
Matrix Matrix::operator*(const float scalar) const{
  Matrix new_mat(*this);
  MultiplyByScalar(new_mat, scalar);
  return new_mat;
}

/**
 * multiply all matrix cells in scalar, multiplication on the left and
 * create new matrix with the result values
 * @param scalar scalar to multiply
 * @param matrix matrix to pultiply
 * @return new Matrix with multiplied values
 */
Matrix operator*(const float scalar, const Matrix& matrix){
  return matrix * scalar;
}

/**
 * multiplies Matrix by scalar and updates the given matrix itself
 * @param scalar scalar to multiply
 * @return reference to the matrix that have been multiplied
 */
Matrix& Matrix::operator*=(const float scalar) noexcept{
  MultiplyByScalar(*this, scalar);
  return *this;
}

/**
 * creates new copy of the matrix we called the operator at and divides all
 * its values by scalar if scalar !=0
 * @param scalar scalar to divide by
 * @return new matrix after division
 */
Matrix Matrix::operator/(const float scalar) const{
  if(scalar == 0){
    throw MatrixException(ZERO_DIVISION_ERROR_MSG);
  }
  return ((*this) * (1 / scalar));
}

/**
 * divides Matrix by scalar and updates the given matrix
 * @param scalar scalar to divide by
 * @return reference to the matrix that have been divided
 */
Matrix& Matrix::operator/=(const float scalar){
  if(scalar == 0){
    throw MatrixException(ZERO_DIVISION_ERROR_MSG);
  }
  return ((*this) *= (1 / scalar));
}

/**
 * summing to matrices and insert values to new Matrix
 * @param other matrix to preform addition with
 * @return new Matrix with summation values
 */
Matrix Matrix::operator+(const Matrix& other) const{
  if (this->_cols != other._cols || this->_rows != other._rows){
    throw MatrixException(MATRIX_DIMENSION_ERROR_MSG);
  }
  Matrix new_mat(_rows, _cols);
  for(int i = 0; i < _cell_amount; ++i){
    new_mat._matrix[i] = this->_matrix[i] + other._matrix[i];
  }
  return new_mat;
}

/**
 * adding given matrix values to the matrix we called the operator on
 * @param other matrix to add it's values
 * @return reference to updated matrix
 */
Matrix& Matrix::operator+=(const Matrix& other){
  if (this->_cols != other._cols || this->_rows != other._rows){
    throw MatrixException(MATRIX_DIMENSION_ERROR_MSG);
  }
  for(int i = 0; i < _cell_amount; ++i){
    this->_matrix[i] += other._matrix[i];
  }
  return *this;
}

/**
 * adds the value scalar for every value in the matrix
 * @param scalar number to add
 * @return reference to to updated matrix
 */
Matrix& Matrix::operator+=(const float scalar) noexcept{
  for(int i = 0; i < _cell_amount; ++i){
    this->_matrix[i] += scalar;
  }
  return *this;
}

/**
 * checks if two given matrices are equal
 * @param other matrix to compare to
 * @return true in case of equality, false otherwise
 */
bool Matrix::operator==(const Matrix& other) const noexcept{
  if(_rows != other._rows || _cols != other._cols){
    return false;
  }
  for(int i = 0; i < _cell_amount; ++i){
    if(_matrix[i] != other._matrix[i]){
      return false;
    }
  }
  return true;
}

/**
 * checks if two given marices are not equal
 * @param othermatrix to compare to
 * @return true in case matrices are not equal, false otherwise
 */
bool Matrix::operator!=(const Matrix& other) const noexcept{
  return !(*this == other);
}

/**
 * returns reference to the value in cell in place (i,j) of given matrix
 * @param i row number
 * @param j column number
 * @return reference to the cell in the requested spot
 */
float& Matrix::operator()(const int i, const int j){
  if(i < 0 || i > (_rows - 1) || j < 0 || j > (_cols - 1)){
    throw MatrixException(INDEX_RANGE_ERROR_MSG);
  }
  return _matrix[i * _cols + j];
}

/**
 * returns the value of the matrix in place (i,j)
 * @param i row number
 * @param j column number
 * @return copy of the value in cell (i,j)
 */
float Matrix::operator()(const int i, const int j) const{
  if(i < 0 || i > (_rows - 1) || j < 0 || j > (_cols - 1)){
    throw MatrixException(INDEX_RANGE_ERROR_MSG);
  }
  float tmp = _matrix[i * _cols + j];
  return tmp;
}

/**
 * returns reference to a cell in the matrix by single index: index =
 * i*_rows+j
 * @param index index of the wanted cell
 * @return reference of the requested cell
 */
float& Matrix::operator[](const int index){
  if(index < 0 || index >= _cell_amount){
    throw MatrixException(INDEX_RANGE_ERROR_MSG);
  }
  return _matrix[index];
}

/**
 * return the value of matrix's cell by single index: index = i*_rows+j
 * @param index index of the wanted cell
 * @return copy of the value in the requested cell
 */
float Matrix::operator[](int index) const{
  if(index < 0 || index >= _cell_amount){
    throw MatrixException(INDEX_RANGE_ERROR_MSG);
  }
  float tmp = _matrix[index];
  return tmp;
}

/**
 * input stream operator taking float values and puts them into matrix in
 * the order they where given
 * @param is reference to input stream
 * @param matrix matrix to insert values into
 * @return reference to input stream
 */
std::istream& operator >>(std::istream &is, Matrix& matrix){
  if(!is.good()){
    throw MatrixException(INPUT_STREAM_ERROR_MSG);
  }

  int counter = 0;
  float input_number;
  while(counter < matrix.GetCellAmount() && is >> input_number){
    matrix[counter] = input_number;
    counter++;
  }
  return is;
}

/**
 * output stream operator that send the matrix values by specific format
 * @param os output stream reference
 * @param matrix matrix to send
 * @return reference to output stream
 */
std::ostream& operator<<(std::ostream &os, const Matrix& matrix) noexcept{
  std::string output;
  int printed_counter = 0;
  int total_cells = matrix.GetCellAmount();
  int cols = matrix.GetCols();
  for(int i = 0; i < total_cells - 1; ++i){
    output += std::to_string(matrix[i]);
    printed_counter++;
    if(printed_counter % cols != 0){
      output += " ";
    }else{
      output += "\n";
    }
  }
  output += std::to_string(matrix[(int)total_cells - 1]);
  return os << output;
}

/**
 * prints the matrix according to format:
 * space between after element(except last in row)
 * new line after every row(except last row)
 */
void Matrix::Print() const noexcept{
  std::cout << *this;
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
float Matrix::CalculateCell(const Matrix &mat1, const Matrix &mat2, const
int row, const int col, const int len) noexcept {
  float sum = 0;
  for(int i = 0; i < len; ++i){
    sum += (mat1(row, i) * mat2(i, col));
  }
  return sum;
}

/**
 * multiplying every cell in the matrix by given scalar
 * @param to_update matrix to multiply it's values
 * @param scalar scalar to multiply by
 */
void Matrix::MultiplyByScalar(Matrix& to_update, const float scalar) noexcept{
  for(int i = 0; i < to_update._cell_amount; ++i){
    to_update._matrix[i] *= scalar;
  }
}





