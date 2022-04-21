#pragma once 

/* FOR PLATFORM SPECIFIC IMPLEMENTATIONS GO TO https://xilinx.github.io/Vitis-Tutorials/2020-1/docs/README.html */

#define __CL_ENABLE_EXCEPTIONS
#include <vector> 
#include <unordered_map>
#include <string>
#include <fstream>

#include "CL/opencl.hpp" // using the cpp bindings
#include "CL/opencl.h"

class clBuffer {
private: 
	cl::Buffer m_DeviceBuffer; // memory at Device Buffer  
	void* m_HostData; 
	int m_HostDataSize; 
	int m_workdim[3];
	decltype 
public: 
	clBuffer(int work_dim[3],T* data, int data_size, const cl::Context& context, cl_mem_flags flag) {
	
		for (int i = 0; i < 3; i++) m_workdim[i] = work_dim[i];
		m_HostDataSize = data_size;
		m_DeviceBuffer = cl::Buffer(context, flag, data_size, data, NULL);
}

	clBuffer(const clBuffer& buffer) {
		m_HostData = buffer.m_HostData;
		m_HostDataSize = buffer.m_HostDataSize; 
		for (int i = 0; i < 3; i++) m_workdim[i] = buffer.m_workdim[i];
		m_DeviceBuffer = std::move(buffer.m_DeviceBuffer);
	}
	cl::Buffer& GetDeviceBuffer() {
		return m_DeviceBuffer;
	}

	int* GetWorkDimensions() {
		return m_workdim;
	}
};

class  OpenCLProgram {

	struct clKernel {
		cl::Kernel kernel;
		cl::NDRange range; 
		int status = 0; // finished or not 
		};
	struct clDevice {
		cl::Platform platform; 
		cl::Context context; 
		cl::CommandQueue queue; 
	};

private: 
	static OpenCLProgram* m_Singleton; 
	std::vector<cl::Context>  m_Contexts; 
	std::vector<cl::Platform> m_Platforms; 
	std::vector<cl::Device> m_Devices; 
	std::unordered_map<std::string, clBuffer> m_Buffers;
	std::vector<cl::CommandQueue> m_CommandQueues; 
	std::vector<cl::Program> m_Programs;  
	std::unordered_map<std::string, clKernel> m_Kernels; 
	void FetchPlatforms();
	void FetchDevices(); 
	void CreateContextsAndQueues(); 
	std::vector<std::string>& GetKernelsFromSource(const std::string& file_path); 
public: 

	OpenCLProgram() { m_Singleton };
	~OpenCLProgram(); 
	
	void Init() ; 
	template <typename T> 
	void AddBuffer(const std::string& buffer_name, T* data, int data_size, const cl::Context& context, int work_dimension[3], cl_mem_flags flag); 
	// Operations on kernels 
	void CreateKernels(const std::string& file_path, const cl::Context& context); // must be .cl  file 
	template <typename T, typename... Args> 
	void SetKernelArgs(const std::string& kernel_name, T& value, int index =0 , Args... args) {  // in order; 
		if (m_Kernels.find(kernel_name) != m_Kernels.end()) {
			m_Kernels[kernel_name].kernel.setArg(index, value); 
		}
		SetKernelArgs(kernel_name, args, index = index + 1); 
	}
	void SetKernelWorkDim(const std::string& kernel_name, int work_dimension[3]) {
		m_Kernels[kernel_name].range = cl::NDRange(work_dimension[0], work_dimension[1], work_dimension[2]); 
	}
	void ExecuteAllKernels_b(const std::string& kernel_name, cl::NDRange offset= cl::NullRange) {  // blocking for this case
		if (m_Kernels.find(kernel_name) != m_Kernels.end()) {
			std::vector<cl::Event> clevents{cl::Event()};
			m_CommandQueues[0].enqueueNDRangeKernel(m_Kernels[kernel_name].kernel, offset, m_Kernels[kernel_name].range, cl::NullRange, NULL, &clevents[0]); 
			clevents[0].wait(); 
		}
	}
	cl::Buffer &GetDeviceBuffer(const std::string& buffer_name) {
		return m_Buffers[buffer_name].GetDeviceBuffer(); 
	}
	OpenCLProgram(OpenCLProgram& other) = delete;
	void operator=(const OpenCLProgram&) = delete;
	static OpenCLProgram* GetInstance() {
		if (m_Singleton == nullptr) { m_Singleton = new OpenCLProgram;  } return m_Singleton;
	} 
};


void OpenCLProgram::FetchPlatforms() {

	if (cl::Platform::get(&m_Platforms) != CL_SUCCESS) {
		// error; 
	}
	if (m_Platforms.size() == 0) {
		// didn't find opencl platforms ; 
	}
	
}

void OpenCLProgram::FetchDevices()
{
	// for now only taking devices of first platform of all types 
	for (auto& platform : m_Platforms) {
		platform.getDevices(CL_DEVICE_TYPE_ALL, &m_Devices);;
	}
}

void OpenCLProgram::CreateContextsAndQueues()
{
	for (int i = 0; i < m_Devices.size(); i++) {
		m_Contexts.push_back(cl::Context(m_Devices[i]));
		m_CommandQueues.push_back(cl::CommandQueue(m_Contexts[i]));
	}
}

std::vector<std::string>& OpenCLProgram::GetKernelsFromSource(const std::string& file_path)
{
	std::vector<std::string> kernel_names;
	std::ifstream file(file_path);
	for (std::string line; std::getline(file, line); ) {
		if (line.find("__kernel") != std::string::npos) {
			// found kernel 
			std::size_t pos = line.find(' ', 0);
			std::size_t pos2 = line.find('(', pos + 1);
			std::string kernel_name = line.substr(pos + 1, pos2 - pos - 1);
			kernel_names.push_back(kernel_name);
		}
	}
}

inline void OpenCLProgram::Init()
{
	FetchPlatforms();
	FetchDevices();
	CreateContextsAndQueues();

}
inline void OpenCLProgram::AddBuffer(const std::string& buffer_name, void* data, int data_size,  const cl::Context& context, int work_dimension[3], cl_mem_flags flag)
{
	if (m_Buffers.find(buffer_name) == m_Buffers.end()) {

		m_Buffers[buffer_name] = clBuffer(work_dimension, data,data_size,context, flag);
		return;
	}
	// buffer with name already exists 

}

// With linking 
inline void OpenCLProgram::CreateKernels(const std::string& file_path, const cl::Context& context)
{
	// create program 
	m_Programs.push_back(cl::Program(context, file_path, true));
	// create kernels from string
	for (auto& kernel_name : GetKernelsFromSource(file_path)) {
		m_Kernels[kernel_name].kernel = cl::Kernel::Kernel(m_Programs[m_Programs.size() - 1], kernel_name.c_str());
	}

}

