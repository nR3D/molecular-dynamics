#ifndef PARTICLE_FLUID_LOGGER_HPP
#define PARTICLE_FLUID_LOGGER_HPP

#include <string_view>
#include <type_traits>
#include <filesystem>
#include <fstream>
#include <iostream>

enum class log_level {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

/*
 * @brief Helper class to log events during execution to a file
 * @note Not constructible, use Logger::instance() to access object
 */
class Logger {
public:

    class LogStream {
        friend Logger;
    public:
        LogStream& operator<<(std::string_view message);

        template<typename Numeric>
        std::enable_if_t<std::is_arithmetic_v<Numeric>, LogStream>& operator<<(Numeric message) {
            return operator<<(std::to_string(message));
        }
    private:
        LogStream(log_level _current, log_level _min);

        LogStream& start_message(log_level level);

        LogStream& end_message();

        std::ofstream file;
        log_level current_level, min_cout;
        bool first_message;
    };

    /*
     * @brief Singleton instance of Logger
     */
    static Logger& instance();

    /*
     * @brief Open a folder and create a log file 'fluidchen.log',
     *        if such file already exists results will be appended to it
     * @param[in] folder where the log fill is opened
     * @param[in] min_print_level Every message that will be logged with this or
     *                            a higher level will also be printed through std::cout
     * @warning If another file is already opened, close it first with Logger::close()
     */
    void open(const std::filesystem::path& folder, log_level min_level_to_print = log_level::INFO);


    /*
     * @brief Start a log line of given level, make sure to open a file first
     * @return LogStream
     * @see Logger::open()
     */
    LogStream& log(log_level level = log_level::DEBUG);

    /*
     * @brief Close previously opened connection to log file
     * @see Logger::open()
     */
    void close();

private:
    Logger() : logstream(log_level::DEBUG, log_level::INFO) {};

    LogStream logstream;

    static std::string_view level_to_string(log_level level);
};

#endif // PARTICLE_FLUID_LOGGER_HPP
