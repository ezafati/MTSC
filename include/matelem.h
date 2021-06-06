#ifndef MATELEM_HPP
#define MATELEM_HPP

#include "mstC_types.h"
#include <string>
#include <iostream>


class MateElem{
public:
    MateElem(){}
    MateElem(std::string mtype, std::string elemtype):typemat(mtype){

    }
    ~MateElem() =default;

private:
    std::string typemat;
};
#endif
