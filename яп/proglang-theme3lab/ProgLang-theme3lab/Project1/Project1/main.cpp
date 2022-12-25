#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include "SimpleTimer.h"
#include <CL/cl.hpp>
#include "lodepng.h"
#include <iostream>
#include <vector>
#include <bitset>
#include <array>
#include <string>
#include <intrin.h>

class InstructionSet
{
    // forward declarations
    class InstructionSet_Internal;

public:
    // getters

    static bool AVX(void) { return CPU_Rep.f_1_ECX_[28]; }

private:
    static const InstructionSet_Internal CPU_Rep;

    class InstructionSet_Internal
    {
    public:
        InstructionSet_Internal()
            : f_1_ECX_{ 0 },
            data_{},
            extdata_{}
        {
            //int cpuInfo[4] = {-1};
            std::array<int, 4> cpui;

            // Calling __cpuid with 0x0 as the function_id argument
            // gets the number of the highest valid function ID.
            __cpuid(cpui.data(), 0);
            nIds_ = cpui[0];

            for (int i = 0; i <= nIds_; ++i)
            {
                __cpuidex(cpui.data(), i, 0);
                data_.push_back(cpui);
            }

            // Capture vendor string
            char vendor[0x20];
            memset(vendor, 0, sizeof(vendor));
            *reinterpret_cast<int*>(vendor) = data_[0][1];
            *reinterpret_cast<int*>(vendor + 4) = data_[0][3];
            *reinterpret_cast<int*>(vendor + 8) = data_[0][2];

            // load bitset with flags for function 0x00000001
            if (nIds_ >= 1)
            {
                f_1_ECX_ = data_[1][2];
            }
        };

        int nIds_;
        std::bitset<32> f_1_ECX_;
        std::vector<std::array<int, 4>> data_;
        std::vector<std::array<int, 4>> extdata_;
    };
};

// Initialize static member data
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;

SimpleTimer GlobalTimer;
// Последовательная реализация алгоритма
//img1 и img2 в векторе лежит картнка, ввиже скленных строк. Каждая строка представлена 3 байтми, РГБ
std::vector<unsigned char> SequentialImplementation(const std::vector<unsigned char>& img1, const std::vector<unsigned char>& img2)
{
    unsigned int nRGBPixels = std::min(img1.size(), img2.size());
    std::vector<unsigned char> res(nRGBPixels);
    //покопонентно выполняем 
    for (unsigned int nCnt = 0; nCnt < nRGBPixels; nCnt++)
    {
        int nSubstract = (int)img1[nCnt] - (int)img2[nCnt];
        //если из темной вычитаем светлю получим отрицательную яркость (такого нет)
        res[nCnt] = nSubstract < 0 ? 0 : (char)nSubstract;
    }
    return res;
}
// Векторизация
std::vector<unsigned char> VectorizedImplementation( std::vector<unsigned char>& img1,  std::vector<unsigned char>& img2)
{
    unsigned int nRGBPixels = std::min(img1.size(), img2.size());
    std::vector<unsigned char> res(((nRGBPixels + 31) / 32) * 32);//выравниваем массив на 32 

    if (img1.size() < res.size()) img1.insert(img1.end(), res.size() - img1.size(), 0);
    if (img2.size() < res.size()) img2.insert(img2.end(), res.size() - img2.size(), 0);
    //AVX регистры процессора для выполнения быстрых мат операций, и берут по 32 байта (см 96)
    if (!InstructionSet::AVX())
    {
        std::cout << "avx not supported !!!" << std::endl;
        return res;
    }

    __m256i _img1, _img2, _res, _sub, _zero, _const256 = _mm256_set1_epi8(0xff);
    _zero = _mm256_set1_epi8(0);
    const __m256i;
    
    for (int i = 0; i < res.size(); i += 32) 
    {
        _img1 = _mm256_loadu_epi8(&img1[i]);
        _img2 = _mm256_loadu_epi8(&img2[i]);
        _res = _mm256_subs_epu8(_img1, _img2);    //не только округляет но и обрезает от 0 до 255 (все отрицательные)  

        _mm256_storeu_epi8(&res[i], _res);
    }
    //тк сверху добавили и округлили
    if (res.size() > nRGBPixels) res.erase(res.begin() + nRGBPixels, res.end());

    return res;
}
// OpenMP
std::vector<unsigned char> ОpenMPImplementation(std::vector<unsigned char>& img1, std::vector<unsigned char>& img2)
{
    unsigned int nRGBPixels = std::min(img1.size(), img2.size());
    std::vector<unsigned char> res(nRGBPixels);
    #pragma parallel for shared(img1, img2, res) 
        for (unsigned int nCnt = 0; nCnt < nRGBPixels; nCnt++)
        {
            int nSubstract = (int)img1[nCnt] - (int)img2[nCnt];
            res[nCnt] = nSubstract < 0 ? 0 : (char)nSubstract;
        }

    return res;
}
// OpenCL
std::vector<unsigned char> OpenCLImplementation(const std::vector<unsigned char>& img1, const std::vector<unsigned char>& img2, cl_kernel& kernel, cl_command_queue& cq, cl_mem& res_clmem)
{
    //обработка опенсл
    std::vector<unsigned char> res(img1.size());
    size_t global_size = img1.size();
    size_t local_size = 32; 
    // Начало действий
    cl_int clres = clEnqueueNDRangeKernel(cq, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);
    // Выгружаем (забираем рез из видеокарты)
    clres = clEnqueueReadBuffer(cq, res_clmem, CL_TRUE, 0, global_size * sizeof(unsigned char), &res[0], 0, NULL, NULL);

    //Очистка и завершение команд
    clFlush(cq);
    clFinish(cq);

    return res;
}

