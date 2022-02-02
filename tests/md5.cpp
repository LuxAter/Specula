#include <string>
#include <string_view>

#include <catch2/catch_test_macros.hpp>

#include <specula/md5.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;

TEST_CASE("Static MD5 Hashing Algorithm", "[util]") {
  SECTION("RF1321") {
    CHECK(MD5("").to_string() == "d41d8cd98f00b204e9800998ecf8427e");
    CHECK(MD5("a").to_string() == "0cc175b9c0f1b6a831c399e269772661");
    CHECK(MD5("abc").to_string() == "900150983cd24fb0d6963f7d28e17f72");
    CHECK(MD5("message digest").to_string() ==
          "f96b697d7cb7938d525a2f31aaf161d0");
    CHECK(MD5("abcdefghijklmnopqrstuvwxyz").to_string() ==
          "c3fcd3d76192e4007dfb496cca67e13b");
    CHECK(MD5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
              .to_string() == "d174ab98d277d9f5a5611c2c9f419d9f");
    CHECK(MD5("12345678901234567890123456789012345678901234567890123456789012"
              "345678901234567890")
              .to_string() == "57edf4a22be3c955ac49da2e2107b67a");
  }

  SECTION("std::string") {
    CHECK(MD5(""s).to_string() == "d41d8cd98f00b204e9800998ecf8427e");
    CHECK(MD5("a"s).to_string() == "0cc175b9c0f1b6a831c399e269772661");
    CHECK(MD5("abc"s).to_string() == "900150983cd24fb0d6963f7d28e17f72");
    CHECK(MD5("message digest"s).to_string() ==
          "f96b697d7cb7938d525a2f31aaf161d0");
    CHECK(MD5("abcdefghijklmnopqrstuvwxyz"s).to_string() ==
          "c3fcd3d76192e4007dfb496cca67e13b");
    CHECK(MD5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"s)
              .to_string() == "d174ab98d277d9f5a5611c2c9f419d9f");
    CHECK(MD5("12345678901234567890123456789012345678901234567890123456789012"
              "345678901234567890"s)
              .to_string() == "57edf4a22be3c955ac49da2e2107b67a");
  }
  SECTION("std::string_view") {
    CHECK(MD5(""sv).to_string() == "d41d8cd98f00b204e9800998ecf8427e");
    CHECK(MD5("a"sv).to_string() == "0cc175b9c0f1b6a831c399e269772661");
    CHECK(MD5("abc"sv).to_string() == "900150983cd24fb0d6963f7d28e17f72");
    CHECK(MD5("message digest"sv).to_string() ==
          "f96b697d7cb7938d525a2f31aaf161d0");
    CHECK(MD5("abcdefghijklmnopqrstuvwxyz"sv).to_string() ==
          "c3fcd3d76192e4007dfb496cca67e13b");
    CHECK(
        MD5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"sv)
            .to_string() == "d174ab98d277d9f5a5611c2c9f419d9f");
    CHECK(MD5("12345678901234567890123456789012345678901234567890123456789012"
              "345678901234567890"sv)
              .to_string() == "57edf4a22be3c955ac49da2e2107b67a");
  }

  SECTION("std::vector") {
    CHECK(MD5(std::vector<uint8_t>{}).to_string() ==
          "d41d8cd98f00b204e9800998ecf8427e");
    CHECK(MD5(std::vector<uint8_t>{'a'}).to_string() ==
          "0cc175b9c0f1b6a831c399e269772661");

    CHECK(MD5(std::vector<int>{}).to_string() ==
          "d41d8cd98f00b204e9800998ecf8427e");
    CHECK(MD5(std::vector<int>{'a'}).to_string() ==
          "332ce785e973574a1c5fdaf3eee3f083");
    CHECK(MD5(std::vector<int>{1, -2, 3}).to_string() ==
          "d5ca49daaedc80394cc53e1b3d271c72");
  }

  SECTION("std::pair") {
    CHECK(MD5(std::pair<int, int>{1, 2}).to_string() ==
          "4f04e2bb1318b81190e10694e3e82c30");
    CHECK(MD5(std::pair<int, int>{4, 5}).to_string() ==
          "678157bbe4fd35371e047b4cadf9c46a");
  }
}

TEST_CASE("Updating MD5 Hashing Algorithm", "[util]") {
  SECTION("Multiple update calls") {
    MD5 md5;

    md5.update("abc");
    md5.update("def");
    md5.update("ghi");

    md5.final();
    CHECK(md5.to_string() == "8aa99b1f439ff71293e95357bac6fd94");
  }
  SECTION("Already finalized MD5 throws exception") {
    MD5 md5;
    md5.final();
    CHECK(md5.to_string() == "d41d8cd98f00b204e9800998ecf8427e");

    CHECK_THROWS_AS(md5.update("jkl"), std::runtime_error);
    CHECK_THROWS_AS(md5.final(), std::runtime_error);
  }
  SECTION("Reset after finalized MD5") {
    MD5 md5;
    md5.final();
    CHECK(md5.to_string() == "d41d8cd98f00b204e9800998ecf8427e");

    md5.reset();
    md5.init();

    md5.update('a');
    md5.update('b');
    md5.update('c');

    md5.final();
    CHECK(md5.to_string() == "900150983cd24fb0d6963f7d28e17f72");
  }
  SECTION("Multiple update calls with different types") {
    MD5 md5;

    md5.update("abc");
    md5.update(32);
    md5.update(3.14);

    md5.final();
    CHECK(md5.to_string() == "d6d9ffb567a29bd6b4af34b3f9cf605b");
  }
}