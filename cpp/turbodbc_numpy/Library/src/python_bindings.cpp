#include <turbodbc_numpy/numpy_result_set.h>

#include <pybind11/pybind11.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
// compare http://docs.scipy.org/doc/numpy/reference/c-api.array.html#importing-the-api
// as to why this define is necessary
#define PY_ARRAY_UNIQUE_SYMBOL turbodbc_numpy_API
#include <numpy/ndarrayobject.h>

using turbodbc_numpy::numpy_result_set;

namespace {

numpy_result_set make_numpy_result_set(std::shared_ptr<turbodbc::result_sets::result_set> result_set_pointer)
{
	return numpy_result_set(*result_set_pointer);
}

}

// this function is required to work around issues and compiler warnings with
// the import_array() macro on systems with Python 2/3
#if PY_VERSION_HEX >= 0x03000000
    void * enable_numpy_support()
    {
        import_array();
        return nullptr;
    }
#else
    void enable_numpy_support()
    {
        import_array();
    }
#endif


PYBIND11_PLUGIN(turbodbc_numpy_support) {
    enable_numpy_support();
    pybind11::module module("turbodbc_numpy_support", "Native helpers for turbodbc's NumPy support");

    pybind11::class_<numpy_result_set>(module, "NumpyResultSet")
        .def("fetch_next_batch", &numpy_result_set::fetch_next_batch);

    module.def("make_numpy_result_set", make_numpy_result_set);
    return module.ptr();
}