std::string readCLSources(const std::string& fileName)
{
    std::stringstream buffer;
    std::ifstream f(fileName);
    if (f.is_open())
    {
        buffer << f.rdbuf();
        f.close();
    }

    return buffer.str();
}

int main()
{
    std::string file1("img1.png"), file2("img2.png");

    unsigned int width1, height1;
    unsigned int width2, height2;

    std::vector<unsigned char> inImg1, inImg2;

    auto error = lodepng::decode(inImg1, width1, height1, file1.c_str(), LCT_RGB);
    if (error)
    {
        std::cout << "inImg1 decode error. Error code:" << error << std::endl;
        return 1;
    }

    error = lodepng::decode(inImg2, width2, height2, file2.c_str(), LCT_RGB);
    if (error)
    {
        std::cout << "inImg2 decode error. Error code:" << error << std::endl;
        return 1;
    }

    if(width1 != width2 || height1 != height2)
    {
        std::cout << "Error dimentions inImg1 != inImg2" << std::endl;
        return 1;
    }

    const uint64_t amountOfRuns = 1001;

    SimpleTimer seqTimer;
    for (uint64_t i = 0; i < amountOfRuns; i++) {
        std::vector<unsigned char> seqImg = SequentialImplementation(inImg1, inImg2);
        if (i == 0)
            lodepng::encode("outoseq.png", seqImg, width1, height1, LCT_RGB);
    }
    seqTimer.StopTime();

    SimpleTimer vecTimer;
    for (uint64_t i = 0; i < amountOfRuns; i++)
    {
        std::vector<unsigned char> vecImg = VectorizedImplementation(inImg1, inImg2);
        if (i == 0)
            lodepng::encode("outovec.png", vecImg, width1, height1, LCT_RGB);
    }
    vecTimer.StopTime();

    SimpleTimer openmpTimer;
    for (uint64_t i = 0; i < amountOfRuns; i++)
    {
        std::vector<unsigned char> openmpImg = ОpenMPImplementation(inImg1, inImg2);
        if (i == 0)
            lodepng::encode("outopenmp.png", openmpImg, width1, height1, LCT_RGB);
    }
    openmpTimer.StopTime();

    // OPENCL
    //шейдер который обрабатывает каждый байт (все на видеокарте)
    const char* saxpy_kernel =
        "__kernel                                       \n"
        "void saxpy_kernel(                             \n"
        "           __global unsigned char *img1,       \n"
        "           __global unsigned char *img2,       \n"
        "           __global unsigned char *res)        \n"
        "{                                              \n"
        "    //Get the index of the work-item           \n"
        "    int index = get_global_id(0);              \n"
        "    int nDiv = (int)img1[index] - (int)img2[index]; \n" 
        "    res[index] = ((nDiv) < 0) ? 0 : nDiv;      \n"
        "}                                              \n";
    //инциализация системы (как только получили контекст)
    // Get platform and device information
    cl_platform_id* platforms = NULL;
    cl_uint     num_platforms;
    // Set up the Platform
    cl_int clStatus = clGetPlatformIDs(0, NULL, &num_platforms); //CL_INVALID_PROGRAM_EXECUTABLE
    platforms = (cl_platform_id*)
        malloc(sizeof(cl_platform_id) * num_platforms);
    clStatus = clGetPlatformIDs(num_platforms, platforms, NULL);

    // Get the devices list and choose the device you want to run on
    cl_device_id* device_list = NULL;
    cl_uint           num_devices;

    clStatus = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
    device_list = (cl_device_id*)malloc(sizeof(cl_device_id) * num_devices);
    clStatus = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, num_devices, device_list, NULL);

    // Create one OpenCL context for each device in the platform 
    //чел который делает все, выгружает загружает в буфер и тд
    cl_context context;
    context = clCreateContext(NULL, num_devices, device_list, NULL, NULL, &clStatus);

    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_list[0], 0, &clStatus);

    // Create memory buffers on the device for each vector
    cl_mem img1_clmem = clCreateBuffer(context, CL_MEM_READ_ONLY, inImg1.size() * sizeof(unsigned char), NULL, &clStatus);
    cl_mem img2_clmem = clCreateBuffer(context, CL_MEM_READ_ONLY, inImg1.size() * sizeof(unsigned char), NULL, &clStatus);
    cl_mem res_clmem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, inImg1.size() * sizeof(unsigned char), NULL, &clStatus);
    //загрузли
    // Copy the Buffer img1 and img2 to the device
    clStatus = clEnqueueWriteBuffer(command_queue, img1_clmem, CL_TRUE, 0, inImg1.size() * sizeof(unsigned char), &inImg1[0], 0, NULL, NULL);
    clStatus = clEnqueueWriteBuffer(command_queue, img2_clmem, CL_TRUE, 0, inImg1.size() * sizeof(unsigned char), &inImg2[0], 0, NULL, NULL);

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, (const char**)&saxpy_kernel, NULL, &clStatus);

    // Build the program
    clStatus = clBuildProgram(program, 1, device_list, NULL, NULL, NULL);

    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "saxpy_kernel", &clStatus);

    // Set the arguments of the kernel
    clStatus = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&img1_clmem);
    clStatus = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&img2_clmem);
    clStatus = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&res_clmem);


    SimpleTimer openclTimer;
    for (uint64_t i = 0; i < amountOfRuns; i++)
    {
        std::vector<unsigned char> openclImg = OpenCLImplementation(inImg1, inImg2, kernel, command_queue, res_clmem);
        if (i == 0)
            lodepng::encode("outopencl.png", openclImg, width1, height1, LCT_RGB);

    }
    openclTimer.StopTime();

    // Finally release all OpenCL allocated objects and host buffers.
    clStatus = clReleaseKernel(kernel);
    clStatus = clReleaseProgram(program);
    clStatus = clReleaseMemObject(img1_clmem);
    clStatus = clReleaseMemObject(img2_clmem);
    clStatus = clReleaseMemObject(res_clmem);
    clStatus = clReleaseCommandQueue(command_queue);
    clStatus = clReleaseContext(context);
    free(platforms);
    free(device_list);


    //Вычисление среднего времени выполнениня.
    std::cout << "\nCount runs:\t " << amountOfRuns << "\n\n";

    std::cout << "SequentialImplementation time:\t " << seqTimer.GetTime() / amountOfRuns << "s" << std::endl;
    std::cout << "VectorizedImplementation time:\t " << vecTimer.GetTime() / amountOfRuns << "s" << std::endl;
    std::cout << "OpenMPImplementation time:\t " << openmpTimer.GetTime() / amountOfRuns << "s" << std::endl;
    std::cout << "OpenCLImplementation time:\t " << openclTimer.GetTime() / amountOfRuns << "s" << std::endl;

    std::cout << "\nAll time:\t " << GlobalTimer.GetTime() << "s" << std::endl;
    return 0;
}
