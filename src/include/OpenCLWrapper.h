#pragma once 
#define __CL_ENABLE_EXCEPTIONS
#include <vector> 
#include <unordered_map>
#include "CL/opencl.hpp" // using the cpp bindings
#include "Core.h"
class BNN_LIB clProgram {
	cl::Buffer m_Buffers;

};
class BNN_LIB OpenCLProgram {
	struct clBuffer {
		std::vector<cl::Kernel> m_Kernels;
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
			m_Kernels = buffer.m_Kernels; 
			for (int i = 0; i < 3; i++) m_workdim[i] = buffer.m_workdim[i]; 
			m_DeviceBuffer = buffer.m_DeviceBuffer; 
		}

	};	
private: 
	std::vector<cl::Context>  m_Contexts; 
	std::vector<cl::Platform> m_Platforms; 
	std::vector<cl::Device> m_Devices; 
	std::unordered_map<std::string, clBuffer> m_Buffers;
	std::vector<cl::CommandQueue> m_CommandQueues; 
	
	std::vector<cl::Program> m_Programs;  

	void FetchPlatforms();
	void FetchDevices(); 
	void CreateContextsAndQueues(); 
public: 

	OpenCLProgram(); 
	~OpenCLProgram(); 
	
	void Init() ; 
	
	void AddBuffer(const std::string& buffer_name, const std::vector<float>& data, const cl::Context& context, int work_dimension[3] ,bool READ_ONLY = false); 
	void DivideWorkItems(const std::string& buffer_name, int work_item_count);
	void CreateKernels(const std::string& file_path); // must be .cl  file 

	void AttatchKernelToBuffer(const std::string& kernel_path, const std::string& buffer_name); 
	
	void RunPrograms();
	
};
