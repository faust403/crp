# include <gtest/gtest.h>
# include <crp/sha256.hpp>

TEST(SHA256, BLOCKSIZE) {
    const char* str = "lkajsgndfldjkngsldfjng;kjndsfpognjfdpjnag;ksdfnj;gjnSD[OAMF[GAOFMB[OIFMD[OGFJNKDS;KFMSNA;KJNLJKNpodfnopsdnjf]]]]";
    std::cout << sha256(str, std::strlen(str)) << std::endl;
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}