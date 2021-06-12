
#include "Matrix.h"


#define DIMENSION_ERR_MSG "Invalid matrix dimensions.\n"
#define ZERO_DIVISION_ERR_MSG "Division by zero.\n"
#define INDEX_ERR_MSG "Index out of range.\n"
#define STREAM_ERR_MSG "Error loading from input stream.\n"
#define BAD_ALLOC_ERR_MSG "Allocation failed.\n"



enum Failures {SUCCESS,TEST1FAIL, TEST2FAIL, TEST3FAIL, TEST4FAIL, TEST5FAIL,
    TEST6FAIL, TEST7FAIL, TEST8FAIL};

int Test1();
int Test2();
int Test3();
int Test4();
int Test5();
int Test6();
int Test7();
int Test8();

int main() {
  std::cout<< "Test 1: constructors & destructors"<< std::endl;
  int test1_result = Test1();
  if(test1_result != SUCCESS){
    std::cout<< "TEST 1 FAILED!"<< std::endl<< std::endl;
    return test1_result;
  }
  std::cout<< "TEST 1 PASSED!"<< std::endl<< std::endl;

  std::cout << "Test 2: operator ="<<std::endl;
  int test2_result = Test2();
  if(test2_result != SUCCESS){
    std::cout<< "TEST 2 FAILED!"<< std::endl<< std::endl;
    return test2_result;
  }
  std::cout<< "TEST 2 PASSED!"<< std::endl<< std::endl;

  std::cout << "Test 3: operator * for matrices and scalars, *= matrices and "
               "scalars"<<std::endl;
  int test3_result = Test3();
  if(test3_result != SUCCESS){
    std::cout<< "TEST 3 FAILED!"<< std::endl<< std::endl;
    return test3_result;
  }
  std::cout<< "TEST 3 PASSED!"<< std::endl<< std::endl;

  std::cout << "Test 4: operator / and /=" <<std::endl;
  int test4_result = Test4();
  if(test4_result != SUCCESS){
    std::cout << "TEST 4 FAILED!"<< std::endl<< std::endl;
    return test4_result;
  }
  std::cout<< "TEST 4 PASSED!"<< std::endl<< std::endl;

  std::cout << "Test 5: operator +, += for Matrices and += for scalar"
  <<std::endl;
  int test5_result = Test5();
  if(test5_result != SUCCESS){
    std::cout << "TEST 5 FAILED!"<< std::endl<< std::endl;
    return test5_result;
  }
  std::cout<< "TEST 5 PASSED!"<< std::endl<< std::endl;

  std::cout << "Test 6: operator == and !="<<std::endl;
  int test6_result = Test6();
  if(test6_result != SUCCESS){
    std::cout << "TEST 6 FAILED!"<< std::endl<< std::endl;
    return test6_result;
  }
  std::cout<< "TEST 6 PASSED!"<< std::endl<< std::endl;

  std::cout << "Test 7: operator () and []"<<std::endl;
  int test7_result = Test7();
  if(test7_result != SUCCESS){
    std::cout << "TEST 7 FAILED!"<< std::endl<< std::endl;
    return test7_result;
  }
  std::cout<< "TEST 7 PASSED!"<< std::endl<< std::endl;

  std::cout << "Test 8: operator << and >> and print()"<<std::endl;
  int test8_result = Test8();
  if(test8_result != SUCCESS){
    std::cout << "TEST 8 FAILED!"<< std::endl<< std::endl;
    return test8_result;
  }
  std::cout<< "TEST 8 PASSED!"<< std::endl<< std::endl;


  std::cout <<"ALL TESTS PASSED! :)"<<std::endl;
  // TODO missing vectorize and print
  // TODO add check for >> if the input not in one line

}
int Test8() {
  Matrix mat(4,4);
  std::cin >> mat;
  std::cout << "check that both prints are equal"<<std::endl;
  std::cout << mat;
  std::cout<<std::endl << std::endl;
  mat.Print();
  std::cout<<std::endl << std::endl;
  return SUCCESS;
}

