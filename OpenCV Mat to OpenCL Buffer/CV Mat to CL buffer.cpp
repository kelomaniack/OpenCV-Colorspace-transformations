#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

//My code using opencl buffers    
cv::Mat image = cv::imread("scene.jpg", CV_LOAD_IMAGE_COLOR);
cv::Mat imageRGBA;

int width = image.rows;
int height = image.cols;

cl_context CreateContext()
{
	cl_int errNum;
	cl_uint numPlatforms;
	cl_platform_id firstPlatformId;
	cl_context context = NULL;
	errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
	if (errNum != CL_SUCCESS || numPlatforms <= 0)
	{
		std::cerr << "Failed to find any OpenCL platforms." << std::endl;
		return NULL;
	}

	cl_context_properties contextProperties[] =
	{
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)firstPlatformId,
		0
	};
	context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_GPU,
		NULL, NULL, &errNum);
	if (errNum != CL_SUCCESS)
	{
		std::cout << "Could not create GPU context, trying CPU..." << std::endl;
		context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_CPU, NULL, NULL, &errNum);
		if (errNum != CL_SUCCESS)
		{
			std::cerr << "Failed to create an OpenCL GPU or CPU context." << std::endl;
			return NULL;
		}
	}
	return context;
}


cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device)
{
	cl_int errNum;
	cl_device_id *devices;
	cl_command_queue commandQueue = NULL;
	size_t deviceBufferSize = -1;
	errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Failed call to clGetContextInfo(...,GL_CONTEXT_DEVICES,...)";
		return NULL;
	}

	if (deviceBufferSize <= 0)
	{
		std::cerr << "No devices available.";
		return NULL;
	}
	devices = new cl_device_id[deviceBufferSize / sizeof(cl_device_id)];
	errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, NULL);
	if (errNum != CL_SUCCESS)
	{
		delete[] devices;
		std::cerr << "Failed to get device IDs";
		return NULL;
	}

	commandQueue = clCreateCommandQueue(context, devices[0], CL_QUEUE_PROFILING_ENABLE, &errNum);

	if (commandQueue == NULL)
	{
		delete[] devices;
		std::cerr << "Failed to create commandQueue for device 0";
		return NULL;
	}

	*device = devices[0];
	delete[] devices;
	return commandQueue;
}

cl_program CreateProgram(cl_context context, cl_device_id device, const char* fileName)
{
	cl_int errNum;
	cl_program program;

	std::ifstream kernelFile(fileName, std::ios::in);
	if (!kernelFile.is_open())
	{
		std::cerr << "Failed to open file for reading: " << fileName << std::endl;
		return NULL;
	}

	std::ostringstream oss;
	oss << kernelFile.rdbuf();

	std::string srcStdStr = oss.str();
	const char *srcStr = srcStdStr.c_str();
	program = clCreateProgramWithSource(context, 1,
		(const char**)&srcStr,
		NULL, NULL);
	if (program == NULL)
	{
		std::cerr << "Failed to create CL program from source." << std::endl;
		return NULL;
	}
	errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

	if (errNum != CL_SUCCESS)
	{
		char buildLog[16384];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
			sizeof(buildLog), buildLog, NULL);
		std::cerr << "Error in kernel: " << std::endl;
		std::cerr << buildLog;
		clReleaseProgram(program);
		system("pause");
		return NULL;
	}
	return program;
}

bool CreateMemObjects(cl_context context, cl_mem memObjects[2], float *src_ptr)
{

	cl_int errNum = 0;

	memObjects[0] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)*(width*height * 3), src_ptr, NULL);
	memObjects[1] = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float)*(width*height), NULL, NULL);


	if (memObjects[0] == NULL || memObjects[1] == NULL)
	{
		std::cerr << "Error creating memory objects" << std::endl;
		return false;          
	}
	return true;
}

