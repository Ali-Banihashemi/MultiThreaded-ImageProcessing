#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>

void apply_horizantal_flip(cv::Mat *input)
{
    cv::Mat temp = input->clone();
    for (int row = 0; row < temp.rows; ++row)
    {
        for (int col = 0; col < temp.cols; ++col)
        {
            input->at<cv::Vec3b>(row, temp.cols - col - 1) = temp.at<cv::Vec3b>(row, col);
        }
    }
    temp.release();
    return;
}

void apply_vertical_flip(cv::Mat *input)
{
    cv::Mat temp = input->clone();
    for (int row = 0; row < temp.rows; ++row)
    {
        for (int col = 0; col < temp.cols; ++col)
        {
            input->at<cv::Vec3b>(temp.rows - row - 1, col) = temp.at<cv::Vec3b>(row, col);
        }
    }
    temp.release();
    return;
}

float pixel_value(float input)
{
    if (input > 255)
    {
        return 255;
    }
    else if (input < 0)
    {
        return 0;
    }
    return input;
}

int binary_value(int input)
{
    if (input > 127)
    {
        return 255;
    }
    else
    {
        return 0;
    }
}

void apply_sepia(cv::Mat *input)
{
    for (int row = 0; row < input->rows; ++row)
    {
        for (int col = 0; col < input->cols; ++col)
        {
            float blue = input->at<cv::Vec3b>(row, col)[2] * 0.131 +
                         input->at<cv::Vec3b>(row, col)[1] * 0.534 +
                         input->at<cv::Vec3b>(row, col)[0] * 0.272;
            float green = input->at<cv::Vec3b>(row, col)[2] * 0.168 +
                          input->at<cv::Vec3b>(row, col)[1] * 0.686 +
                          input->at<cv::Vec3b>(row, col)[0] * 0.349;
            float red = input->at<cv::Vec3b>(row, col)[2] * 0.189 +
                        input->at<cv::Vec3b>(row, col)[1] * 0.769 +
                        input->at<cv::Vec3b>(row, col)[0] * 0.393;
            input->at<cv::Vec3b>(row, col)[0] = pixel_value(blue);
            input->at<cv::Vec3b>(row, col)[1] = pixel_value(green);
            input->at<cv::Vec3b>(row, col)[2] = pixel_value(red);
        }
    }
    return;
}

void apply_convolution3(cv::Mat *input, std::vector<std::vector<int>> kernel, int devide)
{
    cv::Mat temp = input->clone();
    cv::resize(*input, *input, cv::Size(input->cols - 2, input->rows - 2));
    for (int row = 1; row < temp.rows - 1; ++row)
    {
        for (int col = 1; col < temp.cols - 1; ++col)
        {
            float sum = temp.at<uchar>(row - 1, col - 1) * kernel[0][0] +
                        temp.at<uchar>(row - 1, col) * kernel[0][1] +
                        temp.at<uchar>(row - 1, col + 1) * kernel[0][2] +
                        temp.at<uchar>(row, col - 1) * kernel[1][0] +
                        temp.at<uchar>(row, col) * kernel[1][1] +
                        temp.at<uchar>(row, col + 1) * kernel[1][2] +
                        temp.at<uchar>(row + 1, col - 1) * kernel[2][0] +
                        temp.at<uchar>(row + 1, col) * kernel[2][1] +
                        temp.at<uchar>(row + 1, col + 1) * kernel[2][2];
            input->at<uchar>(row - 1, col - 1) = sum / devide;
        }
    }
    temp.release();
    return;
}

void apply_average(cv::Mat *input)
{
    std::vector<std::vector<int>> kernel = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    int devide = 9;
    apply_convolution3(input, kernel, devide);
    return;
}

void apply_binary(cv::Mat *input)
{
    for (int row = 0; row < input->rows; ++row)
    {
        for (int col = 0; col < input->cols; ++col)
        {
            input->at<uchar>(row, col) = binary_value(input->at<uchar>(row, col));
        }
    }
    return;
}

void apply_laplace(cv::Mat *input)
{
    std::vector<std::vector<int>> kernel = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    int devide = 2;
    apply_convolution3(input, kernel, devide);
    return;
}

int main(int argc, char *argv[])
{
    auto start = std::chrono::high_resolution_clock::now();
    if (argc != 2)
    {
        std::cout << "Bad arguments!" << std::endl;
        exit(EXIT_FAILURE);
    }
    const std::string FILE_PATH(argv[1]);

    cv::Mat img_color;
    img_color = cv::imread(FILE_PATH, cv::ImreadModes::IMREAD_ANYCOLOR);

    if (img_color.empty())
    {
        std::cout << "input image empty" << std::endl;
        exit(EXIT_FAILURE);
    }

    apply_horizantal_flip(&img_color);
    apply_vertical_flip(&img_color);
    apply_sepia(&img_color);
    imwrite("First_Serial.bmp", img_color);
    img_color.release();
    /////
    cv::Mat img_gray;
    img_gray = cv::imread(FILE_PATH, cv::ImreadModes::IMREAD_GRAYSCALE);

    if (img_gray.empty())
    {
        std::cout << "input image empty" << std::endl;
        exit(EXIT_FAILURE);
    }

    apply_average(&img_gray);
    apply_binary(&img_gray);
    apply_laplace(&img_gray);
    imwrite("Second_Serial.bmp", img_gray);
    img_gray.release();

    auto end = std::chrono::high_resolution_clock::now();
    auto len = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    float result = len.count();
    std::cout << "Execution Time: " << result / 1000000 << std::endl;
}