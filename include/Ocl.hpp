#ifndef OCL_HPP
#define OCL_HPP

#ifdef __APPLE__
# include <OpenCL/opencl.h>
#else
# include <CL/cl.h>
#endif

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

class Ocl {

	cl_platform_id		platform;
	cl_device_id		device;

	cl_context			context;
	cl_command_queue	commands;

	std::string			program_string;
	cl_program			program;

	cl_kernel			kernel;
	cl_mem				inputBuffer;
	cl_mem				outputBuffer;

	public:
		Ocl(void);
		~Ocl(void);

		void GetPlatform(void);
		void GetDevice(void);

		void GetContext(void);
		void GetCommandQueue(void);

		void LoadProgram(std::string);
		void CreateProgram(void);
		void BuildProgram(void);

		void CreateKernel(const char *function);

		void InitKernel(void);

		void ExecKernel(void);
		void ResultKernel(void);
};

#endif
