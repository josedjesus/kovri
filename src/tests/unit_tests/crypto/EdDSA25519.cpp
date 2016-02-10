/**
 * Copyright (c) 2015-2016, The Kovri I2P Router Project
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include "crypto/EdDSA25519.h"

BOOST_AUTO_TEST_SUITE(EdDSA25519Tests)

struct EDDSAFixture {
  EDDSAFixture()
    : verifier(public_key),
      signer(private_key) {}

  uint8_t private_key[32] = {
    0xe1, 0xec, 0xff, 0xa6, 0xcd, 0x4e, 0xc7, 0x09, 0x2f, 0x87,
    0x44, 0xaf, 0x48, 0xb3, 0x7f, 0x63, 0x71, 0x63, 0x1e, 0x01,
    0xf7, 0x20, 0xe9, 0x0a, 0xfa, 0x3c, 0x90, 0xec, 0x97, 0x4c,
    0x16, 0x27
  };

  uint8_t public_key[32] = {
    0x0f, 0x90, 0x8b, 0xaf, 0xef, 0x40, 0x79, 0xb5, 0x94, 0xb5,
    0x13, 0xf9, 0xf6, 0x02, 0x65, 0xef, 0x4d, 0x95, 0xa4, 0x84,
    0x2d, 0xc7, 0x23, 0x1b, 0x93, 0xe4, 0x2e, 0x9d, 0x45, 0x52,
    0xed, 0x62
  };

  i2p::crypto::EDDSA25519Verifier verifier;
  i2p::crypto::EDDSA25519Signer signer;
};

BOOST_FIXTURE_TEST_CASE(EdDSA25519KeyLength, EDDSAFixture) {
  BOOST_CHECK_EQUAL(
      verifier.GetPublicKeyLen(),
      i2p::crypto::EDDSA25519_PUBLIC_KEY_LENGTH);
}

BOOST_FIXTURE_TEST_CASE(EdDSA25519SignatureLength, EDDSAFixture) {
  BOOST_CHECK_EQUAL(
      verifier.GetSignatureLen(),
      i2p::crypto::EDDSA25519_SIGNATURE_LENGTH);
}

BOOST_FIXTURE_TEST_CASE(EdDSA25519Sign, EDDSAFixture) {
  const uint8_t message[33] = {
    0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20,
    0x74, 0x65, 0x73, 0x74, 0x20, 0x6d, 0x65, 0x73, 0x73, 0x61,
    0x67, 0x65, 0x21, 0x20, 0x2d, 0x45, 0x69, 0x6e, 0x4d, 0x42,
    0x79, 0x74, 0x65
  };
  const uint8_t signature[64] = {
    0xfa, 0x69, 0x31, 0x22, 0x61, 0xb5, 0x4f, 0xf5, 0x7e, 0x20,
    0xa6, 0x05, 0x91, 0xe7, 0xab, 0x41, 0x43, 0x48, 0x85, 0x02,
    0xac, 0xcf, 0x3f, 0x17, 0x13, 0x09, 0x3e, 0x25, 0x3c, 0x15,
    0x6d, 0xc3, 0x55, 0xcd, 0x8a, 0x30, 0x07, 0xe2, 0x41, 0xa1,
    0x98, 0x24, 0xe5, 0xc0, 0x9e, 0x90, 0xbb, 0x9e, 0x6b, 0xe8,
    0x41, 0x84, 0x2d, 0x13, 0x2e, 0x1f, 0x2a, 0x46, 0x31, 0x2e,
    0x5c, 0x94, 0x7e, 0x0d
  };
  uint8_t output[64] = {};
  signer.Sign(message, 33, output);
  BOOST_CHECK_EQUAL_COLLECTIONS(output, output + 64, signature, signature + 64);
}

BOOST_FIXTURE_TEST_CASE(EdDSA25519Verify, EDDSAFixture) {
  const uint8_t message[33] = {
    0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20,
    0x74, 0x65, 0x73, 0x74, 0x20, 0x6d, 0x65, 0x73, 0x73, 0x61,
    0x67, 0x65, 0x21, 0x20, 0x2d, 0x45, 0x69, 0x6e, 0x4d, 0x42,
    0x79, 0x74, 0x65
  };
  const uint8_t signature[64] = {
    0xfa, 0x69, 0x31, 0x22, 0x61, 0xb5, 0x4f, 0xf5, 0x7e, 0x20,
    0xa6, 0x05, 0x91, 0xe7, 0xab, 0x41, 0x43, 0x48, 0x85, 0x02,
    0xac, 0xcf, 0x3f, 0x17, 0x13, 0x09, 0x3e, 0x25, 0x3c, 0x15,
    0x6d, 0xc3, 0x55, 0xcd, 0x8a, 0x30, 0x07, 0xe2, 0x41, 0xa1,
    0x98, 0x24, 0xe5, 0xc0, 0x9e, 0x90, 0xbb, 0x9e, 0x6b, 0xe8,
    0x41, 0x84, 0x2d, 0x13, 0x2e, 0x1f, 0x2a, 0x46, 0x31, 0x2e,
    0x5c, 0x94, 0x7e, 0x0d
  };
  BOOST_CHECK(verifier.Verify(message, 33, signature));
}

BOOST_FIXTURE_TEST_CASE(EdDSA25519VerifyBadMsg, EDDSAFixture) {
  const uint8_t message[10] = {};
  const uint8_t signature[64] = {
    0xfa, 0x69, 0x31, 0x22, 0x61, 0xb5, 0x4f, 0xf5, 0x7e, 0x20,
    0xa6, 0x05, 0x91, 0xe7, 0xab, 0x41, 0x43, 0x48, 0x85, 0x02,
    0xac, 0xcf, 0x3f, 0x17, 0x13, 0x09, 0x3e, 0x25, 0x3c, 0x15,
    0x6d, 0xc3, 0x55, 0xcd, 0x8a, 0x30, 0x07, 0xe2, 0x41, 0xa1,
    0x98, 0x24, 0xe5, 0xc0, 0x9e, 0x90, 0xbb, 0x9e, 0x6b, 0xe8,
    0x41, 0x84, 0x2d, 0x13, 0x2e, 0x1f, 0x2a, 0x46, 0x31, 0x2e,
    0x5c, 0x94, 0x7e, 0x0d
  };
  BOOST_CHECK(!verifier.Verify(message, 10, signature));
}

BOOST_FIXTURE_TEST_CASE(EdDSA25519VerifyBadSignature, EDDSAFixture) {
  const uint8_t message[33] = {
    0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20,
    0x74, 0x65, 0x73, 0x74, 0x20, 0x6d, 0x65, 0x73, 0x73, 0x61,
    0x67, 0x65, 0x21, 0x20, 0x2d, 0x45, 0x69, 0x6e, 0x4d, 0x42,
    0x79, 0x74, 0x65
  };
  const uint8_t signature[64] = {};
  BOOST_CHECK(!verifier.Verify(message, 33, signature));
}

BOOST_AUTO_TEST_SUITE_END()
