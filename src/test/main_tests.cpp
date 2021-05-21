// Copyright (c) 2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php .

#include "chainparams.h"
#include "main.h"

#include "test/test_bitcoin.h"

#include <boost/signals2/signal.hpp>
#include <boost/test/unit_test.hpp>


BOOST_FIXTURE_TEST_SUITE(main_tests, TestingSetup)

const CAmount INITIAL_SUBSIDY = 12.5 * COIN;

BOOST_AUTO_TEST_CASE(block_subsidy_test)
{
    TestBlockSubsidyHalvings(Params(CBaseChainParams::MAIN).GetConsensus()); // As in main
    TestBlockSubsidyHalvings(20000, Consensus::HALVING_INTERVAL, Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT); // Pre-Blossom
    TestBlockSubsidyHalvings(50, 150, 80); // As in regtest
    TestBlockSubsidyHalvings(500, 1000, 900); // Just another interval
    TestBlockSubsidyHalvings(500, 1000, 3000); // Multiple halvings before Blossom activation
}

BOOST_AUTO_TEST_CASE(subsidy_limit_test)
{
    const Consensus::Params& consensusParams = Params(CBaseChainParams::MAIN).GetConsensus();

    CAmount nSum = 0;
    int nHeight = 0;
    // Mining slow start
    for (; nHeight < consensusParams.nSubsidySlowStartInterval; nHeight++) {
        CAmount nSubsidy = GetBlockSubsidy(nHeight, consensusParams);
        BOOST_CHECK(nSubsidy <= INITIAL_SUBSIDY);
        nSum += nSubsidy;
        BOOST_CHECK(MoneyRange(nSum));
    }
    BOOST_CHECK_EQUAL(nSum, 12500000000000ULL);

    // Regular mining
    CAmount nSubsidy;
    do {
        nSubsidy = GetBlockSubsidy(nHeight, consensusParams);
        BOOST_CHECK(nSubsidy <= INITIAL_SUBSIDY);
        nSum += nSubsidy;
        BOOST_ASSERT(MoneyRange(nSum));
        ++nHeight;
    } while (nSubsidy > 0);

    // Changing the block interval from 10 to 2.5 minutes causes truncation
    // effects to occur earlier (from the 9th halving interval instead of the
    // 11th), decreasing the total monetary supply by 0.0693 ZEC.
    // BOOST_CHECK_EQUAL(nSum, 2099999997690000ULL);
    // Reducing the interval further to 1.25 minutes has a similar effect,
    // decreasing the total monetary supply by another 0.09240 ZEC.
    // BOOST_CHECK_EQUAL(nSum, 2099999990760000ULL);
    BOOST_CHECK_EQUAL(nSum, 2099999981520000LL);
}

bool ReturnFalse() { return false; }
bool ReturnTrue() { return true; }

BOOST_AUTO_TEST_CASE(test_combiner_all)
{
    boost::signals2::signal<bool (), CombinerAll> Test;
    BOOST_CHECK(Test());
    Test.connect(&ReturnFalse);
    BOOST_CHECK(!Test());
    Test.connect(&ReturnTrue);
    BOOST_CHECK(!Test());
    Test.disconnect(&ReturnFalse);
    BOOST_CHECK(Test());
    Test.disconnect(&ReturnTrue);
    BOOST_CHECK(Test());
}

BOOST_AUTO_TEST_SUITE_END()
