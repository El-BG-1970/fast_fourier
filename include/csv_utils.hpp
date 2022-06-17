#pragma once
#include <csv/csv.h>

std::vector<double> *read_data(const char* filename) {
    auto temps = new std::vector<double>;
    io::CSVReader<1> in(filename);
    in.read_header(io::ignore_extra_column, "temp");
    double temp;
    while(in.read_row(temp)){
        temps->push_back(temp);
    }
    return temps;
}