int Test7() {
  Matrix m1(5,5);
  float* m1_vals = m1.GetMatrix();
  for(int i = 0; i < 25; ++i){
    m1_vals[i] = (float)i;
  }

  for(int i = 0; i < 5; ++i){
    for(int j = 0; j < 5; ++j){
      if(m1(i,j) != (float)(i*5+j)){
        std::cerr <<"operator () returned incorrect value" << std::endl;
        return TEST7FAIL;
      }
    }
  }

  for(int i = 0; i < 5; ++i){
    for(int j = 0; j < 5; ++j){
      m1(i,j) = -m1(i,j);
    }
  }
  for(int i = 0; i < 5; ++i){
    for(int j = 0; j < 5; ++j){
      if(m1(i,j) != (float)(-(i*5+j))){
        std::cerr << "operator () for non constant matrix didnt return "
                     "reference correctly"<<std::endl;
        return TEST7FAIL;
      }
    }
  }

  for(int i = 0; i < 25; ++i){
    if(m1[i] != (float)-i){
      std::cerr <<"operator [] returned incorrect value"<<
      std::endl;
      return TEST7FAIL;
    }
  }
  m1[5] = 0;
  if(m1_vals[5] != 0){
    std::cerr <<"operator [] for non constant matrix didnt return reference "
                "correctly" << std::endl;
    return TEST7FAIL;
  }

  try{
    m1(6,6) = 4;
  }catch (const MatrixException &err){
    std::string err_msg = err.what();
    if(err_msg != INDEX_ERR_MSG){
      std::cerr <<"returned incorrect error message for operator ()"<<std::endl;
      return TEST7FAIL;
    }
  }
  catch (const std::exception &err){
    std::cerr <<"wrong exception thrown in operator()"<<std::endl;
    return TEST7FAIL;
  }

  try{
    m1(1,-3) = 4;
  }catch (const MatrixException &err){
    std::string err_msg = err.what();
    if(err_msg != INDEX_ERR_MSG){
      std::cerr <<"returned incorrect error message for operator ()"<<std::endl;
      return TEST7FAIL;
    }
  }
  catch (const std::exception &err){
    std::cerr <<"wrong exception thrown in operator()"<<std::endl;
    return TEST7FAIL;
  }

  try{
    m1[30] = 4;
  }catch (const MatrixException &err){
    std::string err_msg = err.what();
    if(err_msg != INDEX_ERR_MSG){
      std::cerr <<"returned incorrect error message for operator []"<<std::endl;
      return TEST7FAIL;
    }
  }
  catch (const std::exception &err){
    std::cerr <<"wrong exception thrown in operator[]"<<std::endl;
    return TEST7FAIL;
  }

  try{
    m1[-2] = 4;
  }catch (const MatrixException &err){
    std::string err_msg = err.what();
    if(err_msg != INDEX_ERR_MSG){
      std::cerr <<"returned incorrect error message for operator []"<<std::endl;
      return TEST7FAIL;
    }
  }
  catch (const std::exception &err){
    std::cerr <<"wrong exception thrown in operator[]"<<std::endl;
    return TEST7FAIL;
  }
  return SUCCESS;
}

int Test6() {
  Matrix m1(3,3);
  float* m1_vals = m1.GetMatrix();
  for(int i = 0; i < 9; ++i){
    m1_vals[i] = 10;
  }

  Matrix m2(3,3);
  float* m2_vals = m2.GetMatrix();
  for(int i = 0; i < 9; ++i){
    m2_vals[i] = 10;
  }

  Matrix m3(3,3);
  float* m3_vals = m3.GetMatrix();
  for(int i = 0; i < 9; ++i){
    m3_vals[i] = 11;
  }

  Matrix m4(4,4);
  float* m4_vals = m4.GetMatrix();
  for(int i = 0; i < 16; ++i){
    m4_vals[i] = 10;
  }

  if(!(m1 == m2)){
    std::cerr << "operator == gave incorrect result" <<std::endl;
    return TEST6FAIL;
  }
  if(!(m1 != m3)){
    std::cerr << "operator != gave incorrect result" <<std::endl;
    return TEST6FAIL;
  }
  if(m1 == m4){
    std::cerr << "operator == gave incorrect result" <<std::endl;
    return TEST6FAIL;
  }
  return SUCCESS;
}
int Test5() {
  Matrix m1(3,3);
  float* m1_vals = m1.GetMatrix();
  for(int i = 0; i < 9; ++i){
    m1_vals[i] = 10;
  }
  Matrix m2 =m1 / 2;

  Matrix m1_2 = m1 + m2;
  float* m1_2_vals = m1_2.GetMatrix();
  for(int i = 0; i < 9; ++i){
    if(m1_2_vals[i] != 15){
      std::cerr << "operator + gave incorrect result" <<std::endl;
      return TEST5FAIL;
    }
  }
  Matrix m3(2,3);
  try{
    Matrix m4 = m1 + m3;
  }catch (const MatrixException &err){
    std::string dim_msg = err.what();
    if(dim_msg != DIMENSION_ERR_MSG){
      std::cerr <<"in operator + exeption in case of incorrect dimensions "
                  "gave incorrect error message"<<std::endl;
      return TEST5FAIL;
    }
  }
  catch (std::exception &err){
    std::cerr <<"incorrect exception thrown" <<std::endl;
    return TEST5FAIL;
  }

  m1 += m2;
  float* new_m1_vals = m1.GetMatrix();
  for(int i = 0; i < 9; ++i){
    if(new_m1_vals[i] != 15){
      std::cerr << "operator += gave incorrect output"<<std::endl;
      return TEST5FAIL;
    }
  }
  try{
    m1 += m3;
  }catch (const MatrixException &err){
    std::string dim_msg = err.what();
    if(dim_msg != DIMENSION_ERR_MSG){
      std::cerr <<"in operator += exeption in case of incorrect dimensions "
                  "gave incorrect error message"<<std::endl;
      return TEST5FAIL;
    }
  }
  catch (std::exception &err){
    std::cerr <<"incorrect exception thrown" <<std::endl;
    return TEST5FAIL;
  }

  m1 += 5;
  float* new_m1_vals2 = m1.GetMatrix();
  for(int i = 0; i < 9; ++i){
    if(new_m1_vals2[i] != 20){
      std::cerr << "operator += with scalar gave incorrect output"<<std::endl;
      return TEST5FAIL;
    }
  }
  return SUCCESS;
}

