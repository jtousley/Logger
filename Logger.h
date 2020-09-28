/*
Copyright 2020 tousley@gdls.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <sstream>
#include <future>

#include "ConcurrentQueue.h"

namespace Logging {

enum class LogLevel {
  LEVEL_NONE,
  LEVEL_FATAL,
  LEVEL_ERROR,
  LEVEL_WARN,
  LEVEL_INFO,
  LEVEL_DEBUG,
  LEVEL_TRACE,
  LEVEL_TRACE1,
  LEVEL_TRACE2,
  LEVEL_TRACE3,
  LEVEL_TRACE4,
  LEVEL_TRACE5
};

class LogMessage {
 public:
  LogMessage(const LogLevel level, const std::string file, const std::string function,
             //   const std::string &module,
             const std::string& msg);

  LogLevel getLogLevel() const;
  std::string getFileOrClassname() const;
  std::string getFunction() const;
  std::string getMsg() const;

  LogMessage(const LogMessage& rhs);
  LogMessage operator=(const LogMessage& rhs);
  friend std::ostream& operator<<(std::ostream& out, const LogMessage& msg);

  static std::string logLevelToString(const LogLevel);

 private:
  LogLevel m_logLevel;
  std::string m_file;
  std::string m_function;
  std::string m_msg;
};

class LogManager {
 public:
  ~LogManager();
  static LogManager& getManager();

  void setLevel(LogLevel level);
  void setFile(std::string logFile);

  void log(const LogLevel& level, const std::string& file, const std::string& function,
           const std::string& msg);

  void start();
  void stop();

 protected:
  LogManager();
  void run();
  LogLevel m_globalLevel;
  std::string m_logFile;
  bool m_running;
  bool m_useFile;

 private:
  ConcurrentQueue<LogMessage> m_queue;
  std::future<void> m_future;
  std::mutex m_mutex;
};

class Loggable {

 public:
  /**
   * Creates a object which is loggable.
   *
   * @param name
   */
  inline Loggable(const char* name) : m_loggerName(name) {}

  /**
   * Creates a object which is loggable.
   *
   * @param name
   */
  inline Loggable(const std::string& name) : m_loggerName(name) {}

  /**
   * Virtual destructor.
   */
  inline virtual ~Loggable() {}

 protected:
  /**
   * An object for the logger name.
   */
  std::string m_loggerName;
};

}  // namespace Logging

#define _S(message)                                                                                \
  ({                                                                                               \
    std::stringstream _msg;                                                                        \
    _msg << message;                                                                               \
    _msg.str();                                                                                    \
  })

#define LOG_FATAL(msg)                                                                             \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_FATAL, __FILE__, __FUNCTION__,    \
                                        _S(msg))
#define LOG_SEVERE(msg)                                                                            \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_FATAL, __FILE__, __FUNCTION__,    \
                                        _S(msg))
#define LOG_ERROR(msg)                                                                             \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_ERROR, __FILE__, __FUNCTION__,    \
                                        _S(msg))
#define LOG_WARN(msg)                                                                              \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_WARN, __FILE__, __FUNCTION__,     \
                                        _S(msg))
#define LOG_INFO(msg)                                                                              \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_INFO, __FILE__, __FUNCTION__,     \
                                        _S(msg))
#define LOG_DEBUG(msg)                                                                             \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_DEBUG, __FILE__, __FUNCTION__,    \
                                        _S(msg))
#define LOG_TRACE(msg)                                                                             \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_TRACE, __FILE__, __FUNCTION__,    \
                                        _S(msg))
#define LOG_TRACE1(msg)                                                                            \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_TRACE1, __FILE__, __FUNCTION__,   \
                                        _S(msg))
#define LOG_TRACE2(msg)                                                                            \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_TRACE2, __FILE__, __FUNCTION__,   \
                                        _S(msg))
#define LOG_TRACE3(msg)                                                                            \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_TRACE3, __FILE__, __FUNCTION__,   \
                                        _S(msg))
#define LOG_TRACE4(msg)                                                                            \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_TRACE4, __FILE__, __FUNCTION__,   \
                                        _S(msg))
#define LOG_TRACE5(msg)                                                                            \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_TRACE5, __FILE__, __FUNCTION__,   \
                                        _S(msg))

// loggable macros
#define LOGGABLE_FATAL(msg)                                                                        \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_FATAL, m_loggerName,              \
                                        __FUNCTION__, _S(msg))
#define LOGGABLE_ERROR(msg)                                                                        \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_ERROR,                            \
                                        m_loggerName,             \ __FUNCTION__, _S(msg))
#define LOGGABLE_WARN(msg)                                                                         \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_WARN,                             \
                                        m_loggerName,             \ __FUNCTION__, _S(msg))
#define LOGGABLE_INFO(msg)                                                                         \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_INFO,                             \
                                        m_loggerName,             \ __FUNCTION__, _S(msg))
#define LOGGABLE_DEBUG(msg)                                                                        \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_DEBUG,                            \
                                        m_loggerName,             \ __FUNCTION__, _S(msg))
#define LOGGABLE_TRACE(msg)                                                                        \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_TRACE,                            \
                                        m_loggerName,             \ __FUNCTION__, _S(msg))
#define LOGGABLE_TRACE1(msg)                                                                       \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_TRACE1,                           \
                                        m_loggerName,             \ __FUNCTION__, _S(msg))
#define LOGGABLE_TRACE2(msg)                                                                       \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_TRACE2,                           \
                                        m_loggerName,             \ __FUNCTION__, _S(msg))
#define LOGGABLE_TRACE3(msg)                                                                       \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_TRACE3,                           \
                                        m_loggerName,             \ __FUNCTION__, _S(msg))
#define LOGGABLE_TRACE4(msg)                                                                       \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_TRACE4,                           \
                                        m_loggerName,             \ __FUNCTION__, _S(msg))
#define LOGGABLE_TRACE5(msg)                                                                       \
  Logging::LogManager::getManager().log(Logging::LogLevel::LEVEL_TRACE5,                           \
                                        m_loggerName,             \ __FUNCTION__, _S(msg))

#endif
