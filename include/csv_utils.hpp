#pragma once
#include <iostream>
#include <fstream>
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

void write_data(const char* filename, std::vector<double> &data) {
    std::ofstream f;
    f.open(filename);

    f << "x,y" << std::endl;
    for (size_t i = 0; i < data.size(); i++) {
        f << i << "," << data[i] << std::endl;
    }
    f.close();
}