int Test4() {
  Matrix m1(3, 3);
  float *m1_vals = m1.GetMatrix();
  for (int i = 0; i < 9; ++i) {
    m1_vals[i] = 10;
  }
  Matrix m2 = m1 / 4;
  float *m2_vals = m2.GetMatrix();
  for (int i = 0; i < 9; ++i) {
    if (m2_vals[i] != 2.5) {
      std::cerr << "operator / failed of gave wrong output" << std::endl;
      return TEST4FAIL;
    }
  }

  m1 /= 2;
  for (int i = 0; i < 9; ++i) {
    if (m1_vals[i] != 5) {
      std::cerr << "operator /= failed of gave wrong output" << std::endl;
      return TEST4FAIL;
    }
  }

  try {
    m1 /= 0;
  } catch (const MatrixException &err) {
    std::string error_msg = err.what();
    if (error_msg != ZERO_DIVISION_ERR_MSG) {
      std::cerr << "exception in case of division by 0 gave wrong error "
                   "message in /= operator" << std::endl;
      return TEST4FAIL;
    }
  }
  catch (std::exception &err){
    std::cerr <<"incorrect exception thrown" <<std::endl;
    return TEST4FAIL;

  }
  try {
    Matrix m3 = m1 / 0;
  } catch (const MatrixException &err) {
    std::string error_msg2 = err.what();
    if (error_msg2 != ZERO_DIVISION_ERR_MSG) {
      std::cerr << "exception in case of division by 0 gave wrong error "
                   "message in / operator" << std::endl;
      return TEST4FAIL;
    }
  }
  catch (std::exception &err){
    std::cerr <<"incorrect exception thrown" <<std::endl;
    return TEST4FAIL;

  }
  return SUCCESS;
}

int Test3() {
  Matrix m(3,3);
  float* m_vals = m.GetMatrix();
  for(int i = 0; i < 9; i++){
    m_vals[i] = 2;
  }

  Matrix m2 = m * 3;
  Matrix m3 = 3 * m;
  float* m2_vals = m2.GetMatrix();
  float* m3_vals = m3.GetMatrix();
  for(int i = 0; i < 9; i++){
    if(m2_vals[i] != 6 ){
      std::cerr << "multiplication with scalar on the left failed" << std::endl;
      return TEST3FAIL;
    }
  }
  for(int i = 0; i < 9; i++){
    if(m3_vals[i] != 6 ){
      std::cerr << "multiplication with scalar on the right failed" <<
      std::endl;
      return TEST3FAIL;
    }
  }

  m *= 5;
  auto m_vals_updated = m.GetMatrix();
  for(int i = 0; i < 9; ++i){
    if(m_vals_updated[i] != 10){
      std::cerr << " matrix wasnt updated correctly with operator *=" <<
      std::endl;
      return TEST3FAIL;
    }
  }

  Matrix m4 = m * m2;
  auto m4_vals = m4.GetMatrix();
  for(int i = 0; i < 9;++i){
    if(m4_vals[i] != 180){
      std::cerr <<"matrix multiplication * returned incorrect result" <<
      std::endl;
      return TEST3FAIL;
    }
  }

  Matrix m5(4,4);
  try{
    Matrix m6 = m * m5;
  }catch (const MatrixException &err){
    std::string err_msg = err.what();
    if(err_msg != DIMENSION_ERR_MSG){
      std::cerr <<"matrix multiplication * threw incorrect string for error" <<
                std::endl;
      return TEST3FAIL;
    }
  }
  catch(const std::exception &err){
    std::cerr <<"wrong exception thrown in operator *"<<std::endl;
    return TEST3FAIL;
  }

  m *= m2;
  for(int i = 0; i < 9;++i){
    if(m4_vals[i] != 180) {
      std::cerr << "matrix multiplication *= returned incorrect result" <<
                std::endl;
      return TEST3FAIL;
    }
  }
  try{
    m *= m5;
  }catch (const MatrixException &err){
    std::string err_msg = err.what();
    if(err_msg != DIMENSION_ERR_MSG){
      std::cerr <<"matrix multiplication *= threw incorrect string for error" <<
                std::endl;
      return TEST3FAIL;
    }
  }
  catch(const std::exception &err){
    std::cerr <<"wrong exception thrown in operator *="<<std::endl;
    return TEST3FAIL;
  }

  return SUCCESS;
}

