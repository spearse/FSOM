#include "unit_testing.h"

#include "fsom/Table.hpp"

enum TestTableConstants
{
	kTableTestSize = 256
};

typedef fsom::Table<float> test_table_t;

TEST_CASE("Table: can construct", "[table]")
{
	test_table_t table(kTableTestSize);
	REQUIRE(true);
}

TEST_CASE("Table: check size returns the expected value", "[table]")
{
	test_table_t table(kTableTestSize);
	REQUIRE((table.get_size() + test_table_t::kGuardValues) == kTableTestSize);
}

TEST_CASE("Table: can construct and fill", "[table]")
{
	test_table_t table(kTableTestSize);
	table.fill_zero();
	table.fill_clipped(0.5f);
	table.fill_cos();
	table.fill_hann();
	table.fill_linear();
	table.fill_linear_offset();
	table.fill_noise();
	table.fill_sine();
	table.fill_square();
	table.fill_triangle();
}

TEST_CASE("Table: table element access (at)", "[table]")
{
	test_table_t table(kTableTestSize);

	for (test_table_t::size_type n = 0; n < table.get_size(); ++n)
	{
		table.at(n) = static_cast<float>(n);
	}

	for (test_table_t::size_type n = 0; n < table.get_size(); ++n)
	{
		REQUIRE(EQ_TOL(table.at(n), static_cast<float>(n)));
	}
}

TEST_CASE("Table: table element access (array style [])", "[table]")
{
	test_table_t table(kTableTestSize);

	for (test_table_t::size_type n = 0; n < table.get_size(); ++n)
	{
		table[n] = static_cast<float>(n);
	}

	for (test_table_t::size_type n = 0; n < table.get_size(); ++n)
	{
		REQUIRE(EQ_TOL(table[n], static_cast<float>(n)));
	}
}

TEST_CASE("Table: table element access (ranged for)", "[table]")
{
	test_table_t table(kTableTestSize);

	int n = 0;
	for (auto& it : table)
	{
		it = static_cast<float>(n);
		++n;
	}

	n = 0;
	for (const auto& it : table)
	{
		REQUIRE(EQ_TOL(it, static_cast<float>(n)));
		++n;
	}
}

TEST_CASE("Table: linear lookup", "[table]")
{
	test_table_t table(kTableTestSize);
	table.fill_linear();

	const int kMax = kTableTestSize - test_table_t::kGuardValues;
	for (int n = 0; n < kMax; ++n)
	{
		float f = (float)n / (float)kMax;
		REQUIRE(EQ_TOL(table.linear_lookup((float)n), f));
	}
}