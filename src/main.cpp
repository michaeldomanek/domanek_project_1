#include "CLI11.hpp"

#include <iostream>
#include <thread>
#include <vector>
#include <exception>
#include <string>
#include <future>
#include <chrono>
#include <queue>
#include <bitset>

using namespace std;

string getTwoscomplement(string str) {
    int i; 
    for (i = str.length() - 1 ; i >= 0 ; i--) {
        if (str[i] == '1')
            break; 
    }

    if (i == -1) 
        return '1' + str; 
  
    for (int j = i-1 ; j >= 0; j--) {  
        if (str[j] == '1') 
            str[j] = '0'; 
        else
            str[j] = '1'; 
    } 

    return str;
} 

void fillWithSign(string &binary, const char &sign) {
    if (binary.length() % 7 != 0) {
        binary = string(7 - (binary.length() % 7), sign).append(binary);
    }
}

string translatePosition(string binary) {
    string leb128binary{" 0" + binary.substr(0, 7)};
    for (size_t i = 7; i < binary.length(); i += 7) {
        leb128binary = " 1" + binary.substr(i, 7) + leb128binary;
    }
    
    return leb128binary;
}

string toSignedLeb128(const int &number) {
    if (!number) {
        return "0";
    }

    string binary{bitset<17>(abs(number)).to_string()};
    binary.erase(0, binary.find_first_not_of('0'));
    binary = "0" + binary;
    
    if(number < 0) {
        binary = getTwoscomplement(binary);
    }

    fillWithSign(binary, number >= 0 ? '0' : '1');

    return translatePosition(binary);
    
}

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

    cout << toSignedLeb128(start) << endl;
    
    // while(true) {
    //     promise<string> promise;
    //     future<string> future{promise.get_future()};

    //     thread t1{[&promise]{
    //         promise.set_value("1101000101111000");
    //         this_thread::sleep_for(chrono::seconds(1));
    //     }};

    //     this_thread::sleep_for(chrono::seconds(1));

    //     thread t2{[&future]{
    //         cout << future.get() << endl;
    //     }};

    //     t1.join();
    //     t2.join();
    // }

    // random_device rd;
    // mt19937 gen{rd()};
    // uniform_int_distribution<> dis{0, 100};

    
    
}