int Test2() {
  Matrix m1(3,4);
  Matrix m2(2,2);
  float* m2_vals = m2.GetMatrix();
  m2_vals[0]=1;
  m2_vals[1]=2;
  m2_vals[2]=3;
  m2_vals[3]=4;
  m1 = m2;
  float* m1_vals = m1.GetMatrix();
  for(int i = 0; i < 4; i++){
    if(m1_vals[i] != (float)i+1){
      std::cerr <<"new values of m1 are not updated correctly" << std::endl;
      return TEST2FAIL;
    }
  }
  if(m1.GetRows() != 2 || m1.GetCols() != 2){
    std::cerr << "m1 rows or cols havent been updated correctly" << std::endl;
    return TEST2FAIL;
  }

  Matrix m3 = m2;
  float* m3_vals = m3.GetMatrix();
  for(int i = 0; i < 4; i++){
    if(m3_vals[i] != (float)i+1){
      std::cerr << "copy constructor using = isnt working" << std::endl;
      return TEST2FAIL;
    }
  }
  if(m3.GetRows() != 2 || m3.GetCols() != 2){
    std::cerr << "m3 rows or cols havent been updated correctly" << std::endl;
    return TEST2FAIL;
  }
  return SUCCESS;

}

int Test1() {
  Matrix no_args;
  if(no_args.GetRows() != 1 || no_args.GetCols() != 1){
    std::cerr << "incorrect number of rows or cols in default constructor\n"
    << std::endl;
    return TEST1FAIL;
  }
  int no_args_size = no_args.GetCols() * no_args.GetRows();
  float *no_args_vals = no_args.GetMatrix();
  for(int i = 0; i < no_args_size; ++i){
    if(no_args_vals[i] != 0){
      std::cerr << "matrix not initialized to 0" << std::endl;
      return TEST1FAIL;
    }
  }

  Matrix mat(3,4);
  if(mat.GetRows() != 3 || mat.GetCols() != 4){
    std::cerr << "incorrect number of rows or cols in mat constructor\n"
              << std::endl;
    return TEST1FAIL;
  }
  int mat_size = mat.GetCols() * mat.GetRows();
  float *mat_vals = mat.GetMatrix();
  for(int i = 0; i < mat_size; ++i){
    if(mat_vals[i] != 0){
      std::cerr << "matrix not initialized to 0" << std::endl;
      return TEST1FAIL;
    }
  }

  Matrix mat_copy(mat);
  if(mat_copy.GetRows() != 3 || mat_copy.GetCols() != 4){
    std::cerr << "incorrect number of rows or cols in mat_copy constructor\n"
              << std::endl;
    return TEST1FAIL;
  }
  int mat_copy_size = mat_copy.GetCols() * mat_copy.GetRows();
  float *mat_copy_vals = mat_copy.GetMatrix();
  for(int i = 0; i < mat_copy_size; ++i){
    if(mat_copy_vals[i] != 0){
      std::cerr << "matrix not initialized to 0" << std::endl;
      return TEST1FAIL;
    }
  }
  try{
    Matrix too_big(100000000, 10000000);
  }catch(const MatrixException &err){
    const std::string err_string = err.what();
    if(err_string !=BAD_ALLOC_ERR_MSG){
      std::cerr << "check exception in allocation failure- wrong string or "
                   "wrong exception thrown" << std::endl;
      return TEST1FAIL;
    }
  }
  catch(const std::exception &err){
    std::cerr << "wrong exception thrown in constructor" << std::endl;
    return TEST1FAIL;
  }
  return SUCCESS;
}

