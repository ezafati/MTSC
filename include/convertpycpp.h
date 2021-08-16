#ifndef MATELEM_HPP
#define MATELEM_HPP

#include "Python.h"
#include <boost/python.hpp>



template<typename T>
struct vector_to_list
{
    static PyObject* convert(const std::vector<T>& vector)
    {
        boost::python::list result;
        //typename std::vector<T>::iterator iter;
        for (auto iter = vector.begin(); iter != vector.end(); ++iter) {
	    	result.append(*iter);
	    }
        return boost::python::incref(result.ptr());
    }
};


#endif