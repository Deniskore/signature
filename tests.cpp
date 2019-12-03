#include <gtest/gtest.h>

#include <climits>
#include <iostream>

#include "hashcalc.h"
#include "utils.h"

std::string get_str(const std::string& file_name) {
  std::ifstream t(file_name);
  std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
  return str;
}

std::string repeat(const std::string& input, size_t num) {
  std::ostringstream os;
  std::fill_n(std::ostream_iterator<std::string>(os), num, input);
  return os.str();
}

TEST(HashCalc, empty) {
  try {
    CHashCalc calc("test_files//empty.bin", "out2.result", 1024);
    calc.run();
  } catch (const std::runtime_error&) {
    std::cout << "Found runtime_error, that's good!" << std::endl;
    return;
  }
  FAIL() << "Expected runtime_error";
}

TEST(HashCalc, 8ByteMaxValue) {
  try {
    CHashCalc calc("test_files//empty.bin", "out3.result", ULLONG_MAX);
    calc.run();
  } catch (const std::invalid_argument&) {
    std::cout << "Found invalid_argument, that's good!" << std::endl;
    return;
  }
  FAIL() << "Expected invalid argument";
}

TEST(HashCalc, 3chars) {
  CHashCalc calc("test_files//3chars.txt", "out4.result", 3);
  calc.run();
  const auto str = get_str("out4.result");
  EXPECT_STREQ(str.data(),
            "4933436445555b9dcfe7dd5a7f9d3af7ccc8085b2253e9c906cc7a74ccda7dc8\n"
            "7eda97062ae5483e38b077be91902df4fcff3243a57008ac64397d1805a15ad5\n"
            "69caabe9e205c19fe2d304e753931f2e44ef9c587e7f9609761139112b9a8770\n"
            "c625a631703a780292f1f91a8439c79795e567e9c14330df735f70bc91fa894d\n"
            "3efdf25a945224519018d183d7016ab44ec98f806e5aa1270cfffeb78705277b\n");
}

TEST(HashCalc, Numbers) {
  CHashCalc calc("test_files//numbers.txt", "out5.result", 1);
  calc.run();
  const auto str = get_str("out5.result");
  EXPECT_STREQ(str.data(),
            "5feceb66ffc86f38d952786c6d696c79c2dbc239dd4e91b46729d73a27fb57e9\n"
            "6b86b273ff34fce19d6b804eff5a3f5747ada4eaa22f1d49c01e52ddb7875b4b\n"
            "d4735e3a265e16eee03f59718b9b5d03019c07d8b6c51f90da3a666eec13ab35\n"
            "4e07408562bedb8b60ce05c1decfe3ad16b72230967de01f640b7e4729b49fce\n"
            "4b227777d4dd1fc61c6f884f48641d02b4d121d3fd328cb08b5531fcacdabf8a\n"
            "ef2d127de37b942baad06145e54b0c619a1f22327b2ebbcfbec78f5564afe39d\n"
            "e7f6c011776e8db7cd330b54174fd76f7d0216b612387a5ffcfb81e6f0919683\n"
            "7902699be42c8a8e46fbbb4501726517e86b22c56a189f7625a6da49081b2451\n"
            "2c624232cdd221771294dfbb310aca000a0df6ac8b66b696d90ef06fdefb64a3\n"
            "19581e27de7ced00ff1ce50b2047e7a567c76b1cbaebabe5ef03f7c3017bb5b7\n");
}

