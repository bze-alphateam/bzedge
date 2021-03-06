// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php .

#ifndef BITCOIN_CHAINPARAMS_H
#define BITCOIN_CHAINPARAMS_H

#include "chainparamsbase.h"
#include "consensus/params.h"
#include "primitives/block.h"
#include "protocol.h"

#include <vector>

struct CDNSSeedData {
    std::string name, host;
    CDNSSeedData(const std::string &strName, const std::string &strHost) : name(strName), host(strHost) {}
};

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

struct EHparameters {
    unsigned char n;
    unsigned char k;
    unsigned short int nSolSize;
};

//EH sol size = (pow(2, k) * ((n/(k+1))+1)) / 8;
static const EHparameters eh200_9 = {200,9,1344};
static const EHparameters eh144_5 = {144,5,100};
static const EHparameters eh96_5 = {96,5,68};
static const EHparameters eh48_5 = {48,5,36};
static const unsigned int MAX_EH_PARAM_LIST_LEN = 2;

typedef std::map<int, uint256> MapCheckpoints;

struct CCheckpointData {
    MapCheckpoints mapCheckpoints;
    int64_t nTimeLastCheckpoint;
    int64_t nTransactionsLastCheckpoint;
    double fTransactionsPerDay;
};

class CBaseKeyConstants : public KeyConstants {
public:
    const std::vector<unsigned char>& Base58Prefix(Base58Type type) const { return base58Prefixes[type]; }
    const std::string& Bech32HRP(Bech32Type type) const { return bech32HRPs[type]; }

    std::vector<unsigned char> base58Prefixes[KeyConstants::MAX_BASE58_TYPES];
    std::string bech32HRPs[KeyConstants::MAX_BECH32_TYPES];
};

/**
 * CChainParams defines various tweakable parameters of a given instance of the
 * Bitcoin system. There are three: the main network on which people trade goods
 * and services, the public test network which gets reset from time to time and
 * a regression test mode which is intended for private networks only. It has
 * minimal difficulty to ensure that blocks can be found instantly.
 */
class CChainParams: public KeyConstants 
{
public:
    const Consensus::Params& GetConsensus() const { return consensus; }
    const CMessageHeader::MessageStartChars& MessageStart() const { return pchMessageStart; }
    const std::vector<unsigned char>& AlertKey() const { return vAlertPubKey; }
    int GetDefaultPort() const { return nDefaultPort; }

    CAmount SproutValuePoolCheckpointHeight() const { return nSproutValuePoolCheckpointHeight; }
    CAmount SproutValuePoolCheckpointBalance() const { return nSproutValuePoolCheckpointBalance; }
    uint256 SproutValuePoolCheckpointBlockHash() const { return hashSproutValuePoolCheckpointBlock; }
    bool ZIP209Enabled() const { return fZIP209Enabled; }

    const CBlock& GenesisBlock() const { return genesis; }
    /** Make miner wait to have peers to avoid wasting work */
    bool MiningRequiresPeers() const { return fMiningRequiresPeers; }
    /** Default value for -checkmempool and -checkblockindex argument */
    bool DefaultConsistencyChecks() const { return fDefaultConsistencyChecks; }
    /** Policy: Filter transactions that do not match well-defined patterns */
    bool RequireStandard() const { return fRequireStandard; }
    int64_t PruneAfterHeight() const { return nPruneAfterHeight; }
    
    EHparameters eh_epoch_1_params() const { return eh_epoch_1; }
    EHparameters eh_epoch_2_params() const { return eh_epoch_2; }
    unsigned long eh_epoch_1_end() const { return eh_epoch_1_endblock; }
    unsigned long eh_epoch_2_start() const { return eh_epoch_2_startblock; }
    
    uint32_t get_bze_pers_start() const { return bze_pers_start_blocktime; }
        
    unsigned int EquihashN() const { return consensus.nEquihashN; }
    unsigned int EquihashK() const { return consensus.nEquihashK; }
    
    int MasternodeCountDrift() const { return nMasternodeCountDrift; }

