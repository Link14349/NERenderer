//
// Created by Link on 2022/2/2.
//

#ifndef OPENCLLEARNING_OPENCL_H
#define OPENCLLEARNING_OPENCL_H

#include <iostream>
#ifdef __APPLE__ //Mac OSX has a different name for the header file
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdarg>

class Program;
class OpenCL {
    friend class Program;
public:
    OpenCL();
    void loadProgram(const std::string& filename, Program& p);
    ~OpenCL() {
        clReleaseCommandQueue(commands);
        clReleaseContext(context);
    }
private:
    cl_device_id device_id;             // compute device id
    cl_context context;                 // compute context
    cl_command_queue commands;          // compute command queue
//    std::vector<cl_program> programs;
};

class Program {
    friend class OpenCL;
public:
    Program() = default;
    void loadKernel(const std::string& kernelName, size_t c);// 载入核心并设置需求任务数
    void loadKernel(const std::string& kernelName);
    void setCount(size_t c);
    template <class T>
    cl_mem createBuffer(cl_mem_flags flag);
    template <class T>
    void writeBuffer(cl_mem buffer, T* data);
    template <class T>
    void readBuffer(cl_mem buffer, T* data);
    size_t realCount() const { return rCount; }
    template <class T>
    void setArg(T* data);
    void execute(bool wait = true);
    size_t getRCount() const { return rCount; }
    ~Program();
//    void runKernel(const std::string& kernelName);
private:
    int argLen = 0;
    cl_kernel kernel;
    cl_program program;
    size_t count;// 需求任务数量
    size_t rCount;// 实际任务数量
    size_t local;
    std::vector<cl_mem> buffers;
    OpenCL* openCL;
};

template<class T>
cl_mem Program::createBuffer(cl_mem_flags flag) {
    int err;
    auto buffer = clCreateBuffer(openCL->context, flag, sizeof(T) * rCount, NULL, &err);
    if (buffer) {
        buffers.push_back(buffer);
        return buffer;
    }
    // else
    std::cerr << "ERROR::OPEN_CL_PROGRAM::CREATE_BUFFER::ALLOCATE_DEVICE_MEMORY_FAILED" << err << std::endl;
    exit(1);
}

template<class T>
void Program::writeBuffer(cl_mem buffer, T *data) {
    int err = clEnqueueWriteBuffer(openCL->commands, buffer, CL_TRUE, 0, sizeof(float) * rCount, data, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        std::cerr << "ERROR::OPEN_CL_PROGRAM::WRITE_BUFFER::WRITE_TO_SOURCE_ARRAY_FAILED" << err << std::endl;
        exit(1);
    }
}

template<class T>
void Program::readBuffer(cl_mem buffer, T *data) {
    int err = clEnqueueReadBuffer(openCL->commands, buffer, CL_TRUE, 0, sizeof(T) * count, data, 0, NULL, NULL );
    if (err != CL_SUCCESS) {
        std::cerr << "ERROR::OPEN_CL_PROGRAM::READ_BUFFER::READ_BUFFER_FAILED" << err << std::endl;
        exit(1);
    }
}

template<class T>
void Program::setArg(T* data) {
    int err = clSetKernelArg(kernel, argLen++, sizeof(T), data);
    if (err != CL_SUCCESS) {
        std::cerr << "ERROR::OPEN_CL_PROGRAM::SET_ARG::SET_KERNEL_ARGUMENTS_FAILED" << err << std::endl;
        exit(1);
    }
}

#endif //OPENCLLEARNING_OPENCL_H