TEST(HashCalc, Alphabet) {
  CHashCalc calc("test_files//alphabet.txt", "out6.result", 1);
  calc.run();
  const auto str = get_str("out6.result");
  EXPECT_STREQ(str.data(),
            "ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb\n"
            "3e23e8160039594a33894f6564e1b1348bbd7a0088d42c4acb73eeaed59c009d\n"
            "2e7d2c03a9507ae265ecf5b5356885a53393a2029d241394997265a1a25aefc6\n"
            "18ac3e7343f016890c510e93f935261169d9e3f565436429830faf0934f4f8e4\n"
            "3f79bb7b435b05321651daefd374cdc681dc06faa65e374e38337b88ca046dea\n"
            "252f10c83610ebca1a059c0bae8255eba2f95be4d1d7bcfa89d7248a82d9f111\n"
            "cd0aa9856147b6c5b4ff2b7dfee5da20aa38253099ef1b4a64aced233c9afe29\n"
            "aaa9402664f1a41f40ebbc52c9993eb66aeb366602958fdfaa283b71e64db123\n"
            "de7d1b721a1e0632b7cf04edf5032c8ecffa9f9a08492152b926f1a5a7e765d7\n"
            "189f40034be7a199f1fa9891668ee3ab6049f82d38c68be70f596eab2e1857b7\n"
            "8254c329a92850f6d539dd376f4816ee2764517da5e0235514af433164480d7a\n"
            "acac86c0e609ca906f632b0e2dacccb2b77d22b0621f20ebece1a4835b93f6f0\n"
            "62c66a7a5dd70c3146618063c344e531e6d4b59e379808443ce962b3abd63c5a\n"
            "1b16b1df538ba12dc3f97edbb85caa7050d46c148134290feba80f8236c83db9\n"
            "65c74c15a686187bb6bbf9958f494fc6b80068034a659a9ad44991b08c58f2d2\n"
            "148de9c5a7a44d19e56cd9ae1a554bf67847afb0c58f6e12fa29ac7ddfca9940\n"
            "8e35c2cd3bf6641bdb0e2050b76932cbb2e6034a0ddacc1d9bea82a6ba57f7cf\n"
            "454349e422f05297191ead13e21d3db520e5abef52055e4964b82fb213f593a1\n"
            "043a718774c572bd8a25adbeb1bfcd5c0256ae11cecf9f9c3f925d0e52beaf89\n"
            "e3b98a4da31a127d4bde6e43033f66ba274cab0eb7eb1c70ec41402bf6273dd8\n"
            "0bfe935e70c321c7ca3afc75ce0d0ca2f98b5422e008bb31c00c6d7f1f1c0ad6\n"
            "4c94485e0c21ae6c41ce1dfe7b6bfaceea5ab68e40a2476f50208e526f506080\n"
            "50e721e49c013f00c62cf59f2163542a9d8df02464efeb615d31051b0fddc326\n"
            "2d711642b726b04401627ca9fbac32f5c8530fb1903cc4db02258717921a4881\n"
            "a1fce4363854ff888cff4b8e7875d600c2682390412a8cf79b37d0b11148b0fa\n"
            "594e519ae499312b29433b7dd8a97ff068defcba9755b6d5d00e84c524d67b06\n"
            "559aead08264d5795d3909718cdd05abd49572e84fe55590eef31a88a08fdffd\n"
            "df7e70e5021544f4834bbee64a9e3789febc4be81470df629cad6ddb03320a5c\n"
            "6b23c0d5f35d1b11f9b683f0b0a617355deb11277d91ae091d399c655b87940d\n"
            "3f39d5c348e5b79d06e842c114e6cc571583bbf44e4b0ebfda1a01ec05745d43\n"
            "a9f51566bd6705f7ea6ad54bb9deb449f795582d6529a0e22207b8981233ec58\n"
            "f67ab10ad4e4c53121b6a5fe4da9c10ddee905b978d3788d2723d7bfacbe28a9\n"
            "333e0a1e27815d0ceee55c473fe3dc93d56c63e3bee2b3b4aee8eed6d70191a3\n"
            "44bd7ae60f478fae1061e11a7739f4b94d1daf917982d33b6fc8a01a63f89c21\n"
            "a83dd0ccbffe39d071cc317ddf6e97f5c6b1c87af91919271f9fa140b0508c6c\n"
            "6da43b944e494e885e69af021f93c6d9331c78aa228084711429160a5bbd15b5\n"
            "86be9a55762d316a3026c2836d044f5fc76e34da10e1b45feee5f18be7edb177\n"
            "72dfcfb0c470ac255cde83fb8fe38de8a128188e03ea5ba5b2a93adbea1062fa\n"
            "08f271887ce94707da822d5263bae19d5519cb3614e0daedc4c7ce5dab7473f1\n"
            "8ce86a6ae65d3692e7305e2c58ac62eebd97d3d943e093f577da25c36988246b\n"
            "c4694f2e93d5c4e7d51f9c5deb75e6cc8be5e1114178c6a45b6fc2c566a0aa8c\n"
            "5c62e091b8c0565f1bafad0dad5934276143ae2ccef7a5381e8ada5b1a8d26d2\n"
            "4ae81572f06e1b88fd5ced7a1a000945432e83e1551e6f721ee9c00b8cc33260\n"
            "8c2574892063f995fdf756bce07f46c1a5193e54cd52837ed91e32008ccf41ac\n"
            "8de0b3c47f112c59745f717a626932264c422a7563954872e237b223af4ad643\n"
            "e632b7095b0bf32c260fa4c539e9fd7b852d0de454e9be26f24d0d6f91d069d3\n"
            "a25513c7e0f6eaa80a3337ee18081b9e2ed09e00af8531c8f7bb2542764027e7\n"
            "de5a6f78116eca62d7fc5ce159d23ae6b889b365a1739ad2cf36f925a140d0cc\n"
            "fcb5f40df9be6bae66c1d77a6c15968866a9e6cbd7314ca432b019d17392f6f4\n"
            "4b68ab3847feda7d6c62c1fbcbeebfa35eab7351ed5e78f4ddadea5df64b8015\n"
            "18f5384d58bcb1bba0bcd9e6a6781d1a6ac2cc280c330ecbab6cb7931b721552\n"
            "bbeebd879e1dff6918546dc0c179fdde505f2a21591c9a9c96e36b054ec5af83\n");
}

