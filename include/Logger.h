#ifndef LOGGER_H_
#define LOGGER_H_

#include <ostream>

class Logger {
public:
	Logger();
	~Logger();

	static void init(std::ostream *stream);

	static std::ostream &logInfo(const char *file, int line);
	static std::ostream &logWarning(const char *file, int line);
	static std::ostream &logError(const char *file, int line);

private:
	std::ostream *_stream;
	static Logger _instance;
};

#define LOGINFO Logger::logInfo(__FILE__, __LINE__)
#define LOGWARNING Logger::logWarning(__FILE__, __LINE__)
#define LOGERROR Logger::logError(__FILE__, __LINE__)

#endif
