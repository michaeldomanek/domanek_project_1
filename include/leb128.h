#pragma once

#include <bitset>
#include <string>

using namespace std;

namespace LEB128 {
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
        
        return leb128binary;
    }

    string toSignedLeb128(const int &number) {
        if (!number) {
            return "0";
        }

        string binary{bitset<17>(abs(number)).to_string()};
        binary.erase(0, binary.find_first_not_of('0'));
        binary = "0" + binary;

        cout << binary << endl;
        
        if (number < 0) {
            binary = getTwoscomplement(binary);
        }

        fillWithSign(binary, number >= 0 ? '0' : '1');

        return translatePosition(binary);   
    }

    int signedLeb128toDecimal(string value) {
        string binary{""};
        bool isNegative{false};
        
        while (true) {
            bool isLastByte{value.front() == '0'};

            binary = value.substr(1, 7) + binary;
            value.erase(0, 8);

            if (isLastByte) {
                break;
            }
        }

        if (binary.front() == '1') {
            binary = getTwoscomplement(binary);
            isNegative = true;
        }

        binary.erase(0, binary.find_first_not_of('0'));

        int decimal = (int)bitset<17>(binary).to_ulong();
        
        if (isNegative) {
            decimal = -decimal;
        }
        
        return decimal;
    }
}