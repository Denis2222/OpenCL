# include <Ocl.hpp>

void CL_CALLBACK onOpenCLError(const char *errinfo,  const void *private_info, size_t cb, void *user_data)
{
	(void) private_info;
	(void) cb;
	(void) user_data;
    printf("Error while creating context or working in this context : %s", errinfo);
}

Ocl::Ocl(void) {
	this->GetPlatform();
	this->GetDevice();
	this->GetContext();
	this->GetCommandQueue();
	this->program = 0;
	this->LoadProgram(std::string("./kernel/simple.cl"));
	this->CreateProgram();
	this->BuildProgram();
	this->CreateKernel("square");

	this->InitKernel();
	this->ExecKernel();
	this->ResultKernel();
}

Ocl::~Ocl(void) {

}

void Ocl::GetPlatform(void) {
	int	result;
	result = clGetPlatformIDs(1, &this->platform, NULL);
	if(result != CL_SUCCESS)
	{
		std::cout << "Error while getting available platforms\n" << std::endl;
		exit(1);
	}
	std::cout << "Success get Platform\n" << std::endl;
}

void Ocl::GetDevice(void) {
	int	result;
	result = clGetDeviceIDs(this->platform, CL_DEVICE_TYPE_GPU, 1, &this->device, NULL);
	if(result != CL_SUCCESS)
	{
		std::cout << "Error while getting available devices\n" << std::endl;
		exit(1);
	}
	std::cout << "Success get Device\n" << std::endl;
}

void Ocl::GetContext(void) {
	int result;
	this->context = clCreateContext(0, 1, &this->device, &onOpenCLError, NULL, &result);
	if(result != CL_SUCCESS)
	{
		std::cout << "Error while creating the OpenCL context" << std::endl;
		exit(1);
	}
	std::cout << "Success get Context\n" << std::endl;
}

void Ocl::GetCommandQueue(void) {
	int result;
	this->commands = clCreateCommandQueue(this->context, this->device, 0, &result);
	if(result != CL_SUCCESS)
	{
		std::cout << "Error while creating the command queue" << std::endl;
		exit(1);
	}
	std::cout << "Success create command queue\n" << std::endl;
}

void Ocl::LoadProgram(std::string path) {
	std::ifstream kernelFile(path);
	std::stringstream kernelstream;
	kernelstream << kernelFile.rdbuf();
	std::string kernelstring;
	this->program_string = kernelstream.str();
	std::cout << "Program load :\n" << this->program_string << std::endl;
}

void Ocl::CreateProgram(void) {
	int result;

	const char *src;
	src = this->program_string.c_str();
	this->program = clCreateProgramWithSource(this->context, 1, (const char**)&src, NULL, &result);
	if(result != CL_SUCCESS)
	{
		std::cout << "Error while creating the program" << std::endl;
		exit(1);
	}
	std::cout << "Create Program OK" << std::endl;
}

void Ocl::BuildProgram(void) {
	int result = clBuildProgram(this->program, 1, &this->device, NULL, NULL, NULL);
	if(result != CL_SUCCESS)
	{
		char *buf = (char*)malloc(sizeof(char) * 2048);
		size_t len;
		if (buf == NULL)
		{
			std::cout << "Malloc faild" << std::endl;
			exit(1);
		}
		clGetProgramBuildInfo(this->program, this->device, CL_PROGRAM_BUILD_LOG, sizeof(buf), buf, &len);
		std::cout << "Build Program cl failed" << std::endl;;
		write(STDOUT_FILENO, buf, len);
		exit(0);
	}
	std::cout << "Build Program OK" << std::endl;
}

void Ocl::CreateKernel(const char *function) {
	int	result;

	this->kernel = clCreateKernel(this->program, function, &result);
	if(result != CL_SUCCESS)
	{
		std::cout << "Error while creating the kernel" << std::endl;
		exit(1);
	}
	std::cout << "Kernel Create OK" << std::endl;
}

#define NB 50

