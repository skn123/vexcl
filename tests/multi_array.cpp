#define BOOST_TEST_MODULE MultiArray
#include <boost/test/unit_test.hpp>
#include <vexcl/element_index.hpp>
#include <vexcl/multi_array.hpp>
#include "context_setup.hpp"

BOOST_AUTO_TEST_CASE(create)
{
    using vex::extents;
    using vex::indices;
    using vex::range;

    std::vector<cl::CommandQueue> queue(1, ctx.queue(0));

    vex::multi_array<double, 2> x(queue, extents[1024][1024]);

    BOOST_CHECK(x.size<0>() == 1024);
    BOOST_CHECK(x.size<1>() == 1024);

    auto view = x(indices[5][range(0,100)]);

    BOOST_CHECK(view.size<0>() == 100);
}

BOOST_AUTO_TEST_CASE(arithmetics)
{
    using vex::extents;

    std::vector<cl::CommandQueue> queue(1, ctx.queue(0));

    vex::multi_array<double, 3> x(queue, extents[32][32][32]);
    vex::multi_array<double, 3> y(queue, extents[32][32][32]);

    x.vec() = 2 * M_PI * vex::element_index();
    y.vec() = pow(sin(x.vec()), 2.0) + pow(cos(x.vec()), 2.0);

    check_sample(y.vec(), [](size_t, double v) {
            BOOST_CHECK_CLOSE(v, 1, 1e-8);
            });
}

BOOST_AUTO_TEST_CASE(slicing)
{
    using vex::extents;
    using vex::indices;
    using vex::range;
    using vex::_;

    std::vector<cl::CommandQueue> queue(1, ctx.queue(0));

    vex::multi_array<double, 3> x(queue, extents[32][32][32]);
    vex::multi_array<double, 3> y(queue, extents[32][32][32]);

    for(size_t i = 0; i < x.size<0>(); ++i)
        x(indices[i][_][_]).vec() = i;

    for(size_t i = 0; i < x.size<0>(); ++i)
        y(indices[_][_][i]).vec() = x(indices[i][_][_]).vec();

    check_sample(y.vec(), [](size_t idx, double v) {
            BOOST_CHECK_EQUAL(v, idx % 32);
            });
}

BOOST_AUTO_TEST_SUITE_END()
