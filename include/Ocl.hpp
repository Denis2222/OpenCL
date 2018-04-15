#ifndef OCL_HPP
#define OCL_HPP

#include <OpenCL/opencl.h>
#include <stdio.h>
#include <iostream>

class Ocl {

	cl_platform_id		platform;
	cl_device_id		device;

	cl_context			context;
	cl_command_queue	commands;

	public:
		Ocl(void);
		~Ocl(void);

		void GetPlatform(void);
		void GetDevice(void);

		void GetContext(void);
		void GetCommandQueue(void);
};

#endif
