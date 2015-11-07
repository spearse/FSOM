#include "unit_testing.h"

#include "fsom/utilities.hpp"

TEST_CASE("MultiChannelBuffer default construct different channels", "[MultiChannelBuffer]")
{
	fsom::MultiChannelBuffer a(1, 4096);
	fsom::MultiChannelBuffer b(2, 4096);
	fsom::MultiChannelBuffer c(8, 4096);
}
TEST_CASE("MultiChannelBuffer default construct different buffer sizes", "[MultiChannelBuffer]")
{
	for (int n = 0; n < 8; ++n)
	{
		fsom::MultiChannelBuffer::size_type size = 1 << n;
		fsom::MultiChannelBuffer mb(2, size);
		REQUIRE(mb.size() == size);
	}
}

TEST_CASE("MultiChannelBuffer write into each channel", "[MultiChannelBuffer]")
{
	const fsom::channel_index_t kChannels = 8;
	const fsom::MultiChannelBuffer::size_type kSize = 4096;
	fsom::MultiChannelBuffer mb(kChannels, kSize);

	float** pBuffers = mb.get_buffers();
	for (fsom::channel_index_t n = 0; n < kChannels; ++n)
	{
		std::fill(pBuffers[n], pBuffers[n] + kSize, static_cast<float>(n));
	}

	for (fsom::channel_index_t n = 0; n < kChannels; ++n)
	{
		REQUIRE(pBuffers[n][0] == static_cast<float>(n));
		REQUIRE(pBuffers[n][kSize-1] == static_cast<float>(n));
	}
}