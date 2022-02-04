//
// Created by Link on 2022/2/2.
//

#include <iostream>
#include "OpenCL.h"

OpenCL::OpenCL() {
    auto err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    if (err != CL_SUCCESS) {
        std::cerr << "ERROR::OPEN_CL::OPEN_CL::DEVICE_GROUP_CREATION_FAILED" << err << std::endl;
        return;
    }

    // Create a compute context
    // 建立一个GPU计算的上下文环境，一组上下文环境保存一组相关的状态、内存等资源
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context) {
        std::cerr << "ERROR::OPEN_CL::OPEN_CL::COMPUTE_CONTEXT_CREATION_FAILED" << err << std::endl;
        return;
    }

    // Create a command commands
    //使用获取到的GPU设备和上下文环境监理一个命令队列，其实就是给GPU的任务队列
    commands = clCreateCommandQueue(context, device_id, 0, &err);
    if (!commands) {
        std::cerr << "ERROR::OPEN_CL::OPEN_CL::COMMAND_CREATION_FAILED" << err << std::endl;
        return;
    }
}

void OpenCL::loadProgram(const std::string &filename, Program& p) {
    int err;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(filename);
    } catch (std::ifstream::failure e) {
        std::cerr << "ERROR::OPEN_CL::LOAD_PROGRAM::FILE_NOT_SUCCESSFULLY_READ" << err << std::endl;
        exit(1);
    }
    std::ostringstream stream;
    stream << file.rdbuf();
    file.close();
    std::string kernelSourceStr(stream.str());
    const char *kernelSource = kernelSourceStr.c_str();
    // Create the compute program from the source buffer
    //将内核程序的字符串加载到上下文环境
    auto program = clCreateProgramWithSource(context, 1, (const char **) & kernelSource, NULL, &err);
    if (!program) {
        std::cerr << "ERROR::OPEN_CL::LOAD_PROGRAM::COMPUTE_PROGRAM_CREATION_FAILED" << err << std::endl;
        exit(1);
    }

    // Build the program executable
    //根据所使用的设备，将程序编译成目标机器语言代码，跟通常的编译类似，
    //内核程序的语法类错误信息都会在这里出现，所以一般尽可能打印完整从而帮助判断。
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        size_t len;
        char buffer[2048];

        std::cerr << "ERROR::OPEN_CL::LOAD_PROGRAM::PROGRAM_EXECUTABLE_BUILDING_FAILED" << err << std::endl;
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        std::cerr << buffer << std::endl;
        exit(1);
    }
//    programs.push_back(program);
    p.program = program;
    p.openCL = this;
}

void Program::loadKernel(const std::string &kernelName, size_t c) {
    int err;
    // Create the compute kernel in the program we wish to run
    //使用内核程序的函数名建立一个计算内核
    kernel = clCreateKernel(program, kernelName.c_str(), &err);
    if (!kernel || err != CL_SUCCESS) {
        std::cerr << "ERROR::OPEN_CL_PROGRAM::LOAD_KERNEL::COMPUTE_KERNEL_CREATION_FAILED" << err << std::endl;
        exit(1);
    }
    count = c;
    err = clGetKernelWorkGroupInfo(kernel, openCL->device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    if (err != CL_SUCCESS) {
        std::cerr << "ERROR::OPEN_CL_PROGRAM::LOAD_KERNEL::RETRIEVE_KERNEL_WORK_GROUP_INFO_FAILED" << err << std::endl;
        exit(1);
    }
    rCount = (count / local + 1) * local;
}


void Program::loadKernel(const std::string &kernelName) {
    int err;
    // Create the compute kernel in the program we wish to run
    //使用内核程序的函数名建立一个计算内核
    kernel = clCreateKernel(program, kernelName.c_str(), &err);
    if (!kernel || err != CL_SUCCESS) {
        std::cerr << "ERROR::OPEN_CL_PROGRAM::LOAD_KERNEL::COMPUTE_KERNEL_CREATION_FAILED" << err << std::endl;
        exit(1);
    }
    err = clGetKernelWorkGroupInfo(kernel, openCL->device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    if (err != CL_SUCCESS) {
        std::cerr << "ERROR::OPEN_CL_PROGRAM::LOAD_KERNEL::RETRIEVE_KERNEL_WORK_GROUP_INFO_FAILED" << err << std::endl;
        exit(1);
    }
}

void Program::setCount(size_t c) {
    count = c;
    rCount = (count / local + 1) * local;
}

void Program::execute(bool wait) {
    int err = clEnqueueNDRangeKernel(openCL->commands, kernel, 1, NULL, &rCount, &local, 0, NULL, NULL);
    if (err) {
        printf("Error: Failed to execute kernel!\n");
        std::cerr << "ERROR::OPEN_CL_PROGRAM::EXECUTE::EXECUTE_KERNEL_FAILED" << err << std::endl;
        exit(1);
    }
    if (wait) clFinish(openCL->commands);
}

Program::~Program() {
    for (auto& buffer : buffers) clReleaseMemObject(buffer);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
}
