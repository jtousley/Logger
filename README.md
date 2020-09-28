// jsltousley@gmail.com
A simple C++11 logger

Usage:
Logging::LogManager& logger(Logging::LogManager::getManager());
logger.setLevel(Logging::LogLevel::LEVEL_INFO);
logger.setFile(/path/to/file); // optional
logger.start();
LOG_INFO("Something to log");

...

logger.stop();
