#pragma once 
#define __CL_ENABLE_EXCEPTIONS
#include <vector> 
#include <unordered_map>
#include "CL/opencl.hpp" // using the cpp bindings
#include "Core.h"
class BNN_LIB OpenCLProgram {
	struct clBuffer {
			cl::Buffer m_DeviceBuffer; // memory at Device Buffer  
		std::vector<float> m_HostData; // Memory Buffer at host device 
		int m_workdim[3]; 
		clBuffer(int work_dim[3] , const std::vector<float>&  data, const cl::Context& context , bool READ_ONLY) {
			
			m_DeviceBuffer = cl::Buffer(context, data.begin(), data.end(), READ_ONLY); 
			for (int i = 0  ; i < 3; i ++) m_workdim[i] = work_dim[i]; 
			m_HostData = data; 
			
		}

		clBuffer(clBuffer& buffer) {
			m_HostData = buffer.m_HostData; 
			for (int i = 0; i < 3; i++) m_workdim[i] = buffer.m_workdim[i]; 
			m_DeviceBuffer = buffer.m_DeviceBuffer; 
		}
		cl::Buffer& GetDeviceBuffer() {
			return m_DeviceBuffer; 
		}
		int* GetWorkDimensions() {
			return m_workdim; 
		}
		
	};	
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

	OpenCLProgram(); 
	~OpenCLProgram(); 
	
	void Init() ; 
	
	void AddBuffer(const std::string& buffer_name, const std::vector<float>& data, const cl::Context& context, int work_dimension[3] ,bool READ_ONLY = false); 
	
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
	void ExecuteKernel_b(const std::string& kernel_name, cl::NDRange offset= cl::NullRange) {  // blocking for this case
		if (m_Kernels.find(kernel_name) != m_Kernels.end()) {
			std::vector<cl::Event> clevents{cl::Event()};
			m_CommandQueues[0].enqueueNDRangeKernel(m_Kernels[kernel_name].kernel, offset, m_Kernels[kernel_name].range, cl::NullRange, NULL, &clevents[0]); 
			clevents[0].wait(); 
		}
	}
	cl::Buffer &GetDeviceBuffer(const std::string& buffer_name) {
		return m_Buffers[buffer_name].GetDeviceBuffer(); 
	}
	
};
