#include "stdafx.h"
#include "pplx/pplxtasks.h"
#include "cpprest/details/basic_types.h"
#include "cpprest/asyncrt_utils.h"

pplx::task<void> tce_throw()
{
    pplx::task_completion_event<void> tce;

    pplx::task_from_result()
        .then([]
        {
            return pplx::task_from_exception<void>(std::runtime_error("tce - runtime error"));
        })
        .then([tce](pplx::task<void> previous_task)
        {
            try
            {
                previous_task.get();
            }
            catch (const std::exception&)
            {
                tce.set_exception(std::current_exception());
            }
        });

    return pplx::create_task(tce);
}

int main()
{
    try
    {
        tce_throw().wait();
    }
    catch (const std::exception& e)
    {
        ucout << utility::conversions::to_string_t(e.what()) << std::endl;
    }
}
