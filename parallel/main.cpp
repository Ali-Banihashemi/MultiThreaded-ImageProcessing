#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <pthread.h>
#include <unistd.h>

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>

#define THR 8

struct thread_in
{
    int start;
    int end;
    cv::Mat *result;
    cv::Mat *refrence;
};

void *apply_horizantal_flip(void *in)
{
    struct thread_in *thread_input = (struct thread_in *)in;
    for (int row = thread_input->start; row < thread_input->end; row++)
    {
        for (int col = 0; col < thread_input->refrence->cols; col++)
        {
            thread_input->result->at<cv::Vec3b>(row, thread_input->refrence->cols - col - 1) = thread_input->refrence->at<cv::Vec3b>(row, col);
        }
    }
    return NULL;
}

void *apply_vertical_flip(void *in)
{
    struct thread_in *thread_input = (struct thread_in *)in;
    for (int row = thread_input->start; row < thread_input->end; row++)
    {
        for (int col = 0; col < thread_input->refrence->cols; col++)
        {
            thread_input->result->at<cv::Vec3b>(thread_input->refrence->rows - row - 1, col) = thread_input->refrence->at<cv::Vec3b>(row, col);
        }
    }
    return NULL;
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

void *apply_sepia(void *in)
{
    struct thread_in *thread_input = (struct thread_in *)in;
    for (int row = thread_input->start; row < thread_input->end; row++)
    {
        for (int col = 0; col < thread_input->result->cols; col++)
        {
            float blue = thread_input->result->at<cv::Vec3b>(row, col)[2] * 0.131 +
                         thread_input->result->at<cv::Vec3b>(row, col)[1] * 0.534 +
                         thread_input->result->at<cv::Vec3b>(row, col)[0] * 0.272;
            float green = thread_input->result->at<cv::Vec3b>(row, col)[2] * 0.168 +
                          thread_input->result->at<cv::Vec3b>(row, col)[1] * 0.686 +
                          thread_input->result->at<cv::Vec3b>(row, col)[0] * 0.349;
            float red = thread_input->result->at<cv::Vec3b>(row, col)[2] * 0.189 +
                        thread_input->result->at<cv::Vec3b>(row, col)[1] * 0.769 +
                        thread_input->result->at<cv::Vec3b>(row, col)[0] * 0.393;
            thread_input->result->at<cv::Vec3b>(row, col)[0] = pixel_value(blue);
            thread_input->result->at<cv::Vec3b>(row, col)[1] = pixel_value(green);
            thread_input->result->at<cv::Vec3b>(row, col)[2] = pixel_value(red);
        }
    }
    return NULL;
}

void *apply_average(void *in)
{
    std::vector<std::vector<int>> kernel = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    int devide = 9;

    struct thread_in *thread_input = (struct thread_in *)in;

    for (int row = thread_input->start; row < thread_input->end; row++)
    {

        if (row == 0 || row >= thread_input->refrence->rows - 1)
        {

            continue;
        }
        for (int col = 1; col < thread_input->refrence->cols - 1; col++)
        {
            float sum = thread_input->refrence->at<uchar>(row - 1, col - 1) * kernel[0][0] +
                        thread_input->refrence->at<uchar>(row - 1, col) * kernel[0][1] +
                        thread_input->refrence->at<uchar>(row - 1, col + 1) * kernel[0][2] +
                        thread_input->refrence->at<uchar>(row, col - 1) * kernel[1][0] +
                        thread_input->refrence->at<uchar>(row, col) * kernel[1][1] +
                        thread_input->refrence->at<uchar>(row, col + 1) * kernel[1][2] +
                        thread_input->refrence->at<uchar>(row + 1, col - 1) * kernel[2][0] +
                        thread_input->refrence->at<uchar>(row + 1, col) * kernel[2][1] +
                        thread_input->refrence->at<uchar>(row + 1, col + 1) * kernel[2][2];
            thread_input->result->at<uchar>(row - 1, col - 1) = sum / devide;
        }
    }
    return NULL;
}

void *apply_binary(void *in)
{
    struct thread_in *thread_input = (struct thread_in *)in;
    for (int row = thread_input->start; row < thread_input->end; row++)
    {
        for (int col = 0; col < thread_input->result->cols; ++col)
        {
            thread_input->result->at<uchar>(row, col) = binary_value(thread_input->result->at<uchar>(row, col));
        }
    }
    return NULL;
}

void *apply_laplace(void *in)
{
    std::vector<std::vector<int>> kernel = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    int devide = 2;

    struct thread_in *thread_input = (struct thread_in *)in;

    for (int row = thread_input->start + 1; row < thread_input->end - 1; row++)
    {
        for (int col = 1; col < thread_input->refrence->cols - 1; col++)
        {
            float sum = thread_input->refrence->at<uchar>(row - 1, col - 1) * kernel[0][0] +
                        thread_input->refrence->at<uchar>(row - 1, col) * kernel[0][1] +
                        thread_input->refrence->at<uchar>(row - 1, col + 1) * kernel[0][2] +
                        thread_input->refrence->at<uchar>(row, col - 1) * kernel[1][0] +
                        thread_input->refrence->at<uchar>(row, col) * kernel[1][1] +
                        thread_input->refrence->at<uchar>(row, col + 1) * kernel[1][2] +
                        thread_input->refrence->at<uchar>(row + 1, col - 1) * kernel[2][0] +
                        thread_input->refrence->at<uchar>(row + 1, col) * kernel[2][1] +
                        thread_input->refrence->at<uchar>(row + 1, col + 1) * kernel[2][2];
            thread_input->result->at<uchar>(row - 1, col - 1) = sum / devide;
        }
    }
    return NULL;
}

void do_threading(cv::Mat *img, void *(*function)(void *), bool resize)
{

    pthread_t threads[THR];
    pthread_attr_t temp_attr;

    int thread_size = img->rows / THR;

    pthread_attr_init(&temp_attr);
    pthread_attr_setdetachstate(&temp_attr, PTHREAD_CREATE_JOINABLE);

    cv::Mat temp = img->clone();
    struct thread_in inputs[THR];
    int sum = 0;

    if (resize)
    {
        cv::resize(*img, *img, cv::Size(img->cols - 2, img->rows - 2));
    }

    for (int i = 0; i < THR - 1; i++)
    {
        inputs[i].start = sum;
        inputs[i].end = sum + thread_size;
        inputs[i].refrence = &temp;
        inputs[i].result = img;
        pthread_create(&threads[i], &temp_attr, function, (void *)&inputs[i]);

        sum += thread_size;
    }

    inputs[THR - 1].start = sum;
    inputs[THR - 1].end = img->rows;
    inputs[THR - 1].refrence = &temp;
    inputs[THR - 1].result = img;
    pthread_create(&threads[THR - 1], &temp_attr, function, (void *)&inputs[THR - 1]);

    pthread_attr_destroy(&temp_attr);
    for (int i = 0; i < THR; i++)
    {
        pthread_join(threads[i], NULL);
    }
    temp.release();
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

    do_threading(&img_color, apply_horizantal_flip, false);
    do_threading(&img_color, apply_vertical_flip, false);
    do_threading(&img_color, apply_sepia, false);
    imwrite("First_Parallel.bmp", img_color);
    img_color.release();
    /////
    cv::Mat img_gray;
    img_gray = cv::imread(FILE_PATH, cv::ImreadModes::IMREAD_GRAYSCALE);

    if (img_gray.empty())
    {
        std::cout << "input image empty" << std::endl;
        exit(EXIT_FAILURE);
    }

    do_threading(&img_gray, apply_average, true);
    do_threading(&img_gray, apply_binary, false);
    do_threading(&img_gray, apply_laplace, true);
    imwrite("Second_Parallel.bmp", img_gray);
    img_gray.release();

    auto end = std::chrono::high_resolution_clock::now();
    auto len = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    float result = len.count();
    std::cout << "Execution Time: " << result / 1000000 << std::endl;
}