    std::string CurrencyUnits() const { return strCurrencyUnits; }
    uint32_t BIP44CoinType() const { return bip44CoinType; }
    /** Make miner stop after a block is found. In RPC, don't return until nGenProcLimit blocks are generated */
    bool MineBlocksOnDemand() const { return fMineBlocksOnDemand; }
    /** In the future use NetworkIDString() for RPC fields */
    bool TestnetToBeDeprecatedFieldRPC() const { return fTestnetToBeDeprecatedFieldRPC; }
    /** Return the BIP70 network string (main, test or regtest) */
    std::string NetworkIDString() const { return strNetworkID; }
    const std::vector<CDNSSeedData>& DNSSeeds() const { return vSeeds; }
    const std::vector<unsigned char>& Base58Prefix(Base58Type type) const { 
        return keyConstants.Base58Prefix(type); 
    }
    const std::string& Bech32HRP(Bech32Type type) const { 
        return keyConstants.Bech32HRP(type); 
    }
    const std::vector<SeedSpec6>& FixedSeeds() const { return vFixedSeeds; }
    const CCheckpointData& Checkpoints() const { return checkpointData; }
    int PoolMaxTransactions() const { return nPoolMaxTransactions; }

    std::string SporkKey() const { return strSporkKey; }
    std::string ObfuscationPoolDummyAddress() const { return strObfuscationPoolDummyAddress; }
    int64_t StartMasternodePayments() const { return nStartMasternodePayments; }
    int64_t Budget_Fee_Confirmations() const { return nBudget_Fee_Confirmations; }

    /** Enforce coinbase consensus rule in regtest mode */
    void SetRegTestCoinbaseMustBeShielded() { consensus.fCoinbaseMustBeShielded = true; }
    int GetNewTimeRule() const { return newTimeRule; }
    int GetMasternodeProtectionBlock() const { return masternodeProtectionBlock; }
    int GetMasternodeCollateral() const { return masternodeCollateral; }
protected:
    CChainParams() {}

    Consensus::Params consensus;
    CMessageHeader::MessageStartChars pchMessageStart;
    //! Raw pub key bytes for the broadcast alert signing key.
    std::vector<unsigned char> vAlertPubKey;
    int nDefaultPort = 0;
    uint64_t nPruneAfterHeight = 0;
    EHparameters eh_epoch_1 = eh200_9;
    EHparameters eh_epoch_2 = eh144_5;
    unsigned long eh_epoch_1_endblock = 150000;
    unsigned long eh_epoch_2_startblock = 140000;
    
    uint32_t bze_pers_start_blocktime;
    
    std::vector<CDNSSeedData> vSeeds;
    CBaseKeyConstants keyConstants;
    std::string strNetworkID;
    std::string strCurrencyUnits;
    uint32_t bip44CoinType;
    CBlock genesis;
    std::vector<SeedSpec6> vFixedSeeds;
    bool fMiningRequiresPeers = false;
    bool fDefaultConsistencyChecks = false;
    bool fRequireStandard = false;
    bool fMineBlocksOnDemand = false;
    bool fTestnetToBeDeprecatedFieldRPC = false;
    int nMasternodeCountDrift;
    int nPoolMaxTransactions;
    std::string strSporkKey;
    std::string strObfuscationPoolDummyAddress;
    int64_t nStartMasternodePayments;
    int64_t nBudget_Fee_Confirmations;
    CCheckpointData checkpointData;
    int newTimeRule = 2000000000; //Will remain at this value if not otherwise defined in chain class.
    int masternodeProtectionBlock;
    int masternodeCollateral;
    CAmount nSproutValuePoolCheckpointHeight = 0;
    CAmount nSproutValuePoolCheckpointBalance = 0;
    uint256 hashSproutValuePoolCheckpointBlock;
    bool fZIP209Enabled = false;
};

/**
 * Return the currently selected parameters. This won't change after app
 * startup, except for unit tests.
 */
const CChainParams &Params();

/**
 * @returns CChainParams for the given BIP70 chain name.
 */
CChainParams& Params(const std::string& chain);

/**
 * Sets the params returned by Params() to those for the given BIP70 chain name.
 * @throws std::runtime_error when the chain is not supported.
 */
void SelectParams(const std::string& chain);

/**
 * Allows modifying the network upgrade regtest parameters.
 */
void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight);

void UpdateRegtestPow(
    int64_t nPowMaxAdjustDown,
    int64_t nPowMaxAdjustUp,
    uint256 powLimit,
    bool noRetargeting);

int validEHparameterList(EHparameters *ehparams, unsigned long blockheight, const CChainParams& params);

#endif // BITCOIN_CHAINPARAMS_H