void Cleanup(cl_context context, cl_command_queue commandQueue, cl_program program, cl_kernel kernel, cl_mem memObjects[2])
{
	for (int i = 0; i < 2; i++)
	{
		if (memObjects[i] != 0)
			clReleaseMemObject(memObjects[i]);
	}
	if (commandQueue != 0)
		clReleaseCommandQueue(commandQueue);

	if (kernel != 0)
		clReleaseKernel(kernel);

	if (program != 0)
		clReleaseProgram(program);

	if (context != 0)
		clReleaseContext(context);
}


int main()
{
	cl_context context = 0;
	cl_command_queue commandQueue = 0;
	cl_program program = 0;
	cl_device_id device = 0;
	cl_kernel kernel = 0;
	cl_mem memObjects[2] = { 0, 0 };
	cl_int errNum;
	cl_event myEvent;

	cl_ulong start_time, end_time;
	double kernelExecTimeNs;


	//cvtColor(image, imageRGBA, CV_BGR2RGB, 0);
	float *buffer = reinterpret_cast<float *>(image.data);


	float *tur_image1, *src_ptr;
	tur_image1 = (float*)malloc((width*height) * sizeof(float));
	src_ptr = (float*)malloc((width*height * 3) * sizeof(float));

	context = CreateContext();
	if (context == NULL)
	{
		std::cerr << "Failed to create OpenCL context." << std::endl;
		return 1;
	}

	commandQueue = CreateCommandQueue(context, &device);
	if (commandQueue == NULL)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}

	program = CreateProgram(context, device, "hsl_threshold.cl");
	if (program == NULL)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}

	kernel = clCreateKernel(program, "hsl_threshold", NULL);
	if (kernel == NULL)
	{
		std::cerr << "Failed to create kernel" << std::endl;
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1; 
	}

	printf("height:%d\n", height);//image height
	printf("width:%d\n", width);//image width

	imshow("color image", image);
	cvWaitKey(0);

	memcpy(src_ptr, buffer, sizeof(unsigned char)*(width*height * 3));

	if (!CreateMemObjects(context, memObjects, src_ptr/*, tur_image1*/))
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}

	errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &memObjects[0]);
	errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &memObjects[1]);

	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error setting kernel arguments" << std::endl;
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}

	cout << "Kernel arguments set successfully\n";
	size_t globalWorkSize[1] = { height*width};
	size_t localWorkSize[1] = { 256 };

	errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, &myEvent);

	clWaitForEvents(1, &myEvent);

	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error queuing kernel for execution." << std::endl;
		Cleanup(context, commandQueue, program, kernel, memObjects);
		system("pause");
		return 1;
	}
	clFinish(commandQueue);


	clGetEventProfilingInfo(myEvent, CL_PROFILING_COMMAND_START, sizeof(start_time), &start_time, NULL);
	clGetEventProfilingInfo(myEvent, CL_PROFILING_COMMAND_END, sizeof(end_time), &end_time, NULL);

	kernelExecTimeNs = end_time - start_time;

	printf("\nExecution time in milliseconds = %0.3f ms\n", (kernelExecTimeNs / 1000000.0));
	cout << "\n Kernel timings \n" << kernelExecTimeNs << "seconds";

	errNum = clEnqueueReadBuffer(commandQueue, memObjects[1], CL_TRUE,
		0, (height*width) * sizeof(float), tur_image1,
		0, NULL, NULL);

	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error reading result buffer." << std::endl;
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}

	memcpy(buffer, tur_image1, sizeof(tur_image1));
	cv::Mat img(width, height, CV_8UC3, buffer);
	

	imshow("hsl_thresh", img);
	imwrite("output.bmp", img);
	cvWaitKey(0);

	std::cout << std::endl;
	std::cout << "Image displayed Successfully" << std::endl;

	Cleanup(context, commandQueue, program, kernel, memObjects);
	printf("\n Free opencl resources");
	std::cin.get();
	return 0;

}