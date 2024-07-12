# MultiThreaded Image processing

This repository contains a project focused on applying various image filters to bitmap images using both serial and parallel processing techniques. The goal is to explore and compare the performance differences between these two approaches.

This was done for the Operating Systems course in 2024 at the University of Tehran under the instruction of Professor Kargahi.

## Features

- **Horizontal Mirror Filter**: Flips the image horizontally.
- **Vertical Mirror Filter**: Flips the image vertically.
- **Sepia Filter**: Applies a vintage sepia tone to the image.
- **Edge Detection**: Detects edges in a grayscale image using a box blur filter followed by Laplacian edge detection.

## Libraries Used

- **OpenCV**: Used for reading and saving images.
- **PThread Library**: Used for parallel implementation.

## Project Structure

The repository is divided into two main directories:

- `serial`: Contains the serial version of the program.
- `parallel`: Contains the parallel version of the program.

Both directories include:
- Source code files
- Makefile for compiling the code

## Compilation and Execution

First, ensure you have a C++11 standard compiler installed.
This project was tested on a Linux system, but it should also run on other operating systems with minimal adjustments.

To run this project on a Linux system after cloning the repository:

### Serial Implementation

Navigate to the `serial` directory and compile the code using the provided Makefile:

```
cd serial
make
```
Place your desired image (input_image.bmp) in the respective directory before proceeding with execution.

Run the executable with the input image:

```
./ImageFilters.out <input_image.bmp>
```



### Parallel Implementation

Navigate to the `parallel` directory and compile the code using the provided Makefile:

```
cd parallel
make
```

Place your desired image (input_image.bmp) in the respective directory before proceeding with execution.

Run the executable with the input image:

```
./ImageFilters.out <input_image.bmp>
```

### Measuring Performance

The execution time for both serial and parallel implementations will be printed to the standard output:

```
Execution Time: <execution_time> seconds
```

## Input and Output

Input: The program expects a 24-bit bitmap image.

![Sample Input Image](https://github.com/Ali-Banihashemi/MultiThreaded_ImageProcessing/blob/main/Samples/input.png)

Output: The processed images will be saved as:

Serial_First.bmp and Serial_Second.bmp for the serial version

Parallel_First.bmp and Parallel_Second.bmp for the parallel version

### First Image (Normal Filters):

In each directory (serial and parallel), the first image (Serial_First.bmp or Parallel_First.bmp) showcases the results after applying all standard filters. These include horizontal and vertical mirroring, along with a sepia tone effect.

![Sample First Output Image](https://github.com/Ali-Banihashemi/MultiThreaded_ImageProcessing/blob/main/Samples/output1.png)

### Second Image (Edge Detection):

The second image (Serial_Second.bmp or Parallel_Second.bmp) in each directory illustrates the outcome of a comprehensive edge detection process. This includes applying a Box blur, a binary kernel, and a Laplacian kernel to highlight edges within the image.

![Sample Second Output Image](https://github.com/Ali-Banihashemi/MultiThreaded_ImageProcessing/blob/main/Samples/output2.png)

## Contact
Feel free to reach out to me via [LinkedIn](https://www.linkedin.com/in/alibanihashemi02/) or [Email](mailto:alibanihashemi@outlook.com) for any questions or collaborations.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
