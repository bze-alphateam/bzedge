// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017-2018 The SnowGem developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MASTERNODE_SYNC_H
#define MASTERNODE_SYNC_H

#include <atomic>

#define MASTERNODE_SYNC_INITIAL 0
#define MASTERNODE_SYNC_SPORKS 1
#define MASTERNODE_SYNC_LIST 2
#define MASTERNODE_SYNC_MNW 3
#define MASTERNODE_SYNC_BUDGET 4
#define MASTERNODE_SYNC_BUDGET_PROP 10
#define MASTERNODE_SYNC_BUDGET_FIN 11
#define MASTERNODE_SYNC_FAILED 998
#define MASTERNODE_SYNC_FINISHED 999

#define MASTERNODE_SYNC_TIMEOUT 5
#define MASTERNODE_SYNC_THRESHOLD 2

class CMasternodeSync;
extern CMasternodeSync masternodeSync;

//
// CMasternodeSync : Sync masternode assets in stages
//

class CMasternodeSync
{
public:
    std::map<uint256, int> mapSeenSyncMNB;
    std::map<uint256, int> mapSeenSyncMNW;
    std::map<uint256, int> mapSeenSyncBudget;

    int64_t lastMasternodeList;
    int64_t lastMasternodeWinner;
    int64_t lastBudgetItem;
    int64_t lastFailure;
    int nCountFailures;

    std::atomic<int64_t> lastProcess;
    std::atomic<bool> fBlockchainSynced;

    // sum of all counts
    int sumMasternodeList;
    int sumMasternodeWinner;
    int sumBudgetItemProp;
    int sumBudgetItemFin;
    // peers that reported counts
    int countMasternodeList;
    int countMasternodeWinner;
    int countBudgetItemProp;
    int countBudgetItemFin;

    // Count peers we've requested the list from
    int RequestedMasternodeAssets;
    int RequestedMasternodeAttempt;

    // Time when current masternode asset sync started
    int64_t nAssetSyncStarted;

    CMasternodeSync();

    void AddedMasternodeList(const uint256& hash);
    void AddedMasternodeWinner(const uint256& hash);
    void AddedBudgetItem(const uint256& hash);
    void GetNextAsset();
    std::string GetSyncStatus();
    int GetSyncValue() const { return RequestedMasternodeAssets; }
    void ProcessMessage(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);
    bool IsBudgetFinEmpty() const { return sumBudgetItemFin == 0 && countBudgetItemFin > 0; }
    bool IsBudgetPropEmpty() const { return sumBudgetItemProp == 0 && countBudgetItemProp > 0; }

    void Reset();
    void Process();
    bool IsFailed() const { return RequestedMasternodeAssets == MASTERNODE_SYNC_FAILED; }
    bool IsBlockchainSynced();
    bool IsMasternodeListSynced() const { return RequestedMasternodeAssets > MASTERNODE_SYNC_LIST; }
    bool IsWinnersListSynced() const { return RequestedMasternodeAssets > MASTERNODE_SYNC_MNW; }
    bool IsSynced() const { return RequestedMasternodeAssets == MASTERNODE_SYNC_FINISHED; }
    void ClearFulfilledRequest();
};

#endif