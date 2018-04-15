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
