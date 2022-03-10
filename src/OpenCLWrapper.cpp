#include "include\OpenCLWrapper.h"
#include <fstream>
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
	for (auto& platform : m_Platforms){
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
			std::size_t pos2 = line.find(' ', pos + 1); 
			std::string kernel_name = line.substr(pos+1, pos2-pos-1 ); 
			kernel_names.push_back(kernel_name); 
		}
	}
}

void OpenCLProgram::Init()
{
	FetchPlatforms();
	FetchDevices(); 
	CreateContextsAndQueues(); 
	
}
void OpenCLProgram::AddBuffer(const std::string& buffer_name, const std::vector<float>& data, const cl::Context& context, int work_dimension[3], bool READ_ONLY)
{
	if (m_Buffers.find(buffer_name) == m_Buffers.end()) {
	
		m_Buffers[buffer_name] = std::move(clBuffer(clBuffer(work_dimension, data, context, READ_ONLY))); 
		return; 
	}
	// buffer with name already exists 

}

// With linking 
void OpenCLProgram::CreateKernels(const std::string& file_path, const cl::Context& context)
{
	// create program 
	m_Programs.push_back(cl::Program(context, file_path, true)); 
	// create kernels from string
	for (auto& kernel_name : GetKernelsFromSource(file_path)) {
		m_Kernels[kernel_name].kernel = cl::Kernel(m_Programs[m_Programs.size() - 1], kernel_name.c_str()); 
	}

}

