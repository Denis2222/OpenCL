#include <stdio.h>
#include <Ocl.hpp>
#include <time.h>
/*
# define CL_CHECK(_expr) if (_expr != CL_SUCCESS) perror("Cl_error");
# define CL_CHECK_ERR1(_expr) ({cl_int e;typeof(_expr) _ret = _expr;
# define CL_CHECK_ERR2() if (e != CL_SUCCESS)perror("Cl_error");_ret;})
# define CL_CHECK_ERR(_expr) CL_CHECK_ERR1(_expr) CL_CHECK_ERR2()

void	pfn_notify(const char *errinfo, const void *private_info, size_t cb,
                void *user_data)
{
    (void)private_info;
    (void)user_data;
    (void)cb;
    printf("%s\n", errinfo);
}

typedef struct			s_data_cl {
	cl_mem				buf_struck;
	cl_mem				buf_end;
	// size_t				buf_img_size;s
	size_t				work;
	unsigned int		obj_c;
    cl_platform_id		ptf_id;
    cl_device_id		dev_id;
    cl_context			context;
    cl_command_queue	cmd_queue;
    cl_program			program;
    cl_kernel			kernel;
}						t_data_cl;

void		init_open_cl(t_data_cl *cl) {

	int	result;
	result = clGetPlatformIDs(1, &cl->ptf_id, NULL);
	if(result != CL_SUCCESS)
	{
		printf("Error while getting available platforms");
		exit(1);
	}

	CL_CHECK(clGetDeviceIDs(cl->ptf_id, CL_DEVICE_TYPE_GPU, 1, &cl->dev_id, NULL));
	CL_CHECK_ERR(cl->context = clCreateContext(NULL, 1, &cl->dev_id, pfn_notify , NULL, &e));
	CL_CHECK_ERR(cl->cmd_queue = clCreateCommandQueue(cl->context, cl->dev_id, 0, &e));
	//load_file(cl, "srcs/fractol.cl", "mandelbrot");
	//CL_CHECK_ERR(cl->buf_struck = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, sizeof(t_struck), NULL, &e));
	//CL_CHECK_ERR(cl->buf_end = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, 4 * sizeof(char) * X * Y, NULL, &e));

	printf("Platform ID:%d \n", cl->ptf_id);
	//printf("Device ID:%d \n", cl->dev_id);
}
*/
int main(void) {
	//t_data_cl cl;
	Ocl *cl = new Ocl();
	//init_open_cl(&cl);

	(void)cl;

	return (0);
}
