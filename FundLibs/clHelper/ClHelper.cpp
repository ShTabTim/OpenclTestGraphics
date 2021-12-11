#include "ClHelper.h"
#include "../FileHelper/FileHelper.h"

//clContext
clContext::~clContext() { finit(); }

void clContext::init() {
	cl_platform_id platform_id = 0;

	cl_uint ret_num_platforms = 0;
	cl_uint ret_num_devices = 0;

	error = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);// gets available platforms
	error = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);// gets available devices 

	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &error);// creates context  
	command_queue = clCreateCommandQueue(context, device_id, 0, &error);// creates queue command 
}

void clContext::finit() {
	if(!command_queue)clFinish(command_queue);
	if(!command_queue)clReleaseCommandQueue(command_queue);
	if(!context)clReleaseContext(context);
}
//clContext

//clProgram
cl_program clProgram::getId() { return id; }

clProgram::clProgram() {}
clProgram::clProgram(clContext* clC) { init(clC); }
clProgram::~clProgram() { finit(); }

void clProgram::init(clContext* clC) {
	this->clC = clC;
}
void clProgram::finit() {
	if (!id)clReleaseProgram(id);
}
void clProgram::create(const char* path) {
	std::string sttr = rdFile(path);
	const char* source_str = sttr.c_str();
	size_t source_size = sttr.length();
#if FilePrint == 1
	std::cout << source_size << "\n" << source_str << "\n";
#endif
	id = clCreateProgramWithSource(clC->context, 1, (const char**)&source_str, (const size_t*)&source_size, &(clC->error));// creates program binary code 
}
void clProgram::build() {
	clC->error = clBuildProgram(id, 1, &(clC->device_id), NULL, NULL, NULL);// compilates program 
}
//clProgram

//clKernel
clKernel::clKernel() {}
clKernel::clKernel(clContext* clC, clProgram* clP) { init(clC, clP); }
clKernel::~clKernel() { finit(); }

void clKernel::init(clContext* clC, clProgram* clP) { this->clC = clC; this->clP = clP; }
void clKernel::finit() { if(!id)clReleaseKernel(id); }

void clKernel::create(const char* fncName) {
	id = clCreateKernel(clP->getId(), fncName, &(clC->error));// creates kernel
}
void clKernel::setArg(cl_mem* id, size_t numInd) {
	clC->error = clSetKernelArg(this->id, numInd, sizeof(cl_mem), (void*)id);// set the parameter 
}
void clKernel::doFnc(const size_t* global_work_size) {
	clC->error = clEnqueueNDRangeKernel(clC->command_queue, id, 1, NULL, global_work_size, NULL, 0, NULL, NULL);// does the kernel 
}
//clKernel