#include "leb128.h"
#include "CLI11.hpp"
#include "json.hpp"
#include "toml.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <future>
#include <chrono>
#include <random>
#include <fstream>

using namespace std;
using json = nlohmann::json;
using namespace toml;

auto logger = spdlog::basic_logger_mt("LEB128 Logger", "./leb128.log");

int getNextElement(const vector<int> &values) {
    static vector<int>::size_type i = 0;
    if(i == values.size()) {
        i = 0;
    }
    return values[i++];
}

auto throwValidationError(const CLI::App &app, const string &error) {
    try {
        logger->error(error);
        throw CLI::ValidationError(error);
    } catch (const CLI::Error &e) {
        return app.exit(e);
    }
}

auto writeJsonFile(const string &name, const json &outputJson) {
    ofstream o(name);
    o << std::setw(4) << outputJson << endl;
}

int main(int argc, char* argv[]) {
    CLI::App app{"Simulation of data transfer with LEB128"};

    logger->set_level(spdlog::level::trace);
    logger->set_pattern("[%Y %m %d %H:%M:%S,%e] [%l] [thread %t] %v");

    bool transferUnsigned{false};
    app.add_flag("-u,--unsigned", transferUnsigned, "Encode with unsigned LEB128");

    bool showEncoded{false};
    app.add_flag("--show-encoded", showEncoded, "Show encoded values");

    int start{-100001};
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

    string jsonOutputName;
    app.add_option("--json-output-name", jsonOutputName, "Name of json output file");

    string tomlPath;
    app.add_option("--toml-path", tomlPath, "Name of toml configuration file (overrides)")
    ->check(CLI::ExistingFile);

    startOption->excludes(valuesOption);
    endOption->excludes(valuesOption);

    CLI11_PARSE(app, argc, argv);

    if(tomlPath != "") {
        table configuration;
        try {
            configuration = parse_file(tomlPath);
            if(configuration["values"] && (configuration["start"] || configuration["end"])) {
                return throwValidationError(app, "values can't be combined with start or end");
            }

            if(configuration["values"]) {
                for (const auto &elem : *configuration["values"].as_array()) {
                    int val{elem.value<int>().value()};
                    if(val < -100000 || val > 100000) {
                        return throwValidationError(app, "values not in range -100000 to 100000");
                    }
                    values.push_back(val);
                }
            } else {
                if (configuration["start"]) {
                    start = configuration["start"].value<int>().value();
                    if(start < -100000 || start > 100000) {
                        return throwValidationError(app, "start not in range -100000 to 100000");
                    }
                }

                if (configuration["end"]) {
                    end = configuration["end"].value<int>().value();
                    if(end < -100000 || end > 100000) {
                        return throwValidationError(app, "end not in range -100000 to 100000");
                    }
                }
            }

            if(configuration["unsigned"]) {
                transferUnsigned = configuration["unsigned"].value<bool>().value();
            }
            if(configuration["show-encoded"]) {
                showEncoded = configuration["show-encoded"].value<bool>().value();
            }
            if(configuration["delay"]) {
                delay = configuration["delay"].value<unsigned short>().value();
            }
            if (configuration["json-output-name"]) {
                jsonOutputName = configuration["json-output-name"].value<string>().value();
            }

        } catch (const toml::parse_error& err) {
            return throwValidationError(app, "error in configuation file - parsing failed:\n" + string(err.what()));
        }
    }

    if (start == -100001) {
        start = transferUnsigned ? 0 : -100000;
    }

    if(transferUnsigned and start < 0) {
        return throwValidationError(app, "--start: start must be between 0 and 100000");
    } else if (start > end) {
        return throwValidationError(app, "--start --end: start must be smaller than end");
    }

    random_device rd;
    mt19937 gen{rd()};
    uniform_int_distribution<> dis{start, end};

    json outputJson;

    if(jsonOutputName != "") {
        if(values.size()) {
            outputJson["values"] = values;
        } else {
            outputJson["start"] = start;
            outputJson["end"] = end;
        }

        outputJson["unsigned"] = transferUnsigned;
        outputJson["show-encoded"] = showEncoded;
        outputJson["delay"] = delay;
        outputJson["data"] = json::array();
        writeJsonFile(jsonOutputName, outputJson);
    }

    logger->info("===============================================================");
    logger->info("stared new simulation of data transfer with LEB128");

    LEB128 leb128{logger};

    while (true) {
        promise<string> promise;
        future<string> future{promise.get_future()};

        thread t1{[&]{
            string binary;
            int value;

            if(values.size()) {
                value = getNextElement(values);
            } else {
                value = dis(gen);
            }

            cout << "value to transfer: " << value << endl;

            if (transferUnsigned) {
                binary = leb128.toUnsignedLeb128(value);
            } else {
                binary = leb128.toSignedLeb128(value);
            }

            if(jsonOutputName != "") {
                outputJson["data"].push_back({{"transfered", value}});
                writeJsonFile(jsonOutputName, outputJson);
            }

            if (showEncoded) {
                cout << "encoded value: " << binary << endl;
            }

            promise.set_value(binary);
            this_thread::sleep_for(chrono::milliseconds(delay));
        }};

        thread t2{[&]{
            string binary = future.get();
            int value;

            if (transferUnsigned) {
                value = leb128.unsignedLeb128toDecimal(binary);
            } else {
                value = leb128.signedLeb128toDecimal(binary);
            }

            if(jsonOutputName != "") {
                outputJson["data"].back().push_back({"received", value});
                writeJsonFile(jsonOutputName, outputJson);
            }
            
            cout << "received value: " << value << endl;
        }};

        t1.join();
        t2.join();
    }
}
