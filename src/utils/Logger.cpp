#include "Logger.hpp"

std::string_view Logger::level_to_string(log_level level) {
    switch (level) {
        case log_level::DEBUG:
            return "DEBUG";
        case log_level::INFO:
            return "INFO";
        case log_level::WARNING:
            return "WARNING";
        case log_level::ERROR:
            return "ERROR";
        default:
            std::cerr << "Log level " << static_cast<int>(level) << " not recognized" << std::endl;
            return "LEVEL NOT RECOGNIZED";
    }
}
void Logger::close() {
    logstream.end_message();
    logstream.file.close();
}

void Logger::open(const std::filesystem::path &folder, log_level min_level_to_print) {
    if(logstream.file.is_open())
        std::cerr << "Logger already opened in another file";
    else {
        logstream.file.open(folder / "data.log", std::ios_base::app);
        logstream.min_cout = min_level_to_print;
    }
}
Logger &Logger::instance() {
    static Logger instance;
    return instance;
}
Logger::LogStream &Logger::log(log_level level) {
    return logstream.start_message(level);
}
Logger::LogStream &Logger::LogStream::operator<<(std::string_view message) {
    if(file.is_open()) {
        file << ' ' << message;
        if(current_level >= min_cout)
            std::cout << ' ' << message;
    } else {
        std::cerr << "Tried to log before opening file" << std::endl;
    }
    return *this;
}
Logger::LogStream::LogStream(log_level _current, log_level _min)
        : current_level(_current), min_cout(_min), first_message(true) {}

Logger::LogStream &Logger::LogStream::start_message(log_level level) {
    current_level = level;
    if(!first_message) {
        file << std::endl;
        if(current_level >= min_cout)
            std::cout << std::endl;
    }
    else
        first_message = false;
    if(file.is_open()) {
        file << '[' << level_to_string(current_level) << ']';
        if(current_level >= min_cout)
            std::cout << '[' << level_to_string(current_level) << ']';
    } else {
        std::cerr << "Tried to log before opening file" << std::endl;
    }
    return *this;
}

Logger::LogStream &Logger::LogStream::end_message() {
    if(!first_message) {
        file << std::endl;
        if(current_level >= min_cout)
            std::cout << std::endl;
    }
    return *this;
}
