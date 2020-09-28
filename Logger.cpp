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

#include "Logger.h"

#include <iostream>
#include <fstream>

namespace {
std::mutex mutex_;
}

namespace Logging {

LogMessage::LogMessage(const LogLevel level, const std::string file, const std::string function,
                       //   const std::string &module,
                       const std::string& msg)
    : m_logLevel(level), m_file(file), m_function(function), m_msg(msg) {}

LogLevel LogMessage::getLogLevel() const {
  return m_logLevel;
}
std::string LogMessage::getFileOrClassname() const {
  size_t pos = m_file.rfind('/');
  if (pos == std::string::npos) {
    pos = -1;
  }
  return m_file.substr(pos + 1, m_file.size());
}
std::string LogMessage::getFunction() const {
  return m_function;
}
std::string LogMessage::getMsg() const {
  return m_msg;
}

LogMessage::LogMessage(const LogMessage& rhs)
    : m_logLevel(rhs.getLogLevel()),
      m_file(rhs.getFileOrClassname()),
      m_function(rhs.getFunction()),
      m_msg(rhs.getMsg()) {}

LogMessage LogMessage::operator=(const LogMessage& rhs) {
  m_logLevel = rhs.getLogLevel();
  m_file = rhs.getFileOrClassname();
  m_function = rhs.getFunction();
  m_msg = rhs.getMsg();
  return *this;
}

std::ostream& operator<<(std::ostream& out, const LogMessage& msg) {
  out << "[" << LogMessage::logLevelToString(msg.getLogLevel()) << "]"
      << " " << msg.getFileOrClassname() << " " << msg.getFunction() << " - " << msg.getMsg();
  return out;
}

std::string LogMessage::logLevelToString(const LogLevel level) {
  switch (level) {
  case LogLevel::LEVEL_FATAL:
    return "FATAL";
  case LogLevel::LEVEL_ERROR:
    return "ERROR";
  case LogLevel::LEVEL_WARN:
    return "WARN";
  case LogLevel::LEVEL_INFO:
    return "INFO";
  case LogLevel::LEVEL_DEBUG:
    return "DEBUG";
  case LogLevel::LEVEL_TRACE:
    return "TRACE";
  case LogLevel::LEVEL_TRACE1:
    return "TRACE1";
  case LogLevel::LEVEL_TRACE2:
    return "TRACE2";
  case LogLevel::LEVEL_TRACE3:
    return "TRACE3";
  case LogLevel::LEVEL_TRACE4:
    return "TRACE4";
  case LogLevel::LEVEL_TRACE5:
    return "TRACE5";
  default:
    break;
  }
  return "NONE";
}

LogManager::LogManager() : m_running(false), m_useFile(false) {}

LogManager& LogManager::getManager() {
  std::unique_lock<std::mutex> mlock(mutex_);
  static LogManager* manager;

  if (!manager) {
    manager = new LogManager();
  }

  return *manager;
}

void LogManager::setLevel(LogLevel level) {
  m_globalLevel = level;
}

void LogManager::setFile(std::string logFile) {
  m_useFile = true;
  m_logFile = logFile;

  std::ofstream ofs;
  ofs.open(m_logFile, std::ofstream::trunc);
  ofs.close();
}

void LogManager::log(const LogLevel& level, const std::string& file, const std::string& function,
                     const std::string& msg) {
  LogMessage logMsg(level, file, function, msg);
  m_queue.push(logMsg);
}

void LogManager::start() {
  std::unique_lock<std::mutex> mlock(m_mutex);
  m_running = true;
  m_future = std::async(std::launch::async, &Logging::LogManager::run, &*this);
}

void LogManager::stop() {
  std::unique_lock<std::mutex> mlock(m_mutex);
  m_running = false;
  LOG_TRACE("With this, I proclaim myself president of the tautology club");
  if (m_future.valid()) {
    m_future.wait();
    m_future.get();
  }
}

void LogManager::run() {
  while (m_running) {
    LogMessage msg = m_queue.pop();
    if (m_globalLevel >= msg.getLogLevel()) {
      // print to console
      std::cout << msg << std::endl;
      if (m_useFile) {
        // print to file
        std::ofstream ofs;
        ofs.open(m_logFile, std::ofstream::out | std::ofstream::app);
        ofs << msg << std::endl;
        ofs.close();
      }
    }
  }
}

}  // namespace Logging