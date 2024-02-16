#include "server.h"

#include <iostream>
#include <thread>
#include <boost/asio/thread_pool.hpp>
#include <boost/program_options.hpp>

//#define BOOST_ASIO_ENABLE_HENDLER_TRACKING

namespace po = boost::program_options;

/** @brief Server for performs intersection and symmetrical difference operations.
*
*/
int main(int argc, char **argv) {
    //processing сommand line argument
    po::options_description desc {"Options"};
    desc.add_options()
            ("help,h", "Server for package processing of commands")
            ("port,p", po::value<unsigned short>() -> default_value(9000), "listening port");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }

    try {
        ba::thread_pool pool(2);
        ba::signal_set poolSignals{pool, SIGINT, SIGTERM};
        poolSignals.async_wait([&pool](auto, auto) { pool.stop(); });

        ba::io_context ioContext;
        ba::signal_set ioContextSignals{ioContext, SIGINT, SIGTERM};
        ioContextSignals.async_wait([&ioContext](auto, auto) { ioContext.stop(); });
        std::thread dataThread([&ioContext]() { ioContext.run(); });

        ba::io_context parserIoContext;
        ba::signal_set pIoContextSignals{parserIoContext, SIGINT, SIGTERM};
        pIoContextSignals.async_wait([&parserIoContext](auto, auto) { parserIoContext.stop(); });
        std::thread parserThread([&parserIoContext]() { parserIoContext.run(); });

        //start server
        {
            Data data(ioContext, parserIoContext);
            Server s(pool, data);
            s.listen(vm["port"].as<unsigned short>());
            pool.join();
            dataThread.join();
            parserThread.join();
        }
        return EXIT_SUCCESS;
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
    } catch (...) {
        std::cerr << "Fatal error: unknown" << std::endl;
    }

    return EXIT_FAILURE;
}