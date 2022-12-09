#include <iostream>
#include <Eigen/Dense>

int main()
{
    typedef Eigen::Matrix<float, 4, 3> Matrix43f;
    Matrix43f A;
    Eigen::Vector4f b;
    A << 1,2,3,  4,5,6,  7,8,10, 11,12,13;
    b << 3, 3, 4, 6;
    std::cout << "Here is the matrix A:\n" << A << std::endl;
    std::cout << "Here is the vector b:\n" << b << std::endl;
    Eigen::Vector3f x = A.colPivHouseholderQr().solve(b);
    std::cout << "The solution is:\n" << x << std::endl;


    // dynamic matrix size
    int rows = 4;
    int cols = 3;
    Eigen::MatrixXd A_dyn(rows, cols);
    A_dyn << 1,2,3,  4,5,6,  7,8,10, 11,12,13;
    Eigen::VectorXd b_dyn(rows);
    b_dyn << 3, 3, 4, 6;
    Eigen::VectorXd x_dyn = A_dyn.colPivHouseholderQr().solve(b_dyn);
    std::cout << "The solution is:\n" << x_dyn << std::endl;

}