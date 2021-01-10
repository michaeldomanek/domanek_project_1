#pragma once

#include <bitset>
#include <string>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

using namespace std;

class LEB128 {
    private:
        shared_ptr<spdlog::logger> logger;

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
            string leb128binary{"0" + binary.substr(0, 7)};
            for (size_t i = 7; i < binary.length(); i += 7) {
                leb128binary = "1" + binary.substr(i, 7) + leb128binary;
            }
            logger->debug("Binary with translated positions: " + leb128binary);
            
            return leb128binary;
        }

    public: 
        LEB128(shared_ptr<spdlog::logger> logger) {
            this->logger = logger;
        }

        string toSignedLeb128(const int &number) {
            logger->debug("Convert to signed LEB128");
            logger->debug("Number: " + to_string(number));

            if (!number) {
                logger->debug("Number is zero => return: " + to_string(number));
                return "00000000";
            }

            string binary{bitset<17>(abs(number)).to_string()};
            binary.erase(0, binary.find_first_not_of('0'));
            binary = "0" + binary;
            logger->debug("Binary number: " + binary);
            
            if (number < 0) {
                binary = getTwoscomplement(binary);
                logger->debug("Two's complement: " + binary);
            }

            fillWithSign(binary, number >= 0 ? '0' : '1');
            logger->debug("Binary with sign: " + binary);

            return translatePosition(binary);   
        }

        string toUnsignedLeb128(const int &number) {
            logger->debug("Convert to unsigned LEB128");
            logger->debug("Number: " + to_string(number));

            if (!number) {
                logger->debug("Number is zero => return: " + to_string(number));
                return "00000000";
            }

            string binary{bitset<17>(number).to_string()};
            binary.erase(0, binary.find_first_not_of('0'));
            logger->debug("Binary number: " + binary);

            fillWithSign(binary, '0');
            logger->debug("Binary with sign: " + binary);

            return translatePosition(binary);   
        }

        int signedLeb128toDecimal(string value) {
            string binary{""};
            bool isNegative{false};

            logger->debug("Convert signed LEB128 to decimal");
            logger->debug("LEB128 encoded binary: " + value);
            
            while (true) {
                bool isLastByte{value.front() == '0'};

                binary = value.substr(1, 7) + binary;
                value.erase(0, 8);

                if (isLastByte) {
                    break;
                }
            }

            logger->debug("Binary with translated positions: " + binary);

            if (binary.front() == '1') {
                binary = getTwoscomplement(binary);
                logger->debug("Two's complement: " + binary);
                isNegative = true;
            }

            binary.erase(0, binary.find_first_not_of('0'));

            logger->debug("Binary number: " + binary);

            int decimal = (int)bitset<17>(binary).to_ulong();

            if (isNegative) {
                decimal = -decimal;
            }

            logger->debug("Number: " + to_string(decimal));
            
            return decimal;
        }

        int unsignedLeb128toDecimal(string value) {
            string binary{""};

            logger->debug("Convert unsigned LEB128 to decimal");
            logger->debug("LEB128 encoded binary: " + value);
            
            while (true) {
                bool isLastByte{value.front() == '0'};

                binary = value.substr(1, 7) + binary;
                value.erase(0, 8);

                if (isLastByte) {
                    break;
                }
            }

            logger->debug("Binary with translated positions: " + binary);

            binary.erase(0, binary.find_first_not_of('0'));

            logger->debug("Binary number: " + binary);

            int decimal = (int)bitset<17>(binary).to_ulong();

            logger->debug("Number: " + to_string(decimal));
            
            return decimal;
        }
};