TEST(HashCalc, 1Byte) {
  CHashCalc calc("test_files//1byte.bin", "out7.result", 1024);
  calc.run();
  const auto str = get_str("out7.result");
  EXPECT_STREQ(str.data(), "9e076ceaf246b6003d9c2680a2b4cf0bffd069805902b0b5edeebf49039fe4bd\n");
}

TEST(HashCalc, 1Byte_read_with_128mb_block) {
  CHashCalc calc("test_files//1byte.bin", "out8.result", HashCalc::one_megabyte * 128);
  calc.run();
  const auto str = get_str("out8.result");
  EXPECT_STREQ(str.data(), "9e076ceaf246b6003d9c2680a2b4cf0bffd069805902b0b5edeebf49039fe4bd\n");
}

TEST(HashCalc, 256Asterisks) {
  CHashCalc calc("test_files//256a.txt", "out9.result", 256);
  calc.run();
  const auto str = get_str("out9.result");
  EXPECT_STREQ(str.data(), "c2927fc2fa9e7d021b12818b814cef6cba9d47c12aba078254f28748826f5964\n");
}

TEST(HashCalc, 256AsterisksOneByOne) {
  CHashCalc calc("test_files//256a.txt", "out10.result", 1);
  calc.run();
  const auto str = get_str("out10.result");
  const std::string str2 =
      repeat("684888c0ebb17f374298b65ee2807526c066094c701bcc7ebbe1c1095f494fc1\n", 256);
  EXPECT_STREQ(str.data(), str2.data());
}

TEST(HashCalc, 1024Asterisks) {
  CHashCalc calc("test_files//1024a.txt", "out11.result", 1024);
  calc.run();
  const auto str = get_str("out11.result");
  EXPECT_STREQ(str.data(), "b2256110f2c4226de0008dd4382a388e033f211b617bd3237135ab1d59a722b6\n");
}

TEST(HashCalc, 1mbOfZeroes) {
  CHashCalc calc("test_files//1mb_00.bin", "out12.result", 1024 * 1024);
  calc.run();
  const auto str = get_str("out12.result");
  EXPECT_STREQ(str.data(), "30e14955ebf1352266dc2ff8067e68104607e750abb9d3b36582b8af909fcb58\n");
}

TEST(HashCalc, 100mbOfZeroes) {
  CHashCalc calc("test_files//100mb_00.bin", "out13.result", 1024 * 1024 * 100);
  calc.run();
  const auto str = get_str("out13.result");
  EXPECT_STREQ(str.data(), "20492a4d0d84f8beb1767f6616229f85d44c2827b64bdbfb260ee12fa1109e0e\n");
}

TEST(HashCalc, 100mbOfSeparateBlocks5mb) {
  CHashCalc calc("test_files//100mb_00.bin", "out14.result", 1024 * 1024 * 5);
  calc.run();
  const auto str = get_str("out14.result");
  EXPECT_STREQ(str.data(),
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n"
            "c036cbb7553a909f8b8877d4461924307f27ecb66cff928eeeafd569c3887e29\n");
}

TEST(HashCalc, 100mbOfSeparateBlocks3mb) {
  CHashCalc calc("test_files//100mb_00.bin", "out1.result", 1024 * 1024 * 3);
  calc.run();
  const auto str = get_str("out1.result");
  EXPECT_STREQ(str.data(),
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "bbd05cf6097ac9b1f89ea29d2542c1b7b67ee46848393895f5a9e43fa1f621e5\n"
            "30e14955ebf1352266dc2ff8067e68104607e750abb9d3b36582b8af909fcb58\n");
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
