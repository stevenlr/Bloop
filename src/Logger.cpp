#include "Logger.h"

#include <iostream>

using namespace std;

Logger Logger::_instance;

Logger::Logger() :
		_stream(&cerr)
{
}

Logger::~Logger()
{
	if (_stream != &cout && _stream != &cerr && _stream != &clog)
		delete _stream;
}

void Logger::init(ostream *stream)
{
	Logger::_instance._stream = stream;
}

ostream &Logger::logInfo(const char *file, int line)
{
	*Logger::_instance._stream << "[INFO] " << file << ":l." << line << endl;
	return *Logger::_instance._stream;
}

ostream &Logger::logWarning(const char *file, int line)
{
	*Logger::_instance._stream << "[WARNING] " << file << ":l." << line << endl;
	return *Logger::_instance._stream;
}

ostream &Logger::logError(const char *file, int line)
{
	*Logger::_instance._stream << "[ERROR] " << file << ":l." << line << endl;
	return *Logger::_instance._stream;
}
