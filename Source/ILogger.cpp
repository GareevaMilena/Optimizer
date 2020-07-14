#include "ILogger.h"
#include <iostream>
#include <fstream>
#include <vector>

namespace {
	class MiLogger : public ILogger {
	private:
		std::ofstream logger;
		std::vector<void*> clients;

	public:
		void log(char const* pMsg, enum RESULT_CODE err);
		void destroyLogger(void* pClient);
		RESULT_CODE setLogFile(char const* pLogFile);
		static MiLogger *createLogger(void *pClient);
		static std::unique_ptr<MiLogger> singleLogger;
		
		void printError(enum RESULT_CODE r);

		~MiLogger() = default;
	};
}

std::unique_ptr<MiLogger> MiLogger::singleLogger = nullptr;

ILogger::~ILogger() {}

ILogger* ILogger::createLogger(void* pClient)
{
	return MiLogger::createLogger(pClient);
}

MiLogger *MiLogger::createLogger(void *pClient)
{
	if (singleLogger == nullptr) {
		singleLogger = std::make_unique<MiLogger>();
	}
	for (size_t i = 0; i < singleLogger->clients.size(); i++)
	{			
		if (singleLogger->clients[i] == pClient)
		{
			singleLogger-> logger << "This client already exists" << std::endl;
			return singleLogger.get();
		}
	}
	singleLogger->clients.push_back(pClient);
	return singleLogger.get();
}

void MiLogger::destroyLogger(void* pClient)
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i] == pClient)
		{
			clients.erase(clients.begin() + i);
			return;
		}
	}
	RESULT_CODE err = RESULT_CODE::NOT_FOUND;
	printError(err);
	logger << "This client was not found" << std::endl;
	return;
}


void MiLogger::printError(enum RESULT_CODE r)
{
	switch (r)
	{
	case RESULT_CODE::SUCCESS:
		logger << "Error code: " << "SUCCESS" << std::endl;
		break;
	case RESULT_CODE::OUT_OF_MEMORY:
		logger << "Error code: " << "OUT_OF_MEMORY" << std::endl;
		break;
	case RESULT_CODE::BAD_REFERENCE:
		logger << "Error code: " << "BAD_REFERENCE" << std::endl;
		break;
	case RESULT_CODE::WRONG_DIM:
		logger << "Error code: " << "WRONG_DIM" << std::endl;
		break;
	case RESULT_CODE::DIVISION_BY_ZERO:
		logger << "Error code: " << "DIVISION_BY_ZERO" << std::endl;
		break;
	case RESULT_CODE::NAN_VALUE:
		logger << "Error code: " << "NAN_VALUE" << std::endl;
		break;
	case RESULT_CODE::FILE_ERROR:
		logger << "Error code: " << "FILE_ERROR" << std::endl;
		break;
	case RESULT_CODE::OUT_OF_BOUNDS:
		logger << "Error code: " << "OUT_OF_BOUNDS" << std::endl;
		break;
	case RESULT_CODE::NOT_FOUND:
		logger << "Error code: " << "NOT_FOUND" << std::endl;
		break;
	case RESULT_CODE::WRONG_ARGUMENT:
		logger << "Error code: " << "WRONG_ARGUMENT" << std::endl;
		break;
	case RESULT_CODE::CALCULATION_ERROR:
		logger << "Error code: " << "CALCULATION_ERROR" << std::endl;
		break;
	case RESULT_CODE::MULTIPLE_DEFINITION:
		logger << "Error code: " << "MULTIPLE_DEFINITION" << std::endl;
		break;
	}
}

void MiLogger::log(char const* pMsg, enum RESULT_CODE err)
{
	printError(err);
	logger << pMsg << std::endl;
	logger << std::endl;
}

RESULT_CODE MiLogger::setLogFile(char const* pLogFile)
{
	logger.open(pLogFile);
	if (logger.is_open())
		return RESULT_CODE::SUCCESS;
	std::cout << "Logger not created" << std::endl;
	return RESULT_CODE::FILE_ERROR;
}


