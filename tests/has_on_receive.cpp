#include "catch.hpp"

#include "pipes/helpers/has_onReceive.hpp"

#include "pipes/impl/concepts.hpp"

#include "pipes/join.hpp"
#include "pipes/filter.hpp"
#include "pipes/drop.hpp"
#include "pipes/dev_null.hpp"
#include "pipes/transform.hpp"

struct dummy_without_onReceive{};

struct dummy_with_wrong_onReceive
{
    template <typename T>
    auto onReceive(T&&) { return 5;}
};

struct dummy_with_templated_onReceive
{
    template <typename T, typename U, pipes::detail::IsAPipeline<U> = 0>
    auto onReceive(T&&, U&&) { return 5;}
};

struct dummy_with_variadic_templated_onReceive
{
    template <typename ...T, typename U>
    auto onReceive(T&&... t, U&& u) { return 5;}
};


struct dummy_predicate
{
    template <typename T>
    bool operator()(const T&&) { return true; }
};

struct dummy_transform_function
{
    template <typename T>
    auto operator()(const T&& t) { return t; }
};

TEST_CASE("dummy does not have onReceive")
{
    using sfinae_result = typename pipes::detail::has_onReceive_method<dummy_without_onReceive>;
    REQUIRE((std::is_base_of<std::true_type, sfinae_result>::value == false));
}

TEST_CASE("dummy wrong onReceive")
{
    using sfinae_result = typename pipes::detail::has_onReceive_method<dummy_with_wrong_onReceive>;
    REQUIRE((std::is_base_of<std::true_type, sfinae_result>::value == false));
}

TEST_CASE("dummy has onReceive")
{
	using sfinae_result = typename pipes::detail::has_onReceive_method<dummy_with_templated_onReceive>;
    REQUIRE((std::is_base_of<std::true_type, sfinae_result>::value));
}

TEST_CASE("variadic has onReceive")
{
	using sfinae_result = typename pipes::detail::has_onReceive_method<dummy_with_variadic_templated_onReceive>;
    REQUIRE((std::is_base_of<std::true_type, sfinae_result>::value));
}

TEST_CASE("join has onReceive")
{
	using sfinae_result = typename pipes::detail::has_onReceive_method<pipes::join_pipe>;
    REQUIRE((std::is_base_of<std::true_type, sfinae_result>::value));
}

TEST_CASE("filter has onReceive")
{
	using sfinae_result = typename pipes::detail::has_onReceive_method<pipes::filter_pipe<dummy_predicate>>;
    REQUIRE((std::is_base_of<std::true_type, sfinae_result>::value));
}

TEST_CASE("drop has onReceive")
{
	using sfinae_result = typename pipes::detail::has_onReceive_method<pipes::drop>;
    REQUIRE((std::is_base_of<std::true_type, sfinae_result>::value));
}

TEST_CASE("transform has onReceive")
{
	using sfinae_result = typename pipes::detail::has_onReceive_method<pipes::transform_pipe<dummy_transform_function>>;
    REQUIRE((std::is_base_of<std::true_type, sfinae_result>::value));
}

TEST_CASE("dev_null wrong onReceive")
{
	using sfinae_result = typename pipes::detail::has_onReceive_method<pipes::dev_null>;
    REQUIRE((std::is_base_of<std::true_type, sfinae_result>::value == false));
}
