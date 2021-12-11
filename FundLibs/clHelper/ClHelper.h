#pragma once
#include <CL/cl.h>

#define FilePrint 0

class clContext;
template <typename T>
class clMemBuf;
class clProgram;

class clContext {
public:
	int error = 0;
	cl_command_queue command_queue = 0;
	cl_device_id device_id = 0;
	cl_context context = 0;

	~clContext();

	void init();
	void finit();
};

class clProgram {
private:
	cl_program id = 0;
	clContext* clC = 0;
public:
	clProgram();
	clProgram(clContext* clC);
	~clProgram();

	void init(clContext* clC);
	void finit();

	void create(const char* path);
	void build();

	cl_program getId();
};

class clKernel {
private:
	cl_kernel id = 0;
	clProgram* clP = 0;
	clContext* clC = 0;
public:
	clKernel();
	clKernel(clContext* clC, clProgram* clP);
	~clKernel();

	void init(clContext* clC, clProgram* clP);
	void finit();

	void create(const char* fncName);
	void setArg(cl_mem* id, size_t numInd);
	void doFnc(const size_t* global_work_size);
};

template <typename T>
class clMemBuf {
private:
	T mem = 0;
	clContext* clC = 0;
	cl_mem id = 0;
public:
	clMemBuf() {}
	clMemBuf(clContext* clC) { init(clC); }
	~clMemBuf() { finit(); }

	void init(clContext* clC) { this->clC = clC; }
	void finit() { if (!id)clReleaseMemObject(id); }

	void create(size_t bitSizeBuffer) { id = clCreateBuffer(clC->context, CL_MEM_READ_WRITE, bitSizeBuffer, NULL, &(clC->error)); }
	void write(size_t from, size_t size) { clC->error = clEnqueueWriteBuffer(clC->command_queue, id, CL_TRUE, from, size, mem, 0, NULL, NULL); }
	void read(size_t from, size_t size) { clC->error = clEnqueueReadBuffer(clC->command_queue, id, CL_TRUE, from, size, mem, 0, NULL, NULL); }

	cl_mem* getsId() { return &id; }
	cl_mem getId() { return id; }

	T getMem() { return mem; }
	void setMem(T _mem) { mem = _mem; }
};