void Ocl::InitKernel(void) {
	size_t numberOfValues = NB;
	size_t sizeOfBuffers = NB*sizeof(int);
	int* inputIntegers = (int*)malloc(sizeOfBuffers);  //Our input data array
	for(size_t i=0 ; i < NB ; i++)            //We put some numbers in it
	{
		inputIntegers[i] = i;       					//I know, I'm very lazy.....
	}

	int result;
	this->inputBuffer = clCreateBuffer(this->context, CL_MEM_READ_ONLY, sizeOfBuffers, NULL, &result);
	if(result != CL_SUCCESS)
	{
		std::cout << "Error while initializing input data" << std::endl;
		exit(1);
	}


	cl_bool     blockingWrite = CL_TRUE;    //We want to wait until the copy is finished
	size_t      offset = 0;                 //We start at 0 in our host buffer for the copy
	cl_event    dataInputCopyEvent;         //An identifier for this copy operation
	cl_event*   eventsToWait = NULL;        //List of events we want to wait the end before
						//performing this copy operation
	cl_uint     numEvents = 0;              //Number of events

	//Copying the data the device input buffer we've just created
	result = clEnqueueWriteBuffer(this->commands, this->inputBuffer, blockingWrite, offset,
			sizeOfBuffers, inputIntegers, numEvents, eventsToWait, &dataInputCopyEvent);
	if(result != CL_SUCCESS)
	{
		std::cout << "Error while writing input data" << std::endl;
		exit(1);
	}

	this->outputBuffer = clCreateBuffer(this->context, CL_MEM_WRITE_ONLY, sizeOfBuffers, NULL, &result);
	if(result != CL_SUCCESS)
	{
		std::cout << "Error while initializing output data" << std::endl;
		exit(1);
	}

	result = 0;
	result |= clSetKernelArg(this->kernel, 0, sizeof(cl_mem), &this->inputBuffer);
	result |= clSetKernelArg(this->kernel, 1, sizeof(cl_mem), &this->outputBuffer);
	result |= clSetKernelArg(this->kernel, 2, sizeof(size_t), &numberOfValues);
	if(result != CL_SUCCESS)
	{
		std::cout << "Error while setting kernel arguments" << std::endl;
		exit(1);
	}

	std::cout << "InitKernel OK" << std::endl;
}

void Ocl::ExecKernel(void) {
	/****************************************************************************************/
	/*****                   Finally, execution of the kernel !!!!                      *****/
	/****************************************************************************************/
	//Declarations
	cl_uint   workDim = 1;						//We can use dimensions to organize data. Here
												//we only got 1 dimension in our array, so we
												//use 1.
	size_t*   globalWorkOffset = NULL;			//Offsets used for each dimension to give
												//identfication values to work-items
	size_t    globalWorkSize =  NB;				//Number of values for each dimension we use
	size_t    localWorkSize;					//Size of a work-group in each dimension
	cl_event  kernelExecEvent;					//The event for the execution of the kernel
	cl_event*   eventsToWait = NULL;			//List of events we want to wait the end before
												//performing this copy operation
	cl_uint     numEvents = 0;					//Number of events

	//If have to set a correct work group size. The total number of work-items we want to run
	//(in our case, numberOfValues work-items) should be divisable by the work-group size.
	//I will not make to muche test here, just the case where numberOfValues is less than
	//the maximum work-group size.
	int result;
	result = clGetKernelWorkGroupInfo(this->kernel, this->device, CL_KERNEL_WORK_GROUP_SIZE,
					sizeof(localWorkSize), &localWorkSize, NULL);
	if(localWorkSize > NB) localWorkSize = NB;
	if(result != CL_SUCCESS)
	{
		std::cout << "Error while getting maximum work group size" << std::endl;
		exit(1);
	}

	//Execution
	result = clEnqueueNDRangeKernel(this->commands, this->kernel, workDim, globalWorkOffset, &globalWorkSize,
					&localWorkSize, numEvents, eventsToWait, &kernelExecEvent);
	if(result != CL_SUCCESS)
	{
		std::cout << "Error while executing the kernel" << std::endl;
		exit(1);
	}
}

void Ocl::ResultKernel(void)
{
	/****************************************************************************************/
	/*****                          Getting back the results                            *****/
	/****************************************************************************************/
	//Declarations
	cl_bool	blockingRead = CL_TRUE;
	size_t		offset = 0;
	int			*resultArray;
	cl_event	readResultsEvent;		//The event for the execution of the kernel
	cl_event*   eventsToWait = NULL;		//List of events we want to wait the end before
										//performing this copy operation
	size_t numEvents = 0;              //Number of events
	size_t sizeOfBuffers = NB*sizeof(int);
	//Allocations
	resultArray = (int*)malloc(NB*sizeof(int));

	//Waiting for all commands to end. Note we coul have use the kernelExecEvent as an event
	//to wait the end. But the clFinish function is simplier in this case.
	clFinish(this->commands);

	//Execution
	clEnqueueReadBuffer(this->commands, this->outputBuffer, blockingRead, offset, sizeOfBuffers,
						resultArray, numEvents, eventsToWait, &readResultsEvent);

	/****************************************************************************************/
	/*****                              Output of result                                *****/
	/****************************************************************************************/
	for(int i=0 ; i < NB ; i++)
	{
		std::cout << resultArray[i] << "\t";
		if((i+1) % 10 == 0 && i != 0)
		{
			std::cout << std::endl;
		}
	}
}
