#include "CLI11.hpp"
#include "leb128.h"

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <future>
#include <chrono>
#include <random>

using namespace std;

auto throwValidationError(const CLI::App &app, const string &error) {
    try {
        throw CLI::ValidationError(error);
    } catch (const CLI::Error &e) {
        return app.exit(e);
    }
}


int main(int argc, char* argv[]) {
    CLI::App app{"Simulation of data transfer with LEB128"};

    bool transferUnsigned{false};
    app.add_flag("-u,--unsigned", transferUnsigned, "Encode with unsigned Leb128");

    int start{-100000};
    auto startOption {
        app.add_option("-s,--start", start, "Start of the range of random numbers")
        ->check(CLI::Range(-100000, 100000))
    };

    int end{100000};
    auto endOption {
        app.add_option("-e,--end", end, "End of the range of random numbers")
        ->check(CLI::Range(-100000, 100000))
    };

    unsigned short delay{1000};
    app.add_option("-d,--delay", delay, "Delay between data transfer in ms");

    vector<int> values;
    auto valuesOption{
        app.add_option("-v,--values", values, "Values to transfer in a loop")
        ->check(CLI::Range(-100000, 100000))
    };

    string jsonOutputPath;
    app.add_option("--json-output-name", jsonOutputPath, "Name of json output file");

    string tomlOutputPath;
    app.add_option("--toml-output-name", tomlOutputPath, "Name of toml output file");

    string fileOutputPath;
    app.add_option("--file-output-name", fileOutputPath, "Name of plain output file");

    startOption->excludes(valuesOption);
    startOption->needs(endOption);
    endOption->needs(startOption);

    CLI11_PARSE(app, argc, argv);

    if(transferUnsigned and start < 0) {
        return throwValidationError(app, "--start: start must be between 0 and 100000");
    } else if (start > end) {
        return throwValidationError(app, "--start --end: start must be smaller than end");
    }

    random_device rd;
    mt19937 gen{rd()};
    uniform_int_distribution<> dis{start, end};

    while(true) {
        promise<string> promise;
        future<string> future{promise.get_future()};

        thread t1{[&]{
            int value{dis(gen)};

            cout << value << endl;

            promise.set_value(LEB128::toSignedLeb128(value));
            this_thread::sleep_for(chrono::seconds(1));
        }};

        thread t2{[&future]{
            cout << LEB128::signedLeb128toDecimal(future.get()) << endl;
        }};

        t1.join();
        t2.join();
    }
}
